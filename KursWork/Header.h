#pragma once
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl/gl.h>
#include <string.h>
#include <stb_easy_font.h>
#include <math.h>
#define PI 3.1415926f

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

/* Массивы цветов холста */
float colorCanvasBG[] = { 0.9, 0.9, 0.8 };
float colorCanvasFrame[] = { 0.8, 0.8, 0.7 };

/* Переменные */
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

	/* Регистрируем класс окна */
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

/* Функции отрисовки фигур */
/*=========================*/
/* отрисовка окружности*/
void drawCircle(float cx, float cy, float radius)
{
	glColor3f(0.0, 0.0, 0.0); // задаём цвет для окружности
	glBegin(GL_LINE_LOOP); // начинаем отрисовку с соединением точек
	for (float ii = 0; ii < 100; ii++)
	{
		float theta = 2.0f * PI * (ii / 100); // получаем угол

		float x = radius * cosf(theta);// считаем X
		float y = radius * sinf(theta);// считаем Y

		glVertex2f(x + cx, y + cy);// строим точку
	}
	glEnd();
}

/* отрисовка точки */
void drawPoint(float cx, float cy, float angle, float radius, float r, float g, float b)
{
	float x = radius * cos(angle * (PI / 180)); // перевод в декартовые
	float y = radius * sin(angle * (PI / 180)); // координаты
	glPointSize(10); // задаём размер точки
	glBegin(GL_POINTS); // начинаем отрисовку с режимом "точек"
	glColor3f(r, g, b);   
	glVertex2f(x + cx, y + cy); // задаём цвет и рисуем точку с учётом центра
	glEnd();
}

/* отрисовка прямоугольника(для холста) */
void drawPryamoygol(float cx, float cy)
{
	glLineWidth(3); // выбираем толщину линии
	glColor3f(0.9, 0.9, 0.8); // устанавливаем цвет, получая на вход RGB-массив
	glBegin(GL_TRIANGLE_FAN); // начинаем отрисовку с переданным через параметр режимом
	glVertex2f(0.9 + cx, 0.85 + cy);     //--------------
	glVertex2f(-0.9 + cx, 0.85 + cy);    // Рисуем вершины прямоугольника
	glVertex2f(-0.9 + cx, -0.85 + cy);   // с учётом полученных ширины и высоты
	glVertex2f(0.9 + cx, -0.85 + cy);    //--------------
	glEnd();
}

/* Функция для вывода текста */
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


/* Структура кнопки */
typedef struct
{
	char name[20];   //name - название кнопки,
	char text[30];	 //tex - текст на кнопке,
	float vert[8];   //vert - массив координат вершин, 
	BOOL hover;      //hover - проверка наведения
}TButton;

/* Массив кнопок */
TButton btn[] = {
	{"R-","(-) Radius Value (-)", {350,520, 580,520, 580,550, 350,550}, FALSE},
	{"R+","(+) Radius Value (+)", {350,480, 580,480, 580,510, 350,510}, FALSE},
	{"S-","(-) Rotate Speed (-)", {20,520, 250,520, 250,550, 20,550}, FALSE},
	{"S+","(+) Rotate Speed (+) ", {20,480, 250,480, 250,510, 20,510}, FALSE}
};

int btnCnt = sizeof(btn) / sizeof(btn[0]); // Кол-во кнопок в массиве кнопок

/* Функция отрисовки кнопки*/
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

/* Функция нажатия на кнопку */
BOOL PointInButton(int x, int y, TButton btn)
{
	return (x > btn.vert[0]) && (x < btn.vert[4]) &&
		   (y > btn.vert[1]) && (y < btn.vert[5]);
}

/* Функция отрисовки меню */
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


/* Функция подключения OpenGL */
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

/* Функция отключения OpenGL */
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}