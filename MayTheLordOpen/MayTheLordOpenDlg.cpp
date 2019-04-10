#include "stdafx.h"
#include "MayTheLordOpen.h"
#include "MayTheLordOpenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* Class for "About" dialog  */

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


/* Implemenation of event handlers for main window */

CMayTheLordOpenDlg::CMayTheLordOpenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAYTHELORDOPEN_DIALOG, pParent)
	, customTerminatorValue(_T(""))
	, inputDataBoxValue(_T(""))
	, selectedCOMPortName(_T(""))
	, selectedBaudRate(_T(""))
	, selectedDataBits(_T(""))
	, selectedParityBit(_T(""))
	, selectedStopBits(_T(""))
	, selectedHardwareFlowControl(_T(""))
	, outputDataBoxValue(_T(""))
	, terminatorSign("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMayTheLordOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, COMPortSelectionBox);
	DDX_Control(pDX, IDC_RADIO8, flowControlHardware);
	DDX_Control(pDX, IDC_RADIO9, flowControlSoftware);
	DDX_Control(pDX, IDC_RADIO6, flowControlDisabled);
	DDX_Control(pDX, IDC_RADIO1, noTerminator);
	DDX_Control(pDX, IDC_RADIO2, CRTerminator);
	DDX_Control(pDX, IDC_RADIO3, LFTerminator);
	DDX_Control(pDX, IDC_RADIO4, CRLFTerminator);
	DDX_Control(pDX, IDC_RADIO5, customTerminator);
	DDX_Control(pDX, IDC_COMBO7, hardwareFlowControlSelectionBox);
	DDX_Control(pDX, IDC_EDIT1, customTerminatorEditBox);
	DDX_Control(pDX, IDC_COMBO2, baudRateSelection);
	DDX_Control(pDX, IDC_COMBO3, dataBitsSelection);
	DDX_Control(pDX, IDC_COMBO4, parityBitSelection);
	DDX_Control(pDX, IDC_COMBO5, stopBitsSelection);
	DDX_Text(pDX, IDC_EDIT1, customTerminatorValue);
	DDX_Text(pDX, IDC_EDIT3, inputDataBoxValue);
	DDX_Control(pDX, IDC_EDIT3, inputDataBox);
	DDX_Control(pDX, IDC_BUTTON1, connectButton);
	DDX_Control(pDX, IDC_BUTTON2, sendButton);
	DDX_CBString(pDX, IDC_COMBO1, selectedCOMPortName);
	DDX_CBString(pDX, IDC_COMBO2, selectedBaudRate);
	DDX_CBString(pDX, IDC_COMBO3, selectedDataBits);
	DDX_CBString(pDX, IDC_COMBO4, selectedParityBit);
	DDX_CBString(pDX, IDC_COMBO5, selectedStopBits);
	DDX_CBString(pDX, IDC_COMBO7, selectedHardwareFlowControl);
	DDX_Control(pDX, IDC_EDIT4, outputDataBox);
	DDX_Text(pDX, IDC_EDIT4, outputDataBoxValue);
	DDX_Control(pDX, IDC_BUTTON4, clearButton);

	DDX_Control(pDX, IDC_CHECK1, DTRControl);
	DDX_Control(pDX, IDC_CHECK2, DSRControl);
	DDX_Control(pDX, IDC_CHECK3, RTSControl);
	DDX_Control(pDX, IDC_CHECK4, CTSControl);
	DDX_Control(pDX, IDC_BUTTON3, pingButton);
}

BEGIN_MESSAGE_MAP(CMayTheLordOpenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO1, &CMayTheLordOpenDlg::OnCOMPortDropdownCombo)
	ON_BN_CLICKED(IDC_RADIO6, &CMayTheLordOpenDlg::OnFlowControlDisabledClicked)
	ON_BN_CLICKED(IDC_RADIO8, &CMayTheLordOpenDlg::OnFlowControlHardwareClicked)
	ON_BN_CLICKED(IDC_RADIO9, &CMayTheLordOpenDlg::OnFlowControlSoftwareClicked)
	ON_BN_CLICKED(IDC_RADIO1, &CMayTheLordOpenDlg::OnNoTerminatorClicked)
	ON_BN_CLICKED(IDC_RADIO2, &CMayTheLordOpenDlg::OnCRTerminatorClicked)
	ON_BN_CLICKED(IDC_RADIO3, &CMayTheLordOpenDlg::OnLFTerminatorClicked)
	ON_BN_CLICKED(IDC_RADIO4, &CMayTheLordOpenDlg::OnCRLFTerminatorClicked)
	ON_BN_CLICKED(IDC_RADIO5, &CMayTheLordOpenDlg::OnCustomTerminatorClicked)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMayTheLordOpenDlg::OnCOMPortSelected)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMayTheLordOpenDlg::OnBaudRateSelected)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CMayTheLordOpenDlg::OnDataBitsSelected)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CMayTheLordOpenDlg::OnParityBitSelected)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CMayTheLordOpenDlg::OnStopBitsSelected)
	ON_BN_CLICKED(IDC_BUTTON1, &CMayTheLordOpenDlg::OnConnectButtonClicked)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CMayTheLordOpenDlg::OnHardwareFlowControlTypeSelected)
	ON_BN_CLICKED(IDC_BUTTON2, &CMayTheLordOpenDlg::OnSendButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON4, &CMayTheLordOpenDlg::OnClearOutputBufferButtonClicked)
	ON_BN_CLICKED(IDC_CHECK1, &CMayTheLordOpenDlg::OnDTRButtonClicked)
	ON_BN_CLICKED(IDC_CHECK3, &CMayTheLordOpenDlg::OnRTSButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON3, &CMayTheLordOpenDlg::OnPingButtonClicked)
END_MESSAGE_MAP()


BOOL CMayTheLordOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	flowControlDisabled.SetCheck(TRUE);
	noTerminator.SetCheck(TRUE);
	hardwareFlowControlSelectionBox.EnableWindow(FALSE);
	customTerminatorEditBox.EnableWindow(FALSE);
	inputDataBox.EnableWindow(FALSE);
	outputDataBox.EnableWindow(FALSE);
	sendButton.EnableWindow(FALSE);
	clearButton.EnableWindow(FALSE);
	DTRControl.EnableWindow(FALSE);
	DSRControl.EnableWindow(FALSE);
	RTSControl.EnableWindow(FALSE);
	CTSControl.EnableWindow(FALSE);
	pingButton.EnableWindow(FALSE);

	noTerminator.EnableWindow(TRUE);
	CRTerminator.EnableWindow(TRUE);
	LFTerminator.EnableWindow(TRUE);
	CRLFTerminator.EnableWindow(TRUE);
	customTerminator.EnableWindow(TRUE);

	flowControlDisabled.EnableWindow(TRUE);
	flowControlHardware.EnableWindow(TRUE);
	flowControlSoftware.EnableWindow(TRUE);

	COMPortSelectionBox.EnableWindow(TRUE);
	baudRateSelection.EnableWindow(TRUE);
	dataBitsSelection.EnableWindow(TRUE);
	parityBitSelection.EnableWindow(TRUE);
	stopBitsSelection.EnableWindow(TRUE);

	baudRateSelection.AddString(_T("1200"));
	baudRateSelection.AddString(_T("2400"));
	baudRateSelection.AddString(_T("4800"));
	baudRateSelection.AddString(_T("9600"));
	baudRateSelection.AddString(_T("14400"));
	baudRateSelection.AddString(_T("19200"));
	baudRateSelection.AddString(_T("38400"));
	baudRateSelection.AddString(_T("57600"));
	baudRateSelection.AddString(_T("115200"));
	baudRateSelection.SetCurSel(3);

	dataBitsSelection.AddString(_T("7"));
	dataBitsSelection.AddString(_T("8"));
	dataBitsSelection.SetCurSel(1);

	parityBitSelection.AddString(_T("None"));
	parityBitSelection.AddString(_T("Even"));
	parityBitSelection.AddString(_T("Odd"));
	parityBitSelection.SetCurSel(0);

	stopBitsSelection.AddString(_T("1"));
	stopBitsSelection.AddString(_T("2"));
	stopBitsSelection.SetCurSel(0);

	hardwareFlowControlSelectionBox.AddString(_T("RTS/CTS"));
	hardwareFlowControlSelectionBox.AddString(_T("DTR/DSR"));
	hardwareFlowControlSelectionBox.AddString(_T("RTS/CTS(Manual)"));
	hardwareFlowControlSelectionBox.AddString(_T("DTR/DSR(Manual)"));

	connectionStatus = false;

	threadResources.toCTSStatePointer = &CTSControl;
	threadResources.toDSRStatePointer = &DSRControl;
	threadResources.toOutputDataBoxPointer = &outputDataBox;

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMayTheLordOpenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMayTheLordOpenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMayTheLordOpenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnFlowControlDisabledClicked() {
	if (flowControlHardware.GetCheck() == 1 || flowControlSoftware.GetCheck() == 1) {
		flowControlHardware.SetCheck(FALSE);
		flowControlSoftware.SetCheck(FALSE);
		flowControlDisabled.SetCheck(TRUE);
		hardwareFlowControlSelectionBox.EnableWindow(FALSE);
		hardwareFlowControlSelectionBox.SetCurSel(-1);

		serialPort.disableFlowControl();
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnFlowControlHardwareClicked() {
	if (flowControlDisabled.GetCheck() == 1 || flowControlSoftware.GetCheck() == 1) {
		flowControlHardware.SetCheck(TRUE);
		flowControlSoftware.SetCheck(FALSE);
		flowControlDisabled.SetCheck(FALSE);

		hardwareFlowControlSelectionBox.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnHardwareFlowControlTypeSelected()
{
	UpdateData(TRUE);
	if (selectedHardwareFlowControl == _T("DTR/DSR")) serialPort.enableHardwareFlowControl(DTR_DSR_AUTO);
	else if (selectedHardwareFlowControl == _T("RTS/CTS")) serialPort.enableHardwareFlowControl(RTS_CTS_AUTO);
	else if (selectedHardwareFlowControl == _T("DTR/DSR(Manual)")) serialPort.enableHardwareFlowControl(DTR_DSR_MANUAL);
	else if (selectedHardwareFlowControl == _T("RTS/CTS(Manual)")) serialPort.enableHardwareFlowControl(RTS_CTS_MANUAL);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnFlowControlSoftwareClicked() {
	if (flowControlHardware.GetCheck() == 1 || flowControlDisabled.GetCheck() == 1) {
		flowControlHardware.SetCheck(FALSE);
		flowControlSoftware.SetCheck(TRUE);
		flowControlDisabled.SetCheck(FALSE);
		hardwareFlowControlSelectionBox.EnableWindow(FALSE);
		hardwareFlowControlSelectionBox.SetCurSel(-1);

		serialPort.disableFlowControl();
		serialPort.enableSoftwareFlowControl();
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnNoTerminatorClicked()
{
	if (CRTerminator.GetCheck() == 1 || LFTerminator.GetCheck() == 1 || CRLFTerminator.GetCheck() == 1 || customTerminator.GetCheck() == 1) {
		noTerminator.SetCheck(TRUE);
		CRTerminator.SetCheck(FALSE);
		LFTerminator.SetCheck(FALSE);
		CRLFTerminator.SetCheck(FALSE);
		customTerminator.SetCheck(FALSE);
		customTerminatorEditBox.EnableWindow(FALSE);

		terminatorSign = "";
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnCRTerminatorClicked()
{
	if (noTerminator.GetCheck() == 1 || LFTerminator.GetCheck() == 1 || CRLFTerminator.GetCheck() == 1 || customTerminator.GetCheck() == 1) {
		noTerminator.SetCheck(FALSE);
		CRTerminator.SetCheck(TRUE);
		LFTerminator.SetCheck(FALSE);
		CRLFTerminator.SetCheck(FALSE);
		customTerminator.SetCheck(FALSE);
		customTerminatorEditBox.EnableWindow(FALSE);

		terminatorSign = "\r";
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnLFTerminatorClicked()
{
	if (noTerminator.GetCheck() == 1 || CRTerminator.GetCheck() == 1 || CRLFTerminator.GetCheck() == 1 || customTerminator.GetCheck() == 1) {
		noTerminator.SetCheck(FALSE);
		CRTerminator.SetCheck(FALSE);
		LFTerminator.SetCheck(TRUE);
		CRLFTerminator.SetCheck(FALSE);
		customTerminator.SetCheck(FALSE);
		customTerminatorEditBox.EnableWindow(FALSE);

		terminatorSign = "\n";
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnCRLFTerminatorClicked()
{
	if (noTerminator.GetCheck() == 1 || CRTerminator.GetCheck() == 1 || LFTerminator.GetCheck() == 1 || customTerminator.GetCheck() == 1) {
		noTerminator.SetCheck(FALSE);
		CRTerminator.SetCheck(FALSE);
		LFTerminator.SetCheck(FALSE);
		CRLFTerminator.SetCheck(TRUE);
		customTerminator.SetCheck(FALSE);
		customTerminatorEditBox.EnableWindow(FALSE);

		terminatorSign = "\r\n";
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnCustomTerminatorClicked()
{
	if (noTerminator.GetCheck() == 1 || CRTerminator.GetCheck() == 1 || LFTerminator.GetCheck() == 1 || CRLFTerminator.GetCheck() == 1) {
		UpdateData(TRUE);
		noTerminator.SetCheck(FALSE);
		CRTerminator.SetCheck(FALSE);
		LFTerminator.SetCheck(FALSE);
		CRLFTerminator.SetCheck(FALSE);
		customTerminator.SetCheck(TRUE);
		customTerminatorEditBox.EnableWindow(TRUE);
	}
}

/*
*	Methods for events handling can be added
*
*/
void CMayTheLordOpenDlg::serialPortEventLoop(listenerEventResources *pointer)
{
	unsigned long capturedCOMPortEvent;
	
	while ( serialPort.waitForEvent(capturedCOMPortEvent) ) {
		switch (capturedCOMPortEvent)
		{
		case EV_RXCHAR:
			processReceivedData( pointer->toOutputDataBoxPointer );
			break;
		case EV_CTS:
			processCTSStateChange(pointer->toCTSStatePointer);
			break;
		case EV_DSR:
			processDSRStateChange(pointer->toDSRStatePointer);
			break;

		case EV_BREAK:
			//MessageBox(_T("EV_BREAK"), _T("Info"), MB_OK);
			break;

		case EV_ERR:
			//MessageBox(_T("EV_ERR"), _T("Info"), MB_OK);
			break;

		case EV_RING:
			//MessageBox(_T("EV_RING"), _T("Info"), MB_OK);
			break;

		case EV_RLSD:
			//MessageBox(_T("EV_RLSD"), _T("Info"), MB_OK);
			break;

		case EV_RXFLAG:
			//MessageBox(_T("EV_RXFLAG"), _T("Info"), MB_OK);
			break;

		case EV_TXEMPTY:
			//MessageBox(_T("EV_TXEMPTY"), _T("Info"), MB_OK);
			break;
		default:
			//MessageBox(_T("Exiting thread would occur"), _T("Info"), MB_OK);
			break;
		}
	}
}

/*
*	Finished
*
*/
UINT __cdecl CMayTheLordOpenDlg::serialPortEventLoopStatic(LPVOID pParam)
{
	CMayTheLordOpenDlg* dlg = (CMayTheLordOpenDlg*)pParam;
	dlg->serialPortEventLoop( &(dlg->threadResources) );
	return 0;
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::enableCommunicationWindows(void)
{
	outputDataBox.EnableWindow(TRUE);
	inputDataBox.EnableWindow(TRUE);
	sendButton.EnableWindow(TRUE);
	clearButton.EnableWindow(TRUE);
	pingButton.EnableWindow(TRUE);
	DTRControl.EnableWindow(TRUE);
	DSRControl.EnableWindow(TRUE);
	RTSControl.EnableWindow(TRUE);
	CTSControl.EnableWindow(TRUE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::disableCommunicationWindws(void)
{
	inputDataBox.EnableWindow(FALSE);
	outputDataBox.EnableWindow(FALSE);
	sendButton.EnableWindow(FALSE);
	clearButton.EnableWindow(FALSE);
	pingButton.EnableWindow(FALSE);
	DTRControl.EnableWindow(FALSE);
	DSRControl.EnableWindow(FALSE);
	RTSControl.EnableWindow(FALSE);
	CTSControl.EnableWindow(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::changeDialogToConnectionState(void)
{
	COMPortSelectionBox.EnableWindow(FALSE);
	connectionStatus = true;
	connectButton.SetWindowTextW(_T("Disconnect"));
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::changeDialogToDisconnectionState(void)
{
	serialPort.close();
	COMPortSelectionBox.EnableWindow(TRUE);
	COMPortSelectionBox.SetCurSel(-1);
	connectionStatus = false;
	disableCommunicationWindws();
	connectButton.SetWindowTextW(_T("Connect"));
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::processReceivedData(CEdit * toOutputDataBoxPointer)
{
	std::string receivedData;
	CString sWindowText;
	receivedData = serialPort.read();

	toOutputDataBoxPointer->GetWindowText(sWindowText);
	if (receivedData == "+PING") serialPort.write((CStringA)"+OK");
	else if (receivedData == "+OK")
	{
			endTimer = std::chrono::high_resolution_clock::now();
			timeElapsed = (endTimer - startTimer);
			CString durationTimeText;
			durationTimeText.Format(_T("Time elapsed: %f seconds\r\n"), timeElapsed);
			toOutputDataBoxPointer->SetWindowTextW(durationTimeText);
	}
	else toOutputDataBoxPointer->SetWindowTextW((sWindowText + receivedData.c_str()));
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::processCTSStateChange(CButton * toCTSStatePointer)
{
	if (serialPort.getCTSLineState()) toCTSStatePointer->SetCheck(TRUE);
	else toCTSStatePointer->SetCheck(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::processDSRStateChange(CButton * toDSRStatePointer)
{
	if (serialPort.getDSRLineState()) toDSRStatePointer->SetCheck(TRUE);
	else toDSRStatePointer->SetCheck(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnCOMPortDropdownCombo()
{
	std::vector<std::string> avaliablePorts = SerialConnection::scanAvaliableSerialPort();
	COMPortSelectionBox.ResetContent();
	for (unsigned int i = 0; i < avaliablePorts.size(); i++) {
		COMPortSelectionBox.AddString((CString)(avaliablePorts[i].c_str()));
	}
	UpdateData(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnCOMPortSelected()
{
	COMPortSelectionBox.GetLBText(COMPortSelectionBox.GetCurSel(), selectedCOMPortName);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnBaudRateSelected()
{
	UpdateData(TRUE);

	unsigned long baudRate;
	if (selectedBaudRate == _T("1200")) baudRate = CBR_1200;
	else if (selectedBaudRate == _T("2400")) baudRate = CBR_2400;
	else if (selectedBaudRate == _T("4800")) baudRate = CBR_4800;
	else if (selectedBaudRate == _T("9600")) baudRate = CBR_9600;
	else if (selectedBaudRate == _T("14400")) baudRate = CBR_14400;
	else if (selectedBaudRate == _T("19200")) baudRate = CBR_19200;
	else if (selectedBaudRate == _T("38400")) baudRate = CBR_38400;
	else if (selectedBaudRate == _T("57600")) baudRate = CBR_57600;
	else if (selectedBaudRate == _T("115200")) baudRate = CBR_115200;

	if (!serialPort.setBaudRate(baudRate)) {
		changeDialogToDisconnectionState();
		MessageBox(_T("Unable to set serial port parameter!\r\nPlease make sure the choosen port is still avaliable."),
			_T("Serial port out of sync"), MB_ICONERROR);
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnDataBitsSelected()
{
	UpdateData(TRUE);

	unsigned int dataField;
	if (selectedDataBits == _T("7")) dataField = 7;
	else if (selectedDataBits == _T("8")) dataField = 8;

	if (!serialPort.setDataFieldSize(dataField)) {
		changeDialogToDisconnectionState();
		MessageBox(_T("Unable to set serial port parameter!\r\nPlease make sure the choosen port is still avaliable."), 
					_T("Serial port out of sync"), MB_ICONERROR);
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnParityBitSelected()
{
	UpdateData(TRUE);

	unsigned int parityBit;
	if (selectedParityBit == _T("None")) parityBit = NOPARITY;
	else if (selectedParityBit == _T("Even")) parityBit = EVENPARITY;
	else if (selectedParityBit == _T("Odd")) parityBit = ODDPARITY;
	
	if( !serialPort.setParityBitType(parityBit) ) {
		changeDialogToDisconnectionState();
		MessageBox(_T("Unable to set serial port parameter!\r\nPlease make sure the choosen port is still avaliable."),
			_T("Serial port out of sync"), MB_ICONERROR);
	}
		
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnStopBitsSelected()
{
	UpdateData(TRUE);

	unsigned int stopBits;
	if (selectedStopBits == _T("1")) stopBits = 0;
	else if (selectedStopBits == _T("2")) stopBits = 2;

	if (!serialPort.setStopBitsNumber(stopBits)) {
		changeDialogToDisconnectionState();
		MessageBox(_T("Unable to set serial port parameter!\r\nPlease make sure the choosen port is still avaliable."),
			_T("Serial port out of sync"), MB_ICONERROR);
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnConnectButtonClicked()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!connectionStatus) {
		if (!selectedCOMPortName.GetLength()) {
			MessageBox(_T("No serial port was selected!\r\nPort to be opened should be avaliable on the dropdown list."), _T("No Serial Port Selected"), MB_ICONWARNING);
			return;
		}
		
		if (serialPort.open(selectedCOMPortName)) 
		{
			changeDialogToConnectionState();
			enableCommunicationWindows();
			AfxBeginThread(serialPortEventLoopStatic, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		}
		else 
		{
			MessageBox(_T("Unable to open specified serial port!\r\nPlease make sure the choosen port is still avaliable."), _T("Error Opening Serial Port"), MB_ICONERROR);
		}
	}
	else {
		changeDialogToDisconnectionState();
	}
	
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnSendButtonClicked()
{
	UpdateData(TRUE);
	if (customTerminator.GetCheck()) terminatorSign = customTerminatorValue;
	CStringA dataToSend(inputDataBoxValue);	// Convert to UTF-8
	dataToSend.Append(terminatorSign);
	if (serialPort.write(dataToSend)) inputDataBoxValue = _T("");
	else 
	{
		MessageBox(_T("Unable to send data!\r\nPlease make sure the choosen port is still avaliable."), _T("Critical Data Sending Failure"), MB_ICONERROR);
		serialPort.close();
		connectionStatus = false;
		disableCommunicationWindws();
		connectButton.SetWindowTextW(_T("Connect"));
	}
	UpdateData(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnClearOutputBufferButtonClicked()
{
	outputDataBoxValue = (_T(""));
	UpdateData(FALSE);
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnDTRButtonClicked()
{
	UpdateData(TRUE);
	if (serialPort.getHardwareFlowControlType() == DTR_DSR_MANUAL) {
		UpdateData(FALSE);
		if (DTRControl.GetCheck() == 1) {
			DTRControl.SetCheck(FALSE);
			serialPort.setDTRLineState(false);
		}
		else {
			DTRControl.SetCheck(TRUE);
			serialPort.setDTRLineState(true);
		}
		UpdateData(FALSE);
	}	
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnRTSButtonClicked()
{
	UpdateData(TRUE);
	if (serialPort.getHardwareFlowControlType() == RTS_CTS_MANUAL) {
		if (RTSControl.GetCheck() == 1) {
			RTSControl.SetCheck(FALSE);
			serialPort.setRTSLineState(false);
		}
		else {
			RTSControl.SetCheck(TRUE);
			serialPort.setRTSLineState(true);
		}
		UpdateData(FALSE);
	}
}

/*
*	Finished
*
*/
void CMayTheLordOpenDlg::OnPingButtonClicked()
{
	serialPort.write((CStringA)"+PING");
	startTimer = std::chrono::high_resolution_clock::now();
}
