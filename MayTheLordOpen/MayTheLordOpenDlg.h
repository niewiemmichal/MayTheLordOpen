#pragma once
#include "SerialConnection.h"
#include <chrono>

struct listenerEventResources {
	CButton *toDSRStatePointer;
	CButton *toCTSStatePointer;
	CEdit *toOutputDataBoxPointer;
};

// CMayTheLordOpenDlg dialog
class CMayTheLordOpenDlg : public CDialogEx
{

public:
	CMayTheLordOpenDlg(CWnd* pParent = nullptr);	// standard constructor

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAYTHELORDOPEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	SerialConnection serialPort;
	listenerEventResources threadResources;
	bool connectionStatus;

	void serialPortEventLoop(listenerEventResources *pointer);
	static UINT __cdecl serialPortEventLoopStatic(LPVOID pParam);

	std::chrono::steady_clock::time_point startTimer;
	std::chrono::steady_clock::time_point endTimer;
	std::chrono::duration<double> timeElapsed;

	CString selectedCOMPortName;
	CString selectedBaudRate;
	CString selectedDataBits;
	CString selectedParityBit;
	CString selectedStopBits;
	CString selectedHardwareFlowControl;

	CStringA terminatorSign;

	void enableCommunicationWindows(void);
	void disableCommunicationWindws(void);
	void changeDialogToConnectionState(void);
	void changeDialogToDisconnectionState(void);

	void processReceivedData(CEdit *toOutputDataBoxPointer);
	void processCTSStateChange(CButton *toCTSStatePointer);
	void processDSRStateChange(CButton *toDSRStatePointer);

public:
	CComboBox COMPortSelectionBox;
	CComboBox baudRateSelection;
	CComboBox dataBitsSelection;
	CComboBox parityBitSelection;
	CComboBox stopBitsSelection;
	CButton connectButton;
	afx_msg void OnCOMPortDropdownCombo();
	afx_msg void OnCOMPortSelected();
	afx_msg void OnBaudRateSelected();
	afx_msg void OnDataBitsSelected();
	afx_msg void OnParityBitSelected();
	afx_msg void OnStopBitsSelected();

	CButton flowControlDisabled;
	CButton flowControlHardware;
	CButton flowControlSoftware;
	CComboBox hardwareFlowControlSelectionBox;
	afx_msg void OnFlowControlDisabledClicked();
	afx_msg void OnFlowControlHardwareClicked();
	afx_msg void OnFlowControlSoftwareClicked();
	afx_msg void OnHardwareFlowControlTypeSelected();
	
	CButton noTerminator;
	CButton CRTerminator;
	CButton LFTerminator;
	CButton CRLFTerminator;
	CButton customTerminator;
	CEdit customTerminatorEditBox;
	CString customTerminatorValue;
	afx_msg void OnNoTerminatorClicked();
	afx_msg void OnCRTerminatorClicked();
	afx_msg void OnLFTerminatorClicked();
	afx_msg void OnCRLFTerminatorClicked();
	afx_msg void OnCustomTerminatorClicked();
	afx_msg void OnConnectButtonClicked();
	
	CString inputDataBoxValue;
	CEdit inputDataBox;
	CButton sendButton;
	afx_msg void OnSendButtonClicked();

	CEdit outputDataBox;
	CString outputDataBoxValue;
	CButton clearButton;
	afx_msg void OnClearOutputBufferButtonClicked();
	

	CButton DTRControl;
	CButton DSRControl;
	CButton RTSControl;
	CButton CTSControl;
	afx_msg void OnDTRButtonClicked();
	afx_msg void OnRTSButtonClicked();
	
	CButton pingButton;
	afx_msg void OnPingButtonClicked();
};
