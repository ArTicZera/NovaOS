/*
    Coded by ArTic/JhoPro

    A RAM File system implementation. It's very simple and useful
    We can create files, directories, etc.
*/

#include "../Include/stdint.h"
#include "../Memory/alloc.h"
#include "../Memory/mem.h"
#include "../Font/text.h"
#include "../ELF/elf.h"

#include "memfs.h"

FileSystem* fs = (FileSystem*) FSADDRESS;

Directory* currentDir;

void InitFileSystem()
{
    fs = (FileSystem*) FSADDRESS;

    memset(fs, 0, sizeof(FileSystem));

    strncpy(fs->root.name, "", MAXFILENAME);

    fs->root.parent = NULL;

    // 🔥 alinhado
    fs->root.nextFreeBlock = (FSADDRESS + sizeof(FileSystem) + 3) & ~3;

    currentDir = &fs->root;
}

int CreateFile(const char* filename, const LPBYTE data, DWORD size, BYTE permissions)
{
    if (size == 0 || strlen(filename) >= MAXFILENAME)
        return -1;

    int fileIndex = -1;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentDir->files[i].filename[0] == '\0')
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1)
        return -2;

    // 🔥 ALIGN (ESSENCIAL)
    DWORD dataOffset = (currentDir->nextFreeBlock + 3) & ~3;

    currentDir->nextFreeBlock = dataOffset + size;

    strncpy(currentDir->files[fileIndex].filename, filename, MAXFILENAME);
    currentDir->files[fileIndex].size = size;
    currentDir->files[fileIndex].dataOffset = dataOffset;
    currentDir->files[fileIndex].permissions = permissions;

    memcpy((void*)dataOffset, data, size);

    return 0;
}

int ReadFile(const char* filename, LPBYTE buffer, LPDWORD size)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            if (!(currentDir->files[i].permissions & PERM_R))
            {
                Debug("Permission Denied!\n", 0x01); 
                return -1;
            }

            *size = currentDir->files[i].size;

            LPBYTE fileData = (LPBYTE) currentDir->files[i].dataOffset;
            memcpy(buffer, fileData, *size);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0xFFFF0000);

    return -1;
}

int DeleteFile(const char* filename)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            memset(currentDir->files[i].filename, 0x00, MAXFILENAME);

            currentDir->files[i].size = 0x00000000;
            currentDir->files[i].dataOffset = 0x00000000;
            currentDir->files[i].permissions = 0x00;

            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0xFFFF0000);

    return -1; 
}

int FileInfo(const char* filename) 
{
    Print("\n", 0x00);

    int index = FindFile(filename);

    if (index == -1)
    {
        return -1;
    }

    FileHeader* file = &currentDir->files[index];

    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            Print("\nFilename: ", 0xFF00FF00);
            Print(currentDir->files[i].filename, 0xFFFFFFFF);

            Print("\nSize: ", 0xFF00FF00);
            PrintInt(currentDir->files[i].size, 0xFFFFFFFF);

            Print("\nData Offset: ", 0xFF00FF00);
            PrintInt(currentDir->files[i].dataOffset, 0xFFFFFFFF);

            Print("\nPermissions: ", 0xFF00FF00);

            Print((file->permissions & PERM_R)  ? "R" : "-", 0xFFFFFFFF);
            Print((file->permissions & PERM_W) ? "W" : "-", 0xFFFFFFFF);
            Print((file->permissions & PERM_X)  ? "X" : "-", 0xFFFFFFFF);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0xFFFF0000);

    return -1;
}

int FindFile(const char* filename) 
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            return i;
        }
    }

    Print("\n\nInvalid File!", 0xFFFF0000);

    return -1;
}

int RenameFile(const char* oldFilename, const char* newFilename) 
{
    int fileIndex = FindFile(oldFilename);

    if (fileIndex == -1) 
    {
        return -1;
    }

    if (strlen(newFilename) >= MAXFILENAME) 
    {
        return -2; 
    }

    strncpy(currentDir->files[fileIndex].filename, newFilename, MAXFILENAME);

    return 0x00;
}

void ListFiles()
{
    Print("\n\n", 0x00);

    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentDir->files[i].filename[0] != '\0')
        {
            Print(currentDir->files[i].filename, 0xFF00FFFF);
            Print(" ", 0x00);
        }
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] != NULL)
        {
            Print(currentDir->subdirs[i]->name, 0xFFFFFF00);
            Print(" ", 0x00);
        }
    }
}

int CanExecute(const char* filename)
{
    int index = FindFile(filename);

    if (index == -1)
    {
        return -1;
    }

    if (currentDir->files[index].permissions & PERM_X)
    {
        return 1;
    }

    return 0;
}

//-------------------------------------------------------

//Dir Functions

int MakeDir(const char* dir)
{
    if (strlen(dir) >= MAXFILENAME || dir[0] == '\0')
        return -1;

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] == NULL)
        {
            // 🔥 ALIGN
            DWORD addr = (currentDir->nextFreeBlock + 3) & ~3;

            Directory* newDir = (Directory*) addr;

            currentDir->nextFreeBlock = addr + sizeof(Directory);

            memset(newDir, 0, sizeof(Directory));

            strncpy(newDir->name, dir, MAXFILENAME - 1);

            newDir->parent = currentDir;
            newDir->nextFreeBlock = (DWORD)newDir + sizeof(Directory);

            currentDir->subdirs[i] = newDir;

            return 0;
        }
    }

    return -2;
}

int DeleteDir(const char* dirname)
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] != 0x00 && strncmp(currentDir->subdirs[i]->name, dirname, MAXFILENAME) == 0x00)
        {
            Directory* dir = currentDir->subdirs[i];

            for (int j = 0; j < MAXFILES; j++)
            {
                if (dir->files[j].filename[0] != '\0')
                {
                    return -1;
                }
            }

            for (int j = 0; j < MAXSUBDIR; j++)
            {
                if (dir->subdirs[j] != NULL)
                {
                    return -1;
                }
            }

            currentDir->subdirs[i] = NULL;

            return 0x00;
        }
    }

    return -2;
}

int ChangeDir(const char* dirname)
{
    if (strcmp(dirname, "..") == 0x00)
    {
        if (currentDir->parent != 0x00)
        {
            currentDir = currentDir->parent;

            return 0x00;
        }

        return -1;
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] != 0x00 && strncmp(currentDir->subdirs[i]->name, dirname, MAXFILENAME) == 0x00)
        {
            currentDir = currentDir->subdirs[i];

            return 0x00;
        }
    }

    return -2;
}

void ListDirs()
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (fs->currentDir->subdirs[i] != NULL)
        {
            Print(fs->currentDir->subdirs[i]->name, 0xFF00FFFF);
            Print(" ", 0x00);
        }
    }
}

Directory* GetRootDir()
{
    return currentDir;
}

void PrintCurrentDir() 
{
    Print("[", 0xFFFFFFFF);
    Print("root", 0x0000FF00);
    Print("@", 0xFFFFFFFF);
    Print("novaos", 0xFF00FFFF);
    Print(":", 0xFFFFFFFF);

    char path[256] = ""; // Start empty string
    Directory* temp = currentDir;

    while (temp != NULL)
    {
        char buffer[MAXFILENAME + 2]; // +2 to include "/" e and null terminator
        buffer[0] = '/'; // Adds "/" in the start
        strncpy(buffer + 1, temp->name, MAXFILENAME); // Copy dirs name after "/"
        buffer[MAXFILENAME + 1] = '\0'; // Make it finishes with null

        //Adds actuall dir to its address
        char tempPath[256];
        strcpy(tempPath, path); //Save path
        strcpy(path, buffer);   //Start new Dir
        strcat(path, tempPath);

        temp = temp->parent; //Fathers dir
    }

    for (int i = 0; i < 256; i++)
    {
        path[i] = path[i + 1];
    }
    
    Print(path,  0xFFFFFFFF);
    Print("]# ", 0xFFFFFFFF);
}

void RunProgram(char* filename)
{
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '\n' || filename[i] == '\r')
        {
            Print("Comparing with: [", 0xFF00FFFF);
            Print(currentDir->files[i].filename, 0xFF00FFFF);
            Print("]\n", 0xFF00FFFF);
            
            filename[i] = '\0';
            break;
        }
    }

    DWORD size;
    LPBYTE buffer = AllocateMemory(1024 * 1024);

    Print("Trying to open: [", 0xFFFFFFFF);
    Print(filename, 0xFFFFFFFF);
    Print("]\n", 0xFFFFFFFF);

    if (ReadFile(filename, buffer, &size) != 0)
    {
        Print("Searching for: [", 0xFFFFFF00);
        Print(filename, 0xFFFFFF00);
        Print("]\n", 0xFFFFFF00);
        
    }

    Print("Magic: ", 0xFFFFFFFF);
    PrintHex(*(DWORD*)buffer, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("File loaded!\n", 0x02);

    void (*entry)() = (void (*)()) LoadELF(buffer);

    if (!entry)
    {
        Debug("Invalid ELF!\n", 0x01);
        return;
    }

    Print("Entry: ", 0xFFFFFFFF);
    PrintHex((DWORD)entry, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("Executing...\n", 0x02);

    entry();
}

char* get_filename(char* path)
{
    char* last = path;

    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            last = &path[i + 1];
        }
    }

    return last;
}

FileHeader* GetFileHeader(const char* filename)
{
    Directory* dir = currentDir;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (dir->files[i].filename[0] == 0)
            continue;

        if (strcmp(dir->files[i].filename, filename) == 0)
        {
            return &dir->files[i];
        }
    }

    return NULL;
}

int ReadFileChunk(const char* filename, BYTE* buffer, DWORD offset, DWORD bytesToRead)
{
    FileHeader* file = GetFileHeader(filename);

    if (file == NULL)
        return -1;

    if (offset >= file->size)
        return -2;

    if (offset + bytesToRead > file->size)
        bytesToRead = file->size - offset;

    BYTE* src = (BYTE*)(FSADDRESS + file->dataOffset + offset);

    memcpy(buffer, src, bytesToRead);

    return bytesToRead;
}
