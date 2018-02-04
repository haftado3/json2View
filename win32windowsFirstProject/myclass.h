#pragma once
#include "json.hpp"
#include "jsonParser.h"
#include <winuser.h>
#include "controlMaker.h"
#include "TooltipMaker.h"
#include "addItem2Tree.h"
#include "createToolbar.h"
#include "resource.h"
#define UD_MAX_POS 30
#define UD_MIN_POS 0

#define TB_MAX_POS 100
#define TB_MIN_POS 0
using json = nlohmann::json;
class myclass
{
public:
	void setJson(json);
	LPCWSTR initControl(int i);
	void loadIcons(HWND parent, HDC hdc, int i);
	void adjustControl(int i,HWND parent);
	void create(HWND parent, HINSTANCE hInst);
private:
	void loadFont(HDC hdc,HWND parent);
	item* ptr = nullptr;
	json_parser jp1;
	HWND jsonControl[MAXCONTROLS]{};
	DWORD controlStyle = 0;
	RECT rectangle{};
	INT_PTR A[MAXCONTROLS]{};
	HFONT hFont = nullptr;
	int nFontSize = 13;
	HBITMAP iconsENimages[MAXCONTROLS]{};
};

inline void myclass::loadFont(HDC hdc,HWND parent) {
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
	ReleaseDC(parent, hdc);
}

inline void myclass::setJson(json myJson)
{
	ptr= jp1.initialize(myJson);
}


inline LPCWSTR myclass::initControl(int i)
{
	if (ptr[i].ComponentType == "button") {
		if (ptr[i].TextAlignment == "right")controlStyle = BS_RIGHT;
		else if (ptr[i].TextAlignment == "left") controlStyle = BS_LEFT;
		else controlStyle = BS_CENTER;
	}
	rectangle = RECT{ ptr[i].LocationX, ptr[i].LocationY, ptr[i].Width, ptr[i].Height };
	return charArray2utf(jp1.getText(i), i);
}
void myclass::loadIcons(HWND parent, HDC hdc, int i) {
	const HDC hMemDC = CreateCompatibleDC(hdc);
	try
	{
		if (ptr[i].Icon == "imageVenus")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"VENUS.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "ship")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"ship.bmp", IMAGE_BITMAP, ptr[i].Width, ptr[i].Height, LR_LOADFROMFILE));
		if (ptr[i].Icon == "exitIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"Close_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // 0,0 is meaning use default size of icon
		if (ptr[i].Icon == "miniIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"down_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "addIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"add_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "cancelIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"cancl_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (ptr[i].Icon == "okIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"ok_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	}
	catch (exception e)
	{
		MessageBox(parent, L"Cant load icon file.", L"ERORR!", MB_OK);
	}
	SelectObject(hMemDC, iconsENimages[i]);
	DeleteDC(hMemDC);
	ReleaseDC(parent, hdc);
}
inline void myclass::adjustControl(int i,HWND parent)
{
	try {
		loadIcons(parent, GetDC(jsonControl[i]), i);
		SendMessage(jsonControl[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
	}
	catch (exception e) {}

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

	
	try {
		const LPWSTR controlInfo = LPWSTR(charArray2utf(jp1.getInfo(i), i));
		CreateMyTooltip(jsonControl[i], controlInfo, hFont);
	}
	catch(exception e){}
}

inline void myclass::create(HWND parent,HINSTANCE hInst)
{
	loadFont(GetDC(parent),parent);
	for (int i = 0; i < MAXCONTROLS; i++)
	{
		if (ptr[i].ComponentType == "button") {

			A[i] = 10000 + i;
			if (ptr[i].ClickEvent == "newwindow") {
				A[i] = 1223;
			}
			controlStyle = BS_PUSHBUTTON;

			jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);
			/*
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
		}
		if (ptr[i].ComponentType == "label") {
			A[i] = 10800 + i;
			controlStyle = NULL;
			jsonControl[i] = createControl(L"static", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);
		}
		if (ptr[i].ComponentType == "image") {
			A[i] = 10900 + i;
			controlStyle = SS_BITMAP;
			jsonControl[i] = createControl(L"static", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);
		}
		//		if (ptr[i].ComponentType == "image") {
		//			A[i] = 10900 + i;
		//			controlStyle = SS_BITMAP;
		//			const LPCWSTR imageText = initControl(i);
		//			jsonControl[i] = createControl(L"static", hwnd, hInst, controlStyle, rectangle, A[i], imageText);
		//			loadIcons(hwnd, GetDC(jsonControl[i]), i);
		//			SendMessage(jsonControl[i], STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
		//			finalTouch(i);
		//		}
		if (ptr[i].ComponentType == "checkbox") {
			A[i] = 11000 + i;
			controlStyle = BS_CHECKBOX;
			jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			CheckDlgButton(jsonControl[i], A[i], BST_CHECKED);
			adjustControl(i, parent);;
		}
		if (ptr[i].ComponentType == "textbox") {
			A[i] = 12000 + i;
			controlStyle = WS_BORDER;
			jsonControl[i] = createControl(L"edit", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;

		}
		if (ptr[i].ComponentType == "groupbox") {
			A[i] = 12200 + i;
			controlStyle = BS_GROUPBOX | BS_RIGHT;
			jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
		}
		if (ptr[i].ComponentType == "radiobutton") {
			A[i] = 12400 + i;
			if (ptr[i].Newgroup) {
				controlStyle = BS_AUTORADIOBUTTON | WS_GROUP;
				jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
				SendMessage(jsonControl[i], BM_SETCHECK, 1, 0);
			}
			else
			{
				controlStyle = BS_AUTORADIOBUTTON;
				jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			}
			adjustControl(i, parent);;
		}
		if (ptr[i].ComponentType == "combobox") {
			A[i] = 12600 + i;
			controlStyle = CBS_DROPDOWNLIST;
			jsonControl[i] = createControl(L"combobox", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			for (int q = 0; q < DROPSIZE; q++) {
				LPCWSTR comboboxItem = jp1.getChilds(i)[q];
				SendMessage(jsonControl[i], CB_ADDSTRING, 0, LPARAM(comboboxItem));
			}
			SendMessage(jsonControl[i], CB_SETCURSEL, 0, 0);
		}

		if (ptr[i].ComponentType == "updown") {
			A[i] = 13000 + i;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_UPDOWN_CLASS;
			InitCommonControlsEx(&icex);
			controlStyle = UDS_SETBUDDYINT | UDS_ALIGNLEFT;
			DWORD editStyle = WS_BORDER | ES_READONLY | SS_CENTER; //read and write with EM_SETREADONLY
			jsonControl[i] = createControl(UPDOWN_CLASS, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			jsonControl[i + 1] = createControl(L"edit", parent, hInst, editStyle, rectangle, A[i] + 1, L"0");
			SendMessage(jsonControl[i], UDM_SETBUDDY, (WPARAM)jsonControl[i + 1], 0);
			SendMessageW(jsonControl[i], UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
			i++;
		}
		if (ptr[i].ComponentType == "trackbar") {
			A[i] = 13200 + i;
			controlStyle = TBS_AUTOTICKS;
			DWORD editStyle = WS_BORDER | ES_READONLY | SS_CENTER;
			RECT staticrect = { 0,0,30,20 };
			jsonControl[i] = createControl(TRACKBAR_CLASS, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			jsonControl[i + 1] = createControl(L"static", parent, hInst, editStyle, staticrect, A[i] + 1, L"0");
			jsonControl[i + 2] = createControl(L"static", parent, hInst, editStyle, staticrect, A[i] + 2, L"100");
			SendMessageW(jsonControl[i], TBM_SETRANGE, TRUE, MAKELONG(TB_MIN_POS, TB_MAX_POS));
			SendMessageW(jsonControl[i], TBM_SETPAGESIZE, 0, 10);
			SendMessageW(jsonControl[i], TBM_SETTICFREQ, 10, 0);
			SendMessageW(jsonControl[i], TBM_SETPOS, FALSE, 0);
			SendMessageW(jsonControl[i], TBM_SETBUDDY, TRUE, (LPARAM)jsonControl[i + 1]);
			SendMessageW(jsonControl[i], TBM_SETBUDDY, FALSE, (LPARAM)jsonControl[i + 2]);
		}
		if (ptr[i].ComponentType == "monthcal") {
			A[i] = 13400 + i;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(icex);
			icex.dwICC = ICC_DATE_CLASSES;
			InitCommonControlsEx(&icex);
			jsonControl[i] = createControl(MONTHCAL_CLASS, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;

		}
		if (ptr[i].ComponentType == "progressbar") {

			A[i] = 13600 + i;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_PROGRESS_CLASS;
			InitCommonControlsEx(&icex);
			controlStyle = PBS_SMOOTH;
			jsonControl[i] = createControl(PROGRESS_CLASS, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			SendMessage(jsonControl[i], PBM_SETRANGE, 0, MAKELPARAM(0, 100));
			SendMessage(jsonControl[i], PBM_SETSTEP, 1, 0);
			SendMessage(jsonControl[i], PBM_SETPOS, 100, 0);
		}
		if (ptr[i].ComponentType == "menubar") {
			HMENU hMenubar = CreateMenu();
			HMENU hMenu = CreateMenu();
			for (int q = 0; q < DROPSIZE; q++) {
				LPCWSTR menuItem = jp1.getChilds(i)[q];
				AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, menuItem);
			}
			NONCLIENTMETRICS rNCM;
			rNCM.cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &rNCM, 0);
			lstrcpyn(rNCM.lfMenuFont.lfFaceName, L"B Nazanin", sizeof(rNCM.lfMenuFont.lfFaceName));
			SystemParametersInfo(SPI_SETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &rNCM, 0);
			rNCM.lfMenuFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(GetDC(jsonControl[i]), LOGPIXELSY), 72);
			SetMenu(parent, hMenubar);
		}
		if (ptr[i].ComponentType == "listbox") {
			A[i] = 13800 + i;
			controlStyle = ES_AUTOVSCROLL | LBS_NOTIFY | WS_BORDER;
			jsonControl[i] = createControl(TEXT("listbox"), parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			for (int q = 0; q < DROPSIZE; q++) {
				LPCWSTR listboxItem = jp1.getChilds(i)[q];
				SendMessage(jsonControl[i], LB_ADDSTRING, 0, LPARAM(listboxItem));    //LB_DELETESTRING //LB_GETTEXT 
			}
			adjustControl(i, parent);;
		}

		if (ptr[i].ComponentType == "statusbar") {
			InitCommonControls();
			A[i] = 14000 + i;
			controlStyle = SBARS_SIZEGRIP;
			jsonControl[i] = createControl(STATUSCLASSNAME, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			int statwidths[] = { 100, -1 };
			SendMessage(jsonControl[i], SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
		}
		if (ptr[i].ComponentType == "scrollbar") {
			A[i] = 14200 + i;
			if (ptr[i].Horizontal) controlStyle = SBS_HORZ;
			else controlStyle = SBS_VERT;     //LVS_NOSCROLL | LVS_REPORT | LVS_NOCOLUMNHEADER | WS_VSCROLL | LVS_SHOWSELALWAYS | LVS_SINGLESEL
			jsonControl[i] = createControl(L"Scrollbar", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			SetScrollRange(jsonControl[i], SB_CTL, 0, 800, FALSE);
			adjustControl(i, parent);;
		}
		if (ptr[i].ComponentType == "treeview") {
			A[i] = 12800 + i;
			INITCOMMONCONTROLSEX commonCtrls;
			commonCtrls.dwSize = sizeof(commonCtrls);
			commonCtrls.dwICC = ICC_TREEVIEW_CLASSES;   // TreeView class name
			InitCommonControlsEx(&commonCtrls);
			controlStyle = WS_BORDER | TVS_HASLINES;
			jsonControl[i] = createControl(WC_TREEVIEW, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			for (int q = 0; q < DROPSIZE; q++) {
				LPCWSTR treeitem = jp1.getChilds(i)[q];
				if (treeitem != nullptr) AddItemToTree(jsonControl[i], treeitem, 1);
				//instead of 1 need to add item level here /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/////////
				else break;
			}

		}
		if (ptr[i].ComponentType == "tabcontrol") {
			A[i] = 13000 + i;
			controlStyle = WS_BORDER;
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_TAB_CLASSES;
			InitCommonControlsEx(&icex);
			jsonControl[i] = createControl(WC_TABCONTROL, parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			TCITEM tie;
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = -1;
			for (int q = 0; q < DROPSIZE; q++) {
				tie.pszText = (LPWSTR)jp1.getChilds(i)[q];
				if (jp1.getChilds(i)[q] != nullptr) TabCtrl_InsertItem(jsonControl[i], q, &tie);
				//instead of 1 need to add item level here /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/////////
				else break;
			}
			//SetWindowTheme(jsonControl[i], L"Explorer", NULL);
		}
		if (ptr[i].ComponentType == "richedit") {
			LoadLibrary(TEXT("Riched32.dll"));
			A[i] = 13000 + i;
			controlStyle = WS_BORDER | WS_TABSTOP | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL;
			jsonControl[i] = createControl(L"RICHEDIT", parent, hInst, controlStyle, rectangle, A[i], initControl(i));
			adjustControl(i, parent);;
			//SendMessage(jsonControl[i], EM_SETCUEBANNER,1, (LPARAM)L"Username");

		}
		if (ptr[i].ComponentType == "syslink") {
			//RECT rc ={ 400,0,100,100 };
			A[i] = 13000 + i;
			//CreateSysLink(parent, hInst, rc);
			controlStyle = WS_TABSTOP;
			initControl(i);
			jsonControl[i] = createControl(WC_LINK, parent, hInst, controlStyle, rectangle, A[i],
				L"For more information, <A HREF=\"http://www.microsoft.com\">click here</A> " \
				L"or <A ID=\"idInfo\">here</A>.");
			adjustControl(i, parent);;



		}
		if (ptr[i].ComponentType == "toolbar") {
			if (ptr[i].Name == "toolbar1")CreateSimpleToolbar(parent, hInst);
		}
		//array cell is empty
		if (ptr[i].ComponentType.empty()) {
			break;
		}
	}
}
