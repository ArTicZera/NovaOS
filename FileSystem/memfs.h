#define MAXFILENAME 0x0F
#define MAXFILES    0x0A
#define MAXSUBDIR   0x05
#define FSADDRESS   0x2000

typedef struct Directory Directory;

typedef struct
{
    char filename[MAXFILENAME];
    DWORD size;
    DWORD dataOffset;
} FileHeader;

struct Directory
{
    char name[MAXFILENAME];
    FileHeader files[MAXFILES];

    Directory* subdirs[MAXSUBDIR];
    Directory* parent;

    DWORD nextFreeBlock;
};

typedef struct
{
    Directory root;
    Directory* currentDir;
} FileSystem;

void InitFileSystem();
int CreateFile(const char* filename, const LPBYTE data, DWORD size);
int ReadFile(const char* filename, LPBYTE buffer, LPDWORD size);
int DeleteFile(const char* filename);
int FindFile(const char* filename);
int FileInfo(const char* filename);
int RenameFile(const char* oldFilename, const char* newFilename);
void ListFiles();

int MakeDir(const char* dir);
int DeleteDir(const char* dirname);
int ChangeDir(const char* dirname);
void ListDirs();

void PrintCurrentDir();

Directory* GetRootDir();
