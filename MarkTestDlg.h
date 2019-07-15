#include <pylon\PylonIncludes.h>
#include <pylon\gige\BaslerGigECamera.h>
#include <pylon\PylonGUI.h>
#include <pylon\InstantCamera.h>

#include "CvvImage.h"
#include "afxwin.h"

#pragma once

//CMarkTestDlg �Ի���
void ThreadFunc();
//BOOL PreTranslateMessage(MSG* pMsg) 
class CMarkTestDlg : public CDialogEx
{
// ����
public:
	CMarkTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMarkTestDlg();
// �Ի�������
	enum { IDD = IDD_MARKTEST_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
public:  /*****PLC******/
	HINSTANCE hDMTDll;
	int  (WINAPI *RequestData)(int comm_type, int conn_num, int slave_addr, int func_code,const unsigned char *sendbuf, int sendlen);
	int  (WINAPI *ResponseData)(int comm_type, int conn_num, int *slave_addr, int *func_code,unsigned char *recvbuf);
	int  (WINAPI *OpenModbusSerial)(int conn_num, int baud_rate,int data_len, char parity,int stop_bits, int modbus_mode);
	void (WINAPI *CloseSerial)(int conn_num);
	int  (WINAPI *GetLastSerialErr)();
	void (WINAPI *ResetSerialErr)();

	unsigned  int TestStep;
	unsigned char readData;
	CString    sendData;
	unsigned char *receiveData;
	long    XData,YData;
	bool  geneResultFlag;
	bool  doubleResultFlag;
	bool  testResultFlag;

	void InitialPLC();  /***PLC***/
	bool InitialSerial();
	void CloseCom(int com);
	BOOL SerialReadData(CString strReqData,unsigned char * ReadData);
	BOOL SerialWriteData(CString strReqData);
	/***********/

// ʵ��
protected:
	HICON m_hIcon;
	HANDLE hThread;
	DWORD ThreadID;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton4();
	CString Y_Distance;
	CString L_X_Distance;
	CString R_X_Distance;
	afx_msg void OnBnClickedButton5();
	IplImage* CMarkTestDlg::imfill(IplImage* src);
	CString Delta_Y;
	CString Delta_X;
	afx_msg void OnBnClickedButton6();
	CString Stand_Y;
	CString Stand_X;
	CString Delt_Y;
	CString Delt_X;
	afx_msg void OnBnClickedButton7();
	virtual BOOL PreTranslateMessage(MSG* pMsg);  //*********
	CString Panel_ID;
	CString Angle;
	CString Bvalue;
	CString Avalue;
	CString Cvalue;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton12();
};
