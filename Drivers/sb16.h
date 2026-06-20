void ResetSoundBlaster();
void WriteDSP(BYTE cmd);
void SoundBlasterHandler();
void SoundBlasterInit();
void SoundBlasterDisable();
void SetupDMA(LPBYTE buffer, WORD size);
void SoundBlasterPlay(LPBYTE buffer, WORD size);
void SetupSoundBlaster();
void PlayBytebeat(LPBYTE buffer, WORD size);