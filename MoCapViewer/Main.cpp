#include <windows.h>
#include "BVH.h"

static int win_width, win_height;
static float camDist = 100.0f, camHeight = 0.0f, camYaw = 0.0f;

bool animating = true;
unsigned int numFrame = 0;

BVH *bvh = NULL;

void initRendering()
{
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glClearColor(0.5f, 0.5f, 0.8f, 0.0f);
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, -camHeight, -camDist);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, -15.0f, 0.0f);

	//do stuff, like rendering bones, animation and whatever here
	glColor3f( 1.0f, 0.0f, 0.0f );
	if ( bvh )
	{
		// compute passed time
		static DWORD  lastTime = 0;
		static float delta = 0.0f;
		DWORD  currTime = GetTickCount64();
		delta += (currTime - lastTime) * 0.001f;
		lastTime = currTime;
		
		if (delta > 0.03f)
			delta = 0.03f;

		float frameTime = bvh->getFrameTime();
		if (delta > frameTime)
		{
			delta -= frameTime;
			if (animating)
				numFrame++; 
		}
		
		bvh->Render(numFrame);

	}

	glutSwapBuffers();

	glutPostRedisplay();
}

void  keyboard(unsigned char key, int mx, int my)
{
	switch(key)
	{
	case 'w': {
					camHeight += 2.5f;
					if (camHeight > 30.0f) camHeight = 30.0f;
					break;
			  }
	case 's': {
					camHeight -= 2.5f;
					if (camHeight < -30.0f) camHeight = -30.0f;
					break;
			  }
	case 'a': {
					camYaw -= 2.5f;
					if (camYaw < 0.0f) camYaw += 360.0f;
					break;
			  }
	case 'd': {
					camYaw += 2.5f;
					if (camYaw > 360.0f) camYaw -= 360.0f;
					break;
			  }
	case 'q': {
					camDist -= 2.0f;
					if (camDist < 30.0f) camDist = 30.0f;
					break;
			  }
	case 'e': {
					camDist += 2.0f;
					if (camDist > 150.0f) camDist = 150.0f;
					break;
			  }
	case 'p': { 
					animating = !animating;
					break;
			  }
	case 'i': {
					if (!animating && numFrame > 0)
						numFrame--;
					break;
			  }
	case 'o': {
					if (!animating)
						numFrame++;
					break;
			  }
	case 'l': { 
					wchar_t file_name[260];

					// OpenFileName Struct
					OPENFILENAMEW open_file;
					ZeroMemory(&open_file, sizeof(OPENFILENAMEW));
					open_file.lStructSize = sizeof(OPENFILENAMEW);
					open_file.hwndOwner = NULL;
					open_file.lpstrFilter = L"BVH Motion Data (*.bvh)\0*.bvh\0All (*.*)\0*.*\0";
					open_file.nFilterIndex = 1;
					open_file.lpstrFile = file_name;
					file_name[0] = '\0';
					open_file.nMaxFile = sizeof(file_name);
					open_file.lpstrTitle = L"Select a BVH file";
					open_file.lpstrDefExt = L"bvh";
					open_file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					// Open File then go on
					if( GetOpenFileNameW(&open_file) == TRUE)
					{
						// if animation running
						if (bvh)
							delete  bvh;
						bvh = new BVH(file_name);

						// delete if loading failed
						if (!bvh->isLoaded())
						{
							delete  bvh;
							bvh = NULL;
						}

						//	start of the animation
						numFrame = 0;
					}
					break;
			}
	}
	
	glutPostRedisplay();
}

void  reshape(int w, int h)
{
    glViewport(0, 0, w, h);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w/h, 1, 200);

	win_width = w;
	win_height = h;
}



int  main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize( 900, 500 );
	glutInitWindowPosition( 100, 50 );
    glutCreateWindow("MoCap Viewer");
	
	glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	
	initRendering();

    glutMainLoop();
    return 0;
}

