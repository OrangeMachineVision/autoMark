#include "stdafx.h"
#include "MarkTest.h"
#include "MarkTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

/*文件头 basler申明*/
using namespace Pylon;
#include <pylon/gige/BaslerGigECamera.h>
typedef Pylon::CBaslerGigECamera Camera_t;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;

CRect rect;    //左
CDC *pDC; 
HDC hDC; 
CWnd *pwnd;
CRect rect1;   //右
CDC *pDC1;
HDC hDC1;
CWnd *pwnd1;
CRect rect2;   //中
CDC *pDC2;
HDC hDC2;
CWnd *pwnd2;
CDC *pDC3;    //result
HDC hDC3;
CWnd *pwnd3;
CRect rect3;
int count1;
int cameraFlag;
volatile BOOL m_bRun;
int light;
int YuzhiTestL;
int YuzhiTestR;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CMarkTestDlg::CMarkTestDlg(CWnd* pParent): CDialogEx(CMarkTestDlg::IDD, pParent)
	, Y_Distance(_T(""))
	, L_X_Distance(_T(""))
	, R_X_Distance(_T(""))
	, Delta_Y(_T(""))
	, Delta_X(_T(""))
	, Stand_Y(_T(""))
	, Stand_X(_T(""))
	, Delt_Y(_T(""))
	, Delt_X(_T(""))
	, Panel_ID(_T(""))
	, Angle(_T(""))
	, Avalue(_T(""))
	, Cvalue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMarkTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, Y_Distance);
	DDX_Text(pDX, IDC_EDIT5, L_X_Distance);
	DDX_Text(pDX, IDC_EDIT6, Delta_Y);
	DDX_Text(pDX, IDC_EDIT10, Delta_X);
	DDX_Text(pDX, IDC_EDIT11, Stand_Y);
	DDX_Text(pDX, IDC_EDIT12, Stand_X);
	DDX_Text(pDX, IDC_EDIT13, Delt_Y);
	DDX_Text(pDX, IDC_EDIT14, Delt_X);
	DDX_Text(pDX, IDC_EDIT15, Panel_ID);
	DDX_Text(pDX, IDC_EDIT16, Angle);
	DDX_Text(pDX, IDC_EDIT17, Bvalue);
	DDX_Text(pDX, IDC_EDIT18, Avalue);
	DDX_Text(pDX, IDC_EDIT19, Cvalue);
}

BEGIN_MESSAGE_MAP(CMarkTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMarkTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMarkTestDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CMarkTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMarkTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMarkTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMarkTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMarkTestDlg::OnBnClickedButton8)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON9, &CMarkTestDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMarkTestDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMarkTestDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON3, &CMarkTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON12, &CMarkTestDlg::OnBnClickedButton12)
END_MESSAGE_MAP()

BOOL CMarkTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	pwnd = GetDlgItem(IDC_VIDEO1);   //左
	pDC =pwnd->GetDC();
	hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	pwnd1 = GetDlgItem(IDC_VIDEO2);  //右
	pDC1 =pwnd1->GetDC();
	hDC1= pDC1->GetSafeHdc();
	pwnd1->GetClientRect(&rect1);
	pwnd2 = GetDlgItem(IDC_VIDEO3);  //中
	pDC2 =pwnd2->GetDC();
	hDC2= pDC2->GetSafeHdc();
	pwnd2->GetClientRect(&rect2);
	pwnd3 = GetDlgItem(IDC_Image);  //result
	pDC3 =pwnd3->GetDC();
	hDC3= pDC3->GetSafeHdc();
	pwnd3->GetClientRect(&rect3);

	((CEdit*)GetDlgItem(IDC_EDIT15))->SetLimitText(10); //限制 文本框字数
	TestStep=0;
	count1=0;
	cameraFlag=0;
	light=0;
	YuzhiTestL=10;
    YuzhiTestR=10;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMarkTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
CMarkTestDlg::~CMarkTestDlg()  //析构函数
{
	m_bRun = FALSE; 
	Sleep(1000);
	CDialogEx::OnClose();
}
void CMarkTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标 
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//pDC3->StretchBlt(rect3.left,rect3.top,rect3.Width(),rect3.Height(),&MemDC,0,0,160,90,SRCCOPY); 
		CDialogEx::OnPaint();
	}
}

HCURSOR CMarkTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



IplImage* rawImage;IplImage* rawImage1;IplImage* rawImage2;
IplImage* rawImage0;IplImage* rawImage10;IplImage* rawImage20;
IplImage* rawImagew;IplImage* rawImage1w;IplImage* rawImage2w;

CvvImage m_CvvImage;CvvImage m_CvvImage1;CvvImage m_CvvImage2;
string  str,str1,str2;
IplImage* ipl;
IplImage* ipl1;
IplImage* ipl2;
CvPoint pt10,pt20;
CvPoint pt11,pt21;
CvPoint pt12,pt22;
CvPoint pt10s,pt20s;
CvPoint pt11s,pt21s;
CvPoint pt12s,pt22s;
int arr1[1]; int arr2[1];           
CvPoint  *ptzx; CvPoint  *ptzy;
CvPoint Rect_a1[2];CvPoint Rect_a2[2];
CvPoint  *ptzx1; CvPoint  *ptzy1;
CvPoint Rect_a11[2];CvPoint Rect_a21[2];
CvPoint  *ptzx2; CvPoint  *ptzy2;
CvPoint Rect_a12[2];CvPoint Rect_a22[2];
double Bili_L;double Bili_R;double Bili_C;
double Panel_Y;double Panel_X;double B;
int sxais;int syais;
int sxais1;int syais1;
int sxais2;int syais2;

int baojing;
double sPanel_Y;double sPanel_X;
IplImage* sipl;
IplImage* sipl1;
IplImage* sipl2;
CvPoint ptmin,ptmax;
CvPoint ptminw,ptmaxw;

int qiehuan;
CString PanelID;
CString jieguo;
int Lxais;int Lyais;int Rxais;int Ryais;
double Xpianyi;double Ypianyi;


void ThreadProc() //线程函数  
{  
	cameraFlag=1;
	arr1[0]=2; 
	arr2[0]=2;
	Pylon::PylonAutoInitTerm autoInitTerm; //初始化相机
	CTlFactory& TlFactory = CTlFactory::GetInstance();
	ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());
	DeviceInfoList_t devices;
	if (3!= pTl->EnumerateDevices(devices))
	{AfxMessageBox("三目视觉系统初始化失败");return;}

	/*1*/
	Camera_t Camera(pTl->CreateDevice(devices[0]));
	str = Camera.GetDeviceInfo().GetSerialNumber(); //获取相机序列号

	Camera.Open();
	Camera_t::StreamGrabber_t StreamGrabber(Camera.GetStreamGrabber(0));
	StreamGrabber.Open();
	Camera.PixelFormat.SetValue(PixelFormat_Mono8);
	Camera.OffsetX.SetValue(0);
	Camera.OffsetY.SetValue(0);
	Camera.Width.SetValue(Camera.Width.GetMax());
	Camera.Height.SetValue(Camera.Height.GetMax());
	int Width=(int)(Camera.Width.GetMax());
	int Height=(int)(Camera.Height.GetMax());
	GenApi::IEnumEntry* acquisitionStart = Camera.TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
	if ( acquisitionStart && GenApi::IsAvailable( acquisitionStart))
	{
		Camera.TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
		Camera.TriggerMode.SetValue( TriggerMode_Off);
	}
	GenApi::IEnumEntry* frameStart = Camera.TriggerSelector.GetEntry( TriggerSelector_FrameStart);
	if ( frameStart && GenApi::IsAvailable( frameStart))
	{
		Camera.TriggerSelector.SetValue( TriggerSelector_FrameStart);
		Camera.TriggerMode.SetValue( TriggerMode_Off);
	}
	Camera.AcquisitionMode.SetValue(AcquisitionMode_SingleFrame);
	Camera.ExposureMode.SetValue(ExposureMode_Timed);
	if(str=="21587499")
	Camera.ExposureTimeRaw.SetValue(3185);
	if(str=="21587498")
	Camera.ExposureTimeRaw.SetValue(3185); //曝光时间
	//Camera.ExposureTimeRaw.SetValue(3185); //曝光时间

	const size_t ImageSize = (size_t)(Camera.PayloadSize.GetValue());
	uint8_t * const pBuffer = new uint8_t[ ImageSize ];
	StreamGrabber.MaxBufferSize.SetValue(ImageSize);
	StreamGrabber.MaxNumBuffer.SetValue(1);
	StreamGrabber.PrepareGrab();
	const StreamBufferHandle hBuffer =StreamGrabber.RegisterBuffer(pBuffer, ImageSize);
	StreamGrabber.QueueBuffer(hBuffer, NULL);

	/*2*/
	Camera_t Camera1(pTl->CreateDevice(devices[1]));
	str1 = Camera1.GetDeviceInfo().GetSerialNumber();

	Camera1.Open();
	Camera_t::StreamGrabber_t StreamGrabber1(Camera1.GetStreamGrabber(0));
	StreamGrabber1.Open();
	Camera1.PixelFormat.SetValue(PixelFormat_Mono8);
	Camera1.OffsetX.SetValue(0);
	Camera1.OffsetY.SetValue(0);
	Camera1.Width.SetValue(Camera1.Width.GetMax());
	Camera1.Height.SetValue(Camera1.Height.GetMax());
	int Width1=(int)(Camera1.Width.GetMax());
	int Height1=(int)(Camera1.Height.GetMax());
	GenApi::IEnumEntry* acquisitionStart1 = Camera1.TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
	if ( acquisitionStart1 && GenApi::IsAvailable( acquisitionStart1))
	{
		Camera1.TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
		Camera1.TriggerMode.SetValue( TriggerMode_Off);
	}
	GenApi::IEnumEntry* frameStart1 = Camera1.TriggerSelector.GetEntry( TriggerSelector_FrameStart);//
	if ( frameStart1 && GenApi::IsAvailable( frameStart1))
	{
		Camera1.TriggerSelector.SetValue( TriggerSelector_FrameStart);
		Camera1.TriggerMode.SetValue( TriggerMode_Off);
	}
	Camera1.AcquisitionMode.SetValue(AcquisitionMode_SingleFrame);
	Camera1.ExposureMode.SetValue(ExposureMode_Timed);
	
	if(str1=="21587499")
	Camera1.ExposureTimeRaw.SetValue(3185); 
	if(str1=="21587498")
	Camera1.ExposureTimeRaw.SetValue(3185); //曝光时间
	//Camera1.ExposureTimeRaw.SetValue(3185); //曝光时间
	
	const size_t ImageSize1 = (size_t)(Camera1.PayloadSize.GetValue());
	uint8_t * const pBuffer1 = new uint8_t[ ImageSize1 ];
	StreamGrabber1.MaxBufferSize.SetValue(ImageSize1);
	StreamGrabber1.MaxNumBuffer.SetValue(1);
	StreamGrabber1.PrepareGrab();
	const StreamBufferHandle hBuffer1 =StreamGrabber1.RegisterBuffer(pBuffer1, ImageSize1);
	StreamGrabber1.QueueBuffer(hBuffer1, NULL);

	/*3*/
	Camera_t Camera2(pTl->CreateDevice(devices[2]));
	str2= Camera2.GetDeviceInfo().GetSerialNumber();

	Camera2.Open();
	Camera_t::StreamGrabber_t StreamGrabber2(Camera2.GetStreamGrabber(0));
	StreamGrabber2.Open();
	Camera2.PixelFormat.SetValue(PixelFormat_Mono8);
	Camera2.OffsetX.SetValue(0);
	Camera2.OffsetY.SetValue(0);
	Camera2.Width.SetValue(Camera2.Width.GetMax());
	Camera2.Height.SetValue(Camera2.Height.GetMax());
	int Width2=(int)(Camera2.Width.GetMax());
	int Height2=(int)(Camera2.Height.GetMax());
	GenApi::IEnumEntry* acquisitionStart2 = Camera2.TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
	if ( acquisitionStart2 && GenApi::IsAvailable( acquisitionStart2))
	{
		Camera2.TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
		Camera2.TriggerMode.SetValue( TriggerMode_Off);
	}
	GenApi::IEnumEntry* frameStart2 = Camera2.TriggerSelector.GetEntry( TriggerSelector_FrameStart);//
	if ( frameStart2 && GenApi::IsAvailable( frameStart2))
	{
		Camera2.TriggerSelector.SetValue( TriggerSelector_FrameStart);
		Camera2.TriggerMode.SetValue( TriggerMode_Off);
	}
	Camera2.AcquisitionMode.SetValue(AcquisitionMode_SingleFrame);
	Camera2.ExposureMode.SetValue(ExposureMode_Timed);

	if(str2=="21587499")
	Camera2.ExposureTimeRaw.SetValue(3185); 
	if(str2=="21587498")
	Camera2.ExposureTimeRaw.SetValue(3185); //曝光时间
	//Camera2.ExposureTimeRaw.SetValue(3185); //曝光时间

	const size_t ImageSize2 = (size_t)(Camera2.PayloadSize.GetValue());
	uint8_t * const pBuffer2 = new uint8_t[ ImageSize2 ];
	StreamGrabber2.MaxBufferSize.SetValue(ImageSize2);
	StreamGrabber2.MaxNumBuffer.SetValue(2);
	StreamGrabber2.PrepareGrab();
	const StreamBufferHandle hBuffer2 =StreamGrabber2.RegisterBuffer(pBuffer2, ImageSize2);
	StreamGrabber2.QueueBuffer(hBuffer2, NULL);

	rawImage = cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,1);
	rawImage1 = cvCreateImage(cvSize(Width1,Height1),IPL_DEPTH_8U,1);
	rawImage2 = cvCreateImage(cvSize(Width2,Height2),IPL_DEPTH_8U,1);
	
	rawImage0 = cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,3);
	rawImage10 = cvCreateImage(cvSize(Width1,Height1),IPL_DEPTH_8U,3);
	rawImage20 = cvCreateImage(cvSize(Width2,Height2),IPL_DEPTH_8U,3);

	rawImagew = cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,3);
	rawImage1w = cvCreateImage(cvSize(Width1,Height1),IPL_DEPTH_8U,3);
	rawImage2w = cvCreateImage(cvSize(Width2,Height2),IPL_DEPTH_8U,3);

	m_bRun = TRUE;  
	
	while(m_bRun)  
	{
		/*1*/
		if(str=="21620157")
		{
			if(light==0)
				Camera.ExposureTimeRaw.SetValue(980);
			else
				Camera.ExposureTimeRaw.SetValue(16485);
		}
		Camera.AcquisitionStart.Execute();
		StreamGrabber.GetWaitObject().Wait(3000);
		/*if (StreamGrabber.GetWaitObject().Wait(3000))
		{*/
			GrabResult Result;
			StreamGrabber.RetrieveResult(Result);
			/*if (Result.Succeeded())
			{*/
				const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();//图像裸数据
				cvSetData(rawImage,(char*)pImageBuffer,rawImage->width);  //相机图像 转换至 OpenCV数据
				cvCvtColor( rawImage, rawImage0, CV_GRAY2BGR );           //转RGB，可显示彩框
				cvCvtColor( rawImage, rawImagew, CV_GRAY2BGR );
				
				if(str=="21587499")//左
				{
					if(qiehuan==2)
						cvRectangle(rawImage0, pt10, pt20, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
						cvRectangle(rawImage0, ptmin, ptmax, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage0, pt10s, pt20s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage0, &ptzx, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage0, &ptzy, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage.CopyOf(rawImage0,1);
					m_CvvImage.DrawToHDC(hDC, &rect);
				}

				if(str=="21587498")//右
				{
					if(qiehuan==2)
						cvRectangle(rawImage0, pt11, pt21, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
						cvRectangle(rawImage0, ptminw, ptmaxw, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage0, pt11s, pt21s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage0, &ptzx1, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage0, &ptzy1, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage1.CopyOf(rawImage0,1);
					m_CvvImage1.DrawToHDC(hDC1, &rect1);
				}

				if(str=="21620157")//中
				{
					if(qiehuan==1)
					{
						cvRectangle(rawImage0, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);
					}
					if(qiehuan==2)
						cvRectangle(rawImage0, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage0, pt12s, pt22s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage0, &ptzx2, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage0, &ptzy2, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage2.CopyOf(rawImage0,2);
					m_CvvImage2.DrawToHDC(hDC2, &rect2);
				}

				StreamGrabber.QueueBuffer(Result.Handle(), NULL);
			/*}
			else
			{
				StreamGrabber.FinishGrab();
				StreamGrabber.Close();
				Camera.Close();
				delete[] pBuffer;
				break;
			}*/
		/*}
		else
		{
			StreamGrabber.FinishGrab();
			StreamGrabber.Close();
			Camera.Close();
			delete[] pBuffer;
			break;
		}*/
		
		/*2*/
		if(str1=="21620157")
		{
			if(light==0)
				Camera1.ExposureTimeRaw.SetValue(980);
			else
				Camera1.ExposureTimeRaw.SetValue(16485);
		}
		Camera1.AcquisitionStart.Execute();
		StreamGrabber1.GetWaitObject().Wait(3000);
		/*if (StreamGrabber1.GetWaitObject().Wait(3000))
		{*/
			GrabResult Result1;
			StreamGrabber1.RetrieveResult(Result1);
			/*if (Result1.Succeeded())
			{*/
				const uint8_t *pImageBuffer1 = (uint8_t *) Result1.Buffer();
				cvSetData(rawImage1,(char*)pImageBuffer1,rawImage1->width);
				cvCvtColor( rawImage1, rawImage10, CV_GRAY2BGR );
				cvCvtColor( rawImage1, rawImage1w, CV_GRAY2BGR );

				if(str1=="21587499")//左
				{
					if(qiehuan==2)
						cvRectangle(rawImage10, pt10, pt20, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
						cvRectangle(rawImage10, ptmin, ptmax, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage10, pt10s, pt20s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage10, &ptzx, arr1, 1, 1, CV_RGB(255,0,0), 10); //
						cvPolyLine(rawImage10, &ptzy, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage.CopyOf(rawImage10,1);
					m_CvvImage.DrawToHDC(hDC, &rect);
				}

				if(str1=="21587498")//右
				{
					if(qiehuan==2)
						cvRectangle(rawImage10, pt11, pt21, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
						cvRectangle(rawImage10, ptminw, ptmaxw, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage10, pt11s, pt21s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage10, &ptzx1, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage10, &ptzy1, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage1.CopyOf(rawImage10,1);
					m_CvvImage1.DrawToHDC(hDC1, &rect1);
				}

				if(str1=="21620157")//中
				{
					if(qiehuan==1)
					{
						cvRectangle(rawImage10, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);
					}
					if(qiehuan==2)
					{cvRectangle(rawImage10, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);}
					if(qiehuan==0)
					{
						cvRectangle(rawImage10, pt12s, pt22s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage10, &ptzx2, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage10, &ptzy2, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage2.CopyOf(rawImage10,2);
					m_CvvImage2.DrawToHDC(hDC2, &rect2);
				}

				StreamGrabber1.QueueBuffer(Result1.Handle(), NULL);
			/*}
			else
			{
				StreamGrabber1.FinishGrab();
				StreamGrabber1.Close();
				Camera1.Close();
				delete[] pBuffer1;
				break;
			}
		}
		else
		{
			StreamGrabber1.FinishGrab();
			StreamGrabber1.Close();
			Camera1.Close();
			delete[] pBuffer1;
			break;
		}*/

		/*3*/
		if(str2=="21620157")
		{
			if(light==0)
				Camera2.ExposureTimeRaw.SetValue(980);
			else
				Camera2.ExposureTimeRaw.SetValue(16485);
		}
		Camera2.AcquisitionStart.Execute();
		StreamGrabber2.GetWaitObject().Wait(3000);
		/*if (StreamGrabber2.GetWaitObject().Wait(3000))
		{*/
			GrabResult Result2;
			StreamGrabber2.RetrieveResult(Result2);
			/*if (Result2.Succeeded())
			{*/
				const uint8_t *pImageBuffer2 = (uint8_t *) Result2.Buffer();
				cvSetData(rawImage2,(char*)pImageBuffer2,rawImage2->width);

				cvCvtColor( rawImage2, rawImage20, CV_GRAY2BGR );
				cvCvtColor( rawImage2, rawImage2w, CV_GRAY2BGR );
				
				if(str2=="21587499")//左
				{
					if(qiehuan==2)
					cvRectangle(rawImage20, pt10, pt20, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
					cvRectangle(rawImage20, ptmin, ptmax, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage20, pt10s, pt20s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage20, &ptzx, arr1, 1, 1, CV_RGB(255,0,0), 10); //
						cvPolyLine(rawImage20, &ptzy, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage.CopyOf(rawImage20,1);
					m_CvvImage.DrawToHDC(hDC, &rect);
				}

				if(str2=="21587498")//右
				{
					if(qiehuan==2)
					cvRectangle(rawImage20, pt11, pt21, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==1)
					cvRectangle(rawImage20, ptminw, ptmaxw, CV_RGB(0,0,255),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage20, pt11s, pt21s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage20, &ptzx1, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage20, &ptzy1, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage1.CopyOf(rawImage20,1);
					m_CvvImage1.DrawToHDC(hDC1, &rect1);
				}

				if(str2=="21620157")//中
				{
					if(qiehuan==1)
					{
						cvRectangle(rawImage20, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);
					}
					if(qiehuan==2)
					cvRectangle(rawImage20, pt12, pt22, CV_RGB(0,255,0),2, CV_AA,0);
					if(qiehuan==0)
					{
						cvRectangle(rawImage20, pt12s, pt22s, CV_RGB(255,0,0),2, CV_AA,0);
						cvPolyLine(rawImage20, &ptzx2, arr1, 1, 1, CV_RGB(255,0,0), 10); 
						cvPolyLine(rawImage20, &ptzy2, arr2, 1, 1, CV_RGB(255,0,0), 10); 
					}
					m_CvvImage2.CopyOf(rawImage20,2);
					m_CvvImage2.DrawToHDC(hDC2, &rect2);
				}

				StreamGrabber2.QueueBuffer(Result2.Handle(), NULL);
			/*}
			else
			{
				StreamGrabber2.FinishGrab();
				StreamGrabber2.Close();
				Camera2.Close();
				delete[] pBuffer2;
				break;
			}
		}
		else
		{
			StreamGrabber2.FinishGrab();
			StreamGrabber2.Close();
			Camera2.Close();
			delete[] pBuffer2;
			break;
		}*/
	
	}

	if(!m_bRun)
	{
		StreamGrabber.FinishGrab();
		StreamGrabber.Close();
		Camera.Close();
		delete[] pBuffer;

		StreamGrabber1.FinishGrab();
		StreamGrabber1.Close();
		Camera1.Close();
		delete[] pBuffer1;

		StreamGrabber2.FinishGrab();
		StreamGrabber2.Close();
		Camera2.Close();
		delete[] pBuffer2;
	}

}  

void CMarkTestDlg::OnBnClickedButton1()
{
	if(cameraFlag==0)
	{
		qiehuan=0;
		hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,NULL,0,&ThreadID);  
	}
}

void CMarkTestDlg::OnBnClickedButton2()
{
	m_bRun = FALSE; 
}

void CMarkTestDlg::OnClose()
{
	OnBnClickedButton2();
	Sleep(1000);
	CDialogEx::OnClose();
}

/***Panel目标定位***/

void CMarkTestDlg::OnBnClickedButton4()
{
	light=1;
	count1++;
	char ch2[20];
	GetDlgItem(IDC_EDIT15)->GetWindowText(ch2,20);
	Panel_ID=ch2;

	if(Panel_ID=="")
	{
		AfxMessageBox("请扫描Panel的ID号，谢谢");
		return;
	}
	else
	{
		UpdateData(TRUE);
		baojing=0;
		qiehuan=2;

		if(str=="21587499")ipl=cvCloneImage(rawImage);
		if(str1=="21587499")ipl=cvCloneImage(rawImage1);
		if(str2=="21587499")ipl=cvCloneImage(rawImage2);

		if(str=="21587498")ipl1=cvCloneImage(rawImage);
		if(str1=="21587498")ipl1=cvCloneImage(rawImage1);
		if(str2=="21587498")ipl1=cvCloneImage(rawImage2);

		if(str=="21620157")ipl2=cvCloneImage(rawImagew);
		if(str1=="21620157")ipl2=cvCloneImage(rawImage1w);
		if(str2=="21620157")ipl2=cvCloneImage(rawImage2w);

		if(ipl&&ipl1&&ipl2)
		{
			LoopTestL:
			IplImage *src;  /*左*/
			cvSetImageROI(ipl, cvRect(ipl->width/4, ipl->height/4, ipl->width/2, ipl->height/2)); //connect识别
			src = cvCreateImage(cvGetSize(ipl),ipl->depth,ipl->nChannels);
			cvCopy(ipl, src, NULL);
			cvResetImageROI(ipl);

			IplImage* ROI_erzhi = cvCreateImage( cvGetSize(src), 8, 1 ); 
			cvThreshold( src, ROI_erzhi,YuzhiTestL,255,CV_THRESH_BINARY);//110
			cvNamedWindow( "L", 0 );
			cvResizeWindow("L",cvGetSize(src).width*0.2,cvGetSize(src).height*0.2);
			cvShowImage("L",ROI_erzhi);cvWaitKey(1);
			
			/*cvNamedWindow("test-l",0);  
			cvResizeWindow("test-l",cvGetSize(ROI_erzhi).width*0.2,cvGetSize(ROI_erzhi).height*0.2);
			cvShowImage("test-l",ROI_erzhi); */

			CvMemStorage *storage=cvCreateMemStorage();
			CvContourScanner scanner=NULL;
			CvSeq *contour=0;
			double tmparea=0.0; 
			double xinzhaung10=0.0;
			double xinzhaung20=0.0;
			CvRect rect;
			int xmin=10000;
			int ymin=10000;
			int xmax=0;
			int ymax=0;
			int xais=0;
			int yais=0;
			scanner=cvStartFindContours(ROI_erzhi,storage,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour=cvFindNextContour(scanner))
			{
				tmparea=fabs(cvContourArea(contour));//面积
				rect=cvBoundingRect(contour,0);
				xinzhaung10=rect.height/(double)rect.width;
				xinzhaung20=tmparea/(rect.height*rect.width);
				if (12000>tmparea&&tmparea>8000)
				if(0.9<xinzhaung10&&xinzhaung10<1.1)
				if(0.4<xinzhaung20&&xinzhaung20<0.545)
				if(rect.y>200)
				{
					if(xmin>rect.x&&rect.x>0)
					{xmin=rect.x;}
					if(ymin>rect.y&&rect.y>0)
					{ymin=rect.y;}
					if(xmax<rect.x+rect.width&&rect.x+rect.width<=cvGetSize(ipl).width)
					{xmax=rect.x+rect.width;}
					if(ymax<rect.y+rect.height&&(rect.y+rect.height)<=cvGetSize(ipl).height)
					{ymax=rect.y+rect.height;}
				}
			}
			contour = cvEndFindContours(&scanner); 
			pt10 = cvPoint(ipl->width/4+xmin, ipl->height/4+ymin), pt20 = cvPoint(ipl->width/4+xmax, ipl->height/4+ymax);
			xais=(ipl->width/4+xmin+ipl->width/4+xmax)/2;yais=(ipl->height/4+ymin+ipl->height/4+ymax)/2;
			if ((xais==5000)||(yais==5000)||((xmax-xmin)*(ymax-ymin)>100000))
			{
				YuzhiTestL=YuzhiTestL+20;
				if(YuzhiTestL<256)
				{
					cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&src);
					cvReleaseMemStorage(&storage);
					storage=NULL;
					goto LoopTestL;
				}
				else
				{
					testResultFlag=true;
					baojing=1;
					AfxMessageBox("Panel左上Mark定位失败，请调整Panel位置或者清理Mark表面后重新TEST，谢谢");
					/*cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&ipl);
					cvReleaseImage(&ipl1);
					cvReleaseImage(&ipl2);
					cvReleaseImage(&src);*/
					/*YuzhiTestL=10;
					YuzhiTestR=10;*/
					//return;
			    }	
			 }
			//YuzhiTestL=10;

			LoopTestR:
			IplImage *src1;	/*右*/
			cvSetImageROI(ipl1, cvRect(ipl1->width/3, ipl1->height/5, ipl1->width/2, ipl1->height/2)); //connect识别  **
			src1 = cvCreateImage(cvGetSize(ipl1),ipl1->depth,ipl1->nChannels);
			cvCopy(ipl1, src1, NULL);
			cvResetImageROI(ipl1);

			IplImage* ROI_erzhi1 = cvCreateImage( cvGetSize(src1), 8, 1 ); 
			cvThreshold( src1, ROI_erzhi1,YuzhiTestR,255,CV_THRESH_BINARY);//60

			CvMemStorage *storage1=cvCreateMemStorage();
			CvContourScanner scanner1=NULL;
			CvSeq *contour1=0;
			double tmparea1=0.0; 
			double xinzhaung1=0.0;
			double xinzhaung2=0.0;
			CvRect rect1;
			int xmin1=10000;
			int ymin1=10000;
			int xmax1=0;
			int ymax1=0;
			int xais1=0;
			int yais1=0;
			scanner1=cvStartFindContours(ROI_erzhi1,storage1,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour1=cvFindNextContour(scanner1))
			{
				tmparea1=fabs(cvContourArea(contour1));//面积
				rect1=cvBoundingRect(contour1,0);
				xinzhaung1=rect1.height/(double)rect1.width;
				xinzhaung2=tmparea1/(rect1.height*rect1.width);
				if (12000>tmparea1&&tmparea1>8000)
				if(0.9<xinzhaung1&&xinzhaung1<1.1)
				if(0.4<xinzhaung2&&xinzhaung2<0.545)
				{
					if(rect1.x<1000)
					{
					if(xmin1>rect1.x&&rect1.x>0)
					{xmin1=rect1.x;}
					if(ymin1>rect1.y&&rect1.y>0)
					{ymin1=rect1.y;}
					if(xmax1<rect1.x+rect1.width&&rect1.x+rect1.width<=cvGetSize(ipl1).width)
					{xmax1=rect1.x+rect1.width;}
					if(ymax1<rect1.y+rect1.height&&(rect1.y+rect1.height)<=cvGetSize(ipl1).height)
					{ymax1=rect1.y+rect1.height;}
					}
				}
			}
			contour1 = cvEndFindContours(&scanner1); 
			pt11 = cvPoint(xmin1+ipl1->width/3, ipl1->height/5+ymin1), pt21 = cvPoint(ipl1->width/3+xmax1, ipl1->height/5+ymax1);
			xais1=(ipl1->width/3+xmin1+ipl1->width/3+xmax1)/2;yais1=(ipl1->height/5+ymin1+ipl1->height/5+ymax1)/2;
			if ((xais1==5000)||(yais1==5000)||((xmax1-xmin1)*(ymax1-ymin1)>100000))
			{
				YuzhiTestR=YuzhiTestR+20;
				if(YuzhiTestR<256)
				{
					cvReleaseImage(&ROI_erzhi1);
					cvReleaseImage(&src1);
					cvReleaseMemStorage(&storage1);
					storage1=NULL;
					goto LoopTestR;
				}
				else
				{
					testResultFlag=true;
					baojing=1;
					AfxMessageBox("Panel右上Mark定位失败，请调整Panel位置或者清理Mark表面后重新TEST，谢谢");
					/*cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&ipl);
					cvReleaseImage(&ROI_erzhi1);
					cvReleaseImage(&ipl1);
					cvReleaseImage(&ipl2);
					cvReleaseImage(&src1);
					cvReleaseImage(&src);*/
					/*YuzhiTestL=10;
					YuzhiTestR=10;*/
					//return;
			    }	
			}
			//YuzhiTestR=10;
			int xais2=0;/***新方法***/
			int yais2=0;
			int xmin2=10000;
			int ymin2=10000;
			int xmax2=0;
			int ymax2=0;
			IplImage *temp1, *ftmp;//*src2,
			//int w=ipl2->width;int h=ipl2->height;
			/*cvSetImageROI(ipl2, cvRect(0, 0, ipl2->width, ipl2->height));
			src2 = cvCreateImage(cvGetSize(ipl2),8, 3);
			cvCopy(ipl2, src2, NULL);
			cvResetImageROI(ipl2);*/
			temp1 = cvLoadImage("pc1.bmp");
  
			int iwidth = ipl2->width - temp1->width + 1;  
			int iheight = ipl2->height - temp1->height + 1;  
  
			ftmp = cvCreateImage(cvSize(iwidth, iheight), 32, 1);  
      
			double min_val;  
			double max_val;  
			CvPoint min_loc;  
			CvPoint max_loc;  
  
			cvMatchTemplate(ipl2, temp1, ftmp, 0);  
			cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);  
			//cvRectangle(ipl2, cvPoint(min_loc.x, min_loc.y),cvPoint((min_loc.x + temp1->width),(min_loc.y + temp1->height)), CV_RGB(0,255,0), 10);  
			xmin2=min_loc.x;
			ymin2=min_loc.y;
			xmax2=min_loc.x + temp1->width;
			ymax2=min_loc.y + temp1->height;
			xais2=(xmin2+xmax2)/2;yais2=(ymin2+ymax2)/2;
			pt12 = cvPoint(xmin2, ymin2), pt22 = cvPoint(xmax2, ymax2);
	
			if ((xais2==5000)||(yais2==5000)||((xmax2-xmin2)*(ymax2-ymin2)<150000))
			{
				testResultFlag=true;
				baojing=1;
				AfxMessageBox("Panel中间Connect定位失败，请调整光源后重新TEST，谢谢");
				/*cvReleaseImage(&ROI_erzhi);
				cvReleaseImage(&ipl);
				cvReleaseImage(&ROI_erzhi1);
				cvReleaseImage(&ipl1);
				cvReleaseImage(&ipl2);
				cvReleaseImage(&src1);
				cvReleaseImage(&src2);
				cvReleaseImage(&src);
				cvReleaseImage(&temp1);
				cvReleaseImage(&ftmp);*/
				/*YuzhiTestL=10;
				YuzhiTestR=10;*/
				//return;
			}

			cvReleaseImage(&ROI_erzhi);
			cvReleaseImage(&ipl);
			cvReleaseImage(&ROI_erzhi1);
			cvReleaseImage(&ipl1);
			cvReleaseImage(&ipl2);
			cvReleaseImage(&src1);
			//cvReleaseImage(&src2);
			cvReleaseImage(&src);
			cvReleaseImage(&temp1);
			cvReleaseImage(&ftmp);
			cvReleaseMemStorage(&storage1);
			cvReleaseMemStorage(&storage);
			

			temp1=NULL; //关键
			storage1=NULL;
			storage=NULL;
			YuzhiTestL=10;
			YuzhiTestR=10;
			
			double wuli_x1,wuli_y1;
			double wuli_x2,wuli_y2;
			double wuli_x3,wuli_y3;
			wuli_x1=2.2538-(xais-sxais)*Bili_L;
			wuli_y1=2.7622+(yais-syais)*Bili_L;
			wuli_x2=2.2538+232.696-(xais1-sxais1)*Bili_R;
			wuli_y2=2.7457+(yais1-syais1)*Bili_R;
			wuli_x3=2.2538+125.248+(xais2-sxais2)*Bili_C;
			wuli_y3=2.7457+4.432+(yais2-syais2)*Bili_C;
			double a,b,c;
			c=sqrt(pow((wuli_x1-wuli_x3),2)+(wuli_y1-wuli_y3)*(wuli_y1-wuli_y3));
			a=sqrt(pow((wuli_x2-wuli_x3),2)+(wuli_y2-wuli_y3)*(wuli_y2-wuli_y3));
			b=sqrt(pow((wuli_x1-wuli_x2),2)+(wuli_y1-wuli_y2)*(wuli_y1-wuli_y2));
			Panel_Y=81.432-sqrt((a+b+c)*(b+c-a)*(a+c-b)*(a+b-c))/(2*b);
			Panel_X=sqrt(pow(c,2)-(a+b+c)*(b+c-a)*(a+c-b)*(a+b-c)/(4*pow(b,2)))-115.248;

			/*Panel_Y=Panel_Y-0.2*(Panel_Y-81.432);
			Panel_X=Panel_X-0.2*(Panel_X-115.248);*/
			Y_Distance.Format(_T("%lf mm"),Panel_Y);
			L_X_Distance.Format(_T("%lf mm"),Panel_X);

			Bvalue.Format(_T("%lf mm"),b);
			Avalue.Format(_T("%lf mm"),a);
			Cvalue.Format(_T("%lf mm"),c);

			Delta_Y.Format(_T("%lf mm"),Panel_Y-77);
			Delta_X.Format(_T("%lf mm"),Panel_X-10);

			double an=atan(wuli_y2-wuli_y1)/(wuli_x2-wuli_x1);
			Angle.Format(_T("%lf"),abs(an*(360/6.28)));

			CDC MemDC; 
			CBitmap m_Bitmap1;
			if(baojing==1||abs(Panel_Y-77)>0.4||abs(Panel_X-10)>0.4)
				{m_Bitmap1.LoadBitmap(IDB_BITMAPNO);
			}
			else
			{
				if(abs(Panel_Y-77)<0.3&&abs(Panel_X-10)<0.3)
				{
					m_Bitmap1.LoadBitmap(IDB_BITMAPOK);
					jieguo="OK";
				}
				else
				{
					m_Bitmap1.LoadBitmap(IDB_BITMAPNG);
					jieguo="NG";
				}

				SYSTEMTIME st;
				CString DateTime;
				GetLocalTime(&st);
				DateTime.Format("%4d-%2d-%2d %2d:%2d:%2d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

				/*CString path1,path2,path3;
				path1.Format("D:\\MarkImage\\L%s.bmp",Panel_ID);
				path2.Format("D:\\MarkImage\\R%s.bmp",Panel_ID);
				path3.Format("D:\\MarkImage\\C%s.bmp",Panel_ID);
				cvSaveImage( path1, rawImage ); 
				cvSaveImage( path2, rawImage1 ); 
				cvSaveImage(path3, rawImage2 ); */

				char ch2[20];
				GetDlgItem(IDC_EDIT15)->GetWindowText(ch2,20);
				PanelID=ch2;

				char* pszFileName="test.txt";  	/***本地文件存储***/
				CStdioFile myFile;
				CFileException fileException;
				if(myFile.Open(pszFileName,CFile::typeText|CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate),&fileException)
				{
					myFile.SeekToEnd();
					myFile.WriteString("\n");
					myFile.WriteString(PanelID);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(jieguo);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(Y_Distance);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(L_X_Distance);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(DateTime);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(Bvalue);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(Avalue);
					myFile.WriteString("    ");
					myFile.SeekToEnd();
					myFile.WriteString(Cvalue);
					myFile.Close();
				}

				/***数据库存储***/
				/*_ConnectionPtr pConn;
				if (FAILED(pConn.CreateInstance("ADODB.Connection")))
				{
					AfxMessageBox("Create Instance failed!");
					return;
				}
				CString strSRC;
				strSRC="Driver=SQL Server;Server=";
				strSRC+="20140818-1403";
				strSRC+=";Database=";
				strSRC+="108MARK";
				strSRC+=";UID=sa;PWD=s12345";

				CString strSQL;
				strSQL.Format("Insert into MarkTest_Info values ('%s','%s','%lf','%lf','%s','%s','%s','%s')",PanelID,jieguo,Panel_Y,Panel_X,path1,path2,path3,DateTime);
	
				_variant_t varSRC(strSRC);
				_variant_t varSQL(strSQL);
				_bstr_t bstrSRC(strSRC);

				if (FAILED(pConn->Open(bstrSRC,"","",-1)))
				{
					AfxMessageBox("Can not open Database!");
					pConn.Release();
					return;
				}

				COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
				pConn->Execute(_bstr_t(strSQL),&vtOptional,-1);
				pConn.Release();*/
				/****************/

				Panel_ID="";

				testResultFlag=false;

			}
			MemDC.CreateCompatibleDC(NULL);
			MemDC.SelectObject(&m_Bitmap1);
			pDC3->StretchBlt(rect3.left,rect3.top,rect3.Width(),rect3.Height(),&MemDC,0,0,160,90,SRCCOPY); 

			(CEdit *)GetDlgItem(IDC_EDIT15)->SetFocus();  //鼠标焦点  默认
	
		}
		
		UpdateData(FALSE);

	}
}

/*标准版目标定位*/
void CMarkTestDlg::OnBnClickedButton5()
{
	light=0;
	UpdateData(TRUE);
	qiehuan=0;
	//左
	if(str=="21587499")ipl=cvCloneImage(rawImage);
	if(str1=="21587499")ipl=cvCloneImage(rawImage1);
	if(str2=="21587499")ipl=cvCloneImage(rawImage2);
	//右
	if(str=="21587498")ipl1=cvCloneImage(rawImage);
	if(str1=="21587498")ipl1=cvCloneImage(rawImage1);
	if(str2=="21587498")ipl1=cvCloneImage(rawImage2);
	//中
	if(str=="21620157")ipl2=cvCloneImage(rawImagew);
	if(str1=="21620157")ipl2=cvCloneImage(rawImage1w);
	if(str2=="21620157")ipl2=cvCloneImage(rawImage2w);


	if(ipl&&ipl1&&ipl2)
	{
		//左
		IplImage* ROI_erzhi = cvCreateImage( cvGetSize(ipl), 8, 1 ); 
		cvThreshold( ipl, ROI_erzhi,50,255,CV_THRESH_BINARY);
		cvNot(ROI_erzhi,ROI_erzhi);
		/*IplConvKernel *element=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
		cvMorphologyEx( ROI_erzhi, ROI_erzhi, NULL,element, CV_MOP_OPEN, 8); 
*/
		
	IplConvKernel *element=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
	cvMorphologyEx( ROI_erzhi, ROI_erzhi, NULL,element, CV_MOP_OPEN, 2); 

		cvNamedWindow("test-l",0);  
			cvResizeWindow("test-l",cvGetSize(ROI_erzhi).width*0.2,cvGetSize(ROI_erzhi).height*0.2);
			cvShowImage("test-l",ROI_erzhi); 
		CvMemStorage *storage=cvCreateMemStorage();
		CvContourScanner scanner=NULL;
		CvSeq *contour=0;
		double tmparea=0.0; 
		double xinzhaung=0.0;
		double xinzhaung01=0.0;
		CvRect rect;
		int xmin=10000;
		int ymin=10000;
		int xmax=0;
		int ymax=0;
		sxais=0;
		syais=0;
		scanner=cvStartFindContours(ROI_erzhi,storage,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
		while (contour=cvFindNextContour(scanner))
		{
			tmparea=fabs(cvContourArea(contour));//面积
			rect=cvBoundingRect(contour,0);
			xinzhaung=tmparea/(double)(rect.height*rect.width); //面积/矩形面积
			xinzhaung01=rect.width/(double)(rect.height);
			if (700000>tmparea&&tmparea>600000)
			if(0.5<xinzhaung&&xinzhaung<0.9)
			if(1.2>xinzhaung01&&xinzhaung01>0.8)
			{
				if(xmin>rect.x&&rect.x>0)
				{xmin=rect.x;}
				if(ymin>rect.y&&rect.y>0)
				{ymin=rect.y;}
				if(xmax<rect.x+rect.width&&rect.x+rect.width<=cvGetSize(ipl).width)
				{xmax=rect.x+rect.width;}
				if(ymax<rect.y+rect.height&&(rect.y+rect.height)<=cvGetSize(ipl).height)
				{ymax=rect.y+rect.height;}
			}
		}
		contour = cvEndFindContours(&scanner); 
		pt10s = cvPoint(xmin+55, ymin), pt20s = cvPoint(xmax-80, ymax);
		sxais=(xmin+xmax-25)/2;syais=(ymin+ymax)/2;

		CvPoint pre_pt = cvPoint (cvRound(sxais-60), cvRound(syais));  
		CvPoint cur_pt = cvPoint (cvRound(sxais+60), cvRound(syais));  
		Rect_a1[0]=pre_pt;Rect_a1[1]=cur_pt;
		ptzx=Rect_a1;  
		CvPoint pre_pt2 = cvPoint (cvRound(sxais), cvRound(syais-60));  
		CvPoint cur_pt2 = cvPoint (cvRound(sxais), cvRound(syais+60));  
		Rect_a2[0]=pre_pt2;Rect_a2[1]=cur_pt2;
		ptzy=Rect_a2;

		Bili_L=0.00226;

		//右
		IplImage* ROI_erzhi1 = cvCreateImage( cvGetSize(ipl1), 8, 1 ); 
		cvThreshold( ipl1, ROI_erzhi1,50,255,CV_THRESH_BINARY); 
		cvNot(ROI_erzhi1,ROI_erzhi1); 
		/*IplConvKernel *element1=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
		cvMorphologyEx( ROI_erzhi1, ROI_erzhi1, NULL,element1, CV_MOP_OPEN, 8); 
*/
	
	
	cvMorphologyEx( ROI_erzhi1, ROI_erzhi1, NULL,element, CV_MOP_OPEN, 2); 

		CvMemStorage *storage1=cvCreateMemStorage();
		CvContourScanner scanner1=NULL;
		CvSeq *contour1=0;
		double tmparea1=0.0; 
		double xinzhaung11=0.0;
		double xinzhaung1=0.0;
		CvRect rect1;
		int xmin1=10000;
		int ymin1=10000;
		int xmax1=0;
		int ymax1=0;
		sxais1=0;
		syais1=0;
		scanner1=cvStartFindContours(ROI_erzhi1,storage1,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
		while (contour1=cvFindNextContour(scanner1))
		{
			tmparea1=fabs(cvContourArea(contour1));//面积
			rect1=cvBoundingRect(contour1,0);
			xinzhaung1=tmparea1/(double)(rect1.height*rect1.width);
			xinzhaung11=rect1.width/(double)rect1.height;
			if (800000>tmparea1&&tmparea1>600000)
			if(1.2>xinzhaung11&&xinzhaung11>0.8)
			if(0.5<xinzhaung1&&xinzhaung1<0.9)
			{
				if(xmin1>rect1.x&&rect1.x>0)
				{xmin1=rect1.x;}
				if(ymin1>rect1.y&&rect1.y>0)
				{ymin1=rect1.y;}
				if(xmax1<rect1.x+rect1.width&&rect1.x+rect1.width<=cvGetSize(ipl1).width)
				{xmax1=rect1.x+rect1.width;}
				if(ymax1<rect1.y+rect1.height&&(rect1.y+rect1.height)<=cvGetSize(ipl1).height)
				{ymax1=rect1.y+rect1.height;}
			}
		}
		contour1 = cvEndFindContours(&scanner1); 
		pt11s = cvPoint(xmin1, ymin1), pt21s = cvPoint(xmax1, ymax1);
		sxais1=(xmin1+xmax1)/2;syais1=(ymin1+ymax1)/2;

		CvPoint pre_pt1 = cvPoint (cvRound(sxais1-60), cvRound(syais1));  
		CvPoint cur_pt1 = cvPoint (cvRound(sxais1+60), cvRound(syais1));  
		Rect_a11[0]=pre_pt1;Rect_a11[1]=cur_pt1;
		ptzx1=Rect_a11;  
		CvPoint pre_pt21 = cvPoint (cvRound(sxais1), cvRound(syais1-60));  
		CvPoint cur_pt21 = cvPoint (cvRound(sxais1), cvRound(syais1+60));  
		Rect_a21[0]=pre_pt21;Rect_a21[1]=cur_pt21;
		ptzy1=Rect_a21;

		Bili_R=0.00226;

		//中
		int xmin2=10000;
		int ymin2=10000;
		int xmax2=0;
		int ymax2=0;
		sxais2=0;
		syais2=0;
		IplImage *src2;
		cvSetImageROI(ipl2, cvRect(0, 0, ipl2->width, ipl2->height)); //connect识别
		int w=ipl2->width;int h=ipl2->height;
		src2 = cvCreateImage(cvGetSize(ipl2),ipl2->depth,ipl2->nChannels);
		cvCopy(ipl2, src2, NULL);
		cvResetImageROI(ipl2);

		IplImage  *temp1, *ftmp;
		temp1 = cvLoadImage("bc.bmp");
	
		int iwidth = src2->width - temp1->width + 1;  
		int iheight = src2->height - temp1->height + 1;  
  
		ftmp = cvCreateImage(cvSize(iwidth, iheight), 32, 1);  
      
		double min_val;  
		double max_val;  
		CvPoint min_loc;  
		CvPoint max_loc;  
  
		cvMatchTemplate(src2, temp1, ftmp,0);  
		cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);  
		cvRectangle(src2, cvPoint(min_loc.x, min_loc.y),cvPoint((min_loc.x + temp1->width),(min_loc.y + temp1->height)), CV_RGB(0,255,0), 10);  
    
		xmin2=min_loc.x;
		ymin2=min_loc.y;
		xmax2=min_loc.x + temp1->width;
		ymax2=min_loc.y + temp1->height;
		sxais2=(xmin2+xmax2)/2;
		syais2=(ymin2+ymax2)/2;

		pt12s = cvPoint(xmin2, ymin2), pt22s = cvPoint(xmax2, ymax2);

		CvPoint pre_pt02 = cvPoint (cvRound(sxais2-60), cvRound(syais2));  
		CvPoint cur_pt02 = cvPoint (cvRound(sxais2+60), cvRound(syais2));  
		Rect_a12[0]=pre_pt02;Rect_a12[1]=cur_pt02;
		ptzx2=Rect_a12;  
		CvPoint pre_pt22 = cvPoint (cvRound(sxais2), cvRound(syais2-60));  
		CvPoint cur_pt22 = cvPoint (cvRound(sxais2), cvRound(syais2+60));  
		Rect_a22[0]=pre_pt22;Rect_a22[1]=cur_pt22;
		ptzy2=Rect_a22;

		Bili_C=0.0099;//0.0124

		cvReleaseImage(&ROI_erzhi);
		cvReleaseImage(&ipl);
		cvReleaseImage(&ROI_erzhi1);
		cvReleaseImage(&ipl1);
		cvReleaseImage(&ipl2);
		cvReleaseImage(&src2);
		cvReleaseImage(&temp1);
		cvReleaseImage(&ftmp);
		temp1=NULL;   //关键
	
	}

	UpdateData(FALSE);
	//light=0;
}

	/*填充算法*/
	IplImage* CMarkTestDlg::imfill(IplImage* src)
	{ 
		CvScalar white = CV_RGB( 255, 255, 255 );
		IplImage* dst = cvCreateImage( cvGetSize(src), 8, 3);
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contour = 0;
		cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
		cvZero( dst );
		for( ; contour != 0; contour = contour->h_next )
		{cvDrawContours( dst, contour, white, white, 0, CV_FILLED);}
		IplImage* bin_imgFilled = cvCreateImage(cvGetSize(src), 8, 1);
		cvInRangeS(dst, white, white, bin_imgFilled);
		return bin_imgFilled;
	}

	/*标准版CHECK */
	void CMarkTestDlg::OnBnClickedButton6()
	{
		light=0;
		UpdateData(TRUE);
		//左
		if(str=="21587499")sipl=cvCloneImage(rawImage);
		if(str1=="21587499")sipl=cvCloneImage(rawImage1);
		if(str2=="21587499")sipl=cvCloneImage(rawImage2);
		//右
		if(str=="21587498")sipl1=cvCloneImage(rawImage);
		if(str1=="21587498")sipl1=cvCloneImage(rawImage1);
		if(str2=="21587498")sipl1=cvCloneImage(rawImage2);
		//中
		if(str=="21620157")sipl2=cvCloneImage(rawImagew);
		if(str1=="21620157")sipl2=cvCloneImage(rawImage1w);
		if(str2=="21620157")sipl2=cvCloneImage(rawImage2w);

		if(sipl&&sipl1&&sipl2)
		{
			//左
			IplImage* ROI_erzhi = cvCreateImage( cvGetSize(sipl), 8, 1 ); 
			cvThreshold( sipl, ROI_erzhi,50,255,CV_THRESH_BINARY); 
			cvNot(ROI_erzhi,ROI_erzhi); 
			/*IplConvKernel *element=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
			cvMorphologyEx( ROI_erzhi, ROI_erzhi, NULL,element, CV_MOP_OPEN, 8);
*/
			
	IplConvKernel *element=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
	cvMorphologyEx( ROI_erzhi, ROI_erzhi, NULL,element, CV_MOP_OPEN, 2); 

			CvMemStorage *storage=cvCreateMemStorage();
			CvContourScanner scanner=NULL;
			CvSeq *contour=0;
			double tmparea=0.0; 
			double xinzhaung=0.0;
			double xinzhaung01=0.0;
			CvRect rect;
			int xmin=10000;
			int ymin=10000;
			int xmax=0;
			int ymax=0;
			scanner=cvStartFindContours(ROI_erzhi,storage,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour=cvFindNextContour(scanner))
			{
				tmparea=fabs(cvContourArea(contour));//面积
				rect=cvBoundingRect(contour,0);
				xinzhaung=tmparea/(double)(rect.height*rect.width);
				xinzhaung01=rect.width/(double)(rect.height);
				if (700000>tmparea&&tmparea>600000)
				if(1.2>xinzhaung01&&xinzhaung01>0.8)
				if(0.5<xinzhaung&&xinzhaung<0.9)
				{
					if(xmin>rect.x&&rect.x>0)
					{xmin=rect.x;}
					if(ymin>rect.y&&rect.y>0)
					{ymin=rect.y;}
					if(xmax<rect.x+rect.width&&rect.x+rect.width<=cvGetSize(sipl).width)
					{xmax=rect.x+rect.width;}
					if(ymax<rect.y+rect.height&&(rect.y+rect.height)<=cvGetSize(sipl).height)
					{ymax=rect.y+rect.height;}
				}
			}
			contour = cvEndFindContours(&scanner); 
			int ssxais=(xmin+xmax-25)/2,ssyais=(ymin+ymax)/2;

			//右
			IplImage* ROI_erzhi1 = cvCreateImage( cvGetSize(sipl1), 8, 1 ); 	
			cvThreshold( sipl1, ROI_erzhi1,50,255,CV_THRESH_BINARY);  //connect 250  mark  179
			cvNot(ROI_erzhi1,ROI_erzhi1); 
			/*IplConvKernel *element1=cvCreateStructuringElementEx(5,5,1,1,CV_SHAPE_RECT,0);
			cvMorphologyEx( ROI_erzhi1, ROI_erzhi1, NULL,element1, CV_MOP_OPEN, 8);  
*/
			
	
	cvMorphologyEx( ROI_erzhi1, ROI_erzhi1, NULL,element, CV_MOP_OPEN, 2); 

			CvMemStorage *storage1=cvCreateMemStorage();
			CvContourScanner scanner1=NULL;
			CvSeq *contour1=0;
			double tmparea1=0.0; 
			double xinzhaung1=0.0;
			double xinzhaung11=0.0;
			CvRect rect1;
			int xmin1=10000;
			int ymin1=10000;
			int xmax1=0;
			int ymax1=0;
			scanner1=cvStartFindContours(ROI_erzhi1,storage1,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour1=cvFindNextContour(scanner1))
			{
				tmparea1=fabs(cvContourArea(contour1));//面积
				rect1=cvBoundingRect(contour1,0);
				xinzhaung1=tmparea1/(double)(rect1.height*rect1.width);
				xinzhaung11=rect1.width/(double)rect1.height;
				if (800000>tmparea1&&tmparea1>600000)
				if(1.2>xinzhaung11&&xinzhaung11>0.8)
				if(0.5<xinzhaung1&&xinzhaung1<0.9)
				{
					if(xmin1>rect1.x&&rect1.x>0)
					{xmin1=rect1.x;}
					if(ymin1>rect1.y&&rect1.y>0)
					{ymin1=rect1.y;}
					if(xmax1<rect1.x+rect1.width&&rect1.x+rect1.width<=cvGetSize(sipl1).width)
					{xmax1=rect1.x+rect1.width;}
					if(ymax1<rect1.y+rect1.height&&(rect1.y+rect1.height)<=cvGetSize(sipl1).height)
					{ymax1=rect1.y+rect1.height;}
				}
			}
			contour1 = cvEndFindContours(&scanner1); 
			int ssxais1=(xmin1+xmax1)/2,ssyais1=(ymin1+ymax1)/2;

		   //中
			IplImage *src2;
			cvSetImageROI(sipl2, cvRect(0, 0, sipl2->width, sipl2->height)); //connect识别
			src2 = cvCreateImage(cvGetSize(sipl2),sipl2->depth,sipl2->nChannels);
			cvCopy(sipl2, src2, NULL);
			cvResetImageROI(sipl2);

			int xmin2=10000;
			int ymin2=10000;
			int xmax2=0;
			int ymax2=0;
			
			IplImage  *temp1, *ftmp;
			temp1 = cvLoadImage("bc.bmp");
	
			int iwidth = src2->width - temp1->width + 1;  
			int iheight = src2->height - temp1->height + 1;  
  
			ftmp = cvCreateImage(cvSize(iwidth, iheight), 32, 1);  
      
			double min_val;  
			double max_val;  
			CvPoint min_loc;  
			CvPoint max_loc;  
  
			cvMatchTemplate(src2, temp1, ftmp,0);  
			cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);  
			cvRectangle(src2, cvPoint(min_loc.x, min_loc.y),cvPoint((min_loc.x + temp1->width),(min_loc.y + temp1->height)), CV_RGB(0,255,0), 10);  
    
			xmin2=min_loc.x;
			ymin2=min_loc.y;
			xmax2=min_loc.x + temp1->width;
			ymax2=min_loc.y + temp1->height;
	
			int ssxais2=(xmin2+xmax2)/2,ssyais2=(ymin2+ymax2)/2;

			double swuli_x1,swuli_y1;
			double swuli_x2,swuli_y2;
			double swuli_x3,swuli_y3;
			/*swuli_x1=2.2538-(ssxais-sxais)*Bili_L;
			swuli_y1=2.7622+(ssyais-syais)*Bili_L;
			swuli_x2=2.2538+232.717-(ssxais1-sxais1)*Bili_R;
			swuli_y2=2.7457+(ssyais1-syais1)*Bili_R;
			swuli_x3=2.2538+125.242+(ssyais2-syais2)*Bili_C;
			swuli_y3=2.7457+4.434+(ssxais2-sxais2)*Bili_C;

			double a,b,c;
	
			c=sqrt(pow((swuli_x1-swuli_x3),2)+(swuli_y1-swuli_y3)*(swuli_y1-swuli_y3));
			a=sqrt(pow((swuli_x2-swuli_x3),2)+(swuli_y2-swuli_y3)*(swuli_y2-swuli_y3));
			b=sqrt(pow((swuli_x1-swuli_x2),2)+(swuli_y1-swuli_y2)*(swuli_y1-swuli_y2));

			sPanel_Y=81.432-sqrt(1-pow((pow(a,2)+pow(c,2)-pow(b,2))/(2*a*c),2))*a;
			sPanel_X=a*(pow(a,2)+pow(c,2)-pow(b,2))/(2*a*c)-c*(115.248/(117.448+115.248));*/
			swuli_x1=2.2538-(ssxais-sxais)*Bili_L;
			swuli_y1=2.7622+(ssyais-syais)*Bili_L;
			swuli_x2=2.2538+232.696-(ssxais1-sxais1)*Bili_R;
			swuli_y2=2.7457+(ssyais1-syais1)*Bili_R;
			swuli_x3=2.2538+125.248+(ssyais2-syais2)*Bili_C;
			swuli_y3=2.7457+4.432+(ssxais2-sxais2)*Bili_C;
			double a,b,c;
			c=sqrt(pow((swuli_x1-swuli_x3),2)+(swuli_y1-swuli_y3)*(swuli_y1-swuli_y3));
			a=sqrt(pow((swuli_x2-swuli_x3),2)+(swuli_y2-swuli_y3)*(swuli_y2-swuli_y3));
			b=sqrt(pow((swuli_x1-swuli_x2),2)+(swuli_y1-swuli_y2)*(swuli_y1-swuli_y2));

			sPanel_Y=81.432-sqrt((a+b+c)*(b+c-a)*(a+c-b)*(a+b-c))/(2*b);
			sPanel_X=sqrt(pow(c,2)-(a+b+c)*(b+c-a)*(a+c-b)*(a+b-c)/(4*pow(b,2)))-115.248;

			/*wuli_x1=2.2538-(xais-sxais)*Bili_L;
			wuli_y1=2.7622+(yais-syais)*Bili_L;
			wuli_x2=2.2538+232.696-(xais1-sxais1)*Bili_R;
			wuli_y2=2.7457+(yais1-syais1)*Bili_R;
			wuli_x3=2.2538+125.248+(xais2-sxais2)*Bili_C;
			wuli_y3=2.7457+4.432+(yais2-syais2)*Bili_C;
			double a,b,c;
			c=sqrt(pow((wuli_x1-wuli_x3),2)+(wuli_y1-wuli_y3)*(wuli_y1-wuli_y3));
			a=sqrt(pow((wuli_x2-wuli_x3),2)+(wuli_y2-wuli_y3)*(wuli_y2-wuli_y3));
			b=sqrt(pow((wuli_x1-wuli_x2),2)+(wuli_y1-wuli_y2)*(wuli_y1-wuli_y2));
			Panel_Y=81.432-sqrt((a+b+c)*(b+c-a)*(a+c-b)*(a+b-c))/(2*b);
			Panel_X=sqrt(pow(c,2)-(a+b+c)*(b+c-a)*(a+c-b)*(a+b-c)/(4*pow(b,2)))-115.248;*/

			Stand_Y.Format(_T("%lf mm"),sPanel_Y);
			Stand_X.Format(_T("%lf mm"),sPanel_X);

			Delt_Y.Format(_T("%lf mm"),sPanel_Y-76.998);
			Delt_X.Format(_T("%lf mm"),sPanel_X-9.984);

			cvReleaseImage(&ROI_erzhi);
			cvReleaseImage(&sipl);
			cvReleaseImage(&ROI_erzhi1);
			cvReleaseImage(&sipl1);
			//cvReleaseImage(&ROI_erzhi2);
			cvReleaseImage(&sipl2);
			cvReleaseImage(&src2);
			cvReleaseImage(&temp1);
			cvReleaseImage(&ftmp);
			temp1=NULL;   //关键
		}
		UpdateData(FALSE);

		//light=0;
	}

	/*自动引导 generate*/
	void CMarkTestDlg::OnBnClickedButton7()
	{
		light=1;
		qiehuan=1;

		Ypianyi=0.0;

		char ch2[20];
		GetDlgItem(IDC_EDIT15)->GetWindowText(ch2,20);
		Panel_ID=ch2;

		if(str=="21587499")ipl=cvCloneImage(rawImagew);
		if(str1=="21587499")ipl=cvCloneImage(rawImage1w);
		if(str2=="21587499")ipl=cvCloneImage(rawImage2w);

		if(str=="21587498")ipl1=cvCloneImage(rawImagew);
		if(str1=="21587498")ipl1=cvCloneImage(rawImage1w);
		if(str2=="21587498")ipl1=cvCloneImage(rawImage2w);

		if(str=="21620157")ipl2=cvCloneImage(rawImagew);
		if(str1=="21620157")ipl2=cvCloneImage(rawImage1w);
		if(str2=="21620157")ipl2=cvCloneImage(rawImage2w);

	    if(ipl&&ipl1&&ipl2)
		{
			LoopTestL:
			IplImage *src;  /*左*/
			cvSetImageROI(ipl, cvRect(ipl->width/4, ipl->height/4, ipl->width/2, ipl->height/2)); //connect识别
			src = cvCreateImage(cvGetSize(ipl),ipl->depth,ipl->nChannels);
			cvCopy(ipl, src, NULL);
			cvResetImageROI(ipl);

			IplImage* ROI_erzhi = cvCreateImage( cvGetSize(src), 8, 1 ); 
			cvThreshold( src, ROI_erzhi,YuzhiTestL,255,CV_THRESH_BINARY);//110
			cvNamedWindow( "L", 0 );
			cvResizeWindow("L",cvGetSize(src).width*0.2,cvGetSize(src).height*0.2);
			cvShowImage("L",ROI_erzhi);cvWaitKey(1);
			
			/*cvNamedWindow("test-l",0);  
			cvResizeWindow("test-l",cvGetSize(ROI_erzhi).width*0.2,cvGetSize(ROI_erzhi).height*0.2);
			cvShowImage("test-l",ROI_erzhi); */

			CvMemStorage *storage=cvCreateMemStorage();
			CvContourScanner scanner=NULL;
			CvSeq *contour=0;
			double tmparea=0.0; 
			double xinzhaung10=0.0;
			double xinzhaung20=0.0;
			CvRect rect;
			int xmin=10000;
			int ymin=10000;
			int xmax=0;
			int ymax=0;
			int xais=0;
			int yais=0;
			scanner=cvStartFindContours(ROI_erzhi,storage,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour=cvFindNextContour(scanner))
			{
				tmparea=fabs(cvContourArea(contour));//面积
				rect=cvBoundingRect(contour,0);
				xinzhaung10=rect.height/(double)rect.width;
				xinzhaung20=tmparea/(rect.height*rect.width);
				if (12000>tmparea&&tmparea>8000)
				if(0.9<xinzhaung10&&xinzhaung10<1.1)
				if(0.4<xinzhaung20&&xinzhaung20<0.545)
				if(rect.y>200)
				{
					if(xmin>rect.x&&rect.x>0)
					{xmin=rect.x;}
					if(ymin>rect.y&&rect.y>0)
					{ymin=rect.y;}
					if(xmax<rect.x+rect.width&&rect.x+rect.width<=cvGetSize(ipl).width)
					{xmax=rect.x+rect.width;}
					if(ymax<rect.y+rect.height&&(rect.y+rect.height)<=cvGetSize(ipl).height)
					{ymax=rect.y+rect.height;}
				}
			}
			contour = cvEndFindContours(&scanner); 
			pt10 = cvPoint(ipl->width/4+xmin, ipl->height/4+ymin), pt20 = cvPoint(ipl->width/4+xmax, ipl->height/4+ymax);
			xais=(ipl->width/4+xmin+ipl->width/4+xmax)/2;yais=(ipl->height/4+ymin+ipl->height/4+ymax)/2;
			if ((xais==5000)||(yais==5000)||((xmax-xmin)*(ymax-ymin)>100000))
			{
				YuzhiTestL=YuzhiTestL+20;
				if(YuzhiTestL<256)
				{
					cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&src);
					cvReleaseMemStorage(&storage);
					storage=NULL;
					goto LoopTestL;
				}
				else
				{
					testResultFlag=true;
					baojing=1;
					AfxMessageBox("Panel左上Mark定位失败，请调整Panel位置或者清理Mark表面后重新TEST，谢谢");
					/*cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&ipl);
					cvReleaseImage(&ipl1);
					cvReleaseImage(&ipl2);
					cvReleaseImage(&src);*/
					/*YuzhiTestL=10;
					YuzhiTestR=10;*/
					//return;
			    }	
			 }

			LoopTestR:
			IplImage *src1;	/*右*/
			cvSetImageROI(ipl1, cvRect(ipl1->width/3, ipl1->height/5, ipl1->width/2, ipl1->height/2)); //connect识别  **
			src1 = cvCreateImage(cvGetSize(ipl1),ipl1->depth,ipl1->nChannels);
			cvCopy(ipl1, src1, NULL);
			cvResetImageROI(ipl1);

			IplImage* ROI_erzhi1 = cvCreateImage( cvGetSize(src1), 8, 1 ); 
			cvThreshold( src1, ROI_erzhi1,YuzhiTestR,255,CV_THRESH_BINARY);//60

			CvMemStorage *storage1=cvCreateMemStorage();
			CvContourScanner scanner1=NULL;
			CvSeq *contour1=0;
			double tmparea1=0.0; 
			double xinzhaung1=0.0;
			double xinzhaung2=0.0;
			CvRect rect1;
			int xmin1=10000;
			int ymin1=10000;
			int xmax1=0;
			int ymax1=0;
			int xais1=0;
			int yais1=0;
			scanner1=cvStartFindContours(ROI_erzhi1,storage1,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
			while (contour1=cvFindNextContour(scanner1))
			{
				tmparea1=fabs(cvContourArea(contour1));//面积
				rect1=cvBoundingRect(contour1,0);
				xinzhaung1=rect1.height/(double)rect1.width;
				xinzhaung2=tmparea1/(rect1.height*rect1.width);
				if (12000>tmparea1&&tmparea1>8000)
				if(0.9<xinzhaung1&&xinzhaung1<1.1)
				if(0.4<xinzhaung2&&xinzhaung2<0.545)
				{
					if(rect1.x<1000)
					{
					if(xmin1>rect1.x&&rect1.x>0)
					{xmin1=rect1.x;}
					if(ymin1>rect1.y&&rect1.y>0)
					{ymin1=rect1.y;}
					if(xmax1<rect1.x+rect1.width&&rect1.x+rect1.width<=cvGetSize(ipl1).width)
					{xmax1=rect1.x+rect1.width;}
					if(ymax1<rect1.y+rect1.height&&(rect1.y+rect1.height)<=cvGetSize(ipl1).height)
					{ymax1=rect1.y+rect1.height;}
					}
				}
			}
			contour1 = cvEndFindContours(&scanner1); 
			pt11 = cvPoint(xmin1+ipl1->width/3, ipl1->height/5+ymin1), pt21 = cvPoint(ipl1->width/3+xmax1, ipl1->height/5+ymax1);
			xais1=(ipl1->width/3+xmin1+ipl1->width/3+xmax1)/2;yais1=(ipl1->height/5+ymin1+ipl1->height/5+ymax1)/2;
			if ((xais1==5000)||(yais1==5000)||((xmax1-xmin1)*(ymax1-ymin1)>100000))
			{
				YuzhiTestR=YuzhiTestR+20;
				if(YuzhiTestR<256)
				{
					cvReleaseImage(&ROI_erzhi1);
					cvReleaseImage(&src1);
					cvReleaseMemStorage(&storage1);
					storage1=NULL;
					goto LoopTestR;
				}
				else
				{
					testResultFlag=true;
					baojing=1;
					AfxMessageBox("Panel右上Mark定位失败，请调整Panel位置或者清理Mark表面后重新TEST，谢谢");
					/*cvReleaseImage(&ROI_erzhi);
					cvReleaseImage(&ipl);
					cvReleaseImage(&ROI_erzhi1);
					cvReleaseImage(&ipl1);
					cvReleaseImage(&ipl2);
					cvReleaseImage(&src1);
					cvReleaseImage(&src);*/
					/*YuzhiTestL=10;
					YuzhiTestR=10;*/
					//return;
			    }	
			}

			int xais2=0;   /*中间*/
			int yais2=0;
			int xmin2=10000;
			int ymin2=10000;
			int xmax2=0;
			int ymax2=0;
			IplImage *temp1, *ftmp;
			/*int w=ipl2->width;int h=ipl2->height;
			cvSetImageROI(ipl2, cvRect(0,0, ipl2->width, ipl2->height)); 
			src2 = cvCreateImage(cvGetSize(ipl2),8, 3);
			cvCopy(ipl2, src2, NULL);
			cvResetImageROI(ipl2);*/
			temp1 = cvLoadImage("pc1.bmp");
  
			int iwidth = ipl2->width - temp1->width + 1;  
			int iheight = ipl2->height - temp1->height + 1;  
  
			ftmp = cvCreateImage(cvSize(iwidth, iheight), 32, 1);  
      
			double min_val;  
			double max_val;  
			CvPoint min_loc;  
			CvPoint max_loc;  
  
			cvMatchTemplate(ipl2, temp1, ftmp, 0);  
			cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);  
			//cvRectangle(src, cvPoint(min_loc.x, min_loc.y),cvPoint((min_loc.x + temp1->width),(min_loc.y + temp1->height)), CV_RGB(0,255,0), 10);  
			xmin2=min_loc.x;
			ymin2=min_loc.y;
			xmax2=min_loc.x + temp1->width;
			ymax2=min_loc.y + temp1->height;
			xais2=(xmin2+xmax2)/2;yais2=(ymin2+ymax2)/2;
			pt12 = cvPoint(xmin2, ymin2), pt22 = cvPoint(xmax2, ymax2);
			if ((xais2==5000)||(yais2==5000)||((xmax2-xmin2)*(ymax2-ymin2)<150000))
			{baojing=1;AfxMessageBox("Panel中间Connect定位失败，请调整光源后重新TEST，谢谢");}

		
			cvReleaseImage(&ROI_erzhi);
			cvReleaseImage(&ipl);
			cvReleaseImage(&ROI_erzhi1);
			cvReleaseImage(&ipl1);
			cvReleaseImage(&ipl2);
			cvReleaseImage(&src1);
			//cvReleaseImage(&src2);
			cvReleaseImage(&src);
			cvReleaseImage(&temp1);
			cvReleaseImage(&ftmp);
			cvReleaseMemStorage(&storage1);
			cvReleaseMemStorage(&storage);
			

			temp1=NULL; //关键
			storage1=NULL;
			storage=NULL;
			YuzhiTestL=10;
			YuzhiTestR=10;

			double wuli_x3,wuli_y3;
			

			wuli_y3=(yais2-syais2)*Bili_C-((yais+yais1-syais-syais1)/2)*Bili_L; //实际物理偏移 负->C向左上偏->panel向左上移动->即传给PLC的值
			wuli_x3=(xais2-sxais2)*Bili_C;
		
			xais=wuli_x3/Bili_L+sxais;
			yais=wuli_y3/Bili_L+syais;
			xais1=wuli_x3/Bili_R+sxais1;
			yais1=wuli_y3/Bili_R+syais1;

			Xpianyi=wuli_x3;
			Ypianyi=wuli_y3;  //物理偏移 PLC补偿
			Lxais=xais;Lyais=yais;Rxais=xais1;Ryais=yais1;//Mark 图像 理论位置

			if((Ypianyi<0.01&&Ypianyi>0.0)||(Ypianyi>-0.01&&Ypianyi<0.0))
				geneResultFlag=false;
			else
				geneResultFlag=true;

			ptmin = cvPoint(xais-(0.15/Bili_L)-30, yais+40-(0.15/Bili_L)-30), ptmax = cvPoint(xais+(0.15/Bili_L)+30, yais+40+(0.15/Bili_L)+30); 
			ptminw = cvPoint(xais1-(0.15/Bili_R)-30, yais1-(0.15/Bili_R)-30), ptmaxw = cvPoint(xais1+(0.15/Bili_R)+30, yais1+(0.15/Bili_R)+30); 
		}
		
	}
	//
	CString strSN(_T(""));	/*扫描枪文本框 自动跳转*/
	CString m_SN(_T(""));
	BOOL CMarkTestDlg::PreTranslateMessage(MSG* pMsg)
	{
		if(pMsg -> message == WM_KEYDOWN)//判断是否有按键按下
		{ 
			if(strcmp(strSN,_T("")) && (pMsg -> wParam != 13)) //VK_RETURN = 13
			{
				m_SN = _T("");
				UpdateData(FALSE);
				strSN=_T("");
			}
			if(pMsg -> wParam == VK_ESCAPE) //Esc 键
			return TRUE;
			if(pMsg -> wParam == VK_RETURN)//Enter键
			{
				UpdateData(TRUE);
				strSN = m_SN;
				return TRUE;
			}
		 }
		return CDialogEx::PreTranslateMessage(pMsg);
	}


	void CMarkTestDlg::OnBnClickedButton8()
	{
		//if(str=="21527623")ipl=cvCloneImage(rawImage);
		//if(str1=="21527623")ipl=cvCloneImage(rawImage1);
		//if(str2=="21527623")ipl=cvCloneImage(rawImage2);

		//if(str=="21497823")ipl1=cvCloneImage(rawImage);
		//if(str1=="21497823")ipl1=cvCloneImage(rawImage1);
		//if(str2=="21497823")ipl1=cvCloneImage(rawImage2);

		//qiehuan=3;

		//
		//IplImage *src;/*左*/
		//cvSetImageROI(ipl, cvRect(ipl->width/4, ipl->height/9, ipl->width/2, ipl->height/2)); //connect识别
		//src = cvCreateImage(cvGetSize(ipl),ipl->depth,ipl->nChannels);
		//cvCopy(ipl, src, NULL);
		//cvResetImageROI(ipl);

		//IplImage* ROI_erzhi = cvCreateImage( cvGetSize(src), 8, 1 ); 
		//cvThreshold( src, ROI_erzhi,160,255,CV_THRESH_BINARY);
		//
		//CvMemStorage *storage=cvCreateMemStorage();
		//CvContourScanner scanner=NULL;
		//CvSeq *contour=0;
		//double tmparea=0.0; 
		//double xinzhaung=0.0;
		//double xinzhaung10=0.0;
		//double xinzhaung20=0.0;
		//CvRect rect;
		//int xmin=10000;
		//int ymin=10000;
		//int xmax=0;
		//int ymax=0;
		//int xais=0;
		//int yais=0;
		//scanner=cvStartFindContours(ROI_erzhi,storage,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
		//while (contour=cvFindNextContour(scanner))
		//{
		//	tmparea=fabs(cvContourArea(contour));//面积
		//	rect=cvBoundingRect(contour,0);
		//	xinzhaung10=rect.height/(double)rect.width;
		//	xinzhaung20=tmparea/(rect.height*rect.width);
		//	if (14000>tmparea&&tmparea>6000)
		//	if(0.7<xinzhaung10&&xinzhaung10<1.3)
		//		if(0.4<xinzhaung20&&xinzhaung20<0.575)
		//		{
		//			if(xmin>rect.x&&rect.x>0)
		//			{xmin=rect.x;}
		//			if(ymin>rect.y&&rect.y>0)
		//			{ymin=rect.y;}
		//			if(xmax<rect.x+rect.width&&rect.x+rect.width<=cvGetSize(ipl).width)
		//			{xmax=rect.x+rect.width;}
		//			if(ymax<rect.y+rect.height&&(rect.y+rect.height)<=cvGetSize(ipl).height)
		//			{ymax=rect.y+rect.height;}
		//		}
		//}
		//contour = cvEndFindContours(&scanner); 
		//pt10 = cvPoint(ipl->width/4+xmin, ipl->height/9+ymin), pt20 = cvPoint(ipl->width/4+xmax, ipl->height/9+ymax);
		//xais=(ipl->width/4+xmin+ipl->width/4+xmax)/2;yais=(ipl->height/9+ymin+ipl->height/9+ymax)/2;

		//
		//IplImage *src1;/*right*/
		//cvSetImageROI(ipl1, cvRect(ipl1->width/5, ipl1->height/3, ipl1->width/2, ipl1->height/2)); //connect识别
		//src1 = cvCreateImage(cvGetSize(ipl1),ipl1->depth,ipl1->nChannels);
		//cvCopy(ipl1, src1, NULL);
		//cvResetImageROI(ipl1);

		//IplImage* ROI_erzhi1 = cvCreateImage( cvGetSize(src1), 8, 1 ); 
		//cvThreshold( src1, ROI_erzhi1,160,255,CV_THRESH_BINARY);

		//CvMemStorage *storage1=cvCreateMemStorage();
		//CvContourScanner scanner1=NULL;
		//CvSeq *contour1=0;
		//double tmparea1=0.0; 
		//double xinzhaung1=0.0;
		//double xinzhaung2=0.0;
		//CvRect rect1;
		//int xmin1=10000;
		//int ymin1=10000;
		//int xmax1=0;
		//int ymax1=0;
		//int xais1=0;
		//int yais1=0;
		//scanner1=cvStartFindContours(ROI_erzhi1,storage1,sizeof(CvContour),CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
		//while (contour1=cvFindNextContour(scanner1))
		//{
		//	tmparea1=fabs(cvContourArea(contour1));//面积
		//	rect1=cvBoundingRect(contour1,0);
		//	xinzhaung1=rect1.height/(double)rect1.width;
		//	xinzhaung2=tmparea1/(rect1.height*rect1.width);
		//		if (14000>tmparea1&&tmparea1>6000)
		//		if(0.7<xinzhaung1&&xinzhaung1<1.3)
		//			if(0.4<xinzhaung2&&xinzhaung2<0.565)
		//		{
		//			if(xmin1>rect1.x&&rect1.x>0)
		//			{xmin1=rect1.x;}
		//			if(ymin1>rect1.y&&rect1.y>0)
		//			{ymin1=rect1.y;}
		//			if(xmax1<rect1.x+rect1.width&&rect1.x+rect1.width<=cvGetSize(ipl1).width)
		//			{xmax1=rect1.x+rect1.width;}
		//			if(ymax1<rect1.y+rect1.height&&(rect1.y+rect1.height)<=cvGetSize(ipl1).height)
		//			{ymax1=rect1.y+rect1.height;}
		//		}
		//}
		//contour1 = cvEndFindContours(&scanner1); 
		//pt11 = cvPoint(ipl1->width/5+xmin1, ipl1->height/3+ymin1), pt21 = cvPoint(ipl1->width/5+xmax1, ipl1->height/3+ymax1);
		//xais1=(ipl1->width/5+xmin1+ipl1->width/5+xmax1)/2;yais1=(ipl1->height/3+ymin1+ipl1->height/3+ymax1)/2;

		//cvReleaseImage(&ipl);
		//cvReleaseImage(&ipl1);
		//cvReleaseImage(&ROI_erzhi);
		//cvReleaseImage(&ROI_erzhi1);
		//cvReleaseImage(&src);
		//cvReleaseImage(&src1);

		//if(Lyais-10<yais&&Lyais+10>yais&&Ryais-10<yais1&&Ryais+10>yais1)
		//{
		//	doubleResultFlag=false;
		//}
		//else
		//{
		//	doubleResultFlag=true;
		//}
			
		doubleResultFlag=false;
	}

//#ifdef EXP_LINK 
void CMarkTestDlg::InitialPLC()  /**PLC**/
{

	hDMTDll = LoadLibrary("DMT");
	RequestData = (int (WINAPI *)(int, int, int, int, const unsigned char *,int))GetProcAddress(hDMTDll,"RequestData");
	ResponseData = (int (WINAPI *)(int, int, int *, int *, unsigned char *))GetProcAddress(hDMTDll,"ResponseData");
	OpenModbusSerial = (int (WINAPI *)(int, int, int, char, int, int))GetProcAddress(hDMTDll,"OpenModbusSerial");
	CloseSerial = (void (WINAPI *)(int))GetProcAddress(hDMTDll,"CloseSerial");
	GetLastSerialErr = (int (WINAPI *)())GetProcAddress(hDMTDll,"GetLastSerialErr");
	ResetSerialErr = (void (WINAPI *)())GetProcAddress(hDMTDll,"ResetSerialErr");
}

bool CMarkTestDlg::InitialSerial()
{
	InitialPLC() ;
	if(OpenModbusSerial(1,9600,7,'E',1,1) == -1)
	{
		KillTimer(10);
		::MessageBoxA(m_hWnd,"连接PLC失败！","提示",MB_OK);
		return false;
	}
	return true;
}
void CMarkTestDlg::CloseCom(int com)
{
	CloseSerial(com);
}
BOOL CMarkTestDlg::SerialReadData(CString strReqData,unsigned char * ReadData)
{
	unsigned char sendbuf[1024] = {0};
	unsigned char recvbuf[1024] = {0};

	int modbus_addr = 0;
	int modbus_func = 0;
	int modbus_addr_ret = 0;
	int modbus_func_ret = 0;
	int sendlen = 0;
	int i = 0;
	if (InitialSerial()!=true)
	{
		return FALSE;
	}
	if(strReqData.GetLength()%2 != 0) // input data must be even number
	{
		AfxMessageBox("Modbus Data Must Be Even Number");
		return FALSE;
	}
	char A2H[3] = {0};
	for (i = 0 ; i < strReqData.GetLength() ; i+=2)
	{
		int num = 0;
		A2H[0] = strReqData.GetAt(i);
		A2H[1] = strReqData.GetAt(i+1);
		sscanf(A2H,"%02X",&num);
		if (i == 0)
			modbus_addr = num;
		else if (i == 2)
			modbus_func = num;
		else
			sendbuf[sendlen++] = num;
	}
	int req = RequestData(0, 1, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
	if(req == -1)
	{
		::MessageBoxA(m_hWnd,"连接PLC失败！","提示",MB_OK);
		CloseSerial(1);
		return FALSE;
	}

	int res = ResponseData(0, 1, &modbus_addr_ret, &modbus_func_ret, ReadData); // modbus response

	if(res > 0)
	{
		CloseCom(1);
		return true;
	}
	else
	{
		CloseCom(1);
		return FALSE;
	}
}
BOOL CMarkTestDlg::SerialWriteData(CString strReqData)
{
	unsigned char sendbuf[1024] = {0};
	unsigned char recvbuf[1024] = {0};

	int modbus_addr = 0;
	int modbus_func = 0;
	int modbus_addr_ret = 0;
	int modbus_func_ret = 0;
	int sendlen = 0;
	int i = 0;
	if (InitialSerial()!=true)
	{
		return FALSE;
	}
	if(strReqData.GetLength()%2 != 0) // input data must be even number
	{
		AfxMessageBox("Modbus Data Must Be Even Number");
		return FALSE;
	}
	char A2H[3] = {0};
	for (i = 0 ; i < strReqData.GetLength() ; i+=2)
	{
		int num = 0;
		A2H[0] = strReqData.GetAt(i);
		A2H[1] = strReqData.GetAt(i+1);
		sscanf(A2H,"%02X",&num);
		if (i == 0)
			modbus_addr = num;
		else if (i == 2)
			modbus_func = num;
		else
			sendbuf[sendlen++] = num;
	}

	int req = RequestData(0, 1, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
	if(req == -1)
	{
		AfxMessageBox("连接PLC失败！");
		CloseSerial(1);
		return FALSE;
	}

	int res = ResponseData(0, 1, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response

	if(res > 0)
	{
		CloseCom(1);
		return true;
	}
	else
	{
		CloseCom(1);
		return FALSE;
	}
}
//#endif

void CMarkTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	static  int  ConnectTime=0;
	CString str;
	static unsigned char RecData[8]={0};
	static unsigned char rdData;
	CString str0,str1,str2,str3;
	int a1=0;
	int a2=0;
	int a3=0; 
	int a4=0;
	switch(nIDEvent)
	{
	case 1:

		break;
	case 10:
		switch(TestStep)
		{
		case 0:
			if (SerialReadData("010109900001",&rdData))       //等待读取翻转信号M400:H0990
			{
				if ((rdData&0x01)==1)
				{
				    if (!SerialWriteData("010509900000"))   //复位m400
					{
						::MessageBoxA(m_hWnd,"复位m400信号失败！","提示",MB_OK);
					}
					 TestStep=1;
				}
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"读取启动测试信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
		case 1:
			str="010314060002";        //当前X轴位置D1030：H1406
			if (SerialReadData(str,RecData))
			{
				YData=(long)(RecData[0]*256+RecData[1]+(RecData[2]<<24)+(RecData[3]<<16));   //获得当前X轴位置
				//YData=(long)(RecData[4]*256+RecData[5]+(RecData[6]<<16)+(RecData[7]<<24));   //获得当前Y轴位置
				Sleep(1000);
				OnBnClickedButton7();   //触发抓取图片处理   gene
				TestStep=2;
			}
			else
			{
				//SerialWriteData("01050011FF00DC3F");         //表示读取失败
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"读取启动测试信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
		case 2:   //等待图片处理结束 
			      if(geneResultFlag)
				  {
					// XData+=Xpianyi*10000;    // 待 测试********
					 YData=Ypianyi*10000;  //一个像素 对应  多少脉冲        
					// Sleep(1000);
					  TestStep=3;
				  }
				  else
				  {
					  TestStep=7;
				  }
				 geneResultFlag=NULL;
			break;
		case 3:	
			a1= (YData&0x000000FF);   //提取最高位字节
			str0.Format("%.2X",a1);

			a2= (YData&0x0000FF00)>>8;
			str1.Format("%.2X",a2);
			
			a3= (YData&0x00FF0000)>>16;
			str2.Format("%.2X",a3);

			a4= (YData&0xFF000000)>>24;//提取最低位字节
			str3.Format("%.2X",a4);
			str="011017DA000204"+str1+str0+str3+str2;  
			if (SerialWriteData(str))       //发送X位置:D2010 H17DA
			{
				TestStep=4;
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
		case 4:
			sendData="0105092CFF00";
			if (SerialWriteData(sendData))       //发送调整命令m300（H092C）给PLC 
			{
					 TestStep=5;
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"读取启动测试信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
             break;
		case 5:			
			 if (SerialReadData("0101092C0001",&rdData))       //等待读取plc x y 轴调整完成信号M300（H092C）
			{
				if ((rdData&0x01)==0)
				{
					Sleep(1000);
					OnBnClickedButton8();                 //确认 
					if(doubleResultFlag==true)
					{
						OnBnClickedButton7(); 
						TestStep=2;
					}
					else
					{TestStep=7;}
					doubleResultFlag=NULL;
				}
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"读取启动测试信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
		case 6:			

			break;
		case 7:			
			sendData="0105092DFF00";
			if (SerialWriteData(sendData))       //发送下压命令m301（H092D）给PLC 
			{
					 TestStep=8;
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"发送下压命令信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
		case 8:			
			 if (SerialReadData("010109910001",&rdData))       //等待读取plc 动作完成信号M401（H0991）
			{
				if ((rdData&0x01)==1)
				{
					
					if (!SerialWriteData("010509910000"))   //复位m401（H0991）
					{
						::MessageBoxA(m_hWnd,"复位m401信号失败！","提示",MB_OK);
					}
					 TestStep=9;
				}
			}
			else
			{
				ConnectTime++;
				if (ConnectTime==5)
				{
					::MessageBoxA(m_hWnd,"读取动作完成信号失败！","提示",MB_OK);
					KillTimer(10);
					ConnectTime=0;
				}
			}
			break;
			case 9:		
				TestStep=10;
				Sleep(1000);
				OnBnClickedButton4();   //test	

			break;
			case 10:	
					if(testResultFlag==false)
					{
						TestStep=12;
						count1=0;
					}
					else
					{
						TestStep=11;
						count1=0;
					}
			break;
			case 11:	//shoudong
			if(testResultFlag==false)
			{
				TestStep=12;
				count1=0;
			}
			else
			{
				if(count1==3)
				{
					TestStep=12;
					count1=0;
				}
			}
			break;
			case 12:			
				sendData="0105092EFF00";
				if (SerialWriteData(sendData))       //发送测试完成命令m302（H092E）给PLC 
				{
						 TestStep=0;
				}
				else
				{
					ConnectTime++;
					if (ConnectTime==5)
					{
						::MessageBoxA(m_hWnd,"读取启动测试信号失败！","提示",MB_OK);
						KillTimer(10);
						ConnectTime=0;
					}
				}
			break;
		default: 
			break;
		}		
		break;
	case 100:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMarkTestDlg::OnBnClickedButton9()
{
	count1=0;
	SetTimer(10,250,NULL); //启动PLC定时
}

void CMarkTestDlg::OnBnClickedButton10()
{
	TestStep=0;
	KillTimer(10);
}

void CMarkTestDlg::OnBnClickedButton11()
{
	OnBnClickedButton10();
	OnBnClickedButton9();
}


void CMarkTestDlg::OnBnClickedButton3()
{
	light=1;
}


void CMarkTestDlg::OnBnClickedButton12()
{
	light=0;
}
