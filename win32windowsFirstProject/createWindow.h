#include <Windows.h>
#pragma once
class createWindowControl {

public:
	static createWindowControl* CreateTestWindow(HINSTANCE hInstance);
	static createWindowControl* CreateTestWindow(HINSTANCE hInstance, HWND hParent);
	createWindowControl() : m_hWnd(0) {}
	~createWindowControl();

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LPCWSTR m_pszClassName;
	HWND m_hWnd;

};

LPCWSTR createWindowControl::m_pszClassName = L"viewvars";
//HWND parent;
createWindowControl * createWindowControl::CreateTestWindow(HINSTANCE hInst) {

	WNDCLASS wincl;
	if (!GetClassInfo(hInst, m_pszClassName, &wincl)) {
		wincl.style = CS_HREDRAW | CS_VREDRAW;
		wincl.hInstance = hInst;
		wincl.lpszClassName = m_pszClassName;
		wincl.lpfnWndProc = WindowProc;
		wincl.cbClsExtra = 0;
		wincl.cbWndExtra = 0;
		wincl.hIcon = NULL;
		wincl.hCursor = NULL;
		wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wincl.lpszMenuName = NULL;
		if (RegisterClass(&wincl) == 0) {
			MessageBox(NULL, L"The window class failed to register.", L"Error", 0);
			return 0;
		}
	}

	createWindowControl * pviewvars = new createWindowControl;
	HWND hWnd = CreateWindow(m_pszClassName, L"Test", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 50, 50, 300, 300, NULL, NULL, hInst, pviewvars);
	if (hWnd == NULL) {
		delete pviewvars;
		MessageBox(NULL, L"Problem creating the window.", L"Error", 0);
		return 0;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return pviewvars;

}
createWindowControl * createWindowControl::CreateTestWindow(HINSTANCE hInst, HWND hParent) {
	//parent = hParent;
	
	WNDCLASS wincl;
	if (!GetClassInfo(hInst, m_pszClassName, &wincl)) {
		wincl.style = CS_HREDRAW | CS_VREDRAW;
		wincl.hInstance = hInst;
		wincl.lpszClassName = m_pszClassName;
		wincl.lpfnWndProc = WindowProc;
		wincl.cbClsExtra = 0;
		wincl.cbWndExtra = 0;
		wincl.hIcon = NULL;
		wincl.hCursor = NULL;
		wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wincl.lpszMenuName = NULL;
		if (RegisterClass(&wincl) == 0) {
			MessageBox(NULL, L"The window class failed to register.", L"Error", 0);
			return 0;
		}
	}

	createWindowControl * pviewvars = new createWindowControl;
	HWND hWnd = CreateWindow(m_pszClassName, L"Test", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 50, 50, 300, 300, hParent, NULL, hInst, pviewvars);
	if (hWnd == NULL) {
		delete pviewvars;
		MessageBox(NULL, L"Problem creating the window.", L"Error", 0);
		return 0;
	}
	//EnableWindow(parent, false);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return pviewvars;

}


LRESULT CALLBACK createWindowControl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

	case WM_NCCREATE: {
		CREATESTRUCT * pcs = (CREATESTRUCT*)lParam;
		createWindowControl * pviewvars = (createWindowControl*)pcs->lpCreateParams;
		pviewvars->m_hWnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)pcs->lpCreateParams);
		return TRUE;
	}

	case WM_DESTROY: {
		createWindowControl * pviewvars = (createWindowControl *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pviewvars) pviewvars->m_hWnd = 0;
		ReleaseDC(hwnd,GetDC(hwnd));
		//EnableWindow(parent, true);
		break;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}

	return 0;

}

createWindowControl::~createWindowControl() {
	if (m_hWnd) DestroyWindow(m_hWnd);
}