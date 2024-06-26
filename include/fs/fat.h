/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef FAT12_H
#define FAT12_H

#include <lib/string.h>
#include <types.h>
#include <fs/vfs.h>
#include <hal/device.h>

#define FAT12       0
#define FAT16       1
#define FAT32       2
#define EXFAT       3

// 33 bytes
typedef struct {
    uint8_t oem_name[8];
    uint16_t sector_bytes;
    uint8_t cluster_sectors;
    uint16_t reserved_sectors;
    uint8_t n_fats;
    uint16_t n_dir_entries;
    uint16_t n_sectors;
    uint8_t media;
    uint16_t fat_sectors;
    uint16_t track_sectors;
    uint16_t cylinder_heads;
    uint32_t hidden_sectors;
    uint32_t long_sectors;
} __attribute((__packed__)) bios_parameter_block_t;

// 28 bytes
typedef struct {
    uint32_t fat_sectors;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t info_cluster;
    uint16_t backup_boot;
    uint16_t reserved[6];
} __attribute__((__packed__)) bios_parameter_block_ext_t;

// 512 bytes
typedef struct {
    uint8_t ignore[3];
    bios_parameter_block_t bpb;
    bios_parameter_block_ext_t bpb_ext;
    uint8_t fill[448];
} __attribute__((__packed__)) bootsector_t;

#define DIR_RO          1
#define DIR_HIDDEN      2
#define DIR_SYS         4
#define DIR_VOL_LABEL   8
#define DIR_SUBDIR      0x10
#define DIR_ARCHIVE     0x20
#define DIR_DEVICE      0x60

typedef struct directory {
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t attrs;
    uint8_t reserved;
    uint8_t time_created_ms;
    uint16_t time_created;
    uint16_t date_created;
    uint16_t date_last_accessed;
    uint16_t first_cluster_high_bytes;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t first_cluster;
    uint32_t file_size;
} __attribute__((__packed__)) directory_t;

#define NAME_LEN 11

void fat_mount(device_t *dev);
void to_dos_file_name(char *name, char *str);
void to_normal_file_name(char *name, char *str);
uint32_t get_phys_sector(file *f);
directory_t *fat_get_dir(file *f);
int fat_touch(char *name);
void fat_read(file *f, char *buf);
void fat_write(file *f, char *str);
int fat_delete(char *name);
void fat_close(file *f);
file fat_directory(char *dir_name, int devid);
file fat_open_subdir(file directory, char *name);
file fat_open(char *name);
file fat_cd(char *dir);
file fat_search(char *name);
void fat_ls(char *dir);
void fat_init(filesystem *fs_fat);

#endif