#include <stdafx.h>
#include "CMyDIB.h"

// 建構子
CMyDIB::CMyDIB()
{
    DibInfo = NULL;
    DibBits = NULL;
    DibArry = NULL;
}

// 解構子
CMyDIB::~CMyDIB()
{
    delete DibInfo;
    delete [] DibBits;
    delete [] DibArry;
}

// 載入點陣圖檔案
BOOL CMyDIB::LoadBMPFile ( CFile &file )
{
    BITMAPFILEHEADER bmBFH;
    BITMAPINFOHEADER bmBIH;
    long      lSize;
    ULONGLONG ulHeadPos;
    int       iPaletteSize = 0;
    int       iRet;
    int       iBMPHeaderSize;
    // 讀取檔案指標位置
    ulHeadPos = file.GetPosition();
    // 讀取點陣圖 Bitmap File Header
    iRet = file.Read ( &bmBFH, sizeof(BITMAPFILEHEADER) );
    // 檢查點陣圖檔頭大小、點陣圖識別字串 
    if ( iRet != sizeof(BITMAPFILEHEADER) ||
         bmBFH.bfType != 0x4d42 )
    {
        // 傳回錯誤值
        return FALSE;
    }
    // 讀取點陣圖 Bitmap Info Header
    iRet = file.Read ( &bmBIH, sizeof(BITMAPINFOHEADER) );
    // 檢查點陣圖資訊檔頭大小
    if ( iRet != sizeof(BITMAPINFOHEADER) )
    {
        // 傳回錯誤值
        return FALSE;
    }
    // 判斷 RGBQUAD 大小
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
    // 計算 BITMAPINFO 大小
    iBMPHeaderSize = sizeof(BITMAPINFOHEADER) + 
        ( iPaletteSize * sizeof ( RGBQUAD ) );
    // 分配 BITMAPINFO 記憶體
    DibInfo = (BITMAPINFO*) new BYTE [ iBMPHeaderSize ];
    // 設定 Bitmap Info Header
    DibInfo->bmiHeader = bmBIH;
    // 倘若有使用調色盤
    if ( iPaletteSize > 0 )
    {
        // 讀取調色盤
        iRet = file.Read( &(DibInfo->bmiColors[0]),
            iPaletteSize * sizeof (RGBQUAD) );
        // 檢查調色盤大小
        if ( iRet != int( iPaletteSize * sizeof( RGBQUAD) ) )
        {
            // 釋放點陣圖資訊檔頭與調色盤
            delete [] DibInfo;
            // 清空點陣圖資訊檔頭與調色盤
            DibInfo = NULL;
            // 傳回錯誤值
            return FALSE;
        }
    }
    // ????????????????
    DWORD dwBytesPerLine;
    dwBytesPerLine = ( DibInfo->bmiHeader.biWidth * DibInfo->bmiHeader.biBitCount + 7 ) / 8;
    dwBytesPerLine = ( dwBytesPerLine + 3 ) / 4;
    dwBytesPerLine = dwBytesPerLine * 4;    
    // 計算像素陣列大小
    lSize = dwBytesPerLine * DibInfo->bmiHeader.biHeight;
    // 分配像素陣列記憶體
    DibBits = (BYTE*) new BYTE [ lSize ];
    DibArry = (BYTE*) new BYTE [ lSize ];
    // 移動檔案指標到像素陣列
    file.Seek( ulHeadPos + bmBFH.bfOffBits, CFile::begin );
    // 由點陣圖檔案讀取像素資料到記憶體
    iRet = file.Read ( DibBits, lSize );
    // 檢查讀取資料大小
    if ( iRet != int( lSize ) )
    {
        // 倘若資料大小不正確
        delete [] DibInfo;
        delete [] DibBits;
        DibInfo = NULL;
        DibBits = NULL;
        // 傳回錯誤值
        return FALSE;
    }
    // 傳回正確值
    return TRUE;
}

// 轉換點陣資料上下位置
BOOL CMyDIB::ExchangeBits ( int iChange )
{
    long lIndex1 = 0;
    long lX1 = 0;
    long lY1 = 0;
    long lIndex2 = 0;
    long lX2 = 0;
    long lY2 = 0;
    // 處理
    try
    {
        // 將像素資料轉到暫存陣列中
        if ( iChange == 1 )
        {
            // 變數初始歸零
            lIndex1 = 0;
            lIndex2 = 0;
            lX1 = 0;
            lX2 = 0;
            lY1 = 0;
            lY2 = 0;
            // Y 軸由下往上
            for ( lY1 = DibInfo->bmiHeader.biHeight - 1; lY1 >= 0 ; lY1-- )
            {
                // X 軸由左往右
                for ( lX1 = 0; lX1 < DibInfo->bmiHeader.biWidth; lX1++ )
                {
                    // 索引1由下往上
                    lIndex1 = ( lX1 * 3 ) + ( lY1 * DibInfo->bmiHeader.biWidth * 3 );
                    // 索引2由上往下
                    lIndex2 = ( lX2 * 3 ) + ( lY2 * DibInfo->bmiHeader.biWidth * 3 );
                    // 將資料轉為由上往下到暫存陣列中
                    DibArry[lIndex2+0] = DibBits[lIndex1+0];
                    DibArry[lIndex2+1] = DibBits[lIndex1+1];
                    DibArry[lIndex2+2] = DibBits[lIndex1+2];
                    // 暫存 X 軸由左往右
                    lX2++;
                }
                // 暫存 X 軸歸零
                lX2 = 0;
                // 暫存 Y 軸由上往下
                lY2++;
            }
        }
        // 將暫存資料轉到像素陣列中
        else if ( iChange == 2 )
        {
            // 變數初始歸零
            lIndex1 = 0;
            lIndex2 = 0;
            lX1 = 0;
            lX2 = 0;
            lY1 = 0;
            lY2 = 0;
            // Y 軸由下往上
            for ( lY1 = DibInfo->bmiHeader.biHeight - 1; lY1 >= 0 ; lY1-- )
            {
                // X 軸由左往右
                for ( lX1 = 0; lX1 < DibInfo->bmiHeader.biWidth; lX1++ )
                {
                    // 索引1由下往上
                    lIndex1 = ( lX1 * 3 ) + ( lY1 * DibInfo->bmiHeader.biWidth * 3 );
                    // 索引2由上往下
                    lIndex2 = ( lX2 * 3 ) + ( lY2 * DibInfo->bmiHeader.biWidth * 3 );
                    // 將暫存資料轉入像素資料中
                    DibBits[lIndex2 + 0] = DibArry[lIndex1 + 0];
                    DibBits[lIndex2 + 1] = DibArry[lIndex1 + 1];
                    DibBits[lIndex2 + 2] = DibArry[lIndex1 + 2];
                    // 暫存 X 軸由左往右
                    lX2++;
                }
                // 暫存 X 軸歸零
                lX2 = 0;
                // 暫存 Y 軸由上往下
                lY2++;
            }
        }
    }
    // 錯誤處理
    catch (CException* e)
    {
        // 傳回錯誤值
        return FALSE;
    }
    // 傳回成功值
    return TRUE;
}

// 顯示點陣圖檔
BOOL CMyDIB::ShowBMPFile ( CDC &cdc, CRect &crSrc, CRect &crDst )
{
    int iRet;
    // 檢查點陣圖檔頭資訊
    if ( !DibInfo )
    {
        // 若有錯，則傳回錯誤
        return FALSE;
    }
    // 顯示可縮放點陣圖檔
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
    // 檢查是否有顯示
    if ( !iRet )
    {
        // 傳回錯誤值
        return FALSE;
    }
    // 傳回正確值
    return TRUE;
}