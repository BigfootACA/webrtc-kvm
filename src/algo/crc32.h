#ifndef CRC32_H
#define CRC32_H
#include<cstdint>
#include<cstddef>
extern uint32_t crc32(uint32_t seed,const uint8_t*buf,size_t len);
extern uint32_t crc32_exclude_offset(uint32_t seed,const uint8_t*buf,size_t len,size_t exclude_off,size_t exclude_len);
#endif