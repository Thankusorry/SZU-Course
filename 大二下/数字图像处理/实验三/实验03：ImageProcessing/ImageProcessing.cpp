// ImageProcessing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <direct.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

BOOL ReadImage(LPSTR, char *, int, int); // ��ȡͼ����Ϣ��������Image[][]��
void ShowImage(char *, int, int, int, int);
BOOL ReadBmpImage(LPSTR, char *);
void ShowBmpImage(char *, int, int, int, int);
void OpenImageFileDlg(char *);

void ShowImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect); // ��ʾͼ�񲿷�(����)����
void ShowLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos);			// ���м�������(����)��ʾ
void ShowTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos);			// �������°�Ҷ����ʾ
void ShowGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos);				// ������ʾ

void ShowBmpImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect); // ��ʾBmpͼ�񲿷�(����)����
void ShowBmpLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos);
void ShowBmpTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos);
void ShowBmpGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos);

// ͼ����ǿ

void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage); // ��ʾֱ��ͼ����
void ImageGrayReversionProcessing(char *Image, int wImage, int hImage);				   // ͼ��Ҷȷ�ת����
void ThresholdImage(char *oImage, int wImage, int hImage);							   // ͼ���ֵ������

HDC hWinDC;
int ImageWidth, ImageHeight;
char ImgDlgFileName[MAX_PATH];
char ImgDlgFileDir[MAX_PATH];
char OrgImage[1024 * 1024];
#define IMAGEWIDTH 256
#define IMAGEHEIGHT 256
#define XPOS 100
#define YPOS 100

// Global Variables:
HINSTANCE hInst;					 // current instance
TCHAR szTitle[MAX_LOADSTRING];		 // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // The title bar text

// Foward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGEPROCESSING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGEPROCESSING);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_IMAGEPROCESSING);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDC_IMAGEPROCESSING;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
HWND hWind;					// ���ھ��
char NewImage[1024 * 1024]; // �µ�ͼ�񻺴�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message)
	{
	case WM_CREATE:
		hWind = hWnd; // ��ȡ���ھ��
		hWinDC = GetWindowDC(hWnd);
		getcwd(ImgDlgFileDir, MAX_PATH);
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_HISTOGRAMSHOW: // ��ʾֱ��ͼ
			ImageHistogramShowProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_THRESHOLD: // ͼ���ֵ��
			ThresholdImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_GRAYREVERSION: // ͼ��Ҷȷ�ת
			ImageGrayReversionProcessing(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_SHOWRAWIMAGE:
			OpenImageFileDlg("��ͼ���ļ�");
			ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;

		case IDM_GRAYTOLOW: // ����
			ShowGrayToLow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
		case IDM_LEFTRIGHTOPEN: // �����ҿ���
			ShowLeftRightOpen(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
		case IDM_TBSHUTTER: // �������°�Ҷ��
			ShowTopBottomShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
			// Bmpͼ��:
		case IDM_SHOWBMPIMAGE:
			OpenImageFileDlg("��Bmpͼ���ļ�");
			ReadBmpImage(ImgDlgFileName, OrgImage);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_GRAYTOLOW: // ����
			ShowBmpGrayToLow(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_LEFTRIGHTOPEN: // �����ҿ���
			ShowBmpLeftRightOpen(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_TBSHUTTER: // �������°�Ҷ��
			ShowBmpTopBottomShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		RECT rt;
		GetClientRect(hWnd, &rt);
		DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/************************************************************************************************
 *																								*
 *   ���޸�ʽRAWͼ���ļ�																			*
 *																								*
 ************************************************************************************************/
BOOL ReadImage(LPSTR ImageFileName, char *oImage, int wImage, int hImage) // ��ȡͼ����Ϣ��������Image[][]��
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR)
	{
		MessageBox(NULL, ImageFileName, "���ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, oImage, wImage * hImage);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
 *																								*
 *   ��ʾRAWͼ��																					*
 *																								*
 ************************************************************************************************/
void ShowImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;

	for (i = 0; i < hImage; i++)
	{
		for (j = 0; j < wImage; j++)
		{
			SetPixel(hWinDC, j + yPos, i + xPos, RGB(Image[i * wImage + j], Image[i * wImage + j], Image[i * wImage + j]));
		}
	}
}

/************************************************************************************************
 *																								*
 *   ��BMPͼ���У�����ͼ��ͷ��Ϣ����Ҫ����ͼ�񳤶ȺͿ��											*
 *																								*
 ************************************************************************************************/
BOOL ReadBmpImage(LPSTR ImageFileName, char *oImage)
{
	BITMAPFILEHEADER bfImage;
	BITMAPINFOHEADER biImage;

	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR)
	{
		MessageBox(NULL, ImageFileName, "���ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, &bfImage, sizeof(BITMAPFILEHEADER));

	if ((bfImage.bfType != 0x4d42))
	{ // "BM"
		MessageBox(NULL, NULL, "��bmp�ļ�������Ϣ", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight = biImage.biHeight;
	if (biImage.biBitCount != 24)
	{ // 24λ��ɫͼ��
		MessageBox(NULL, NULL, "����24λbmpͼ��", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, bfImage.bfOffBits, 0);
	_lread(Image_fp, oImage, biImage.biWidth * biImage.biHeight * 3);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
 *																								*
 *   ��ʾBMPͼ��																					*
 *																								*
 ************************************************************************************************/
void ShowBmpImage(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	for (i = 0; i < hImage; i++)
	{
		for (j = 0; j < wImage; j++)
		{
			int line = hImage - 1 - i;
			SetPixel(hWinDC, j + yPos, line + xPos,
					 RGB(Image[(i * wImage + j) * 3 + 2],
						 Image[(i * wImage + j) * 3 + 1],
						 Image[(i * wImage + j) * 3]));
		}
	}
}
/************************************************************************************************
 *																								*
 *   �򿪶��ļ���ϵͳ�Ի���																		*
 *																								*
 ************************************************************************************************/
void OpenImageFileDlg(char *OPDLTitle)
{
	char FileTitle[100];
	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = TEXT("raw files\0*.raw\0All File\0*.*\0\0");
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = ImgDlgFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = FileTitle;
	ofn.nMaxFileTitle = 99;
	ofn.lpstrInitialDir = ImgDlgFileDir;
	ofn.lpstrTitle = OPDLTitle;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "raw";
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	ImgDlgFileName[0] = '\0';
	GetOpenFileName(&ofn);

	getcwd(ImgDlgFileDir, MAX_PATH);
}

/************************************************************************************************
 *																								*
 *  ��ʾͼ�񲿷�(����)������																	*
 *																								*
 ************************************************************************************************/
void ShowImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect)
{
	int i, j;
	int r, g, b;
	for (i = ShowRect.top; i < ShowRect.bottom; i++)
	{
		for (j = ShowRect.left; j < ShowRect.right; j++)
		{
			r = g = b = (BYTE)Image[i * wImage + j];
			SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
		}
	}
}

/************************************************************************************************
 *																								*
 *  ��ʾBMPͼ�񲿷�(����)������																*
 *																								*
 ************************************************************************************************/
void ShowBmpImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect)
{
	int i, j;
	int r, g, b;
	for (i = ShowRect.top; i < ShowRect.bottom; i++)
	{
		int line = hImage - i - 1;
		for (j = ShowRect.left; j < ShowRect.right; j++)
		{
			r = (BYTE)Image[(i * wImage + j) * 3 + 2];
			g = (BYTE)Image[(i * wImage + j) * 3 + 1];
			b = (BYTE)Image[(i * wImage + j) * 3];
			SetPixel(hWinDC, j + yPos, line + xPos, RGB(r, g, b));
		}
	}
}

/************************************************************************************************
 *																								*
 *  ���м�������(����)��ʾ����																	*
 *																								*
 ************************************************************************************************/

void ShowLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // ��մ���
	ShowRect.top = 0;
	ShowRect.bottom = hImage;
	for (i = 0; i < wImage / 2; i++)
	{
		ShowRect.left = wImage / 2 - i;
		ShowRect.right = ShowRect.left + 1;							// ��ʾһ��
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // ����

		ShowRect.left = wImage / 2 + i;
		ShowRect.right = ShowRect.left + 1;							// ��ʾһ��
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // �ҿ���
		Sleep(1);
	}
}

/************************************************************************************************
 *																								*
 *  ���м�������(����)��ʾBmp����																*
 *																								*
 ************************************************************************************************/

void ShowBmpLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // ��մ���
	ShowRect.top = 0;
	ShowRect.bottom = hImage;
	for (i = 0; i < wImage / 2; i++)
	{
		ShowRect.left = wImage / 2 - i;
		ShowRect.right = ShowRect.left + 1;							   // ��ʾһ��
		ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // ����

		ShowRect.left = wImage / 2 + i;
		ShowRect.right = ShowRect.left + 1;							   // ��ʾһ��
		ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // �ҿ���
		Sleep(1);
	}
}
/************************************************************************************************
 *																								*
 *  �������°�Ҷ����ʾ����																		*
 *																								*
 ************************************************************************************************/

void ShowTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // ��մ���
	ShowRect.left = 0;
	ShowRect.right = hImage;
	int SHUTTERNUM = 4;
	for (i = 0; i < wImage / SHUTTERNUM; i++)
	{ // SHUTTERNUM�ǰ�Ҷ������
		for (j = 0; j < SHUTTERNUM; j++)
		{
			ShowRect.top = i + (j * wImage / SHUTTERNUM);
			ShowRect.bottom = ShowRect.top + 1; // ��ʾһ��
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
			Sleep(1);
		}
	}
}
/************************************************************************************************
 *																								*
 *  �������°�Ҷ����ʾBmp����																	*
 *																								*
 ************************************************************************************************/
void ShowBmpTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // ��մ���
	ShowRect.left = 0;
	ShowRect.right = wImage;					   // Bmpͼ���������
	int SHUTTERNUM = 2;							   // SHUTTERNUM�ǰ�Ҷ������
	for (i = hImage / SHUTTERNUM - 1; i >= 0; i--) // ���µ���
	{
		for (j = 0; j < SHUTTERNUM; j++)
		{
			ShowRect.top = i + (j * hImage / SHUTTERNUM);
			ShowRect.bottom = ShowRect.top + 1; // ��ʾһ��
			ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
			Sleep(1);
		}
	}
}
/************************************************************************************************
 *																								*
 *  ������ʾ����																				*
 *																								*
 ************************************************************************************************/
void ShowGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;
	int r, g, b, gray;
	int GRAYCHANGENUM = 20;
	for (m = 1; m < GRAYCHANGENUM; m++)
	{
		for (i = 0; i < hImage; i++)
		{
			for (j = 0; j < wImage; j++)
			{
				gray = (BYTE)Image[i * wImage + j];
				r = g = b = gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, i + xPos, RGB(r, g, b));
			}
		}
	}
}

/************************************************************************************************
 *																								*
 *  ������ʾBmp����																				*
 *																								*
 ************************************************************************************************/
void ShowBmpGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j, m;
	int r, g, b, r_gray, g_gray, b_gray;
	int GRAYCHANGENUM = 20;
	for (m = 1; m < GRAYCHANGENUM; m++)
	{
		for (i = 0; i < hImage; i++)
		{
			int line = hImage - i - 1;
			for (j = 0; j < wImage; j++)
			{

				r_gray = (BYTE)Image[(i * wImage + j) * 3 + 2];
				g_gray = (BYTE)Image[(i * wImage + j) * 3 + 1];
				b_gray = (BYTE)Image[(i * wImage + j) * 3];
				r = r_gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				g = g_gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				b = b_gray * (GRAYCHANGENUM - m) / GRAYCHANGENUM;
				SetPixel(hWinDC, j + yPos, line + xPos, RGB(r, g, b));
			}
		}
	}
}
/************************************************************************************************
 *																								*
 *  ��ʾֱ��ͼ����																				 *
 *																								*
 ************************************************************************************************/
#define HISTOGRAMHIEGHT 150
void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int i, j, Gray, MaxGrayCount;
	int GrayCount[256];
	for (i = 0; i < 256; i++)
		GrayCount[i] = 0;
	for (i = 0; i < wImage * hImage; i++)
	{
		Gray = (BYTE)oImage[i];
		GrayCount[Gray]++;
	}
	MaxGrayCount = GrayCount[0];
	for (i = 0; i < 256; i++)
		if (MaxGrayCount < GrayCount[i])
			MaxGrayCount = GrayCount[i];
	for (i = 0; i < 256 * 256; i++)
		nImage[i] = (BYTE)192;
	for (j = 0; j < 256; j++)
	{
		GrayCount[j] = (int)(GrayCount[j] * HISTOGRAMHIEGHT / MaxGrayCount);
		for (i = 0; i < GrayCount[j]; i++)
			nImage[(255 - i) * 256 + j] = 0;
	}
}

/************************************************************************************************
 *																								*
 *  ͼ��Ҷȷ�ת����																		      *
 *																								*
 ************************************************************************************************/
void ImageGrayReversionProcessing(char *Image, int wImage, int hImage)
{
	int i;

	for (i = 0; i < wImage * hImage; i++)
		Image[i] = (BYTE)(255 - (BYTE)Image[i]);
}

/************************************************************************************************
 *																								*
 *  ͼ���ֵ������																				 *
 *																								*
 ************************************************************************************************/
int iThreshold = 128;
void ThresholdImage(char *Image, int wImage, int hImage)
{
	int i;

	for (i = 0; i < wImage * hImage; i++)
		if ((BYTE)Image[i] < iThreshold)
			Image[i] = 0;
		else
			Image[i] = (BYTE)255;
}
