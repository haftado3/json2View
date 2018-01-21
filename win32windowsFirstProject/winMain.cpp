#include <windows.h>  
#include <cstdlib>  
#include <string>  
#include <tchar.h>  
#include "json.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include "jsonParser.h"
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
//consts for resources
#define ID_CLOSE 0x001  
#define ID_MINIMIZE 0x002   
#define ID_test 110;

//declaring method wndproc
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);

//local variables
string locationXString, componentType;
BOOL isMouseDownOnCloseButton = FALSE;
BOOL isMouseDownOnMinimizeButton = FALSE;
BOOL isMouseDownOnDemoButton = FALSE;
HWND hwnd;
HINSTANCE hInst;
jsonParser a;

//declaring typedate
typedef std::basic_string<TCHAR> ustring;

//consts for components
enum {
	IDBC_DEFPUSHBUTTON = 200,
	IDBC_PUSHBUTTON,
	IDBC_AUTOCHECKBOX,
	IDBC_AUTORADIOBUTTON,
	IDBC_GROUPBOX,
	IDBC_ICON,
	IDBC_BITMAP
};

// function to draw close button  
void DrawCloseButton(HDC hdc)
{
	if (isMouseDownOnCloseButton)
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 30;
		rc.bottom = 30;
		HBRUSH br = CreateSolidBrush(RGB(200, 30, 30));
		FillRect(hdc, &rc, br);

		SetBkColor(hdc, RGB(200, 30, 30));
		SetTextColor(hdc, RGB(255, 255, 255));

		TextOut(hdc, 10, 8, L"X", 1);
	}
	else
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 30;
		rc.bottom = 30;
		HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rc, br);

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));

		TextOut(hdc, 10, 8, L"X", 1);
	}

}

//draw minimize button for back color & text  
void DrawMinimizeButton(HDC hdc)
{
	if (isMouseDownOnMinimizeButton)
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 30;
		rc.bottom = 30;
		HBRUSH br = CreateSolidBrush(RGB(60, 60, 100));
		FillRect(hdc, &rc, br);

		SetBkColor(hdc, RGB(60, 60, 100));
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 10, 5, L"_", 1);
	}
	else
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 30;
		rc.bottom = 30;
		HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rc, br);

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 10, 5, L"_", 1);
	}
}

// function to fill rectangles to look like border for left,right & bottom direction  
void Draw_LeftRightBottom_Rectangles(RECT rect, HDC hdc, HBRUSH brush, int width, int height)
{
	RECT leftrect, rightrect, bottomrect;
	leftrect.left = 0;
	leftrect.top = rect.bottom - 266;
	leftrect.right = 4;
	leftrect.bottom = height;
	//fill left rect of window for border  
	FillRect(hdc, &leftrect, brush);

	rightrect.left = width - 4;
	rightrect.top = rect.bottom - 266;
	rightrect.right = width;
	rightrect.bottom = height;
	//fill right rect of window  
	FillRect(hdc, &rightrect, brush);

	bottomrect.left = 0;
	bottomrect.top = height - 4;
	bottomrect.right = width;
	bottomrect.bottom = height;
	//fill bottom rect of window  
	FillRect(hdc, &bottomrect, brush);
}

HWND CreateButton(const HWND hParent, const HINSTANCE hInst, DWORD dwStyle, const RECT& rc, INT_PTR id, const ustring& caption)
{
	dwStyle |= WS_CHILD | WS_VISIBLE;
	return CreateWindow(              //extended styles
		_T("button"),                 //control 'class' name
		caption.c_str(),              //control caption
		dwStyle,                      //control style 
		rc.left,                      //position: left
		rc.top,                       //position: top
		rc.right,                     //width
		rc.bottom,                    //height
		hParent,                      //parent window handle
									  //control's ID
		(HMENU)id,
		hInst,                        //application instance
		0);                           //user defined info

}

INT_PTR A[ARRAYSIZE];
HWND jsonButton[ARRAYSIZE], minimize_button, close_button, customButton;

int OnCreate(const HWND hwnd, CREATESTRUCT *cs)
{


	minimize_button = CreateWindow(TEXT("button"), TEXT(""),
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		538, 0, 30, 30, hwnd, (HMENU)ID_MINIMIZE,
		hInst, NULL);
	close_button = CreateWindow(TEXT("button"), TEXT(""),
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		568, 0, 30, 30, hwnd, (HMENU)ID_CLOSE,
		hInst, NULL);
	
	//loading json file 
	std::ifstream file;
	file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open("C:\\Users\\Mahdi\\Documents\\Visual Studio 2015\\Projects\\Win32Project1\\Win32Project1\\button.json");
	}
	catch(std::ifstream::failure e)
	{
		MessageBox(hwnd, L"Cant load Json file.", L"ERORR!", MB_OK);
		exit(EXIT_FAILURE);
	}
	json j = json::parse(file);
	item* ptr = a.initialize(j);
	//------------------

	// i may want to change font from user point of view or include font into my app.
	HFONT hFont = CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ARABIC_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, L"B Nazanin Regular");

	for (int i = 0; i < ARRAYSIZE; i++) {
		if (ptr[i].ComponentType == "button") {

			//an id for button which i saved it , was inside create button
			A[i] = static_cast<INT_PTR>(IDBC_DEFPUSHBUTTON);

			//converting readed text to usable data for win32

			string s = ptr[i].Text;

			wstring w = wstring(s.begin(), s.end());
			ustring us = w.c_str();

			//location and size of button
			RECT rc = { ptr[i].LocationX  ,  ptr[i].LocationY  ,  ptr[i].Width  ,  ptr[i].Height };

			//creating instance of all buttons components
			jsonButton[i] = CreateButton(hwnd, hInst, BS_PUSHBUTTON, rc, A[i], us);

			//sending font message to wndproc : this is gonna change font of all buttons to hfont : in case you want to change font for anywindows
			// this guy need a try catch block cause client side may have not wanted font
			SendMessage(jsonButton[i], WM_SETFONT, WPARAM(hFont), TRUE);

			//setting button disablity
			if (ptr[i].Disabled == 1) {
				EnableWindow(jsonButton[i], false);
			}

		}
		if (ptr[i].ComponentType == "") {
			break;
		}
	}
	return 0;
}

LRESULT OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (wParam)
	{

	default:
		break;
	}
	return 0;
}
//main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	ustring classText = _T("Main Page");
	ustring classname = _T("main");
	WNDCLASS wndclass;
	MSG msg;


	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(60, 60, 60)));//set back color to window  
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = classname.c_str();
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;


	// check this window class is registered or not  
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Window class is not registered"), TEXT("Error...."), MB_ICONERROR);
		return 0;
	}
	//int desktopwidth = GetSystemMetrics(SM_CXSCREEN); //geting desktop width
	//int desktopheight = GetSystemMetrics(SM_CYSCREEN); //geting desktop height

	hwnd = CreateWindow(classname.c_str(),  //window name  
		classText.c_str(),   // window text  
		WS_POPUP, //set POPUP window style for no border & controls  
		100,      //window position x  //desktopwidth/4
		100,      //window position y  //desktopheight/4
		600,      //width  //desktopwidth/2
		500,      // height  //desktopheight/2
		0,          //parent wnd handle
		0,          //menu handle/wnd id
		hInst,      //app instance
		0          //user defined info
	);

	// show & update created window  
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// get message from queue  
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}


// WndProc function  
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND minimize_button, close_button, customButton;
	static int X, Y;
	LRESULT move = NULL;
	LPDRAWITEMSTRUCT pdis;
	HICON hIcon;

	switch (message)
	{
	case WM_CREATE:
		return OnCreate(hwnd, reinterpret_cast<CREATESTRUCT*>(lParam));

		break;


	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		RECT rect;
		HBRUSH brush;

		GetClientRect(hwnd, &rect);
		rect.bottom = rect.bottom - 466;
		brush = CreateSolidBrush(RGB(0, 0, 0));

		// fill rectangle for top layered title border  
		FillRect(hdc, &rect, brush);

		// call above function  
		Draw_LeftRightBottom_Rectangles(rect, hdc, brush, X, Y);

		//draw text as a window by setting white text color & back color  
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 35, 7, TEXT("Main Windows"), 12); //last parameter is array length of text

													   //draw text on window  
													   //SetBkColor(hdc, RGB(60, 60, 60));
													   //SetTextColor(hdc, RGB(200, 200, 200));
													   //TextOut(hdc, 160, 120, TEXT("test text"), 9);

													   //draw icon  
													   //hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDB_PNG1));

													   //DrawIconEx(hdc, 5, 5, hIcon, 20, 20, 0, brush, 0);

		EndPaint(hwnd, &ps);

		break;

		//draw item  
	case WM_DRAWITEM:
		pdis = (LPDRAWITEMSTRUCT)lParam;

		switch (pdis->CtlID)
		{
		case ID_CLOSE:
			//draw close button  
			DrawCloseButton(pdis->hDC);
			break;

		case ID_MINIMIZE:
			//draw minimize button  
			DrawMinimizeButton(pdis->hDC);
			break;
		}

		//if button is selected then change values  
		if (pdis->itemState && ODS_SELECTED)
		{
			isMouseDownOnCloseButton = TRUE;
			isMouseDownOnMinimizeButton = TRUE;
			isMouseDownOnDemoButton = TRUE;
		}
		else
		{
			isMouseDownOnCloseButton = FALSE;
			isMouseDownOnMinimizeButton = FALSE;
			isMouseDownOnDemoButton = FALSE;
		}


		break;

		//actions of buttons  
	case WM_COMMAND:

		switch (wParam)
		{
		case ID_CLOSE:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;

		case ID_MINIMIZE:
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
			break;

		}
		return OnCommand(wParam, lParam);
		break;

		//size  
	case WM_SIZE:
		X = LOWORD(lParam);
		Y = HIWORD(lParam);
		break;

		//color changing for button
	case WM_CTLCOLOREDIT:

		//move window 
	case WM_NCHITTEST:
		RECT rc;
		POINT pt;

		GetCursorPos(&pt);

		GetWindowRect(hwnd, &rc);
		rc.bottom = rc.bottom - 466;

		//if cursor position is within top layered drawn rectangle then  
		//set move to HTCAPTION for moving the window from its client  
		if (pt.x <= rc.right && pt.x >= rc.left && pt.y <= rc.bottom && pt.y >= rc.top)
		{
			move = DefWindowProc(hwnd, message, wParam, lParam);
			if (move == HTCLIENT)
			{
				move = HTCAPTION;
			}
		}

		return move;

		break;


	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}