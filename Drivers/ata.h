#define ATA_DATA       0x1F0
#define ATA_ERROR      0x1F1
#define ATA_FEATURES   0x1F1
#define ATA_SECCOUNT   0x1F2
#define ATA_LBA_LOW    0x1F3
#define ATA_LBA_MID    0x1F4
#define ATA_LBA_HIGH   0x1F5
#define ATA_DRIVE      0x1F6
#define ATA_COMMAND    0x1F7
#define ATA_STATUS     0x1F7
#define ATA_ALT_STATUS 0x3F6
#define ATA_CMD_PACKET 0xA0
#define ATAPI_CMD_READ 0xA8

#define DATA 0
#define ERROR_R 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7
#define CONTROL 0x206
#define ALTERNATE_STATUS 0

void LBA28_read_sector(BYTE drive, DWORD LBA, DWORD sector, LPWORD addr);
void LBA28_write_sector(BYTE drive, DWORD LBA, DWORD sector, LPWORD buffer);

int read_cdrom(WORD port, BYTE slave, DWORD lba, DWORD sectors, LPWORD buffer);