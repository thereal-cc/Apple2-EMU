#include "disk.h"
#include "utils/tinyfiledialogs.h"

disk_t load_disk()
{
    // Init Blank Disk
    disk_t disk = {0};

    // Choose Rom
    const char *filters[] = { "*.dsk", "*.nib", "*.woz" };
    const char *disk_path = tinyfd_openFileDialog("Load Disk", "", 1, filters, "Choose Disk Image", 0);
    if (disk_path == NULL) {
        fprintf(stderr, "Error: No image selected or invalid path\n");
        disk.loaded = false;
        return disk;
    }

    // Check File Extension, set format based on it
    const char *ext = strrchr(disk_path, '.');
    if (ext && strcmp(ext, "dsk") == 0) disk.format = DSK;
    else if (ext && strcmp(ext, "nib") == 0) disk.format = NIB;
    else if (ext && strcmp(ext, "woz") == 0) disk.format = WOZ;

    FILE *f = fopen(disk_path, "rb");
    if (!f) {
        fprintf(stderr, "Error: Could not open disk image %s\n", disk_path);
        disk.loaded = false;
        return disk;
    }

    size_t bytes_read = fread(disk.dsk_data, 1, TRACKS * SECTORS * BYTES, f);
    fclose(f);

    if (bytes_read != TRACKS * SECTORS * BYTES) {
        fprintf(stderr, "Error: Disk image wrong size (got %zu, expected %d)\n",
                bytes_read, TRACKS * SECTORS * BYTES);
        disk.loaded = false;
        return disk;
    }

    disk.current_track = 0;
    disk.current_sector = 0;
    disk.current_byte = 0;
    disk.loaded = true;
    disk.write_mode = false;
    
    return disk;
}

bool save_disk(disk_t *disk, const char* disk_path)
{
    return true;
}

u8 read_disk_register(disk_t *disk)
{
    u8 byte = 0;

    switch (disk->format) {
        case DSK:
            break;
        case NIB:
            break;
        case WOZ:
            break;
    }

    disk->current_byte++;
    if (disk->current_byte == BYTES - 1)
    {
        disk->current_byte = 0;
        disk->current_sector = (disk->current_sector + 1) % SECTORS;
    }

    return byte;
}