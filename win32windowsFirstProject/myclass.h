#pragma once
#include <fstream>
#include "json.hpp"
#include "jsonParser.h"
#include <bemapiset.h>
#include <winuser.h>
#include "controlMaker.h"
#include <wincon.h>

class myclass
{
public:
	myclass(std::ifstream file);
	void create();
private:
	using json = nlohmann::json;
	item* ptr;
	json_parser jp1;
	HWND jsonControl[MAXCONTROLS];
};

inline myclass::myclass(std::ifstream file)
{
	json myJ = json::parse(file);
	ptr= jp1.initialize(myJ);
}

inline void myclass::create()
{
	for ( int i=0;i<MAXCONTROLS;i++)
	{
		if (ptr[i].ComponentType == "button") {
			controlStyle = BS_PUSHBUTTON;
			const LPCWSTR buttonText = initControl(i);

			if (ptr[i].ClickEvent == "returnBtnsName") {
				iReturnBtns = i;
				A[i] = 10001;
			}
			if (ptr[i].ClickEvent == "newwindow") {
				A[i] = 1223;

			}
			jsonControl[i] = createControl(L"button", hwnd, hInst, controlStyle, rectangle, A[i], buttonText);
			finalTouch(i);
			loadIcons(hwnd, GetDC(jsonControl[i]), i);
			SendMessage(jsonControl[i], BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(iconsENimages[i]));
			const LPWSTR buttoninfo = LPWSTR(charArray2utf(a.getInfo(i), i));
			CreateMyTooltip(jsonControl[i], buttoninfo, hFont);
		}
	}
}
