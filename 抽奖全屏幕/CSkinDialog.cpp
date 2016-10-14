//
// CSkinDialog.cpp
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#include "stdafx.h"
#include "CSkinDialog.h"
#include "CUISkinXmlReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString strUserInput[3];
CRect rect;


/////////////////////////////////////////////////////////////////////////////
CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
	, mInitializing(true)
	, mDragging(false)
{
}

CSkinDialog::CSkinDialog(LPCTSTR lpszTemplateName, CWnd * pParentWnd)
	: CDialog(lpszTemplateName, pParentWnd)
	, mInitializing(true)
	, mDragging(false)
{
}

CSkinDialog::~CSkinDialog()
{
}

/*
void CSkinDialog::LoadDialogImages()
{
	CString dialogText;
	GetWindowText(dialogText);

	if (mTitleText == "")
	{
		if (dialogText != "")
		{
			SetTagName(dialogText);
		}
	}
	else
	{
		SetTagName(mTitleText);
	}

	CRgn windowRegion;
	LoadImageMap(windowRegion);
	if ((HRGN)windowRegion)
	{
		SetWindowRgn((HRGN)windowRegion, TRUE);

		CRect windowRect;
		GetWindowRect(&windowRect);
		if (mTitleText != "")
		{
			windowRect.left = mSkinXPos;
			windowRect.top  = mSkinYPos;
		}
		windowRect.right  = windowRect.left + mSkinWidth;
		windowRect.bottom = windowRect.top + mSkinHeight;
		MoveWindow(windowRect);
	}
}*/

void CSkinDialog::MoveDialog(CPoint& inCursorPos)
{
	// 不能移动窗口 [1/26/2011 Han Honglei]
	//CRect windowRect;
	//GetWindowRect(&windowRect);
	//windowRect.left += (inCursorPos.x - mCursorPos.x);
	//windowRect.right += (inCursorPos.x - mCursorPos.x);
	//windowRect.top += (inCursorPos.y - mCursorPos.y);
	//windowRect.bottom += (inCursorPos.y - mCursorPos.y);
	//MoveWindow(windowRect);
	//mCursorPos = inCursorPos;
}

BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	//{{AFX_MSG_MAP(CSkinDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSkinDialog::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint currentPoint;
	currentPoint.x = point.x;
	currentPoint.y = point.y;
	ClientToScreen(&currentPoint);
//	DoContextHelp(&currentPoint);

	CDialog::OnRButtonUp(nFlags, point);
}

BOOL CSkinDialog::OnInitDialog()
{
	BOOL pass = CDialog::OnInitDialog();

	LoadDialogImages();
	mInitializing = false;	

	m_nCur = 0;
	m_bStartRand = false;

	m_nMin = 1;					// 最小最大大数
	m_nMax = 200;
	m_nRefreshTime = 60;		// 数字刷新时间间隔
	m_nCurSpecial = 0;			// 当前选定的作弊数字
	m_nSpecialNum = 15;			// 作弊数字一共15个
	// 作弊数字
	int tmp[] = {27,44,101,65,72,128,133,154,79,11,39,86,120,5,95};
	memcpy(m_nvSpecial, tmp, m_nSpecialNum*sizeof(int));

	for(int i = 0; i < 100; i++)
	{
		m_GetNum[i] = 0 ;
	}
	// 得到屏幕的大小 [1/26/2011 Han Honglei]
	nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	nFullHeight=GetSystemMetrics(SM_CYSCREEN);    
	SetStretchBltMode (mImageProcessor.GetImageDC(), STRETCH_HALFTONE);
	StretchBlt(mImageProcessor.GetImageDC(),0, 0,nFullWidth,nFullHeight, mImageProcessor.GetImageDC(), 0, 0,mSkinWidth,mSkinHeight, SRCCOPY );

	return pass;
}
/////////////////////////////////////////////////////////////////////////////
// CSkinDialog message handlers

void CSkinDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (mImageProcessor.IsReady())
	{
		CDC* pImageDC = CDC::FromHandle(mImageProcessor.GetImageDC());

		dc.BitBlt(0, 0, nFullWidth,nFullHeight, pImageDC, 0, 0, SRCCOPY);

		AddFontResource(_T("309.ttf"/*"Candara.ttf"*/));

		CString   s;   
		CFont font;
		dc.SetBkMode(TRANSPARENT);

		VERIFY(font.CreateFont(
			310*nFullWidth/1920.0,                       // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("309"/*"Candara"*/)));           // lpszFacename


		// Do something with the font just created...
		CFont* def_font = dc.SelectObject(&font);
		dc.SetTextColor(RGB(246,227,39));   

		int DrawCur = m_nCur;
		if (!m_bStartRand && m_nCur > 0)
		{
			DrawCur = m_nCur -1;
		}


		s.Format(_T("%d"),m_GetNum[DrawCur]);//该函数和c里的printf差不多.   

		int Value[3];
		if (m_GetNum[DrawCur] > 99)
		{
			Value[0] = m_GetNum[DrawCur]/100; //得到百位数 比如1200/100=12%10=2;
			Value[1] = m_GetNum[DrawCur]/10%10; //得到百位数 比如1200/100=12%10=2;
			Value[2] = m_GetNum[DrawCur]%10; //得到百位数 比如1200/100=12%10=2;

		}
		else if(m_GetNum[DrawCur] > 9)
		{
			Value[0] = 0; //得到百位数 比如1200/100=12%10=2;
			Value[1] = m_GetNum[DrawCur]/10%10; //得到百位数 比如1200/100=12%10=2;
			Value[2] = m_GetNum[DrawCur]%10; //得到百位数 比如1200/100=12%10=2;
		}
		else
		{
			Value[0] = 0; //得到百位数 比如1200/100=12%10=2;
			Value[1] = 0; //得到百位数 比如1200/100=12%10=2;
			Value[2] = m_GetNum[DrawCur]; //得到百位数 比如1200/100=12%10=2;
		}	
		float fTextX1 = nFullWidth/2.8;
		float fTextX2 = nFullWidth/2.17;
		float fTextX3 = nFullWidth/1.78;
		float fTextX[3] = {fTextX1, fTextX2, fTextX3};
		float fTextY = nFullHeight/2.5;
		for (int i = 0; i < 3; i++)
		{
			s.Format(_T("%d"),Value[i]);
			dc.TextOut(fTextX[i], fTextY, s);
			//dc.TextOut(700+i*250, 450, s);
		}
		// Done with the font.  Delete the font object.
		font.DeleteObject(); 
	}
	else
	{
		CRect windowRect;
		GetClientRect(&windowRect);

		HBRUSH myBrush = ::CreateSolidBrush(RGB(128, 128, 128));
		if (myBrush)
		{
			::FillRect(dc.GetSafeHdc(), windowRect, myBrush);
			::DeleteObject(myBrush);
		}
	}
}

BOOL CSkinDialog::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSkinDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!mDragging)
	{
		SetCapture();
		mDragging = true;
		GetCursorPos(&mCursorPos);
		CRect r(20,20,GetSystemMetrics(SM_CXMAXIMIZED)-10,GetSystemMetrics(SM_CYMAXIMIZED)-10);
		::ClipCursor(r);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CSkinDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (mDragging)
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		MoveDialog(cursorPos);
		mDragging = false;
		ReleaseCapture();
		::ClipCursor(NULL);
		this->SetFocus ();
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CSkinDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (mDragging && nFlags == MK_LBUTTON)
	{
		CPoint cursorPos;
		GetCursorPos(&cursorPos);
		MoveDialog(cursorPos);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CSkinDialog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);	
}

BOOL CSkinDialog::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_SYSMENU;
	return CDialog::PreCreateWindow(cs);
}

void CSkinDialog::OnNcDestroy() 
{
	CDialog::OnNcDestroy();
}

int CSkinDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	LONG lg = GetWindowLong(this->m_hWnd ,GWL_STYLE);
	lg|=WS_SYSMENU;
	SetWindowLong(this->m_hWnd ,GWL_STYLE,lg);
	lpCreateStruct->style |= WS_SYSMENU   ;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CSkinDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default


	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSkinDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bStartRand)
	{
		m_GetNum[m_nCur] = (rand() % (m_nMax-m_nMin+1)) + m_nMin ;
	}
	Invalidate(true);
	__super::OnTimer(nIDEvent);
}
