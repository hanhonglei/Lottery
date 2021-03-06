// ImageViewerDlg.cpp : implementation file
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "ImageViewer.h"
#include "ImageViewerDlg.h"
#include ".\imageviewerdlg.h"
#include "safe_defs.h"
#include "ImageProcessor.h"
#include "MiscUtils.h"
#include "CUISkinXmlReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString strUserInput[3];


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageViewerDlg dialog

int nUserInput[3];

CImageViewerDlg::CImageViewerDlg(CWnd* pParent /*=NULL*/)
: INHERITED(CImageViewerDlg::IDD, pParent)
, mImageFile("")
, mProcessor(new ImageProcessor())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Create internal components.
	CUISkinXmlReader::CreateInstance();
	nUserInput[0] = 1;
	nUserInput[1] = 150;
	nUserInput[2] = 60;
	for (int i = 0 ; i < 3; i++)
	{
		CAboutDlg dlgAbout;

		dlgAbout.DoModal();
	}

}

CImageViewerDlg::~CImageViewerDlg()
{
	SAFE_DELETE(mProcessor);

	// Delete internal components.
	CUISkinXmlReader::DeleteInstance();
}

void CImageViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	INHERITED::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, mPictureWnd);
	//DDX_Control(pDX, IDC_BUTTON_OPEN, mButtonOpen);
	//DDX_Control(pDX, IDC_BUTTON_MIN, mButtonMinimize);
	//DDX_Control(pDX, IDC_BUTTON_EXIT, mButtonExit);
	//DDX_Control(pDX, IDC_BUTTON_RELOAD, mButtonReload);
	//DDX_Control(pDX, IDC_BUTTON_CLEAN, mButtonClean);
	//DDX_Control(pDX, IDC_BUTTON_INVERT, mButtonInvert);
	//DDX_Control(pDX, IDC_BUTTON_SAVE_AS, mButtonSaveAs);
	//DDX_Control(pDX, IDC_BUTTON_OVERLAY_LOGO, mButtonLogo);
	//DDX_Control(pDX, IDC_BUTTON_OVERLAY_SYSTIME, mButtonSysTime);
	//DDX_Control(pDX, IDC_BUTTON_ABOUT, mButtonAbout);
}

BEGIN_MESSAGE_MAP(CImageViewerDlg, INHERITED)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnBnClickedButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY_LOGO, OnBnClickedButtonOverlayLogo)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAY_SYSTIME, OnBnClickedButtonOverlaySystime)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnBnClickedButtonClean)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_GREYSCALE, OnBnClickedButtonGreyscale)
	ON_BN_CLICKED(IDC_BUTTON_MIN, OnBnClickedButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnBnClickedButtonExit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CImageViewerDlg message handlers

BOOL CImageViewerDlg::OnInitDialog()
{
	INHERITED::OnInitDialog();
	SetWindowText("ImageViewer");

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	///////////////////////////////////////////////////////////////////////
	// Load skins...
	//mButtonMinimize.LoadButtonImages();
	//mButtonExit.LoadButtonImages();
	//mButtonOpen.LoadButtonImages();
	//mButtonReload.LoadButtonImages();
	//mButtonClean.LoadButtonImages();
	//mButtonInvert.LoadButtonImages();
	//mButtonSaveAs.LoadButtonImages();
	//mButtonLogo.LoadButtonImages();
	//mButtonSysTime.LoadButtonImages();
	//mButtonAbout.LoadButtonImages();


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CImageViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		INHERITED::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageViewerDlg::OnPaint() 
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
		INHERITED::OnPaint();

		DisplayImage();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageViewerDlg::LoadDialogImages()
{
	// Get the skin for the main window.
	CUISkinXmlReader* pReader = CUISkinXmlReader::GetInstance();
	mSkinFileName = pReader->GetMainWindowSkin();

	CRgn windowRegion;
	LoadImageMap(windowRegion);
	if ((HRGN)windowRegion)
	{
		SetWindowRgn((HRGN)windowRegion, TRUE);
	}

	// Adjust properties for the image box
	long x, y, width, height;
	if (pReader->GetImageBox(x, y, width, height))
	{
		CRect rect(x, y, x+width, y+height);
		mPictureWnd.MoveWindow(&rect);
	}
}

void CImageViewerDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CImageViewerDlg::OnBnClickedButtonOpen()
{
	CString imageFile;
	if (MiscUtils::BrowseImageFile(imageFile))
	{
		mImageFile = imageFile;

		if (mProcessor->LoadFile(mImageFile))
		{
			DisplayImage();
		}
	}
}

void CImageViewerDlg::OnBnClickedButtonSaveAs()
{
	CString destFile;
	if (MiscUtils::SaveAsFile(destFile))
	{
		if (mProcessor)
		{
			mProcessor->SaveFile(destFile);
		}
	}
}

void CImageViewerDlg::OnBnClickedButtonReload()
{
	if (mProcessor)
	{
		mProcessor->Reload();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonClean()
{
	if (mProcessor)
	{
		mProcessor->Clean();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonInvert()
{
	if (mProcessor)
	{
		mProcessor->Invert();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonGreyscale()
{
	if (mProcessor)
	{
		mProcessor->Greyscale();
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonOverlayLogo()
{
	if (mProcessor)
	{
		CString logoFile = MiscUtils::GetAppFolder() + "\\logo.gif";
		mProcessor->OverlayLogo(0, 0, logoFile);
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonOverlaySystime()
{
	if (mProcessor)
	{
		char* szTime = "0000-00-00(00:00:00)";
		SIZE txtSize;
		::GetTextExtentPoint32(::GetWindowDC(NULL), szTime, (int)strlen(szTime), &txtSize);

		long startX = 0;
		long StartY = 0;
		long imageWidth, imageHeight;
		if (mProcessor->GetInfo(&imageWidth, &imageHeight))
		{
			startX = imageWidth - txtSize.cx;
		}

		mProcessor->OverlaySystemTime(startX, StartY);
		DisplayImage();
	}
}

void CImageViewerDlg::OnBnClickedButtonMin()
{
	ShowWindow(SW_MINIMIZE);
}

void CImageViewerDlg::OnBnClickedButtonExit()
{
	INHERITED::OnOK();
}

void CImageViewerDlg::DisplayImage(void)
{
	if (mProcessor)
	{
		mProcessor->Display(&mPictureWnd);
	}
}

int nInputNum = 0;

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	if (nInputNum >= 3)
		return TRUE;

	strUserInput[nInputNum].Format(_T("%d"), nUserInput[nInputNum]);

	CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_EDIT1);   
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC);

	CString s;
	switch(nInputNum)
	{
	case 0:
		s.Format(_T("%s"), "输入起始值");
		break;
	case 1:
		s.Format(_T("%s"), "输入结束值");
		break;
	case 2:
		s.Format(_T("%s"), "输入刷新速度");
		break;
	}
	pStatic->SetWindowText(s);

	//付值   
	pEdit->SetWindowText(strUserInput[nInputNum]);   
		nInputNum+=1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CEdit* pEdit   =   (CEdit*)  GetDlgItem(IDC_EDIT1); 
	pEdit->GetWindowText(strUserInput[nInputNum-1]);

	OnOK();
}

void CImageViewerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CSkinDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CImageViewerDlg::OnOK()
{
	m_nMin = atoi(strUserInput[0].GetString());
	m_nMax = atoi(strUserInput[1].GetString());
	m_nRefreshTime = atoi(strUserInput[2].GetString());

	srand( (unsigned)time( NULL ) );


	//if(m_nCur >= 10)
	//	return;
	KillTimer(1);
	m_bStartRand = !m_bStartRand;

	if(m_nCur > (m_nMax - m_nMin))
	{
		m_GetNum[m_nCur] = 0;
		m_bStartRand = true;
		Invalidate();
		return;
	}

	if(!m_bStartRand)
	{
		m_nCur+=1;
		bool bFault = true;
		while(bFault)
		{
			bFault = false;
			for(int i = 0; i < m_nCur-1; i++)
			{
				if(m_GetNum[m_nCur-1] == m_GetNum[i] || m_GetNum[m_nCur-1] == m_nSpecial)
				{
					m_GetNum[m_nCur-1] = (rand() % (m_nMax-m_nMin+1)) + m_nMin ;
					bFault = true;
					Invalidate(true);

				}
			}
		}
		// 如果特殊作弊数字没有被选中的话，有一定几率当前值为特殊数，但10次内肯定出现一次
		if(!m_bSpecial && m_nCur <= 20)
		{
			int randmNum = (m_nCur > 10)?(20-m_nCur+1):(10-m_nCur+1);
			if (rand()%randmNum == 0)
			{
				m_GetNum[m_nCur-1] = m_nSpecial;
				m_bSpecial = true;
			}
		}
		if (m_nCur == 10 || m_nCur == 20)
		{
			m_bSpecial = false;
		}
	}
	else
	{
		m_GetNum[m_nCur] = (rand() % (m_nMax-m_nMin+1)) + m_nMin ;
		SetTimer(1, m_nRefreshTime, 0);
	}
	Invalidate(true);	
}
