#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
UIClass* UIClassC=0;
DEFINEFUNCTION DragWindowFun(LTExecState* es)
{
	UIClassC->DragWindow();
	return JsUndefined();
}

DEFINEFUNCTION QuitAppFun(LTExecState* es)
{
	UIClassC->QuitApp();
	return JsUndefined();
}
DEFINEFUNCTION MinWindowFun(LTExecState* es)
{
	UIClassC->MinWindow();
	return JsUndefined();
}

RECT CenterWindow(int w,int h)
{
	int scrWidth,scrHeight;
	RECT rect;
	scrWidth=GetSystemMetrics(SM_CXSCREEN);
	scrHeight=GetSystemMetrics(SM_CYSCREEN);
	rect.left=(scrWidth-w)/2;
	rect.top=(scrHeight-h)/2;
	return rect;
}


UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 640,wh=510;
	OleInitialize(0);

	RECT rect = CenterWindow(ww,wh);

	ltskinview= new LTFrameView(this,L"LTFrame-Canvas",NULL,WS_POPUP | WS_VISIBLE,rect.left,rect.top,ww,wh,0);
	UIClassC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun,0);
	ltskinview->BindUserFunction("QuitApp",QuitAppFun,0);
	ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);

	SetWindowText(ltskinview->windowHandle(),L"LTFrame-Canvas");	


	wstring path = wstring(applicationpath)+L"./template/game/index.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
} 


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}
void UIClass::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void UIClass::QuitApp()
{
	ltskinview->CloseWindow();
}
void UIClass::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);

}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"Canvas";
}
