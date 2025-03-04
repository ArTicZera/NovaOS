BYTE IdentifyATA(BYTE channel, BYTE drive);
void ReadFromATA(BYTE channel, BYTE drive, DWORD lba, WORD* buffer);
void WriteByteToATA(BYTE channel, BYTE drive, DWORD lba, BYTE data);
