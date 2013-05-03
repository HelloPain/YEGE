﻿#ifndef Inc_ege_e_head_h_
#define Inc_ege_e_head_h_

#include "ege.h"
#include "thread_queue.h"

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

#ifndef ASSERT_TRUE
#ifdef _DEBUG
#include <cassert>
#define ASSERT_TRUE(e) assert((e) != MUSIC_ERROR)
#else
#define ASSERT_TRUE(e) (void(0))
#endif
#endif

#include <gdiplus.h>

#define QUEUE_LEN           1024
#define UPDATE_MAX_CALL     0xFF
#define RENDER_TIMER_ID     916
#define IMAGE_INIT_FLAG     0x20100916
#define FLOAT_EPS           1e-3f

#define IFATODOB(A, B)  ( (A) && (B, 0) )
#define IFNATODOB(A, B) ( (A) || (B, 0) )

#define CONVERT_IMAGE(pimg) ( ((size_t)(pimg)<0x20 ?\
							   ((pimg) ?\
								(graph_setting.img_page[(size_t)(pimg) & 0xF])\
								: (--graph_setting.update_mark_count, graph_setting.imgtarget))\
								   : pimg) )

#define CONVERT_IMAGE_CONST(pimg) ( (size_t)(pimg)<0x20 ?\
									((pimg) ?\
									 (graph_setting.img_page[(size_t)(pimg) & 0xF])\
									 : graph_setting.imgtarget)\
										: pimg)

#define CONVERT_IMAGE_F(pimg) CONVERT_IMAGE(pimg)

#define CONVERT_IMAGE_F_CONST(pimg) CONVERT_IMAGE_CONST(pimg)

#define CONVERT_IMAGE_END

#ifndef DEFAULT_CHARSET
#define DEFAULT_CHARSET ANSI_CHARSET
#endif

#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifndef __int3264
#if defined(_WIN64)
typedef __int64 ::LONG_PTR, *P::LONG_PTR;
typedef unsigned __int64 ::ULONG_PTR, *PULONG_PTR;

#define __int3264   __int64

#else
typedef _W64 long LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

#define __int3264   __int32

#endif
#endif

typedef ::ULONG_PTR DWORD_PTR, *PDWORD_PTR;
typedef unsigned int uint32;
typedef intptr_t POINTER_SIZE;

#ifndef _MSC_VER
#define GRADIENT_FILL_RECT_H 0x00000000
#define GRADIENT_FILL_RECT_V 0x00000001
#define GRADIENT_FILL_TRIANGLE 0x00000002
#define GRADIENT_FILL_OP_FLAG 0x000000ff

extern "C"
{
	WINGDIAPI WINAPI BOOL
	GradientFill(::HDC hdc, PTRIVERTEX pVertex, ::ULONG nVertex, PVOID pMesh,
		::ULONG nMesh, ::ULONG ulMode);
}
#endif

namespace ege
{

enum dealmessage_update
{
	NORMAL_UPDATE = false,
	FORCE_UPDATE = true,
};

// 定义图像对象
class IMAGE
{
	int     m_initflag;

public:
	::HDC         m_hDC;
	::HBITMAP     m_hBmp;
	int         m_width;
	int         m_height;
	PDWORD      m_pBuffer;
	color_t     m_color;
	color_t     m_fillcolor;
	bool        m_aa;

private:
	int  newimage(::HDC hdc, int width, int height);
	int  deleteimage();

public:
	viewporttype        m_vpt;
	textsettingstype    m_texttype;
	linestyletype       m_linestyle;
	float               m_linewidth;
	color_t             m_bk_color;
	void*               m_pattern_obj;
	int                 m_pattern_type;
	void*               m_texture;

private:
	void inittest(const wchar_t* strCallFunction = nullptr) const;

public:
	IMAGE();
	IMAGE(int width, int height);
	IMAGE(IMAGE& img);              // 拷贝构造函数
	IMAGE& operator = (const IMAGE& img); // 赋值运算符重载函数
	~IMAGE();
	void set_pattern(void* obj, int type);
	void delete_pattern();
	void gentexture(bool gen);

	::HDC getdc()        const
	{
		return m_hDC;
	}
	int getwidth()     const
	{
		return m_width;
	}
	int getheight()    const
	{
		return m_height;
	}
	PDWORD getbuffer() const
	{
		return m_pBuffer;
	}

	int  createimage(int width, int height);
	int  resize(int width, int height);
	void copyimage(IMAGE* pSrcImg);
	void getimage(int srcX, int srcY, int srcWidth, int srcHeight);
	void getimage(IMAGE* pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);
	int  getimage(const char* pImgFile, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(const wchar_t* pImgFile, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(const char* pResType, const char* pResName, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(const wchar_t* pResType, const wchar_t* pResName, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(void* pMem, long size);
	void putimage(int dstX, int dstY, ::DWORD dwRop = SRCCOPY) const;
	void putimage(int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, ::DWORD dwRop = SRCCOPY) const;
	void putimage(IMAGE* pDstImg, int dstX, int dstY, ::DWORD dwRop = SRCCOPY) const;
	void putimage(IMAGE* pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, ::DWORD dwRop = SRCCOPY) const;
	void putimage(IMAGE* pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight, ::DWORD dwRop = SRCCOPY) const;
	int  saveimage(const char*  filename);
	int  saveimage(const wchar_t* filename);
	int  getpngimg(FILE* fp);
	int  savepngimg(FILE* fp, int bAlpha);
	int
	putimage_transparent(
		IMAGE* imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		color_t crTransparent,  // color to make transparent
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_alphablend(
		IMAGE* imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		unsigned char alpha,    // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_alphatransparent(
		IMAGE* imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		color_t crTransparent,  // color to make transparent
		unsigned char alpha,    // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_withalpha(
		IMAGE* imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);

	int
	putimage_alphafilter(
		IMAGE* imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		IMAGE* imgalpha,        // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	imagefilter_blurring_4(
		int intensity,
		int alpha,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int nHeightDest
	);
	int
	imagefilter_blurring_8(
		int intensity,
		int alpha,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int nHeightDest
	);
	int
	imagefilter_blurring(
		int intensity,
		int alpha,
		int nXOriginDest = 0,
		int nYOriginDest = 0,
		int nWidthDest = 0,
		int nHeightDest = 0
	);
	int putimage_rotate(
		IMAGE* imgtexture,
		int nXOriginDest,
		int nYOriginDest,
		float centerx,
		float centery,
		float radian,
		int btransparent = 0,           // transparent (1) or not (0)
		int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
		int smooth = 0
	);

	int putimage_rotatezoom(
		IMAGE* imgtexture,
		int nXOriginDest,
		int nYOriginDest,
		float centerx,
		float centery,
		float radian,
		float zoom,
		int btransparent = 0,           // transparent (1) or not (0)
		int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
		int smooth = 0
	);
};


template<typename T>
struct count_ptr
{
	explicit count_ptr(T* p)
	{
		//m_mutex = new Mutex;
		m_cnt = new long(1);
		m_p = p;
	}
	~count_ptr()
	{
		//m_mutex->Lock();
		--*m_cnt;
		if(*m_cnt == 0)
		{
			delete m_p;
			m_p = static_cast<T*>(0);
			delete m_cnt;
			m_cnt = static_cast<long*>(0);
		}
		//Mutex* mutex = m_mutex;
		//m_mutex = static_cast<Mutex*> ( 0 );
		//mutex->UnLock();
	}
	count_ptr(const count_ptr<T>& count_ptr_)
	{
		//m_mutex = count_ptr_.m_mutex;
		//m_mutex->Lock();
		m_p = count_ptr_.m_p;
		m_cnt = count_ptr_.m_cnt;
		++*m_cnt;
		//m_mutex->UnLock();
	}
	count_ptr<T>& operator= (const count_ptr<T>& count_ptr_)
	{
		//m_mutex->Lock();
		--*m_cnt;
		if(*m_cnt == 0)
		{
			delete m_p;
			m_p = static_cast<T*>(0);
			delete m_cnt;
			m_cnt = static_cast<long*>(0);
		}
		//Mutex* mutex = m_mutex;
		//m_mutex = count_ptr_.m_mutex;
		//mutex->UnLock();

		//m_mutex->Lock();
		m_p = count_ptr_.m_p;
		m_cnt = static_cast<long*>(count_ptr_.m_cnt);
		++*m_cnt;
		//m_mutex->UnLock();
		return *this;
	}
	operator T* () const
	{
		return m_p;
	}
	T& operator * () const
	{
		return *m_p;
	}
	T* operator -> () const
	{
		return m_p;
	}
private:
	T* m_p;
	long* m_cnt;
	//Mutex* m_mutex;
};

} // namespace ege

#endif /*Inc_ege_head_h_*/