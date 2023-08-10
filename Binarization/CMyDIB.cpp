#include <stdafx.h>
#include "CMyDIB.h"

// �غc�l
CMyDIB::CMyDIB()
{
    DibInfo = NULL;
    DibBits = NULL;
    DibArry = NULL;
}

// �Ѻc�l
CMyDIB::~CMyDIB()
{
    delete DibInfo;
    delete [] DibBits;
    delete [] DibArry;
}

// ���J�I�}���ɮ�
BOOL CMyDIB::LoadBMPFile ( CFile &file )
{
    BITMAPFILEHEADER bmBFH;
    BITMAPINFOHEADER bmBIH;
    long      lSize;
    ULONGLONG ulHeadPos;
    int       iPaletteSize = 0;
    int       iRet;
    int       iBMPHeaderSize;
    // Ū���ɮ׫��Ц�m
    ulHeadPos = file.GetPosition();
    // Ū���I�}�� Bitmap File Header
    iRet = file.Read ( &bmBFH, sizeof(BITMAPFILEHEADER) );
    // �ˬd�I�}�����Y�j�p�B�I�}���ѧO�r�� 
    if ( iRet != sizeof(BITMAPFILEHEADER) ||
         bmBFH.bfType != 0x4d42 )
    {
        // �Ǧ^���~��
        return FALSE;
    }
    // Ū���I�}�� Bitmap Info Header
    iRet = file.Read ( &bmBIH, sizeof(BITMAPINFOHEADER) );
    // �ˬd�I�}�ϸ�T���Y�j�p
    if ( iRet != sizeof(BITMAPINFOHEADER) )
    {
        // �Ǧ^���~��
        return FALSE;
    }
    // �P�_ RGBQUAD �j�p
    switch ( bmBIH.biBitCount )
    {
    case 1:
        iPaletteSize = 2;
        break;
    case 4:
        iPaletteSize = 16;
        break;
    case 8:
        iPaletteSize = 256;
        break;
    }
    // �p�� BITMAPINFO �j�p
    iBMPHeaderSize = sizeof(BITMAPINFOHEADER) + 
        ( iPaletteSize * sizeof ( RGBQUAD ) );
    // ���t BITMAPINFO �O����
    DibInfo = (BITMAPINFO*) new BYTE [ iBMPHeaderSize ];
    // �]�w Bitmap Info Header
    DibInfo->bmiHeader = bmBIH;
    // �խY���ϥνզ�L
    if ( iPaletteSize > 0 )
    {
        // Ū���զ�L
        iRet = file.Read( &(DibInfo->bmiColors[0]),
            iPaletteSize * sizeof (RGBQUAD) );
        // �ˬd�զ�L�j�p
        if ( iRet != int( iPaletteSize * sizeof( RGBQUAD) ) )
        {
            // �����I�}�ϸ�T���Y�P�զ�L
            delete [] DibInfo;
            // �M���I�}�ϸ�T���Y�P�զ�L
            DibInfo = NULL;
            // �Ǧ^���~��
            return FALSE;
        }
    }
    // ????????????????
    DWORD dwBytesPerLine;
    dwBytesPerLine = ( DibInfo->bmiHeader.biWidth * DibInfo->bmiHeader.biBitCount + 7 ) / 8;
    dwBytesPerLine = ( dwBytesPerLine + 3 ) / 4;
    dwBytesPerLine = dwBytesPerLine * 4;    
    // �p�⹳���}�C�j�p
    lSize = dwBytesPerLine * DibInfo->bmiHeader.biHeight;
    // ���t�����}�C�O����
    DibBits = (BYTE*) new BYTE [ lSize ];
    DibArry = (BYTE*) new BYTE [ lSize ];
    // �����ɮ׫��Ш칳���}�C
    file.Seek( ulHeadPos + bmBFH.bfOffBits, CFile::begin );
    // ���I�}���ɮ�Ū��������ƨ�O����
    iRet = file.Read ( DibBits, lSize );
    // �ˬdŪ����Ƥj�p
    if ( iRet != int( lSize ) )
    {
        // �խY��Ƥj�p�����T
        delete [] DibInfo;
        delete [] DibBits;
        DibInfo = NULL;
        DibBits = NULL;
        // �Ǧ^���~��
        return FALSE;
    }
    // �Ǧ^���T��
    return TRUE;
}

// �ഫ�I�}��ƤW�U��m
BOOL CMyDIB::ExchangeBits ( int iChange )
{
    long lIndex1 = 0;
    long lX1 = 0;
    long lY1 = 0;
    long lIndex2 = 0;
    long lX2 = 0;
    long lY2 = 0;
    // �B�z
    try
    {
        // �N����������Ȧs�}�C��
        if ( iChange == 1 )
        {
            // �ܼƪ�l�k�s
            lIndex1 = 0;
            lIndex2 = 0;
            lX1 = 0;
            lX2 = 0;
            lY1 = 0;
            lY2 = 0;
            // Y �b�ѤU���W
            for ( lY1 = DibInfo->bmiHeader.biHeight - 1; lY1 >= 0 ; lY1-- )
            {
                // X �b�ѥ����k
                for ( lX1 = 0; lX1 < DibInfo->bmiHeader.biWidth; lX1++ )
                {
                    // ����1�ѤU���W
                    lIndex1 = ( lX1 * 3 ) + ( lY1 * DibInfo->bmiHeader.biWidth * 3 );
                    // ����2�ѤW���U
                    lIndex2 = ( lX2 * 3 ) + ( lY2 * DibInfo->bmiHeader.biWidth * 3 );
                    // �N����ର�ѤW���U��Ȧs�}�C��
                    DibArry[lIndex2+0] = DibBits[lIndex1+0];
                    DibArry[lIndex2+1] = DibBits[lIndex1+1];
                    DibArry[lIndex2+2] = DibBits[lIndex1+2];
                    // �Ȧs X �b�ѥ����k
                    lX2++;
                }
                // �Ȧs X �b�k�s
                lX2 = 0;
                // �Ȧs Y �b�ѤW���U
                lY2++;
            }
        }
        // �N�Ȧs�����칳���}�C��
        else if ( iChange == 2 )
        {
            // �ܼƪ�l�k�s
            lIndex1 = 0;
            lIndex2 = 0;
            lX1 = 0;
            lX2 = 0;
            lY1 = 0;
            lY2 = 0;
            // Y �b�ѤU���W
            for ( lY1 = DibInfo->bmiHeader.biHeight - 1; lY1 >= 0 ; lY1-- )
            {
                // X �b�ѥ����k
                for ( lX1 = 0; lX1 < DibInfo->bmiHeader.biWidth; lX1++ )
                {
                    // ����1�ѤU���W
                    lIndex1 = ( lX1 * 3 ) + ( lY1 * DibInfo->bmiHeader.biWidth * 3 );
                    // ����2�ѤW���U
                    lIndex2 = ( lX2 * 3 ) + ( lY2 * DibInfo->bmiHeader.biWidth * 3 );
                    // �N�Ȧs�����J������Ƥ�
                    DibBits[lIndex2 + 0] = DibArry[lIndex1 + 0];
                    DibBits[lIndex2 + 1] = DibArry[lIndex1 + 1];
                    DibBits[lIndex2 + 2] = DibArry[lIndex1 + 2];
                    // �Ȧs X �b�ѥ����k
                    lX2++;
                }
                // �Ȧs X �b�k�s
                lX2 = 0;
                // �Ȧs Y �b�ѤW���U
                lY2++;
            }
        }
    }
    // ���~�B�z
    catch (CException* e)
    {
        // �Ǧ^���~��
        return FALSE;
    }
    // �Ǧ^���\��
    return TRUE;
}

// ����I�}����
BOOL CMyDIB::ShowBMPFile ( CDC &cdc, CRect &crSrc, CRect &crDst )
{
    int iRet;
    // �ˬd�I�}�����Y��T
    if ( !DibInfo )
    {
        // �Y�����A�h�Ǧ^���~
        return FALSE;
    }
    // ��ܥi�Y���I�}����
    iRet = ::StretchDIBits ( cdc.m_hDC,	
                             crDst.left,	
                             crDst.top,	
                             crDst.Width(),
                             crDst.Height(),
                             crSrc.left,	
                             crSrc.top,	
                             crSrc.Width(),
                             crSrc.Height(),
                             DibBits,	
                             DibInfo,	
                             DIB_RGB_COLORS,	
                             SRCCOPY );	
    // �ˬd�O�_�����
    if ( !iRet )
    {
        // �Ǧ^���~��
        return FALSE;
    }
    // �Ǧ^���T��
    return TRUE;
}