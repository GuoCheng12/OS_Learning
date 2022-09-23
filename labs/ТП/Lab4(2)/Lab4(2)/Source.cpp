#include <Windows.h>

#define ButtonFirst 0
#define ButtonTo 1
#define CmbBox 4
#define ChildWindow1 5
#define HOTKEX 7

bool onRead = false, onSolve = false;
char *Buffer;
HWND hwnd2;
HWND *hEdit;
HWND hverA, hverB;
HWND hwndComboBox;
HWND hwndButton3, hwndButton4;
WNDPROC DefEditProc;
HWND CreateEdit(int i, LPWSTR text, int x, int y, HWND hwnd, int hmenu);

bool read_graph();
extern "C" char minDistance(int* Buffer, int count, int verA, int verB);
char minDist(char* Buffer, int count);
void CreateEdits(int);

int count, curEdit = 0;
char verA, verB;

LRESULT CALLBACK Wnd2Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		static int sx, sy;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		onRead = false;
		break;

	case WM_COMMAND:
		switch (wParam) {
		case 8:
			char option[21];
			GetDlgItemText(hwnd, 4, LPWSTR(option), 20);
			count = char(option[0]) - 48;
			CreateEdits(count);
			hverA = CreateEdit(90, L"", 200, 50, hwnd2, 90);
			hverB = CreateEdit(91, L"", 200, 75, hwnd2, 91);
			break;

		case 9:
			read_graph();
			onRead = true;
			break;
		}
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
			if (curEdit < count * count - 1) {
				curEdit++;
				SetFocus(hEdit[curEdit]);
			}
			else {
				if (curEdit == count * count - 1) {
					curEdit++;
					SetFocus(hverA);
				}
				else {
					SetFocus(hverB);
					curEdit = -1;
				}
			}
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
	HWND result;
	char res;
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ButtonFirst:
			ShowWindow(hwnd2, SW_SHOW);
			UpdateWindow(hwnd2);
			break;

		case ButtonTo:
			res = minDist(Buffer, count);
			result = CreateEdit(92, L"", 200, 100, hwnd2, 92);
			char temp[10] = {};
			temp[0] = res + 48;
			SetWindowTextW(result, LPWSTR(temp));
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

	if (hwnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	HWND hwndButton1 = CreateButton(L"Данные", 50, 50, 75, 50, hwnd, ButtonFirst);
	HWND hwndButton2 = CreateButton(L"Решение", 150, 50, 75, 50, hwnd, ButtonTo);

	hwndButton3 = CreateButton(L"Выбрать", 100, 200, 60, 30, hwnd2, 8);
	hwndButton4 = CreateButton(L"Ввести", 200, 200, 60, 30, hwnd2, 9);

	hwndComboBox = CreateComboBox(L"", 20, 200, 50, 100, hwnd2, CmbBox);
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"1");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"2");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"3");
	SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"4");

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);
	EnableWindow(hwndButton2, false);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		if (onRead) {
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
	GetWindowText(hverA, LPWSTR(buff), 10);
	verA = buff[0] - 48;
	GetWindowText(hverB, LPWSTR(buff), 10);
	verB = buff[0] - 48;
	return true;
}

char minDist(char* Buffer, int count) {
	int *graph = (int *)malloc(count * count * sizeof(int));
	int *W = (int *)malloc(count * count * sizeof(int));
	for (int i = 0; i < strlen(Buffer); i++) {
		if (Buffer[i] >= 48 && Buffer[i] <= 57) {
			graph[i] = Buffer[i] - 48;
		}
	}

	for (int i = 0; i < count * count; i++)
		if (graph[i] != 0)
			W[i] = graph[i];
		else
			W[i] = 100;
	char res = minDistance(W, count, verA, verB);
	//////////////////////////////
	free(graph);
	free(W);
	return res;
}