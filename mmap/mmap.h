#pragma once

#include <filesystem>
#include <memory>


namespace logger {

class MMap {
 public:
 void Exit_();

  using fpath = std::filesystem::path;

  explicit MMap(fpath file_path);

  ~MMap();

  MMap(const MMap&) = delete;

  MMap& operator=(const MMap&) = delete;

  void Resize(size_t new_size);

  uint8_t* Data() const;

  size_t Size() const;

  size_t Capacity_() const noexcept { return capacity_; }

  void Clear();

  void Push(const void* data, size_t size);

  double GetRatio() const;

  bool Empty() const { return Size() == 0; }

 private:
  struct MmapHeader {
    static constexpr uint32_t kMagic = 0xdeadbeef;
    uint32_t magic = kMagic;
    uint32_t size;
  };

 private:
  void Reserve_(size_t new_size);

  void EnsureCapacity_(size_t new_size);


  bool TryMap_(size_t capacity);

  void Unmap_();

  void Sync_();

  bool IsValid_() const;

  MmapHeader* Header_() const;

  void Init_();

  

 private:
  fpath file_path_;
  void* handle_;
  size_t capacity_;
};

}  // namespace logger
