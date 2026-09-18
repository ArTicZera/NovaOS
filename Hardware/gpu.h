#define INTEL 0x8086
#define AMD 0x1002
#define NVIDIA 0x10DE

void GetGPUVendor(char *vendor);
void GetGPUModel(char *model);
void GetGPUName(char *name);
//void GetGPUVideoRAM();
// Pls, make VRAM detect

void ShowGPUInfo();
void ShowGPUName();

void DetectGPU();
