#include "stdafx.h"

#include "SerialConnection.h"

#include "SerialWriterException.h"


SerialConnection::SerialConnection()
{
	hCOMPort = NULL;

	COMPortConfiguration = { 0 };
	COMPortTimeouts = { 0 };

	overlapedSerialRead = { 0 };
	overlapedSerialWrite = { 0 };

	baudRate = CBR_9600;
	dataFieldSize = 8;
	stopBitsNumber = 0;
	parityBitType = NOPARITY;

	hardwareFlowControlType = 0;
	
}


SerialConnection::SerialConnection(unsigned long baudRate, unsigned int dataFieldSize, unsigned int stopBitsNumber, unsigned int parityBitType)
{
	hCOMPort = NULL;

	COMPortConfiguration = { 0 };
	COMPortTimeouts = { 0 };

	overlapedSerialRead = { 0 };
	overlapedSerialWrite = { 0 };

	this->baudRate = baudRate;
	this->dataFieldSize = dataFieldSize;
	this->stopBitsNumber = stopBitsNumber;
	this->parityBitType = parityBitType;

	hardwareFlowControlType = 0;
}


bool SerialConnection::open(unsigned int serialPortNumber)
{
	CString serialPortName;
	serialPortName.Format(_T("COM%d"), serialPortNumber);
	return open(serialPortName);
}


bool SerialConnection::open(CString serialPortName)
{
	hCOMPort = CreateFile(serialPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (!(hCOMPort == INVALID_HANDLE_VALUE)) {
		if (GetCommState(hCOMPort, &COMPortConfiguration)) {
			COMPortConfiguration.BaudRate = baudRate;
			COMPortConfiguration.ByteSize = dataFieldSize;
			COMPortConfiguration.StopBits = stopBitsNumber;
			COMPortConfiguration.Parity = parityBitType;
			if (!SetCommState(hCOMPort, &COMPortConfiguration)) return false;
			COMPortTimeouts.ReadIntervalTimeout = 30;
			COMPortTimeouts.ReadTotalTimeoutConstant = 30;
			COMPortTimeouts.ReadTotalTimeoutMultiplier = 30;
			COMPortTimeouts.WriteTotalTimeoutConstant = 30;
			COMPortTimeouts.WriteTotalTimeoutMultiplier = 30;
			if (!SetCommTimeouts(hCOMPort, &COMPortTimeouts)) return false;
			setEventsMask(EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY);
		}
		return true;
	} 
	else return false;
}


bool SerialConnection::open(std::string serialPortName)
{
	CString serialPortNameC(serialPortName.c_str());
	return  open(serialPortNameC);
}


unsigned int SerialConnection::write(std::string dataToSend)
{
	CStringA dataToSendC(dataToSend.c_str());
	return write(dataToSendC);
}


unsigned int SerialConnection::write(CStringA dataToSend)
{
	unsigned long writtenBytes;
	unsigned long dataWaitingResult;

	overlapedSerialWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlapedSerialWrite.hEvent) {
		if (!WriteFile(hCOMPort, dataToSend, strlen(dataToSend), &writtenBytes, &overlapedSerialWrite)) {
			if (GetLastError() != ERROR_IO_PENDING) throw new SerialWriterException();
			else {
				dataWaitingResult = WaitForSingleObject(overlapedSerialWrite.hEvent, INFINITE);
				switch (dataWaitingResult)
				{
				case WAIT_OBJECT_0:
					if (!GetOverlappedResult(hCOMPort, &overlapedSerialWrite, &writtenBytes, FALSE)) throw new SerialWriterException();
					else return writtenBytes;
					break;
				default:
					throw new SerialWriterException();
					break;
				}
			}
		}
		else throw new SerialWriterException();	// Port connection lost - port may e unavaliable
	}
	else throw new SerialWriterException();
}


std::string SerialConnection::read(void)
{
	DWORD dwRead;
	DWORD dwRes;
	BOOL fWaitingOnRead = FALSE;

	char inputBuffer[INPUT_BUFFER_SIZE + 1] = {0};
	inputBuffer[INPUT_BUFFER_SIZE] = 0;

	overlapedSerialRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (overlapedSerialRead.hEvent != NULL)
	{
		if (!fWaitingOnRead)
		{
			// Issue read operation.
			if (!ReadFile(hCOMPort, inputBuffer, INPUT_BUFFER_SIZE, NULL, &overlapedSerialRead)) {
				if (GetLastError() != ERROR_IO_PENDING) return false;
				else fWaitingOnRead = TRUE;
			}
			else {
				// read completed immediately
				// HandleASuccessfulRead(lpBuf, dwRead);
				std::string result = inputBuffer;
				return result;
			}
		}
	}
	while (fWaitingOnRead) {
		if (fWaitingOnRead) {
			dwRes = WaitForSingleObject(overlapedSerialRead.hEvent, READ_TIMEOUT);
			switch (dwRes)
			{
				// Read completed.
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(hCOMPort, &overlapedSerialRead, &dwRead, FALSE));	// Error in communications; report it.
				else {
					// Read completed successfully.
					// Reset flag so that another opertion can be issued.
					fWaitingOnRead = FALSE;
					// HandleASuccessfulRead(lpBuf, dwRead);
					std::string result = inputBuffer;
					return result;
				}
				break;

			case WAIT_TIMEOUT:
				// Operation isn't complete yet. fWaitingOnRead flag isn't
				// changed since I'll loop back around, and I don't want
				// to issue another read until the first one finishes.
				// This is a good time to do some background work.
				break;

			default:
				// Error in the WaitForSingleObject; abort.
				// This indicates a problem with the OVERLAPPED structure's
				// event handle.
				break;
			}
		}
	}

	
	return std::string();
}


bool SerialConnection::setEventsMask(unsigned long eventMask)
{
	return SetCommMask(hCOMPort, eventMask);
}


unsigned long SerialConnection::waitForEvent(unsigned long & eventSignature)
{
	return WaitCommEvent(hCOMPort, &eventSignature, NULL);
}


bool SerialConnection::close(void)
{
	if (hCOMPort)
	{
		CloseHandle(hCOMPort);
		hCOMPort = NULL;
		return true;
	}
	else return false;
}


SerialConnection::~SerialConnection()
{
	if (hCOMPort) CloseHandle(hCOMPort);
}


bool SerialConnection::setBaudRate(unsigned long baudRate)
{
	this->baudRate = baudRate;
	if (hCOMPort) {
		COMPortConfiguration.BaudRate = baudRate;
		if (SetCommState(hCOMPort, &COMPortConfiguration)) return true;
		else return false;
	}
	return true;
}


bool SerialConnection::setDataFieldSize(unsigned long dataFieldSize)
{
	this->dataFieldSize = dataFieldSize;
	if (hCOMPort) {
		COMPortConfiguration.ByteSize = dataFieldSize;
		if (SetCommState(hCOMPort, &COMPortConfiguration)) return true;
		else return false;
	}
	return true;
}


bool SerialConnection::setStopBitsNumber(unsigned long stopBitsNumber)
{
	this->stopBitsNumber = stopBitsNumber;
	if (hCOMPort) {
		COMPortConfiguration.StopBits = stopBitsNumber;
		if (SetCommState(hCOMPort, &COMPortConfiguration)) return true;
		else return false;
	}
	return true;
}


bool SerialConnection::setParityBitType(unsigned long parityBitType)
{
	this->parityBitType = parityBitType;
	if (hCOMPort) {
		COMPortConfiguration.Parity = parityBitType;
		if (SetCommState(hCOMPort, &COMPortConfiguration)) return true;
		else return false;
	}
	return true;
}


bool SerialConnection::enableHardwareFlowControl(unsigned int type)
{
	switch (type)
	{
	case DTR_DSR_AUTO:
		// Sending control configuration
		COMPortConfiguration.fOutxCtsFlow = FALSE;
		COMPortConfiguration.fOutxDsrFlow = TRUE;
		// Receiving control configuration
		COMPortConfiguration.fRtsControl = RTS_CONTROL_ENABLE;	// Line status set to active
		COMPortConfiguration.fDtrControl = DTR_CONTROL_HANDSHAKE;	// Line status controlled automaticaly
		hardwareFlowControlType = DTR_DSR_AUTO;
		break;
	case DTR_DSR_MANUAL:
		// Sending control configuration
		COMPortConfiguration.fOutxCtsFlow = FALSE;
		COMPortConfiguration.fOutxDsrFlow = TRUE;
		// Receiving control configuration
		COMPortConfiguration.fRtsControl = RTS_CONTROL_DISABLE;	// Line status set to inactive
		COMPortConfiguration.fDtrControl = DTR_CONTROL_DISABLE;	// Line status set to inactive
		hardwareFlowControlType = DTR_DSR_MANUAL;
		break;
	case RTS_CTS_AUTO:
		// Sending control configuration
		COMPortConfiguration.fOutxCtsFlow = TRUE;
		COMPortConfiguration.fOutxDsrFlow = FALSE;
		// Receiving control configuration
		COMPortConfiguration.fRtsControl = RTS_CONTROL_HANDSHAKE; // Line status controlled automaticaly
		COMPortConfiguration.fDtrControl = DTR_CONTROL_ENABLE;	// Line status set to active
		hardwareFlowControlType = RTS_CTS_AUTO;
		break;
	case RTS_CTS_MANUAL:
		// Sending control configuration
		COMPortConfiguration.fOutxCtsFlow = TRUE;
		COMPortConfiguration.fOutxDsrFlow = FALSE;
		// Receiving control configuration
		COMPortConfiguration.fRtsControl = RTS_CONTROL_DISABLE;	// Line status set to inactive
		COMPortConfiguration.fDtrControl = DTR_CONTROL_DISABLE;	// Line status set to inactive
		hardwareFlowControlType = RTS_CTS_MANUAL;
		break;
	default:
		return false;
		break;
	}

	if (hCOMPort) {
		if (SetCommState(hCOMPort, &COMPortConfiguration)) {
			hardwareFlowControlType = type;
			return true;
		}
		else {
			hardwareFlowControlType = 0;
			return false;
		}
	}
	else {
		hardwareFlowControlType = type;
		return true;
	}
	
}


bool SerialConnection::enableSoftwareFlowControl(void)
{
	COMPortConfiguration.fInX = TRUE;
	COMPortConfiguration.fOutX = TRUE;
	COMPortConfiguration.fTXContinueOnXoff = FALSE;
	if (hCOMPort) 
	{
		if (SetCommState(hCOMPort, &COMPortConfiguration)) {
			hardwareFlowControlType = 0;
			return true;
		}
		else return false;
	}
	else 
	{
		hardwareFlowControlType = 0;
		return true;
	}
}


bool SerialConnection::disableFlowControl(void)
{
	COMPortConfiguration.fOutxCtsFlow = FALSE;
	COMPortConfiguration.fOutxDsrFlow = FALSE;
	COMPortConfiguration.fRtsControl = RTS_CONTROL_ENABLE;
	COMPortConfiguration.fDtrControl = DTR_CONTROL_ENABLE;
	COMPortConfiguration.fDsrSensitivity = FALSE;

	COMPortConfiguration.fInX = FALSE;
	COMPortConfiguration.fOutX = FALSE;

	if (hCOMPort)
	{
		if (SetCommState(hCOMPort, &COMPortConfiguration)) {
			hardwareFlowControlType = 0;
			return true;
		}
		else return false;
	}
	else
	{
		hardwareFlowControlType = 0;
		return true;
	}
}


bool SerialConnection::setDTRLineState(bool DTRLineState)
{
	if (hardwareFlowControlType == DTR_DSR_MANUAL) {
		if (DTRLineState) return EscapeCommFunction(hCOMPort, SETDTR);
		else return EscapeCommFunction(hCOMPort, CLRDTR);
	}
	else return false;
}


bool SerialConnection::setRTSLineState(bool RTSLineState)
{
	if (hardwareFlowControlType == RTS_CTS_MANUAL) {
		if (RTSLineState) return EscapeCommFunction(hCOMPort, SETRTS);
		else return EscapeCommFunction(hCOMPort, CLRRTS);
	}
	else return false;
}


bool SerialConnection::getCTSLineState(void) const
{
	DWORD statusByte = 0;
	if (GetCommModemStatus(hCOMPort, &statusByte)) return (statusByte & MS_CTS_ON);
	else return false;
}


bool SerialConnection::getDSRLineState(void) const
{
	DWORD statusByte = 0;
	if (GetCommModemStatus(hCOMPort, &statusByte)) return (statusByte & MS_DSR_ON);
	else return false;
}

unsigned int SerialConnection::getHardwareFlowControlType(void) const
{
	return hardwareFlowControlType;
}


std::vector<std::string> SerialConnection::scanAvaliableSerialPort(void)
{
	CString serialPortName;
	COMMCONFIG comPortStructurePointer;
	unsigned long structureSize = sizeof(comPortStructurePointer);
	std::vector<std::string> avaliablePortsList;

	for (unsigned int i = 1; i < MAX_PORT_SCAN; i++) {
		serialPortName.Format(_T("COM%d"), i);
		if (GetDefaultCommConfig(serialPortName, &comPortStructurePointer, &structureSize)) {
			avaliablePortsList.push_back("COM" + std::to_string(i));
		}
	}
	return avaliablePortsList;
}
