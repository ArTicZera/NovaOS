struct multiboot_aout_symbol_table
{
  DWORD tabsize;
  DWORD strsize;
  DWORD addr;
  DWORD reserved;
};

struct multiboot_elf_section_header_table
{
  DWORD num;
  DWORD size;
  DWORD addr;
  DWORD shndx;
};

struct multiboot_module_t
{
    DWORD mod_start;
    DWORD mod_end;
    DWORD string;
    DWORD reserved;
} __attribute__((packed));

struct vbe_mode_info_t 
{
    WORD attributes;
    BYTE winA, winB;
    WORD granularity;
    WORD winsize;
    WORD segmentA, segmentB;
    DWORD realFctPtr;
    WORD pitch;

    WORD Xres, Yres;
    BYTE Wchar, Ychar, planes, bpp, banks;
    BYTE memory_model, bank_size, image_pages;
    BYTE reserved0;

    BYTE red_mask, red_position;
    BYTE green_mask, green_position;
    BYTE blue_mask, blue_position;
    BYTE reserved_mask, reserved_position;
    BYTE directcolor_attributes;

    DWORD framebuffer; // 🔥 AQUI!!!
    DWORD offscreen_mem_off;
    WORD offscreen_mem_size;

    BYTE reserved1[206];
} __attribute__((packed));

struct multiboot_info{
    DWORD flags;
    DWORD mem_lower;
    DWORD mem_upper;
    DWORD boot_device;

    DWORD cmdline;
    DWORD mods_count;
    DWORD mods_addr;

    union{
        struct multiboot_aout_symbol_table aout_sym;
        struct multiboot_elf_section_header_table elf_sec;
    } u;

    DWORD mmap_length;
    DWORD mmap_addr;

    DWORD drives_length;
    DWORD drives_addr;

    DWORD config_table;
    DWORD boot_loader_name;

    DWORD apm_table;

    DWORD vbe_control_info;
    DWORD vbe_mode_info;
    WORD vbe_mode;
    WORD vbe_interface_seg;
    WORD vbe_interface_off;
    WORD vbe_interface_len;
} __attribute__((packed));

struct multiboot_mmap_entry
{
  DWORD size;
  DWORD addr_low;
  DWORD addr_high;
  DWORD len_low;
  DWORD len_high;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  DWORD type;
} __attribute__((packed));