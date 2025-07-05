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
#include <windows.h>

using namespace std;

#define MAX_LOADSTRING 100

BOOL GetImageDimensions(LPSTR ImageFileName, int *width, int *height);

BOOL ReadImage(LPSTR, char *, int, int); // 读取图像信息并保存在Image[][]中
BOOL ReadImage(const char *filename, char **oImage, int *width, int *height);
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

void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage);	// 显示直方图函数
void ImageGrayReversionProcessing(char *Image, int wImage, int hImage);					// 图像灰度反转函数
void ThresholdImage(char *oImage, char *nImage, int wImage, int hImage, int Threshold); // 图像位平面分割
void ThresholdImage2(char *oImage, int wImage, int hImage, int Threshold);				// 图像二值化函数
void ImageHistogramEqualization(char *oImage, char *nImage, int wImage, int hImage);	// 直方图均衡化
void GammaCorrection(char *oImage, char *nImage, int wImage, int hImage, double gamma); // 伽马校正

// 图像滤波

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage, int *Mask, int MaskWH, int MaskCoff);		  // 图像(3x3模板处理)函数
void AverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // 图像均值滤波函数
void GuassAverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);									  // 图像高斯(平滑)滤波函数
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // 最大值滤波
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // 最大值滤波
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage);										  // 中值滤波3×3
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage);										  // 中值滤波5×5
void BilateralFilterProcessing(char *oImage, char *nImage, int wImage, int hImage, double sigmaSpace, double sigmaColor); // 双边滤波
void ErosionProcessing(char *oImage, char *nImage, int wImage, int hImage);												  // 腐蚀操作
void DilationProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // 膨胀操作
void AdaptiveMedianFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);								  // 自适应中值滤波
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
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array);																   // 简单几何变换函数
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage, int xPos, int yPos);																		   // 平移变换
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage, double xScaling, double yScaling);															   // 缩放变换
void ImageRotation(char *oImage, char *nImage, int wImage, int hImage, double iAngle);																				   // 旋转变换
void ImageMirror(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal);																				   // 镜像变换
void ImageShearing(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal, int k);																		   // 错切变换
void SphereLensTransform(char *oImage, char *nImage, int wImage, int hImage, double centerX, double centerY, double radius, double strength);						   // 球面透镜
unsigned char BilinearInterpolation(char *image, int w, int h, double x, double y);																					   // 双线性插值辅助函数
void PerspectiveTransformation(char *oImage, char *nImage, int wImage, int hImage, double *pt_Array);																   // 透视变换
bool InversePerspectiveTransform(double u, double v, double *x, double *y, double *matrix);																			   // 透视校正
void RadialTransformation(char *oImage, char *nImage, int wImage, int hImage, double *rt_Array);																	   // 径向变换函数
void ImageBarrelDistortion(char *oImage, char *nImage, int wImage, int hImage, double k1, double k2);																   // 桶形/枕形畸变
void ImageDistortionCorrection(char *oImage, char *nImage, int wImage, int hImage, double k1, double k2);															   // 径向畸变校正
double SolveUndistortedRadius(double distortedRadius, double k1, double k2);																						   // 反向求解无畸变半径																																									   // 双线性插值
void SwirlTransformation(char *oImage, char *nImage, int wImage, int hImage, double strength, double radius);														   // 漩涡变换函数
void ImageSwirlTransformation(char *oImage, char *nImage, int wImage, int hImage, double strength, double radius);													   // 漩涡变换接口函数
void ImageWaveTransformation(char *oImage, char *nImage, int wImage, int hImage, int waveType, double amplitude, double frequency);									   // 波浪变换
void WaveTransformation(char *oImage, char *nImage, int wImage, int hImage, double amplitudeX, double frequencyX, double amplitudeY, double frequencyY, int waveType); // 波浪变换函数
void CalculatePerspectiveMatrix(double srcPoints[8], double dstPoints[8], double matrix[8]);																		   // 计算透视变换矩阵
void ImagePerspectiveCorrection(char *oImage, char *nImage, int wImage, int hImage, int correctionType);															   // 图像透视校正
void SolveLinearSystem(double A[8][8], double b[8], double x[8], int n);																							   // 求解线性方程组
void PerspectiveTransformationImproved(char *oImage, char *nImage, int wImage, int hImage, double *matrix);															   // 改进的透视变换
// 创新功能1:
// 频域处理函数声明
void IdealLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);							  // 理想低通滤波器
void ButterworthLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);					  // 巴特沃斯低通滤波器
void GaussianLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);						  // 高斯低通滤波器
void FFT1D(double *realPart, double *imagPart, int n, bool inverse);													  // 一维快速傅里叶变换
void FFT2D(double *realPart, double *imagPart, int width, int height, bool inverse);									  // 二维快速傅里叶变换
void FrequencyDomainSmoothing(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType);	  // 频域平滑滤波
void ImageToComplex(char *image, double *realPart, double *imagPart, int width, int height);							  // 图像转复数形式
void ComplexToImage(double *realPart, double *imagPart, char *image, int width, int height);							  // 复数转图像形式
int ReverseBits(int num, int bits);																						  // 位反转函数
void ShowFrequencySpectrum(double *realPart, double *imagPart, int width, int height, int xPos, int yPos);				  // 显示频谱图
void FrequencyDomainEdgeDetection(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType); // 频域边缘检测
void IdealHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);					  // 理想高通滤波器
void GaussianHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);				  // 高斯高通滤波器
void ButterworthHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);			  // 巴特沃斯高通滤波器

// 创新功能2:质量评估
double CalculatePSNR(char *original, char *processed, int width, int height);				 // 计算峰值信噪比
double CalculateMSE(char *original, char *processed, int width, int height);				 // 计算均方误差
double CalculateSSIM(char *original, char *processed, int width, int height);				 // 计算结构相似性指数
double CalculateSharpness(char *image, int width, int height);								 // 计算图像清晰度
double CalculateContrast(char *image, int width, int height);								 // 计算图像对比度
void ComprehensiveQualityAssessment(char *original, char *processed, int width, int height); // 综合质量评估

// 创新功能3：图像分割
void AdaptiveThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage, int blockSize);					 // 自适应阈值分割
void RegionGrowingSegmentation(char *oImage, char *nImage, int wImage, int hImage, int seedX, int seedY, int threshold); // 区域生长分割
void BoundaryTracing(char *oImage, char *nImage, int wImage, int hImage, int threshold);								 // 边界跟踪
void OtsuThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage);										 // Otsu阈值分割
void WatershedSegmentation(char *oImage, char *nImage, int wImage, int hImage);											 // 分水岭分割
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
		case IDM_SHOWRAWIMAGE:
			OpenImageFileDlg("打开图像文件");
			// ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			// ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			ReadBmpImage(ImgDlgFileName, OrgImage);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_HISTOGRAMSHOW: // 显示直方图
			ImageHistogramShowProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_THRESHOLD: // 图像二值化(位平面7)
			ThresholdImage2(OrgImage, ImageWidth, ImageHeight, 128);
			// ShowImage(OrgImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 900);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE1: // 位平面0
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_DIVIEDPLANE2: // 位平面1
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE3: // 位平面2
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_DIVIEDPLANE4: // 位平面3
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 8);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_DIVIEDPLANE5: // 位平面4
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 16);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE6: // 位平面5
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 32);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			break;
		case IDM_DIVIEDPLANE7: // 位平面6
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 64);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 300);
			break;
		case IDM_BALANCED: // 图像均衡化
			ImageHistogramEqualization(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			ImageHistogramShowProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_GRAYREVERSION: // 图像灰度反转
			ImageGrayReversionProcessing(OrgImage, ImageWidth, ImageHeight);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case GammaCorrection2: // 伽马校正2
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case GammaCorrection4: // 伽马校正4
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case GammaCorrection0_5: // 伽马校正0.5
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case GammaCorrection0_2: // 伽马校正0.2
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_GRAYTOLOW: // 淡入
			ShowGrayToLow(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_LEFTRIGHTOPEN: // 向左右开门
			ShowLeftRightOpen(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_TBSHUTTER: // 由上向下百叶窗
			ShowTopBottomShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
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
			AverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_AVERAGEGAUSS: // 高斯滤波器
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MAXFILTER: // 最大值滤波
			MaxFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MINFILTER: // 最大值滤波
			MinFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER3: // 中值滤波3*3
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER5: // 中值滤波5*5
			MidvalueFilterProcessing5(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
			// 随机噪声
		case IDM_SaltPeper: // 椒盐噪声
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0.5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Salt: // 盐噪声
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 1);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Pepper: // 椒噪声
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Gauss: // 高斯噪声
			AddGaussianNoise(OrgImage, ImageWidth, ImageHeight, 0, 5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
			// 边缘检测
		case IDM_LAPLACEFILTER: // Laplace边缘检测滤波
			LaplaceEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_LAPLACEFILTER2: // Laplace+高斯滤波边缘检测滤波
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			LaplaceEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_SobelFILTER: // Sobel边缘检测滤波
			SobelEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PrewittFILTER: // Prewitt边缘检测滤波
			PrewittEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 900);
			break;
		case IDM_RobertsFILTER: // Roberts边缘检测滤波
			RobertsEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, IMAGEWIDTH - 1, IMAGEHEIGHT - 1, XPOS, YPOS + 1200);
			break;
		case IDM_KirschFILTER: // Kirsch边缘检测滤波
			KirschEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;

		case IDM_CannyFILTER: // Canny边缘检测滤波
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			SobelEdgeProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			CannyEdgeProcessing(NewImage2, NewImage3, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage3, ImageWidth, ImageHeight, XPOS, YPOS + 1500);
			break;
		case IDM_BilateralFilter: // 双边滤波
			BilateralFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight, 5.0, 25.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ErosionProcessing1: // 形态学滤波--腐蚀
			ErosionProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ErosionProcessing2: // 形态学滤波--膨胀
			DilationProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_OpenProcessing: // 形态学滤波--开运算 腐蚀 + 膨胀
			ErosionProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			DilationProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_CloseProcessing: // 形态学滤波--闭运算 膨胀 + 腐蚀
			DilationProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ErosionProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_AdaptiveMedianFilter: // 自适应中值滤波
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			// 高斯噪声
			AddGaussianNoise(OrgImage, ImageWidth, ImageHeight, 0, 5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 600);
			// 展示椒盐噪音
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0.5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;

		case IDM_TRANSLATION: // 图像平移
			ImageTranslation(OrgImage, NewImage, ImageWidth, ImageHeight, 20, 30);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_SCALING: // 图像缩放
			ImageScaling(OrgImage, NewImage, ImageWidth, ImageHeight, 2.0, 0.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ROTATION: // 图像旋转
			ImageRotation(OrgImage, NewImage, ImageWidth, ImageHeight, 45.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalMirror: // 图像水平翻转
			ImageMirror(OrgImage, NewImage, ImageWidth, ImageHeight, true);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_VerticalMirror: // 图像竖直翻转
			ImageMirror(OrgImage, NewImage, ImageWidth, ImageHeight, false);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalShearing: // 图像水平错切
			ImageShearing(OrgImage, NewImage, ImageWidth, ImageHeight, true, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_VerticalShearing: // 图像竖直错切
			ImageShearing(OrgImage, NewImage, ImageWidth, ImageHeight, false, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PerspectiveCorrection1: // 图像透视变换
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_PerspectiveCorrection2: // 图像透视变换
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PerspectiveCorrection3: // 图像透视变换
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 3);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 900);
			break;
		case IDM_PerspectiveCorrection4: // 图像透视变换
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_BarrelDistortion1: // 图像桶形畸变（正值）
			ImageBarrelDistortion(OrgImage, NewImage, ImageWidth, ImageHeight, 0.5, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_BarrelDistortion2: // 图像枕形畸变（负值）
			ImageBarrelDistortion(OrgImage, NewImage, ImageWidth, ImageHeight, -0.4, -0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_ImageDistortionCorrection2: // 校正畸变
			ImageDistortionCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.4, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_SwirlTransformation: // 漩涡变换
			ImageSwirlTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 2.0, IMAGEWIDTH);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_WaveTransformation1: // 水平波浪变换
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 1, 20.0, 3.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_WaveTransformation2: // 双向波浪变换
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 3, 15.0, 2.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_WaveTransformation3: // 径向波浪变换
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 4, 25.0, 1.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;

		case IDM_IDEAL_FILTER: // 理想低通滤波
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_GAUSSIAN_FILTER: // 高斯低通滤波
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_BUTTERWORTH_FILTER: // 巴特沃斯低通滤波
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_IDEAL_HIGH_FILTER: // 理想高通滤波（边缘检测）
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_GAUSSIAN_HIGH_FILTER: // 高斯高通滤波（边缘检测）
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;
		case IDM_BUTTERWORTH_HIGH_FILTER: // 巴特沃斯高通滤波（边缘检测）
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			break;
		case IDM_FREQ_SPECTRUM: // 显示频谱
		{
			double *realPart = new double[IMAGEWIDTH * IMAGEHEIGHT];
			double *imagPart = new double[IMAGEWIDTH * IMAGEHEIGHT];
			// 将图像数据转换为double类型
			for (int i = 0; i < IMAGEWIDTH * IMAGEHEIGHT; i++)
			{
				realPart[i] = (double)NewImage[i];
				imagPart[i] = 0.0;
			}
			FFT2D(realPart, imagPart, ImageWidth, ImageHeight, false);
			ShowFrequencySpectrum(realPart, imagPart, ImageWidth, ImageHeight, XPOS + 600, YPOS + 300);
			delete[] realPart;
			delete[] imagPart;
		}
		break;
		case IDM_QUALITY_ASSESSMENT: // 综合质量评估
			if (OrgImage != NULL && NewImage != NULL)
			{
				ComprehensiveQualityAssessment(OrgImage, NewImage, ImageWidth, ImageHeight);
			}
			else
			{
				MessageBox(hWnd, "请先加载原始图像和处理后图像", "提示", MB_OK);
			}
			break;
		case IDM_ADAPTIVE_THRESHOLD: // 自适应阈值分割
			AdaptiveThresholdSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight, 15);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_OTSU_THRESHOLD: // Otsu自动阈值
			OtsuThresholdSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;

		case IDM_REGION_GROWING: // 区域生长分割
			// 使用图像中心作为种子点，阈值为30
			RegionGrowingSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight,
									  ImageWidth / 2, ImageHeight / 2, 30);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_BOUNDARY_TRACING: // 边界追踪
			BoundaryTracing(OrgImage, NewImage, ImageWidth, ImageHeight, 128);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_WATERSHED: // 分水岭分割
			WatershedSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
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
 *   显示RAW图像																					*
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

	// if ((bfImage.bfType != 0x4d42))
	// { // "BM"
	// 	MessageBox(NULL, NULL, "打开bmp文件出错信息", MB_OK);
	// 	return FALSE;
	// }

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight = biImage.biHeight;
	// if (biImage.biBitCount != 24)
	// { // 24位彩色图像
	// 	MessageBox(NULL, NULL, "不是24位bmp图像", MB_OK);
	// 	return FALSE;
	// }

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
			// SetPixel(hWinDC, j + yPos, line + xPos,
			// 		 RGB(Image[(i * wImage + j) * 3 + 2],
			// 			 Image[(i * wImage + j) * 3 + 1],
			// 			 Image[(i * wImage + j) * 3]));
			SetPixel(hWinDC, j + yPos, line + xPos, RGB(Image[(i * wImage + j)], Image[(i * wImage + j)], Image[(i * wImage + j)]));
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
	ofn.lpstrFilter = TEXT("All File\0*.*\0\0");
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
 *  直方图均衡化																				 *
 *																								*
 ************************************************************************************************/

void ImageHistogramEqualization(char *oImage, char *nImage, int wImage, int hImage)
{
	const int L = 256;
	int i;
	// 1. 计算原始图像的灰度直方图
	int GrayCount[L] = {0}; // 初始化直方图数组
	int imageSize = wImage * hImage;

	for (i = 0; i < imageSize; i++)
	{
		// 使用无符号字符来正确处理0-255的灰度值
		unsigned char gray = (unsigned char)oImage[i];
		if (gray >= L)
			gray = L - 1; // 安全边界检查
		GrayCount[gray]++;
	}

	// 2. 计算累积分布函数 (hs->hp)
	double hs[L] = {0};
	for (i = 0; i < L; i++)
	{
		hs[i] = (double)GrayCount[i] / imageSize; // 使用总像素数进行归一化
	}

	// 3. 计算累积分布函数hp
	double hp[L] = {0};
	hp[0] = hs[0];
	for (i = 1; i < L; i++)
	{
		hp[i] = hp[i - 1] + hs[i]; // 累积分布
	}
	// 4. 映射像素到新灰度值 gx = 255 * hp[f(x)]
	for (i = 0; i < imageSize; i++)
	{
		unsigned char orig = (unsigned char)oImage[i];
		if (orig >= L)
			orig = L - 1; // 安全边界检查

		// 应用映射公式
		double newValue = (L - 1) * hp[orig];

		// 确保结果在0-255范围内
		if (newValue < 0)
			newValue = 0;
		if (newValue > L - 1)
			newValue = L - 1;

		// 转换为字符（需要显式转换为unsigned char避免负值）
		nImage[i] = (char)((unsigned char)(newValue + 0.5)); // 四舍五入
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
 *  图像幂次函数																		      *
 *																								*
 ************************************************************************************************/
void GammaCorrection(char *oImage, char *nImage, int wImage, int hImage, double gamma)
{
	int imageSize = wImage * hImage;
	double c = 1.0;
	for (int i = 0; i < imageSize; i++)
	{
		// 将输入灰度值归一化到 [0, 1] 范围
		double normalized = (unsigned char)oImage[i] / 255.0;

		// 应用幂次变换：s = c * (r)^gamma
		double result = c * pow(normalized, gamma);

		// 确保结果在 [0,1] 范围内
		if (result > 1.0)
			result = 1.0;
		if (result < 0.0)
			result = 0.0;

		// 将结果映射回 [0,255] 并四舍五入
		nImage[i] = (unsigned char)(result * 255.0 + 0.5);
	}
}
/************************************************************************************************
 *																								*
 *  图像位平面分割函数																				 *
 *																								*
 ************************************************************************************************/
void ThresholdImage(char *oImage, char *nImage, int wImage, int hImage, int Threshold)
{
	int i;
	for (i = 0; i < wImage * hImage; i++)
	{
		int result = oImage[i] & Threshold;
		if (result != 0)
			nImage[i] = (BYTE)255;
		else
			nImage[i] = 0;
	}
}
void ThresholdImage2(char *oImage, int wImage, int hImage, int Threshold)
{
	int i;
	for (i = 0; i < wImage * hImage; i++)
	{
		int result = oImage[i] & Threshold;
		if (result != 0)
			oImage[i] = (BYTE)255;
		else
			oImage[i] = 0;
	}
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

/************************************************************************************************
 *																								*
 *  双线性插值辅助函数    																		  *
 *																								*
 ************************************************************************************************/
unsigned char BilinearInterpolation(char *image, int w, int h, double x, double y)
{
	int x1 = (int)floor(x);
	int y1 = (int)floor(y);
	int x2 = x1 + 1;
	int y2 = y1 + 1;

	// 边界检查
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 >= w)
		x2 = w - 1;
	if (y2 >= h)
		y2 = h - 1;

	// 插值权重
	double a = x - x1;
	double b = y - y1;

	// 获取四个邻点的像素值
	unsigned char p11 = image[y1 * w + x1];
	unsigned char p12 = image[y1 * w + x2];
	unsigned char p21 = image[y2 * w + x1];
	unsigned char p22 = image[y2 * w + x2];

	// 计算插值结果
	double val = (1 - a) * (1 - b) * p11 +
				 a * (1 - b) * p12 +
				 (1 - a) * b * p21 +
				 a * b * p22;

	return (unsigned char)(val + 0.5); // 四舍五入
}
/************************************************************************************************
 *																								*
 *  透视变换函数    																			  *
 *																								*
 ************************************************************************************************/
void PerspectiveTransformation(char *oImage, char *nImage, int wImage, int hImage, double *pt_Array)
{
	double axy, bxy, wxy;
	int i, sx, sy, ox, oy;
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0; // 清空新图像
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // 以图像中心为原点
			oy = sy - hImage / 2;
			// 透视变换公式: x' = (ax+by+c)/(gx+hy+1), y' = (dx+ey+f)/(gx+hy+1)
			wxy = pt_Array[6] * (double)ox + pt_Array[7] * (double)oy + 1.0;
			if (wxy != 0.0)
			{
				axy = (pt_Array[0] * (double)ox + pt_Array[1] * (double)oy + pt_Array[2]) / wxy;
				bxy = (pt_Array[3] * (double)ox + pt_Array[4] * (double)oy + pt_Array[5]) / wxy;
			}
			else
			{
				axy = 0.0;
				bxy = 0.0;
			}
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
 *  四点透视校正（更实用的梯形校正）    														    *
 *																								*
 ************************************************************************************************/
// 计算透视变换矩阵（8元线性方程组求解）
void CalculatePerspectiveMatrix(double srcPoints[8], double dstPoints[8], double matrix[8])
{
	// 构建线性方程组 Ax = b
	// 对于透视变换: x' = (ax+by+c)/(gx+hy+1), y' = (dx+ey+f)/(gx+hy+1)
	// 重新整理得到线性方程组

	double A[8][8];
	double b[8];

	// 为4个点对建立8个方程
	for (int i = 0; i < 4; i++)
	{
		double x = srcPoints[i * 2];	 // 源点x坐标
		double y = srcPoints[i * 2 + 1]; // 源点y坐标
		double u = dstPoints[i * 2];	 // 目标点x坐标
		double v = dstPoints[i * 2 + 1]; // 目标点y坐标

		// 第一个方程: u*(gx+hy+1) = ax+by+c
		// 整理为: ax + by + c + 0*d + 0*e + 0*f - u*g*x - u*h*y = u
		A[i * 2][0] = x;	  // a的系数
		A[i * 2][1] = y;	  // b的系数
		A[i * 2][2] = 1;	  // c的系数
		A[i * 2][3] = 0;	  // d的系数
		A[i * 2][4] = 0;	  // e的系数
		A[i * 2][5] = 0;	  // f的系数
		A[i * 2][6] = -u * x; // g的系数
		A[i * 2][7] = -u * y; // h的系数
		b[i * 2] = u;

		// 第二个方程: v*(gx+hy+1) = dx+ey+f
		// 整理为: 0*a + 0*b + 0*c + dx + ey + f - v*g*x - v*h*y = v
		A[i * 2 + 1][0] = 0;	  // a的系数
		A[i * 2 + 1][1] = 0;	  // b的系数
		A[i * 2 + 1][2] = 0;	  // c的系数
		A[i * 2 + 1][3] = x;	  // d的系数
		A[i * 2 + 1][4] = y;	  // e的系数
		A[i * 2 + 1][5] = 1;	  // f的系数
		A[i * 2 + 1][6] = -v * x; // g的系数
		A[i * 2 + 1][7] = -v * y; // h的系数
		b[i * 2 + 1] = v;
	}

	// 使用高斯消元法求解线性方程组
	SolveLinearSystem(A, b, matrix, 8);
}

// 高斯消元法求解线性方程组
void SolveLinearSystem(double A[8][8], double b[8], double x[8], int n)
{
	int i, k;
	// 前向消元
	for (i = 0; i < n; i++)
	{
		// 寻找主元
		int maxRow = i;
		for (k = i + 1; k < n; k++)
		{
			if (fabs(A[k][i]) > fabs(A[maxRow][i]))
			{
				maxRow = k;
			}
		}

		// 交换行
		if (maxRow != i)
		{
			for (k = 0; k < n; k++)
			{
				double temp = A[i][k];
				A[i][k] = A[maxRow][k];
				A[maxRow][k] = temp;
			}
			double temp = b[i];
			b[i] = b[maxRow];
			b[maxRow] = temp;
		}

		// 消元
		for (k = i + 1; k < n; k++)
		{
			if (fabs(A[i][i]) > 1e-10)
			{ // 避免除零
				double factor = A[k][i] / A[i][i];
				for (int j = i; j < n; j++)
				{
					A[k][j] -= factor * A[i][j];
				}
				b[k] -= factor * b[i];
			}
		}
	}

	// 回代求解
	for (i = n - 1; i >= 0; i--)
	{
		x[i] = b[i];
		for (int j = i + 1; j < n; j++)
		{
			x[i] -= A[i][j] * x[j];
		}
		if (fabs(A[i][i]) > 1e-10)
		{
			x[i] /= A[i][i];
		}
	}
}
void FourPointPerspectiveCorrection(char *oImage, char *nImage, int wImage, int hImage,
									double topLeftX, double topLeftY,
									double topRightX, double topRightY,
									double bottomLeftX, double bottomLeftY,
									double bottomRightX, double bottomRightY)
{
	// 源图像的四个角点（梯形）
	double srcPoints[8] = {
		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomLeftX, bottomLeftY,
		bottomRightX, bottomRightY};

	// 目标图像的四个角点（标准矩形）
	double dstPoints[8] = {
		0, 0,				   // 左上角
		wImage - 1, 0,		   // 右上角
		0, hImage - 1,		   // 左下角
		wImage - 1, hImage - 1 // 右下角
	};

	// 计算透视变换矩阵
	double matrix[8];
	CalculatePerspectiveMatrix(srcPoints, dstPoints, matrix);

	// 执行透视变换
	PerspectiveTransformationImproved(oImage, nImage, wImage, hImage, matrix);
}
void PerspectiveTransformationImproved(char *oImage, char *nImage, int wImage, int hImage, double *matrix)
{
	int i;
	// 清空目标图像
	for (i = 0; i < wImage * hImage; i++)
	{
		nImage[i] = 0;
	}

	// 透视变换参数
	double a = matrix[0], b = matrix[1], c = matrix[2];
	double d = matrix[3], e = matrix[4], f = matrix[5];
	double g = matrix[6], h = matrix[7];

	// 对目标图像的每个像素进行反向映射
	for (int v = 0; v < hImage; v++)
	{
		for (int u = 0; u < wImage; u++)
		{
			// 反向透视变换：从目标坐标(u,v)计算源坐标(x,y)
			// 解方程组：
			// u = (ax + by + c) / (gx + hy + 1)
			// v = (dx + ey + f) / (gx + hy + 1)

			// 使用迭代法或直接求解
			double x, y;
			if (InversePerspectiveTransform(u, v, &x, &y, matrix))
			{
				// 检查源坐标是否在有效范围内
				if (x >= 0 && x < wImage - 1 && y >= 0 && y < hImage - 1)
				{
					// 使用双线性插值获取像素值
					unsigned char pixelValue = BilinearInterpolation(oImage, wImage, hImage, x, y);
					nImage[v * wImage + u] = pixelValue;
				}
			}
		}
	}
}

// 反向透视变换求解
bool InversePerspectiveTransform(double u, double v, double *x, double *y, double *matrix)
{
	double a = matrix[0], b = matrix[1], c = matrix[2];
	double d = matrix[3], e = matrix[4], f = matrix[5];
	double g = matrix[6], h = matrix[7];

	// 解方程组（使用克拉默法则）：
	// u*(gx + hy + 1) = ax + by + c
	// v*(gx + hy + 1) = dx + ey + f
	//
	// 整理得：
	// (a - ug)x + (b - uh)y = u - c
	// (d - vg)x + (e - vh)y = v - f

	double A11 = a - u * g;
	double A12 = b - u * h;
	double A21 = d - v * g;
	double A22 = e - v * h;

	double B1 = u - c;
	double B2 = v - f;

	double det = A11 * A22 - A12 * A21;

	if (fabs(det) < 1e-10)
	{
		return false; // 矩阵奇异，无解
	}

	*x = (B1 * A22 - B2 * A12) / det;
	*y = (A11 * B2 - A21 * B1) / det;

	return true;
}
void ImagePerspectiveCorrection(char *oImage, char *nImage, int wImage, int hImage,
								int correctionType)
{
	double w = wImage - 1;
	double h = hImage - 1;

	switch (correctionType)
	{
	case 1: // 上窄下宽的梯形校正
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   w * 0.2, 0, w * 0.8, 0,
									   0, h, w, h);
		break;
	case 2: // 上宽下窄的梯形校正
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, 0, w, 0,
									   w * 0.2, h, w * 0.8, h);
		break;
	case 3: // 左窄右宽的梯形校正
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, h * 0.2, w, 0,
									   0, h * 0.8, w, h);
		break;
	case 4: // 左宽右窄的梯形校正
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, 0, w, h * 0.2,
									   0, h, w, h * 0.8);
		break;
	}
}
/************************************************************************************************
 *																														*
 *  径向变换函数（非线性变换）    																	  *
 *																														*
 ************************************************************************************************/
void RadialTransformation(char *oImage, char *nImage, int wImage, int hImage, double *rt_Array)
{
	double axy, bxy, radius, newRadius, angle, normalizedRadius;
	int i, sx, sy, ox, oy;
	double centerX = wImage / 2.0;
	double centerY = hImage / 2.0;
	double maxRadius = sqrt(centerX * centerX + centerY * centerY);

	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0; // 清空新图像
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // 以图像中心为原点
			oy = sy - hImage / 2;
			// 计算极坐标
			radius = sqrt((double)(ox * ox + oy * oy));
			if (radius > 0.0)
			{
				angle = atan2((double)oy, (double)ox);
				// 归一化半径
				normalizedRadius = radius / maxRadius;
				// 改进的径向变换公式，效果更明显
				newRadius = radius * (1.0 + rt_Array[0] * normalizedRadius + rt_Array[1] * normalizedRadius * normalizedRadius);
				// 限制变换范围，避免过度变形
				if (newRadius > maxRadius * 2.0)
					newRadius = maxRadius * 2.0;
				if (newRadius < 0.0)
					newRadius = 0.0;
				// 转换回直角坐标
				axy = newRadius * cos(angle) + centerX;
				bxy = newRadius * sin(angle) + centerY;
			}
			else
			{
				axy = centerX;
				bxy = centerY;
			}
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy * wImage + sx] = 0; // 超出范围部分置0
			else
				nImage[sy * wImage + sx] = oImage[((int)bxy) * wImage + (int)axy];
		}
	}
}

/************************************************************************************************
 *																								*
 *  桶形/枕形畸变    																			  *
 *																								*
 ************************************************************************************************/
void ImageBarrelDistortion(char *oImage, char *nImage, int wImage, int hImage,
						   double k1, double k2)
{
	double rt_Array[2];
	rt_Array[0] = k1; // 一次项系数
	rt_Array[1] = k2; // 二次项系数

	RadialTransformation(oImage, nImage, wImage, hImage, rt_Array);
}
/************************************************************************************************
 *                                                                                              *
 *  反向求解无畸变半径（牛顿迭代法）                                                            *
 *                                                                                              *
 ************************************************************************************************/
double SolveUndistortedRadius(double distortedRadius, double k1, double k2)
{
	if (distortedRadius == 0.0)
		return 0.0;

	double r = distortedRadius; // 初始估计值

	// 牛顿迭代法求解：r_d = r_u * (1 + k1*r_u_norm + k2*r_u_norm^2)
	// 其中 r_u_norm = r_u / maxRadius，但这里我们直接用归一化的distortedRadius
	for (int i = 0; i < 10; i++) // 最多迭代10次
	{
		// 注意：这里的r已经是归一化的半径
		double r2 = r * r;

		// f(r) = r * (1 + k1*r + k2*r^2) - distortedRadius
		double f = r * (1.0 + k1 * r + k2 * r2) - distortedRadius;

		// f'(r) = 1 + 2*k1*r + 3*k2*r^2
		double df = 1.0 + 2.0 * k1 * r + 3.0 * k2 * r2;

		if (fabs(df) < 1e-10) // 避免除零
			break;

		double newR = r - f / df;

		// 收敛检查
		if (fabs(newR - r) < 1e-8)
			break;

		r = newR;

		// 防止发散
		if (r < 0)
			r = distortedRadius * 0.5;
	}

	return r;
}

/************************************************************************************************
 *                                                                                              *
 *  径向畸变校正函数                                                                            *
 *                                                                                              *
 ************************************************************************************************/
void ImageDistortionCorrection(char *oImage, char *nImage, int w, int h, double k1, double k2)
{
	char *tempImage = new char[w * h];
	memcpy(tempImage, oImage, w * h);

	double centerX = w / 2.0;
	double centerY = h / 2.0;
	double maxRadius = sqrt(centerX * centerX + centerY * centerY);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// 计算相对于中心的坐标
			double dx = x - centerX;
			double dy = y - centerY;
			double distortedRadius = sqrt(dx * dx + dy * dy);

			if (distortedRadius > 0)
			{
				// 归一化半径
				double normalizedRadius = distortedRadius / maxRadius;

				// 求解无畸变半径
				double undistortedRadius = SolveUndistortedRadius(normalizedRadius, k1, k2) * maxRadius;

				// 计算源图像坐标
				double scale = undistortedRadius / distortedRadius;
				double srcX = centerX + dx * scale;
				double srcY = centerY + dy * scale;

				// 边界检查
				if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h)
				{
					// 使用现有的双线性插值函数
					unsigned char pixel = BilinearInterpolation(tempImage, w, h, srcX, srcY);
					nImage[y * w + x] = pixel;
				}
				else
				{
					nImage[y * w + x] = 0; // 边界外设为黑色
				}
			}
			else
			{
				nImage[y * w + x] = tempImage[y * w + x]; // 中心点保持不变
			}
		}
	}

	delete[] tempImage;
}
/************************************************************************************************
 *																								*
 *  漩涡变换    																		     *
 *																								*
 ************************************************************************************************/
void SwirlTransformation(char *oImage, char *nImage, int wImage, int hImage,
						 double strength, double radius)
{
	double centerX = wImage / 2.0;
	double centerY = hImage / 2.0;
	int i;

	// 清空目标图像
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0;

	for (int sy = 0; sy < hImage; sy++)
	{
		for (int sx = 0; sx < wImage; sx++)
		{
			// 计算相对于中心的坐标
			double dx = sx - centerX;
			double dy = sy - centerY;
			double r = sqrt(dx * dx + dy * dy);

			if (r < radius && r > 0)
			{
				// 计算原始角度
				double angle = atan2(dy, dx);

				// 计算漩涡角度偏移（使用正弦衰减函数）
				double factor = sin(3.1415926 * r / (2.0 * radius));
				double swirlAngle = strength * factor;

				// 应用漩涡变换
				double newAngle = angle + swirlAngle;

				// 转换回直角坐标
				double srcX = centerX + r * cos(newAngle);
				double srcY = centerY + r * sin(newAngle);

				// 边界检查和像素映射
				if (srcX >= 0 && srcX < wImage - 1 && srcY >= 0 && srcY < hImage - 1)
				{
					// 使用双线性插值
					int x1 = (int)srcX;
					int y1 = (int)srcY;
					int x2 = x1 + 1;
					int y2 = y1 + 1;

					double fx = srcX - x1;
					double fy = srcY - y1;

					// 获取四个邻近像素
					unsigned char p11 = oImage[y1 * wImage + x1];
					unsigned char p12 = oImage[y2 * wImage + x1];
					unsigned char p21 = oImage[y1 * wImage + x2];
					unsigned char p22 = oImage[y2 * wImage + x2];

					// 双线性插值计算
					double result = p11 * (1 - fx) * (1 - fy) +
									p21 * fx * (1 - fy) +
									p12 * (1 - fx) * fy +
									p22 * fx * fy;

					nImage[sy * wImage + sx] = (unsigned char)result;
				}
				else
				{
					nImage[sy * wImage + sx] = 0;
				}
			}
			else
			{
				// 超出漩涡半径，直接复制原像素
				nImage[sy * wImage + sx] = oImage[sy * wImage + sx];
			}
		}
	}
}

/************************************************************************************************
 *                                                                                              *
 *  漩涡变换接口函数                                                                            *
 *                                                                                              *
 ************************************************************************************************/
void ImageSwirlTransformation(char *oImage, char *nImage, int wImage, int hImage,
							  double strength, double radius)
{
	// 参数验证
	if (strength < -10.0)
		strength = -10.0;
	if (strength > 10.0)
		strength = 10.0;
	if (radius <= 0)
		radius = min(wImage, hImage) / 4.0;
	if (radius > min(wImage, hImage) / 2.0)
		radius = min(wImage, hImage) / 2.0;

	SwirlTransformation(oImage, nImage, wImage, hImage, strength, radius);
}
/************************************************************************************************
 *                                                                                              *
 *  波浪变换函数（Wave Transformation）                                                         *
 *                                                                                              *
 ************************************************************************************************/
void WaveTransformation(char *oImage, char *nImage, int wImage, int hImage,
						double amplitudeX, double frequencyX,
						double amplitudeY, double frequencyY,
						int waveType)
{
	int i;

	// 清空目标图像
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0;
	double M_PI = 3.1415926;
	for (int sy = 0; sy < hImage; sy++)
	{
		for (int sx = 0; sx < wImage; sx++)
		{
			double srcX = sx;
			double srcY = sy;

			// 根据波浪类型应用不同的变换
			switch (waveType)
			{
			case 1: // 水平波浪
				srcX = sx + amplitudeX * sin(2.0 * M_PI * sy * frequencyX / hImage);
				break;

			case 2: // 垂直波浪
				srcY = sy + amplitudeY * sin(2.0 * M_PI * sx * frequencyY / wImage);
				break;

			case 3: // 双向波浪
				srcX = sx + amplitudeX * sin(2.0 * M_PI * sy * frequencyX / hImage);
				srcY = sy + amplitudeY * sin(2.0 * M_PI * sx * frequencyY / wImage);
				break;

			case 4: // 径向波浪
			{
				double centerX = wImage / 2.0;
				double centerY = hImage / 2.0;
				double dx = sx - centerX;
				double dy = sy - centerY;
				double r = sqrt(dx * dx + dy * dy);
				double angle = atan2(dy, dx);

				double newR = r + amplitudeX * sin(2.0 * M_PI * r * frequencyX / min(wImage, hImage));
				srcX = centerX + newR * cos(angle);
				srcY = centerY + newR * sin(angle);
			}
			break;

			case 5: // 扭曲波浪
			{
				double phaseX = 2.0 * M_PI * sy * frequencyX / hImage;
				double phaseY = 2.0 * M_PI * sx * frequencyY / wImage;
				srcX = sx + amplitudeX * sin(phaseX + phaseY);
				srcY = sy + amplitudeY * cos(phaseX - phaseY);
			}
			break;
			}

			// 边界检查和像素映射
			if (srcX >= 0 && srcX < wImage - 1 && srcY >= 0 && srcY < hImage - 1)
			{
				// 使用双线性插值
				int x1 = (int)srcX;
				int y1 = (int)srcY;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				double fx = srcX - x1;
				double fy = srcY - y1;

				// 获取四个邻近像素
				unsigned char p11 = oImage[y1 * wImage + x1];
				unsigned char p12 = oImage[y2 * wImage + x1];
				unsigned char p21 = oImage[y1 * wImage + x2];
				unsigned char p22 = oImage[y2 * wImage + x2];

				// 双线性插值计算
				double result = p11 * (1 - fx) * (1 - fy) +
								p21 * fx * (1 - fy) +
								p12 * (1 - fx) * fy +
								p22 * fx * fy;

				nImage[sy * wImage + sx] = (unsigned char)result;
			}
			else
			{
				nImage[sy * wImage + sx] = 0;
			}
		}
	}
}

/************************************************************************************************
 *                                                                                              *
 *  波浪变换接口函数                                                                            *
 *                                                                                              *
 ************************************************************************************************/
void ImageWaveTransformation(char *oImage, char *nImage, int wImage, int hImage,
							 int waveType, double amplitude, double frequency)
{
	// 参数验证和调整
	if (amplitude < 0)
		amplitude = -amplitude;
	if (amplitude > min(wImage, hImage) / 4.0)
		amplitude = min(wImage, hImage) / 4.0;
	if (frequency <= 0)
		frequency = 1.0;
	if (frequency > 20.0)
		frequency = 20.0;

	double amplitudeX = amplitude;
	double amplitudeY = amplitude;
	double frequencyX = frequency;
	double frequencyY = frequency;

	// 根据波浪类型调整参数
	switch (waveType)
	{
	case 1: // 水平波浪
		amplitudeY = 0;
		break;
	case 2: // 垂直波浪
		amplitudeX = 0;
		break;
	case 3: // 双向波浪
		// 保持默认参数
		break;
	case 4: // 径向波浪
		frequencyY = 0;
		break;
	case 5: // 扭曲波浪
		frequencyX = frequency * 0.7;
		frequencyY = frequency * 1.3;
		break;
	}

	WaveTransformation(oImage, nImage, wImage, hImage,
					   amplitudeX, frequencyX, amplitudeY, frequencyY, waveType);
}
/************************************************************************************************
 *                                                                                              *
 *  双边滤波函数                                                                           		  *
 *                                                                                              *
 ************************************************************************************************/

void BilateralFilterProcessing(char *oImage, char *nImage, int wImage, int hImage,
							   double sigmaSpace, double sigmaColor)
{
	int radius = 5; // 滤波半径
	double PI = 3.14159265359;

	for (int i = radius; i < hImage - radius; i++)
	{
		for (int j = radius; j < wImage - radius; j++)
		{
			double weightSum = 0.0;
			double pixelSum = 0.0;
			unsigned char centerPixel = (unsigned char)oImage[i * wImage + j];

			for (int m = -radius; m <= radius; m++)
			{
				for (int n = -radius; n <= radius; n++)
				{
					unsigned char neighborPixel = (unsigned char)oImage[(i + m) * wImage + (j + n)];

					// 计算空间权重
					double spatialWeight = exp(-(m * m + n * n) / (2 * sigmaSpace * sigmaSpace));

					// 计算颜色权重
					double colorDiff = centerPixel - neighborPixel;
					double colorWeight = exp(-(colorDiff * colorDiff) / (2 * sigmaColor * sigmaColor));

					double totalWeight = spatialWeight * colorWeight;
					weightSum += totalWeight;
					pixelSum += neighborPixel * totalWeight;
				}
			}

			nImage[i * wImage + j] = (unsigned char)(pixelSum / weightSum);
		}
	}
}
/************************************************************************************************
 *                                                                                              *
 *  形态学滤波                                                                                    *
 *                                                                                              *
 ************************************************************************************************/
// 腐蚀操作
void ErosionProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int structElement[9] = {1, 1, 1,
							1, 1, 1,
							1, 1, 1};
	int k = 1;

	for (int i = k; i < hImage - k; i++)
	{
		for (int j = k; j < wImage - k; j++)
		{
			unsigned char minVal = 255;
			for (int m = -k; m <= k; m++)
			{
				for (int n = -k; n <= k; n++)
				{
					if (structElement[(m + k) * 3 + (n + k)] == 1)
					{
						unsigned char pixel = (unsigned char)oImage[(i + m) * wImage + (j + n)];
						if (pixel < minVal)
							minVal = pixel;
					}
				}
			}
			nImage[i * wImage + j] = minVal;
		}
	}
}
// 膨胀操作
void DilationProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	int structElement[9] = {1, 1, 1,
							1, 1, 1,
							1, 1, 1};
	int k = 1;

	for (int i = k; i < hImage - k; i++)
	{
		for (int j = k; j < wImage - k; j++)
		{
			unsigned char maxVal = 0;
			for (int m = -k; m <= k; m++)
			{
				for (int n = -k; n <= k; n++)
				{
					if (structElement[(m + k) * 3 + (n + k)] == 1)
					{
						unsigned char pixel = (unsigned char)oImage[(i + m) * wImage + (j + n)];
						if (pixel > maxVal)
							maxVal = pixel;
					}
				}
			}
			nImage[i * wImage + j] = maxVal;
		}
	}
}
/************************************************************************************************
 *                                                                                              *
 *  自适应中值滤波                                                                           	   *
 *                                                                                              *
 ************************************************************************************************/
void AdaptiveMedianFilterProcessing(char *oImage, char *nImage, int wImage, int hImage)
{
	for (int i = 2; i < hImage - 2; i++)
	{
		for (int j = 2; j < wImage - 2; j++)
		{
			int windowSize = 3;
			bool found = false;

			while (windowSize <= 7 && !found)
			{
				int k = windowSize / 2;
				int pixels[49]; // 最大7x7窗口
				int count = 0;

				// 收集窗口内像素
				for (int m = -k; m <= k; m++)
				{
					for (int n = -k; n <= k; n++)
					{
						if (i + m >= 0 && i + m < hImage && j + n >= 0 && j + n < wImage)
						{
							pixels[count++] = (unsigned char)oImage[(i + m) * wImage + (j + n)];
						}
					}
				}

				// 排序
				for (int p = 0; p < count - 1; p++)
				{
					for (int q = 0; q < count - 1 - p; q++)
					{
						if (pixels[q] > pixels[q + 1])
						{
							int temp = pixels[q];
							pixels[q] = pixels[q + 1];
							pixels[q + 1] = temp;
						}
					}
				}

				int zmin = pixels[0];
				int zmax = pixels[count - 1];
				int zmed = pixels[count / 2];
				int zxy = (unsigned char)oImage[i * wImage + j];

				// 阶段A
				if (zmed > zmin && zmed < zmax)
				{
					// 阶段B
					if (zxy > zmin && zxy < zmax)
					{
						nImage[i * wImage + j] = zxy;
					}
					else
					{
						nImage[i * wImage + j] = zmed;
					}
					found = true;
				}
				else
				{
					windowSize += 2;
				}
			}

			if (!found)
			{
				nImage[i * wImage + j] = (unsigned char)oImage[i * wImage + j];
			}
		}
	}
}

/************************************************************************************************
 *                                                                                              *
 *  频域图像处理函数实现                                                                            *
 *                                                                                              *
 ************************************************************************************************/

// 位反转函数
int ReverseBits(int num, int bits)
{
	int result = 0;
	for (int i = 0; i < bits; i++)
	{
		result = (result << 1) | (num & 1);
		num >>= 1;
	}
	return result;
}

// 一维FFT变换
void FFT1D(double *realPart, double *imagPart, int n, bool inverse)
{
	double M_PI = 3.1415926;
	// 确保n是2的幂
	int bits = 0;
	int temp = n;
	while (temp > 1)
	{
		temp >>= 1;
		bits++;
	}

	// 如果n不是2的幂，需要填充或使用其他方法
	if ((1 << bits) != n)
	{
		// 使用DFT处理非2的幂情况
		double *tempReal = new double[n];
		double *tempImag = new double[n];

		// 复制原始数据
		for (int i = 0; i < n; i++)
		{
			tempReal[i] = realPart[i];
			tempImag[i] = imagPart[i];
		}

		// DFT计算
		for (int k = 0; k < n; k++)
		{
			double sumReal = 0.0;
			double sumImag = 0.0;

			for (int j = 0; j < n; j++)
			{
				double angle = 2.0 * M_PI * k * j / n * (inverse ? 1 : -1);
				double cosVal = cos(angle);
				double sinVal = sin(angle);

				sumReal += tempReal[j] * cosVal - tempImag[j] * sinVal;
				sumImag += tempReal[j] * sinVal + tempImag[j] * cosVal;
			}

			if (inverse)
			{
				realPart[k] = sumReal / n;
				imagPart[k] = sumImag / n;
			}
			else
			{
				realPart[k] = sumReal;
				imagPart[k] = sumImag;
			}
		}

		delete[] tempReal;
		delete[] tempImag;
		return;
	}

	// 位反转排序
	for (int i = 0; i < n; i++)
	{
		int j = ReverseBits(i, bits);
		if (i < j)
		{
			// 交换实部和虚部
			double tempReal = realPart[i];
			double tempImag = imagPart[i];
			realPart[i] = realPart[j];
			imagPart[i] = imagPart[j];
			realPart[j] = tempReal;
			imagPart[j] = tempImag;
		}
	}

	// Cooley-Tukey FFT算法
	for (int len = 2; len <= n; len <<= 1)
	{
		double angle = 2.0 * M_PI / len * (inverse ? 1 : -1);
		double wlenReal = cos(angle);
		double wlenImag = sin(angle);

		for (int i = 0; i < n; i += len)
		{
			double wReal = 1.0;
			double wImag = 0.0;

			for (int j = 0; j < len / 2; j++)
			{
				double uReal = realPart[i + j];
				double uImag = imagPart[i + j];
				double vReal = realPart[i + j + len / 2] * wReal - imagPart[i + j + len / 2] * wImag;
				double vImag = realPart[i + j + len / 2] * wImag + imagPart[i + j + len / 2] * wReal;

				realPart[i + j] = uReal + vReal;
				imagPart[i + j] = uImag + vImag;
				realPart[i + j + len / 2] = uReal - vReal;
				imagPart[i + j + len / 2] = uImag - vImag;

				// 更新旋转因子
				double tempReal = wReal * wlenReal - wImag * wlenImag;
				wImag = wReal * wlenImag + wImag * wlenReal;
				wReal = tempReal;
			}
		}
	}

	// 逆变换需要归一化
	if (inverse)
	{
		for (int i = 0; i < n; i++)
		{
			realPart[i] /= n;
			imagPart[i] /= n;
		}
	}
}

// 二维FFT变换
void FFT2D(double *realPart, double *imagPart, int width, int height, bool inverse)
{
	int i, j;
	// 对每一行进行一维FFT
	double *tempReal = new double[width];
	double *tempImag = new double[width];

	for (i = 0; i < height; i++)
	{
		// 提取一行数据
		for (j = 0; j < width; j++)
		{
			tempReal[j] = realPart[i * width + j];
			tempImag[j] = imagPart[i * width + j];
		}

		// 对这一行进行FFT
		FFT1D(tempReal, tempImag, width, inverse);

		// 将结果写回
		for (j = 0; j < width; j++)
		{
			realPart[i * width + j] = tempReal[j];
			imagPart[i * width + j] = tempImag[j];
		}
	}

	delete[] tempReal;
	delete[] tempImag;

	// 对每一列进行一维FFT
	tempReal = new double[height];
	tempImag = new double[height];

	for (j = 0; j < width; j++)
	{
		// 提取一列数据
		for (i = 0; i < height; i++)
		{
			tempReal[i] = realPart[i * width + j];
			tempImag[i] = imagPart[i * width + j];
		}

		// 对这一列进行FFT
		FFT1D(tempReal, tempImag, height, inverse);

		// 将结果写回
		for (i = 0; i < height; i++)
		{
			realPart[i * width + j] = tempReal[i];
			imagPart[i * width + j] = tempImag[i];
		}
	}

	delete[] tempReal;
	delete[] tempImag;
}

// 图像转换为复数形式
void ImageToComplex(char *image, double *realPart, double *imagPart, int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		realPart[i] = (double)static_cast<unsigned char>(image[i]);
		imagPart[i] = 0.0;
	}
}

// 复数转换为图像
void ComplexToImage(double *realPart, double *imagPart, char *image, int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		// 使用实部重建图像，而不是幅度
		double value = realPart[i];
		// 限制在0-255范围内
		if (value > 255)
			value = 255;
		if (value < 0)
			value = 0;
		image[i] = (char)(unsigned char)(value + 0.5);
	}
}

// 理想低通滤波器
void IdealLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double cutoffRadius = cutoffFreq * min(width, height) / 2;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 理想低通滤波器：距离大于截止频率则置零
			if (distance > cutoffRadius)
			{
				realPart[v * width + u] = 0.0;
				imagPart[v * width + u] = 0.0;
			}
		}
	}
}
// 高斯低通滤波器
void GaussianLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double sigma = cutoffFreq * min(width, height) / 4;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 高斯低通滤波器
			double H = exp(-(distance * distance) / (2 * sigma * sigma));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

// 巴特沃斯低通滤波器
void ButterworthLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double D0 = cutoffFreq * min(width, height) / 2;
	int n = 2; // 巴特沃斯阶数

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 巴特沃斯低通滤波器
			double H = 1.0 / (1.0 + pow(distance / D0, 2 * n));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

void FrequencyDomainSmoothing(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType)
{
	int i = 0, row, col;
	// 分配内存
	double *realPart = new double[wImage * hImage];
	double *imagPart = new double[wImage * hImage];

	// 1. 图像转换为复数
	ImageToComplex(oImage, realPart, imagPart, wImage, hImage);
	// 1.5. 频域中心化预处理：乘以(-1)^(row+col)
	for (row = 0; row < hImage; row++)
	{
		for (col = 0; col < wImage; col++)
		{
			if ((row + col) % 2 == 1)
			{
				realPart[row * wImage + col] *= -1;
				imagPart[row * wImage + col] *= -1;
			}
		}
	}
	// 2. 正向FFT
	FFT2D(realPart, imagPart, wImage, hImage, false);

	// 3. 应用频域滤波器
	switch (filterType)
	{
	case 0: // 理想低通
		IdealLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 1: // 高斯低通
		GaussianLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 2: // 巴特沃斯低通
		ButterworthLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	}

	// 4. 反向FFT
	FFT2D(realPart, imagPart, wImage, hImage, true);
	// 4.5. 频域中心化后处理：再次乘以(-1)^(row+col)
	for (row = 0; row < hImage; row++)
	{
		for (col = 0; col < wImage; col++)
		{
			if ((row + col) % 2 == 1)
			{
				realPart[row * wImage + col] *= -1;
				imagPart[row * wImage + col] *= -1;
			}
		}
	}
	// 5. 复数转换为图像
	ComplexToImage(realPart, imagPart, nImage, wImage, hImage);

	// 释放内存
	delete[] realPart;
	delete[] imagPart;
}
// 显示频谱
void ShowFrequencySpectrum(double *realPart, double *imagPart, int width, int height, int xPos, int yPos)
{
	int i, j;
	char *spectrumImage = new char[width * height];

	// 计算幅度谱
	double maxMagnitude = 0;
	for (i = 0; i < width * height; i++)
	{
		double magnitude = sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]);
		if (magnitude > maxMagnitude)
		{
			maxMagnitude = magnitude;
		}
	}

	// 对数变换并归一化
	for (i = 0; i < width * height; i++)
	{
		double magnitude = sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]);
		magnitude = log(1 + magnitude) / log(1 + maxMagnitude) * 255;
		spectrumImage[i] = (char)(unsigned char)magnitude;
	}

	// 频谱中心化显示
	char *centeredSpectrum = new char[width * height];
	int halfWidth = width / 2;
	int halfHeight = height / 2;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			int newI = (i + halfHeight) % height;
			int newJ = (j + halfWidth) % width;
			centeredSpectrum[newI * width + newJ] = spectrumImage[i * width + j];
		}
	}

	ShowImage(centeredSpectrum, width, height, xPos, yPos);

	delete[] spectrumImage;
	delete[] centeredSpectrum;
}

// 理想高通滤波器
void IdealHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double cutoffRadius = cutoffFreq * min(width, height) / 2;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 理想高通滤波器：距离小于截止频率则置零
			if (distance <= cutoffRadius)
			{
				realPart[v * width + u] = 0.0;
				imagPart[v * width + u] = 0.0;
			}
		}
	}
}

// 高斯高通滤波器
void GaussianHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double sigma = cutoffFreq * min(width, height) / 4;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 高斯高通滤波器：H = 1 - exp(-D?/(2σ?))
			double H = 1.0 - exp(-(distance * distance) / (2.0 * sigma * sigma));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

// 巴特沃斯高通滤波器
void ButterworthHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double D0 = cutoffFreq * min(width, height) / 2;
	int n = 2; // 巴特沃斯阶数

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// 计算到频域中心的距离
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// 巴特沃斯高通滤波器：H = 1 / (1 + (D0/D)^(2n))
			double H = 1.0 / (1.0 + pow(D0 / (distance + 1e-6), 2 * n)); // 加小值避免除零

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}
// 频域边缘检测
void FrequencyDomainEdgeDetection(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType)
{
	int i = 0;
	int row, col;
	// 分配内存
	double *realPart = new double[wImage * hImage];
	double *imagPart = new double[wImage * hImage];

	// 1. 图像转换为复数
	ImageToComplex(oImage, realPart, imagPart, wImage, hImage);

	// 1.5. 频域中心化预处理：乘以(-1)^(row+col)
	for (row = 0; row < hImage; row++)
	{
		for (col = 0; col < wImage; col++)
		{
			if ((row + col) % 2 == 1)
			{
				realPart[row * wImage + col] *= -1;
				imagPart[row * wImage + col] *= -1;
			}
		}
	}

	// 2. 正向FFT
	FFT2D(realPart, imagPart, wImage, hImage, false);

	// 3. 应用频域高通滤波器
	switch (filterType)
	{
	case 0: // 理想高通
		IdealHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 1: // 高斯高通
		GaussianHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 2: // 巴特沃斯高通
		ButterworthHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	}

	// 4. 反向FFT
	FFT2D(realPart, imagPart, wImage, hImage, true);

	// 4.5. 频域中心化后处理：再次乘以(-1)^(row+col)
	for (row = 0; row < hImage; row++)
	{
		for (col = 0; col < wImage; col++)
		{
			if ((row + col) % 2 == 1)
			{
				realPart[row * wImage + col] *= -1;
				imagPart[row * wImage + col] *= -1;
			}
		}
	}

	// 5. 复数转换为图像
	ComplexToImage(realPart, imagPart, nImage, wImage, hImage);

	// 释放内存
	delete[] realPart;
	delete[] imagPart;
}

// 计算PSNR
double CalculatePSNR(char *original, char *processed, int width, int height)
{
	double mse = 0.0;
	int totalPixels = width * height;

	// 计算均方误差 (MSE)
	for (int i = 0; i < totalPixels; i++)
	{
		double diff = (double)((unsigned char)original[i]) - (double)((unsigned char)processed[i]);
		mse += diff * diff;
	}
	mse /= totalPixels;

	// 避免除零错误
	if (mse == 0.0)
		return 100.0; // 完全相同的图像

	// 计算PSNR
	double psnr = 10.0 * log10((255.0 * 255.0) / mse);
	return psnr;
}

// 计算MSE
double CalculateMSE(char *original, char *processed, int width, int height)
{
	double mse = 0.0;
	int totalPixels = width * height;

	for (int i = 0; i < totalPixels; i++)
	{
		double diff = (double)((unsigned char)original[i]) - (double)((unsigned char)processed[i]);
		mse += diff * diff;
	}

	return mse / totalPixels;
}

// 计算SSIM
double CalculateSSIM(char *original, char *processed, int width, int height)
{
	int i;
	double c1 = 6.5025;	 // (0.01 * 255)^2
	double c2 = 58.5225; // (0.03 * 255)^2

	// 计算均值
	double mean1 = 0.0, mean2 = 0.0;
	int totalPixels = width * height;

	for (i = 0; i < totalPixels; i++)
	{
		mean1 += (unsigned char)original[i];
		mean2 += (unsigned char)processed[i];
	}
	mean1 /= totalPixels;
	mean2 /= totalPixels;

	// 计算方差和协方差
	double var1 = 0.0, var2 = 0.0, cov = 0.0;
	for (i = 0; i < totalPixels; i++)
	{
		double diff1 = (unsigned char)original[i] - mean1;
		double diff2 = (unsigned char)processed[i] - mean2;
		var1 += diff1 * diff1;
		var2 += diff2 * diff2;
		cov += diff1 * diff2;
	}
	var1 /= (totalPixels - 1);
	var2 /= (totalPixels - 1);
	cov /= (totalPixels - 1);

	// 计算SSIM
	double numerator = (2 * mean1 * mean2 + c1) * (2 * cov + c2);
	double denominator = (mean1 * mean1 + mean2 * mean2 + c1) * (var1 + var2 + c2);

	return numerator / denominator;
}
// 计算图像清晰度 (基于梯度)
double CalculateSharpness(char *image, int width, int height)
{
	double sharpness = 0.0;

	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			// Sobel算子计算梯度
			int gx = -1 * (unsigned char)image[(i - 1) * width + (j - 1)] +
					 1 * (unsigned char)image[(i - 1) * width + (j + 1)] +
					 -2 * (unsigned char)image[i * width + (j - 1)] +
					 2 * (unsigned char)image[i * width + (j + 1)] +
					 -1 * (unsigned char)image[(i + 1) * width + (j - 1)] +
					 1 * (unsigned char)image[(i + 1) * width + (j + 1)];

			int gy = -1 * (unsigned char)image[(i - 1) * width + (j - 1)] +
					 -2 * (unsigned char)image[(i - 1) * width + j] +
					 -1 * (unsigned char)image[(i - 1) * width + (j + 1)] +
					 1 * (unsigned char)image[(i + 1) * width + (j - 1)] +
					 2 * (unsigned char)image[(i + 1) * width + j] +
					 1 * (unsigned char)image[(i + 1) * width + (j + 1)];

			sharpness += sqrt(gx * gx + gy * gy);
		}
	}

	return sharpness / ((width - 2) * (height - 2));
}
// 计算图像对比度
double CalculateContrast(char *image, int width, int height)
{
	int i;
	double mean = 0.0;
	int totalPixels = width * height;

	// 计算均值
	for (i = 0; i < totalPixels; i++)
	{
		mean += (unsigned char)image[i];
	}
	mean /= totalPixels;

	// 计算标准差
	double variance = 0.0;
	for (i = 0; i < totalPixels; i++)
	{
		double diff = (unsigned char)image[i] - mean;
		variance += diff * diff;
	}
	variance /= totalPixels;

	return sqrt(variance);
}
// 综合图像质量评估
void ComprehensiveQualityAssessment(char *original, char *processed, int width, int height)
{
	char message[1000];

	double psnr = CalculatePSNR(original, processed, width, height);
	double mse = CalculateMSE(original, processed, width, height);
	double ssim = CalculateSSIM(original, processed, width, height);
	double sharpness = CalculateSharpness(processed, width, height);
	double contrast = CalculateContrast(processed, width, height);

	sprintf(message,
			"图像质量评估结果:\n\n"
			"PSNR: %.2f dB\n"
			"MSE: %.2f\n"
			"SSIM: %.4f\n"
			"清晰度: %.2f\n"
			"对比度: %.2f\n\n"
			"质量评级: %s",
			psnr, mse, ssim, sharpness, contrast,
			(psnr > 30 && ssim > 0.9) ? "优秀" : (psnr > 25 && ssim > 0.8) ? "良好"
											 : (psnr > 20 && ssim > 0.6)   ? "一般"
																		   : "较差");

	MessageBoxEx(hWind, message, "图像质量评估结果",
				 MB_OK | MB_ICONINFORMATION | MB_TOPMOST,
				 MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
}

// 自适应阈值分割
void AdaptiveThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage, int blockSize)
{
	int i;
	int halfBlock = blockSize / 2;

	for (i = 0; i < hImage; i++)
	{
		for (int j = 0; j < wImage; j++)
		{
			// 计算局部区域的平均值
			int sum = 0;
			int count = 0;

			for (int di = -halfBlock; di <= halfBlock; di++)
			{
				for (int dj = -halfBlock; dj <= halfBlock; dj++)
				{
					int ni = i + di;
					int nj = j + dj;

					if (ni >= 0 && ni < hImage && nj >= 0 && nj < wImage)
					{
						sum += (unsigned char)oImage[ni * wImage + nj];
						count++;
					}
				}
			}

			int localMean = sum / count;
			int currentPixel = (unsigned char)oImage[i * wImage + j];

			// 自适应阈值分割
			if (currentPixel > localMean - 10) // 可调整的偏移量
				nImage[i * wImage + j] = (BYTE)255;
			else
				nImage[i * wImage + j] = 0;
		}
	}
}

// Otsu自动阈值分割
void OtsuThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage)
{
	int i;
	// 计算直方图
	int histogram[256] = {0};
	int totalPixels = wImage * hImage;

	for (i = 0; i < totalPixels; i++)
	{
		histogram[(unsigned char)oImage[i]]++;
	}

	// 计算最佳阈值
	double maxVariance = 0;
	int bestThreshold = 0;

	for (int t = 0; t < 256; t++)
	{
		// 计算前景和背景的权重
		double w0 = 0, w1 = 0;
		for (i = 0; i <= t; i++)
			w0 += histogram[i];
		for (i = t + 1; i < 256; i++)
			w1 += histogram[i];

		if (w0 == 0 || w1 == 0)
			continue;

		// 计算前景和背景的平均灰度
		double u0 = 0, u1 = 0;
		for (i = 0; i <= t; i++)
			u0 += i * histogram[i];
		for (i = t + 1; i < 256; i++)
			u1 += i * histogram[i];
		u0 /= w0;
		u1 /= w1;

		// 计算类间方差 (标准Otsu公式)
		w0 /= totalPixels;
		w1 /= totalPixels;
		double variance = w0 * w1 * (u0 - u1) * (u0 - u1);

		if (variance > maxVariance)
		{
			maxVariance = variance;
			bestThreshold = t;
		}
	}

	// 应用最佳阈值
	for (i = 0; i < totalPixels; i++)
	{
		if ((unsigned char)oImage[i] > bestThreshold)
			nImage[i] = (BYTE)255;
		else
			nImage[i] = 0;
	}
}
// 使用队列进行区域生长
struct Point
{
	int x, y;
};
// 区域生长分割
void RegionGrowingSegmentation(char *oImage, char *nImage, int wImage, int hImage, int seedX, int seedY, int threshold)
{
	int i;
	// 初始化结果图像
	memset(nImage, (BYTE)255, wImage * hImage);

	// 创建访问标记数组
	bool *visited = new bool[wImage * hImage];
	memset(visited, false, wImage * hImage * sizeof(bool));

	Point *queue = new Point[wImage * hImage];
	int front = 0, rear = 0;

	// 种子点
	if (seedX >= 0 && seedX < wImage && seedY >= 0 && seedY < hImage)
	{
		Point seedPoint;
		seedPoint.x = seedX;
		seedPoint.y = seedY;
		queue[rear++] = seedPoint;
		visited[seedY * wImage + seedX] = true;
		nImage[seedY * wImage + seedX] = 0;

		int seedValue = (unsigned char)oImage[seedY * wImage + seedX];

		// 8连通区域生长
		int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
		int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

		while (front < rear)
		{
			Point current = queue[front++];

			for (i = 0; i < 8; i++)
			{
				int nx = current.x + dx[i];
				int ny = current.y + dy[i];

				if (nx >= 0 && nx < wImage && ny >= 0 && ny < hImage && !visited[ny * wImage + nx])
				{
					int pixelValue = (unsigned char)oImage[ny * wImage + nx];

					// 判断是否满足生长条件
					if (abs(pixelValue - seedValue) <= threshold)
					{
						visited[ny * wImage + nx] = true;
						nImage[ny * wImage + nx] = 0;
						Point seedPoint;
						seedPoint.x = nx;
						seedPoint.y = ny;
						queue[rear++] = seedPoint;
					}
				}
			}
		}
	}

	delete[] visited;
	delete[] queue;
}

// 边界追踪（Moore邻域追踪）
void BoundaryTracing(char *oImage, char *nImage, int wImage, int hImage, int threshold)
{
	int i;
	// 首先进行二值化
	char *binaryImage = new char[wImage * hImage];
	for (i = 0; i < wImage * hImage; i++)
	{
		if ((unsigned char)oImage[i] > threshold)
			binaryImage[i] = (BYTE)255;
		else
			binaryImage[i] = 0;
	}

	// 初始化结果图像
	memset(nImage, 0, wImage * hImage);

	// Moore邻域方向（8连通）
	int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
	int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

	// 寻找边界点并追踪
	for (i = 1; i < hImage - 1; i++)
	{
		for (int j = 1; j < wImage - 1; j++)
		{
			if ((unsigned char)binaryImage[i * wImage + j] == 255)
			{
				// 检查是否为边界点
				bool isBoundary = false;
				for (int k = 0; k < 8; k++)
				{
					int ni = i + dy[k];
					int nj = j + dx[k];
					if ((unsigned char)binaryImage[ni * wImage + nj] == 0)
					{
						isBoundary = true;
						break;
					}
				}

				if (isBoundary)
				{
					nImage[i * wImage + j] = (BYTE)255;
				}
			}
		}
	}

	delete[] binaryImage;
}

// 分水岭分割（简化版本）
void WatershedSegmentation(char *oImage, char *nImage, int wImage, int hImage)
{
	int i, j;
	// 计算梯度图像
	char *gradientImage = new char[wImage * hImage];

	for (i = 1; i < hImage - 1; i++)
	{
		for (j = 1; j < wImage - 1; j++)
		{
			// Sobel算子计算梯度
			int gx = -1 * (unsigned char)oImage[(i - 1) * wImage + (j - 1)] +
					 1 * (unsigned char)oImage[(i - 1) * wImage + (j + 1)] +
					 -2 * (unsigned char)oImage[i * wImage + (j - 1)] +
					 2 * (unsigned char)oImage[i * wImage + (j + 1)] +
					 -1 * (unsigned char)oImage[(i + 1) * wImage + (j - 1)] +
					 1 * (unsigned char)oImage[(i + 1) * wImage + (j + 1)];

			int gy = -1 * (unsigned char)oImage[(i - 1) * wImage + (j - 1)] +
					 -2 * (unsigned char)oImage[(i - 1) * wImage + j] +
					 -1 * (unsigned char)oImage[(i - 1) * wImage + (j + 1)] +
					 1 * (unsigned char)oImage[(i + 1) * wImage + (j - 1)] +
					 2 * (unsigned char)oImage[(i + 1) * wImage + j] +
					 1 * (unsigned char)oImage[(i + 1) * wImage + (j + 1)];

			int gradient = (int)sqrt(gx * gx + gy * gy);
			if (gradient > 255)
				gradient = 255;
			gradientImage[i * wImage + j] = (char)gradient;
		}
	}

	// 简化的分水岭：基于梯度阈值分割
	int gradientThreshold = 50;
	for (i = 0; i < wImage * hImage; i++)
	{
		if ((unsigned char)gradientImage[i] > gradientThreshold)
			nImage[i] = (BYTE)255; // 边界
		else
			nImage[i] = 0; // 区域内部
	}

	delete[] gradientImage;
}