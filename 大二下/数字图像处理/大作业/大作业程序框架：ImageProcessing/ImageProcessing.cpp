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

BOOL ReadImage(LPSTR, char *, int, int); // ��ȡͼ����Ϣ��������Image[][]��
BOOL ReadImage(const char *filename, char **oImage, int *width, int *height);
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

void ImageHistogramShowProcessing(char *oImage, char *nImage, int wImage, int hImage);	// ��ʾֱ��ͼ����
void ImageGrayReversionProcessing(char *Image, int wImage, int hImage);					// ͼ��Ҷȷ�ת����
void ThresholdImage(char *oImage, char *nImage, int wImage, int hImage, int Threshold); // ͼ��λƽ��ָ�
void ThresholdImage2(char *oImage, int wImage, int hImage, int Threshold);				// ͼ���ֵ������
void ImageHistogramEqualization(char *oImage, char *nImage, int wImage, int hImage);	// ֱ��ͼ���⻯
void GammaCorrection(char *oImage, char *nImage, int wImage, int hImage, double gamma); // ٤��У��

// ͼ���˲�

void ImageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage, int *Mask, int MaskWH, int MaskCoff);		  // ͼ��(3x3ģ�崦��)����
void AverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // ͼ���ֵ�˲�����
void GuassAverageMaskProcessing(char *oImage, char *nImage, int wImage, int hImage);									  // ͼ���˹(ƽ��)�˲�����
void MaxFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // ���ֵ�˲�
void MinFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // ���ֵ�˲�
void MidvalueFilterProcessing3(char *oImage, char *nImage, int wImage, int hImage);										  // ��ֵ�˲�3��3
void MidvalueFilterProcessing5(char *oImage, char *nImage, int wImage, int hImage);										  // ��ֵ�˲�5��5
void BilateralFilterProcessing(char *oImage, char *nImage, int wImage, int hImage, double sigmaSpace, double sigmaColor); // ˫���˲�
void ErosionProcessing(char *oImage, char *nImage, int wImage, int hImage);												  // ��ʴ����
void DilationProcessing(char *oImage, char *nImage, int wImage, int hImage);											  // ���Ͳ���
void AdaptiveMedianFilterProcessing(char *oImage, char *nImage, int wImage, int hImage);								  // ����Ӧ��ֵ�˲�
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
void SimpleGeometricTranslation(char *oImage, char *nImage, int wImage, int hImage, double *sgt_Array);																   // �򵥼��α任����
void ImageTranslation(char *oImage, char *nImage, int wImage, int hImage, int xPos, int yPos);																		   // ƽ�Ʊ任
void ImageScaling(char *oImage, char *nImage, int wImage, int hImage, double xScaling, double yScaling);															   // ���ű任
void ImageRotation(char *oImage, char *nImage, int wImage, int hImage, double iAngle);																				   // ��ת�任
void ImageMirror(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal);																				   // ����任
void ImageShearing(char *oImage, char *nImage, int wImage, int hImage, bool Horizontal, int k);																		   // ���б任
void SphereLensTransform(char *oImage, char *nImage, int wImage, int hImage, double centerX, double centerY, double radius, double strength);						   // ����͸��
unsigned char BilinearInterpolation(char *image, int w, int h, double x, double y);																					   // ˫���Բ�ֵ��������
void PerspectiveTransformation(char *oImage, char *nImage, int wImage, int hImage, double *pt_Array);																   // ͸�ӱ任
bool InversePerspectiveTransform(double u, double v, double *x, double *y, double *matrix);																			   // ͸��У��
void RadialTransformation(char *oImage, char *nImage, int wImage, int hImage, double *rt_Array);																	   // ����任����
void ImageBarrelDistortion(char *oImage, char *nImage, int wImage, int hImage, double k1, double k2);																   // Ͱ��/���λ���
void ImageDistortionCorrection(char *oImage, char *nImage, int wImage, int hImage, double k1, double k2);															   // �������У��
double SolveUndistortedRadius(double distortedRadius, double k1, double k2);																						   // ��������޻���뾶																																									   // ˫���Բ�ֵ
void SwirlTransformation(char *oImage, char *nImage, int wImage, int hImage, double strength, double radius);														   // ���б任����
void ImageSwirlTransformation(char *oImage, char *nImage, int wImage, int hImage, double strength, double radius);													   // ���б任�ӿں���
void ImageWaveTransformation(char *oImage, char *nImage, int wImage, int hImage, int waveType, double amplitude, double frequency);									   // ���˱任
void WaveTransformation(char *oImage, char *nImage, int wImage, int hImage, double amplitudeX, double frequencyX, double amplitudeY, double frequencyY, int waveType); // ���˱任����
void CalculatePerspectiveMatrix(double srcPoints[8], double dstPoints[8], double matrix[8]);																		   // ����͸�ӱ任����
void ImagePerspectiveCorrection(char *oImage, char *nImage, int wImage, int hImage, int correctionType);															   // ͼ��͸��У��
void SolveLinearSystem(double A[8][8], double b[8], double x[8], int n);																							   // ������Է�����
void PerspectiveTransformationImproved(char *oImage, char *nImage, int wImage, int hImage, double *matrix);															   // �Ľ���͸�ӱ任
// ���¹���1:
// Ƶ����������
void IdealLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);							  // �����ͨ�˲���
void ButterworthLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);					  // ������˹��ͨ�˲���
void GaussianLowPassFilter(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq);						  // ��˹��ͨ�˲���
void FFT1D(double *realPart, double *imagPart, int n, bool inverse);													  // һά���ٸ���Ҷ�任
void FFT2D(double *realPart, double *imagPart, int width, int height, bool inverse);									  // ��ά���ٸ���Ҷ�任
void FrequencyDomainSmoothing(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType);	  // Ƶ��ƽ���˲�
void ImageToComplex(char *image, double *realPart, double *imagPart, int width, int height);							  // ͼ��ת������ʽ
void ComplexToImage(double *realPart, double *imagPart, char *image, int width, int height);							  // ����תͼ����ʽ
int ReverseBits(int num, int bits);																						  // λ��ת����
void ShowFrequencySpectrum(double *realPart, double *imagPart, int width, int height, int xPos, int yPos);				  // ��ʾƵ��ͼ
void FrequencyDomainEdgeDetection(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType); // Ƶ���Ե���
void IdealHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);					  // �����ͨ�˲���
void GaussianHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);				  // ��˹��ͨ�˲���
void ButterworthHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq);			  // ������˹��ͨ�˲���

// ���¹���2:��������
double CalculatePSNR(char *original, char *processed, int width, int height);				 // �����ֵ�����
double CalculateMSE(char *original, char *processed, int width, int height);				 // ����������
double CalculateSSIM(char *original, char *processed, int width, int height);				 // ����ṹ������ָ��
double CalculateSharpness(char *image, int width, int height);								 // ����ͼ��������
double CalculateContrast(char *image, int width, int height);								 // ����ͼ��Աȶ�
void ComprehensiveQualityAssessment(char *original, char *processed, int width, int height); // �ۺ���������

// ���¹���3��ͼ��ָ�
void AdaptiveThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage, int blockSize);					 // ����Ӧ��ֵ�ָ�
void RegionGrowingSegmentation(char *oImage, char *nImage, int wImage, int hImage, int seedX, int seedY, int threshold); // ���������ָ�
void BoundaryTracing(char *oImage, char *nImage, int wImage, int hImage, int threshold);								 // �߽����
void OtsuThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage);										 // Otsu��ֵ�ָ�
void WatershedSegmentation(char *oImage, char *nImage, int wImage, int hImage);											 // ��ˮ��ָ�
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
		case IDM_SHOWRAWIMAGE:
			OpenImageFileDlg("��ͼ���ļ�");
			// ReadImage(ImgDlgFileName, OrgImage, IMAGEWIDTH, IMAGEHEIGHT);
			// ShowImage(OrgImage, IMAGEWIDTH, IMAGEHEIGHT, XPOS, YPOS);
			ReadBmpImage(ImgDlgFileName, OrgImage);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_HISTOGRAMSHOW: // ��ʾֱ��ͼ
			ImageHistogramShowProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_THRESHOLD: // ͼ���ֵ��(λƽ��7)
			ThresholdImage2(OrgImage, ImageWidth, ImageHeight, 128);
			// ShowImage(OrgImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 900);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE1: // λƽ��0
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_DIVIEDPLANE2: // λƽ��1
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE3: // λƽ��2
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_DIVIEDPLANE4: // λƽ��3
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 8);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_DIVIEDPLANE5: // λƽ��4
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 16);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;
		case IDM_DIVIEDPLANE6: // λƽ��5
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 32);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			break;
		case IDM_DIVIEDPLANE7: // λƽ��6
			ThresholdImage(OrgImage, NewImage, ImageWidth, ImageHeight, 64);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 300);
			break;
		case IDM_BALANCED: // ͼ����⻯
			ImageHistogramEqualization(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			ImageHistogramShowProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_GRAYREVERSION: // ͼ��Ҷȷ�ת
			ImageGrayReversionProcessing(OrgImage, ImageWidth, ImageHeight);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case GammaCorrection2: // ٤��У��2
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case GammaCorrection4: // ٤��У��4
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case GammaCorrection0_5: // ٤��У��0.5
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case GammaCorrection0_2: // ٤��У��0.2
			GammaCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_GRAYTOLOW: // ����
			ShowGrayToLow(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_LEFTRIGHTOPEN: // �����ҿ���
			ShowLeftRightOpen(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
		case IDM_TBSHUTTER: // �������°�Ҷ��
			ShowTopBottomShutter(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
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
			AverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_AVERAGEGAUSS: // ��˹�˲���
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MAXFILTER: // ���ֵ�˲�
			MaxFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MINFILTER: // ���ֵ�˲�
			MinFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER3: // ��ֵ�˲�3*3
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_MidFILTER5: // ��ֵ�˲�5*5
			MidvalueFilterProcessing5(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
			// �������
		case IDM_SaltPeper: // ��������
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0.5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Salt: // ������
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 1);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Pepper: // ������
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_Gauss: // ��˹����
			AddGaussianNoise(OrgImage, ImageWidth, ImageHeight, 0, 5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS, YPOS);
			break;
			// ��Ե���
		case IDM_LAPLACEFILTER: // Laplace��Ե����˲�
			LaplaceEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_LAPLACEFILTER2: // Laplace+��˹�˲���Ե����˲�
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			LaplaceEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_SobelFILTER: // Sobel��Ե����˲�
			SobelEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PrewittFILTER: // Prewitt��Ե����˲�
			PrewittEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 900);
			break;
		case IDM_RobertsFILTER: // Roberts��Ե����˲�
			RobertsEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, IMAGEWIDTH - 1, IMAGEHEIGHT - 1, XPOS, YPOS + 1200);
			break;
		case IDM_KirschFILTER: // Kirsch��Ե����˲�
			KirschEdgeProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;

		case IDM_CannyFILTER: // Canny��Ե����˲�
			GuassAverageMaskProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			SobelEdgeProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			CannyEdgeProcessing(NewImage2, NewImage3, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage3, ImageWidth, ImageHeight, XPOS, YPOS + 1500);
			break;
		case IDM_BilateralFilter: // ˫���˲�
			BilateralFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight, 5.0, 25.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ErosionProcessing1: // ��̬ѧ�˲�--��ʴ
			ErosionProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ErosionProcessing2: // ��̬ѧ�˲�--����
			DilationProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_OpenProcessing: // ��̬ѧ�˲�--������ ��ʴ + ����
			ErosionProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			DilationProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_CloseProcessing: // ��̬ѧ�˲�--������ ���� + ��ʴ
			DilationProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ErosionProcessing(NewImage, NewImage2, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage2, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_AdaptiveMedianFilter: // ����Ӧ��ֵ�˲�
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			// ��˹����
			AddGaussianNoise(OrgImage, ImageWidth, ImageHeight, 0, 5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS + 600);
			// չʾ��������
			AddSaltAndPepperNoise(OrgImage, ImageWidth, ImageHeight, 0.5, 0.5);
			ShowBmpImage(OrgImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			MidvalueFilterProcessing3(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			AdaptiveMedianFilterProcessing(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;

		case IDM_TRANSLATION: // ͼ��ƽ��
			ImageTranslation(OrgImage, NewImage, ImageWidth, ImageHeight, 20, 30);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_SCALING: // ͼ������
			ImageScaling(OrgImage, NewImage, ImageWidth, ImageHeight, 2.0, 0.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_ROTATION: // ͼ����ת
			ImageRotation(OrgImage, NewImage, ImageWidth, ImageHeight, 45.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalMirror: // ͼ��ˮƽ��ת
			ImageMirror(OrgImage, NewImage, ImageWidth, ImageHeight, true);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_VerticalMirror: // ͼ����ֱ��ת
			ImageMirror(OrgImage, NewImage, ImageWidth, ImageHeight, false);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_HorizontalShearing: // ͼ��ˮƽ����
			ImageShearing(OrgImage, NewImage, ImageWidth, ImageHeight, true, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_VerticalShearing: // ͼ����ֱ����
			ImageShearing(OrgImage, NewImage, ImageWidth, ImageHeight, false, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PerspectiveCorrection1: // ͼ��͸�ӱ任
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_PerspectiveCorrection2: // ͼ��͸�ӱ任
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_PerspectiveCorrection3: // ͼ��͸�ӱ任
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 3);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 900);
			break;
		case IDM_PerspectiveCorrection4: // ͼ��͸�ӱ任
			ImagePerspectiveCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 4);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_BarrelDistortion1: // ͼ��Ͱ�λ��䣨��ֵ��
			ImageBarrelDistortion(OrgImage, NewImage, ImageWidth, ImageHeight, 0.5, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_BarrelDistortion2: // ͼ�����λ��䣨��ֵ��
			ImageBarrelDistortion(OrgImage, NewImage, ImageWidth, ImageHeight, -0.4, -0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_ImageDistortionCorrection2: // У������
			ImageDistortionCorrection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.4, 0.2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_SwirlTransformation: // ���б任
			ImageSwirlTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 2.0, IMAGEWIDTH);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_WaveTransformation1: // ˮƽ���˱任
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 1, 20.0, 3.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_WaveTransformation2: // ˫���˱任
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 3, 15.0, 2.0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS);
			break;
		case IDM_WaveTransformation3: // �����˱任
			ImageWaveTransformation(OrgImage, NewImage, ImageWidth, ImageHeight, 4, 25.0, 1.5);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;

		case IDM_IDEAL_FILTER: // �����ͨ�˲�
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_GAUSSIAN_FILTER: // ��˹��ͨ�˲�
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;
		case IDM_BUTTERWORTH_FILTER: // ������˹��ͨ�˲�
			FrequencyDomainSmoothing(OrgImage, NewImage, ImageWidth, ImageHeight, 0.3, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;
		case IDM_IDEAL_HIGH_FILTER: // �����ͨ�˲�����Ե��⣩
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 0);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 300);
			break;
		case IDM_GAUSSIAN_HIGH_FILTER: // ��˹��ͨ�˲�����Ե��⣩
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 1);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 300, YPOS + 600);
			break;
		case IDM_BUTTERWORTH_HIGH_FILTER: // ������˹��ͨ�˲�����Ե��⣩
			FrequencyDomainEdgeDetection(OrgImage, NewImage, ImageWidth, ImageHeight, 0.05, 2);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS + 600, YPOS);
			break;
		case IDM_FREQ_SPECTRUM: // ��ʾƵ��
		{
			double *realPart = new double[IMAGEWIDTH * IMAGEHEIGHT];
			double *imagPart = new double[IMAGEWIDTH * IMAGEHEIGHT];
			// ��ͼ������ת��Ϊdouble����
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
		case IDM_QUALITY_ASSESSMENT: // �ۺ���������
			if (OrgImage != NULL && NewImage != NULL)
			{
				ComprehensiveQualityAssessment(OrgImage, NewImage, ImageWidth, ImageHeight);
			}
			else
			{
				MessageBox(hWnd, "���ȼ���ԭʼͼ��ʹ����ͼ��", "��ʾ", MB_OK);
			}
			break;
		case IDM_ADAPTIVE_THRESHOLD: // ����Ӧ��ֵ�ָ�
			AdaptiveThresholdSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight, 15);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_OTSU_THRESHOLD: // Otsu�Զ���ֵ
			OtsuThresholdSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 600);
			break;

		case IDM_REGION_GROWING: // ���������ָ�
			// ʹ��ͼ��������Ϊ���ӵ㣬��ֵΪ30
			RegionGrowingSegmentation(OrgImage, NewImage, ImageWidth, ImageHeight,
									  ImageWidth / 2, ImageHeight / 2, 30);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_BOUNDARY_TRACING: // �߽�׷��
			BoundaryTracing(OrgImage, NewImage, ImageWidth, ImageHeight, 128);
			ShowBmpImage(NewImage, ImageWidth, ImageHeight, XPOS, YPOS + 300);
			break;

		case IDM_WATERSHED: // ��ˮ��ָ�
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

	// if ((bfImage.bfType != 0x4d42))
	// { // "BM"
	// 	MessageBox(NULL, NULL, "��bmp�ļ�������Ϣ", MB_OK);
	// 	return FALSE;
	// }

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight = biImage.biHeight;
	// if (biImage.biBitCount != 24)
	// { // 24λ��ɫͼ��
	// 	MessageBox(NULL, NULL, "����24λbmpͼ��", MB_OK);
	// 	return FALSE;
	// }

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
 *  ֱ��ͼ���⻯																				 *
 *																								*
 ************************************************************************************************/

void ImageHistogramEqualization(char *oImage, char *nImage, int wImage, int hImage)
{
	const int L = 256;
	int i;
	// 1. ����ԭʼͼ��ĻҶ�ֱ��ͼ
	int GrayCount[L] = {0}; // ��ʼ��ֱ��ͼ����
	int imageSize = wImage * hImage;

	for (i = 0; i < imageSize; i++)
	{
		// ʹ���޷����ַ�����ȷ����0-255�ĻҶ�ֵ
		unsigned char gray = (unsigned char)oImage[i];
		if (gray >= L)
			gray = L - 1; // ��ȫ�߽���
		GrayCount[gray]++;
	}

	// 2. �����ۻ��ֲ����� (hs->hp)
	double hs[L] = {0};
	for (i = 0; i < L; i++)
	{
		hs[i] = (double)GrayCount[i] / imageSize; // ʹ�������������й�һ��
	}

	// 3. �����ۻ��ֲ�����hp
	double hp[L] = {0};
	hp[0] = hs[0];
	for (i = 1; i < L; i++)
	{
		hp[i] = hp[i - 1] + hs[i]; // �ۻ��ֲ�
	}
	// 4. ӳ�����ص��»Ҷ�ֵ gx = 255 * hp[f(x)]
	for (i = 0; i < imageSize; i++)
	{
		unsigned char orig = (unsigned char)oImage[i];
		if (orig >= L)
			orig = L - 1; // ��ȫ�߽���

		// Ӧ��ӳ�乫ʽ
		double newValue = (L - 1) * hp[orig];

		// ȷ�������0-255��Χ��
		if (newValue < 0)
			newValue = 0;
		if (newValue > L - 1)
			newValue = L - 1;

		// ת��Ϊ�ַ�����Ҫ��ʽת��Ϊunsigned char���⸺ֵ��
		nImage[i] = (char)((unsigned char)(newValue + 0.5)); // ��������
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
 *  ͼ���ݴκ���																		      *
 *																								*
 ************************************************************************************************/
void GammaCorrection(char *oImage, char *nImage, int wImage, int hImage, double gamma)
{
	int imageSize = wImage * hImage;
	double c = 1.0;
	for (int i = 0; i < imageSize; i++)
	{
		// ������Ҷ�ֵ��һ���� [0, 1] ��Χ
		double normalized = (unsigned char)oImage[i] / 255.0;

		// Ӧ���ݴα任��s = c * (r)^gamma
		double result = c * pow(normalized, gamma);

		// ȷ������� [0,1] ��Χ��
		if (result > 1.0)
			result = 1.0;
		if (result < 0.0)
			result = 0.0;

		// �����ӳ��� [0,255] ����������
		nImage[i] = (unsigned char)(result * 255.0 + 0.5);
	}
}
/************************************************************************************************
 *																								*
 *  ͼ��λƽ��ָ��																				 *
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

/************************************************************************************************
 *																								*
 *  ˫���Բ�ֵ��������    																		  *
 *																								*
 ************************************************************************************************/
unsigned char BilinearInterpolation(char *image, int w, int h, double x, double y)
{
	int x1 = (int)floor(x);
	int y1 = (int)floor(y);
	int x2 = x1 + 1;
	int y2 = y1 + 1;

	// �߽���
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 >= w)
		x2 = w - 1;
	if (y2 >= h)
		y2 = h - 1;

	// ��ֵȨ��
	double a = x - x1;
	double b = y - y1;

	// ��ȡ�ĸ��ڵ������ֵ
	unsigned char p11 = image[y1 * w + x1];
	unsigned char p12 = image[y1 * w + x2];
	unsigned char p21 = image[y2 * w + x1];
	unsigned char p22 = image[y2 * w + x2];

	// �����ֵ���
	double val = (1 - a) * (1 - b) * p11 +
				 a * (1 - b) * p12 +
				 (1 - a) * b * p21 +
				 a * b * p22;

	return (unsigned char)(val + 0.5); // ��������
}
/************************************************************************************************
 *																								*
 *  ͸�ӱ任����    																			  *
 *																								*
 ************************************************************************************************/
void PerspectiveTransformation(char *oImage, char *nImage, int wImage, int hImage, double *pt_Array)
{
	double axy, bxy, wxy;
	int i, sx, sy, ox, oy;
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0; // �����ͼ��
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // ��ͼ������Ϊԭ��
			oy = sy - hImage / 2;
			// ͸�ӱ任��ʽ: x' = (ax+by+c)/(gx+hy+1), y' = (dx+ey+f)/(gx+hy+1)
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
 *  �ĵ�͸��У������ʵ�õ�����У����    														    *
 *																								*
 ************************************************************************************************/
// ����͸�ӱ任����8Ԫ���Է�������⣩
void CalculatePerspectiveMatrix(double srcPoints[8], double dstPoints[8], double matrix[8])
{
	// �������Է����� Ax = b
	// ����͸�ӱ任: x' = (ax+by+c)/(gx+hy+1), y' = (dx+ey+f)/(gx+hy+1)
	// ��������õ����Է�����

	double A[8][8];
	double b[8];

	// Ϊ4����Խ���8������
	for (int i = 0; i < 4; i++)
	{
		double x = srcPoints[i * 2];	 // Դ��x����
		double y = srcPoints[i * 2 + 1]; // Դ��y����
		double u = dstPoints[i * 2];	 // Ŀ���x����
		double v = dstPoints[i * 2 + 1]; // Ŀ���y����

		// ��һ������: u*(gx+hy+1) = ax+by+c
		// ����Ϊ: ax + by + c + 0*d + 0*e + 0*f - u*g*x - u*h*y = u
		A[i * 2][0] = x;	  // a��ϵ��
		A[i * 2][1] = y;	  // b��ϵ��
		A[i * 2][2] = 1;	  // c��ϵ��
		A[i * 2][3] = 0;	  // d��ϵ��
		A[i * 2][4] = 0;	  // e��ϵ��
		A[i * 2][5] = 0;	  // f��ϵ��
		A[i * 2][6] = -u * x; // g��ϵ��
		A[i * 2][7] = -u * y; // h��ϵ��
		b[i * 2] = u;

		// �ڶ�������: v*(gx+hy+1) = dx+ey+f
		// ����Ϊ: 0*a + 0*b + 0*c + dx + ey + f - v*g*x - v*h*y = v
		A[i * 2 + 1][0] = 0;	  // a��ϵ��
		A[i * 2 + 1][1] = 0;	  // b��ϵ��
		A[i * 2 + 1][2] = 0;	  // c��ϵ��
		A[i * 2 + 1][3] = x;	  // d��ϵ��
		A[i * 2 + 1][4] = y;	  // e��ϵ��
		A[i * 2 + 1][5] = 1;	  // f��ϵ��
		A[i * 2 + 1][6] = -v * x; // g��ϵ��
		A[i * 2 + 1][7] = -v * y; // h��ϵ��
		b[i * 2 + 1] = v;
	}

	// ʹ�ø�˹��Ԫ��������Է�����
	SolveLinearSystem(A, b, matrix, 8);
}

// ��˹��Ԫ��������Է�����
void SolveLinearSystem(double A[8][8], double b[8], double x[8], int n)
{
	int i, k;
	// ǰ����Ԫ
	for (i = 0; i < n; i++)
	{
		// Ѱ����Ԫ
		int maxRow = i;
		for (k = i + 1; k < n; k++)
		{
			if (fabs(A[k][i]) > fabs(A[maxRow][i]))
			{
				maxRow = k;
			}
		}

		// ������
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

		// ��Ԫ
		for (k = i + 1; k < n; k++)
		{
			if (fabs(A[i][i]) > 1e-10)
			{ // �������
				double factor = A[k][i] / A[i][i];
				for (int j = i; j < n; j++)
				{
					A[k][j] -= factor * A[i][j];
				}
				b[k] -= factor * b[i];
			}
		}
	}

	// �ش����
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
	// Դͼ����ĸ��ǵ㣨���Σ�
	double srcPoints[8] = {
		topLeftX, topLeftY,
		topRightX, topRightY,
		bottomLeftX, bottomLeftY,
		bottomRightX, bottomRightY};

	// Ŀ��ͼ����ĸ��ǵ㣨��׼���Σ�
	double dstPoints[8] = {
		0, 0,				   // ���Ͻ�
		wImage - 1, 0,		   // ���Ͻ�
		0, hImage - 1,		   // ���½�
		wImage - 1, hImage - 1 // ���½�
	};

	// ����͸�ӱ任����
	double matrix[8];
	CalculatePerspectiveMatrix(srcPoints, dstPoints, matrix);

	// ִ��͸�ӱ任
	PerspectiveTransformationImproved(oImage, nImage, wImage, hImage, matrix);
}
void PerspectiveTransformationImproved(char *oImage, char *nImage, int wImage, int hImage, double *matrix)
{
	int i;
	// ���Ŀ��ͼ��
	for (i = 0; i < wImage * hImage; i++)
	{
		nImage[i] = 0;
	}

	// ͸�ӱ任����
	double a = matrix[0], b = matrix[1], c = matrix[2];
	double d = matrix[3], e = matrix[4], f = matrix[5];
	double g = matrix[6], h = matrix[7];

	// ��Ŀ��ͼ���ÿ�����ؽ��з���ӳ��
	for (int v = 0; v < hImage; v++)
	{
		for (int u = 0; u < wImage; u++)
		{
			// ����͸�ӱ任����Ŀ������(u,v)����Դ����(x,y)
			// �ⷽ���飺
			// u = (ax + by + c) / (gx + hy + 1)
			// v = (dx + ey + f) / (gx + hy + 1)

			// ʹ�õ�������ֱ�����
			double x, y;
			if (InversePerspectiveTransform(u, v, &x, &y, matrix))
			{
				// ���Դ�����Ƿ�����Ч��Χ��
				if (x >= 0 && x < wImage - 1 && y >= 0 && y < hImage - 1)
				{
					// ʹ��˫���Բ�ֵ��ȡ����ֵ
					unsigned char pixelValue = BilinearInterpolation(oImage, wImage, hImage, x, y);
					nImage[v * wImage + u] = pixelValue;
				}
			}
		}
	}
}

// ����͸�ӱ任���
bool InversePerspectiveTransform(double u, double v, double *x, double *y, double *matrix)
{
	double a = matrix[0], b = matrix[1], c = matrix[2];
	double d = matrix[3], e = matrix[4], f = matrix[5];
	double g = matrix[6], h = matrix[7];

	// �ⷽ���飨ʹ�ÿ���Ĭ���򣩣�
	// u*(gx + hy + 1) = ax + by + c
	// v*(gx + hy + 1) = dx + ey + f
	//
	// ����ã�
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
		return false; // �������죬�޽�
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
	case 1: // ��խ�¿������У��
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   w * 0.2, 0, w * 0.8, 0,
									   0, h, w, h);
		break;
	case 2: // �Ͽ���խ������У��
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, 0, w, 0,
									   w * 0.2, h, w * 0.8, h);
		break;
	case 3: // ��խ�ҿ������У��
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, h * 0.2, w, 0,
									   0, h * 0.8, w, h);
		break;
	case 4: // �����խ������У��
		FourPointPerspectiveCorrection(oImage, nImage, wImage, hImage,
									   0, 0, w, h * 0.2,
									   0, h, w, h * 0.8);
		break;
	}
}
/************************************************************************************************
 *																														*
 *  ����任�����������Ա任��    																	  *
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
		nImage[i] = 0; // �����ͼ��
	for (sy = 0; sy < hImage; sy++)
	{
		for (sx = 0; sx < wImage; sx++)
		{
			ox = sx - wImage / 2; // ��ͼ������Ϊԭ��
			oy = sy - hImage / 2;
			// ���㼫����
			radius = sqrt((double)(ox * ox + oy * oy));
			if (radius > 0.0)
			{
				angle = atan2((double)oy, (double)ox);
				// ��һ���뾶
				normalizedRadius = radius / maxRadius;
				// �Ľ��ľ���任��ʽ��Ч��������
				newRadius = radius * (1.0 + rt_Array[0] * normalizedRadius + rt_Array[1] * normalizedRadius * normalizedRadius);
				// ���Ʊ任��Χ��������ȱ���
				if (newRadius > maxRadius * 2.0)
					newRadius = maxRadius * 2.0;
				if (newRadius < 0.0)
					newRadius = 0.0;
				// ת����ֱ������
				axy = newRadius * cos(angle) + centerX;
				bxy = newRadius * sin(angle) + centerY;
			}
			else
			{
				axy = centerX;
				bxy = centerY;
			}
			if ((axy < 0.0) || (axy >= wImage - 1) || (bxy < 0.0) || (bxy >= hImage - 1))
				nImage[sy * wImage + sx] = 0; // ������Χ������0
			else
				nImage[sy * wImage + sx] = oImage[((int)bxy) * wImage + (int)axy];
		}
	}
}

/************************************************************************************************
 *																								*
 *  Ͱ��/���λ���    																			  *
 *																								*
 ************************************************************************************************/
void ImageBarrelDistortion(char *oImage, char *nImage, int wImage, int hImage,
						   double k1, double k2)
{
	double rt_Array[2];
	rt_Array[0] = k1; // һ����ϵ��
	rt_Array[1] = k2; // ������ϵ��

	RadialTransformation(oImage, nImage, wImage, hImage, rt_Array);
}
/************************************************************************************************
 *                                                                                              *
 *  ��������޻���뾶��ţ�ٵ�������                                                            *
 *                                                                                              *
 ************************************************************************************************/
double SolveUndistortedRadius(double distortedRadius, double k1, double k2)
{
	if (distortedRadius == 0.0)
		return 0.0;

	double r = distortedRadius; // ��ʼ����ֵ

	// ţ�ٵ�������⣺r_d = r_u * (1 + k1*r_u_norm + k2*r_u_norm^2)
	// ���� r_u_norm = r_u / maxRadius������������ֱ���ù�һ����distortedRadius
	for (int i = 0; i < 10; i++) // ������10��
	{
		// ע�⣺�����r�Ѿ��ǹ�һ���İ뾶
		double r2 = r * r;

		// f(r) = r * (1 + k1*r + k2*r^2) - distortedRadius
		double f = r * (1.0 + k1 * r + k2 * r2) - distortedRadius;

		// f'(r) = 1 + 2*k1*r + 3*k2*r^2
		double df = 1.0 + 2.0 * k1 * r + 3.0 * k2 * r2;

		if (fabs(df) < 1e-10) // �������
			break;

		double newR = r - f / df;

		// �������
		if (fabs(newR - r) < 1e-8)
			break;

		r = newR;

		// ��ֹ��ɢ
		if (r < 0)
			r = distortedRadius * 0.5;
	}

	return r;
}

/************************************************************************************************
 *                                                                                              *
 *  �������У������                                                                            *
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
			// ������������ĵ�����
			double dx = x - centerX;
			double dy = y - centerY;
			double distortedRadius = sqrt(dx * dx + dy * dy);

			if (distortedRadius > 0)
			{
				// ��һ���뾶
				double normalizedRadius = distortedRadius / maxRadius;

				// ����޻���뾶
				double undistortedRadius = SolveUndistortedRadius(normalizedRadius, k1, k2) * maxRadius;

				// ����Դͼ������
				double scale = undistortedRadius / distortedRadius;
				double srcX = centerX + dx * scale;
				double srcY = centerY + dy * scale;

				// �߽���
				if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h)
				{
					// ʹ�����е�˫���Բ�ֵ����
					unsigned char pixel = BilinearInterpolation(tempImage, w, h, srcX, srcY);
					nImage[y * w + x] = pixel;
				}
				else
				{
					nImage[y * w + x] = 0; // �߽�����Ϊ��ɫ
				}
			}
			else
			{
				nImage[y * w + x] = tempImage[y * w + x]; // ���ĵ㱣�ֲ���
			}
		}
	}

	delete[] tempImage;
}
/************************************************************************************************
 *																								*
 *  ���б任    																		     *
 *																								*
 ************************************************************************************************/
void SwirlTransformation(char *oImage, char *nImage, int wImage, int hImage,
						 double strength, double radius)
{
	double centerX = wImage / 2.0;
	double centerY = hImage / 2.0;
	int i;

	// ���Ŀ��ͼ��
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0;

	for (int sy = 0; sy < hImage; sy++)
	{
		for (int sx = 0; sx < wImage; sx++)
		{
			// ������������ĵ�����
			double dx = sx - centerX;
			double dy = sy - centerY;
			double r = sqrt(dx * dx + dy * dy);

			if (r < radius && r > 0)
			{
				// ����ԭʼ�Ƕ�
				double angle = atan2(dy, dx);

				// �������нǶ�ƫ�ƣ�ʹ������˥��������
				double factor = sin(3.1415926 * r / (2.0 * radius));
				double swirlAngle = strength * factor;

				// Ӧ�����б任
				double newAngle = angle + swirlAngle;

				// ת����ֱ������
				double srcX = centerX + r * cos(newAngle);
				double srcY = centerY + r * sin(newAngle);

				// �߽��������ӳ��
				if (srcX >= 0 && srcX < wImage - 1 && srcY >= 0 && srcY < hImage - 1)
				{
					// ʹ��˫���Բ�ֵ
					int x1 = (int)srcX;
					int y1 = (int)srcY;
					int x2 = x1 + 1;
					int y2 = y1 + 1;

					double fx = srcX - x1;
					double fy = srcY - y1;

					// ��ȡ�ĸ��ڽ�����
					unsigned char p11 = oImage[y1 * wImage + x1];
					unsigned char p12 = oImage[y2 * wImage + x1];
					unsigned char p21 = oImage[y1 * wImage + x2];
					unsigned char p22 = oImage[y2 * wImage + x2];

					// ˫���Բ�ֵ����
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
				// �������а뾶��ֱ�Ӹ���ԭ����
				nImage[sy * wImage + sx] = oImage[sy * wImage + sx];
			}
		}
	}
}

/************************************************************************************************
 *                                                                                              *
 *  ���б任�ӿں���                                                                            *
 *                                                                                              *
 ************************************************************************************************/
void ImageSwirlTransformation(char *oImage, char *nImage, int wImage, int hImage,
							  double strength, double radius)
{
	// ������֤
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
 *  ���˱任������Wave Transformation��                                                         *
 *                                                                                              *
 ************************************************************************************************/
void WaveTransformation(char *oImage, char *nImage, int wImage, int hImage,
						double amplitudeX, double frequencyX,
						double amplitudeY, double frequencyY,
						int waveType)
{
	int i;

	// ���Ŀ��ͼ��
	for (i = 0; i < wImage * hImage; i++)
		nImage[i] = 0;
	double M_PI = 3.1415926;
	for (int sy = 0; sy < hImage; sy++)
	{
		for (int sx = 0; sx < wImage; sx++)
		{
			double srcX = sx;
			double srcY = sy;

			// ���ݲ�������Ӧ�ò�ͬ�ı任
			switch (waveType)
			{
			case 1: // ˮƽ����
				srcX = sx + amplitudeX * sin(2.0 * M_PI * sy * frequencyX / hImage);
				break;

			case 2: // ��ֱ����
				srcY = sy + amplitudeY * sin(2.0 * M_PI * sx * frequencyY / wImage);
				break;

			case 3: // ˫����
				srcX = sx + amplitudeX * sin(2.0 * M_PI * sy * frequencyX / hImage);
				srcY = sy + amplitudeY * sin(2.0 * M_PI * sx * frequencyY / wImage);
				break;

			case 4: // ������
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

			case 5: // Ť������
			{
				double phaseX = 2.0 * M_PI * sy * frequencyX / hImage;
				double phaseY = 2.0 * M_PI * sx * frequencyY / wImage;
				srcX = sx + amplitudeX * sin(phaseX + phaseY);
				srcY = sy + amplitudeY * cos(phaseX - phaseY);
			}
			break;
			}

			// �߽��������ӳ��
			if (srcX >= 0 && srcX < wImage - 1 && srcY >= 0 && srcY < hImage - 1)
			{
				// ʹ��˫���Բ�ֵ
				int x1 = (int)srcX;
				int y1 = (int)srcY;
				int x2 = x1 + 1;
				int y2 = y1 + 1;

				double fx = srcX - x1;
				double fy = srcY - y1;

				// ��ȡ�ĸ��ڽ�����
				unsigned char p11 = oImage[y1 * wImage + x1];
				unsigned char p12 = oImage[y2 * wImage + x1];
				unsigned char p21 = oImage[y1 * wImage + x2];
				unsigned char p22 = oImage[y2 * wImage + x2];

				// ˫���Բ�ֵ����
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
 *  ���˱任�ӿں���                                                                            *
 *                                                                                              *
 ************************************************************************************************/
void ImageWaveTransformation(char *oImage, char *nImage, int wImage, int hImage,
							 int waveType, double amplitude, double frequency)
{
	// ������֤�͵���
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

	// ���ݲ������͵�������
	switch (waveType)
	{
	case 1: // ˮƽ����
		amplitudeY = 0;
		break;
	case 2: // ��ֱ����
		amplitudeX = 0;
		break;
	case 3: // ˫����
		// ����Ĭ�ϲ���
		break;
	case 4: // ������
		frequencyY = 0;
		break;
	case 5: // Ť������
		frequencyX = frequency * 0.7;
		frequencyY = frequency * 1.3;
		break;
	}

	WaveTransformation(oImage, nImage, wImage, hImage,
					   amplitudeX, frequencyX, amplitudeY, frequencyY, waveType);
}
/************************************************************************************************
 *                                                                                              *
 *  ˫���˲�����                                                                           		  *
 *                                                                                              *
 ************************************************************************************************/

void BilateralFilterProcessing(char *oImage, char *nImage, int wImage, int hImage,
							   double sigmaSpace, double sigmaColor)
{
	int radius = 5; // �˲��뾶
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

					// ����ռ�Ȩ��
					double spatialWeight = exp(-(m * m + n * n) / (2 * sigmaSpace * sigmaSpace));

					// ������ɫȨ��
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
 *  ��̬ѧ�˲�                                                                                    *
 *                                                                                              *
 ************************************************************************************************/
// ��ʴ����
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
// ���Ͳ���
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
 *  ����Ӧ��ֵ�˲�                                                                           	   *
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
				int pixels[49]; // ���7x7����
				int count = 0;

				// �ռ�����������
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

				// ����
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

				// �׶�A
				if (zmed > zmin && zmed < zmax)
				{
					// �׶�B
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
 *  Ƶ��ͼ������ʵ��                                                                            *
 *                                                                                              *
 ************************************************************************************************/

// λ��ת����
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

// һάFFT�任
void FFT1D(double *realPart, double *imagPart, int n, bool inverse)
{
	double M_PI = 3.1415926;
	// ȷ��n��2����
	int bits = 0;
	int temp = n;
	while (temp > 1)
	{
		temp >>= 1;
		bits++;
	}

	// ���n����2���ݣ���Ҫ����ʹ����������
	if ((1 << bits) != n)
	{
		// ʹ��DFT�����2�������
		double *tempReal = new double[n];
		double *tempImag = new double[n];

		// ����ԭʼ����
		for (int i = 0; i < n; i++)
		{
			tempReal[i] = realPart[i];
			tempImag[i] = imagPart[i];
		}

		// DFT����
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

	// λ��ת����
	for (int i = 0; i < n; i++)
	{
		int j = ReverseBits(i, bits);
		if (i < j)
		{
			// ����ʵ�����鲿
			double tempReal = realPart[i];
			double tempImag = imagPart[i];
			realPart[i] = realPart[j];
			imagPart[i] = imagPart[j];
			realPart[j] = tempReal;
			imagPart[j] = tempImag;
		}
	}

	// Cooley-Tukey FFT�㷨
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

				// ������ת����
				double tempReal = wReal * wlenReal - wImag * wlenImag;
				wImag = wReal * wlenImag + wImag * wlenReal;
				wReal = tempReal;
			}
		}
	}

	// ��任��Ҫ��һ��
	if (inverse)
	{
		for (int i = 0; i < n; i++)
		{
			realPart[i] /= n;
			imagPart[i] /= n;
		}
	}
}

// ��άFFT�任
void FFT2D(double *realPart, double *imagPart, int width, int height, bool inverse)
{
	int i, j;
	// ��ÿһ�н���һάFFT
	double *tempReal = new double[width];
	double *tempImag = new double[width];

	for (i = 0; i < height; i++)
	{
		// ��ȡһ������
		for (j = 0; j < width; j++)
		{
			tempReal[j] = realPart[i * width + j];
			tempImag[j] = imagPart[i * width + j];
		}

		// ����һ�н���FFT
		FFT1D(tempReal, tempImag, width, inverse);

		// �����д��
		for (j = 0; j < width; j++)
		{
			realPart[i * width + j] = tempReal[j];
			imagPart[i * width + j] = tempImag[j];
		}
	}

	delete[] tempReal;
	delete[] tempImag;

	// ��ÿһ�н���һάFFT
	tempReal = new double[height];
	tempImag = new double[height];

	for (j = 0; j < width; j++)
	{
		// ��ȡһ������
		for (i = 0; i < height; i++)
		{
			tempReal[i] = realPart[i * width + j];
			tempImag[i] = imagPart[i * width + j];
		}

		// ����һ�н���FFT
		FFT1D(tempReal, tempImag, height, inverse);

		// �����д��
		for (i = 0; i < height; i++)
		{
			realPart[i * width + j] = tempReal[i];
			imagPart[i * width + j] = tempImag[i];
		}
	}

	delete[] tempReal;
	delete[] tempImag;
}

// ͼ��ת��Ϊ������ʽ
void ImageToComplex(char *image, double *realPart, double *imagPart, int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		realPart[i] = (double)static_cast<unsigned char>(image[i]);
		imagPart[i] = 0.0;
	}
}

// ����ת��Ϊͼ��
void ComplexToImage(double *realPart, double *imagPart, char *image, int width, int height)
{
	for (int i = 0; i < width * height; i++)
	{
		// ʹ��ʵ���ؽ�ͼ�񣬶����Ƿ���
		double value = realPart[i];
		// ������0-255��Χ��
		if (value > 255)
			value = 255;
		if (value < 0)
			value = 0;
		image[i] = (char)(unsigned char)(value + 0.5);
	}
}

// �����ͨ�˲���
void IdealLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double cutoffRadius = cutoffFreq * min(width, height) / 2;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// �����ͨ�˲�����������ڽ�ֹƵ��������
			if (distance > cutoffRadius)
			{
				realPart[v * width + u] = 0.0;
				imagPart[v * width + u] = 0.0;
			}
		}
	}
}
// ��˹��ͨ�˲���
void GaussianLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double sigma = cutoffFreq * min(width, height) / 4;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// ��˹��ͨ�˲���
			double H = exp(-(distance * distance) / (2 * sigma * sigma));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

// ������˹��ͨ�˲���
void ButterworthLowPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double D0 = cutoffFreq * min(width, height) / 2;
	int n = 2; // ������˹����

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// ������˹��ͨ�˲���
			double H = 1.0 / (1.0 + pow(distance / D0, 2 * n));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

void FrequencyDomainSmoothing(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType)
{
	int i = 0, row, col;
	// �����ڴ�
	double *realPart = new double[wImage * hImage];
	double *imagPart = new double[wImage * hImage];

	// 1. ͼ��ת��Ϊ����
	ImageToComplex(oImage, realPart, imagPart, wImage, hImage);
	// 1.5. Ƶ�����Ļ�Ԥ��������(-1)^(row+col)
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
	// 2. ����FFT
	FFT2D(realPart, imagPart, wImage, hImage, false);

	// 3. Ӧ��Ƶ���˲���
	switch (filterType)
	{
	case 0: // �����ͨ
		IdealLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 1: // ��˹��ͨ
		GaussianLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 2: // ������˹��ͨ
		ButterworthLowPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	}

	// 4. ����FFT
	FFT2D(realPart, imagPart, wImage, hImage, true);
	// 4.5. Ƶ�����Ļ������ٴγ���(-1)^(row+col)
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
	// 5. ����ת��Ϊͼ��
	ComplexToImage(realPart, imagPart, nImage, wImage, hImage);

	// �ͷ��ڴ�
	delete[] realPart;
	delete[] imagPart;
}
// ��ʾƵ��
void ShowFrequencySpectrum(double *realPart, double *imagPart, int width, int height, int xPos, int yPos)
{
	int i, j;
	char *spectrumImage = new char[width * height];

	// ���������
	double maxMagnitude = 0;
	for (i = 0; i < width * height; i++)
	{
		double magnitude = sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]);
		if (magnitude > maxMagnitude)
		{
			maxMagnitude = magnitude;
		}
	}

	// �����任����һ��
	for (i = 0; i < width * height; i++)
	{
		double magnitude = sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]);
		magnitude = log(1 + magnitude) / log(1 + maxMagnitude) * 255;
		spectrumImage[i] = (char)(unsigned char)magnitude;
	}

	// Ƶ�����Ļ���ʾ
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

// �����ͨ�˲���
void IdealHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double cutoffRadius = cutoffFreq * min(width, height) / 2;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// �����ͨ�˲���������С�ڽ�ֹƵ��������
			if (distance <= cutoffRadius)
			{
				realPart[v * width + u] = 0.0;
				imagPart[v * width + u] = 0.0;
			}
		}
	}
}

// ��˹��ͨ�˲���
void GaussianHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double sigma = cutoffFreq * min(width, height) / 4;

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// ��˹��ͨ�˲�����H = 1 - exp(-D?/(2��?))
			double H = 1.0 - exp(-(distance * distance) / (2.0 * sigma * sigma));

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}

// ������˹��ͨ�˲���
void ButterworthHighPassFilter(double *realPart, double *imagPart, int width, int height, double cutoffFreq)
{
	int centerX = width / 2;
	int centerY = height / 2;
	double D0 = cutoffFreq * min(width, height) / 2;
	int n = 2; // ������˹����

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			// ���㵽Ƶ�����ĵľ���
			double distance = sqrt((u - centerX) * (u - centerX) + (v - centerY) * (v - centerY));

			// ������˹��ͨ�˲�����H = 1 / (1 + (D0/D)^(2n))
			double H = 1.0 / (1.0 + pow(D0 / (distance + 1e-6), 2 * n)); // ��Сֵ�������

			realPart[v * width + u] *= H;
			imagPart[v * width + u] *= H;
		}
	}
}
// Ƶ���Ե���
void FrequencyDomainEdgeDetection(char *oImage, char *nImage, int wImage, int hImage, double cutoffFreq, int filterType)
{
	int i = 0;
	int row, col;
	// �����ڴ�
	double *realPart = new double[wImage * hImage];
	double *imagPart = new double[wImage * hImage];

	// 1. ͼ��ת��Ϊ����
	ImageToComplex(oImage, realPart, imagPart, wImage, hImage);

	// 1.5. Ƶ�����Ļ�Ԥ��������(-1)^(row+col)
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

	// 2. ����FFT
	FFT2D(realPart, imagPart, wImage, hImage, false);

	// 3. Ӧ��Ƶ���ͨ�˲���
	switch (filterType)
	{
	case 0: // �����ͨ
		IdealHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 1: // ��˹��ͨ
		GaussianHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	case 2: // ������˹��ͨ
		ButterworthHighPassFilter(realPart, imagPart, wImage, hImage, cutoffFreq);
		break;
	}

	// 4. ����FFT
	FFT2D(realPart, imagPart, wImage, hImage, true);

	// 4.5. Ƶ�����Ļ������ٴγ���(-1)^(row+col)
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

	// 5. ����ת��Ϊͼ��
	ComplexToImage(realPart, imagPart, nImage, wImage, hImage);

	// �ͷ��ڴ�
	delete[] realPart;
	delete[] imagPart;
}

// ����PSNR
double CalculatePSNR(char *original, char *processed, int width, int height)
{
	double mse = 0.0;
	int totalPixels = width * height;

	// ���������� (MSE)
	for (int i = 0; i < totalPixels; i++)
	{
		double diff = (double)((unsigned char)original[i]) - (double)((unsigned char)processed[i]);
		mse += diff * diff;
	}
	mse /= totalPixels;

	// ����������
	if (mse == 0.0)
		return 100.0; // ��ȫ��ͬ��ͼ��

	// ����PSNR
	double psnr = 10.0 * log10((255.0 * 255.0) / mse);
	return psnr;
}

// ����MSE
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

// ����SSIM
double CalculateSSIM(char *original, char *processed, int width, int height)
{
	int i;
	double c1 = 6.5025;	 // (0.01 * 255)^2
	double c2 = 58.5225; // (0.03 * 255)^2

	// �����ֵ
	double mean1 = 0.0, mean2 = 0.0;
	int totalPixels = width * height;

	for (i = 0; i < totalPixels; i++)
	{
		mean1 += (unsigned char)original[i];
		mean2 += (unsigned char)processed[i];
	}
	mean1 /= totalPixels;
	mean2 /= totalPixels;

	// ���㷽���Э����
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

	// ����SSIM
	double numerator = (2 * mean1 * mean2 + c1) * (2 * cov + c2);
	double denominator = (mean1 * mean1 + mean2 * mean2 + c1) * (var1 + var2 + c2);

	return numerator / denominator;
}
// ����ͼ�������� (�����ݶ�)
double CalculateSharpness(char *image, int width, int height)
{
	double sharpness = 0.0;

	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			// Sobel���Ӽ����ݶ�
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
// ����ͼ��Աȶ�
double CalculateContrast(char *image, int width, int height)
{
	int i;
	double mean = 0.0;
	int totalPixels = width * height;

	// �����ֵ
	for (i = 0; i < totalPixels; i++)
	{
		mean += (unsigned char)image[i];
	}
	mean /= totalPixels;

	// �����׼��
	double variance = 0.0;
	for (i = 0; i < totalPixels; i++)
	{
		double diff = (unsigned char)image[i] - mean;
		variance += diff * diff;
	}
	variance /= totalPixels;

	return sqrt(variance);
}
// �ۺ�ͼ����������
void ComprehensiveQualityAssessment(char *original, char *processed, int width, int height)
{
	char message[1000];

	double psnr = CalculatePSNR(original, processed, width, height);
	double mse = CalculateMSE(original, processed, width, height);
	double ssim = CalculateSSIM(original, processed, width, height);
	double sharpness = CalculateSharpness(processed, width, height);
	double contrast = CalculateContrast(processed, width, height);

	sprintf(message,
			"ͼ�������������:\n\n"
			"PSNR: %.2f dB\n"
			"MSE: %.2f\n"
			"SSIM: %.4f\n"
			"������: %.2f\n"
			"�Աȶ�: %.2f\n\n"
			"��������: %s",
			psnr, mse, ssim, sharpness, contrast,
			(psnr > 30 && ssim > 0.9) ? "����" : (psnr > 25 && ssim > 0.8) ? "����"
											 : (psnr > 20 && ssim > 0.6)   ? "һ��"
																		   : "�ϲ�");

	MessageBoxEx(hWind, message, "ͼ�������������",
				 MB_OK | MB_ICONINFORMATION | MB_TOPMOST,
				 MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
}

// ����Ӧ��ֵ�ָ�
void AdaptiveThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage, int blockSize)
{
	int i;
	int halfBlock = blockSize / 2;

	for (i = 0; i < hImage; i++)
	{
		for (int j = 0; j < wImage; j++)
		{
			// ����ֲ������ƽ��ֵ
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

			// ����Ӧ��ֵ�ָ�
			if (currentPixel > localMean - 10) // �ɵ�����ƫ����
				nImage[i * wImage + j] = (BYTE)255;
			else
				nImage[i * wImage + j] = 0;
		}
	}
}

// Otsu�Զ���ֵ�ָ�
void OtsuThresholdSegmentation(char *oImage, char *nImage, int wImage, int hImage)
{
	int i;
	// ����ֱ��ͼ
	int histogram[256] = {0};
	int totalPixels = wImage * hImage;

	for (i = 0; i < totalPixels; i++)
	{
		histogram[(unsigned char)oImage[i]]++;
	}

	// ���������ֵ
	double maxVariance = 0;
	int bestThreshold = 0;

	for (int t = 0; t < 256; t++)
	{
		// ����ǰ���ͱ�����Ȩ��
		double w0 = 0, w1 = 0;
		for (i = 0; i <= t; i++)
			w0 += histogram[i];
		for (i = t + 1; i < 256; i++)
			w1 += histogram[i];

		if (w0 == 0 || w1 == 0)
			continue;

		// ����ǰ���ͱ�����ƽ���Ҷ�
		double u0 = 0, u1 = 0;
		for (i = 0; i <= t; i++)
			u0 += i * histogram[i];
		for (i = t + 1; i < 256; i++)
			u1 += i * histogram[i];
		u0 /= w0;
		u1 /= w1;

		// ������䷽�� (��׼Otsu��ʽ)
		w0 /= totalPixels;
		w1 /= totalPixels;
		double variance = w0 * w1 * (u0 - u1) * (u0 - u1);

		if (variance > maxVariance)
		{
			maxVariance = variance;
			bestThreshold = t;
		}
	}

	// Ӧ�������ֵ
	for (i = 0; i < totalPixels; i++)
	{
		if ((unsigned char)oImage[i] > bestThreshold)
			nImage[i] = (BYTE)255;
		else
			nImage[i] = 0;
	}
}
// ʹ�ö��н�����������
struct Point
{
	int x, y;
};
// ���������ָ�
void RegionGrowingSegmentation(char *oImage, char *nImage, int wImage, int hImage, int seedX, int seedY, int threshold)
{
	int i;
	// ��ʼ�����ͼ��
	memset(nImage, (BYTE)255, wImage * hImage);

	// �������ʱ������
	bool *visited = new bool[wImage * hImage];
	memset(visited, false, wImage * hImage * sizeof(bool));

	Point *queue = new Point[wImage * hImage];
	int front = 0, rear = 0;

	// ���ӵ�
	if (seedX >= 0 && seedX < wImage && seedY >= 0 && seedY < hImage)
	{
		Point seedPoint;
		seedPoint.x = seedX;
		seedPoint.y = seedY;
		queue[rear++] = seedPoint;
		visited[seedY * wImage + seedX] = true;
		nImage[seedY * wImage + seedX] = 0;

		int seedValue = (unsigned char)oImage[seedY * wImage + seedX];

		// 8��ͨ��������
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

					// �ж��Ƿ�������������
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

// �߽�׷�٣�Moore����׷�٣�
void BoundaryTracing(char *oImage, char *nImage, int wImage, int hImage, int threshold)
{
	int i;
	// ���Ƚ��ж�ֵ��
	char *binaryImage = new char[wImage * hImage];
	for (i = 0; i < wImage * hImage; i++)
	{
		if ((unsigned char)oImage[i] > threshold)
			binaryImage[i] = (BYTE)255;
		else
			binaryImage[i] = 0;
	}

	// ��ʼ�����ͼ��
	memset(nImage, 0, wImage * hImage);

	// Moore������8��ͨ��
	int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
	int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

	// Ѱ�ұ߽�㲢׷��
	for (i = 1; i < hImage - 1; i++)
	{
		for (int j = 1; j < wImage - 1; j++)
		{
			if ((unsigned char)binaryImage[i * wImage + j] == 255)
			{
				// ����Ƿ�Ϊ�߽��
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

// ��ˮ��ָ�򻯰汾��
void WatershedSegmentation(char *oImage, char *nImage, int wImage, int hImage)
{
	int i, j;
	// �����ݶ�ͼ��
	char *gradientImage = new char[wImage * hImage];

	for (i = 1; i < hImage - 1; i++)
	{
		for (j = 1; j < wImage - 1; j++)
		{
			// Sobel���Ӽ����ݶ�
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

	// �򻯵ķ�ˮ�룺�����ݶ���ֵ�ָ�
	int gradientThreshold = 50;
	for (i = 0; i < wImage * hImage; i++)
	{
		if ((unsigned char)gradientImage[i] > gradientThreshold)
			nImage[i] = (BYTE)255; // �߽�
		else
			nImage[i] = 0; // �����ڲ�
	}

	delete[] gradientImage;
}