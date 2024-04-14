#pragma once
//#include "Vars.h"

typedef struct {									// Virtual road structure
	unsigned char VergeLeft; // Distance to left Verge
	unsigned char VergeRight; // Distance to right Verge
	unsigned char BarrierLeft; // Distance to left Barrier
	unsigned char BarrierRight; // Distance to right Barrier
	char Flag1;
	char Flag2;
	char Flag3;
	char Flag4;
	int  X; //coordinate X
	int  Z; //coordinate Z
	int  Y; //coordinate Y
	short int slope; // slope
	short int slantA; // slant-A
	short int orientation; // orientation
	short int zero; // blank 0
	short int Yorientation; // y-orientation
	short int slantB; // slant-B
	short int Xorientation; // x-orientation
	short int zero1; // blank 0
	unsigned int refnode; // refnode
	} VirtRd_t;	

unsigned int virtRDlengh = 0;

typedef struct {									// Object structure
      int RefNode; // reference node
      short int BitmapN; // bitmap number
      short int Flip; // flip
      short int Xcoord; // relative x coordinate
      short int Zcoord; // relative z coordinate
      short int Ycoord; // relative y coordinate
	float  X; //absolute coordinate X
	float  Z; //absolute coordinate Z
	float  Y; //absolutecoordinate Y
	float Width;
	float Height;
	int SceneryType; 
	int SceneryFlags;
	int rID;
	int rID2;
	int flags;
	} Object_t;

typedef struct {			// Object params structure
	unsigned char Width;
	unsigned char Height;
	unsigned char SceneryType; 
	unsigned char SceneryFlags;
	unsigned char rID;
	unsigned char rID2;
		char p1;
		short int p2;
		short int p3;
		short int p4;
		char p5;
	} ObjectParams_t;

typedef struct {			// Traffic array
	unsigned char Speed; // Traffic car speed 
	unsigned char CopsSpeed; // Cops trigger speed
	unsigned char Speed2;  // Traffic car speed 2
	} TrafficArray_t;

typedef struct {									// Scenery block structure
    unsigned int CurrentNum; // number of this record (0 for the first, then 1, etc...)
    unsigned char Texture1;  // texture #1
	unsigned char Texture2;  // texture #2
	unsigned char Texture3;  // texture #3
	unsigned char Texture4;  // texture #4
	unsigned char Texture5;  // texture #5
	unsigned char Texture6;  // texture #6
    unsigned char Texture7;  // texture #7
	unsigned char Texture8;  // texture #8
	unsigned char Texture9;  // texture #9
	unsigned char Texture10;  // texture #10
	bool HasLeftFence, HasRightFence;
	char RawFence;
	int FenceTexID;
//----------------
    int PointA0X;  // point A0 X coord 
	int PointA0Z;  // point A0 Z coord
	int PointA0Y;  // point A0 Y coord

	int PointA1X;  // point A1 X coord
	int PointA1Z;  // point A1 Z coord
	int PointA1Y;  // point A1 Y coord

	int PointA2X;  // point A2 X coord
	int PointA2Z;  // point A2 Z coord
	int PointA2Y;  // point A2 Y coord

	int PointA3X;  // point A3 X coord
	int PointA3Z;  // point A3 Z coord
	int PointA3Y;  // point A3 Y coord

	int PointA4X;  // point A4 X coord
	int PointA4Z;  // point A4 Z coord
	int PointA4Y;  // point A4 Y coord

	int PointA5X;  // point A5 X coord
	int PointA5Z;  // point A5 Z coord
	int PointA5Y;  // point A5 Y coord

	int PointA6X;  // point A6 X coord
	int PointA6Z;  // point A6 Z coord
	int PointA6Y;  // point A6 Y coord

	int PointA7X;  // point A7 X coord
	int PointA7Z;  // point A7 Z coord
	int PointA7Y;  // point A7 Y coord

	int PointA8X;  // point A8 X coord
	int PointA8Z;  // point A8 Z coord
	int PointA8Y;  // point A8 Y coord

	int PointA9X;  // point A9 X coord
	int PointA9Z;  // point A9 Z coord
	int PointA9Y;  // point A9 Y coord

	int PointA10X;  // point A10 X coord
	int PointA10Z;  // point A10 Z coord
	int PointA10Y;  // point A10 Y coord
//----------------

    int PointB0X;  // point B0 X coord
	int PointB0Z;  // point B0 Z coord
	int PointB0Y;  // point B0 Y coord

	int PointB1X;  // point B1 X coord
	int PointB1Z;  // point B1 Z coord
	int PointB1Y;  // point B1 Y coord

	int PointB2X;  // point B2 X coord
	int PointB2Z;  // point B2 Z coord
	int PointB2Y;  // point B2 Y coord

	int PointB3X;  // point B3 X coord
	int PointB3Z;  // point B3 Z coord
	int PointB3Y;  // point B3 Y coord

	int PointB4X;  // point B4 X coord
	int PointB4Z;  // point B4 Z coord
	int PointB4Y;  // point B4 Y coord

	int PointB5X;  // point B5 X coord
	int PointB5Z;  // point B5 Z coord
	int PointB5Y;  // point B5 Y coord

	int PointB6X;  // point B6 X coord
	int PointB6Z;  // point B6 Z coord
	int PointB6Y;  // point B6 Y coord

	int PointB7X;  // point B7 X coord
	int PointB7Z;  // point B7 Z coord
	int PointB7Y;  // point B7 Y coord

	int PointB8X;  // point B8 X coord
	int PointB8Z;  // point B8 Z coord
	int PointB8Y;  // point B8 Y coord

	int PointB9X;  // point B9 X coord
	int PointB9Z;  // point B9 Z coord
	int PointB9Y;  // point B9 Y coord

	int PointB10X;  // point B10 X coord
	int PointB10Z;  // point B10 Z coord
	int PointB10Y;  // point B10 Y coord
//----------------


	int PointC0X;  // point C0 X coord
	int PointC0Z;  // point C0 Z coord
	int PointC0Y;  // point C0 Y coord

	int PointC1X;  // point C1 X coord
	int PointC1Z;  // point C1 Z coord
	int PointC1Y;  // point C1 Y coord

	int PointC2X;  // point C2 X coord
	int PointC2Z;  // point C2 Z coord
	int PointC2Y;  // point C2 Y coord

	int PointC3X;  // point C3 X coord
	int PointC3Z;  // point C3 Z coord
	int PointC3Y;  // point C3 Y coord

	int PointC4X;  // point C4 X coord
	int PointC4Z;  // point C4 Z coord
	int PointC4Y;  // point C4 Y coord

	int PointC5X;  // point C5 X coord
	int PointC5Z;  // point C5 Z coord
	int PointC5Y;  // point C5 Y coord

	int PointC6X;  // point C6 X coord
	int PointC6Z;  // point C6 Z coord
	int PointC6Y;  // point C6 Y coord

	int PointC7X;  // point C7 X coord
	int PointC7Z;  // point C7 Z coord
	int PointC7Y;  // point C7 Y coord

	int PointC8X;  // point C8 X coord
	int PointC8Z;  // point C8 Z coord
	int PointC8Y;  // point C8 Y coord

	int PointC9X;  // point C9 X coord
	int PointC9Z;  // point C9 Z coord
	int PointC9Y;  // point C9 Y coord

	int PointC10X;  // point C10 X coord
	int PointC10Z;  // point C10 Z coord
	int PointC10Y;  // point C10 Y coord
//----------------


	int PointD0X;  // point D0 X coord
	int PointD0Z;  // point D0 Z coord
	int PointD0Y;  // point D0 Y coord

	int PointD1X;  // point D1 X coord
	int PointD1Z;  // point D1 Z coord
	int PointD1Y;  // point D1 Y coord

	int PointD2X;  // point D2 X coord
	int PointD2Z;  // point D2 Z coord
	int PointD2Y;  // point D2 Y coord

	int PointD3X;  // point D3 X coord
	int PointD3Z;  // point D3 Z coord
	int PointD3Y;  // point D3 Y coord

	int PointD4X;  // point D4 X coord
	int PointD4Z;  // point D4 Z coord
	int PointD4Y;  // point D4 Y coord

	int PointD5X;  // point D5 X coord
	int PointD5Z;  // point D5 Z coord
	int PointD5Y;  // point D5 Y coord

	int PointD6X;  // point D6 X coord
	int PointD6Z;  // point D6 Z coord
	int PointD6Y;  // point D6 Y coord

	int PointD7X;  // point D7 X coord
	int PointD7Z;  // point D7 Z coord
	int PointD7Y;  // point D7 Y coord

	int PointD8X;  // point D8 X coord
	int PointD8Z;  // point D8 Z coord
	int PointD8Y;  // point D8 Y coord

	int PointD9X;  // point D9 X coord
	int PointD9Z;  // point D9 Z coord
	int PointD9Y;  // point D9 Y coord

	int PointD10X;  // point D10 X coord
	int PointD10Z;  // point D10 Z coord
	int PointD10Y;  // point D10 Y coord
//----------------


	int PointE0X;  // point E0 X coord
	int PointE0Z;  // point E0 Z coord
	int PointE0Y;  // point E0 Y coord

	int PointE1X;  // point E1 X coord
	int PointE1Z;  // point E1 Z coord
	int PointE1Y;  // point E1 Y coord

	int PointE2X;  // point E2 X coord
	int PointE2Z;  // point E2 Z coord
	int PointE2Y;  // point E2 Y coord

	int PointE3X;  // point E3 X coord
	int PointE3Z;  // point E3 Z coord
	int PointE3Y;  // point E3 Y coord

	int PointE4X;  // point E4 X coord
	int PointE4Z;  // point E4 Z coord
	int PointE4Y;  // point E4 Y coord

	int PointE5X;  // point E5 X coord
	int PointE5Z;  // point E5 Z coord
	int PointE5Y;  // point E5 Y coord

	int PointE6X;  // point E6 X coord
	int PointE6Z;  // point E6 Z coord
	int PointE6Y;  // point E6 Y coord

	int PointE7X;  // point E7 X coord
	int PointE7Z;  // point E7 Z coord
	int PointE7Y;  // point E7 Y coord

	int PointE8X;  // point E8 X coord
	int PointE8Z;  // point E8 Z coord
	int PointE8Y;  // point E8 Y coord

	int PointE9X;  // point E9 X coord
	int PointE9Z;  // point E9 Z coord
	int PointE9Y;  // point E9 Y coord

	int PointE10X;  // point E10 X coord
	int PointE10Z;  // point E10 Z coord
	int PointE10Y;  // point E10 Y coord

	int refpointX24; // Reference point on 0x24 = 0x27C (n-1) offsets. = e0 point???
	int refpointZ24;
	int refpointY24;

	int refpointXB4; // Reference point on 0xB4 offsets.
	int refpointZB4;
	int refpointYB4;

	int refpointX144; // Reference point on 0x144 offsets.
	int refpointZ144;
	int refpointY144;

	int refpointX1D4; // Reference point on 0x1D4 offsets.
	int refpointZ1D4;
	int refpointY1D4;
	//----------------
	} Scenery_t;


Object_t OBJArray[numOBJIndexes] = {0};
static VirtRd_t *VirtRd;
static Object_t *Object;
static Scenery_t *Scenery;
unsigned int FileSize;
int NextL;
float ArrayXYZ[numVirtRdIndexes][3] = {0}; // Record #, X, Y, Z
float ArrayVirtRd[numVirtRdIndexes][15] = {0}; // Record#, A0X; A0Y; A0Z; A1X; A1Y; A1Z; A2X; A2Y; A2Z; A3X; A3Y; A3Z; XCenterABS; YCenterABS; ZCenterABS;
float ArrayAngleXY[2400] = {0}; // Record #, AngleXY
const int SIZE = 20000; // размер массива
int arrTextures[SIZE] = {0}; // Store for all texture indexes of the track. Unsorted
unsigned short int arrFenceTex[1000];// Store for all fence texture indexes of the track. Unsorted
unsigned short int arrObjTex[1000];// Store for all object texture indexes of the track. Unsorted
unsigned short int arrObjTex2[1000];// Store for all object texture indexes of the track. Unsorted
int iC=0; // for the arrTextures array
int VRArrFlag1 [numVirtRdIndexes] = {0};
int VRArrFlag2 [numVirtRdIndexes] = {0};
int VRArrFlag3 [numVirtRdIndexes] = {0};
int VRArrFlag4 [numVirtRdIndexes] = {0};
//int iCF=0; // for the arrFenceTextures array


// Create array of VirtRd structs
extern VirtRd_t VIRTUALROADARRAY[numVirtRdIndexes];
// Create array of Objects entries
extern Object_t OBJECTSARRAY[numOBJIndexes];
// Create array of Objects params entries
extern ObjectParams_t OBJECTPARAMSARRAY[numOBJParams];
// Create array of Traffic entries
extern TrafficArray_t TRAFFICARRAY[numSceneryIndexes];
// Create array of Scenery blocks
extern Scenery_t SCENERYARRAY[numSceneryIndexes];
