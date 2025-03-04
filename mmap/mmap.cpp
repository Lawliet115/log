#include "mmap/mmap.h"

#include <string.h>

#include "log_common.h"
#include "utils/file_util.h"
#include "utils/sys_util.h"

namespace logger {
static constexpr size_t kDefaultCapacity = 1024;  // 1KB

MMap::MMap(fpath file_path) : file_path_(std::move(file_path)), handle_(nullptr), capacity_(0) {
    size_t file_size = fs::GetFileSize(file_path_);
    size_t real_size = std::max(file_size, kDefaultCapacity);
    Reserve_(real_size);
    Init_();
}

void MMap::Reserve_(size_t new_size) {
    if(new_size <= capacity_) {
        return;
    }
    size_t page_size = logger::GetPageSize();
    new_size = ((new_size + page_size - 1) / page_size) * page_size;
    if(new_size == capacity_) {
        return;
    }
    Unmap_();
    TryMap_(new_size);
    capacity_ = new_size;
}

void MMap::Init_() {
    MmapHeader* header = Header_();
    if(!header){
      return;
    }
    //如果魔数不是默认的代表没有映射过,需要初始化MmapHeader
    if(header->magic != MmapHeader::kMagic){
        header->magic = MmapHeader::kMagic;
        header->size = 0;
    }
}

void MMap::Exit_() {
    Unmap_();
}

MMap::MmapHeader* MMap::Header_() const {
    if(!handle_) {
        return nullptr;
    }
    //容量不足存储头部结构体
    if(capacity_ < sizeof(MmapHeader)) {
        return nullptr;
    }
    return static_cast<MmapHeader*>(handle_);
}

//返回实际数据的地址跳过头
uint8_t* MMap::Data() const {
    if(!IsValid_()) {
        return nullptr;
    }
    return static_cast<uint8_t*>(handle_) + sizeof(MmapHeader);
}

bool MMap::IsValid_() const {
    MmapHeader* header = Header_();
    if (!header) {
      return false;
    }
    ///如果魔数不是默认的代表没有映射过
    return header->magic == MmapHeader::kMagic;
}

size_t MMap::Size() const { 
    if (!IsValid_()) {
        return 0;
    }
    return Header_()->size;
}

void MMap::Clear() {
    if (!IsValid_()) {
       return ;
    }
    Header_()->size = 0;
}

void MMap::Resize(size_t new_size) {
    if (!IsValid_()) {
        return ;
    }
    EnsureCapacity_(new_size);
    Header_()->size = new_size;
}

//是否扩容
void MMap::EnsureCapacity_(size_t new_size) {
    size_t real_size = new_size + sizeof(MmapHeader);
    if(real_size <= capacity_) {
        return ;
    }
    auto capacity = capacity_;
    while(capacity < real_size) {
        capacity += logger::GetPageSize();
    }
    Reserve_(capacity);
}

void MMap::Push(const void* data, size_t size) {
    if (!IsValid_()) {
        return ;
    }
    size_t new_size = size + Size();
    EnsureCapacity_(new_size);
    memcpy(Data() + Size(), data, size);
    Header_()->size = new_size;
}

double MMap::GetRatio() const{
    if(!IsValid_()){
      return 0.0;
    }
    return static_cast<double>(Size()) / Capacity_() - sizeof(MmapHeader);
  }

  MMap::~MMap() {
    Unmap_();
  };
  

}  
