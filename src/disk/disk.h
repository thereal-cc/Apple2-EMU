#ifndef DISK_H
#define DISK_H

#include "utils/util.h"

#define TRACKS 35
#define SECTORS 16
#define BYTES 256

enum DISK_EXT
{
    DSK,
    NIB,
    WOZ
};

typedef struct
{
    u8 dsk_data[TRACKS][SECTORS][BYTES]; // Raw Sector Data
    u8 current_track;
    u8 current_sector;
    u8 current_byte;
    u8 format;
    bool loaded;
    bool write_mode;
} disk_t;

disk_t load_disk();
bool save_disk(disk_t *disk, const char* disk_path);
u8 read_disk_register(disk_t *disk);
u8 decode_62(u8 data[TRACKS][SECTORS][BYTES]);


#endif