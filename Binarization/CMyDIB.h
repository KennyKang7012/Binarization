#include <afxwin.h>

class CMyDIB : public CObject
{
public:
    CMyDIB ();
    ~CMyDIB ();

    // ���Y
    BITMAPINFO  *DibInfo;
    // �����}�C
    BYTE        *DibBits;
    // �Ȧs�}�C
    BYTE        *DibArry;
    // ���J�I�}���ɮ�
    BOOL LoadBMPFile ( CFile &file );
    // ��ܥi�Y���I�}����
    BOOL ShowBMPFile ( CDC &cdc, CRect &crSrc, CRect &crDst );
    // �ഫ�I�}��ƤW�U��m
    BOOL ExchangeBits ( int iChange );

};