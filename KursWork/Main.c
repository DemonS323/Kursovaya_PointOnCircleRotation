#include "Header.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	CreateWindowClass(hInstance);

	/* Создаём основное окно */
	hwnd = CreateWindowEx(0,
		"GLSample",
		"OpenGL Sample",
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* Подключение OpenGL к окну */
	EnableOpenGL(hwnd, &hDC, &hRC);

	/* Цикл до выхода из программы */
	while (!bQuit)
	{
		/* Проверка сообщений от пользователя */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* Обработка и отправка сообщений */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* Код OpenGL анимации */

			glClearColor(0.87f, 0.72f, 0.6f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			drawPryamoygol(0.0,0.15);
			drawCircle(0.0, 0.15, radius);
			drawPoint(0.0, 0.15, 0, 0, 0, 0, 0);
			drawPoint(0.0, 0.15, angle, radius, 1, 0, 0);

			ShowMenu();

			SwapBuffers(hDC);

			angle += rotate_speed;
			Sleep(1);
		}
	}

	/* Отключение OpenGL*/
	DisableOpenGL(hwnd, hDC, hRC);

	/* Удаление окна*/
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		glViewport(0, 0, width, height);
		glLoadIdentity();
		float k = width / (float)height;
		glOrtho(-k, k, -1, 1, -1, 1);
		break;

	case WM_LBUTTONDOWN:
		for (int i = 0; i <btnCnt; i++)
			if (PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i]))
			{
				if (strcmp(btn[i].name, "S+") == 0)
				{
					if (rotate_speed <= max_speed) { rotate_speed += 0.1; }
				}
				if (strcmp(btn[i].name, "S-") == 0)
				{
					if (rotate_speed > 0) { rotate_speed -= 0.1; }
					if (rotate_speed < 0) { rotate_speed = 0; }
				}
				if (strcmp(btn[i].name, "R+") == 0)
				{
					if (radius <= max_radius) { radius += 0.01; }
				}
				if (strcmp(btn[i].name, "R-") == 0)
				{
					if (radius >= min_radius) { radius -= 0.01; }
				}
			}
		break;

	case WM_MOUSEMOVE:
		for (int i = 0; i < btnCnt; i++)
			btn[i].hover = PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i]);
		break;

	case WM_DESTROY:
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}