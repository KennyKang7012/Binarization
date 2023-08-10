// BinarizationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Binarization.h"
#include "BinarizationDlg.h"

#include "CMyDIB.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBinarizationDlg dialog




CBinarizationDlg::CBinarizationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinarizationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBinarizationDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CBinarizationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBinarizationDlg message handlers

BOOL CBinarizationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBinarizationDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBinarizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBinarizationDlg::OnBnClickedOk()
{
    CFile *fA = new CFile;
    if ( fA->Open(L".\\B_01.bmp", CFile::modeRead ) == false )
    {
        AfxMessageBox(L"B_01.bmp File Open Error ",MB_OK);
        return;
    }
    CFile *fB = new CFile;
    if ( fB->Open(L".\\B_02.bmp", CFile::modeRead ) == false )
    {
        AfxMessageBox(L"B_02.bmp File Open Error ",MB_OK);
        return;
    }


    CMyDIB *imageA = new CMyDIB;
    CMyDIB *imageB = new CMyDIB;

    bool bRetA = imageA->LoadBMPFile( *fA );
    bool bRetB = imageB->LoadBMPFile( *fB );

    imageA->ExchangeBits(1);
    imageB->ExchangeBits(1);

    long lIDXA = 0;
    long lIDXB = 0;

    double byteRGB_RA = 0;
    double byteRGB_GA = 0;
    double byteRGB_BA = 0;
    double dobYUV_YA  = 0;
    double dobYUV_UA  = 0;
    double dobYUV_VA  = 0;

    double byteRGB_RB = 0;
    double byteRGB_GB = 0;
    double byteRGB_BB = 0;
    double dobYUV_YB  = 0;
    double dobYUV_UB  = 0;
    double dobYUV_VB  = 0;
    // The height of image.
    for ( int iY = 0; iY < imageA->DibInfo->bmiHeader.biHeight; iY++ )
    {
        // The width of image.
        for ( int iX = 0; iX < imageA->DibInfo->bmiHeader.biWidth; iX++ )
        {
            // The index of pixel, because our use the three depth bit to present one pixel of color,
            // Therefore, we have to multiple three.
            lIDXA = ( iX * 3 ) + ( iY * imageA->DibInfo->bmiHeader.biWidth * 3 );
            // To get pixel depth of blue channel,
            byteRGB_BA = imageA->DibArry[lIDXA+0];
            // To get pixel depth of green channel.
            byteRGB_GA = imageA->DibArry[lIDXA+1];
            // To get pixel depth of red channel.
            byteRGB_RA = imageA->DibArry[lIDXA+2];
            // To transform RGB to Y (gray scale).
            dobYUV_YA =  (0.299 * byteRGB_RA + 0.587 * byteRGB_GA + 0.114 * byteRGB_BA);
            // Setting our thresholds, To decide which pixel become to white. 
            if ( dobYUV_YA > 60 && dobYUV_YA < 160 )
            {
                lIDXB = ( iX * 3 ) + ( iY * imageB->DibInfo->bmiHeader.biWidth * 3 );
                imageB->DibArry[lIDXB+0] = 255;
                imageB->DibArry[lIDXB+1] = 255;
                imageB->DibArry[lIDXB+2] = 255;
            }
            // otherwise, those pixels will be black.
            else
            {
                lIDXB = ( iX * 3 ) + ( iY * imageB->DibInfo->bmiHeader.biWidth * 3 );
                imageB->DibArry[lIDXB+0] = 0;
                imageB->DibArry[lIDXB+1] = 0;
                imageB->DibArry[lIDXB+2] = 0;
            }
        } // for ( int iX = 0; iX < imageA->DibInfo->bmiHeader.biWidth; iX++ )
    } // for ( int iY = 0; iY < imageA->DibInfo->bmiHeader.biHeight; iY++ )

    imageA->ExchangeBits(2);
    imageB->ExchangeBits(2);

    CClientDC dc( GetDlgItem(IDC_Picture ) );
    ::StretchDIBits ( dc.m_hDC,	
                      1,	
                      1,	
                      512,
                      512,
                      1,	
                      1,	
                      512,
                      512,
                      imageB->DibBits,	
                      imageB->DibInfo,	
                      DIB_RGB_COLORS,	
                      SRCCOPY );	

    delete imageA;
    delete imageB;

    fA->Close();
    fB->Close();

    delete fA;
    delete fB;
}
