#include <Windows.h>

#define ButtonFirst 0
#define ButtonTo 1
#define MenuFirst 2
#define MenuTo 3
#define CmbBox 4
#define ChildWindow1 5
#define ChildWindow2 6
#define HOTKEX 7

#pragma comment(lib, "DynamDLL.lib")
extern "C" __declspec(dllimport) int* getColors(char* Buffer, int count);

bool onRead = false, onSolve = false;
char *Buffer;
HWND hwnd2, hwnd3;
HWND *hEdit;
HWND hwndComboBox;
WNDPROC DefEditProc;
HWND CreateEdit(int i, LPWSTR text, int x, int y, HWND hwnd, int hmenu);

bool read_graph();
void CreateEdits(int);

int count, curEdit = 0;

LRESULT CALLBACK Wnd2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		static int sx, sy;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		break;

	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;

	case WM_COMMAND:
		switch (wParam) {
		case 8:
			char option[21];
			GetDlgItemText(hwnd, 4, LPWSTR(option), 20);
			count = char(option[0]) - 48;
			CreateEdits(count);
			break;

		case 9:
			if (read_graph())
				onSolve = true;
			break;
		}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK Wnd3Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBlackBrush, hOldBrush;
	int *colors;
	switch (message)
	{
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hwnd3, &ps);
		colors = (int*)malloc(count * sizeof(int));
		colors = getColors(Buffer, count);
		for (int i = 0; i < count; i++) {
			hBlackBrush = CreateSolidBrush(RGB((200 * i) % 255, (200 * i) % 255, (200 * i) % 255));
			SelectObject(hdc, hBlackBrush);
			Ellipse(hdc, i % count * 50 + 10, (i + 1) / count * 50 + 10, i % count * 50 + 50, (i + 1) / count * 50 + 50);
			int x = i % count * 50 + 30;
			int y = (i + 1) / count * 50 + 30;
			MoveToEx(hdc, x, y, NULL);
			if (i != count - 1)
				LineTo(hdc, (i + 1) % count * 50 + 30, (i + 2) / count * 50 + 30);
			else
				LineTo(hdc, (0) % count * 50 + 30, (1) / count * 50 + 30);
		}
		EndPaint(hwnd3, &ps);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK NewEditProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_KEYDOWN)
	{
		switch (wparam) {
		case VK_TAB:
			if (curEdit < count * count - 1)
				curEdit = curEdit + 1;
			else
				curEdit = 0;
			SetFocus(hEdit[curEdit]);
			break;
		default:
			break;
		}
		return 0;
	}
	else
	{
		//standard message handler
		return CallWindowProc(DefEditProc, hwnd, msg, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ButtonFirst:
			typedef char*(*MYPROC)(char*, int);
			HINSTANCE hinstLib_1;
			MYPROC ProcAdd;
			hinstLib_1 = LoadLibrary(L"DynamicDLL.dll");
			if (hinstLib_1 != NULL)
				ProcAdd = (MYPROC)GetProcAddress(hinstLib_1, "minDistance");
			if (ProcAdd != NULL) {
				Buffer = (*ProcAdd)(Buffer, count);
				for (int i = 0; i < count * count; i++) {
					char temp[10] = {};
					temp[0] = Buffer[i];
					SetWindowTextW(hEdit[i], LPWSTR(temp));
				}
				FreeLibrary(hinstLib_1);
			}
			break;

		case ButtonTo:
			typedef int(*TOPROC)(char*, int);
			HINSTANCE hinstLib_2;
			TOPROC ToProc;
			hinstLib_2 = LoadLibrary(L"DynamicDLL.dll");
			if (hinstLib_2 != NULL)
				ToProc = (TOPROC)GetProcAddress(hinstLib_2, "inspectionWay");
			if (ToProc != NULL) {
				int minIndex = (*ToProc)(Buffer, count);

				HWND editMinWay;
				editMinWay = CreateEdit(count * count, L"", 150, 50, hwnd2, 10 + count * count);
				SendMessage(editMinWay, EM_SETSEL, WPARAM(0), WPARAM(10));
				SendMessage(editMinWay, (UINT)EM_SETLIMITTEXT, (WPARAM)1, (LPARAM)0);

				char temp[10] = {};
				temp[0] = Buffer[minIndex];
				SetWindowTextW(editMinWay, LPWSTR(temp));
				FreeLibrary(hinstLib_2);
			}
			break;

		case MenuFirst:
			ShowWindow(hwnd2, SW_NORMAL);
			UpdateWindow(hwnd2);
			onSolve = false;
			break;

		case MenuTo:
			if (onSolve) {
				onRead = true;
				ShowWindow(hwnd3, SW_NORMAL);
				UpdateWindow(hwnd3);
			}
			else
				MessageBox(NULL, (LPCWSTR)L"Not data for solves", (LPCWSTR)L"Attention", MB_OK);
			break;

		default:
			break;
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;

	default: return DefWindowProc(hwnd, message, wParam, lParam); break;
	}
}

HWND CreateButton(LPWSTR text, int x, int y, int width, int height, HWND hwnd, int hmenu) {
	return CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		text,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		hwnd,     // Parent window
		(HMENU)hmenu,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL
	);
}

HWND CreateComboBox(LPWSTR text, int x, int y, int width, int height, HWND hwnd, int hmenu) {
	return CreateWindow(
		L"COMBOBOX",
		TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		x,
		y,
		width,
		height,
		hwnd,
		(HMENU)hmenu,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL
	);
}

HWND CreateEdit(int i, LPWSTR text, int x, int y, HWND hwnd, int hmenu) {
	HWND hEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		text,
		WS_CHILD | WS_VISIBLE | ES_NUMBER,
		x,
		y,
		20,
		20,
		hwnd,
		HMENU(i),
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		NULL
	);
	DefEditProc = (WNDPROC)GetWindowLongPtr(hEdit, GWLP_WNDPROC);
	SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)NewEditProc);
	return hEdit;
}

void CreateEdits(int count) {
	hEdit = (HWND *)malloc(count * count * sizeof(HWND));
	for (int i = 0; i < count * count; i++) {
		hEdit[i] = CreateEdit(i, L"", 50 + 40 * (i % count), 50 + 30 * (i / count), hwnd2, 10 + i);
		SendMessage(hEdit[i], EM_SETSEL, WPARAM(0), WPARAM(10));
		SendMessage(hEdit[i], (UINT)EM_SETLIMITTEXT, (WPARAM)1, (LPARAM)0);
	}
	SetFocus(hEdit[0]);
}

WNDCLASS myRegistryClass(HINSTANCE hInst, int cmdMode, WNDPROC proc, LPCWSTR s) {
	WNDCLASS wc{ sizeof(WNDCLASS) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = HBRUSH(BLACK_BRUSH);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInst;
	wc.lpfnWndProc = proc;
	wc.lpszClassName = s;
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wc);
	return wc;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstant, PWSTR pCmdLine, int nCmdShow)
{
	MSG msg{};
	WNDCLASS wc = myRegistryClass(hInstance, nCmdShow, WndProc, L"MyAppClass");
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"Главное окно",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
		0,
		0,
		600,
		600,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	wc = myRegistryClass(hInstance, nCmdShow, Wnd2Proc, L"MyChildClass");
	hwnd2 = CreateWindow(
		wc.lpszClassName,
		L"Ввод данных",
		WS_CHILD | WS_OVERLAPPEDWINDOW,
		300,
		0,
		300,
		300,
		hwnd,
		HMENU(ChildWindow1),
		wc.hInstance,
		nullptr
	);

	wc = myRegistryClass(hInstance, nCmdShow, Wnd3Proc, L"MyChild2Class");
	hwnd3 = CreateWindow(
		wc.lpszClassName,
		L"Выполнение действий",
		WS_CHILD | WS_OVERLAPPEDWINDOW,
		300,
		400,
		200,
		100,
		hwnd,
		HMENU(ChildWindow1),
		wc.hInstance,
		nullptr
	);

	if (hwnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	HMENU hMenu = CreateMenu();
	HMENU hMen = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMen, L"New");
	AppendMenu(hMen, MF_STRING, 2, L"First");
	AppendMenu(hMen, MF_STRING, 3, L"To");
	SetMenu(hwnd, hMenu);

	HWND hwndButton1 = CreateButton(L"1", 50, 50, 75, 50, hwnd, ButtonFirst);
	HWND hwndButton2 = CreateButton(L"2", 150, 50, 75, 50, hwnd, ButtonTo);

	HWND hwndButton3 = CreateButton(L"Select", 100, 200, 50, 30, hwnd2, 8);
	HWND hwndButton4 = CreateButton(L"Solve", 200, 200, 50, 30, hwnd2, 9);

	hwndComboBox = CreateComboBox(L"", 20, 200, 50, 100, hwnd2, CmbBox);
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"1");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"2");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"3");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"4");
	EnableWindow(hwndButton1, false);
	EnableWindow(hwndButton2, false);

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		if (onRead) {
			EnableWindow(hwndButton1, true);
			EnableWindow(hwndButton2, true);
		}
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

DWORD g_BytesTransferred = 0;

bool read_graph() {
	int editlength;
	CHAR buff[10];
	Buffer = new char[count*count];
	for (int i = 0; i < count * count; i++) {
		editlength = GetWindowTextLength(hEdit[i]);
		GetWindowText(hEdit[i], LPWSTR(buff), 10);
		Buffer[i] = buff[0];
	}
	return true;
}