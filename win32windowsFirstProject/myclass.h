#pragma once
#include <fstream>
#include "json.hpp"
#include "jsonParser.h"
#include <bemapiset.h>
#include <winuser.h>
#include "controlMaker.h"
#include <wincon.h>
using json = nlohmann::json;
class myclass
{
public:
	void init(json);
	void create(HWND parent, HINSTANCE hInst);
private:
	void loadFont(HDC hdc,HWND parent);
	item* ptr;
	json_parser jp1;
	HWND jsonControl[MAXCONTROLS];
	DWORD controlStyle;
	RECT rectangle;
	INT_PTR A[MAXCONTROLS];
	HFONT hFont;
	int nFontSize = 13;
	HBITMAP iconsENimages[MAXCONTROLS];
};

void myclass::loadFont(HDC hdc,HWND parent) {
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

inline void myclass::init(json myJson)
{
	ptr= jp1.initialize(myJson);
}

inline void myclass::create(HWND parent,HINSTANCE hInst)
{
	loadFont(GetDC(parent),parent);
	for ( int i=0;i<MAXCONTROLS;i++)
	{
		if (ptr[i].ComponentType == "button") {

			A[i] = 12000 + i;

			controlStyle = BS_PUSHBUTTON;
			if (ptr[i].ComponentType == "button") {
				if (ptr[i].TextAlignment == "right")controlStyle = BS_RIGHT;
				else if (ptr[i].TextAlignment == "left") controlStyle = BS_LEFT;
				else controlStyle = BS_CENTER;
			}

			if (ptr[i].ClickEvent == "newwindow") {
				A[i] = 1223;
			}

			const LPCWSTR buttonText = charArray2utf(jp1.getText(i), i);

			rectangle= RECT{ ptr[i].LocationX, ptr[i].LocationY, ptr[i].Width, ptr[i].Height };

			jsonControl[i] = createControl(L"button", parent, hInst, controlStyle, rectangle, A[i], buttonText);
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

			const LPWSTR buttoninfo = LPWSTR(charArray2utf(jp1.getInfo(i), i));
			CreateMyTooltip(jsonControl[i], buttoninfo, hFont);

			if (ptr[i].Icon == "exitIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"Close_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)); // 0,0 is meaning use default size of icon
			if (ptr[i].Icon == "miniIcon")iconsENimages[i] = HBITMAP(LoadImage(nullptr, L"down_24.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			SendMessage(jsonControl[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
		}
	}
}
