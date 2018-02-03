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
#include <codecvt>
#include <fstream>
#include "jsonParser.h"
#include "controlMaker.h"
#include <Commctrl.h>
#include "jsonMaker.h"
#include "TooltipMaker.h"
#include "converter.h"
#include "addItem2Tree.h"
#include "resource.h"
#include "createWindow.h"
#include "createToolbar.h"
#include <uxtheme.h>
#include "myclass.h"


#define UD_MAX_POS 30
#define UD_MIN_POS 0

#define TB_MAX_POS 100
#define TB_MIN_POS 0

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

void setAlignment(int i) {
	textalign = ptr[i].TextAlignment;
	if (ptr[i].ComponentType == "button") {
		if (textalign == "right")controlStyle = BS_RIGHT;
		else if (textalign == "left") controlStyle = BS_LEFT;
		else controlStyle = BS_CENTER;
	}
	if (ptr[i].ComponentType == "label" || ptr[i].ComponentType == "textbox") {
		if (textalign == "right")controlStyle = SS_RIGHT;
		else if (textalign == "left") controlStyle = SS_LEFT;
		else controlStyle = SS_CENTER;
	}
}



RECT getLocation(int i) {
	return RECT{ ptr[i].LocationX, ptr[i].LocationY, ptr[i].Width, ptr[i].Height };
}
void loadFont(HDC hdc) {
	AddFontResource(L"bnazanin.ttf");
	const TCHAR* fontName = _T("B Nazanin");

	LOGFONT logFont = { 0 };
	logFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	logFont.lfWeight = FW_BOLD;
	logFont.lfCharSet = ARABIC_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = DRAFT_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH;
	_tcscpy_s(logFont.lfFaceName, fontName);
	hFont = CreateFontIndirect(&logFont);
	SystemParametersInfo(SPI_SETFONTSMOOTHING, TRUE, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 0, PVOID(FE_FONTSMOOTHINGCLEARTYPE), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST, 0, PVOID(1600), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	ReleaseDC(hwnd, hdc);
}

void loadIcons(HWND hwnd, HDC hdc, int i) {
	//creating icon for buttons which is supporting text alignment with itself
	const HDC hMemDC = CreateCompatibleDC(hdc);
	try
	{
		//if you want to load from resources try this one :
		//hbitmap[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		//loading icons and putting them int an array of hbitmap
		if (ptr[i].Icon == "imageVenus")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"VENUS.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "ship")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"ship.bmp", IMAGE_BITMAP, ptr[i].Width, ptr[i].Height, LR_LOADFROMFILE));
		if (ptr[i].Icon == "exitIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"Close_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // 0,0 is meaning use default size of icon
		if (ptr[i].Icon == "miniIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"down_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "addIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"add_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "cancelIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"cancl_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "okIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"ok_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	}
	catch (IOEXCEPTION e)
	{
		MessageBox(hwnd, L"Cant load icon file.", L"ERORR!", MB_OK);
	}
	SelectObject(hMemDC, iconsENimages[i]);
	DeleteDC(hMemDC);
	ReleaseDC(hwnd, hdc);
}
void finalTouch(int i)
{
	if (jsonControl[i] == nullptr)
	{
		MessageBox(nullptr, L"Component Creation Failed!", L"Error!", MB_ICONEXCLAMATION);
		exit(EXIT_FAILURE);
	}
	SendMessage(jsonControl[i], WM_SETFONT, WPARAM(hFont), LPARAM(MAKELONG(TRUE, 0)));
	if (ptr[i].Visible == 1)  ShowWindow(jsonControl[i], SW_SHOW);
	else ShowWindow(jsonControl[i], SW_HIDE);
	if (ptr[i].Disabled == 1) {
		EnableWindow(jsonControl[i], false);
	}

	//const LPWSTR buttoninfo = LPWSTR(charArray2utf(a.getInfo(i), i));
	//CreateMyTooltip(jsonControl[i], buttoninfo, hFont);
}
LPCWSTR initControl(int i)
{
	textalign = ptr[i].TextAlignment;
	setAlignment(i);
	rectangle = getLocation(i);
	LPCWSTR jsonText = charArray2utf(a.getText(i), i);
	return jsonText;
}
int OnCreate(HWND hwnd, CREATESTRUCT *cs)
{

	const HDC hdc = GetDC(hwnd);
	loadFont(GetDC(hwnd));
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
	myclass1.init(myJson);
	myclass1.create(hwnd, hInst);
//	ptr = a.initialize(myJson);
//
//	//making font indirect technique
//	//this add font resource has not effect on app
//
//	//making buttons
//	for (int i = 0; i < MAXCONTROLS; i++) {
//		if (ptr[i].ComponentType == "button") {
//			controlStyle = BS_PUSHBUTTON;
//			const LPCWSTR buttonText = initControl(i);
//
//			if (ptr[i].ClickEvent == "returnBtnsName") {
//				iReturnBtns = i;
//				A[i] = 10001;
//			}
//			if (ptr[i].ClickEvent == "minimizeApp") {
//				A[i] = ID_MINIMIZE;
//			}
//			if (ptr[i].ClickEvent == "newwindow") {
//				A[i] = 1223;
//
//			}
//			jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], buttonText);
//			finalTouch(i);
//			loadIcons(hwnd, GetDC(jsonControl[i]), i);
//			SendMessage(jsonControl[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
//			const LPWSTR buttoninfo = LPWSTR(charArray2utf(a.getInfo(i), i));
//			CreateMyTooltip(jsonControl[i], buttoninfo, hFont);
//
//
//
//			/*
//			//background color of button: working but has problem with text
//			HDC hdcBack = GetDC(hwnd);
//			HDC hMemBack = CreateCompatibleDC(hdcBack);
//			HBITMAP hBitmap = CreateCompatibleBitmap(hdcBack, 100, 105);
//			SelectObject(hMemBack, hBitmap);
//			SetDCBrushColor(hMemBack, RGB(0, 0, 255));
//			RECT r = { 0 };
//			r.left = 0;
//			r.right = 100;
//			r.top = 0;
//			r.bottom = 105;
//			FillRect(hMemBack, &r, (HBRUSH)GetStockObject(DC_BRUSH));
//			DeleteDC(hMemBack);
//			ReleaseDC(hwnd, hdcBack);
//			SendMessage(jsonButton[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
//			*/
//
//		}
//		if (ptr[i].ComponentType == "label") {
//			A[i] = 10800 + i;
//			controlStyle = NULL;
//			const LPCWSTR labelText = initControl(i);
//			jsonControl[i] = createControl(L"static", hwnd, hInst, controlStyle, rectangle, A[i], labelText);
//			finalTouch(i);
//		}
//		if (ptr[i].ComponentType == "image") {
//			A[i] = 10900 + i;
//			controlStyle = SS_BITMAP;
//			const LPCWSTR imageText = initControl(i);
//			jsonControl[i] = createControl(L"static", hwnd, hInst, controlStyle, rectangle, A[i], imageText);
//			loadIcons(hwnd, GetDC(jsonControl[i]), i);
//			SendMessage(jsonControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
//			finalTouch(i);
//		}
//
//		if (ptr[i].ComponentType == "checkbox") {
//			A[i] = 11000 + i;
//			controlStyle = BS_CHECKBOX;
//			const LPCWSTR checkBoxText = initControl(i);
//			jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], checkBoxText);
//			CheckDlgButton(jsonControl[i], A[i], BST_CHECKED);
//			finalTouch(i);
//		}
//		if (ptr[i].ComponentType == "textbox") {
//			A[i] = 12000 + i;
//			controlStyle = WS_BORDER;
//			const LPCWSTR textboxText = initControl(i);
//			jsonControl[i] = createControl(L"edit", hwnd, hInst, controlStyle, rectangle, A[i], textboxText);
//			finalTouch(i);
//
//		}
//		if (ptr[i].ComponentType == "groupbox") {
//			A[i] = 12200 + i;
//			controlStyle = BS_GROUPBOX | BS_RIGHT;
//			const LPCWSTR groupboxText = initControl(i);
//			jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], groupboxText);
//			finalTouch(i);
//		}
//		if (ptr[i].ComponentType == "radiobutton") {
//			A[i] = 12400 + i;
//			const LPCWSTR radiobuttonText = initControl(i);
//			if (ptr[i].Newgroup) {
//				controlStyle = BS_AUTORADIOBUTTON | WS_GROUP;
//				jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], radiobuttonText);
//				SendMessage(jsonControl[i], BM_SETCHECK, 1, 0);
//			}
//			else
//			{
//				controlStyle = BS_AUTORADIOBUTTON;
//				jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], radiobuttonText);
//			}
//			finalTouch(i);
//		}
//		if (ptr[i].ComponentType == "combobox") {
//			A[i] = 12600 + i;
//			controlStyle = CBS_DROPDOWNLIST;
//			const LPCWSTR comboboxText = initControl(i);
//			jsonControl[i] = createControl(L"combobox", hwnd, hInst, controlStyle, rectangle, A[i], comboboxText);
//			finalTouch(i);
//			for (int q = 0; q < DROPSIZE; q++) {
//				LPCWSTR comboboxItem = a.getChilds(i)[q];
//				SendMessage(jsonControl[i], CB_ADDSTRING, 0, LPARAM(comboboxItem));
//			}
//			SendMessage(jsonControl[i], CB_SETCURSEL, 0, 0);
//		}
//
//		if (ptr[i].ComponentType == "updown") {
//			A[i] = 13000 + i;
//			INITCOMMONCONTROLSEX icex;
//			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
//			icex.dwICC = ICC_UPDOWN_CLASS;
//			InitCommonControlsEx(&icex);
//			controlStyle = UDS_SETBUDDYINT | UDS_ALIGNLEFT;
//			DWORD editStyle = WS_BORDER | ES_READONLY | SS_CENTER; //read and write with EM_SETREADONLY
//			LPCWSTR updownText = initControl(i);
//			jsonControl[i] = createControl(UPDOWN_CLASS, hwnd, hInst, controlStyle, rectangle, A[i], updownText);
//			finalTouch(i);
//			jsonControl[i + 1] = createControl(L"edit", hwnd, hInst, editStyle, rectangle, A[i] + 1, L"0");
//			SendMessage(jsonControl[i], UDM_SETBUDDY, (WPARAM)jsonControl[i + 1], 0);
//			SendMessageW(jsonControl[i], UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
//			i++;
//		}
//		if (ptr[i].ComponentType == "trackbar") {
//			A[i] = 13200 + i;
//			controlStyle = TBS_AUTOTICKS;
//			DWORD editStyle = WS_BORDER | ES_READONLY | SS_CENTER;
//			RECT staticrect = { 0,0,30,20 };
//			LPCWSTR trackbarText = initControl(i);
//			jsonControl[i] = createControl(TRACKBAR_CLASS, hwnd, hInst, controlStyle, rectangle, A[i], trackbarText);
//			finalTouch(i);
//			jsonControl[i + 1] = createControl(L"static", hwnd, hInst, editStyle, staticrect, A[i] + 1, L"0");
//			jsonControl[i + 2] = createControl(L"static", hwnd, hInst, editStyle, staticrect, A[i] + 2, L"100");
//			SendMessageW(jsonControl[i], TBM_SETRANGE, TRUE, MAKELONG(TB_MIN_POS, TB_MAX_POS));
//			SendMessageW(jsonControl[i], TBM_SETPAGESIZE, 0, 10);
//			SendMessageW(jsonControl[i], TBM_SETTICFREQ, 10, 0);
//			SendMessageW(jsonControl[i], TBM_SETPOS, FALSE, 0);
//			SendMessageW(jsonControl[i], TBM_SETBUDDY, TRUE, (LPARAM)jsonControl[i + 1]);
//			SendMessageW(jsonControl[i], TBM_SETBUDDY, FALSE, (LPARAM)jsonControl[i + 2]);
//		}
//		if (ptr[i].ComponentType == "monthcal") {
//			A[i] = 13400 + i;
//			INITCOMMONCONTROLSEX icex;
//			icex.dwSize = sizeof(icex);
//			icex.dwICC = ICC_DATE_CLASSES;
//			InitCommonControlsEx(&icex);
//			controlStyle = MCS_NOTODAYCIRCLE | WS_BORDER;
//			LPCWSTR monthcalText = initControl(i);
//			jsonControl[i] = createControl(MONTHCAL_CLASS, hwnd, hInst, controlStyle, rectangle, A[i], monthcalText);
//			finalTouch(i);
//
//		}
//		if (ptr[i].ComponentType == "progressbar") {
//
//			A[i] = 13600 + i;
//			INITCOMMONCONTROLSEX icex;
//			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
//			icex.dwICC = ICC_PROGRESS_CLASS;
//			InitCommonControlsEx(&icex);
//			controlStyle = PBS_SMOOTH;
//			LPCWSTR progressbarText = initControl(i);
//			jsonControl[i] = createControl(PROGRESS_CLASS, hwnd, hInst, controlStyle, rectangle, A[i], progressbarText);
//			finalTouch(i);
//			SendMessage(jsonControl[i], PBM_SETRANGE, 0, MAKELPARAM(0, 100));
//			SendMessage(jsonControl[i], PBM_SETSTEP, 1, 0);
//			SendMessage(jsonControl[i], PBM_SETPOS, 100, 0);
//		}
//		if (ptr[i].ComponentType == "menubar") {
//			HMENU hMenubar = CreateMenu();
//			HMENU hMenu = CreateMenu();
//			for (int q = 0; q < DROPSIZE; q++) {
//				LPCWSTR menuItem = a.getChilds(i)[q];
//				AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, menuItem);
//			}
//			NONCLIENTMETRICS rNCM;
//			rNCM.cbSize = sizeof(NONCLIENTMETRICS);
//			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &rNCM, 0);
//			lstrcpyn(rNCM.lfMenuFont.lfFaceName, L"B Nazanin", sizeof(rNCM.lfMenuFont.lfFaceName));
//			SystemParametersInfo(SPI_SETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &rNCM, 0);
//			rNCM.lfMenuFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(GetDC(jsonControl[i]), LOGPIXELSY), 72);
//			SetMenu(hwnd, hMenubar);
//		}
//		if (ptr[i].ComponentType == "listbox") {
//			A[i] = 13800 + i;
//			controlStyle = ES_AUTOVSCROLL | LBS_NOTIFY | WS_BORDER;
//			LPCWSTR listboxText = initControl(i);
//			jsonControl[i] = createControl(TEXT("listbox"), hwnd, hInst, controlStyle, rectangle, A[i], listboxText);
//			for (int q = 0; q < DROPSIZE; q++) {
//				LPCWSTR listboxItem = a.getChilds(i)[q];
//				SendMessage(jsonControl[i], LB_ADDSTRING, 0, LPARAM(listboxItem));    //LB_DELETESTRING //LB_GETTEXT 
//			}
//			finalTouch(i);
//		}
//
//		if (ptr[i].ComponentType == "statusbar") {
//			InitCommonControls();
//			A[i] = 14000 + i;
//			controlStyle = SBARS_SIZEGRIP;
//			LPCWSTR statusbarText = initControl(i);
//			jsonControl[i] = createControl(STATUSCLASSNAME, hwnd, hInst, controlStyle, rectangle, A[i], statusbarText);
//			finalTouch(i);
//			int statwidths[] = { 100, -1 };
//			SendMessage(jsonControl[i], SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
//		}
//		if (ptr[i].ComponentType == "scrollbar") {
//			A[i] = 14200 + i;
//			if (ptr[i].Horizontal) controlStyle = SBS_HORZ;
//			else controlStyle = SBS_VERT;     //LVS_NOSCROLL | LVS_REPORT | LVS_NOCOLUMNHEADER | WS_VSCROLL | LVS_SHOWSELALWAYS | LVS_SINGLESEL
//			LPCWSTR scrollbarText = initControl(i);
//			jsonControl[i] = createControl(L"Scrollbar", hwnd, hInst, controlStyle, rectangle, A[i], scrollbarText);
//			SetScrollRange(jsonControl[i], SB_CTL, 0, 800, FALSE);
//			finalTouch(i);
//		}
//		if (ptr[i].ComponentType == "treeview") {
//			A[i] = 12800 + i;
//			INITCOMMONCONTROLSEX commonCtrls;
//			commonCtrls.dwSize = sizeof(commonCtrls);
//			commonCtrls.dwICC = ICC_TREEVIEW_CLASSES;   // TreeView class name
//			InitCommonControlsEx(&commonCtrls);
//			controlStyle = WS_BORDER | TVS_HASLINES;
//			const LPCWSTR treeviewText = initControl(i);
//			jsonControl[i] = createControl(WC_TREEVIEW, hwnd, hInst, controlStyle, rectangle, A[i], treeviewText);
//			finalTouch(i);
//			for (int q = 0; q < DROPSIZE; q++) {
//				LPCWSTR treeitem = a.getChilds(i)[q];
//				if (treeitem != nullptr) AddItemToTree(jsonControl[i], treeitem, 1);
//				//instead of 1 need to add item level here /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/////////
//				else break;
//			}
//
//		}
//		if (ptr[i].ComponentType == "tabcontrol") {
//			A[i] = 13000 + i;
//			controlStyle = WS_BORDER;
//			INITCOMMONCONTROLSEX icex;
//			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
//			icex.dwICC = ICC_TAB_CLASSES;
//			InitCommonControlsEx(&icex);
//			const LPCWSTR tabcontrolText = initControl(i);
//			jsonControl[i] = createControl(WC_TABCONTROL, hwnd, hInst, controlStyle, rectangle, A[i], tabcontrolText);
//			finalTouch(i);
//			TCITEM tie;
//			tie.mask = TCIF_TEXT | TCIF_IMAGE;
//			tie.iImage = -1;
//			for (int q = 0; q < DROPSIZE; q++) {
//				tie.pszText = (LPWSTR)a.getChilds(i)[q];
//				if (a.getChilds(i)[q] != nullptr) TabCtrl_InsertItem(jsonControl[i], q, &tie);
//				//instead of 1 need to add item level here /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/////////
//				else break;
//			}
//			//SetWindowTheme(jsonControl[i], L"Explorer", NULL);
//		}
//		if (ptr[i].ComponentType == "richedit") {
//			LoadLibrary(TEXT("Riched32.dll"));
//			A[i] = 13000 + i;
//			controlStyle = WS_BORDER | WS_TABSTOP | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL;
//			const LPCWSTR richeditText = initControl(i);
//			jsonControl[i] = createControl(L"RICHEDIT", hwnd, hInst, controlStyle, rectangle, A[i], richeditText);
//			finalTouch(i);
//			//SendMessage(jsonControl[i], EM_SETCUEBANNER,1, (LPARAM)L"Username");
//
//		}
//		if (ptr[i].ComponentType == "syslink") {
//			//RECT rc ={ 400,0,100,100 };
//			A[i] = 13000 + i;
//			//CreateSysLink(hwnd, hInst, rc);
//			controlStyle = WS_TABSTOP;
//			initControl(i);
//			jsonControl[i] = createControl(WC_LINK, hwnd, hInst, controlStyle, rectangle, A[i],
//				L"For more information, <A HREF=\"http://www.microsoft.com\">click here</A> " \
//				L"or <A ID=\"idInfo\">here</A>.");
//			finalTouch(i);
//
//
//
//		}
//		if (ptr[i].ComponentType == "toolbar") {
//			if (ptr[i].Name == "toolbar1")CreateSimpleToolbar(hwnd, hInst);
//		}
//		//array cell is empty
//		if (ptr[i].ComponentType.empty()) {
//			break;
//		}
//		//adding other components
//	}

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



