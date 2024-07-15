#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <strsafe.h>
#include "resource.h"

#define CX_BITMAP 24
#define CY_BITMAP 24

WCHAR szClassName[] = L"Window";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	static HBITMAP hBitmap1;
	static HBITMAP hBitmap2;
	static HBITMAP hBitmap3;
	static HBITMAP hBitmap4;
	static HBITMAP hBitmap5;
	switch (msg)
	{
	case WM_CREATE:
		hCombo = CreateWindowW(L"COMBOBOX", 0, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		{
			hBitmap1 = LoadBitmapW(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCEW(IDB_BITMAP1));
			hBitmap2 = LoadBitmapW(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCEW(IDB_BITMAP2));
			hBitmap3 = LoadBitmapW(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCEW(IDB_BITMAP3));
			hBitmap4 = LoadBitmapW(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCEW(IDB_BITMAP4));
			hBitmap5 = LoadBitmapW(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCEW(IDB_BITMAP5));

			int nIndex;
			nIndex = (int)SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Bitmap1");
			SendMessage(hCombo, CB_SETITEMDATA, nIndex, (LPARAM)hBitmap1);
			nIndex = (int)SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Bitmap2");
			SendMessage(hCombo, CB_SETITEMDATA, nIndex, (LPARAM)hBitmap2);
			nIndex = (int)SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Bitmap3");
			SendMessage(hCombo, CB_SETITEMDATA, nIndex, (LPARAM)hBitmap3);
			nIndex = (int)SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Bitmap4");
			SendMessage(hCombo, CB_SETITEMDATA, nIndex, (LPARAM)hBitmap4);
			nIndex = (int)SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Bitmap5");
			SendMessage(hCombo, CB_SETITEMDATA, nIndex, (LPARAM)hBitmap5);
		}
		break;
	case WM_MEASUREITEM:
		{
			// Set the height of the items in the food groups combo box.
			LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lParam;

			if (lpmis->itemHeight < CY_BITMAP + 2)
				lpmis->itemHeight = CY_BITMAP + 2;

			break;
		}
	case WM_SIZE:
		MoveWindow(hCombo, 10, 10, 256, 256, TRUE);
		break;
	case WM_DRAWITEM:
		{
			COLORREF clrBackground;
			COLORREF clrForeground;
			TEXTMETRIC tm;
			int x;
			int y;
			HRESULT hr;
			size_t cch;

			LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;

			if (lpdis->itemID == -1) // Empty item)
				break;

			// Get the food icon from the item data.
			HBITMAP hbmIcon = (HBITMAP)lpdis->itemData;

			// The colors depend on whether the item is selected.
			clrForeground = SetTextColor(lpdis->hDC,
				GetSysColor(lpdis->itemState & ODS_SELECTED ?
					COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

			clrBackground = SetBkColor(lpdis->hDC,
				GetSysColor(lpdis->itemState & ODS_SELECTED ?
					COLOR_HIGHLIGHT : COLOR_WINDOW));

			// Calculate the vertical and horizontal position.
			GetTextMetrics(lpdis->hDC, &tm);
			y = (lpdis->rcItem.bottom + lpdis->rcItem.top - tm.tmHeight) / 2;
			x = LOWORD(GetDialogBaseUnits()) / 4;

			// Get and display the text for the list item.
			TCHAR achTemp[256];
			SendMessage(lpdis->hwndItem, CB_GETLBTEXT,
				lpdis->itemID, (LPARAM)achTemp);

			hr = StringCchLength(achTemp, 256, &cch);
			if (FAILED(hr))
			{
				// TODO: Write error handler.
			}

			ExtTextOut(lpdis->hDC, CX_BITMAP + 2 * x, y,
				ETO_CLIPPED | ETO_OPAQUE, &lpdis->rcItem,
				achTemp, (UINT)cch, NULL);

			// Restore the previous colors.
			SetTextColor(lpdis->hDC, clrForeground);
			SetBkColor(lpdis->hDC, clrBackground);

			//  Draw the food icon for the item. 
			HDC hdc = CreateCompatibleDC(lpdis->hDC);
			if (hdc == NULL)
				break;

		/*	SelectObject(hdc, hbmMask);
			BitBlt(lpdis->hDC, x, lpdis->rcItem.top + 1,
				CX_BITMAP, CY_BITMAP, hdc, 0, 0, SRCAND);*/

			SelectObject(hdc, hbmIcon);
			BitBlt(lpdis->hDC, x, lpdis->rcItem.top + 1,
				CX_BITMAP, CY_BITMAP, hdc, 0, 0, lpdis->itemState & ODS_SELECTED ? MERGEPAINT : SRCCOPY);

			DeleteDC(hdc);

			// If the item has the focus, draw the focus rectangle.
			if (lpdis->itemState & ODS_FOCUS)
				DrawFocusRect(lpdis->hDC, &lpdis->rcItem);

			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindowW(
		szClassName,
		L"Window",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
