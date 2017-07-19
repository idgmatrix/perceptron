#include "stdafx.h"
#include "resource.h"

#include "perceptron.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE g_hInst;								// current instance
HWND g_hWnd;
HWND hToolbar, hStatus;
HWND hBtn[64];

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

TBBUTTON TOOLBAR = {
		0,
		ID_BUTTON32773,
		TBSTATE_ENABLED,
		TBSTYLE_BUTTON,
		0,
		0};

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND CreateButton(HWND hWnd, int x, int y, int width, int height, UINT id);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NEURONS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NEURONS);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NEURONS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_NEURONS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindow(szWindowClass, szTitle, 
			WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i, j = 0; 
	static int n = 0;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;

	TCHAR szHello[MAX_LOADSTRING];
	LoadString(g_hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	int SBParts[5] = { 100, 200, 300, 400, 500 };
	DRAWITEMSTRUCT * pDITEM;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_TRAIN:
					hDC = GetDC(hWnd);
					for (i = 0; i < 50; i++)
					{
						for (j = 0; j < numTrain; j++)
						{
							PrintPattern(hDC, 0, 20 + 15 * j, j);
							Training(j);
							PrintOutput(j);
						}
					}
					ReleaseDC(hWnd, hDC);
					break;

				case ID_BUTTON32773:
				case IDM_TEST:
					hDC = GetDC(hWnd);

					for (n = 0; n < numTest; n++)
					{
						TextOut(hDC, 0, 450 + n * 15, "  ", 2);
						PrintTestPattern(hDC, 0, 450 + n * 15, n);
						Test(n);
						PrintTestOutput(20, 450 + n * 15, n);
					}					
					ReleaseDC(hWnd, hDC);
					break;

				case IDM_ABOUT:
					DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;

				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

			TextOut(hDC, 50 + 66 * 0, 0, "0", 1);
			TextOut(hDC, 50 + 66 * 1, 0, "1", 1);
			TextOut(hDC, 50 + 66 * 2, 0, "2", 1);
			TextOut(hDC, 50 + 66 * 3, 0, "3", 1);
			TextOut(hDC, 50 + 66 * 4, 0, "4", 1);
			TextOut(hDC, 50 + 66 * 5, 0, "5", 1);
			TextOut(hDC, 50 + 66 * 6, 0, "6", 1);
			TextOut(hDC, 50 + 66 * 7, 0, "7", 1);
			TextOut(hDC, 50 + 66 * 8, 0, "8", 1);
			TextOut(hDC, 50 + 66 * 9, 0, "9", 1);
			TextOut(hDC, 50 + 66 * 10, 0, "Error", 5);

			for (j = 0; j < numTrain; j++)
			{
				PrintPattern(hDC, 0, 20 + 15 * j, j);
				//Training(j);
				PrintOutput(j);
			}

			EndPaint(hWnd, &ps);
			break;

		case WM_SIZE:
			//MoveWindow(hToolbar, 0, 0, 0, 0, FALSE);
			MoveWindow(hStatus, 0, 0, 0, 0, FALSE);
			break;
		
		case WM_CREATE:
			
/*			hToolbar = CreateToolbarEx(hWnd, 
							WS_CHILD|WS_VISIBLE|TBSTYLE_BUTTON, 
							0, 
							1,			// number of buttons in toolbar
							g_hInst, 
							IDR_TOOLBAR1,
							&TOOLBAR,
							1,
							16,
							16,
							16,
							16,
							sizeof(TBBUTTON));
*/			
			///
			hStatus = CreateStatusWindow(WS_CHILD|WS_VISIBLE, NULL, hWnd, 1);
			SendMessage(hStatus, SB_SETPARTS, 5, (long)SBParts);
			SendMessage(hStatus, SB_SETTEXT, 1, (long)"sldkfsdjfd");
			///
			
			GetWindowRect(hToolbar, &rect);
/*			CreateWindow("SCROLLBAR",
						"",
						WS_CHILD|WS_VISIBLE|SBS_VERT,
						10, 
						rect.bottom - rect.top, 
						20, 
						100, 
						hWnd, 
						NULL, 
						g_hInst, 
						NULL);

			CreateWindow("LISTBOX",
						"list",
						WS_CHILD|WS_VISIBLE|LBS_STANDARD,
						100, 
						rect.bottom - rect.top, 
						200, 
						100, 
						hWnd, 
						NULL, 
						g_hInst, 
						NULL);
*/
/*			hBtn = CreateWindow("BUTTON",
						"",
						WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_PUSHLIKE,
						300, 
						rect.bottom - rect.top, 
						16, 
						16, 
						hWnd, 
						(HMENU)123, 
						g_hInst, 
						NULL);
*/			
//			for (j = 0; j < 8; j++)
//			for (i = 0; i < 8; i++)
//			hBtn[i+j*8] = CreateButton(hWnd, 300 + i * 16, rect.bottom - rect.top + j * 16, 16, 16, 200 + (i+j*8));

			InitSimulation();
			break;
		
		case WM_DRAWITEM:
			switch ((UINT)wParam)
			{
				case 123:
					pDITEM = (DRAWITEMSTRUCT *)lParam;
					if (pDITEM->itemAction & ODA_SELECT) 
					{
						if (pDITEM->itemState & ODS_SELECTED) 
							SetPixel(pDITEM->hDC, 8, 8, 0);
						else 
							SetPixel(pDITEM->hDC, 8, 8, 0xffffff);
					}
					break;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

HWND CreateButton(HWND hWnd, int x, int y, int width, int height, UINT id)
{			
	HWND hBtn = CreateWindow("BUTTON",
						"",
						WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_PUSHLIKE,
						x, 
						y, 
						width, 
						height, 
						hWnd, 
						(HMENU)id, 
						g_hInst, 
						NULL);
	return hBtn;
}