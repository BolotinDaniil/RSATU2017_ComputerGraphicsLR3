#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>

#include "mathFunctions.cpp"

#define WINDOW_CLASS_NAME 	L"OPENGL_TEMPLATE"
#define W_WIDTH 			1024
#define W_HEIGHT 			512

#define COLOR_BITS          24
#define DEPTH_BITS          32
#define _NEARZ 				0.1f
#define _FARZ 				100.0f

#define EPS 0.0000001f

using namespace std;

bool _keys[256];//����� ������� �������. ���� _keys[i] == true, �� ������� � ����� i ������

HDC hDC;//��������� �� �������� ���������� ������ ������ ����
HGLRC hRC;//��������� �� �������� �������

struct Point{
	bool operator ==(const Point& p) const {
		return p.v == v;
	}
	
	vec3 v;
	vec3 vt;
	vec3 vn;
};

struct Face{
	Point points[3];
	vec3 normal;

	void calcNormal(){
		vec3 vp[] = { points[0].v, points[1].v, points[2].v };
		normal = GetNormal(vp);
	}

    bool operator ==(const Face& f) const {
        bool equals = true;
        for (int i = 0; i < 3 && equals; i++) {
            Point p = points[i];
            equals = false;
            for (int j = 0; j < 3 && !equals; j++) {
                equals = f.points[j] == p;
            }
        }
        return equals;
    }
};

struct Edge{
	Point pointA;
	Point pointB;
	bool wasVisible;
	
	vector<Face> faces;
	//Face faceA;
	//Face faceB;
};

//Trash
int strToInt(string str);
vector<int> parsePointInfo(string pointInfo);

struct Model{
	Model() 
		: drawContour(false), 
		lit(false),
		translation(0, 0, 0),
		rotation(0, 0, 0),
		scale(1, 1, 1) {
	}
	
	Model(string fileName) 
			: drawContour(false),
			lit(false),
			translation(0, 0, 0),
			rotation(0, 0, 0),
			scale(1, 1, 1) {
		loadModelFromFile(fileName, *this);
	}
	
	void addEdges(Face& face){
		addEdge(face.points[0], face.points[1], face);
		addEdge(face.points[1], face.points[2], face);
		addEdge(face.points[2], face.points[0], face);
	}
	
	void addEdge(Point& pointA, Point& pointB, Face& face){
		bool edgeExists = false;
		for (int i = 0; i < edges.size() && !edgeExists; i++){
			if (edges[i].pointA == pointA && edges[i].pointB == pointB
                || edges[i].pointA == pointB && edges[i].pointB == pointA){
				edges[i].faces.push_back(face);
				edgeExists = true;
			}
		}
		
		if (!edgeExists){
			Edge edge;
			edge.pointA = pointA;
			edge.pointB = pointB;
			edge.wasVisible = true;
			edge.faces.push_back(face);
			
			edges.push_back(edge);
		}
	}
	
	static void loadModelFromFile(const string& fileName, Model& model){
		ifstream input(fileName.c_str());
	    loadModelFromStream(input, model);
	    input.close();
	}
	
	static void loadModelFromStream(istream& input, Model& model){
		vector<vec3> vertices;
    	vector<vec3> textureCoords;
    	vector<vec3> normals;
    	
    	while (!input.eof()){
	    	string str;
	    	input >> str;
	    	if (str == "v"){
	    		vertices.push_back( loadVec3FromStream(input) );
	    		continue;
	    	}
	    	
	    	if (str == "vt"){
	    		textureCoords.push_back( loadVec3FromStream(input) );
	    		continue;
	    	}
	    	
	    	if (str == "vn"){
	    		normals.push_back( loadVec3FromStream(input) );
	    		continue;
	    	}
	    	
	    	if (str == "f"){
	    		Face face;
	    		for (int i = 0; i < 3; i++){
	    			Point p;
	    			vector<int> pointInfo;
	    			
	    			input >> str;
	    			
					pointInfo = parsePointInfo(str);
					if (pointInfo[0] > 0){
						p.v = vertices[pointInfo[0] - 1];
					}
					if (pointInfo[1] > 0){
						p.vt = textureCoords[pointInfo[1] - 1];
					}
	    			if (pointInfo[2] > 0){
						p.vn = normals[pointInfo[2] - 1];
					}
	    			
					face.points[i] = p;
					//model.points.push_back(p);
	    		}
				face.calcNormal();
	    		model.faces.push_back(face);
	    		model.addEdges(face);
	    		continue;
	    	}
	    }
	}
	
	static vec3 loadVec3FromStream(istream& input){
		vec3 v;	
		input >> v.x >> v.y >> v.z;
		return v;
	}
	
    //vector<Point> points;
    vector<Face> faces;
    vector<Edge> edges;
    
    bool drawContour;
    bool lit;
    vec3 translation;
	vec3 rotation;
    vec3 scale;
    
    vec3 color;
};

GLsizei winWidth = W_WIDTH;
GLsizei winHeight = W_HEIGHT;

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

float CalculateFPS();

void renderModel(GLint vpX, GLint vpY, GLsizei vpWidth, GLsizei vpHeight, Model& model, vec3& eye);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow){
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
    winClass.lpszClassName = WINDOW_CLASS_NAME;
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
    
	string filename = "models/teapot.obj";
    //cin >> filename;
    
    Model objModel(filename);
    
    double xangle = 0.0, yangle = 0.0;
    bool bQuit = false;
    
    vec3 eye(0, 0, 6.0);
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
        		eye.z += 0.1;
        	}
        	
        	if(_keys['S']){
        		eye.z -= 0.1;
        	}
        	
        	if(_keys['A']){
        		eye.x -= 0.1;
        	}
        	
        	if(_keys['D']){
        		eye.x += 0.1;
        	}

            if (_keys['Q']) {
                eye.y += 0.1;
            }

            if (_keys['E']) {
                eye.y -= 0.1;
            }
        	
        	//����� ��������� �������� ��������� ����� ���������
			glScissor(0, 0, winWidth / 2, winHeight);
			glClearColor(178.0 / 255.0, 216.0 / 255.0, 237.0 / 255.0, 1.0);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glScissor(winWidth / 2, 0, winWidth / 2, winHeight);
			glClearColor(1,1,1,1);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        	
			objModel.lit = true;
			objModel.drawContour = false;
			objModel.color = vec3(1.0, 1.0, 1.0);
			glClearColor(1, 1, 1, 1);
			renderModel(0, 0, winWidth / 2, winHeight, objModel, eye);
			
			objModel.lit = false;
			objModel.drawContour = true;
			objModel.color = vec3(1, 1, 1);
			glClearColor(1, 1, 1, 1);
			renderModel(winWidth / 2, 0, winWidth / 2, winHeight, objModel, eye);
			
		    SwapBuffers(hDC);

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
        }
    }
    return 0;
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
    glShadeModel(GL_FLAT);
    glClearDepth(1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_LIGHT0);
    GLfloat lightpos[] = {0.0, 0.5, 0.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glClearColor(178.0 / 255.0, 216.0 / 255.0, 237.0 / 255.0, 1.0);
	glEnable(GL_SCISSOR_TEST);
}

void ReSizeGLScene(GLsizei width, GLsizei height){
	if (height==0){
        height=1;
    }
    
    winWidth = width;
    winHeight = height;
}

/*
   Determine whether or not the line segment p1,p2
   Intersects the 3 vertex facet bounded by pa,pb,pc
   Return true/false


   The equation of the line is p = p1 + mu (p2 - p1)
   The equation of the plane is a x + b y + c z + d = 0
                                n.x x + n.y y + n.z z + d = 0
*/

bool lineFacet(vec3 &p1, vec3 &p2, const Face &face)
{
   double d;
   //double EPS = 0.000000001;
   double a1,a2,a3;
   double total,denom,mu;
   vec3 n, pa1, pa2, pa3, p;
	
   vec3 pa = face.points[0].v;
   vec3 pb = face.points[1].v;
   vec3 pc = face.points[2].v;


   /* Calculate the parameters for the plane */
   n.x = (pb.y - pa.y)*(pc.z - pa.z) - (pb.z - pa.z)*(pc.y - pa.y);
   n.y = (pb.z - pa.z)*(pc.x - pa.x) - (pb.x - pa.x)*(pc.z - pa.z);
   n.z = (pb.x - pa.x)*(pc.y - pa.y) - (pb.y - pa.y)*(pc.x - pa.x);
   n = Normalized(n);
   d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;


   /* Calculate the position on the line that intersects the plane */
   denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
   if (fabs(denom) < EPS)         /* Line and plane don't intersect */
      return false;
   mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
   p.x = p1.x + mu * (p2.x - p1.x);
   p.y = p1.y + mu * (p2.y - p1.y);
   p.z = p1.z + mu * (p2.z - p1.z);
   if (mu < 0 || mu > 1)   /* Intersection not along line segment */
      return false;


   /* Determine whether or not the intersection point is bounded by pa,pb,pc */
   pa1.x = pa.x - p.x;
   pa1.y = pa.y - p.y;
   pa1.z = pa.z - p.z;
   pa1 = Normalized(pa1);
   pa2.x = pb.x - p.x;
   pa2.y = pb.y - p.y;
   pa2.z = pb.z - p.z;
   pa2 = Normalized(pa2);
   pa3.x = pc.x - p.x;
   pa3.y = pc.y - p.y;
   pa3.z = pc.z - p.z;
   pa3 = Normalized(pa3);
   a1 = pa1.x*pa2.x + pa1.y*pa2.y + pa1.z*pa2.z;
   a2 = pa2.x*pa3.x + pa2.y*pa3.y + pa2.z*pa3.z;
   a3 = pa3.x*pa1.x + pa3.y*pa1.y + pa3.z*pa1.z;
   total = (acos(a1) + acos(a2) + acos(a3));
   if (fabs(total - 3.14) > EPS)
      return(false);

   return(true);
}

bool edgeVisible(Edge &edge, Model &model, vec3 &eye){
	bool visible = true;
	for (int i=0; i<model.faces.size(); i++){
		vec3 p = edge.pointA.v;
		if (lineFacet(p, eye, model.faces[i]) == false)
			return false;

		p = edge.pointB.v;
		if (lineFacet(p, eye, model.faces[i]) == false)
			return false;
	}
	return visible;
}

void drawContour(Model& model, vec3& eye){
	glLineWidth(3.0);
	
	vec3 oz = Normalized(eye);
	
	glColor3f(0.0, 0.0, 0.0);
	// �� ����� ������������ �� ����������� �������� ����� ����, ������� �� ������� �������� �� ���� ���������������
	// ��-�� ���� �������� ����-���� ������� ��� ������������ ������
	// ���� ����������� ����� FPS
	static int RAND_BORDER = RAND_MAX * 0.25;
	for (int i = 0; i < model.edges.size(); i++) {
        if (model.edges[i].faces.size() >= 2 && (model.edges[i].wasVisible || (rand() < RAND_BORDER))) {
			Edge *edge = &model.edges[i];
            Face faceA = edge->faces[0];
            Face faceB = edge->faces[1];

            vec3 aPoints[] = { faceA.points[0].v, faceA.points[1].v, faceA.points[2].v };
            vec3 na = faceA.normal;

            vec3 bPoints[] = { faceB.points[0].v, faceB.points[1].v, faceB.points[2].v };
            vec3 nb = faceB.normal;

            vec3 v0 = edge->pointA.v;
			v0 = eye - v0;

            double a = na & v0;
            double b = nb & v0;
            // double angle = GetAngle(na, nb);

            if (a*b <= 0) 
			if (edgeVisible(model.edges[i], model, eye)){
				edge->wasVisible = true;
                Point p;
                glBegin(GL_LINES);
                double scale = 1;
                p = edge->pointA;
                //glNormal3f(p.vn.x, p.vn.y, p.vn.z);
                glVertex3f(p.v.x, p.v.y, p.v.z);

                p = edge->pointB;
                //glNormal3f(p.vn.x, p.vn.y, p.vn.z);
                glVertex3f(p.v.x, p.v.y, p.v.z);
                glEnd();
            }
			else edge->wasVisible = false;
        }
	}
}

void renderModel(GLint vpX, GLint vpY, GLsizei vpWidth, GLsizei vpHeight, Model& model,vec3& eye){
	if (model.lit){
		glEnable(GL_LIGHTING);
	}
	
	glScissor(vpX, vpY, vpWidth, vpHeight);
	glViewport(vpX, vpY, vpWidth, vpHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble)vpWidth / (GLdouble)vpHeight, _NEARZ,  _FARZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye.x, eye.y, eye.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glColor3f(model.color.x, model.color.y, model.color.z);
	
    glPushMatrix();
    if (!model.drawContour) {
        for (int i = 0; i < model.faces.size(); i++) {
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < 3; j++) {
                Point p;
                p = model.faces[i].points[j];
                glNormal3f(p.vn.x, p.vn.y, p.vn.z);
                glVertex3f(p.v.x, p.v.y, p.v.z);
            }
            glEnd();
        }
    }
    glPopMatrix();
	
	if (model.drawContour){
		drawContour(model, eye);
	}
	
	if (model.lit){
		glDisable(GL_LIGHTING);
	}
}

int strToInt(string str){
	int result = 0;
	
	for (int i = 0; i < str.length(); i++){
		char cNum = str[i];
		result *= 10;
		result += (cNum - '0');
	}
	
	return result;
}

vector<int> parsePointInfo(string pointInfo){
	vector<int> result;
	
	string dataNum;
	for (int i = 0; i < pointInfo.length(); i++){
		if (pointInfo[i] != '/')
			dataNum.push_back(pointInfo[i]);
		else{
			result.push_back(strToInt(dataNum));
			dataNum.clear();
		}
	}
	result.push_back(strToInt(dataNum));
	
	while(result.size() < 3){
		result.push_back(0);
	}
	
	return result;
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
