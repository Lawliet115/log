#include "mmap/mmap.h"

#include <windows.h>

namespace logger {

class FileHandleDeleter {
 public:
  void operator()(HANDLE h) {
    if (h != NULL)
      CloseHandle(h);
  }
};

using FileHandlePtr = std::unique_ptr<std::remove_pointer_t<HANDLE>, FileHandleDeleter>;

bool MMap::TryMap_(size_t capacity) {
  FileHandlePtr file_handle(CreateFileW(file_path_.wstring().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
  if (file_handle.get() == INVALID_HANDLE_VALUE) {
    return false;
  }
  FileHandlePtr file_mapping_handle;
  file_mapping_handle.reset(CreateFileMapping(file_handle.get(), NULL, PAGE_READWRITE, 0, capacity, NULL));
  if (!file_mapping_handle.get()) {
    return false;
  }

  handle_ = MapViewOfFile(file_mapping_handle.get(), FILE_MAP_ALL_ACCESS, 0, 0, capacity);
  return handle_ != NULL;
}

void MMap::Unmap_() {
  if (handle_) {
    UnmapViewOfFile(handle_);
  }
  handle_ = NULL;
}

void MMap::Sync_() {
  if (handle_) {
    FlushViewOfFile(handle_, capacity_);
  }
}

}  // namespace logger
