#pragma once
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl/gl.h>
#include <string.h>
#include <stb_easy_font.h>
#include <math.h>
#define PI 3.1415926f

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

/* ������� ������ ������ */
float colorCanvasBG[] = { 0.9, 0.9, 0.8 };
float colorCanvasFrame[] = { 0.8, 0.8, 0.7 };

/* ���������� */
float width, height;
float radius = 0.5;
float rotate_speed = 1;
float angle = 0.0;

HWND hwnd;
HDC hDC;
HGLRC hRC;
MSG msg;
BOOL bQuit = FALSE;

void CreateWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	/* ������������ ����� ���� */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
		return 0;
}

/* ������� ��������� ����� */
/*=========================*/
/* ��������� ����������*/
void drawCircle(float cx, float cy, float radius)
{
	glColor3f(0.0, 0.0, 0.0); // ����� ���� ��� ����������
	glBegin(GL_LINE_LOOP); // �������� ��������� � ����������� �����
	for (float ii = 0; ii < 100; ii++)
	{
		float theta = 2.0f * PI * (ii / 100); // �������� ����

		float x = radius * cosf(theta);// ������� X
		float y = radius * sinf(theta);// ������� Y

		glVertex2f(x + cx, y + cy);// ������ �����
	}
	glEnd();
}

/* ��������� ����� */
void drawPoint(float cx, float cy, float angle, float radius, float r, float g, float b)
{
	float x = radius * cos(angle * (PI / 180)); // ������� � ����������
	float y = radius * sin(angle * (PI / 180)); // ����������
	glPointSize(10); // ����� ������ �����
	glBegin(GL_POINTS); // �������� ��������� � ������� "�����"
	glColor3f(r, g, b);   
	glVertex2f(x + cx, y + cy); // ����� ���� � ������ ����� � ������ ������
	glEnd();
}

/* ��������� ��������������(��� ������) */
void drawPryamoygol(float cx, float cy)
{
	glLineWidth(3); // �������� ������� �����
	glColor3f(0.9, 0.9, 0.8); // ������������� ����, ������� �� ���� RGB-������
	glBegin(GL_TRIANGLE_FAN); // �������� ��������� � ���������� ����� �������� �������
	glVertex2f(0.9 + cx, 0.85 + cy);     //--------------
	glVertex2f(-0.9 + cx, 0.85 + cy);    // ������ ������� ��������������
	glVertex2f(-0.9 + cx, -0.85 + cy);   // � ������ ���������� ������ � ������
	glVertex2f(0.9 + cx, -0.85 + cy);    //--------------
	glEnd();
}

/* ������� ��� ������ ������ */
void print_string(float x, float y, char* text, float r, float g, float b)
{
	static char buffer[99999]; // ~500 chars
	int num_quads;

	num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

	glColor3f(r, g, b);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 16, buffer);
	glDrawArrays(GL_QUADS, 0, num_quads * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}


/* ��������� ������ */
typedef struct
{
	char name[20];   //name - �������� ������,
	char text[30];	 //tex - ����� �� ������,
	float vert[8];   //vert - ������ ��������� ������, 
	BOOL hover;      //hover - �������� ���������
}TButton;

/* ������ ������ */
TButton btn[] = {
	{"R-","(-) Radius Value (-)", {350,520, 580,520, 580,550, 350,550}, FALSE},
	{"R+","(+) Radius Value (+)", {350,480, 580,480, 580,510, 350,510}, FALSE},
	{"S-","(-) Rotate Speed (-)", {20,520, 250,520, 250,550, 20,550}, FALSE},
	{"S+","(+) Rotate Speed (+) ", {20,480, 250,480, 250,510, 20,510}, FALSE}
};

int btnCnt = sizeof(btn) / sizeof(btn[0]); // ���-�� ������ � ������� ������

/* ������� ��������� ������*/
void TButton_Show(TButton btn)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	if (btn.hover) glColor3f(0.4, 0.3, 0.2);
	else glColor3f(0.3, 0.2, 0.1);
	glVertexPointer(2, GL_FLOAT, 0, btn.vert);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();
	glTranslatef(btn.vert[0], btn.vert[1], 0);
	glScalef(2, 2, 2);
	print_string(3, 3, btn.text, 0, 0, 0);
	glPopMatrix();
}

/* ������� ������� �� ������ */
BOOL PointInButton(int x, int y, TButton btn)
{
	return (x > btn.vert[0]) && (x < btn.vert[4]) &&
		   (y > btn.vert[1]) && (y < btn.vert[5]);
}

/* ������� ��������� ���� */
void ShowMenu()
{
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	for (int i = 0; i < btnCnt; i++)
	{
		TButton_Show(btn[i]);
	}
	glPopMatrix();
}


/* ������� ����������� OpenGL */
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

/* ������� ���������� OpenGL */
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}