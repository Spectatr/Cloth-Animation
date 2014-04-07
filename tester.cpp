////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Cloth"

//Plane
#define y_cord -25
#define clothx 20
#define clothy 20

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc, argv);

	float position[] = { -0, 30, 0, 0.0 };


	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess[] = { 100.0 };
	


	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 20.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0, -1, 0.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);


	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE | GL_CULL_FACE_MODE);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);



	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0.9, 0.9, 0.9, 0.9 );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	cloth = new ClothSystem(clothx, clothx);

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	cloth->update(.004);

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));

}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	cloth->draw();

	//Draw Plane
	//glutSolidCube(10);
	glBegin(GL_TRIANGLES);
	glColor3f(.3, .3, .3);

	glNormal3f(0, 1, 0);
	glVertex3f(-50, y_cord, 50);
	glVertex3f(50, y_cord, 50);
	glVertex3f(50, y_cord, -50);

	glNormal3f(0, -1, 0);
	glVertex3f(-50, y_cord, 50);
	glVertex3f(-50, y_cord, -50);
	glVertex3f(50, y_cord, -50);
	glEnd();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case 's':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
					cloth->p[i].position.z += .5;
			}
			break;
		case 'w':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
				cloth->p[i].position.z -= .5;
			}
			break;
		case 'a':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
					cloth->p[i].position.x -= .5;
			}
			break;
		case 'd':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
					cloth->p[i].position.x += .5;
			}
			break;
		case 'q':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
					cloth->p[i].position.y += .5;
			}
			break;
		case 'e':
			for (int i = 0; i < clothx * clothy; i++){
				if (!(i % clothx))
					cloth->p[i].position.y -= .5;
			}
			break;	
		case 'z':
			cloth->angle += 1;
			break;
		case'Z':
			cloth->angle -= 1;
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
