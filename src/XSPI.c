#include <Arduino.h>
#include "XSPI.h"
#include "pins.h"

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

#define KSKLo	digitalWrite(KIOSK_PIN, LOW)
#define KSKHi	digitalWrite(KIOSK_PIN, HIGH)

//#define MISO	PORTBbits.RB2

#define Delay1KTCYx(x) delay(100)
#define Delay10KTCYx(x) delay(100)

void XSPIInit()
{
	EJHi; SSHi; XXHi; SCKHi; KSKHi;

    pinMode(KIOSK_PIN, OUTPUT);
    pinMode(EJ_PIN, OUTPUT);
    pinMode(XX_PIN, OUTPUT);
    pinMode(SS_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
}

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

void XSPIShutdown()
{
	SSHi; XXLo; EJLo;
	Delay1KTCYx(0);
	EJHi;

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

void XSPILeaveFlashMode()
{
	SSHi; EJLo;

	Delay10KTCYx(0);

	XXLo; EJHi;
}

BYTE XSPI(BYTE o)
{
	BYTE i = 0x00;
	
	if (o & 0x01) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x01:0x00;
	SCKLo;	
	
	if (o & 0x02) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x02:0x00;
	SCKLo;
	
	if (o & 0x04) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x04:0x00;
	SCKLo;
	
	if (o & 0x08) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x08:0x00;
	SCKLo;
	
	if (o & 0x10) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x10:0x00;
	SCKLo;
	
	if (o & 0x20) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x20:0x00;
	SCKLo;
	
	if (o & 0x40) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x40:0x00;	
	SCKLo;
	
	if (o & 0x80) MOSIHi; else MOSILo;
	SCKHi;
	i |= MISO?0x80:0x00;
	SCKLo;
	
	return i;
}

void XSPIW(BYTE o)
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


BYTE XSPIR()
{
	BYTE i = 0x00;
	MOSILo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x01:0x00;
	SCKLo;	
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x02:0x00;
	SCKLo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x04:0x00;
	SCKLo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x08:0x00;
	SCKLo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x10:0x00;
	SCKLo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x20:0x00;
	SCKLo;

	SCKHi;
	i |= digitalRead(MISO_PIN)?0x40:0x00;	
	SCKLo;
	
	SCKHi;
	i |= digitalRead(MISO_PIN)?0x80:0x00;
	SCKLo;
	
	return i;
}

void XSPIRead(BYTE reg, BYTE *data)
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

void XSPIWriteDWORD(BYTE reg, DWORD data)
{
	XSPIWrite(reg, &data);
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
