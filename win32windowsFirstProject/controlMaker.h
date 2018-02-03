//controlMaker.h
#pragma once
typedef std::basic_string<TCHAR> ustring;
inline HWND createControl(LPCWSTR type, const HWND h_parent, const HINSTANCE h_inst, DWORD dwStyle, const RECT& rc,
                          INT_PTR id,const ustring& caption)
{

	//BS_LEFT for left alignment of text in button
	//BS_RIGHT for right alignment of text in button
	dwStyle |= WS_CHILD | WS_VISIBLE;
	return CreateWindow(              //extended styles
		type,                    //control 'class' name
		caption.c_str(),              //control caption
		dwStyle,                      //control style 
		rc.left,                      //position: left
		rc.top,                       //position: top
		rc.right,                     //width
		rc.bottom,                    //height
		h_parent,                      //parent window handle
									   //control's ID
		HMENU(id),
		h_inst,                        //application instance
		nullptr);                           //user defined info

}
