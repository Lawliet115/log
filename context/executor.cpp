#include "context/executor.h"

namespace logger {
namespace ctx {

Executor::Executor() {
  // 初始化 executor_context_ 和 executor_timer_
  executor_context_ = std::make_unique<ExecutorContext>();
  executor_timer_ = std::make_unique<ExecutorTimer>();
}

Executor::~Executor() {
  // 释放 executor_context_ 和 executor_timer_ 的资源
  executor_context_.reset();
  executor_timer_.reset();
}

// 向任务运行器添加任务并返回对应的 TaskRunnerTag
TaskRunnerTag Executor::AddTaskRunner(const TaskRunnerTag& tag) {
  return executor_context_->AddTaskRunner(tag);
}

// 将任务提交给指定的 TaskRunner 运行
void Executor::PostTask(const TaskRunnerTag& runner_tag, Task task) {
  ExecutorContext::TaskRunner* task_runner = executor_context_->GetTaskRunner(runner_tag);
  task_runner->RunTask(std::move(task));
}

// ExecutorContext 类的 AddTaskRunner 实现
TaskRunnerTag Executor::ExecutorContext::AddTaskRunner(const TaskRunnerTag& tag) {
  // 加锁保证线程安全
  std::lock_guard<std::mutex> lock(mutex_);
  TaskRunnerTag latest_tag = tag;

  // 检查该标签是否已经存在，如果存在则生成新的标签
  while (task_runner_dict_.find(latest_tag) != task_runner_dict_.end()) {
    latest_tag = GetNextRunnerTag();
  }

  // 创建一个新的线程池并启动
  TaskRunnerPtr runner = std::make_unique<ThreadPool>(1);
  runner->Start();

  // 将新建的 TaskRunner 存入字典中
  task_runner_dict_.emplace(latest_tag, std::move(runner));

  return latest_tag;
}

// 获取下一个唯一的 TaskRunnerTag
TaskRunnerTag Executor::ExecutorContext::GetNextRunnerTag() {
  static uint64_t index = 0;
  ++index;
  return index;
}

// 获取指定标签对应的 TaskRunner
Executor::ExecutorContext::TaskRunner* Executor::ExecutorContext::GetTaskRunner(const TaskRunnerTag& tag) {
  // 加锁保证线程安全
  std::lock_guard<std::mutex> lock(mutex_);
  if (task_runner_dict_.find(tag) == task_runner_dict_.end()) {
    return nullptr;  // 如果找不到该标签的 TaskRunner，返回 nullptr
  }
  return task_runner_dict_[tag].get();
}

// ExecutorTimer 类的构造函数
Executor::ExecutorTimer::ExecutorTimer() {
  // 初始化线程池和状态标志
  thread_pool_ = std::make_unique<ThreadPool>(1);
  repeated_task_id_.store(0);
  running_.store(false);
}

// ExecutorTimer 类的析构函数
Executor::ExecutorTimer::~ExecutorTimer() {
  Stop();  // 停止定时器
}

// 启动定时器
bool Executor::ExecutorTimer::Start() {
  if (running_) {
    return true;  // 如果定时器已经在运行，直接返回
  }
  running_.store(true);
  bool ret = thread_pool_->Start();
  // 在线程池中运行定时器任务
  thread_pool_->RunTask(&Executor::ExecutorTimer::Run_, this);
  return ret;
}

// 停止定时器
void Executor::ExecutorTimer::Stop() {
  running_.store(false);
  cond_.notify_all();  // 通知可能在等待的任务
  thread_pool_.reset();  // 释放线程池
}

// 定时器的运行任务，循环处理定时任务队列
void Executor::ExecutorTimer::Run_() {
  while (running_.load()) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      cond_.wait(lock);  // 如果队列为空，等待任务
      continue;
    }
    auto s = queue_.top();
    auto diff = s.time_point - std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::microseconds>(diff).count() > 0) {
      cond_.wait_for(lock, diff);  // 等待直到下一个任务时间
      continue;
    } else {
      queue_.pop();  // 执行任务并从队列中移除
      lock.unlock();
      s.task();
    }
  }
}

// 向定时器添加延迟任务
void Executor::ExecutorTimer::PostDelayedTask(Task task, const std::chrono::microseconds& delta) {
  InternalS s;
  s.time_point = std::chrono::high_resolution_clock::now() + delta;  // 任务执行的时间点
  s.task = std::move(task);  // 移动任务

  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(s);  // 将任务添加到队列
    cond_.notify_all();  // 通知定时器
  }
}

// 添加重复任务并返回任务ID
RepeatedTaskId Executor::ExecutorTimer::PostRepeatedTask(Task task,
                                                         const std::chrono::microseconds& delta,
                                                         uint64_t repeat_num) {
  RepeatedTaskId id = GetNextRepeatedTaskId();
  repeated_id_state_set_.insert(id);  // 将任务ID加入集合
  PostRepeatedTask_(std::move(task), delta, id, repeat_num);  // 添加重复任务
  return id;
}

// 取消指定ID的重复任务
void Executor::ExecutorTimer::CancelRepeatedTask(RepeatedTaskId task_id) {
  repeated_id_state_set_.erase(task_id);  // 从集合中移除任务ID
}

void Executor::ExecutorTimer::PostTask_(Task task,
  std::chrono::microseconds delta,
  RepeatedTaskId repeated_task_id,
  uint64_t repeat_num) {
PostRepeatedTask_(std::move(task), delta, repeated_task_id, repeat_num);
}

// 内部方法：处理重复任务
void Executor::ExecutorTimer::PostRepeatedTask_(Task task,
                                                const std::chrono::microseconds& delta,
                                                RepeatedTaskId repeated_task_id,
                                                uint64_t repeat_num) {
  if (repeated_id_state_set_.find(repeated_task_id) == repeated_id_state_set_.end() || repeat_num == 0) {
    return;  // 如果任务ID不存在或重复次数为0，返回
  }

  task();  // 执行当前任务

  // 准备下一次重复任务
  Task func =
      std::bind(&Executor::ExecutorTimer::PostTask_, this, std::move(task), delta, repeated_task_id, repeat_num - 1);
  InternalS s;
  s.time_point = std::chrono::high_resolution_clock::now() + delta;  // 下一次执行时间点
  s.repeated_id = repeated_task_id;
  s.task = std::move(func);

  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(s);  // 将重复任务添加到队列
    lock.unlock();
    cond_.notify_all();  // 通知定时器
  }
}

}  // namespace ctx
}  // namespace logger
