#ifndef _MV_IMAGE_H
#define _MV_IMAGE_H

#ifdef MVGIGE_EXPORTS
#define MVGIGE_API __declspec(dllexport)
#else
#define MVGIGE_API __declspec(dllimport)
#endif

class MVGIGE_API MVImage
{
public:
	MVImage();
	~MVImage();

	int Create( int nWidth, int nHeight, int nBPP );
	BOOL IsNull();
	int GetWidth();
	int GetHeight();
	void* GetBits();
	int	GetPitch();
	int	GetBPP();
	HBITMAP GetHBitmap();
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight );
	BOOL Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight );
	BOOL Draw( HDC hDestDC, int xDest, int yDest );
	int Save(LPCTSTR pszFileName);
	void *GetCImageHandle();
	void Destroy();
	HDC GetDC();
	void ReleaseDC();
protected:
private:
	HANDLE m_hImage;
};

#endif