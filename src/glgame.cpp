
#include <stdio.h>
#include <windows.h>
#include "glut.h" 
#include "gl.h" 


#pragma warning(disable:4996)

void display(void) {
	cgl.display();
}
void idle(void) {
	cgl.idle();
}
void keyboard( unsigned char key,int a,int b ) {
	cgl.keyboard( key, a, b );
}

void keyboardrelease(unsigned char key,int a,int b){
	cgl.keyboardrelease(key,a,b);
}

void timer( int flag ) {
	cgl.timer();
	glutTimerFunc( CGL::TIMER, timer, 0 );
}

void mouse_move( int x, int y ) {
	cgl.mouseMotion( x, y );
}

void mouse_press( int button, int state, int x, int y ) {
	cgl.mousePress( button, state, x, y );
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
 	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); 
	glutInitWindowPosition(100,100);
	glutCreateWindow("Simple GL Application");	
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(keyboardrelease);
	glutKeyboardFunc(keyboard);
	glutTimerFunc( 1000, timer, 0 );
	glutMotionFunc( mouse_move );
	glutMouseFunc( mouse_press );
	cgl.init();
	glutMainLoop();

	return 0;
}


