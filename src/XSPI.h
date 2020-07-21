#include "types.h"

void XSPIInit(void);

void XSPIEnterFlashMode(void);
void XSPILeaveFlashMode(void);
void XSPIPowerUp();
void XSPIShutdown();

BYTE XSPI(BYTE o);
void XSPIRead(BYTE reg, BYTE *data);
void XSPIWrite(BYTE reg, BYTE *data);

//This are optimized special cases for the functions above
void XSPIW(BYTE o);
void XSPIW0(void);
BYTE XSPIR(void);
WORD XSPIReadWord(BYTE reg);
BYTE XSPIReadByte(BYTE reg);
void XSPIWrite0(BYTE reg);
void XSPIWriteByte(BYTE reg, BYTE d);
void XSPIWriteDWORD(BYTE reg, DWORD data);
