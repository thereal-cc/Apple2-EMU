#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;

// CPU Defines
#define MEMORY_SIZE 0x10000

#define NMI_LOW_ADDR 0xFFFA
#define NMI_HIGH_ADDR 0xFFFB
#define RESET_LOW_ADDR 0xFFFC
#define RESET_HIGH_ADDR 0xFFFD
#define BRK_LOW_ADDR 0xFFFE
#define BRK_HIGH_ADDR 0xFFFF

// Status flag masks
#define UNUSED_FLAG   0x00
#define CARRY_FLAG    0x01
#define ZERO_FLAG     0x02
#define INTERRUPT_FLAG 0x04
#define DECIMAL_FLAG  0x08
#define BREAK_FLAG    0x10
#define OVERFLOW_FLAG 0x40
#define NEGATIVE_FLAG 0x80

#endif