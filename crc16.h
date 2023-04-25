#ifndef __CRC16_H
#define __CRC16_H

#include <stdint.h>
#include <stdio.h>

uint16_t crc_16( const uint8_t *input_str, size_t num_bytes );

#endif /* __CRC16_H */