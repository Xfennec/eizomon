#pragma once

#include "resource.h"
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define KEY_SHIFT	0x10
#define KEY_CTRL	0x11
#define KEY_ALT		0x12
#define KEY_F1		0x70
#define KEY_F2		0x71
#define KEY_F3		0x72
#define KEY_F4		0x73
#define KEY_F5		0x74
#define KEY_F6		0x75
#define KEY_F7		0x76
#define KEY_F8		0x77
#define KEY_F9		0x78
#define KEY_F10		0x79
#define KEY_F11		0x7A
#define KEY_F12		0x7B

#define STATE_SHIFT	0
#define STATE_CTRL	1
#define STATE_ALT	2

ATOM				RegisterClassEizomon(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL ManageRawInput(UINT message, LPARAM lParam);
