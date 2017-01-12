#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include "obj.h"

#define WINDOW_CLASS_NAME 	L"OPENGL_TEMPLATE"
#define W_WIDTH 			512
#define W_HEIGHT 			512

#define COLOR_BITS          24
#define DEPTH_BITS          32
#define _NEARZ 				0.1f
#define _FARZ 				500.0f

using namespace std;

bool _keys[256];//����� ������� �������. ���� _keys[i] == true, �� ������� � ����� i ������

HDC hDC;//��������� �� �������� ���������� ������ ������ ����
HGLRC hRC;//��������� �� �������� �������

//�������-���������� ��������� Windows ��� ������ ����. 
//��� ���� ������ ���� ���������, ������� Windows �������� �����
	//(������ �������, ���� ��������, �������� ������ ���� � �.�.)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��������� ������� ������� ��� ������ ����
void SetDCPixelFormat(HDC hDC);
//��������� ������������� OpenGL
void InitGL();
//������� ��������� ��������� ���������� ������� ������ � ������ ���� � �������� OpenGL,
	//� ������������ � ����������� �������(width) � �������(height) ����
void ReSizeGLScene(GLsizei width, GLsizei height);

void DrawPolygon(vector<glm::vec3> &verteces, vector<int> &indeces);

float CalculateFPS();

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow){

    vector<glm::vec3> vertices;
	vector<int> indeces;
	loadObj("teapot.obj", &vertices, &indeces);
    //����� ������������ ���� ����
	WNDCLASSEX winClass;
	//��������� �� ����
    HWND hWindow;
    //����������, � ������� �� ����� ��������� ��������� ���������� ���� �� Windows
    	//� ���������� �� �� ��������� ����� ������� WindowProc
    MSG message;
    
    //����������� ��������� ������������ ����(��������� ��������� ������ ����)
    winClass.cbSize = sizeof(WNDCLASSEX);//����� ������ ��������� ������ ����
    winClass.style = CS_OWNDC;
    winClass.lpfnWndProc = WindowProc;//��������� �� �������-���������� ��������� ����
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = hInstance;
    //������
    winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //������
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //���� ����, ��� OpenGL ���������� �������� �� �����
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    //���� ��� �� �����
    winClass.lpszMenuName = NULL;
    //��� ������ ����(�� ���� �� ����� ��������� �� ��������� ���� �����)
    winClass.lpszClassName = (LPCWSTR)WINDOW_CLASS_NAME;
    winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    //������������ ��������� ����� ���� � �������
    if (!RegisterClassEx(&winClass)){
    	MessageBox(NULL, L"Can''t create win context!", L"Error", MB_ICONERROR | MB_OK);
    	return 0;
    }
    hWindow = CreateWindowEx(0,
                          WINDOW_CLASS_NAME,//��� ���������� ���� ������ ����
                          WINDOW_CLASS_NAME,//��� ������������ ����(����� �� ��������� � ������ ������)
                          WS_OVERLAPPEDWINDOW,//����� ����(������ MSDN ��� ����� ��������� ����������)
                          CW_USEDEFAULT,//���������� x �������� ������ ���� ����
                          CW_USEDEFAULT,//���������� y �������� ������ ���� ����
                          W_WIDTH,//������ ����
                          W_HEIGHT,//������ ����
                          NULL,//�������� ����(�� ����� ��� ���, ��������� null)
                          NULL,//��������� �� ����(��� �� �����)
                          hInstance,
                          NULL);
    //���������� ����
    ShowWindow(hWindow, nCmdShow);
    
    double xangle = 0.0, yangle = 0.0;
    bool bQuit = false;
    //�������� ���� ���������
    while (!bQuit){
    	//���� ���� ������ ��������� �� Windows
    	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
            //�� ���������� ���
            if (message.message == WM_QUIT){
                bQuit = TRUE;
            }
            else{
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        else{
        	if(_keys['W']){
        		xangle += 0.1;
        	}
        	
        	if(_keys['S']){
        		xangle -= 0.1;
        	}
        	
        	if(_keys['A']){
        		yangle += 0.1;
        	}
        	
        	if(_keys['D']){
        		yangle -= 0.1;
        	}
        	
        	//����� ��������� �������� ��������� ����� ���������
        	glClearColor(0 / 255.0, 0 / 255.0, 255.0 / 255.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glLoadIdentity();
			gluLookAt(	0, 0, 10, //��������� ������ � ������������
						0, 0, 0, //����� � ������� ������� ������
						0.0, 1, 0.0 //������ ��� Y ������
					);
			
			glPushMatrix();
			glRotatef(xangle, 1.0, 0.0, 0.0);
			glRotatef(yangle, 0.0, 1.0, 0.0);
			
			glPushMatrix();
			glTranslatef(0, 0.5, 0);
			glColor3f(0.6, 1.0, 0.6);
			//glBegin(GL_TRIANGLES);
			//	glVertex3f(-1.0, 1.0, -5.0);
			//	glVertex3f(-1.0, -1.0, -5.0);
			//	glVertex3f(1.0, -1.0, -5.0);
			//glEnd();
			glPopMatrix();
			
			//glColor4f(1.0, 0.6, 0.6, 0.5);
			//glBegin(GL_TRIANGLES); //GL_QUADS
			//	glVertex3f(-1.0, 1.0, 0.0);
			//	glVertex3f(-1.0, -1.0, 0.0);
			//	glVertex3f(1.0, -1.0, 0.0);
			// glEnd();
			DrawPolygon(vertices, indeces);
			glPopMatrix();
			
			float fps = CalculateFPS();
			if (fps > 0) { // ������ ���� ������������
				// ��������� ������ � FPS ���� "FPS = 101"
				// � ����� �++ :)
				char str_fps[20];
				itoa(int(fps), str_fps, 10);
				wchar_t wcstr_fps[20];
				mbstowcs(wcstr_fps, str_fps, strlen(str_fps)+1);
				wchar_t str[40] = L"FPS = "; 
				wcsncat(str, wcstr_fps, 10);

				// ������ ��������� ����
				SetWindowText(hWindow, str);
			}

			//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size()*3, &vertices[0], GL_STATIC_DRAW);      //�������
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(int),&indeces[0], GL_STATIC_DRAW); //�������
			//glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		    SwapBuffers(hDC);
        }
    }
    return 0;
}

void DrawPolygon(vector<glm::vec3> &vertices, vector<int> &indeces){

	glColor4f(1.0, 0.6, 0.6, 1);
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_POINT);
	glEnable(GL_LIGHTING);
	for (int i=0, maxi=indeces.size()/3; i<maxi; i++){
		glBegin(GL_TRIANGLES); //GL_QUADS
				glm::vec3 a, b, c;
				a = vertices[indeces[i*3]-1];
				b = vertices[indeces[i*3+1]-1];
				c = vertices[indeces[i*3+2]-1];
				glVertex3f(a.x, a.y, a.z);
				glVertex3f(b.x, b.y, b.z);
				glVertex3f(c.x, c.y, c.z);
		glEnd();
	}
}

float CalculateFPS()
{
    //���� �� �������� ��������� ��������� ����������, �.�. �����, ����� ��� ��������� ���
    //�������� ����� ���������� ������ �-��. �� ����� �� ������� �� �����������, �� ��� �����
    //��������.
	//����������� -1, ���� �� ���� ��������������, ����� �������� fps

    static float framesPerSecond = 0.0f;    //���� ���
    static float lastTime = timeGetTime() * 0.001f;          //��� �������� �����, ��������� � ���������� �����
    static char strFrameRate[50] = {0};     //������ ��� ������

    //��� �� �������� ������� tick count � �������� ��� �� 0.001 ��� ����������� �� ����������� � �������.
    float currentTime = timeGetTime() * 0.001f;
	//float currentTime = clock() * 0.001f;
	float FPS;

    //����������� ������� ������
    ++framesPerSecond;

    //������ ������ �� �������� ������� ��������� ����������� �����. ���� ��������� ������ �������,
    //��� ������, ��� ������� ������ � ����� ������� ����� FPS.
    if(currentTime - lastTime > 1.0f)
    {
        //������������� lastTime � ������� �����. ������ ��� ����� ������������� ��� ���������� �����
        //��� ����. �������.
        lastTime = currentTime;

        // ��������� FPS ��� ������:
        FPS=framesPerSecond;

        //������� FPS
        framesPerSecond = 0;

		return FPS;
    }
	return -1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg){
        case WM_CREATE:{
			hDC = GetDC(hwnd);
            SetDCPixelFormat(hDC);
            hRC = wglCreateContext(hDC);
            wglMakeCurrent(hDC, hRC);

            InitGL();
        }
        break;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        break;

        case WM_DESTROY:
            wglMakeCurrent(0, 0);
            ReleaseDC(hwnd, hDC);
            DeleteDC(hDC);
            PostQuitMessage(0);
            return 0;
        break;

        case WM_KEYDOWN:{
            _keys[wParam] = true;
        }
        break;
        case WM_KEYUP:{
            _keys[wParam] = false;
        }
        break;

        case WM_SIZE:
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

	return 0;
}

void SetDCPixelFormat(HDC hDC){
	PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = COLOR_BITS;
    pfd.cDepthBits = DEPTH_BITS;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(hDC, &pfd);

    SetPixelFormat(hDC, iFormat, &pfd);
}

void InitGL(){
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void ReSizeGLScene(GLsizei width, GLsizei height){
	if (height==0){
        height=1;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-width/2, width/2, -height/2, height/2, 0.1, 100);
    gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, _NEARZ, _FARZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
