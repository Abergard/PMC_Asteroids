#include <windows.h>
//#include <gl/gl.h>
#include <gl\GLU.h>
#include <stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

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

class VEHICLE {
public:
  GLfloat posX;
  GLfloat posY;
  static GLfloat vector;
  GLfloat angle;
};

GLfloat VEHICLE::vector = 0;

class ASTEROID : public VEHICLE {
public:
  GLfloat my_vector;
  GLfloat rotation;
  bool IsExist;

  ASTEROID() {
    IsExist = false;
    this->rotation = 0;
    this->posX = 0;
    this->posY = 0;
    this->angle = 0;
    this->my_vector = vector;
    RandPosition();
  }

  void RandPosition(void) {
    int site = rand() % 2800;

    if (site < 800) {
      this->posX = site - 400;
      this->posY = 300 + 50;
      this->angle = rand() % 180 + 180;
    } else if (site < 1400) {
      this->posX = 400 + 50;
      this->posY = site - 1100;
      this->angle = rand() % 180 + 90;
    } else if (site < 2200) {
      this->posX = site - 1800;
      this->posY = -300 - 50;
      this->angle = rand() % 180;
    } else {
      this->posX = -400 - 50;
      this->posY = site - 2500;
      this->angle = rand() % 180 - 90;
    }

    int speed = rand() % 3 + 1;

    this->my_vector = vector * speed;
  }

  void Draw(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    // only for change
    glTranslatef(posX, posY, 0);
    glRotatef(rotation, 0, 0, 1);
    //

    glBegin(GL_POLYGON);

    glVertex2f(+50.0f, -10.0f);
    glVertex2f(+20.0f, -50.0f);
    glVertex2f(-5.0f, -50.0f);
    glVertex2f(-5.0f, -25.0f);
    glVertex2f(-30.0f, -50.0f);
    glVertex2f(-50.0f, -10.0f);
    glVertex2f(-25.0f, 0.0f);
    glVertex2f(-50.0f, +10.0f);
    glVertex2f(-15.0f, +45.0f);
    glVertex2f(+20.0f, +45.0f);

    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
  }

  bool Update(void) {

    this->posX += cos(angle * M_PI / 180.0f) * my_vector;
    this->posY += sin(angle * M_PI / 180.0f) * my_vector;

    if (this->posX > 400 + 50 || this->posX < -400 - 50)
      return false;

    if (this->posY > 300 + 50 || this->posY < -300 - 50)
      return false;

    rotation += 1;

    if (rotation > 360.0f)
      rotation -= 360.0f;

    return true;
  }
};

class SHIP_CLASS : public VEHICLE {
public:
  bool IsDestroy;
  int destroyCount;
  GLfloat color;

  int slower;

  SHIP_CLASS(void) {
    this->slower = 0;
    this->angle = 0;
    this->vector = 2;
    this->posX = 0;
    this->posY = 0;
    this->destroyCount = 0;
    this->IsDestroy = false;
    this->color = 1.0f;
  }

  void Destroy(void) {
    if (slower == 5) {
      this->IsDestroy = true;
      if (this->color > 0)
        this->color -= 0.2f;

      this->destroyCount = (this->destroyCount + 1) % 6;

      if (this->destroyCount == 0) {
        this->IsDestroy = false;
        this->angle = 0;
        this->posX = 0;
        this->posY = 0;
        this->destroyCount = 0;
        this->color = 1.0f;
      }
    }
    slower = (slower + 1) % 6;
  }

  void Update(bool ShiftBackMode) {
    if (IsDestroy == false) {
      GLfloat MODE = 1.0f;

      if (ShiftBackMode)
        MODE = -1.0f;

      this->posX += MODE * cos(angle * M_PI / 180.0f) * vector;
      this->posY += MODE * sin(angle * M_PI / 180.0f) * vector;

      if (this->posX > 400 || this->posX < -400)
        this->posX *= -1;

      if (this->posY > 300 || this->posY < -300)
        this->posY *= -1;
    }
  }

  void Draw(void) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    // only for change
    glTranslatef(posX, posY, 0);
    glRotatef(angle, 0, 0, 1);
    //

    glBegin(GL_POLYGON);
    glColor3f(this->color, this->color, this->color);
    glVertex2f(+15.0f, 0.0f);
    glVertex2f(-15.0f, -10.0f);
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(-15.0f, +10.0f);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glPopMatrix();
  }
};

LPSTR NazwaKlasy = "GL tutorial";
MSG msg;
HWND g_hWnd;
HDC hDC;
HGLRC hRC;
BOOL quit = FALSE;
SHIP_CLASS racket;
ASTEROID asteroid;
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
