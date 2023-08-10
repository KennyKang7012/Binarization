#include <afxwin.h>

class CMyDIB : public CObject
{
public:
    CMyDIB ();
    ~CMyDIB ();

    // 標頭
    BITMAPINFO  *DibInfo;
    // 像素陣列
    BYTE        *DibBits;
    // 暫存陣列
    BYTE        *DibArry;
    // 載入點陣圖檔案
    BOOL LoadBMPFile ( CFile &file );
    // 顯示可縮放點陣圖檔
    BOOL ShowBMPFile ( CDC &cdc, CRect &crSrc, CRect &crDst );
    // 轉換點陣資料上下位置
    BOOL ExchangeBits ( int iChange );

};