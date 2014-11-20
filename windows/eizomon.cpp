#include "eizomon.h"
#include "eizo_ctrl.h"

#define MAX_LOADSTRING 100

// Globals
HINSTANCE hInst;
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
RAWINPUTDEVICE rid;
bool keyboard_state[3];
eizo_ctrl_monitor *monitor;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	monitor = eizo_ctrl_open(EM_EIZO_FG_2421_VENDORID, EM_EIZO_FG_2421_PRODUCTID, NULL);
	if (!monitor) {
		MessageBox(NULL, L"Error: Can't find monitor.", szWindowClass, MB_OK);
		return false;
	}

	keyboard_state[STATE_SHIFT] = false;
	keyboard_state[STATE_CTRL] = false;
	keyboard_state[STATE_ALT] = false;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EIZOMON, szWindowClass, MAX_LOADSTRING);
	RegisterClassEizomon(hInstance);
	hWnd = CreateWindow(szWindowClass, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

	// Message pump
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1)
			return bRet;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}


ATOM RegisterClassEizomon(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EIZOMON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EIZOMON);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// register interest in raw data
		rid.dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;	// ignore legacy messages and receive system wide keystrokes	
		rid.usUsagePage = 1;							// raw keyboard data only
		rid.usUsage = 6;

		rid.hwndTarget = hWnd;
		RegisterRawInputDevices(&rid, 1, sizeof(rid));

		break;
	case WM_INPUT:
		ManageRawInput(message, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL ManageRawInput(UINT message, LPARAM lParam)
{
	UINT dwSize;

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)) == -1){
		return false;
	}
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL){
		return false;
	}
	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize){
		delete[] lpb;
		return false;
	}
	PRAWINPUT raw = (PRAWINPUT)lpb;
	UINT Event;

	Event = raw->data.keyboard.Message;

	if ((Event == WM_KEYDOWN || Event == WM_SYSKEYDOWN) && raw->data.keyboard.VKey == KEY_SHIFT)
		keyboard_state[STATE_SHIFT] = true;
	if ((Event == WM_KEYUP || Event == WM_SYSKEYUP) && raw->data.keyboard.VKey == KEY_SHIFT)
		keyboard_state[STATE_SHIFT] = false;

	if ((Event == WM_KEYDOWN || Event == WM_SYSKEYDOWN) && raw->data.keyboard.VKey == KEY_CTRL)
		keyboard_state[STATE_CTRL] = true;
	if ((Event == WM_KEYUP || Event == WM_SYSKEYUP) && raw->data.keyboard.VKey == KEY_CTRL)
		keyboard_state[STATE_CTRL] = false;

	if ((Event == WM_KEYDOWN || Event == WM_SYSKEYDOWN) && raw->data.keyboard.VKey == KEY_ALT)
		keyboard_state[STATE_ALT] = true;
	if ((Event == WM_KEYUP || Event == WM_SYSKEYUP) && raw->data.keyboard.VKey == KEY_ALT)
		keyboard_state[STATE_ALT] = false;


	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F1 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER1, 0);
	}
	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F2 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER2, 0);
	}
	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F3 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_USER3, 0);
	}

	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F4 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		if (monitor->backlight_status) {
			eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_BACKLIGHT, 0, 0);
			monitor->backlight_status = 0;
		}
		else {
			eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_BACKLIGHT, 1, 0);
			monitor->backlight_status = 1;
		}
	}

	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F5 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_FPS1, 0);
	}
	if (Event == WM_KEYUP &&  raw->data.keyboard.VKey == KEY_F6 &&
		keyboard_state[STATE_CTRL] && keyboard_state[STATE_SHIFT] && !keyboard_state[STATE_ALT]) {
		eizo_ctrl_command(monitor, EM_EIZO_FG_2421_CMD_MODE, EM_EIZO_FG_2421_MODE_FPS2, 0);
	}


	delete[] lpb;
	return true;
}
