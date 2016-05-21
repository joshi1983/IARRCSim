#include "SplashWindow.hpp"
#include <ctime>
#include <iostream>

using namespace std;

bool SplashWindow::staySleeping=true;

#ifdef _USE_VISUAL_STUDIO_
const wchar_t SW_CLASSNAME[] =
{'S','p','l','a','s','h','W','i','n','d','o','w','C','l','a','s','s'};
#else
const char SW_CLASSNAME[] ="SplashWindowClass";
#endif

#ifndef _UBUNTU_
LRESULT CALLBACK SplashWindow_WndProc (HWND hwnd, UINT message, WPARAM wParam,
						   LPARAM lParam);

DWORD WINAPI SplashWindow_ThreadProc (LPVOID parameter);


HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color, HWND hwnd);

void convertLPCSTRToLPCWSTR(wchar_t * dest,LPCSTR src)
{
	int i=0;
  for (;src[i]!='\0';i++)
	  dest[i]=src[i];

  dest[i]='\0';
}
#endif

void SplashWindow::hide()
{
  staySleeping=false;
}

SplashWindow::SplashWindow ()
{
	clearMembers ();
}

SplashWindow::SplashWindow (
	#ifndef _UBUNTU_
	 LPCSTR
	#else
	 const char *
	#endif
	 bitmapFileName, const int r, const int g, const int b)
{
	clearMembers ();
	createSplashWindow (bitmapFileName, r, g, b);
}

SplashWindow::SplashWindow (DWORD bitmapResourceID, const int r, const int g, const int b)
{
	clearMembers ();
	createSplashWindow (bitmapResourceID, r, g, b);
}

SplashWindow::~SplashWindow ()
{
	destroySplashWindow ();
}

void SplashWindow::showModalWindow (DWORD minDisplayTime)
{
	if (!isValidWindow ())
	{
		return;
	}

#ifndef _UBUNTU_
	MSG msg;
	BOOL gotMSG;
	HANDLE threadHandle;
	DWORD threadID;
	SECURITY_ATTRIBUTES sa;
	bool isThreadFinished = false;

	mMinDisplayTime = minDisplayTime;

	ZeroMemory (&msg, sizeof (MSG));
	ZeroMemory (&sa, sizeof (SECURITY_ATTRIBUTES));

	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	//this critical section will protect mThreadFinished because it
	//is used be the main process and a concurrent thread.
	InitializeCriticalSection (&mLock);

	//create a thread
	threadHandle = CreateThread (&sa, 0, SplashWindow_ThreadProc, this, 0, &threadID);
	mThreadFinished = isThreadFinished = false;

	//Display the window
	UpdateWindow (mHWND);
	ShowWindow (mHWND, SW_SHOW);

	while (true)
	{
		gotMSG = PeekMessage (&msg, 0L, 0L, 0L, PM_REMOVE);

		//safely get the value of mThreadFinsshed
		EnterCriticalSection (&mLock);
		isThreadFinished = mThreadFinished;
		LeaveCriticalSection (&mLock);

		//if the thread is finished, exit the loop
		//thread tuns for at least mMinDisplayTime milliseconds
		if (isThreadFinished)
		{
			break;
		}

		if (gotMSG)
		{
			//Don't try this at home!
			//I am capturing the WM_PAINT message before it
			//is sent to the WndProc. I am sure this is not recommended
			//by MS, but I see no adverse effects.
			if (msg.message == WM_PAINT)
			{
				//Blit the bitmap to the screen
				PAINTSTRUCT paint;

				HDC dc = BeginPaint (mHWND, &paint);
				HDC memDC = CreateCompatibleDC(dc);

				SelectObject(memDC,mBitmap);

				BitBlt(dc,0,0,mBitmapWidth,mBitmapHeight,memDC,0,0,SRCCOPY);

				DeleteObject (dc);
			}
			else
			{
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}

		}

	}

	//close any connections to the thread
	CloseHandle (threadHandle);
	//release the critical section support
	DeleteCriticalSection (&mLock);
	#endif

	//release the resources of this splash window
	destroySplashWindow ();
}

bool SplashWindow::createSplashWindow (
                                       #ifdef _USE_VISUAL_STUDIO_
                                       LPCSTR
                                       #else
                                       const char *
                                       #endif
                                       bitmapFileName

                                       , const int r, const int g, const int b)
{
    #ifdef _USE_VISUAL_STUDIO_
	wchar_t
	#else
	char
	#endif
	 temp[200];

    #ifdef _USE_VISUAL_STUDIO_
	convertLPCSTRToLPCWSTR(temp,bitmapFileName);
    #else
    strcpy(temp,bitmapFileName);
    #endif

#ifndef _UBUNTU_
	mBitmap = (HBITMAP)LoadImage (GetModuleHandle (0L),
								temp,
								IMAGE_BITMAP,
								0,
								0,
								LR_LOADFROMFILE);
	if (mBitmap == 0L)
	{
		return false;
	}

  #endif


	return createWindowHelper (r, g, b);
}

bool SplashWindow::createSplashWindow (DWORD bitmapResourceID, const int r, const int g, const int b)
{
    #ifndef _UBUNTU_
	mBitmap = (HBITMAP)LoadImage (GetModuleHandle (0L),
								MAKEINTRESOURCE (bitmapResourceID),
								IMAGE_BITMAP,
								0,
								0,
								LR_DEFAULTCOLOR);
	if (mBitmap == 0L)
	{
		return false;
	}
  #endif

	return createWindowHelper (r, g, b);
}

bool SplashWindow::isValidWindow () const
{
    #ifdef _UBUNTU_
    return false;
    #else
	return mHWND != 0L;
	#endif
}

    #ifndef _UBUNTU_
HWND SplashWindow::getHWND () const
{
	return mHWND;
}
	#endif

void SplashWindow::loadResources () const
{

}

bool SplashWindow::createWindowHelper (const int r, const int g, const int b)
{
    #ifndef _UBUNTU_
	//check if the bitmap is loaded.
	if (!mBitmap)
	{
		return false;
	}

	BITMAP bm;
	int x, y;


	//create the window...
	WNDCLASS wc;

	wc.lpfnWndProc = SplashWindow_WndProc;
	wc.lpszClassName = SW_CLASSNAME;
	wc.hInstance = GetModuleHandle (0L);

	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject (WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIcon = NULL;

	if (!RegisterClass(&wc))
	{
		DeleteObject (mBitmap);
		clearMembers ();
		return false;
	}

	mHWND = CreateWindowEx (WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
							wc.lpszClassName,
							NULL,
							WS_POPUP,
							0,
							0,
							0,
							0,
							HWND_DESKTOP,
							0L,
							wc.hInstance,
							0L);

	if (!isValidWindow ())
	{
		destroySplashWindow ();
		return false;
	}

	//get the info on our bitmap
	GetObject(mBitmap, sizeof(bm), &bm);

	//get the dimensions
	mBitmapWidth = bm.bmWidth;
	mBitmapHeight = bm.bmHeight;

	//center the splash window on the screen
	x =(GetSystemMetrics(SM_CXFULLSCREEN)-mBitmapWidth)/2;
	y =(GetSystemMetrics(SM_CYFULLSCREEN)-mBitmapHeight)/2;

	MoveWindow (mHWND, x, y, mBitmapWidth, mBitmapHeight, FALSE);

	//if r, g, b are valid values, "cut out" all pixels equal to r,g,b
	if (r != -1 && g != -1 && b != -1)
	{
		HRGN region = CreateRgnFromBitmap (mBitmap, RGB(r,g,b), mHWND);

		if (FAILED (SetWindowRgn (mHWND, region, TRUE)))
		{
			destroySplashWindow ();
			return false;
		}
	}

   #endif

	return true;
}

void SplashWindow::clearMembers ()
{
    #ifndef _UBUNTU_
	mHWND = 0L;
	mBitmap = 0L;
	#endif

	mBitmapWidth = mBitmapHeight = 0;
	mMinDisplayTime = 0;
	mThreadFinished = true;
}

void SplashWindow::destroySplashWindow ()
{
        #ifndef _UBUNTU_
	DestroyWindow (mHWND);
	DeleteObject (mBitmap);
		#endif
	clearMembers ();
	 #ifndef _UBUNTU_
	UnregisterClass (SW_CLASSNAME, GetModuleHandle (0L));
		#endif
}
    #ifndef _UBUNTU_
HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color, HWND hwnd)
{
	// this code is written by Davide Pizzolato

	if (!hBmp) return NULL;

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm );	// get bitmap attributes

	HDC windowHDC = GetDC(hwnd);
	HDC hdc = CreateCompatibleDC(windowHDC);	//Creates a memory device context for the bitmap
	SelectObject(hdc, hBmp);			//selects the bitmap in the device context

	const DWORD RDHDR = sizeof(RGNDATAHEADER);
	const DWORD MAXBUF = 40;		// size of one block in RECTs
	// (i.e. MAXBUF*sizeof(RECT) in bytes)
	LPRECT	pRects;
	DWORD	cBlocks = 0;			// number of allocated blocks

	INT		i, j;					// current position in mask image
	INT		first = 0;				// left position of current scan line
	// where mask was found
	bool	wasfirst = false;		// set when if mask was found in current scan line
	bool	ismask;					// set when current color is mask color

	// allocate memory for region data
	RGNDATAHEADER* pRgnData = (RGNDATAHEADER*)new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
	memset( pRgnData, 0, RDHDR + cBlocks * MAXBUF * sizeof(RECT) );
	// fill it by default
	pRgnData->dwSize	= RDHDR;
	pRgnData->iType		= RDH_RECTANGLES;
	pRgnData->nCount	= 0;
	for ( i = 0; i < bm.bmHeight; i++ )
		for ( j = 0; j < bm.bmWidth; j++ ){
			// get color
			ismask=(GetPixel(hdc, j,bm.bmHeight-i-1)!=color);
			// place part of scan line as RECT region if transparent color found after mask color or
			// mask color found at the end of mask image
			if (wasfirst && ((ismask && (j==(bm.bmWidth-1)))||(ismask ^ (j<bm.bmWidth)))){
				// get offset to RECT array if RGNDATA buffer
				pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
				// save current RECT
				SetRect (&pRects[ pRgnData->nCount++ ], first, bm.bmHeight - i - 1, j+(j==(bm.bmWidth-1)), bm.bmHeight - i );
				// if buffer full reallocate it
				if ( pRgnData->nCount >= cBlocks * MAXBUF ){
					LPBYTE pRgnDataNew = new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
					memcpy( pRgnDataNew, pRgnData, RDHDR + (cBlocks - 1) * MAXBUF * sizeof(RECT) );
					delete pRgnData;
					pRgnData = (RGNDATAHEADER*)pRgnDataNew;
				}
				wasfirst = false;
			} else if ( !wasfirst && ismask ){		// set wasfirst when mask is found
				first = j;
				wasfirst = true;
			}
		}

		DeleteDC(hdc);	//release the bitmap
		DeleteDC (windowHDC);
		// create region
		/*  Under WinNT the ExtCreateRegion returns NULL (by Fable@aramszu.net) */
		//	HRGN hRgn = ExtCreateRegion( NULL, RDHDR + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
		/* ExtCreateRegion replacement { */
		HRGN hRgn=CreateRectRgn(0, 0, 0, 0);

		if ( hRgn == NULL )
		{
			return hRgn;
		}

		pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
		for(i=0;i<(int)pRgnData->nCount;i++)
		{
			HRGN hr=CreateRectRgn(pRects[i].left, pRects[i].top, pRects[i].right, pRects[i].bottom);
			if (FAILED (CombineRgn(hRgn, hRgn, hr, RGN_OR)))
			{
				DeleteObject (hr);
				return hRgn;
			}

			if (hr)
			{
				DeleteObject(hr);
			}
		}

		/* } ExtCreateRegion replacement */

		delete pRgnData;
		return hRgn;
}

DWORD WINAPI SplashWindow_ThreadProc (LPVOID parameter)
{
	SplashWindow* sw = (SplashWindow*)parameter;
	unsigned long startTime = (unsigned long)time (0L);
	unsigned long endTime = 0;
	DWORD threadLength;
	DWORD sleepTime;

	if (sw)
	{
		sw->loadResources ();

	}
	else
	{
		ExitThread (1);
		return 1;
	}

	//determine if the thread needs to continue longer.
	endTime = (unsigned long)time (0L);
	threadLength = (DWORD)(endTime - startTime);

	sleepTime = sw->mMinDisplayTime - threadLength;

	if (sleepTime > 0)
	{
		int count = 0;
		while (SplashWindow::staySleeping && count<sleepTime)
		{
  		     Sleep (200);
			count+=200;
		}
	}

	EnterCriticalSection (&(sw->mLock));
		sw->mThreadFinished = true;
	LeaveCriticalSection (&(sw->mLock));

	ExitThread (0);
	return 0;
}

LRESULT CALLBACK SplashWindow_WndProc (HWND hwnd, UINT message, WPARAM wParam,
						   LPARAM lParam)
{
	return DefWindowProc (hwnd, message, wParam, lParam);
}

#endif
