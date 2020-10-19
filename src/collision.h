#ifndef __COLLISION_H
#define __COLLISION_H

#include <iostream>
#include <map>
#include <vector>
#include "math.h"
using namespace std;

#define OBJ_SPHERE 1
#define OBJ_BOX 2

/** Klasa definiujaca obiekt figury zderzeniowej.
*/
class CObj {
public:
	int id;	/**< Indeks figury zderzeniowej. */
	int type; /**< Typ figury zderzeniowej (OBJ_SPHERE lub OBJ_BOX). */
	float pos[3];	// Polozenie srodka figury zderzeniowej. 
	float dims[3];	/**< Wymiary figury zderzeniowej. */
	float mmatrix[16];	/**< Macierz transformacji figury zderzeniowej. */
	float plane[3][5];	
	float points[8][4];	
};

/** Klasa wspomagajaca wykrywanie kolizji. Dzialanie klasy polega na utworzeniu figur zderzeniowych (prostopadloscianow) 
wokol wybranych obiektow. Klasa umozliwia sprawdzanie czy doszlo do zderzenia dwoch wybranych figur zderzeniowych.
*/
class CCollision {
private:
	map<int, CObj> mlist;	

public:

	~CCollision() {
		}

	//
	void add( CObj obj ) {
		mlist[obj.id] = obj;
		return;
	}

	// 
	void addBox( int id ) {
		
		float dims[3] = {1.0f,1.0f,1.0f};
		float mm[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, mm );
		addBox( id, dims, mm, true );
	}

	/** Utworzenie figury zderzeniowej (prostopadloscianu).
	@param id - indeks figury definiowany przez uzytkownika (powinien byc rozny dla kolejnych figur)
	@param dims[] - tablica z wymiarami figury (powinne byc dostosowane do rozmiarow obiektu)
	*/
	void addBox( int id, float dims[3] ) {
		
		float mm[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, mm );
		addBox( id, dims, mm, true );
	}

	/* Add bounding box to the collision list.
	*
	*	@param id - id number (different for each bounding box
	*	@param dims[] - bounding box dimensions
	*	@param mm[] - transformation matrix (bounding box is transformed based on this matrix)
	*	@param is_changable - if false the bounding box is static and cannot be changed, 
	*		otherwise calling addBox() next time for the same id can change position, transformation matrix 
	*		and/or dimension of the bounding box
	*/
	void addBox( int id, float dims[3], float mm[16], bool is_changable ) {
	
		map<int, CObj>::iterator it = mlist.find( id );
		if( it != mlist.end() ) {
			if( is_changable )
				mlist.erase( it );
			else
				return;
		}

		float pos[3] = {0,0,0};

		CObj obj;
		obj.id = id;
		obj.type = OBJ_BOX;

		for( int i = 0; i < 16; i++ ) 
			obj.mmatrix[i] = mm[i];

		for( int i = 0; i < 3; i++ ) {
			obj.pos[i] = pos[i];
			obj.dims[i] = dims[i];
		}

		// cube planes (3 pairs of planes)
		float normal[][4] = {{1.0f, 0.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f, 0.0f},{0.0f, 0.0f, 1.0f, 0.0f}};

		float pp1[][4] = {{ pos[0]-dims[0]/2, 0.0f, 0.0f, 1.0f},
							{ 0.0f, pos[1]-dims[1]/2, 0.0f, 1.0f},
							{ 0.0f, 0.0f, pos[2]-dims[2]/2, 1.0f}};
		
		float pp2[][4] = {{ pos[0]+dims[0]/2, 0.0f, 0.0f, 1.0f},
							{ 0.0f, pos[1]+dims[1]/2, 0.0f, 1.0f},
							{ 0.0f, 0.0f, pos[2]+dims[2]/2, 1.0f}};	

		for( int i = 0; i < 3; i++ ) {

			matrix_mul( normal[i], mm );
			obj.plane[i][0] = normal[i][0];
			obj.plane[i][1] = normal[i][1];
			obj.plane[i][2] = normal[i][2];

			matrix_mul( pp1[i], mm );
			obj.plane[i][3] = -( normal[i][0] * pp1[i][0] + normal[i][1] * pp1[i][1] + normal[i][2] * pp1[i][2] );
			matrix_mul( pp2[i], mm );
			obj.plane[i][4] = -( normal[i][0] * pp2[i][0] + normal[i][1] * pp2[i][1] + normal[i][2] * pp2[i][2] );
		}

		// cube vertex after transformation
		obj.points[0][0] = pos[0] - dims[0]/2;
		obj.points[0][1] = pos[1] - dims[1]/2;
		obj.points[0][2] = pos[2] - dims[2]/2;
		obj.points[0][3] = 1.0f;
		matrix_mul( obj.points[0], mm );
		obj.points[1][0] = pos[0] + dims[0]/2;
		obj.points[1][1] = pos[1] - dims[1]/2;
		obj.points[1][2] = pos[2] - dims[2]/2;
		obj.points[1][3] = 1.0f;
		matrix_mul( obj.points[1], mm );
		obj.points[2][0] = pos[0] - dims[0]/2;
		obj.points[2][1] = pos[1] + dims[1]/2;
		obj.points[2][2] = pos[2] - dims[2]/2;
		obj.points[2][3] = 1.0f;
		matrix_mul( obj.points[2], mm );
		obj.points[3][0] = pos[0] - dims[0]/2;
		obj.points[3][1] = pos[1] - dims[1]/2;
		obj.points[3][2] = pos[2] + dims[2]/2;
		obj.points[3][3] = 1.0f;
		matrix_mul( obj.points[3], mm );
		obj.points[4][0] = pos[0] + dims[0]/2;
		obj.points[4][1] = pos[1] + dims[1]/2;
		obj.points[4][2] = pos[2] - dims[2]/2;
		obj.points[4][3] = 1.0f;
		matrix_mul( obj.points[4], mm );
		obj.points[5][0] = pos[0] + dims[0]/2;
		obj.points[5][1] = pos[1] - dims[1]/2;
		obj.points[5][2] = pos[2] + dims[2]/2;
		obj.points[5][3] = 1.0f;
		matrix_mul( obj.points[5], mm );
		obj.points[6][0] = pos[0] - dims[0]/2;
		obj.points[6][1] = pos[1] + dims[1]/2;
		obj.points[6][2] = pos[2] + dims[2]/2;
		obj.points[6][3] = 1.0f;
		matrix_mul( obj.points[6], mm );
		obj.points[7][0] = pos[0] + dims[0]/2;
		obj.points[7][1] = pos[1] + dims[1]/2;
		obj.points[7][2] = pos[2] + dims[2]/2;
		obj.points[7][3] = 1.0f;
		matrix_mul( obj.points[7], mm );

		mlist[id] = obj;	
	}

	/** Wykrywanie kolizji. 
	@param id - indeks figury, dla ktorej sprawdzane sa kolizje
	@return Lista z indeksami figur, z ktorymi doszlo do zderzenia, jezeli lista jest pusta, zderzenia nie wystapily.
	*/
	vector<int> check( int id ) {
		vector<int> list_out;
		CObj* obj = &mlist[id];

		if( obj != NULL ) {
			
			CObj* lobj;
			map<int, CObj>::iterator it;
			for( it = mlist.begin(); it != mlist.end(); it++ ) {
				lobj = &it->second;
				if( lobj->id == obj->id ) // skip current object (obj)
					continue;
				
				if( lobj->type == OBJ_BOX ) {

					//cerr << lobj->id << endl;

					for( int i = 0; i < 8; i++ ) 
						if( point_in_box( lobj, obj->points[i] ) == true ) {
							list_out.push_back( lobj->id );
							break;
						}
				}
			}
		}
		return list_out;
	}


	/** Funkcja zwraca parametry figury zderzeniowej.
	@param id - indeks figury
	@return Wskaznik do obiektu.
	*/
	CObj* getObject( int id ) {

		CObj* obj = &mlist[id];

		return obj;
	}


private:
	// 
	bool point_between_planes( float plane1[4], float plane2, float pp[4] ) {

		//cerr << pp[0] << " " << pp[1] << " " << pp[2] << " " << pp[3] << endl;
		//cerr << "plane: " << plane1[0] << " " << plane1[1] << " " << plane1[2] << " " << plane1[3] << endl;
		//exit(1);

		float val1 = plane1[0] * pp[0] + plane1[1] * pp[1] + plane1[2] * pp[2] + plane1[3];
		float val2 = plane1[0] * pp[0] + plane1[1] * pp[1] + plane1[2] * pp[2] + plane2;

		if( ( val1 < 0 && val2 < 0 ) || ( val1 > 0 && val2 > 0 ) ) 
			return false;
		else {
			//cerr << "val1: " << val1 << "   val2: " << val2 << endl;
			return true;
		}
	}

	//
	bool point_in_box( CObj* obj, float pp[4] ) {
		
		for( int i = 0; i < 3; i++ ) 
			if( point_between_planes( obj->plane[i], obj->plane[i][4], pp ) == false ) 
				return false;

		return true;
	}

	//
	void matrix_mul( float pp[4], float mm[16] ) {

		float ret[4];

		for( int i = 0; i < 4; i++ ){
			ret[i] = 0;
			for( int j = 0; j < 4; j++ )
				ret[i] += mm[i+4*j] * pp[j];
		}
		for( int i = 0; i < 4; i++ )
			pp[i] = ret[i];

	}


};



#endif
