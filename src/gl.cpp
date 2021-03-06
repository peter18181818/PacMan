#include <stdio.h>
#include <windows.h>
#include <iostream>

using namespace std;

#include "glut.h"
#include "collision.h"
#include "util.h"
#include "gl.h"

//#define ASE

CGL cgl;

// lista indeksow obiektow
#define OBJ_ENEMY 4
#define OBJ_PLAYER 3

GLuint texture_load(char *file,int numer) {
       // wczytywanie tekstury z pliku dyskowego
       BITMAPINFOHEADER bitmapInfoHeader;
       unsigned char* bitmap_data = CUTIL::LoadBitmapFile( file, &bitmapInfoHeader );
       GLuint textura;
       textura=numer;
       if( bitmap_data != NULL ) {
              glBindTexture(GL_TEXTURE_2D, textura);
              glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
              glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
                             bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap_data);
              glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
              glEnable(GL_TEXTURE_2D);
			  //glDisable(GL_TEXTURE_2D);
       }

       return textura;

};


///////////////////////////////////////// inicjalizacja srodowiska graficznego
void CGL::init( void ) {

	//wczytywanie textur
	ziemia=texture_load("cubetr5.bmp",0);
	sciana=texture_load("cubetr4.bmp",1);
	kula=texture_load("cubetr3.bmp",2);
	przeciwniktexture=texture_load("bosscube.bmp",3);
	gracztexture=texture_load("farao.bmp",4);
	// ustawienie parametrow kamery
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 50, 1, 1, 1000);
	gluLookAt( 0, 0, -7, 0, 0, 0, 0, 1, 0 );

	// wyzerowanie transformacji modelu
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable( GL_DEPTH_TEST ); // wlaczenie bufora Z
	glEnable( GL_NORMALIZE ); // wlaczenie automatycznej normalizacji wektorow po skalowaniu

	// ustawienie parametrow zrodel swiatla
	glEnable(GL_LIGHTING);

/*
	// swiatlo znajdujace sie nad plansza (ruszajace sie z plansza po zmianie kamery)
	GLfloat specular[] = {1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	GLfloat diffuse[] = { 0.9f, 0.9f, 0.9f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_EMISSION, emission);
	glEnable(GL_LIGHT0);

	// swiatlo otoczenia umieszczone w poblizu kamery
	float g_lightPos[4] = { 0, 0, -6, 1 };
	GLfloat color[] = { 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, g_lightPos);
	color[0] = color[1] = color[2] = 0.8f;
	glLightfv(GL_LIGHT2, GL_AMBIENT, color);
	color[0] = color[1] = color[2] = 0.9f;
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color);
	color[0] = color[1] = color[2] = 0.0f;
	glLightfv(GL_LIGHT2, GL_EMISSION, color);
	glLightfv(GL_LIGHT2, GL_SPECULAR, color);
	glEnable(GL_LIGHT2);
*/

	glClearColor( 0.2f, 0.2f, 0.2f, 0.5f ); // ustawienie koloru tla

	glShadeModel( GL_SMOOTH ); // wlaczenie cieniowania Gouraud'a (smooth shading)

	// wczytanie obiektu ASE z pliku dyskowego
	przeciwnik = CUTIL::loadASE( "boscube.ase" );
	gracz = CUTIL::loadASE( "faraon.ase" );




}

///////////////////////////////////////// rysowanie planszy
void CGL::draw_board( float board_width, float board_height ) {

	glPushMatrix();

	float ambient[4] = { 0.3, 0.3, 0.3, 1.0 };
	float diffuse[4] = { 0.1, 0.5, 0.7, 1.0 };
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, color );
	glMaterialfv( GL_FRONT, GL_EMISSION, color);

	float thickness = 0.05f; // grubosc elementow planszy
	//float thickness = 6.4f;
	float wall_high = 0.5f; // wysokosc scian

	glTranslatef( 0.0f, 0.0f, 0.2f );

	// rysowanie podstawy planszy z tekstura
	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, texture );
	glScalef( board_width, board_height, thickness );
	CUTIL::drawBox();
	glBindTexture( GL_TEXTURE_2D, 0 );
	glPopMatrix();

	// rysowanie scian planszy
	glPushMatrix();
	glTranslatef( -board_width/2, 0.0f, -wall_high/2 );
	glScalef( thickness, board_height, wall_high );
	glutSolidCube( 1.0 );
	glPopMatrix();
	glPushMatrix();
	glTranslatef( board_width/2, 0.0f, -wall_high/2 );
	glScalef( thickness, board_height, wall_high );
	glutSolidCube( 1.0 );
	glPopMatrix();
	glPushMatrix();
	glTranslatef( 0.0f, board_height/2, -wall_high/2 );
	glScalef( board_width, thickness, wall_high );
	glutSolidCube( 1.0 );
	glPopMatrix();
	glPushMatrix();
	glTranslatef( 0.0f, -board_height/2, -wall_high/2 );
	glScalef( board_width, thickness, wall_high );
	glutSolidCube( 1.0 );
	glPopMatrix();

	glPopMatrix();
}

///////////////////////////////////////// rysowanie gracza (czerwonego czajnika)
void CGL::draw_player( float x, float y ) {

	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glPushMatrix();

	color[0] = 0.9f;
	color[1] = 0.0f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
	color[0] = 0.2f;
	color[1] = 0.0f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_AMBIENT, color );
	color[0] = color[1] = color [2] = 1.0f;
	glMaterialfv( GL_FRONT, GL_SPECULAR, color );
	color[0] = color[1] = color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_EMISSION, color);
	glMaterialf( GL_FRONT, GL_SHININESS, 100.0f);

	glTranslatef( x, y, 0.0f );
	glScalef( 0.12f, 0.12f, 0.12f );

	glRotatef( -90.0f, 1, 0, 0 );
	glRotatef( -90.0f + playerMovementDirection, 0, 1, 0 ); // obracanie czajnika zgodnie z kierunkiem ruchu

	if( isCollision == true ) {
		glutSolidSphere( 2.0f, 10, 10 );
	}
	else
		//glutSolidTeapot( 3.8f );

	//glScalef( 1.0f,2.9f, 1.0f );
	glScalef( 0.04f,0.04f, 0.04f );
	glBindTexture(GL_TEXTURE_2D, gracztexture);
	glCallList( gracz );
	glBindTexture(GL_TEXTURE_2D, 0);

	// utworzenie wokol gracza strefy wykrywania kolizji
	float dims[3] = { 2.0f, 2.0f, 2.0f };
	collision.addBox( OBJ_PLAYER, dims );

	glPopMatrix();
}




///////////////////////////////////////// rysowanie stref wykrywania kolizji
// (funkcja sluzy do testowania poprawnosci zdefiniowania stref i powinna byc wylaczona w docelowej kompilacji)
void CGL::draw_collisions_boxes( void ) {

	glPushMatrix();

	float col[4] = { 1.0, 1.0, 1.0, 1.0 };

	CObj* obj = collision.getObject( OBJ_PLAYER );
	if( obj != NULL ) {
		glLoadMatrixf( obj->mmatrix );

		glMaterialfv( GL_FRONT, GL_EMISSION, col );
		glMaterialfv( GL_FRONT, GL_DIFFUSE, col);
		glScalef(obj->dims[0], obj->dims[1], obj->dims[2]);

		glutWireCube( 1 );
	}

	obj = collision.getObject( OBJ_ENEMY );
	if( obj != NULL ) {
		glLoadMatrixf( obj->mmatrix );

		glMaterialfv( GL_FRONT, GL_EMISSION, col );
		glMaterialfv( GL_FRONT, GL_DIFFUSE, col);
		glScalef(obj->dims[0], obj->dims[1], obj->dims[2]);

		glutWireCube( 1 );
	}

	glPopMatrix();
}

///////////////////////////////////////// rysowanie obiektow
void CGL::draw_enymy( float pos_x, float pos_y ) {

	glPushMatrix();

	/*#ifndef ASE
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	color[0] = 0.0f;
	color[1] = 0.7f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
	color[0] = 0.2f;
	color[1] = 0.0f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_AMBIENT, color );
	color[0] = color[1] = color [2] = 1.0f;
	glMaterialfv( GL_FRONT, GL_SPECULAR, color );
	color[0] = color[1] = color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_EMISSION, color);
	glMaterialf( GL_FRONT, GL_SHININESS, 100.0f);
	#endif*/

	glTranslatef( pos_x, pos_y, 0.0f );
	glScalef( 0.05f, 0.05f, 0.03f );



	#ifdef ASE
	float dims[3] = { 3.0f, 3.0f, 3.0f};
	// rysowanie obiektu ASE
	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, 0 ); // reset texture
	glScalef( 1.04f, 1.04f, 1.04f );
	glRotatef( 180.0f, 1, 0, 0 );
	glCallList( aseGeometry ); // ASE object list
	glPopMatrix();
	#else
	float dims[3] = { 2.0f, 2.0f, 2.0f};
	glRotatef( -90.0f, 1, 0, 0 );

	//glutSolidTeapot( 3.8f );
	//glScalef( 0.15f,0.15f, 0.15f );
	glScalef( 1.0f,2.9f, 1.0f );
	glBindTexture(GL_TEXTURE_2D, przeciwniktexture);
	glCallList( przeciwnik );
	glBindTexture(GL_TEXTURE_2D, 0);
	#endif




	// utworzenie wokol obiektu strefy wykrywania kolizji
	collision.addBox( OBJ_ENEMY, dims );

	glPopMatrix();
}

/////////////////////////////////////////rysowanie kulek
/*void CGL::draw_ball( float pos_x2, float pos_y2 ) {

	glPushMatrix();

	#ifndef ASE
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	color[0] = 0.0f;
	color[1] = 0.7f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_DIFFUSE, color );
	color[0] = 0.2f;
	color[1] = 0.0f;
	color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_AMBIENT, color );
	color[0] = color[1] = color [2] = 1.0f;
	glMaterialfv( GL_FRONT, GL_SPECULAR, color );
	color[0] = color[1] = color [2] = 0.0f;
	glMaterialfv( GL_FRONT, GL_EMISSION, color);
	glMaterialf( GL_FRONT, GL_SHININESS, 100.0f);
	#endif

	glTranslatef( pos_x2, pos_y2, 0.0f );
	glScalef( 0.15f, 0.15f, 0.15f );

	#ifdef ASE
	float dims[3] = { 3.0f, 3.0f, 3.0f};
	// rysowanie obiektu ASE
	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, 0 ); // reset texture
	glScalef( 1.04f, 1.04f, 1.04f );
	glRotatef( 180.0f, 1, 0, 0 );
	glCallList( aseGeometry ); // ASE object list
	glPopMatrix();
	#else
	float dims[3] = { 2.0f, 2.0f, 2.0f};
	glRotatef( -90.0f, 1, 0, 0 );
	glutSolidSphere(0.2,10,10);
	#endif
}*/
///////////////////////////////////////// glowna procedura przerysowania ekranu

class kwadrat{ //klasa wykorzystywana do utworzenia kolizji gracza ze ścianami
public:
float x;
float y;


kwadrat(float iks,float igrek){

x=iks;
y=igrek;
glPushMatrix();
//glTranslatef(x,y,1.0f);
//CUTIL::drawBox();
glPopMatrix();}

};

vector<kwadrat> tab;
//---------------------
class kulka{
public:
float x;
float y;

class kulka(float iks,float igrek){ //klasa wykorzystywana do utworzenia "kulek do zjadania"

x=iks;
y=igrek;
glPushMatrix();
glTranslatef(x,y,1.0f);
glutSolidSphere(0.2,10,10);
glPopMatrix();}

};

vector<kulka> tabl;
//-----------------
class podloga{
public:
float x;
float y;


podloga(float iks,float igrek){ //klasa wykorzystywana do stworzenia podstawy planszy

x=iks;
y=igrek;
glPushMatrix();
//glTranslatef(x,y,1.0f);
//CUTIL::drawBox();
glPopMatrix();}

};

vector<podloga> tab2;

void CGL::display( void ) {

	cout<<tab.size()<<endl; //wyświetlanie wielkości tablicy

	// kolor tła
	//glClearColor(0.0f, 0.0f, 1.0f, 0.0f); //blue
	//glClearColor(0.0f, 1.0f, 0.0f, 0.0f); //green
	//glClearColor(0.0f, 1.0f, 1.0f, 0.0f); //light blue
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f); //red
	//glClearColor(1.0f, 1.0f, 0.0f, 0.0f); //yellow
	//glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //purple
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //white
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //black

	static int board[23][23]={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					    {1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,1},
						{1,2,1,1,1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,2,1},
						{1,2,2,2,2,2,1,2,2,2,2,1,2,2,2,2,1,2,2,2,2,2,1},
						{1,1,1,2,1,2,1,2,1,1,1,1,1,1,1,2,1,2,1,2,1,1,1},
						{1,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,1},
						{1,2,1,1,1,2,1,1,1,1,2,1,2,1,1,1,1,2,1,1,1,2,1},
						{1,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,1},
						{1,1,1,1,1,2,1,0,1,1,1,1,1,1,1,0,1,2,1,1,1,1,1},
						{0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
						{1,1,1,1,1,2,1,0,1,1,1,1,1,1,1,0,1,2,1,1,1,1,1},
						{0,2,2,2,2,2,0,0,1,0,0,0,0,0,1,0,0,2,2,2,2,2,0},
						{1,1,1,1,1,2,1,0,1,1,0,0,0,1,1,0,1,2,1,1,1,1,1},
						{0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,1,2,1,0,0,0,0},
						{1,1,1,1,1,2,1,1,1,1,0,1,0,1,1,1,1,2,1,1,1,1,1},
						{1,2,2,2,2,2,1,2,2,2,2,1,2,2,2,2,1,2,2,2,2,2,1},
						{1,2,1,1,1,2,1,2,1,1,1,1,1,1,1,2,1,2,1,1,1,2,1},
						{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
						{1,2,1,1,1,2,1,1,1,1,2,1,2,1,1,1,1,2,1,1,1,2,1},
						{1,2,1,1,1,2,1,1,1,1,2,1,2,1,1,1,1,2,1,1,1,2,1},
						{1,0,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,0,1},
						{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	//float board_size=5.5;
	float board_size=5.2; //wielkość planszy

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wyczyszczenie bufora ramki (ang. frame buffer)

	float g_lightPos[4] = { 0, 0, -7, 0 };
	GLfloat color[] = { 0.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, g_lightPos);
	color[0] = color[1] = color[2] = 1.0f;
	glLightfv(GL_LIGHT2, GL_AMBIENT, color);
	color[0] = color[1] = color[2] = 1.0f;
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color);
	color[0] = color[1] = color[2] = 1.0f;
	glLightfv(GL_LIGHT2, GL_EMISSION, color);
	glLightfv(GL_LIGHT2, GL_SPECULAR, color);
	glEnable(GL_LIGHT2);

	float col[4] = { 1.0, 1.0, 0.1, 1.0 };
	glMaterialfv( GL_FRONT, GL_DIFFUSE, col);

	glPushMatrix();
	//glTranslatef(-3.0f,-2.5f,0.0f);

	// obrot calej sceny, parametry camRotateX i camRotateZ modyfikowane sa za pomoca myszy
	//glRotatef( camRotateX, 1, 0, 0 );
	//glRotatef( camRotateZ, 0, 0, 1 );
	glTranslatef(-3.175f,-2.5f,0.0f); //położenie planszy
	//glScalef(3.5f,3.5f,1.0f);
	glRotatef( 23.0f, 1, 0, 0 );
	glScalef(3.0f,3.05f,0.25f);

	if(zmienna==true){
	for (int rows=0;rows<22;rows++)
	{
	for (int cols=0;cols<23;cols++)
	{
		if (board[rows][cols]==1)
		{
			glPushMatrix(); //rysowanie ścian
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(0.0f,0.0f,1.0f);
			tab.push_back(kwadrat(cols,rows));
			glPopMatrix();
		}
		else
		{
			glPushMatrix(); //rysowanie podstawy planszy
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(0.0f,0.0f,1.0f);
			tab2.push_back(podloga(cols,rows));
			glPopMatrix();
		}

		/*if (board[rows][cols]==2)
		{
			glPushMatrix(); //rysowanie "kulek do zjadania"
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(0.0f,0.0f,0.0f);
			tabl.push_back(kulka(cols,rows));
			glPopMatrix();
		}*/
	}
	};
	zmienna=false;
	};

	for (int rows=0;rows<22;rows++)
	{
	for (int cols=0;cols<23;cols++)
	{
		if (board[rows][cols]==1)
		{
			glPushMatrix(); //rysowanie ścian
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(cols,rows,1.0f);
			//glutSolidCube(1.0f);
			glBindTexture(GL_TEXTURE_2D, sciana);
			CUTIL::drawBox();
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
		}
		else
		{
			glPushMatrix(); //rysowanie podstawy planszy
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(cols,rows,2.0f);
			glBindTexture(GL_TEXTURE_2D, ziemia);
			CUTIL::drawBox();
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
		}

		/*if (board[rows][cols]==2)
		{
			glPushMatrix(); //rysowanie "kulek do zjadania"
			glScalef(1.0f/(2.0f*float(board_size)),1.0f/(2.0f*float(board_size)),1.0f);
			glTranslatef(cols,rows,1.0f);
			glBindTexture(GL_TEXTURE_2D, kula);
			glutSolidSphere(0.2,10,10);
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
		}*/
	}
	};


	glPushMatrix(); //wstawienie gracza
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(playerPosX,playerPosY, 2.5f);
	glRotatef( 180.0f, 0, 0, 1 );
	draw_player(0.0f, 0.0f);
    glPopMatrix();

	glPushMatrix(); //wstawienie przeciwnika nr 1
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(eX1,eY1, 2.0f);
	//glRotatef( -90.0f, 1, 0, 0 );
	glRotatef( -90.0f + eMD1, 0, 0, 1 );
    draw_enymy( 0.0f, 0.0f );
    glPopMatrix();

	glPushMatrix(); //wstawienie przeciwnika nr 2
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(eX2,eY2, 2.0f);
    draw_enymy( 0.0f, 0.0f );
    glPopMatrix();

	glPushMatrix(); //wstawienie przeciwnika nr 3
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(eX3,eY3, 2.0f);
    draw_enymy( 0.0f, 0.0f );
    glPopMatrix();

	glPushMatrix(); //wstawienie przeciwnika nr 4
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(eX4,eY4, 2.0f);
    draw_enymy( 0.0f, 0.0f );
    glPopMatrix();

	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX1,bY1, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX2,bY2, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX3,bY3, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX4,bY4, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX5,bY5, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX6,bY6, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX7,bY7, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX8,bY8, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX9,bY9, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX10,bY10, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX11,bY11, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();
	glPushMatrix(); //wstawienie kulki
    glScalef(1.0f/(2.0f*board_size),1.0f/(2.0f*board_size),0.5f);
    glTranslatef(bX12,bY12, 2.0f);
    tabl.push_back(kulka(0.0f,0.0f));
    glPopMatrix();

   for(int i=0;i<tab.size();i++) //kolizje gracza ze ścianami
   {
   if(playerPosY>tab[i].y - 0.85f && playerPosY<tab[i].y + 0.85f && playerPosX<tab[i].x +0.85f && playerPosX>tab[i].x -0.85f)
   {
       if(isMoveUp==true)
       {
       isMoveUp=false;
       playerPosY-=0.15f;
       };
       if(isMoveDown==true)
       {
       isMoveDown=false;
       playerPosY+=0.15f;
       };
       if(isMoveLeft==true)
       {
       isMoveLeft=false;
       playerPosX-=0.15f;
       };
       if(isMoveRight==true)
       {
       isMoveRight=false;
       playerPosX+=0.15f;
       };
   };
};

   /*for(int i=0;i<tabl.size();i++)
   {
   if(playerPosY>tabl[i].y - 0.8f && playerPosY<tabl[i].y + 0.8f && playerPosX<tabl[i].x + 0.8f && playerPosX>tabl[i].x - 0.8f)
   {
	   if(board[(int)playerPosY][(int)playerPosX]==2){ //zjadanie kulek
			board[(int)playerPosY][(int)playerPosX]=0;
			zmienna2=zmienna2-1;}
   };
   };*/

	glPopMatrix();

	// rysowanie licznika czasu
	char str[50];
		sprintf( str, "Czas [s]: %d", /*counter,*/ time_end + (int)(time_counter/TIMER) );
	CUTIL::renderBitmapString( (board_width/2)-0.5, (board_height/2) + 0.8, 0, GLUT_BITMAP_HELVETICA_18	, str );

	//rysowanie licznika zjadania kulek
	char str2[200];
		sprintf(str2, "Pozostało kulek: %d", zmienna2);
	CUTIL::renderBitmapString( (board_width/2)-0.5, (board_height/2) + 0.5, 0, GLUT_BITMAP_HELVETICA_18	, str2 );


	// warunki i wyświetlenie informacji o zakończeniu gry informacji o zakonczeniu gry
	   if((eY1-0.6<playerPosY && eY1+0.6>playerPosY && eX1-0.6<playerPosX && eX1+0.6>playerPosX) || (eY2-0.6<playerPosY && eY2+0.6>playerPosY && eX2-0.6<playerPosX && eX2+0.6>playerPosX) ||
		   (eY3-0.6<playerPosY && eY3+0.6>playerPosY && eX3-0.6<playerPosX && eX3+0.6>playerPosX) || (eY4-0.6<playerPosY && eY4+0.6>playerPosY && eX4-0.6<playerPosX && eX4+0.6>playerPosX))
	{
	char stra[60];
		sprintf( stra, "Przegrales :(");
	CUTIL::renderBitmapString( (board_width/2)-3, (board_height/2) + 0.7, 0, GLUT_BITMAP_HELVETICA_18	, stra );
	koniec = 1;
	}
	   if(zmienna2==0)
	{
	char stra2[200];
		sprintf( stra2, "Wygrales :)");
	CUTIL::renderBitmapString( (board_width/2)-3, (board_height/2) + 0.7, 0, GLUT_BITMAP_HELVETICA_18	, stra2 );
	koniec = 1;
	}


	    if((bY1-0.6<playerPosY && bY1+0.6>playerPosY && bX1-0.6<playerPosX && bX1+0.6>playerPosX) || (bY2-0.6<playerPosY && bY2+0.6>playerPosY && bX2-0.6<playerPosX && bX2+0.6>playerPosX) ||
		   (bY3-0.6<playerPosY && bY3+0.6>playerPosY && bX3-0.6<playerPosX && bX3+0.6>playerPosX) || (bY3-0.6<playerPosY && bY3+0.6>playerPosY && bX3-0.6<playerPosX && bX3+0.6>playerPosX) ||
		   (bY4-0.6<playerPosY && bY4+0.6>playerPosY && bX4-0.6<playerPosX && bX4+0.6>playerPosX) || (bY5-0.6<playerPosY && bY5+0.6>playerPosY && bX5-0.6<playerPosX && bX5+0.6>playerPosX) ||
		   (bY6-0.6<playerPosY && bY6+0.6>playerPosY && bX6-0.6<playerPosX && bX6+0.6>playerPosX) || (bY7-0.6<playerPosY && bY7+0.6>playerPosY && bX7-0.6<playerPosX && bX7+0.6>playerPosX) ||
		   (bY8-0.6<playerPosY && bY8+0.6>playerPosY && bX8-0.6<playerPosX && bX8+0.6>playerPosX) || (bY9-0.6<playerPosY && bY9+0.6>playerPosY && bX9-0.6<playerPosX && bX9+0.6>playerPosX) ||
		   (bY10-0.6<playerPosY && bY10+0.6>playerPosY && bX10-0.6<playerPosX && bX10+0.6>playerPosX) || (bY11-0.6<playerPosY && bY11+0.6>playerPosY && bX11-0.6<playerPosX && bX11+0.6>playerPosX) ||
		   (bY12-0.6<playerPosY && bY12+0.6>playerPosY && bX12-0.6<playerPosX && bX12+0.6>playerPosX)){

		bX1=wkx;
		bY1=wky;
		bX2=wkx;
		bY2=wky;
		bX3=wkx;
		bY3=wky;
		bX4=wkx;
		bY4=wky;
		bX5=wkx;
		bY5=wky;
		bX6=wkx;
		bY6=wky;
		bX7=wkx;
		bY7=wky;
		bX8=wkx;
		bY8=wky;
		bX9=wkx;
		bY9=wky;
		bX10=wkx;
		bY10=wky;
		bX11=wkx;
		bY11=wky;
		bX12=wkx;
		bY12=wky;
		zmienna2=zmienna2-1;
	   }



	/*float radius = 0.3;

	if( (playerPosX+radius) > board_width/1.5 ) {
		playerMovementDirection = playerMovementDirection * -1;
		playerPosX = board_width/1.5 - (radius+0.1);
	}
	if( (playerPosX-radius) < -board_width/1.5 ) {
		playerMovementDirection = playerMovementDirection * -1;
		playerPosX = -board_width/1.5 + (radius+0.1);
	}

	if( (playerPosY+radius) > board_height/1.5 ) {
		playerMovementDirection = 180.0 - playerMovementDirection;
		playerPosY = board_height/1.5 - (radius+0.1);
	}
	if( (playerPosY-radius) < -board_height/1.5 ) {
		playerMovementDirection = 180.0 - playerMovementDirection;
		playerPosY = -board_height/1.5 + (radius+0.1);
	}

	if( playerMovementDirection >= 360.0f )
		playerMovementDirection = playerMovementDirection - 360.0f;
	if( playerMovementDirection < 0.0f )
		playerMovementDirection = playerMovementDirection + 360.0f; */

	/*
	// obrot calej sceny, parametry camRotateX i camRotateZ modyfikowane sa za pomoca myszy
	glRotatef( camRotateX, 1, 0, 0 );
	glRotatef( camRotateZ, 0, 0, 1 );


	// ustawianie polozenia swiatla
	float g_lightPos[4] = { 0.0f, 0.0f, -0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);

	// rysowanie planszy
	draw_board( board_width, board_height);

	// rysowanie gracza
	draw_player( playerPosX, playerPosY );

	// testowanie kolizji gracza ze scianami planszy i zmiana kierunku ruchu w przypadku wystapienia kolizji (odbicie od sciany planszy)
	float radius = 0.3;

	if( (playerPosX+radius) > board_width/2 ) {
		playerMovementDirection = playerMovementDirection * -1;
		playerPosX = board_width/2 - (radius+0.1);
	}
	if( (playerPosX-radius) < -board_width/2 ) {
		playerMovementDirection = playerMovementDirection * -1;
		playerPosX = -board_width/2 + (radius+0.1);
	}

	if( (playerPosY+radius) > board_height/2 ) {
		playerMovementDirection = 180.0 - playerMovementDirection;
		playerPosY = board_height/2 - (radius+0.1);
	}
	if( (playerPosY-radius) < -board_height/2 ) {
		playerMovementDirection = 180.0 - playerMovementDirection;
		playerPosY = -board_height/2 + (radius+0.1);
	}

	if( playerMovementDirection >= 360.0f )
		playerMovementDirection = playerMovementDirection - 360.0f;
	if( playerMovementDirection < 0.0f )
		playerMovementDirection = playerMovementDirection + 360.0f;

	// rysowanie obiektu przeciwnika
	draw_enymy( objPosX, objPosY );

	// testowanie kolizji przeciwnika ze scianami planszy i zmiana kierunku ruchu w przypadku wystapienia kolizji
	if( objPosX > board_width/2 )
		objPosX = board_width/2 - radius;
	if( objPosX < -board_width/2 )
		objPosX = -board_width/2 + radius;
	if( objPosY > board_height/2 )
		objPosY = board_height/2 - radius;
	if( objPosY < -board_height/2 )
		objPosY = -board_height/2 + radius;

	// rysowanie figur wyznaczajacych strefy wykrywania kolizji (funkcja testowa)
	draw_collisions_boxes();

	// wykrywanie kolizji gracza z (przeciwnikiem)
	bool bb = false;
	vector<int> obj_list = collision.check( OBJ_PLAYER );
	for( int i = 0; i < (int)obj_list.size(); i++ ) {
		if( obj_list[i] == OBJ_ENEMY ) {
			isCollision = true;
			bb = true;
		}
	}
	if( !bb && isCollision ) {
		isCollision = false;
		bb = false;
		counter++;
	}



	glPopMatrix();

	// rysowanie licznika oraz informacji o zakonczeniu gry
	char str[50];
	if( time_counter > time_end*TIMER )
		sprintf( str, "Liczba zderzen: %d   KONIEC", counter );
	else
		sprintf( str, "Liczba zderzen: %d   Czas [s]: %d", counter, time_end - (int)(time_counter/TIMER) );
	CUTIL::renderBitmapString( board_width/2, board_height/2 + 0.5, 0, GLUT_BITMAP_HELVETICA_18
		, str );

*/
	glutSwapBuffers(); // wyswietlenie tylniego ekranu


}

///////////////////////////////////////// IDLE
void CGL::idle( void ) {


}

void CGL::keyboardrelease(unsigned char key,int a, int b)
{
	/*switch(key){
	case 'w':
		isMoveUp=false;
		break;

	case 's':
		isMoveDown=false;
		break;

	case 'a':
		isMoveLeft=false;
		break;

	case 'd':
		isMoveRight=false;
		break;

	}*/
}


///////////////////////////////////////// obsluga klawiatury
void CGL::keyboard( unsigned char key,int a,int b ) {

	switch(key) {
		case 27:
			exit(0);
			break;
		case 'a': // zmiana kierunku ruchu czerwonej kuli/gracza
			isMoveUp=false;
			isMoveDown=false;
			isMoveRight=false;
			isMoveLeft=true;
			break;
		case 'd': // zmiana kierunku ruchu czerwonej kuli/gracza
			isMoveUp=false;
			isMoveDown=false;
			isMoveLeft=false;
			isMoveRight=true;
			break;
		case 's':
			isMoveLeft=false;
			isMoveRight=false;
			isMoveUp=false;
			isMoveDown=true;
			break;
		case 'w':
			isMoveLeft=false;
			isMoveRight=false;
			isMoveDown=false;
			isMoveUp=true;
			break;
	}


/*
	switch(key) {
		case 27:
			exit(0);
			break;
		case 's':
			if(isMove==false || fabs(pX-(int)pX)<0.1){
			wybor=1;
			isMove=true;}
			break;
		case 'w':
			if(isMove==false || fabs(pX-(int)pX)<0.1){
			wybor=2;
			isMove=true;
			}
			break;
		case 'd':
			if(isMove==false || fabs(pY-(int)pY)<0.1){
				wybor=3;
				isMove=true;
				cout<<pX<<"teleport"<<endl;
			}
			break;
		case 'a':
			if(isMove==false || fabs(pY-(int)pY)<0.1){
			wybor=4;
			isMove=true;
			}
			break;
		case '[': // zmiana kierunku ruchu czerwonej kuli/gracza
			playerMovementDirection += 6.0f;
			if( playerMovementDirection >= 360.0f )
				playerMovementDirection = playerMovementDirection - 360.0f;
			break;
		case ']': // zmiana kierunku ruchu czerwonej kuli/gracza
			playerMovementDirection -= 6.0f;
			if( playerMovementDirection < 0.0f )
				playerMovementDirection = playerMovementDirection + 360.0f;
			break;
	}
}*/

}



///////////////////////////////////////// timer
void CGL::timer( void ) {

if (koniec != 1){

	//RUCH GRACZA
	if(isMoveUp==true){
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosY+=0.125f;
		playerMovementDirection = 0.0f;
	};

	if(isMoveDown==true){
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosY-=0.125f;
		playerMovementDirection = 180.0f;
	};

	if(isMoveLeft==true){
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosX+=0.125f;
		playerMovementDirection = 90.0f;
	};

	if(isMoveRight==true){
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosX-=0.125f;
		playerMovementDirection = 270.0f;
	};

	if((playerPosX<=0.45f) && (playerPosY<=11.2f) && (playerPosY>=10.7f)){ //teleport gracza z prawej strony planszy do lewej
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosX=20.5f;
		playerPosY=11.04f;}

	else if((playerPosX>=21.75f) && (playerPosY<=11.2f) && (playerPosY>=10.7f)){ //teleport gracza z lewej strony planszy do prawej
		cout<<playerPosX<<" "<<playerPosY<<endl;
		playerPosX=1.5f;
		playerPosY=11.04f;}


	//OKRĘŻNY RUCH PRZECIWNIKA NR 1

	/*if(eX1<=10.0f && eY1==11.1f){ //w lewo
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 90.0f;
		eX1+=espeed;}
	else if(eX1>=10.0f && eY1<=14.95f){ //w gore
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 180.0f;
		eY1+=espeed;}
	else if(eX1>=7.0f && eY1>=14.95f){ //w prawo
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 270.0f;
		eX1-=espeed;}
	else if(eX1<=7.0f && eY1>=16.85f){ //w góre
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 180.0f;
		eY1+=espeed;}//---
	else if(eX1>=5.0f && eY1<=16.85f){ //w prawo
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 270.0f;
		eX1-=espeed;}

	else if(eX1<=5.0f && eY1>=7.0f){ //w dol
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 0.0f;
		eY1-=espeed;}
	else if(eY1<=7.0f){ //w prawo
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 270.0f;
		eX1-=espeed;}
	else if(eX1<=1.15f && eY1>=5.05f){ //w dol
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 0.0f;
		eY1-=espeed;}*/



//RUCH PRZECIWNIKA NR 1

    if(eX1<=10.0f && eY1==11.1f){ //w lewo
		//cout<<eX1<<" "<<eY1<<endl;
		eX1+=espeed;}
	else if(eX1>=10.0f && eY1<=14.95f){ //w gore
		//cout<<eX1<<" "<<eY1<<endl;
		eY1+=espeed;}
	else if(eX1>=7.0f && eY1>=14.95f){ //w prawo
		//cout<<eX1<<" "<<eY1<<endl;
		eX1-=espeed;}
	else if(eX1<=7.0f && eY1<=16.85f){ //w góre
		//cout<<eX1<<" "<<eY1<<endl;
		eY1+=espeed;}
	else if(eX1>=5.0f && eY1>=16.85f){ //w prawo
		//cout<<eX1<<" "<<eY1<<endl;
		eX1-=espeed;}
	else if(eX1<=5.0f && eY1<=20.0f){ //w góre
		//cout<<eX1<<" "<<eY1<<endl;
		eY1+=espeed;}
	else if(eX1>=1.0f && eY1>=20.0f){ //w prawo
		//cout<<eX1<<" "<<eY1<<endl;
		eX1-=espeed;}
	else if(eX1<=1.0f && eY1<=20.1f){ //w góre
		//cout<<eX1<<" "<<eY1<<endl;
		eX1=9.3f;
		eY1=11.1f;}

	/*else if(eX1<=5.0f && eY1>=7.0f){ //w dol
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 0.0f;
		eY1-=espeed;}
	else if(eX1>=5.0f && eY1<=7.0f){ //w prawo
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 270.0f;
		eX1-=espeed;}
	else if(eX1<=1.15f && eY1>=5.05f){ //w dol
		cout<<eX1<<" "<<eY1<<endl;
		eMD1 = 0.0f;
		eY1-=espeed;}*/

	//RUCH PRZECIWNIKA NR 2

	if((eX2==10.1f) && (eY2<=13.0f)){ //w gore
		//cout<<eX2<<" "<<eY2<<endl;
		eY2+=espeed;}
	else if((eX2>=6.9f) && (eY2>=13.0f)){ //w prawo
		//cout<<eX2<<" "<<eY2<<endl;
		eX2-=espeed;}
	else if((eX2<=6.9f) && (eY2>=7.1f)){ //w dół
		//cout<<eX2<<" "<<eY2<<endl;
		eY2-=espeed;}
	else if((eX2>=5.0f) && (eY2<=7.1f)){ //w prawo
		//cout<<eX2<<" "<<eY2<<endl;
		eX2-=espeed;}
	else if((eX2<=5.0f) && (eY2>=3.1f)){ //w dół
		//cout<<eX2<<" "<<eY2<<endl;
		eY2-=espeed;}
	else if((eX2>=1.0f) && (eY2<=3.1f)){ //w prawo
		//cout<<eX2<<" "<<eY2<<endl;
		eX2-=espeed;}
	else if((eX2<=1.0f) && (eY2>=1.0f)){ //w dół
		//cout<<eX2<<" "<<eY2<<endl;
		eY2-=espeed;}
	else if((eX2>=0.8f) && (eY2<=1.0f)){ //powrót
		eX2=10.1f;
		eY2=12.1f;}

	//RUCH PRZECIWNIKA NR 3
	if((eX3==11.8f) && (eY3<=13.0f)){ //w gore
		//cout<<eX3<<" "<<eY3<<endl;
		eY3+=espeed;}
	else if((eX3<=15.05f) && (eY3>=13.0f)){ //w lewo
		//cout<<eX3<<" "<<eY3<<endl;
		eX3+=espeed;}
	else if((eX3>=15.05f) && (eY3>=11.0f)){ //w dol
		//cout<<eX3<<" "<<eY3<<endl;
		eY3-=espeed;}
	else if((eX3<=17.0f) && (eY3<=11.0f)){ //w lewo
		//cout<<eX3<<" "<<eY3<<endl;
		eX3+=espeed;}
	else if((eX3>=17.0f) && (eY3>=3.1f)){ //w dol
		//cout<<eX3<<" "<<eY3<<endl;
		eY3-=espeed;}
	else if((eX3<=21.0f) && (eY3<=3.1f)){ //w lewo
		//cout<<eX3<<" "<<eY3<<endl;
		eX3+=espeed;}
	else if((eX3>=21.0f) && (eY3>=1.0f)){ //w dol
		//cout<<eX3<<" "<<eY3<<endl;
		eY3-=espeed;}
	else if((eX3<=21.1f) && (eY3<=1.0f)){ //w lewo
		//cout<<eX3<<" "<<eY3<<endl;
		eX3=11.8f;
		eY3=12.1f;}

	//RUCH PRZECIWNIKA NR 4
	if(eX4>=12.0f && eY4==11.1f){ //w prawo
		//cout<<eX4<<" "<<eY4<<endl;
		eX4-=espeed;}
	else if(eX4<=12.0f && eY4<=14.95f){ //w gore
		//cout<<eX4<<" "<<eY4<<endl;
		eY4+=espeed;}
	else if(eX4<=14.95f && eY4>=14.95f){ //w lewo
		//cout<<eX4<<" "<<eY4<<endl;
		eX4+=espeed;}
	else if(eX4>=14.95f && eY4<=17.0f){ //w gore
		//cout<<eX4<<" "<<eY4<<endl;
		eY4+=espeed;}
	else if(eX4<=21.0f && eY4>=17.0f){ //w lewo
		//cout<<eX4<<" "<<eY4<<endl;
		eX4+=espeed;}
	else if(eX4>=21.0f && eY4<=20.0f){ //w gore
		//cout<<eX4<<" "<<eY4<<endl;
		eY4+=espeed;}
	else if(eX4<=21.1f && eY4>=20.0f){ //w lewo
		//cout<<eX4<<" "<<eY4<<endl;
		eX4=12.6f;
		eY4=11.1f;}
	/*else if((eX3<=21.5f) && (eY3<=11.0f)){ //w lewo
		cout<<eX3<<" "<<eY3<<endl;
		eX3+=espeed;}
	else if((eX3>=21.5f) && (eY3>=10.9f)){ //teleport
		cout<<eX3<<" "<<eY3<<endl;
		eX3=1.0f;
		eY3=10.85f;}
	else if((eX3<=2.0f) && (eY3<=10.85f)){ //w dol
		cout<<eX3<<" "<<eY3<<endl;
		eY3-=espeed;}*/


	/*if(isMove==true){
		switch(wybor){
			case 1:
				if(board[(int)(pY-0.1)][(int)(pX)]!=1){
					pY-=0.1;
					if(board[(int)pY][(int)pX]==3)
						board[(int)pY][(int)pX]=0;
				}
				else
					isMove=false;
				break;
			case 2:
				if(board[(int)(pY+0.1)][(int)pX]!=1 && board[(int)(pY+1)][(int)pX]!=1){
					pY+=0.1;
					if(board[(int)pY][(int)pX]==3)
						board[(int)pY][(int)pX]=0;
				}
				else
					isMove=false;
				break;
			case 3:
				if(board[(int)(pY)][(int)(pX-0.1)]!=1){
					pX-=0.1;
					if(board[(int)pY][int(pX-1)]==2){
						pX=19;
					}
					if(board[(int)pY][(int)pX]==3)
						board[(int)pY][(int)pX]=0;
				}
				else
					isMove=false;
				break;
			case 4:
				if(board[(int)(pY)][(int)(pX+0.1)]!=1 && board[(int)(pY)][(int)(pX+1)]!=1){
					pX+=0.1;
					if(board[(int)pY][int(pX+1)]==2){
						pX=1;
					}
					if(board[(int)pY][(int)pX]==3)
						board[(int)pY][(int)pX]=0;
				}
				else
					isMove=false;
				break;
		}
	}

	glutPostRedisplay();*/


	// predkosc gracza i przeciwnika rosnaca wraz z uplywem czasu gry
	//float speed = (float)time_counter / (float)(time_end*TIMER) * 7.0f + 1.0f; // predkosc ruchu

	// autonomiczny ruch gracza
	//playerPosX += speed * 0.03f * sin( playerMovementDirection / 180.0f * 3.14f );
	//playerPosY += speed * 0.03f * cos( playerMovementDirection / 180.0f * 3.14f );

	// przeciwnik porusza sie w kierunku gracza
	//float dx = (playerPosX+board_width/2)  - (objPosX+board_width/2);
	//float dy = (playerPosY+board_height/2) - (objPosY+board_height/2);
	//float len = sqrt(dx*dx+dy*dy);
	//dx = dx / len;
	//dy = dy / len;
	//objPosX += dx * speed * 0.01f;
	//objPosY += dy * speed * 0.01f;

	// licznik czasu
	time_counter++;

	glutPostRedisplay();
}

};
///////////////////////////////////////// obsluga ruchu myszy
void CGL::mouseMotion( int x, int y ) {

	// zmiana polozenia planszy
	if( xOld >=0 ) {
		camRotateZ += ( xOld - x );
		if( camRotateZ < 0 )
			camRotateZ += 360;
		if( camRotateZ >= 360 )
			camRotateZ -= 360;
	}
	xOld = x;

	if( yOld >=0 ) {
		camRotateX += ( yOld - y );
		if( camRotateX < 0 )
			camRotateX += 360;
		if( camRotateX >= 360 )
			camRotateX -= 360;
	}
	yOld = y;

	glutPostRedisplay();

}

///////////////////////////////////////// obsluga wcisniecia klawisza myszy
void CGL::mousePress( int button, int state, int x, int y ) {

	if( button == GLUT_LEFT_BUTTON ) {
		xOld = -1;
		yOld = -1;
	}
}




