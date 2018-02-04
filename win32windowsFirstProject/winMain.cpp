//win32Form.cpp
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#pragma comment(lib, "comctl32.lib")

#include <windows.h>  
#include <string>  
#include <tchar.h>  
#include "json.hpp"
#include <locale>
#include <fstream>
#include "jsonParser.h"
#include "controlMaker.h"
#include <Commctrl.h>
#include "jsonMaker.h"
#include "converter.h"
#include "createWindow.h"
#include <uxtheme.h>
#include "myclass.h"




#pragma execution_character_set("utf-8")
//consts for resources
#define ID_CLOSE 0x001  
#define ID_MINIMIZE 0x002   
#define ID_test 110;
const int iconWidth = 25;
const int iconHeight = 25;

//declaring method wndproc
LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//local variables
string locationXString, componentType;
INT_PTR A[MAXCONTROLS];
HWND jsonControl[MAXCONTROLS];
HWND hwnd;
HINSTANCE hInst;
json_parser a;
item* ptr;
int iReturnBtns;
DWORD controlStyle = NULL;
string textalign;
RECT rectangle;
HBITMAP iconsENimages[MAXCONTROLS];
static HFONT hFont = nullptr;
long nFontSize = 13;

typedef std::basic_string<TCHAR> ustring;
typedef std::ifstream::failure IOEXCEPTION;

createWindowControl try1;

int OnCreate(HWND hwnd, CREATESTRUCT *cs)
{

	const HDC hdc = GetDC(hwnd);
	//loading json file 
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(L"index.json");
	}

	catch (IOEXCEPTION e)
	{
		MessageBox(hwnd, L"Cant load Json file.", L"ERORR!", MB_OK);
		exit(EXIT_FAILURE);
	}
	
	const json myJson = json::parse(file);
	myclass myclass1;
	myclass1.setJson(myJson);
	myclass1.create(hwnd, hInst);

	return 0;
}

void toggleCheckBox(int a) {
	const BOOL checked = IsDlgButtonChecked(hwnd, a);
	if (checked) CheckDlgButton(hwnd, a, BST_UNCHECKED);
	else CheckDlgButton(hwnd, a, BST_CHECKED);
}
//main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

	ustring classText = _T("Dynamic Control Loader with json");
	ustring classname = _T("main");
	WNDCLASSEX wndclass;
	MSG msg;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	//***********************************************************************
	wndclass.hbrBackground = HBRUSH(COLOR_WINDOW);//set back color to window  
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = wnd_proc;
	wndclass.lpszClassName = classname.c_str();
	wndclass.lpszMenuName = MAKEINTRESOURCEW(IDB_BITMAP1);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;


	// check this window class is registered or not  
	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(nullptr, TEXT("Window class is not registered"), TEXT("Error...."), MB_ICONERROR);
		return 0;
	}
	//int desktopwidth = GetSystemMetrics(SM_CXSCREEN); //geting desktop width
	//int desktopheight = GetSystemMetrics(SM_CYSCREEN); //geting desktop height

	hwnd = CreateWindow(classname.c_str(),  //window name  
		classText.c_str(),   // window text  
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //if set to popup it will remove any kind of border and buttons on windows default 
		100,      //window position x  //desktopwidth/4
		100,      //window position y  //desktopheight/4
		800,      //width  //desktopwidth/2
		600,      // height  //desktopheight/2
		nullptr,          //parent wnd handle
		nullptr,          //menu handle/wnd id
		hInst,      //app instance
		nullptr          //user defined info
	);
	//***********************************************************************
	SetLayeredWindowAttributes(hwnd, RGB(0x80, 0x00, 0xFF), 0, LWA_COLORKEY);
	// show & update created window  
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);



	// get message from queue  
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}


// WndProc function  
LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_ERASEBKGND:
		//***********************************************************************
		//return TRUE;
		SetBkColor(HDC(wParam), GetSysColor(COLOR_WINDOW));
		SetLayeredWindowAttributes(HWND(hwnd), GetSysColor(COLOR_WINDOW), 0, LWA_COLORKEY);
		break;
	case WM_CREATE:
		return OnCreate(hwnd, reinterpret_cast<CREATESTRUCT*>(lParam));

		//actions of buttons 
		// need to add onCommand like onCreate so it become pretty code.
	case WM_COMMAND:

		switch (wParam)
		{
		case 1223:
			//there is two constructor in createtestwindow which can be seprate window or child window
			try1.CreateTestWindow(hInst, hwnd);

			break;
			//add anynumber of checkbox you want to use 
		case 11001:
		case 11002:
		case 11003:
		case 11004:
		case 11005:
		case 11006:
			toggleCheckBox(wParam);
			break;
		case 10001: //returnBtnsName event from button.json
			jsonMaker JM;
			//giving data to jsonMaker for now i'm giving old json not real exact data need to change this later
			JM.makeJson(ptr, iReturnBtns, hwnd);
			break;
		case 13000:
		case 13001:

			break;
		case ID_CLOSE:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		case WM_CTLCOLORBTN:
		{

		}
		case ID_MINIMIZE:
			SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	default: break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}



