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

BOOL ReadImage(LPSTR, char *, int, int); // 读取图像信息并保存在Image[][]中
void ShowImage(char *, int, int, int, int);
BOOL ReadBmpImage(LPSTR, char *);
void ShowBmpImage(char *, int, int, int, int);
void OpenImageFileDlg(char *);

void ShowImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect); // 显示图像部分(矩形)区域
void ShowLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos);			// 由中间向左右(开门)显示
void ShowTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos);			// 由上向下百叶窗显示
void ShowGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos);				// 淡入显示

void ShowBmpImageRect(char *Image, int wImage, int hImage, int xPos, int yPos, RECT ShowRect); // 显示Bmp图像部分(矩形)区域
void ShowBmpLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos);
void ShowBmpTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos);
void ShowBmpGrayToLow(char *Image, int wImage, int hImage, int xPos, int yPos);

// 图像增强

void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage); // 显示直方图函数
void ImageGrayReversionProcessing(char *Image, int wImage, int hImage);				   // 图像灰度反转函数
void ThresholdImage(char *oImage, int wImage, int hImage);							   // 图像二值化函数

// 图像滤波

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage, int *Mask, int MaskWH, int MaskCoff); // 图像(3x3模板处理)函数
void AverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // 图像均值滤波函数
void GuassAverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);							   // 图像高斯(平滑)滤波函数
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // 最大值滤波
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);									   // 最大值滤波
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage);								   // 中值滤波3×3
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage);								   // 中值滤波5×5

// 随机噪声
void AddSaltAndPepperNoise(char *Image, int wImage, int hImage, double noise_ratio, double salt_vs_pepper); // 随机生成噪声
void AddGaussianNoise(char *Image, int wImage, int hImage, double mean, double sigma);						// 高斯噪声

// 边缘检测
void LaplaceEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // 拉普拉斯算子
void SobelEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Sobel算子
void PrewittEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // Prewitt算子
void RobertsEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage); // Roverts算子
void CannyEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Canny算子
void KirschEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage);	// Kirsch算子

// 几何变换
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array);	 // 简单几何变换函数
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage, int xPos, int yPos);			 // 平移变换
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage, double xScaling, double yScaling); // 缩放变换
void ImageRotation(char *oImage, char *nImage, int wImage, int hImage, double iAngle);					 // 旋转变换
void ImageMirror(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal);					 // 镜像变换
void ImageShearing(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal, int k);			 // 错切变换
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
HWND hWind;					 // 窗口句柄
char NewImage[1024 * 1024];	 // 新的图像缓存
char NewImage2[1024 * 1024]; // 新的图像缓存
char NewImage3[1024 * 1024]; // 新的图像缓存
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
		hWind = hWnd; // 获取窗口句柄
		hWinDC = GetWindowDC(hWnd);
		getcwd(ImgDlgFileDir, MAX_PATH);
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_HISTOGRAMSHOW: // 显示直方图
			ImageHistogramShowProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_THRESHOLD: // 图像二值化
			ThresholdImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_GRAYREVERSION: // 图像灰度反转
			ImageGrayReversionProcessing(OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_SHOWRAWIMAGE:
			OpenImageFileDlg("打开图像文件");
			ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;

		case IDM_GRAYTOLOW: // 淡入
			ShowGrayToLow(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
		case IDM_LEFTRIGHTOPEN: // 向左右开门
			ShowLeftRightOpen(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
		case IDM_TBSHUTTER: // 由上向下百叶窗
			ShowTopBottomShutter(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			break;
			// Bmp图像:
		case IDM_SHOWBMPIMAGE:
			OpenImageFileDlg("打开Bmp图像文件");
			ReadBmpImage(ImgDlgFileName, OrgImage);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_GRAYTOLOW: // 淡入
			ShowBmpGrayToLow(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_LEFTRIGHTOPEN: // 向左右开门
			ShowBmpLeftRightOpen(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case BMP_TBSHUTTER: // 由上向下百叶窗
			ShowBmpTopBottomShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
			// 滤波函数
		case IDM_AVERAGEFILTER: // 平均滤波器
			AverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_AVERAGEGAUSS: // 高斯滤波器
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MAXFILTER: // 最大值滤波
			MaxFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MINFILTER: // 最大值滤波
			MinFilterProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER3: // 中值滤波3*3
			MidvalueFilterProcessing3(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER5: // 中值滤波5*5
			MidvalueFilterProcessing5(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
			// 随机噪声
		case IDM_SaltPeper: // 椒盐噪声
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0.5);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Salt: // 盐噪声
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 1);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Pepper: // 椒噪声
			AddSaltAndPepperNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0.5, 0);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_Gauss: // 高斯噪声
			AddGaussianNoise(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, 0, 5);
			ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
			// 边缘检测
		case IDM_LAPLACEFILTER: // Laplace边缘检测滤波
			LaplaceEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_LAPLACEFILTER2: // Laplace+高斯滤波边缘检测滤波
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			LaplaceEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS + 300, YPOS + 300);
			break;
		case IDM_SobelFILTER: // Sobel边缘检测滤波
			SobelEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 600);
			break;
		case IDM_PrewittFILTER: // Prewitt边缘检测滤波
			PrewittEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 900);
			break;
		case IDM_RobertsFILTER: // Roberts边缘检测滤波
			RobertsEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH - 1, IMAGEHEIGHT - 1, XPOS, YPOS + 1200);
			break;
		case IDM_KirschFILTER: // Kirsch边缘检测滤波
			KirschEdgeProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS + 300, YPOS);
			break;

		case IDM_CannyFILTER: // Canny边缘检测滤波
			GuassAverageMaskProcessing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT);
			SobelEdgeProcessing(NewImage, NewImage2, IMAGEWIDTH, IMAGEHEIGHT);
			CannyEdgeProcessing(NewImage2, NewImage3, IMAGEWIDTH, IMAGEHEIGHT);
			ShowImage(NewImage3, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 1500);
			break;
		case IDM_TRANSLATION: // 图像平移
			ImageTranslation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 20, 30);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_SCALING: // 图像缩放
			ImageScaling(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 2.0, 0.5);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_ROTATION: // 图像旋转
			ImageRotation(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, 45.0);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalMirror: // 图像水平翻转
			ImageMirror(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, true);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_VerticalMirror: // 图像竖直翻转
			ImageMirror(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, false);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalShearing: // 图像水平错切
			ImageShearing(OrgImage, NewImage, IMAGEWIDTH, IMAGEHEIGHT, true, 1);
			ShowImage(NewImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS + 300);
			break;
		case IDM_VerticalShearing: // 图像竖直错切
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
 *   读无格式RAW图像文件																			*
 *																								*
 ************************************************************************************************/
BOOL ReadImage(LPSTR ImageFileName, char *oImage, int wImage, int hImage) // 读取图像信息并保存在Image[][]中
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR)
	{
		MessageBox(NULL, ImageFileName, "打开文件出错信息", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, oImage, wImage * hImage);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
 *																								*
 *   显示RAW图像																				 *
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
 *   从BMP图像中，读出图像头信息，主要包括图像长度和宽度											*
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
		MessageBox(NULL, ImageFileName, "打开文件出错信息", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, &bfImage, sizeof(BITMAPFILEHEADER));

	if ((bfImage.bfType != 0x4d42))
	{ // "BM"
		MessageBox(NULL, NULL, "打开bmp文件出错信息", MB_OK);
		return FALSE;
	}

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight = biImage.biHeight;
	if (biImage.biBitCount != 24)
	{ // 24位彩色图像
		MessageBox(NULL, NULL, "不是24位bmp图像", MB_OK);
		return FALSE;
	}

	_llseek(Image_fp, bfImage.bfOffBits, 0);
	_lread(Image_fp, oImage, biImage.biWidth * biImage.biHeight * 3);
	_lclose(Image_fp);

	return TRUE;
}

/************************************************************************************************
 *																								*
 *   显示BMP图像																					*
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
 *   打开读文件名系统对话框																		*
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
 *  显示图像部分(矩形)区域函数																	*
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
 *  显示BMP图像部分(矩形)区域函数																*
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
 *  由中间向左右(开门)显示函数																	*
 *																								*
 ************************************************************************************************/

void ShowLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // 清空窗口
	ShowRect.top = 0;
	ShowRect.bottom = hImage;
	for (i = 0; i < wImage / 2; i++)
	{
		ShowRect.left = wImage / 2 - i;
		ShowRect.right = ShowRect.left + 1;							// 显示一列
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // 左开门

		ShowRect.left = wImage / 2 + i;
		ShowRect.right = ShowRect.left + 1;							// 显示一列
		ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // 右开门
		Sleep(1);
	}
}

/************************************************************************************************
 *																								*
 *  由中间向左右(开门)显示Bmp函数																*
 *																								*
 ************************************************************************************************/

void ShowBmpLeftRightOpen(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // 清空窗口
	ShowRect.top = 0;
	ShowRect.bottom = hImage;
	for (i = 0; i < wImage / 2; i++)
	{
		ShowRect.left = wImage / 2 - i;
		ShowRect.right = ShowRect.left + 1;							   // 显示一列
		ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // 左开门

		ShowRect.left = wImage / 2 + i;
		ShowRect.right = ShowRect.left + 1;							   // 显示一列
		ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect); // 右开门
		Sleep(1);
	}
}
/************************************************************************************************
 *																								*
 *  由上向下百叶窗显示函数																		*
 *																								*
 ************************************************************************************************/

void ShowTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // 清空窗口
	ShowRect.left = 0;
	ShowRect.right = hImage;
	int SHUTTERNUM = 4;
	for (i = 0; i < wImage / SHUTTERNUM; i++)
	{ // SHUTTERNUM是百叶窗格数
		for (j = 0; j < SHUTTERNUM; j++)
		{
			ShowRect.top = i + (j * wImage / SHUTTERNUM);
			ShowRect.bottom = ShowRect.top + 1; // 显示一行
			ShowImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
			Sleep(1);
		}
	}
}
/************************************************************************************************
 *																								*
 *  由上向下百叶窗显示Bmp函数																	*
 *																								*
 ************************************************************************************************/
void ShowBmpTopBottomShutter(char *Image, int wImage, int hImage, int xPos, int yPos)
{
	int i, j;
	RECT ShowRect;
	InvalidateRgn(hWind, NULL, TRUE);
	UpdateWindow(hWind); // 清空窗口
	ShowRect.left = 0;
	ShowRect.right = wImage;					   // Bmp图像从右向左
	int SHUTTERNUM = 2;							   // SHUTTERNUM是百叶窗格数
	for (i = hImage / SHUTTERNUM - 1; i >= 0; i--) // 从下到上
	{
		for (j = 0; j < SHUTTERNUM; j++)
		{
			ShowRect.top = i + (j * hImage / SHUTTERNUM);
			ShowRect.bottom = ShowRect.top + 1; // 显示一行
			ShowBmpImageRect(Image, wImage, hImage, xPos, yPos, ShowRect);
			Sleep(1);
		}
	}
}
/************************************************************************************************
 *																								*
 *  淡出显示函数																				*
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
 *  淡出显示Bmp函数																				*
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
 *  显示直方图函数																				 *
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
 *  图像灰度反转函数																		      *
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
 *  图像二值化函数																				 *
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
 *  图像(kxk模板处理)函数															 		      *
 *																								*
 ************************************************************************************************/

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage,
						 int *Mask, int MaskWH, int MaskCoff)
{
	int Coff;		// 用于累加和
	int i, j, m, n; // 遍历元素
	int k;
	if (MaskWH % 2 != 0)
	{
		k = (MaskWH - 1) / 2;			 // 确立边界
		for (i = k; i < hImage - k; i++) // 遍历原图像
		{
			for (j = k; j < wImage - k; j++)
			{
				Coff = 0;
				// 遍历滤波器
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
	else // 偶数
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
 *  图像均值滤波函数																		    *
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
 *  图像高斯(平滑)滤波函数																		 *
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
 *  图像最大值滤波函数																		      *
 *																								*
 ************************************************************************************************/
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Coff;		// 用于找到最大值
	int i, j, m, n; // 遍历元素
	int k;
	k = 1; // 3×3
	// 遍历原图像
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			Coff = 0;
			// 遍历滤波器
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
 *  图像最小值滤波函数																		      *
 *																								*
 ************************************************************************************************/
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Coff;		// 用于找到最大值
	int i, j, m, n; // 遍历元素
	int k;
	k = 1; // 3×3
	// 遍历原图像
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			Coff = 256;
			// 遍历滤波器
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
 *  图像中值滤波函数																		      *
 *																								*
 ************************************************************************************************/
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage) // 3×3
{
	int k = 1;
	int Coff[3 * 3];
	int i, j, m, n; // 遍历元素
	// 遍历原图像
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			int cnt = 0;
			// 用于找到中值
			// 遍历滤波器
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
 *  图像中点滤波函数																		      *
 *																								*
 ************************************************************************************************/
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage) // 5×5
{
	int k = 2;
	int Coff[5 * 5];
	int i, j, m, n; // 遍历元素
	// 遍历原图像
	for (i = k; i < hImage - k; i++)
	{
		for (j = k; j < wImage - k; j++)
		{
			int cnt = 0;
			// 用于找到中值
			// 遍历滤波器
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
 *  随机添加椒盐噪声函数																		*
 *																								*
 ************************************************************************************************/
void AddSaltAndPepperNoise(char *Image, int wImage, int hImage, double noise_ratio, double salt_vs_pepper)
{
	int i, row, col;
	int total_pixels = wImage * hImage;
	int num_salt = (int)(total_pixels * noise_ratio * salt_vs_pepper); // 直接转为int
	int num_pepper = (int)(total_pixels * noise_ratio * (1.0 - salt_vs_pepper));

	// 添加盐噪声（白色）
	for (i = 0; i < num_salt; i++)
	{
		row = rand() % hImage;
		col = rand() % wImage;
		Image[row * wImage + col] = (BYTE)255;
	}

	// 添加椒噪声（黑色）
	for (i = 0; i < num_pepper; i++)
	{
		row = rand() % hImage;
		col = rand() % wImage;
		Image[row * wImage + col] = (BYTE)0;
	}
}

/************************************************************************************************
 *																								*
 *  随机添加高斯噪声函数																		*
 *																								*
 ************************************************************************************************/
void AddGaussianNoise(char *Image, int wImage, int hImage, double mean, double sigma)
{
	srand(time(NULL));

	for (int i = 0; i < wImage * hImage; i++)
	{
		// 生成高斯分布
		double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
		double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
		double Z0 = sqrt(-2 * log(U1)) * cos(2 * 3.141592653589793 * U2);

		// 正确读取像素值（无符号）
		unsigned char pixel = static_cast<unsigned char>(Image[i]);
		int noise = static_cast<int>(Z0 * sigma + mean);

		// 计算新像素值
		int new_val = pixel + noise;
		new_val = max(0, min(255, new_val));
		Image[i] = static_cast<char>(new_val);
	}
}

/************************************************************************************************
 *																								*
 *  Laplace算子    																		        *
 *																								*
 ************************************************************************************************/
void LaplaceEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int Mask[9] = {0, 1, 0, // Laplace边缘检测模板
				   1, -4, 1,
				   0, 1, 0};

	ImageMaskProcessing(oImage, nImage, wImage, hImage, Mask, 3, 1); // 不需要除权重和
}
/************************************************************************************************
 *																								*
 *  Sobel 算子    																		        *
 *																								*
 ************************************************************************************************/

void SobelEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	char *nImage1 = new char[wImage * hImage];
	int Mask1[9] = {-1, -2, -1, // 上下检测模板
					0, 0, 0,
					1, 2, 1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 3, 1);
	char *nImage2 = new char[wImage * hImage];
	int Mask2[9] = {-1, 0, 1, // 左右检测模板
					-2, 0, 2,
					-1, 0, 1};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 3, 1);
	// 绝对值相加
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
 *  Prewitt 算子    																		    *
 *																								*
 ************************************************************************************************/
void PrewittEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	char *nImage1 = new char[wImage * hImage];
	int Mask1[9] = {-1, -1, -1, // 上下检测模板
					0, 0, 0,
					1, 1, 1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 3, 1);
	char *nImage2 = new char[wImage * hImage];
	int Mask2[9] = {1, 0, -1, // 左右检测模板
					1, 0, -1,
					1, 0, -1};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 3, 1);
	// 绝对值相加
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
 *  Roberts 算子    																		    *
 *																								*
 ************************************************************************************************/
void RobertsEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	const int newWidth = wImage - 1;
	const int newHeight = hImage - 1;
	char *nImage1 = new char[newWidth * newHeight];
	int Mask1[4] = {1, 0, // 上下检测模板
					0, -1};
	ImageMaskProcessing(oImage, nImage1, wImage, hImage, Mask1, 2, 1); // 改为2×2
	char *nImage2 = new char[newWidth * newHeight];
	int Mask2[4] = {0, 1, // 上下检测模板
					-1, 0};
	ImageMaskProcessing(oImage, nImage2, wImage, hImage, Mask2, 2, 1);
	// 绝对值相加
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
 *  Kirsch 算子    																		    *
 *																								*
 ************************************************************************************************/
void KirschEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	// 定义 Kirsch 算子的 8 个 3×3 卷积核
	const int kirsch_kernels[8][9] = {
		// 0°
		{-3, -3, 5,
		 -3, 0, 5,
		 -3, -3, 5},
		// 45°
		{-3, 5, 5,
		 -3, 0, 5,
		 -3, -3, -3},
		// 90°
		{5, 5, 5,
		 -3, 0, -3,
		 -3, -3, -3},
		// 135°
		{5, 5, -3,
		 5, 0, -3,
		 -3, -3, -3},
		// 180°
		{5, -3, -3,
		 5, 0, -3,
		 5, -3, -3},
		// 225°
		{-3, -3, -3,
		 5, 0, -3,
		 5, 5, -3},
		// 270°
		{-3, -3, -3,
		 -3, 0, -3,
		 5, 5, 5},
		// 315°
		{-3, -3, -3,
		 -3, 0, 5,
		 -3, 5, 5}};

	char *maxEdges = new char[wImage * hImage];
	std::fill_n(maxEdges, wImage * hImage, 0); // 初始化为0

	// 遍历所有8个核
	for (int k = 0; k < 8; k++)
	{
		char *tempResult = new char[wImage * hImage];
		ImageMaskProcessing(oImage, tempResult, wImage, hImage, (int *)kirsch_kernels[k], 3, 1);

		// 更新最大值
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

	// 将最大值结果拷贝到输出图像
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
 *  Canny 算子    																		        *
 *																								*
 ************************************************************************************************/
// 非极大值抑制
// 假设 oImage 是 Sobel 梯度幅值（已计算），nImage 输出最终边缘
void CannyEdgeProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	// 中间变量：梯度方向（0~180度，量化到4个方向）
	unsigned char *dir = new unsigned char[wImage * hImage];
	// 非极大值抑制结果
	unsigned char *nms = new unsigned char[wImage * hImage];
	// 双阈值标记：255=强边缘，128=弱边缘，0=非边缘
	unsigned char *thresholded = new unsigned char[wImage * hImage];
	double M_PI = 3.1415926;
	int i, j;
	// 步骤1：非极大值抑制（NMS）
	for (i = 1; i < hImage - 1; ++i)
	{
		for (j = 1; j < wImage - 1; ++j)
		{
			const int idx = i * wImage + j;
			// 量化梯度方向到 0°, 45°, 90°, 135°
			const float gx = static_cast<signed char>(oImage[idx]); // 假设 oImage 存储 Gx
			const float gy = static_cast<signed char>(dir[idx]);	// 假设 dir 存储 Gy
			float angle = atan2(gy, gx) * 180.0f / M_PI;			// 转为角度
			if (angle < 0)
				angle += 180.0f;

			unsigned char q, r;
			// 根据方向选择比较的邻居
			if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
			{
				q = oImage[idx - wImage]; // 上方
				r = oImage[idx + wImage]; // 下方
			}
			else if (angle >= 22.5 && angle < 67.5)
			{
				q = oImage[idx - wImage + 1]; // 右上
				r = oImage[idx + wImage - 1]; // 左下
			}
			else if (angle >= 67.5 && angle < 112.5)
			{
				q = oImage[idx + 1]; // 右方
				r = oImage[idx - 1]; // 左方
			}
			else
			{
				q = oImage[idx - wImage - 1]; // 左上
				r = oImage[idx + wImage + 1]; // 右下
			}

			// 抑制非极大值
			const unsigned char curr = oImage[idx];
			nms[idx] = (curr >= q && curr >= r) ? curr : 0;
		}
	}

	// 步骤2：双阈值检测（假设高阈值=100，低阈值=50）
	const unsigned char high_thresh = 100;
	const unsigned char low_thresh = 50;
	for (i = 0; i < hImage * wImage; ++i)
	{
		if (nms[i] >= high_thresh)
		{
			thresholded[i] = 255; // 强边缘
		}
		else if (nms[i] >= low_thresh)
		{
			thresholded[i] = 128; // 弱边缘
		}
		else
		{
			thresholded[i] = 0; // 非边缘
		}
	}

	// 步骤3：滞后边缘连接
	for (i = 1; i < hImage - 1; ++i)
	{
		for (j = 1; j < wImage - 1; ++j)
		{
			const int idx = i * wImage + j;
			if (thresholded[idx] == 128)
			{ // 弱边缘
				// 检查8邻域是否存在强边缘
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

	// 输出最终边缘（强边缘 + 连接的弱边缘）
	for (i = 0; i < hImage * wImage; ++i)
	{
		nImage[i] = (thresholded[i] == 255) ? 255 : 0;
	}

	// 释放内存
	delete[] dir;
	delete[] nms;
	delete[] thresholded;
}
/************************************************************************************************
 *																								*
 *  简单几何变换函数    																		  *
 *																								*
 ************************************************************************************************/
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array)
{
	double axy, bxy;
	int i, sx, sy, ox, oy;
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0; // 清空新图像
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // 以图像中心为原点
			oy = sy - hImage / 2;
			axy = sgt_Array[0] * (double)ox + sgt_Array[1] * (double)oy + sgt_Array[2];
			bxy = sgt_Array[3] * (double)ox + sgt_Array[4] * (double)oy + sgt_Array[5];
			axy += wImage / 2; // 恢复图像原点位置
			bxy += hImage / 2;
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy * wImage + sx] = 0; // 超出范围部分置0
			else
				nImage[sy * wImage + sx] = oImage[((int)bxy) * wImage + (int)axy];
		}
	}
}

/************************************************************************************************
 *																								*
 *  平移变换    																		        *
 *																								*
 ************************************************************************************************/
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage,
					  int xPos, int yPos)
{
	double gt_Array[9] = {1, 0, 0,
						  0, 1, 0,
						  0, 0, 1};

	gt_Array[2] = xPos; // 水平方向平移距离
	gt_Array[5] = yPos; // 垂直方向平移距离

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

/************************************************************************************************
 *																								*
 *  缩放变换   																		             *
 *																								*
 ************************************************************************************************/
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage,
				  double xScaling, double yScaling)
{
	double gt_Array[9] = {0, 0, 0,
						  0, 0, 0,
						  0, 0, 1};

	gt_Array[0] = 1.0 / xScaling; // 水平方向放大倍数
	gt_Array[4] = 1.0 / yScaling; // 垂直方向放大倍数

	SimpleGeometricTranslation(oImage, nImage, wImage, hImage, gt_Array);
}

/************************************************************************************************
 *																								*
 *  旋转变换    																		        *
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
 *  镜像变换    																		        *
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
 *  错切    																		        *
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