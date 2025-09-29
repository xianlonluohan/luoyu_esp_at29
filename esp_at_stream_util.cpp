#include <string>
#include <vector>

#include "MicroBit.h"
#include "MicroBitSerial.h"
#include "pxt.h"

using namespace pxt;

MicroBit* getMicroBit() {
  return &uBit;
}

namespace emakefun {

/**
 * 同时搜索多个目标字符串 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
int16_t multiFindUtil(const String* targets, const uint16_t targets_size, const int32_t timeout_ms) {
  if (targets == NULL || targets_size == 0 || timeout_ms < 0) {
    return -1;
  }
  getMicroBit()->display.scroll("T1:" + (int)targets_size);

  std::vector<std::vector<uint8_t>> byte_targets;
  for (uint16_t i = 0; i < targets_size; i++) {
    std::string target_str(MSTR(targets[i]).toCharArray());
    std::vector<uint8_t> byte_target(target_str.begin(), target_str.end());
    byte_targets.push_back(byte_target);
  }
  getMicroBit()->display.scroll("T2:" + (int)byte_targets.size());

  std::vector<uint16_t> offsets(byte_targets.size(), 0);
  const uint64_t end_time = system_timer_current_time() + timeout_ms;

  do {
    if (getMicroBit()->serial.isReadable()) {
      const uint8_t current_byte = getMicroBit()->serial.getc();

      // 修复：将内层循环变量 i 改为 j，避免变量名冲突
      for (uint8_t j = 0; j < byte_targets.size(); j++) {
        const auto& byte_target = byte_targets[j];
        auto& offset = offsets[j];

        if (current_byte == byte_target[offset]) {
          offset += 1;
          if (offset == byte_target.size()) {
            getMicroBit()->display.scroll("T3:" + (int)j);
            return j;  // 返回正确的索引 j
          }
          continue;
        }

        if (offset == 0) {
          continue;
        }

        const uint16_t original_offset = offset;
        while (offset > 0) {
          offset -= 1;
          if (current_byte != byte_target[offset]) {
            continue;
          }
          if (offset == 0) {
            offset += 1;
            break;
          }
          const uint16_t offset_diff = original_offset - offset;
          uint16_t k = 0;  // 使用 k 避免变量名冲突
          for (k = 0; k < offset; k++) {
            if (byte_target[k] != byte_target[k + offset_diff]) {
              break;
            }
          }
          if (k == offset) {
            offset += 1;
            break;
          }
        }
      }
    }
  } while (system_timer_current_time() < end_time);

  return -1;
}
/**
 * 搜索单个目标字符串 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
bool singleFindUtil(const String target, const int32_t timeout_ms) {
  if (target == NULL || timeout_ms < 0) {
    return false;
  }
  std::string target_str(MSTR(target).toCharArray());
  if (target_str.empty()) {
    return false;
  }

  std::vector<uint8_t> byte_target(target_str.begin(), target_str.end());
  uint16_t offset = 0;
  const uint64_t end_time = system_timer_current_time() + timeout_ms;

  do {
    if (getMicroBit()->serial.isReadable()) {
      const uint8_t current_byte = getMicroBit()->serial.getc();

      if (current_byte == byte_target[offset]) {
        offset += 1;
        if (offset == byte_target.size()) {
          return true;
        }
        continue;
      }

      if (offset == 0) {
        continue;
      }

      const uint16_t original_offset = offset;
      while (offset > 0) {
        offset -= 1;
        if (current_byte != byte_target[offset]) {
          continue;
        }
        if (offset == 0) {
          offset += 1;
          break;
        }
        const uint16_t offset_diff = original_offset - offset;
        uint16_t j = 0;  // 使用 j 避免变量名冲突
        for (j = 0; j < offset; j++) {
          if (byte_target[j] != byte_target[j + offset_diff]) {
            break;
          }
        }
        if (j == offset) {
          offset += 1;
          break;
        }
      }
    }
  } while (system_timer_current_time() < end_time);

  return false;
}

/**
 * 跳过下一个字符 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
bool skipNext(const String target, const int32_t timeout_ms) {
  if (target == NULL || target->getUTF8Size() != 1 || timeout_ms < 0) {
    return false;
  }

  const uint8_t target_byte = MSTR(target).toCharArray()[0];
  const uint64_t end_time = system_timer_current_time() + timeout_ms;

  do {
    if (getMicroBit()->serial.isReadable()) {
      return getMicroBit()->serial.getc() == target_byte;
    }
  } while (system_timer_current_time() < end_time);

  return false;
}

/**
 * 解析数字 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
int32_t parseNumber(const int32_t timeout_ms) {
  if (timeout_ms < 0) {
    return 0;
  }

  bool is_negative = false;
  int32_t value = 0;
  const uint64_t end_time = system_timer_current_time() + timeout_ms;

  do {
    if (getMicroBit()->serial.isReadable()) {
      const uint8_t read_char = getMicroBit()->serial.getc();
      if (read_char == '-' && value == 0) {
        is_negative = true;
      } else if ('0' <= read_char && read_char <= '9') {
        value = value * 10 + (read_char - '0');
      } else {
        if (is_negative) {
          value = -value;
        }
        return value;
      }
    }

  } while (system_timer_current_time() < end_time);

  return 0;
}

/**
 * 读取直到分隔符 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
String readUntil(const String delimiter, const int32_t timeout_ms) {
  if (delimiter == NULL || delimiter->getUTF8Size() != 1 || timeout_ms < 0) {
    return NULL;
  }

  const uint8_t delim_byte = MSTR(delimiter).toCharArray()[0];
  const uint64_t end_time = system_timer_current_time() + timeout_ms;
  std::string result;

  do {
    if (getMicroBit()->serial.isReadable()) {
      const uint8_t current_read_byte = getMicroBit()->serial.getc();

      if (current_read_byte == delim_byte) {
        return mkString(result.c_str(), result.length());
      } else {
        result += static_cast<char>(current_read_byte);
      }
    }
  } while (system_timer_current_time() < end_time);

  return NULL;
}

/**
 * 清空接收缓冲区 - 使用 getMicroBit()->serial 逐个字节读取
 */
//%
void emptyRx(const int32_t timeout_ms) {
  if (timeout_ms < 0) {
    return;
  }
  const uint64_t end_time = system_timer_current_time() + timeout_ms;
  do {
    if (getMicroBit()->serial.isReadable()) {
      getMicroBit()->serial.getc();
    }
  } while (system_timer_current_time() < end_time);
}
}  // namespace emakefun