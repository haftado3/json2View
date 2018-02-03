//TooltipMaker.h
#pragma once
inline void CreateMyTooltip(HWND hwnd, LPWSTR tooltip, HFONT hFont) {

	INITCOMMONCONTROLSEX iccex;

	TOOLINFO ti;
	RECT rect;

	iccex.dwICC = ICC_WIN95_CLASSES;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCommonControlsEx(&iccex);

	const HWND hwnd_tt = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
	                             WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
	                             0, 0, 0, 0, hwnd, nullptr, nullptr, nullptr);


	SetWindowPos(hwnd_tt, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	GetClientRect(hwnd, &rect);

	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwnd;
	ti.hinst = nullptr;
	ti.uId = 0;
	ti.lpszText = tooltip;
	ti.rect.left = rect.left;
	ti.rect.top = rect.top;
	ti.rect.right = rect.right;
	ti.rect.bottom = rect.bottom;
	SendMessage(hwnd_tt, WM_SETFONT, WPARAM(hFont), LPARAM(MAKELONG(TRUE, 0)));
	SendMessage(hwnd_tt, TTM_ADDTOOL, 0, LPARAM(LPTOOLINFO(&ti)));
}