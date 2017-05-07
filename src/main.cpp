#include <windows.h>
//#include <gl/gl.h>
#include <gl\GLU.h>
#include <stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vehicle.hpp"
GLfloat Vehicle::vector = 0;
#include "Ship.hpp"
#include "Asteroid.hpp"

// declaration / globals
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDCLASSEX wc;

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void Display();

bool ShiftBackMode = false;
int TimeCount = 0;

int asteroidBuffer = 0;
bool IsAsteroid = false;

float angle = 2.0f;

void Collision(GLfloat first_x, GLfloat first_y, GLfloat second_x,
               GLfloat second_y);

struct ROZMIAR {
  int x;
  int y;
} OKNO = {800, 600};

const char* NazwaKlasy = "GL tutorial";
MSG msg;
HWND g_hWnd;
HDC hDC;
HGLRC hRC;
BOOL quit = FALSE;
Ship racket;
Asteroid asteroid;
int racket_spedd = 0;

const WORD ID_TIMER = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = NazwaKlasy;
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Rejestracja okna nie powiodla sie!", "Register Error",
               MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }

  // WS_OVERLAPPEDWINDOW WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX / WS_CAPTION |
  // WS_POPUPWINDOW
  HWND hWnd = CreateWindowEx(
      WS_EX_CLIENTEDGE, NazwaKlasy, "Zad 2 - Lukasz Zawistowicz",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, OKNO.x,
      OKNO.y, NULL, NULL, hInstance, NULL);

  if (hWnd == NULL) {
    MessageBox(NULL, "Proba utworzenia okna nie powiodla sie!", "Create Error",
               MB_ICONEXCLAMATION);
    return 1;
  }

  // timer
  if (SetTimer(hWnd, ID_TIMER, 20, NULL) == 0)
    MessageBox(hWnd, "Nie mozna utworzyc timera!", "Blad", MB_ICONSTOP);

  // enable OpenGL for the window
  EnableOpenGL(hWnd, &hDC, &hRC);

  // program main loop
  while (!quit) {
    // check for messages
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      // handle or dispatch messages
      if (msg.message == WM_QUIT) {
        quit = TRUE;
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    } else {
      Display();
    }
  }
  // shutdown OpenGL
  DisableOpenGL(hWnd, hDC, hRC);
  // destroy the window explicitly
  DestroyWindow(hWnd);
  return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {

  case WM_CLOSE:
    DestroyWindow(hWnd);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  case WM_KEYDOWN: {
    switch (wParam) {
    case VK_MENU:
      break;

    case VK_ESCAPE:
      PostQuitMessage(0);
      break;

    case VK_LEFT:
      racket.angle += angle;

      if (racket.angle > 360.0f)
        racket.angle -= 360.0f;
      break;

    case VK_RIGHT:
      if (racket.angle <= 0.0f)
        racket.angle = 360.0f - angle;
      else
        racket.angle -= angle;
      break;

    case VK_UP:
      ShiftBackMode = false;
      break;

    case VK_DOWN:
      ShiftBackMode = true;
      break;

    case VK_SPACE:
      asteroid.RandPosition();
      break;

    case VK_TAB:
      racket.IsDestroy = true;
      break;
    }
  }
    return 0;

  case WM_TIMER: {
    if (racket.IsDestroy == false) {
      Collision(racket.posX, racket.posY, asteroid.posX, asteroid.posY);

      if (ShiftBackMode == false) // && ( TimeCount % 2 == 0)
      {
        racket.vector = 2;
        racket.Update(ShiftBackMode);
        //++TEST1;
      }

      if (ShiftBackMode == true) // && ( TimeCount % 4 == 0)
      {
        racket.vector = 1;
        racket.Update(ShiftBackMode);
        // TimeCount = 0;
        //++TEST2;
      }
      //++TimeCount;
    } else
      racket.Destroy();

    if (IsAsteroid == false) {
      if (asteroidBuffer >= 100) {
        asteroidBuffer = 0;
        asteroid.RandPosition();
        IsAsteroid = true;
      } else
        ++asteroidBuffer;
    } else // IsAsteroid == true
    {
      IsAsteroid = asteroid.Update();
    }
  }
    return 0;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}

// Enable OpenGL
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
  PIXELFORMATDESCRIPTOR pfd;
  int format;

  // get the device context (DC)
  *hDC = GetDC(hWnd);

  // set the pixel format for the DC
  ZeroMemory(&pfd, sizeof(pfd));

  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;

  format = ChoosePixelFormat(*hDC, &pfd);
  SetPixelFormat(*hDC, format, &pfd);

  // create and enable the render context (RC)
  *hRC = wglCreateContext(*hDC);
  wglMakeCurrent(*hDC, *hRC);

  glPointSize(2.0f);
  srand(time(NULL));
}

// Disable OpenGL
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);
}

void Display() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-400, 400, -300, 300, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  racket.Draw();

  if (IsAsteroid == true)
    asteroid.Draw();

  SwapBuffers(hDC);
}

void Collision(GLfloat first_x, GLfloat first_y, GLfloat second_x,
               GLfloat second_y) {
  float tmp_x = first_x - second_x;
  float tmp_y = first_y - second_y;

  if (tmp_x < 0)
    tmp_x *= -1;

  if (tmp_y < 0)
    tmp_y *= -1;

  float distance = sqrt((tmp_x * tmp_x) + (tmp_y * tmp_y));

  if (distance < 54.0f)
    racket.IsDestroy = true;
}
