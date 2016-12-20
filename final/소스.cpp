#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)
/*font*/
static int base;
HDC hDC;
HWND hWnd;
/*font end*/
void init(void);

float theta[3], ang;
float viewer[3];
GLubyte m_image[64][64][3];
AUX_RGBImageRec *texRec[2];//텍스쳐 저장공간을 가리키는포인터
GLuint texID[2];

void timer(int value)
{
	ang += 2.0;
	if (ang >= 360.0)
		ang -= 360;

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void LoadTexture()
{
	/*take a monalisa*/
	texRec[0] = auxDIBImageLoad("image/monalisa.bmp");
	texRec[1] = auxDIBImageLoad("image/water.bmp");
	glGenTextures(2, &texID[0]);

	for (register int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete[] texRec[i]->data;
		delete texRec[i];
	}
	/*
	for (register int i = 0; i < 64; i++)
	{
		for (register int j = 0; j < 64; j++)
		{
			int c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			for (register int k = 0; k < 3; k++)
				m_image[i][j][k] = (GLubyte)c;
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0,GL_RGB, GL_UNSIGNED_BYTE, m_image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
}

void killFont(GLvoid)
{
	glDeleteLists(base, 96);
}

void glPrint(const char *fmt, ...)
{
	char text[256];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT); {
		glListBase(base - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	}glPopAttrib();
}

void BuildFont(void)
{
	HFONT font;
	HFONT oldfont;

	base = glGenLists(96);

	font = CreateFontA(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "courier New");
	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x' | 'X':
		theta[0] += 2; break;
	case 'y'|'Y':
		theta[1] += 2; break;
	case 'z'|'Z':
		theta[2] += 2; break;
	case 'b'|'B':
		viewer[0] -= 0.1; break;
	case 'm'|'M':
		viewer[0] += 0.1; break;
	case 'n'|'N':
		viewer[1] -= 0.1; break;
	case 'h'|'H':
		viewer[1] += 0.1; break;
	case 'g' | 'G':
		viewer[2] -= 0.1; break;
	case 'j' | 'J':
		viewer[2] += 0.1; break;
	case '0':
		init(); break;
	case 'q' | 'Q':
		exit(0); break;
	case VK_ESCAPE:
		exit(0); break;
	}
}

void menu(int item)
{
	keyboard((unsigned char)item, 0, 0);
}

void initlight0(void)
{
	GLfloat light0_pos[4] = { 2,0,0,1 };
	GLfloat light0_amb[4] = { 0.1,0.1,0.1,1.0 };
	GLfloat light0_dif[4] = { 1,0,0,1 };
	GLfloat light0_spe[4] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spe);
}

void initlight1(void)
{
	GLfloat light1_pos[4] = { 0,2,0,1 };
	GLfloat light1_amb[4] = { 0,0,0,1 };
	GLfloat light1_dif[4] = { 0,1,0,1 };
	GLfloat light1_spe[4] = { 1,1,1,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spe);
}

void initlight2(void)
{
	GLfloat light2_pos[4] = { 0,0,2,1 };
	GLfloat light2_amb[4] = { 0,0,0,1 };
	GLfloat light2_dif[4] = { 0,0,1,1 };
	GLfloat light2_spe[4] = { 1,1,1,1 };
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_amb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_dif);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_spe);
}
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos3f(1,1,1);
	glColor3f(1, 0, 0);
	glPrint("i love hyeyoon");
	glPushMatrix();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0, 0, 0, 0, 1, 0);
	/*xyz rotate*/
	glRotatef(theta[0], 1, 0, 0);
	glRotatef(theta[1], 0, 1, 0);
	glRotatef(theta[2], 0, 0, 1);
	glDisable(GL_LIGHTING);
	/*
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	glBegin(GL_QUADS); {
		glColor3f(1, 1, 1);
		glRotatef(90, 1, 0, 0);
		//glColor3f(1, 1, 1);
		//glNormal3f(0, 0, 0);
		
		glTexCoord2f(0, 0);	glVertex3f(0, 0, 0);
		glTexCoord2f(5, 0);	glVertex3f(5, 0, 0);
		glTexCoord2f(5, 5);	glVertex3f(5, 0, 5);
		glTexCoord2f(0, 5);	glVertex3f(0, 0, 5);
	}glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	glBegin(GL_QUADS); {
		glRotatef(90, 1, 0, 0);
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0);	glVertex3f(0, 0, 0);
		glTexCoord2f(5, 0);	glVertex3f(5, 0, 0);
		glTexCoord2f(5, 5);	glVertex3f(5, 5, 0);
		glTexCoord2f(0, 5);	glVertex3f(0, 5, 0);
	}glEnd();
	*/
	glBegin(GL_LINES); {
		glColor3f(1, 0, 0);
		glVertex3f(10, 0, 0);
		glVertex3f(0, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 10, 0);
		glVertex3f(0, 0, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 10);
		glVertex3f(0, 0, 0);
	}glEnd();

	glEnable(GL_LIGHTING);
	initlight0();
	glEnable(GL_LIGHT0);
	initlight1();
	glEnable(GL_LIGHT1);
	initlight2();
	glEnable(GL_LIGHT2);
	
	glPushMatrix(); {
		glRotatef(90, 1, 0, 0);
		glRotatef(ang, 0, 1, 0);
		glColor4f(1, 0, 0, 0.8);
		//testt
		glutSolidTorus(0.3, 0.6, 10, 50);
		glTranslatef
	}glPopMatrix();
	glPopMatrix();
	//glColor3f(1, 0, 0);
	//glPushMatrix(); {
		glRotatef(ang, 1, 0, 0);

	//}glPopMatrix();
	glutSwapBuffers();
}


void init(void)
{
	ang = 0;
	glutTimerFunc(1000 / 30, timer, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	
	hWnd = GetActiveWindow();
	hDC = GetDC(hWnd);
	//glNewList(l)
	BuildFont();
	for (int i = 0; i < 3; i++)
		theta[i] = 0;
	for (int i = 0; i < 3; i++)
		viewer[i] = 5;
	LoadTexture();
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	/*transparent*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat fogcolor[4] = { 1, 0.5, 0.5, 1 };
	//glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.15f);
}

void ChangeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	{
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w, 2.0*(float)h / (float)w, -10.0, 10.0);
	}
	else
	{
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h, -2.0, 2.0, -10.0, 10.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("FINAL");
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	SetupRC();
	glutMainLoop();

}