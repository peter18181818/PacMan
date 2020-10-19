#ifndef __GL_H
#define __GL_H

#include "collision.h"

/** Glowa klasa odpowiedzialna za sterowanie aplikacja i rysowanie grafiki. Klasa jest 'wrapperem' dla funkcji wywolywanych 
bezposrednio przez API OpenGL'a. Jej zastosowanie ma na celu wykorzystanie mechanizmow obiektowych do programowania OpenGL'a
(natywnie API OpenGL'a wymaga programowania w ANSI C)). W klasie znajduje sie szereg funkcji wywolywanych przez maszyne 
stanu OpenGL'a. Przykladem moze byc funkcja display() wywolywana w momencie, gdy wymagane jest przerysowanie ekranu.
*/
class CGL {

public: 
	static const int TIMER = 40; /**< Ustawienie interwalu zegara przerysowania grafiki (czas w [ms]). Im krotszy czas tym czesciej przerysowywany bedzie ekran.*/

private:
	int posX, posY;
	int time_counter; /**< Licznik czasu rosnacy w kazdym wywolaniu timer'a. */
	int time_end; /**< Liczba sekund do zakonczenia gry. */
	int counter; /**< Liczba zderzen miedzy graczem i przeciwnikiem. */

	GLuint texture; /**< Indeks tekstury. */
	
	GLuint aseGeometry; /**< Uchwyt do obiektu ASE. */

	int board_width; /**< Szerokosc planszy. */
	int board_height; /**< Wysokosc planszy. */

	float playerMovementDirection; /**< Kierunek ruchu gracza (w deg). */
	float playerPosX; /**< Polozenie gracza (wspolrzedna X). */
	float playerPosY; /**< Polozenie gracza (wspolrzedna Y). */

	float camRotateX; /**< Kat obrotu kamery wokol osi OX. */
	float camRotateZ; /**< Kat obrotu kamery wokol osi OZ. */
	int xOld, yOld;

	CCollision collision; /**<  Obiekt wykrywajacy kolizje. */
	bool isCollision; /**< Flaga wystapienia kolizji. */

	//float objPosX; /**< Pozycja przeciwnika (wspolrzedna X). */
	//float objPosY; /**< Pozycja przeciwnika (wspolrzedna Y). */
	bool isMoveUp;
	bool isMoveDown;
	bool isMoveLeft;
	bool isMoveRight;
	bool zmienna;
	bool koniec; 
	int zmienna2; //zmienna wykorzystywana w liczniku zakoñczenia gry i liczeniu zjedzonych kulek
	
	//zmienne do obs³ugi textur
	GLuint ziemia;
	GLuint sciana;
	GLuint kula;
	GLuint przeciwniktexture;
	GLuint gracztexture;

	//zmienne do obs³ugi modeli
	GLuint przeciwnik;
	GLuint gracz;
	
	int **board;
	int rows;
	int cols;

	//ZMIENNE RUCHU PRZECIWNIKA - DEKLARACJA ZMIENNYCH
	float espeed;
	float eX1, eX2, eX3, eX4;
	float eY1, eY2, eY3, eY4;
	float eMD1;

	//zmienne kulek - deklaracja
	float bX1,bX2,bX3,bX4,bX5,bX6,bX7,bX8,bX9,bX10,bX11,bX12;
	float bY1,bY2,bY3,bY4,bY5,bY6,bY7,bY8,bY9,bY10,bY11,bY12;

	float wkx;
	float wky;

public:

	CGL() {
		time_counter = 0;
		time_end = 0;
		counter = 0;

		texture = 1;

		camRotateX = 0;
		camRotateZ = 0;
		xOld = -1;
		yOld = -1;

		board_width = 6.0f;
		board_height = 4.0f;

		//pocz¹tkowa pozycja gracza
		//playerPosX = 10.75f;
		//playerPosY = 9.0f;
		playerPosX=1.0f;
		playerPosY=1.0f;
		playerMovementDirection = 90.0f; //zmienna do obracania gracza w kierunku ruchu

		//objPosX = 0.0f;
		//objPosY = 0.0f;
		isMoveUp=false;
		isMoveDown=false;
		isMoveLeft=false;
		isMoveRight=false;
		
		zmienna=true;
		zmienna2=189;
		isCollision = false;

		//ZMIENNE RUCHU PRZECIWNIKÓW - PRZYPISANIE WARTOŒCI POCZATKOWYCH
		espeed=0.125f; //prêdkoœæ przeciwników
		eX1=9.3f;
		eY1=11.1f;
		//eMD1 = 90.0f; //zmienna do obracania przeciwników w kierunku ruchu

		eX2=10.1f;
		eY2=12.1f;

		eX3=11.8f;
		eY3=12.1f;
		
		eX4=12.6f;
		eY4=11.1f;

		//zmienne kulek - definicja
		bX1=1.0f;
		bY1=1.0f;
		bX2=2.0f;
		bY2=1.0f;
		bX3=3.0f;
		bY3=1.0f;
		bX4=4.0f;
		bY4=1.0f;
		bX5=5.0f;
		bY5=1.0f;
		bX6=6.0f;
		bY6=1.0f;
		bX7=7.0f;
		bY7=1.0f;
		bX8=8.0f;
		bY8=1.0f;
		bX9=9.0f;
		bY9=1.0f;
		bX10=10.0f;
		bY10=1.0f;
		bX11=11.0f;
		bY11=1.0f;
		bX12=12.0f;
		bY12=1.0f;

		wkx=30.0f;
		wky=30.0f;

		


	}

	/** Inicjalizacja srodowiska graficznego: wczytanie tekstury, ustawienie kamery i swiatel, wczytanie obiektu ASE. Inicjalizacja
		odbywa sie tylko raz po uruchomieniu programu.
	*/
	void init( void );
	void keyboardrelease(unsigned char key,int a,int b);
	
	/** Glowna procedura rysowania grafiki. Wywolywana jest wtedy, gdy ekran wymaga przerysowania (np. obiekt zmienia swoje polozenie, okno bylo zasloniete przez inna aplikacje, itp.).  
	*/
	void display( void );
	
	/** Funkcja wywolywana w przypadku braku aktywnosci innych elementow programu (obecnie funkcja nie jest wykorzystywana). 
	*/
	void idle( void );
	
	/** Funkcja wywolywana cyklicznie co pewien interwal czasu (zobacz TIMER). Odbywa sie w niej zmiana polozenia gracza i przeciwnika oraz zwiekszany jest licznik czasu. 
	*/
	void timer( void );
	
	/** Funkcja obslugujaca klawiature. Zmiana kierunku ruchu gracza nastepuje po nacisnieciu klawiszy '[' oraz ']'. Klawisz 'Esc' przerywa gre.
	*/
	void keyboard( unsigned char key,int a,int b );
	
	/** Obsluga ruchy myszy. Zmiana polozenia kursora myszy z jednoczesnym wcisnieciem lewego przycisku myszy powoduje obrot planszy.
	*/
	void mouseMotion( int x, int y );
	
	/** Obsluga klawiszy myszy, funkcja wywolywana jest po nacisnieciu jednego z klawiszy myszy. Wspomaganie obracania planszy.
	*/
	void mousePress( int button, int state, int x, int y );

	/** Rysowanie gracza oraz definiowanie figury kolizyjnej dla gracza. W przypadku wystapienia kolizji gracza z przeciwnikiem, obiekt gracza (czajnik) zamieniany jest na kule.
	*/
	void draw_player( float x, float y );
	
	/** Rysowanie planszy. 
		@param board_width - szerokosc planszy
		@param board_height - wysokosc planszy
	*/
	void draw_board( float board_width, float board_height );
	
	/** Rysowanie przeciwnika. Obiekt przeciwnika wczytany byl z pliku ASE (patrz metoda init()). 
	*/
	void draw_enymy( float pos_x, float pos_y );
	
	/** Rysowanie figur kilizyjnych dla gracza i przeciwnika (funkcja pomocnicza, powinna byc wylaczona w koncowej wersji gry). 
	*/
	void draw_collisions_boxes( void );

};

extern CGL cgl;

#endif
