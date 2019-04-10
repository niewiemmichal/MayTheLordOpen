#pragma once

#include <Windows.h>
#include <atlstr.h>
#include <string>
#include <vector>

#define INPUT_BUFFER_SIZE 1024
#define READ_TIMEOUT	100

#define DTR_DSR_AUTO	0x01
#define DTR_DSR_MANUAL	0x02
#define RTS_CTS_AUTO	0x03
#define RTS_CTS_MANUAL	0x04

#define MAX_PORT_SCAN	9

class SerialConnection
{
public:
	SerialConnection();

	SerialConnection(unsigned long baudRate, unsigned int dataFieldSize, unsigned int stopBitsNumber, unsigned int parityBitType);

	bool open(unsigned int serialPortNumber);

	bool open(CString serialPortName);

	bool open(std::string serialPortName);

	unsigned int write(std::string dataToSend);

	unsigned int write(CStringA dataToSend);

	std::string read(void);

	bool setEventsMask(unsigned long eventMask);

	unsigned long waitForEvent(unsigned long &eventSignature);

	bool close(void);

	~SerialConnection();

	bool setBaudRate(unsigned long baudRate);

	bool setDataFieldSize(unsigned long dataFieldSize);

	bool setStopBitsNumber(unsigned long stopBitsNumber);

	bool setParityBitType(unsigned long parityBitType);

	bool enableHardwareFlowControl(unsigned int type);

	bool enableSoftwareFlowControl(void);

	bool disableFlowControl(void);

	bool setDTRLineState(bool DTRLineState);

	bool setRTSLineState(bool RTSLineState);

	bool getCTSLineState(void) const;

	bool getDSRLineState(void) const;

	unsigned int getHardwareFlowControlType(void) const;

	static std::vector<std::string> scanAvaliableSerialPort(void);

private:
	HANDLE			hCOMPort;
	DCB				COMPortConfiguration;
	COMMTIMEOUTS	COMPortTimeouts;

	OVERLAPPED		overlapedSerialRead;
	OVERLAPPED		overlapedSerialWrite;

	unsigned long	baudRate;
	unsigned int	dataFieldSize;
	unsigned int	stopBitsNumber;
	unsigned int	parityBitType;

	unsigned int	hardwareFlowControlType;

};

