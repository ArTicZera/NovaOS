/*
    Coded by ArTic/JhoPro

    A RAM File system implementation. It's very simple and useful
    We can create files, directories, etc.
*/

#include "../Include/stdint.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "memfs.h"

FileSystem* fs = (FileSystem*) FSADDRESS;

Directory* currentDir;

void InitFileSystem()
{
    strncpy(fs->root.name, "root", MAXFILENAME);

    fs->root.parent = NULL;
    fs->root.nextFreeBlock = FSADDRESS + sizeof(FileSystem);

    for (int i = 0; i < MAXFILES; i++)
    {
        memset(fs->root.files[i].filename, 0x00, MAXFILENAME);
        fs->root.files[i].size = 0x00000000;
        fs->root.files[i].dataOffset = 0x00000000;
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        fs->root.subdirs[i] = 0x00;
    }

    currentDir = &fs->root;
}

int CreateFile(const char* filename, const LPBYTE data, DWORD size)
{
    //Checks for invalid file name
    if (size == 0 || strlen(filename) >= MAXFILENAME)
    {
        return -1;
    }

    int fileIndex = -1;

    //Checks for enough space
    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentDir->files[i].filename[0] == '\0')
        {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1)
    {
        return -2;
    }

    DWORD dataOffset = currentDir->nextFreeBlock;
    currentDir->nextFreeBlock += size;

    strncpy(currentDir->files[fileIndex].filename, filename, MAXFILENAME);
    currentDir->files[fileIndex].size = size;
    currentDir->files[fileIndex].dataOffset = dataOffset;

    LPBYTE fileData = (LPBYTE) dataOffset;
    memcpy(fileData, data, size);

    return 0x00;
}

int ReadFile(const char* filename, LPBYTE buffer, LPDWORD size)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            *size = currentDir->files[i].size;

            LPBYTE fileData = (LPBYTE) currentDir->files[i].dataOffset;
            memcpy(buffer, fileData, *size);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

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

            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

    return -1; 
}

int FileInfo(const char* filename) 
{
    Print("\n", 0x00);

    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentDir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            Print("\nFilename: ", 0x0A);
            Print(currentDir->files[i].filename, 0x0F);

            Print("\nSize: ", 0x0A);
            PrintInt(currentDir->files[i].size, 0x0F);

            Print("\nData Offset: ", 0x0A);
            PrintInt(currentDir->files[i].dataOffset, 0x0F);
            
            return 0x00;
        }
    }

    Print("\n\nInvalid File!", 0x0C);

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

    Print("\n\nInvalid File!", 0x0C);

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
            Print(currentDir->files[i].filename, 0x0B);
            Print(" ", 0x00);
        }
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] != NULL)
        {
            Print(currentDir->subdirs[i]->name, 0x0E);
            Print(" ", 0x00);
        }
    }
}

//-------------------------------------------------------

//Dir Functions

int MakeDir(const char* dir)
{
    if (strlen(dir) >= MAXFILENAME || dir[0] == '\0') 
    {
        return -1;
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentDir->subdirs[i] == NULL)
        {
            Directory* newDir = (Directory*) currentDir->nextFreeBlock;
            currentDir->nextFreeBlock += sizeof(Directory);

            memset(newDir, 0x00, sizeof(Directory));
            strncpy(newDir->name, dir, MAXFILENAME - 1);
            newDir->parent = currentDir;
            newDir->nextFreeBlock = (DWORD)newDir + sizeof(Directory);

            currentDir->subdirs[i] = newDir;

            return 0x00; // Sucesso
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
            Print(fs->currentDir->subdirs[i]->name, 0x0E);
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
    char path[256] = ""; // Inicializa a string com vazio
    Directory* temp = currentDir;

    while (temp != NULL)
    {
        char buffer[MAXFILENAME + 2]; // +2 para incluir "/" e o terminador nulo
        buffer[0] = '/'; // Adiciona "/" no início
        strncpy(buffer + 1, temp->name, MAXFILENAME); // Copia o nome do diretório após "/"
        buffer[MAXFILENAME + 1] = '\0'; // Garante o terminador nulo

        // Adiciona o diretório atual ao caminho
        char tempPath[256];
        strcpy(tempPath, path); // Salva o caminho atual
        strcpy(path, buffer);  // Começa com o novo diretório
        strcat(path, tempPath); // Concatena o caminho anterior

        temp = temp->parent; // Vai para o diretório pai
    }

    for (int i = 0; i < 256; i++)
    {
        path[i] = path[i + 1];
    }
    
    Print(path, 0x0F);
    Print("> ", 0x0F);
}
