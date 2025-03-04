// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: effective_msg.proto
// Protobuf C++ Version: 5.29.3

#include "effective_msg.pb.h"

#include <algorithm>
#include <type_traits>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;

inline constexpr EffectiveMsg::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : level_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        file_name_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        func_name_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        log_info_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        timestamp_{::int64_t{0}},
        pid_{0},
        tid_{0},
        line_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR EffectiveMsg::EffectiveMsg(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::MessageLite(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::MessageLite(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct EffectiveMsgDefaultTypeInternal {
  PROTOBUF_CONSTEXPR EffectiveMsgDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~EffectiveMsgDefaultTypeInternal() {}
  union {
    EffectiveMsg _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 EffectiveMsgDefaultTypeInternal _EffectiveMsg_default_instance_;
// ===================================================================

class EffectiveMsg::_Internal {
 public:
};

EffectiveMsg::EffectiveMsg(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::MessageLite(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::MessageLite(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:EffectiveMsg)
}
inline PROTOBUF_NDEBUG_INLINE EffectiveMsg::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::EffectiveMsg& from_msg)
      : level_(arena, from.level_),
        file_name_(arena, from.file_name_),
        func_name_(arena, from.func_name_),
        log_info_(arena, from.log_info_),
        _cached_size_{0} {}

EffectiveMsg::EffectiveMsg(
    ::google::protobuf::Arena* arena,
    const EffectiveMsg& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::MessageLite(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::MessageLite(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  EffectiveMsg* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<std::string>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  ::memcpy(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, timestamp_),
           reinterpret_cast<const char *>(&from._impl_) +
               offsetof(Impl_, timestamp_),
           offsetof(Impl_, line_) -
               offsetof(Impl_, timestamp_) +
               sizeof(Impl_::line_));

  // @@protoc_insertion_point(copy_constructor:EffectiveMsg)
}
inline PROTOBUF_NDEBUG_INLINE EffectiveMsg::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : level_(arena),
        file_name_(arena),
        func_name_(arena),
        log_info_(arena),
        _cached_size_{0} {}

inline void EffectiveMsg::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, timestamp_),
           0,
           offsetof(Impl_, line_) -
               offsetof(Impl_, timestamp_) +
               sizeof(Impl_::line_));
}
EffectiveMsg::~EffectiveMsg() {
  // @@protoc_insertion_point(destructor:EffectiveMsg)
  SharedDtor(*this);
}
inline void EffectiveMsg::SharedDtor(MessageLite& self) {
  EffectiveMsg& this_ = static_cast<EffectiveMsg&>(self);
  this_._internal_metadata_.Delete<std::string>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.level_.Destroy();
  this_._impl_.file_name_.Destroy();
  this_._impl_.func_name_.Destroy();
  this_._impl_.log_info_.Destroy();
  this_._impl_.~Impl_();
}

inline void* EffectiveMsg::PlacementNew_(const void*, void* mem,
                                        ::google::protobuf::Arena* arena) {
  return ::new (mem) EffectiveMsg(arena);
}
constexpr auto EffectiveMsg::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(EffectiveMsg),
                                            alignof(EffectiveMsg));
}
PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::internal::ClassDataLite<13> EffectiveMsg::_class_data_ = {
    {
        &_EffectiveMsg_default_instance_._instance,
        &_table_.header,
        nullptr,  // OnDemandRegisterArenaDtor
        nullptr,  // IsInitialized
        &EffectiveMsg::MergeImpl,
        ::google::protobuf::MessageLite::GetNewImpl<EffectiveMsg>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
        &EffectiveMsg::SharedDtor,
        ::google::protobuf::MessageLite::GetClearImpl<EffectiveMsg>(), &EffectiveMsg::ByteSizeLong,
            &EffectiveMsg::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
        PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_._cached_size_),
        true,
    },
    "EffectiveMsg",
};
const ::google::protobuf::internal::ClassData* EffectiveMsg::GetClassData() const {
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<3, 8, 0, 60, 2> EffectiveMsg::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    8, 56,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967040,  // skipmap
    offsetof(decltype(_table_), field_entries),
    8,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallbackLite,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::EffectiveMsg>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string log_info = 8;
    {::_pbi::TcParser::FastUS1,
     {66, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.log_info_)}},
    // string level = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.level_)}},
    // int64 timestamp = 2;
    {::_pbi::TcParser::FastV64S1,
     {16, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.timestamp_)}},
    // int32 pid = 3;
    {::_pbi::TcParser::FastV32S1,
     {24, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.pid_)}},
    // int32 tid = 4;
    {::_pbi::TcParser::FastV32S1,
     {32, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.tid_)}},
    // int32 line = 5;
    {::_pbi::TcParser::FastV32S1,
     {40, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.line_)}},
    // string file_name = 6;
    {::_pbi::TcParser::FastUS1,
     {50, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.file_name_)}},
    // string func_name = 7;
    {::_pbi::TcParser::FastUS1,
     {58, 63, 0, PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.func_name_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string level = 1;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.level_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // int64 timestamp = 2;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.timestamp_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt64)},
    // int32 pid = 3;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.pid_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // int32 tid = 4;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.tid_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // int32 line = 5;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.line_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // string file_name = 6;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.file_name_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string func_name = 7;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.func_name_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string log_info = 8;
    {PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.log_info_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\14\5\0\0\0\0\11\11\10\0\0\0\0\0\0\0"
    "EffectiveMsg"
    "level"
    "file_name"
    "func_name"
    "log_info"
  }},
};

PROTOBUF_NOINLINE void EffectiveMsg::Clear() {
// @@protoc_insertion_point(message_clear_start:EffectiveMsg)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.level_.ClearToEmpty();
  _impl_.file_name_.ClearToEmpty();
  _impl_.func_name_.ClearToEmpty();
  _impl_.log_info_.ClearToEmpty();
  ::memset(&_impl_.timestamp_, 0, static_cast<::size_t>(
      reinterpret_cast<char*>(&_impl_.line_) -
      reinterpret_cast<char*>(&_impl_.timestamp_)) + sizeof(_impl_.line_));
  _internal_metadata_.Clear<std::string>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* EffectiveMsg::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const EffectiveMsg& this_ = static_cast<const EffectiveMsg&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* EffectiveMsg::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const EffectiveMsg& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:EffectiveMsg)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          // string level = 1;
          if (!this_._internal_level().empty()) {
            const std::string& _s = this_._internal_level();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "EffectiveMsg.level");
            target = stream->WriteStringMaybeAliased(1, _s, target);
          }

          // int64 timestamp = 2;
          if (this_._internal_timestamp() != 0) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt64ToArrayWithField<2>(
                    stream, this_._internal_timestamp(), target);
          }

          // int32 pid = 3;
          if (this_._internal_pid() != 0) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<3>(
                    stream, this_._internal_pid(), target);
          }

          // int32 tid = 4;
          if (this_._internal_tid() != 0) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<4>(
                    stream, this_._internal_tid(), target);
          }

          // int32 line = 5;
          if (this_._internal_line() != 0) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<5>(
                    stream, this_._internal_line(), target);
          }

          // string file_name = 6;
          if (!this_._internal_file_name().empty()) {
            const std::string& _s = this_._internal_file_name();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "EffectiveMsg.file_name");
            target = stream->WriteStringMaybeAliased(6, _s, target);
          }

          // string func_name = 7;
          if (!this_._internal_func_name().empty()) {
            const std::string& _s = this_._internal_func_name();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "EffectiveMsg.func_name");
            target = stream->WriteStringMaybeAliased(7, _s, target);
          }

          // string log_info = 8;
          if (!this_._internal_log_info().empty()) {
            const std::string& _s = this_._internal_log_info();
            ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
                _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "EffectiveMsg.log_info");
            target = stream->WriteStringMaybeAliased(8, _s, target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target = stream->WriteRaw(
                this_._internal_metadata_.unknown_fields<std::string>(::google::protobuf::internal::GetEmptyString).data(),
                static_cast<int>(this_._internal_metadata_.unknown_fields<std::string>(::google::protobuf::internal::GetEmptyString).size()), target);
          }
          // @@protoc_insertion_point(serialize_to_array_end:EffectiveMsg)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t EffectiveMsg::ByteSizeLong(const MessageLite& base) {
          const EffectiveMsg& this_ = static_cast<const EffectiveMsg&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t EffectiveMsg::ByteSizeLong() const {
          const EffectiveMsg& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:EffectiveMsg)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
           {
            // string level = 1;
            if (!this_._internal_level().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_level());
            }
            // string file_name = 6;
            if (!this_._internal_file_name().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_file_name());
            }
            // string func_name = 7;
            if (!this_._internal_func_name().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_func_name());
            }
            // string log_info = 8;
            if (!this_._internal_log_info().empty()) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_log_info());
            }
            // int64 timestamp = 2;
            if (this_._internal_timestamp() != 0) {
              total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(
                  this_._internal_timestamp());
            }
            // int32 pid = 3;
            if (this_._internal_pid() != 0) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_pid());
            }
            // int32 tid = 4;
            if (this_._internal_tid() != 0) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_tid());
            }
            // int32 line = 5;
            if (this_._internal_line() != 0) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_line());
            }
          }
          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            total_size += this_._internal_metadata_.unknown_fields<std::string>(::google::protobuf::internal::GetEmptyString).size();
          }
          this_._impl_._cached_size_.Set(::_pbi::ToCachedSize(total_size));
          return total_size;
        }

void EffectiveMsg::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<EffectiveMsg*>(&to_msg);
  auto& from = static_cast<const EffectiveMsg&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:EffectiveMsg)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_level().empty()) {
    _this->_internal_set_level(from._internal_level());
  }
  if (!from._internal_file_name().empty()) {
    _this->_internal_set_file_name(from._internal_file_name());
  }
  if (!from._internal_func_name().empty()) {
    _this->_internal_set_func_name(from._internal_func_name());
  }
  if (!from._internal_log_info().empty()) {
    _this->_internal_set_log_info(from._internal_log_info());
  }
  if (from._internal_timestamp() != 0) {
    _this->_impl_.timestamp_ = from._impl_.timestamp_;
  }
  if (from._internal_pid() != 0) {
    _this->_impl_.pid_ = from._impl_.pid_;
  }
  if (from._internal_tid() != 0) {
    _this->_impl_.tid_ = from._impl_.tid_;
  }
  if (from._internal_line() != 0) {
    _this->_impl_.line_ = from._impl_.line_;
  }
  _this->_internal_metadata_.MergeFrom<std::string>(from._internal_metadata_);
}

void EffectiveMsg::CopyFrom(const EffectiveMsg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:EffectiveMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void EffectiveMsg::InternalSwap(EffectiveMsg* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.level_, &other->_impl_.level_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.file_name_, &other->_impl_.file_name_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.func_name_, &other->_impl_.func_name_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.log_info_, &other->_impl_.log_info_, arena);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.line_)
      + sizeof(EffectiveMsg::_impl_.line_)
      - PROTOBUF_FIELD_OFFSET(EffectiveMsg, _impl_.timestamp_)>(
          reinterpret_cast<char*>(&_impl_.timestamp_),
          reinterpret_cast<char*>(&other->_impl_.timestamp_));
}

// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
