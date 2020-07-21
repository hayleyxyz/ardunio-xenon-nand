#include "types.h"

DWORD XNANDGetConfig();

WORD XNANDReadStart(DWORD block);
void XNANDReadProcess(BYTE *buffer, BYTE words);

WORD XNANDErase(DWORD block);

void XNANDWriteStart(void);
void XNANDWriteProcess(BYTE *buffer, BYTE words);
WORD XNANDWriteExecute(DWORD block);
