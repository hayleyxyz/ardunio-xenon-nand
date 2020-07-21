#include <Arduino.h>
#include <SPI.h>
#include <string.h>

static int SS_PIN = 10;
static int SCK_PIN = 13;
static int MOSI_PIN = 11;
static int MISO_PIN = 12;
static int KIOSK_PIN = 7;
static int EJ_PIN = 5;
static int XX_PIN = 6;

/*
typedef enum _BOOL { FALSE = 0, TRUE } BOOL;	// Undefined size
typedef unsigned char		BYTE;				// 8-bit unsigned
typedef unsigned short int	WORD;				// 16-bit unsigned
typedef unsigned long		DWORD;				// 32-bit unsigned
typedef unsigned long long	QWORD;				// 64-bit unsigned
typedef signed char			CHAR;				// 8-bit signed
typedef signed short int	SHORT;				// 16-bit signed
typedef signed long			LONG;				// 32-bit signed
typedef signed long long	LONGLONG;			// 64-bit signed

#define FLASH_TX_READY                0
#define FLASH_TX_BUSY                 1
#define FLASH_TX_COMPLETING           2

BYTE FlashConfigBuffer[4];
BYTE eraseCycle = 0x00;

#define FlashTxSetCBF(pCBF,len)    \
{                                   \
    FlashTxCBF = pCBF;              \
    FlashTxLen = len;               \
    FlashTxState = FLASH_TX_BUSY;    \
}

WORD FlashTxLen;
BYTE FlashTxState = FLASH_TX_READY;
void (*FlashTxCBF)(BYTE *data, BYTE size);

#define EJLo	digitalWrite(EJ_PIN, LOW)
#define EJHi	digitalWrite(EJ_PIN, HIGH)

#define XXLo	digitalWrite(XX_PIN, LOW)
#define XXHi	digitalWrite(XX_PIN, HIGH)

#define SSLo	digitalWrite(SS_PIN, LOW)
#define SSHi	digitalWrite(SS_PIN, HIGH)

#define SCKLo	digitalWrite(SCK_PIN, LOW)
#define SCKHi	digitalWrite(SCK_PIN, HIGH)

#define MOSILo	digitalWrite(MOSI_PIN, LOW)
#define MOSIHi	digitalWrite(MOSI_PIN, HIGH)

#define KSKLo   digitalWrite(KIOSK_PIN, LOW)
#define KSKHi   digitalWrite(KIOSK_PIN, HIGH)

#define Delay1KTCYx delayMicroseconds
#define Delay10KTCYx delayMicroseconds

void XSPIPowerUp()
{
	KSKLo;
	Delay1KTCYx(0);
	KSKHi;
	Delay1KTCYx(0);
	KSKLo;
	Delay1KTCYx(0);
	KSKHi;
	Delay1KTCYx(0);
	KSKLo;
	Delay1KTCYx(0);
	KSKHi;
	Delay1KTCYx(0);
	KSKLo;
	Delay1KTCYx(0);
	KSKHi;
}

unsigned char XSPIR()
{
	unsigned char i = 0x00;
	MOSILo;
	
	SCKHi;
	i |= MISO?0x01:0x00;
	SCKLo;	
	
	SCKHi;
	i |= MISO?0x02:0x00;
	SCKLo;
	
	SCKHi;
	i |= MISO?0x04:0x00;
	SCKLo;
	
	SCKHi;
	i |= MISO?0x08:0x00;
	SCKLo;
	
	SCKHi;
	i |= MISO?0x10:0x00;
	SCKLo;
	
	SCKHi;
	i |= MISO?0x20:0x00;
	SCKLo;

	SCKHi;
	i |= MISO?0x40:0x00;	
	SCKLo;
	
	SCKHi;
	i |= MISO?0x80:0x00;
	SCKLo;
	
	return i;
}

void XSPIInit() {
  EJHi; SSHi; XXHi; SCKHi; KSKHi;
  pinMode(EJ_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  pinMode(XX_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(KIOSK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
}

void XSPIW(unsigned char o)
{
	if (o & 0x01) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x02) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x04) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x08) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x10) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x20) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x40) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
	
	if (o & 0x80) MOSIHi; else MOSILo;
	SCKHi; SCKLo;	
}

void XSPIRead(unsigned char reg, unsigned char *data)
{
	SSLo;

	XSPIW((reg << 2) | 1);
	
	XSPIW(0xFF);

	*data++ = XSPIR();
	*data++ = XSPIR();
	*data++ = XSPIR();
	*data++ = XSPIR();

	SSHi;
}


void XSPIEnterFlashMode()
{
	XXLo;

	Delay10KTCYx(0);

	SSLo; EJLo;

	Delay10KTCYx(0);

	XXHi; EJHi;

	Delay10KTCYx(0);
}

void XSPIW0()
{
	MOSILo;

	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
	SCKHi; SCKLo;	
}

void FlashReadConfigCB(BYTE *buffer, BYTE len)
{
	memcpy(buffer, FlashConfigBuffer, len);
	if((buffer[1] == 0x8A) || (buffer[1] == 0xAA) || (buffer[2] == 0x8A) || (buffer[2] == 0xAA)) // to be safe without testing
		eraseCycle = 7;
	else
		eraseCycle = 0;
}

void FlashDataInit()
{
	XSPIEnterFlashMode();
	
	XSPIRead(0, FlashConfigBuffer);
	XSPIRead(0, FlashConfigBuffer);

	FlashTxSetCBF(FlashReadConfigCB, 4);
}

void XSPIWrite(BYTE reg, BYTE *data)
{
	SSLo;

	XSPIW((reg << 2) | 2);

	XSPIW(*data++);
	XSPIW(*data++);
	XSPIW(*data++);
	XSPIW(*data++);

	SSHi;
}

BYTE XSPIReadByte(BYTE reg)
{
	BYTE res;

	SSLo;

	XSPIW((reg << 2) | 1);
	
	XSPIW(0xFF);

	res = XSPIR();

	SSHi;

	return res;
}


void XSPIWriteByte(BYTE reg, BYTE d)
{
	SSLo;

	XSPIW((reg << 2) | 2);

	XSPIW(d);
	XSPIW0();
	XSPIW0();
	XSPIW0();

	SSHi;
}


void XSPIWriteDWORD(BYTE reg, DWORD data)
{
	XSPIWrite(reg, (BYTE *)&data);
}

WORD XNANDClearStatus()
{
	BYTE tmp[4];

	XSPIRead(4, tmp);
	XSPIWrite(4, tmp);	
}

BOOL XNANDWaitReady(WORD timeout)
{
	do {
		if (!(XSPIReadByte(0x04) & 0x01))
			return TRUE;
	} while (timeout--);

	return FALSE;
}

WORD XSPIReadWord(BYTE reg)
{
	WORD res;

	SSLo;

	XSPIW((reg << 2) | 1);
	
	XSPIW(0xFF);

	res = XSPIR();
	res|= ((WORD)XSPIR()) << 8;

	SSHi;

	return res;
}

WORD XNANDGetStatus()
{
	return XSPIReadWord(0x04);
}

void XSPIWrite0(BYTE reg)
{
	SSLo;

	XSPIW((reg << 2) | 2);

	XSPIW0();
	XSPIW0();
	XSPIW0();
	XSPIW0();

	SSHi;
}

WORD XNANDReadStart(DWORD block)
{
	WORD res;
	WORD tries = 0x1000;

	XNANDClearStatus();

	XSPIWriteDWORD(0x0C, block << 9);

	XSPIWriteByte(0x08, 0x03);

	if (!XNANDWaitReady(0x1000))
		return 0x8011;

	res = XNANDGetStatus();

	XSPIWrite0(0x0C);

	return res;
}
*/

const byte WRITE = 0b00000010;
const byte READ = 0b00000001;

uint8_t XSPIReadByte(uint8_t reg)
{
	reg = reg << 2;
	// now combine the register address and the command into one byte:
	byte dataToSend = reg | READ;

	// take the chip select low to select the device:
	digitalWrite(SS_PIN, LOW);

	SPI.transfer(dataToSend); //Send register location
	uint8_t result = SPI.transfer(0x00);

	// take the chip select high to de-select:
	digitalWrite(SS_PIN, HIGH);

	return result;
}

void XSPIWrite(uint8_t reg, uint8_t *data)
{
	digitalWrite(SS_PIN, LOW);

	reg = (reg << 2) | WRITE;

	SPI.transfer(reg);
	SPI.transfer(*data++);
	SPI.transfer(*data++);
	SPI.transfer(*data++);
	SPI.transfer(*data++);

	digitalWrite(SS_PIN, HIGH);
}

void writeRegister32(byte thisRegister, uint32_t thisValue) {

  // SCP1000 expects the register address in the upper 6 bits
  // of the byte. So shift the bits left by two bits:
  thisRegister = thisRegister << 2;
  // now combine the register address and the command into one byte:
  byte dataToSend = thisRegister | WRITE;

  // take the chip select low to select the device:
  digitalWrite(SS_PIN, LOW);

  SPI.transfer(dataToSend); //Send register location
  SPI.transfer((void*)&thisValue, sizeof(uint32_t));   //Send value to record into register

  // take the chip select high to de-select:
  digitalWrite(SS_PIN, HIGH);
}

void writeRegister16(byte thisRegister, uint16_t thisValue) {

  // SCP1000 expects the register address in the upper 6 bits
  // of the byte. So shift the bits left by two bits:
  thisRegister = thisRegister << 2;
  // now combine the register address and the command into one byte:
  byte dataToSend = thisRegister | WRITE;

  // take the chip select low to select the device:
  digitalWrite(SS_PIN, LOW);

  SPI.transfer(dataToSend); //Send register location
  SPI.transfer(thisValue);   //Send value to record into register

  // take the chip select high to de-select:
  digitalWrite(SS_PIN, HIGH);
}

void writeRegister8(byte thisRegister, byte thisValue) {

  // SCP1000 expects the register address in the upper 6 bits
  // of the byte. So shift the bits left by two bits:
  thisRegister = thisRegister << 2;
  // now combine the register address and the command into one byte:
  byte dataToSend = thisRegister | WRITE;

  // take the chip select low to select the device:
  digitalWrite(SS_PIN, LOW);

  SPI.transfer(dataToSend); //Send register location
  SPI.transfer(thisValue);   //Send value to record into register

  // take the chip select high to de-select:
  digitalWrite(SS_PIN, HIGH);
}

unsigned int readRegister(byte thisRegister, int bytesToRead) {
  byte inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return
  // SCP1000 expects the register name in the upper 6 bits
  // of the byte. So shift the bits left by two bits:
  thisRegister = thisRegister << 2;
  // now combine the address and the command into one byte
  byte dataToSend = thisRegister | READ;
  // take the chip select low to select the device:
  digitalWrite(SS_PIN, LOW);
  // send the device the register you want to read:
  SPI.transfer(dataToSend);
  // send a value of 0 to read the first byte returned:
  result = SPI.transfer(0x00);
  // decrement the number of bytes left to read:
  bytesToRead--;
  // if you still have another byte to read:
  if (bytesToRead > 0) {
    // shift the first byte left, then get the second byte:
    result = result << 8;
    inByte = SPI.transfer(0x00);
    // combine the byte you just got with the previous one:
    result = result | inByte;
    // decrement the number of bytes left to read:
    bytesToRead--;
  }
  // take the chip select high to de-select:
  digitalWrite(SS_PIN, HIGH);
  // return the result:
  return (result);
}

int XNANDWaitReady(unsigned short timeout)
{
	do {
		if (!(readRegister(0x04, 1) & 0x01))
			return 1;
	} while (timeout--);

	return 0;
}

void XNANDReadProcess(unsigned char *buffer, unsigned char words) {
	while (words--) {
		writeRegister8(0x08, 0);
    	*(unsigned int *)buffer = readRegister(0x10, 4);

    Serial.println(*(unsigned int *)buffer, HEX);

		buffer+=4;
	}
}


void setup() {
  Serial.begin(9600);

  Serial.println("butt");

  pinMode(EJ_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  pinMode(XX_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(KIOSK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);


  SPI.begin();

  writeRegister32(0x0C, 2 << 9);
  writeRegister8(0x08, 0x03);

  if(!XNANDWaitReady(1000)) {
    Serial.println("ERR: XNANDWaitReady()");
  }

//XNANDGetStatus
  auto res = readRegister(0x04, 2);

	writeRegister8(0x0c,  0);

  unsigned char buf[0x100];
  XNANDReadProcess(buf, 0x100 / 4);


}

void loop() {

}