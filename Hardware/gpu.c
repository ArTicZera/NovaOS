#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/printf.h" // Nicolas extension :D
#include "../Font/text.h"

#include "pci.h"
#include "gpu.h"

WORD GPUVendorID = 0;
WORD GPUDeviceID = 0;

static char GPUVendorName[32] = {0};
static char GPUDeviceName[64] = {0};

void GetGPUVendor(char *vendor)
{
    if (GPUVendorID == NVIDIA) strcpy(vendor, "NVIDIA");
    else if (GPUVendorID == AMD) strcpy(vendor, "AMD/ATI");
    else if (GPUVendorID == INTEL) strcpy(vendor, "Intel");
    else strcpy(vendor, "Unknown");
    strcpy(g_vendor_name, vendor);
}

void GetGPUModel(char *model)
{
    sprintf_(model, "0x%04X", GPUDeviceID);
}

void GetGPUName(char *name)
{
    sprintf_(name, "%s GPU (Device %04X)", GPUVendorName, GPUDeviceID);
    strcpy(GPUDeviceName, name);
}

void ShowGPUInfo()
{
    Print("Vendor: %s\n", 0xFFFFFFFF);
    Print(GPUVendorName, 0xFFFFFFFF);
    Print("\n", 0x00);
    Print("Device ID: 0x%04X\n", 0xFFFFFFFF);
    Print(GPUDeviceID, 0xFFFFFFFF);
    Print("\n", 0x00;
    Print("Name: %s\n", 0xFFFFFFFF);
    Print(GPUDeviceName, 0xFFFFFFFF);
    Print("\n", 0x00);
}

void ShowGPUName()
{
    Print(GPUDeviceName, 0xFFFFFFFF);
}

void DetectGPU()
{
    for (uint8_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            DWORD VendorDevice = PCIConfigReadWord(bus, slot, 0, 0x00);
            WORD VendorID = VendorDevice & 0xFFFF;
            WORD DeviceID = (VendorDevice >> 16) & 0xFFFF;

            if (vendor_id == 0xFFFF) continue;

            uint32_t class_code_reg = PCIConfigReadWord(bus, slot, 0, 0x08);
            uint8_t class_code = (class_code_reg >> 24) & 0xFF;

            if (class_code == 0x03)
            {
                GPUVendorID = VendorID;
                GPUDeviceID = DeviceID;
                return;
            }
        }
    }
}
