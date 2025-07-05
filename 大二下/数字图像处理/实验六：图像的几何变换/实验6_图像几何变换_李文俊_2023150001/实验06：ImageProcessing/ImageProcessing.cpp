// ImageProcessing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <direct.h>
#include <commdlg.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

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

// ͼ���˲�

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage, int *Mask, int MaskWH, int MaskCoff); // ͼ��(3x3ģ�崦��)����
void AverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // ͼ���ֵ�˲�����
void GuassAverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);							   // ͼ���˹(ƽ��)�˲�����
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // ���ֵ�˲�
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // ���ֵ�˲�
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage);								   // ��ֵ�˲�3��3
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage);								   // ��ֵ�˲�5��5

// �������
void AddSaltAndPepperNoise(char *Image, int wImage, int hImage, double noise_ratio, double salt_vs_pepper); // �����������
void AddGaussianNoise(char *Image, int wImage, int hImage, double mean, double sigma);						// ��˹����

// ��Ե���
void LaplaceEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // ������˹����
void SobelEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Sobel����
void PrewittEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // Prewitt����
void RobertsEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // Roverts����
void CannyEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Canny����
void KirschEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Kirsch����

// ���α任
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array);	 // �򵥼��α任����
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage, int xPos, int yPos);			 // ƽ�Ʊ任
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage, double xScaling, double yScaling); // ���ű任
void ImageRotation(char *oImage, char *nImage, int wImage, int hImage, double iAngle);					 // ��ת�任
void ImageMirror(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal);					 // ����任
void ImageShearing(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal, int k);			 // ���б任
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
	srand((unsigned)time(NULL));
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
HWND hWind;					 // ���ھ��
char NewImage[1024 * 1024];	 // �µ�ͼ�񻺴�
char NewImage2[1024 * 1024]; // �µ�ͼ�񻺴�
char NewImage3[1024 * 1024]; // �µ�ͼ�񻺴�
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
			// �˲�����
		case IDM_AVERAGEFILTER: // ƽ���˲���
			AverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_AVERAGEGAUSS: // ��˹�˲���
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MAXFILTER: // ���ֵ�˲�
			MaxFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MINFILTER: // ���ֵ�˲�
			MinFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER3: // ��ֵ�˲�3*3
			MidvalueFilterProcessing3(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER5: // ��ֵ�˲�5*5
			MidvalueFilterProcessing5(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
			// �������
		case IDM_SaltPeper: // ��������
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Salt: // ������
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Pepper: // ������
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Gauss: // ��˹����
			AddGaussianNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0, 5);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
			// ��Ե���
		case IDM_LAPLACEFILTER: // Laplace��Ե����˲�
			LaplaceEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_LAPLACEFILTER2: // Laplace+��˹�˲���Ե����˲�
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			LaplaceEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS + 300, YPOS + 300);
			break;
		case IDM_SobelFILTER: // Sobel��Ե����˲�
			SobelEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 600);
			break;
		case IDM_PrewittFILTER: // Prewitt��Ե����˲�
			PrewittEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 900);
			break;
		case IDM_RobertsFILTER: // Roberts��Ե����˲�
			RobertsEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH - 1, IMAGEHEIGHT - 1, XPOS, YPOS + 1200);
			break;
		case IDM_KirschFILTER: // Kirsch��Ե����˲�
			KirschEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS + 300, YPOS);
			break;

		case IDM_CannyFILTER: // Canny��Ե����˲�
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			SobelEdgeProcessing(NewImage, NewImage2, IMAGEWIDTH, IMAGEHEIGHT);
			CannyEdgeProcessing(NewImage2, NewImage3, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage3, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 1500);
			break;
		case IDM_TRANSLATION: // ͼ��ƽ��
			ImageTranslation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 20, 30);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_SCALING: // ͼ������
			ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_ROTATION: // ͼ����ת
			ImageRotation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalMirror: // ͼ��ˮƽ��ת
			ImageMirror(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, true);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_VerticalMirror: // ͼ����ֱ��ת
			ImageMirror(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, false);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalShearing: // ͼ��ˮƽ����
			ImageShearing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, true, 1);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_VerticalShearing: // ͼ����ֱ����
			ImageShearing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, false, 1);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 600);
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
 *   ��ʾRAWͼ��																				 *
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

/************************************************************************************************
 *																								*
 *  ͼ��(kxkģ�崦��)����															 		      *
 *																								*
 ************************************************************************************************/

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage,
						 int *Mask, int MaskWH, int MaskCoff)
{
	int Coff;		// �����ۼӺ�
	int i, j, m, n; // ����Ԫ��
	int k;
	if (MaskWH % 2 != 0)
	{
		k = (MaskWH - 1) / 2;			 // ȷ���߽�
		for (i = k; i < hImage - k; i++) // ����ԭͼ��
		{
			for (j = k; j < wImage - k; j++)
			{
				Coff = 0;
				// �����˲���
				for (m = -k; m <= k; m++)
				{
					for (n = -k; n <= k; n++)
					{

						Coff += (BYTE)oImage[(i + m) * wImage + (j + n)] *
								Mask[(m + k) * MaskWH + (n + k)];
					}
				}
				nImage[i * wImage + j] = (unsigned char)(Coff / MaskCoff);
				if (Coff < 0)
					nImage[i * wImage + j] = 0;
			}
		}
	}
	else // ż��
	{
		k = MaskWH - 1; // k=1
		for (i = 0; i < hImage - k; i++)
		{
			for (j = 0; j < wImage - k; j++)
			{
				Coff = 0;
				for (m = 0; m <= k; m++)
				{
					for (n = 0; n <= k; n++)
					{
						Coff += (BYTE)oImage[(i + m) * wImage + (j + n)] *
								Mask[m * MaskWH + n];
					}
				}
				nImage[i * (wImage - k) + j] = (unsigned char)(Coff / MaskCoff);
				if (Coff < 0)
					nImage[i * (wImage - k) + j] = 0;
			}
		}
	}
}

/************************************************************************************************
 *																								*
 *  ͼ���ֵ�˲�����																		    *
 *																								*
 ************************************************************************************************/
void AverageMaskProcessing(char *oImage, char *nImage,
						   int wImage, int hImage)
{
	int Mask[9] = {1, 1, 1,
				   1, 1, 1,
				   1, 1, 1};

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 9);
}

/************************************************************************************************
 *																								*
 *  ͼ���˹(ƽ��)�˲�����																		 *
 *																								*
 ************************************************************************************************/

void GuassAverageMaskProcessing(char *oImage, char *nImage,
								int wImage, int hImage)
{
	int Mask[9] = {1, 2, 1,
				   2, 4, 2,
				   1, 2, 1};

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 16);
}
/************************************************************************************************
 *																								*
 *  ͼ�����ֵ�˲�����																		      *
 *																								*
 ************************************************************************************************/
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Coff;		// �����ҵ����ֵ
	int i, j, m, n; // ����Ԫ��
	int k;
	k = 1; // 3��3
	// ����ԭͼ��
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			Coff = 0;
			// �����˲���
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{

					if (Coff < (BYTE)oImage[(i + m) * wImage + (j + n)])
						Coff = (BYTE)oImage[(i + m) * wImage + (j + n)];
				}
			}
			nImage[i * wImage + j] = (unsigned char)Coff;
		}
	}
}
/************************************************************************************************
 *																								*
 *  ͼ����Сֵ�˲�����																		      *
 *																								*
 ************************************************************************************************/
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Coff;		// �����ҵ����ֵ
	int i, j, m, n; // ����Ԫ��
	int k;
	k = 1; // 3��3
	// ����ԭͼ��
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			Coff = 256;
			// �����˲���
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{

					if (Coff > (BYTE)oImage[(i + m) * wImage + (j + n)])
						Coff = (BYTE)oImage[(i + m) * wImage + (j + n)];
				}
			}
			nImage[i * wImage + j] = (unsigned char)Coff;
		}
	}
}
/************************************************************************************************
 *																								*
 *  ͼ����ֵ�˲�����																		      *
 *																								*
 ************************************************************************************************/
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage) // 3��3
{
	int k = 1;
	int Coff[3 * 3];
	int i, j, m, n; // ����Ԫ��
	// ����ԭͼ��
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			int cnt = 0;
			// �����ҵ���ֵ
			// �����˲���
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{
					Coff[cnt] = (BYTE)oImage[(i + m) * wImage + (j + n)];
					cnt++;
				}
			}
			for (int t = 0; t < cnt - 1; t++)
			{
				for (int q = 0; q < cnt - 1 - t; q++)
				{
					if (Coff[q] > Coff[q + 1])
					{
						int temp = Coff[q];
						Coff[q] = Coff[q + 1];
						Coff[q + 1] = temp;
					}
				}
			}
			nImage[i * wImage + j] = (unsigned char)Coff[4];
		}
	}
}
/************************************************************************************************
 *																								*
 *  ͼ���е��˲�����																		      *
 *																								*
 ************************************************************************************************/
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage) // 5��5
{
	int k = 2;
	int Coff[5 * 5];
	int i, j, m, n; // ����Ԫ��
	// ����ԭͼ��
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			int cnt = 0;
			// �����ҵ���ֵ
			// �����˲���
			for (m = -k; m <= k; m++)
			{
				for (n = -k; n <= k; n++)
				{
					Coff[cnt] = (BYTE)oImage[(i + m) * wImage + (j + n)];
					cnt++;
				}
			}
			for (int t = 0; t < cnt - 1; t++)
			{
				for (int q = 0; q < cnt - 1 - t; q++)
				{
					if (Coff[q] > Coff[q + 1])
					{
						int temp = Coff[q];
						Coff[q] = Coff[q + 1];
						Coff[q + 1] = temp;
					}
				}
			}
			nImage[i * wImage + j] = (unsigned char)Coff[12];
		}
	}
}
/************************************************************************************************
 *																								*
 *  �����ӽ�����������																		*
 *																								*
 ************************************************************************************************/
void AddSaltAndPepperNoise(char *Image, int wImage, int hImage, double noise_ratio, double salt_vs_pepper)
{
	int i, row, col;
	int total_pixels = wImage * hImage;
	int num_salt = (int)(total_pixels * noise_ratio * salt_vs_pepper); // ֱ��תΪint
	int num_pepper = (int)(total_pixels * noise_ratio * (1.0 - salt_vs_pepper));

	// �������������ɫ��
	for (i = 0; i < num_salt; i++)
	{
		row = rand() % hImage;
		col = rand() % wImage;
		Image[row * wImage + col] = (BYTE)255;
	}

	// ��ӽ���������ɫ��
	for (i = 0; i < num_pepper; i++)
	{
		row = rand() % hImage;
		col = rand() % wImage;
		Image[row * wImage + col] = (BYTE)0;
	}
}

/************************************************************************************************
 *																								*
 *  �����Ӹ�˹��������																		*
 *																								*
 ************************************************************************************************/
void AddGaussianNoise(char *Image, int wImage, int hImage, double mean, double sigma)
{
	srand(time(NULL));

	for (int i = 0; i < wImage * hImage; i++)
	{
		// ���ɸ�˹�ֲ�
		double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
		double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
		double Z0 = sqrt(-2 * log(U1)) * cos(2 * 3.141592653589793 * U2);

		// ��ȷ��ȡ����ֵ���޷��ţ�
		unsigned char pixel = static_cast<unsigned char>(Image[i]);
		int noise = static_cast<int>(Z0 * sigma + mean);

		// ����������ֵ
		int new_val = pixel + noise;
		new_val = max(0, min(255, new_val));
		Image[i] = static_cast<char>(new_val);
	}
}

/************************************************************************************************
 *																								*
 *  Laplace����    																		        *
 *																								*
 ************************************************************************************************/
void LaplaceEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Mask[9] = {0, 1, 0, // Laplace��Ե���ģ��
				   1, -4, 1,
				   0, 1, 0};

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1); // ����Ҫ��Ȩ�غ�
}
/************************************************************************************************
 *																								*
 *  Sobel ����    																		        *
 *																								*
 ************************************************************************************************/

void SobelEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	char *nImage1 = new char[wImage * hImage];
	int Mask1[9] = {-1, -2, -1, // ���¼��ģ��
					0, 0, 0,
					1, 2, 1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 3, 1);
	char *nImage2 = new char[wImage * hImage];
	int Mask2[9] = {-1, 0, 1, // ���Ҽ��ģ��
					-2, 0, 2,
					-1, 0, 1};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 3, 1);
	// ����ֵ���
	for (int i = 0; i < hImage; i++)
	{
		for (int j = 0; j < wImage; j++)
		{
			nImage[i * wImage + j] = abs((unsigned char)nImage1[i * wImage + j]) + abs((unsigned char)nImage2[i * wImage + j]);
		}
	}
	delete[] nImage1, nImage2;
}
/************************************************************************************************
 *																								*
 *  Prewitt ����    																		    *
 *																								*
 ************************************************************************************************/
void PrewittEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	char *nImage1 = new char[wImage * hImage];
	int Mask1[9] = {-1, -1, -1, // ���¼��ģ��
					0, 0, 0,
					1, 1, 1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 3, 1);
	char *nImage2 = new char[wImage * hImage];
	int Mask2[9] = {1, 0, -1, // ���Ҽ��ģ��
					1, 0, -1,
					1, 0, -1};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 3, 1);
	// ����ֵ���
	for (int i = 0; i < hImage; i++)
	{
		for (int j = 0; j < wImage; j++)
		{
			nImage[i * wImage + j] = abs((unsigned char)nImage1[i * wImage + j]) + abs((unsigned char)nImage2[i * wImage + j]);
		}
	}
	delete[] nImage1, nImage2;
}

/************************************************************************************************
 *																								*
 *  Roberts ����    																		    *
 *																								*
 ************************************************************************************************/
void RobertsEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	const int newWidth = wImage - 1;
	const int newHeight = hImage - 1;
	char *nImage1 = new char[newWidth * newHeight];
	int Mask1[4] = {1, 0, // ���¼��ģ��
					0, -1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 2, 1); // ��Ϊ2��2
	char *nImage2 = new char[newWidth * newHeight];
	int Mask2[4] = {0, 1, // ���¼��ģ��
					-1, 0};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 2, 1);
	// ����ֵ���
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			nImage[i * newHeight + j] = abs((unsigned char)nImage1[i * newHeight + j]) + abs((unsigned char)nImage2[i * newHeight + j]);
		}
	}
	delete[] nImage1, nImage2;
}
/************************************************************************************************
 *																								*
 *  Kirsch ����    																		    *
 *																								*
 ************************************************************************************************/
void KirschEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	// ���� Kirsch ���ӵ� 8 �� 3��3 �����
	const int kirsch_kernels[8][9] = {
		// 0��
		{-3, -3, 5,
		 -3, 0, 5,
		 -3, -3, 5},
		// 45��
		{-3, 5, 5,
		 -3, 0, 5,
		 -3, -3, -3},
		// 90��
		{5, 5, 5,
		 -3, 0, -3,
		 -3, -3, -3},
		// 135��
		{5, 5, -3,
		 5, 0, -3,
		 -3, -3, -3},
		// 180��
		{5, -3, -3,
		 5, 0, -3,
		 5, -3, -3},
		// 225��
		{-3, -3, -3,
		 5, 0, -3,
		 5, 5, -3},
		// 270��
		{-3, -3, -3,
		 -3, 0, -3,
		 5, 5, 5},
		// 315��
		{-3, -3, -3,
		 -3, 0, 5,
		 -3, 5, 5}};

	char *maxEdges = new char[wImage * hImage];
	std::fill_n(maxEdges, wImage * hImage, 0); // ��ʼ��Ϊ0

	// ��������8����
	for (int k = 0; k < 8; k++)
	{
		char *tempResult = new char[wImage * hImage];
		ImageMaskProcessing(oImage, tempResult, wImage, hImage, (int *)kirsch_kernels[k], 3, 1);

		// �������ֵ
		for (int i = 0; i < hImage; i++)
		{
			for (int j = 0; j < wImage; j++)
			{
				const int idx = i * wImage + j;
				const int absVal = abs(static_cast<unsigned char>(tempResult[idx]));
				if (absVal > maxEdges[idx])
				{
					maxEdges[idx] = static_cast<char>(absVal);
				}
			}
		}
		delete[] tempResult;
	}

	// �����ֵ������������ͼ��
	for (int i = 0; i < hImage; i++)
	{
		for (int j = 0; j < wImage; j++)
		{
			nImage[i * wImage + j] = maxEdges[i * wImage + j];
		}
	}

	delete[] maxEdges;
}
/************************************************************************************************
 *																								*
 *  Canny ����    																		        *
 *																								*
 ************************************************************************************************/
// �Ǽ���ֵ����
// ���� oImage �� Sobel �ݶȷ�ֵ���Ѽ��㣩��nImage ������ձ�Ե
void CannyEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	// �м�������ݶȷ���0~180�ȣ�������4������
	unsigned char *dir = new unsigned char[wImage * hImage];
	// �Ǽ���ֵ���ƽ��
	unsigned char *nms = new unsigned char[wImage * hImage];
	// ˫��ֵ��ǣ�255=ǿ��Ե��128=����Ե��0=�Ǳ�Ե
	unsigned char *thresholded = new unsigned char[wImage * hImage];
	double M_PI = 3.1415926;
	int i, j;
	// ����1���Ǽ���ֵ���ƣ�NMS��
	for (i = 1; i < hImage - 1; ++i)
	{
		for (j = 1; j < wImage - 1; ++j)
		{
			const int idx = i * wImage + j;
			// �����ݶȷ��� 0��, 45��, 90��, 135��
			const float gx = static_cast<signed char>(oImage[idx]); // ���� oImage �洢 Gx
			const float gy = static_cast<signed char>(dir[idx]);	// ���� dir �洢 Gy
			float angle = atan2(gy, gx) * 180.0f / M_PI;			// תΪ�Ƕ�
			if (angle < 0)
				angle += 180.0f;

			unsigned char q, r;
			// ���ݷ���ѡ��Ƚϵ��ھ�
			if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
			{
				q = oImage[idx - wImage]; // �Ϸ�
				r = oImage[idx + wImage]; // �·�
			}
			else if (angle >= 22.5 && angle < 67.5)
			{
				q = oImage[idx - wImage + 1]; // ����
				r = oImage[idx + wImage - 1]; // ����
			}
			else if (angle >= 67.5 && angle < 112.5)
			{
				q = oImage[idx + 1]; // �ҷ�
				r = oImage[idx - 1]; // ��
			}
			else
			{
				q = oImage[idx - wImage - 1]; // ����
				r = oImage[idx + wImage + 1]; // ����
			}

			// ���ƷǼ���ֵ
			const unsigned char curr = oImage[idx];
			nms[idx] = (curr >= q && curr >= r) ? curr : 0;
		}
	}

	// ����2��˫��ֵ��⣨�������ֵ=100������ֵ=50��
	const unsigned char high_thresh = 100;
	const unsigned char low_thresh = 50;
	for (i = 0; i < hImage * wImage; ++i)
	{
		if (nms[i] >= high_thresh)
		{
			thresholded[i] = 255; // ǿ��Ե
		}
		else if (nms[i] >= low_thresh)
		{
			thresholded[i] = 128; // ����Ե
		}
		else
		{
			thresholded[i] = 0; // �Ǳ�Ե
		}
	}

	// ����3���ͺ��Ե����
	for (i = 1; i < hImage - 1; ++i)
	{
		for (j = 1; j < wImage - 1; ++j)
		{
			const int idx = i * wImage + j;
			if (thresholded[idx] == 128)
			{ // ����Ե
				// ���8�����Ƿ����ǿ��Ե
				bool connected = false;
				for (int di = -1; di <= 1; ++di)
				{
					for (int dj = -1; dj <= 1; ++dj)
					{
						if (thresholded[idx + di * wImage + dj] == 255)
						{
							connected = true;
							break;
						}
					}
					if (connected)
						break;
				}
				thresholded[idx] = connected ? 255 : 0;
			}
		}
	}

	// ������ձ�Ե��ǿ��Ե + ���ӵ�����Ե��
	for (i = 0; i < hImage * wImage; ++i)
	{
		nImage[i] = (thresholded[i] == 255) ? 255 : 0;
	}

	// �ͷ��ڴ�
	delete[] dir;
	delete[] nms;
	delete[] thresholded;
}
/************************************************************************************************
 *																								*
 *  �򵥼��α任����    																		  *
 *																								*
 ************************************************************************************************/
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array)
{
	double axy, bxy;
	int i, sx, sy, ox, oy;
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0; // �����ͼ��
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // ��ͼ������Ϊԭ��
			oy = sy - hImage / 2;
			axy = sgt_Array[0] * (double)ox + sgt_Array[1] * (double)oy + sgt_Array[2];
			bxy = sgt_Array[3] * (double)ox + sgt_Array[4] * (double)oy + sgt_Array[5];
			axy += wImage / 2; // �ָ�ͼ��ԭ��λ��
			bxy += hImage / 2;
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy * wImage + sx] = 0; // ������Χ������0
			else
				nImage[sy * wImage + sx] = oImage[((int)bxy) * wImage + (int)axy];
		}
	}
}

/************************************************************************************************
 *																								*
 *  ƽ�Ʊ任    																		        *
 *																								*
 ************************************************************************************************/
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage,
					  int xPos, int yPos)
{
	double gt_Array[9] = {1, 0, 0,
						  0, 1, 0,
						  0, 0, 1};

	gt_Array[2] = xPos; // ˮƽ����ƽ�ƾ���
	gt_Array[5] = yPos; // ��ֱ����ƽ�ƾ���

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

/************************************************************************************************
 *																								*
 *  ���ű任   																		             *
 *																								*
 ************************************************************************************************/
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage,
				  double xScaling, double yScaling)
{
	double gt_Array[9] = {0, 0, 0,
						  0, 0, 0,
						  0, 0, 1};

	gt_Array[0] = 1.0 / xScaling; // ˮƽ����Ŵ���
	gt_Array[4] = 1.0 / yScaling; // ��ֱ����Ŵ���

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

/************************************************************************************************
 *																								*
 *  ��ת�任    																		        *
 *																								*
 ************************************************************************************************/

void ImageRotation(char *oImage, char *nImage, int wImage, int hImage,
				   double iAngle)
{
	double pi = 3.14159;
	double gt_Array[9] = {0, 0, 0,
						  0, 0, 0,
						  0, 0, 1};
	iAngle = (iAngle / 360) * 2 * pi;
	gt_Array[0] = cos(iAngle);
	gt_Array[1] = -sin(iAngle);
	gt_Array[3] = sin(iAngle);
	gt_Array[4] = cos(iAngle);

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}
/************************************************************************************************
 *																								*
 *  ����任    																		        *
 *																								*
 ************************************************************************************************/

void ImageMirror(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal)
{
	double gt_Array[9] = {-1, 0, 0,
						  0, 1, 0,
						  0, 0, 1};
	double gt_Array2[9] = {1, 0, 0,
						   0, -1, 0,
						   0, 0, 1};
	if (Horizontal)
		SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
	else
		SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array2);
}

/************************************************************************************************
 *																								*
 *  ����    																		        *
 *																								*
 ************************************************************************************************/

void ImageShearing(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal, int k)
{
	double gt_Array[9] = {1, 0, 0,
						  0, 1, 0,
						  0, 0, 1};

	if (Horizontal)
		gt_Array[1] = k;
	else
		gt_Array[3] = k;
	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}