// Based on http://auroux.free.fr/nfs/nfsspecs.txt
//#include "FAM.cpp"
#include "stdafx.h"
#include <iostream>
#include "burger.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <cmath>
#include <math.h>
#include <cstring>
#include <string> 
#include <fstream> 
#include <list>
#include <algorithm>
#include <vector>
#include "Vars.h"
#include "NFS_TRI2OBJ.h"
//#include "NFS_TRI2OBJ.cpp"

#include <direct.h>
//#include <unistd.h>
//#include<sys/stat.h>
//#include<sys/types.h>


using namespace std;
void ExtractScenTexture(int textN, char* infilename);
void ExtractObjectTexture(int textN, char* infilename);
void ExtractObject3DTexture(int textN, char* infilename);
void ExtractHorz(char *infilename);
void MakeHorz(FILE *inputfile);
int Extract3D(float angle, float X, float Y, float Z, int BitmapN, char* infilename, int counter, int vtcounter, int flag, char* FamFileName3DO);
void Extract3DOTextures(char* infilename);
void ExtractTRKDs3DO(char* infilename);
void Extract3DOObjTexture(int textN, char* FamFileName3DO, int chunkN);
void Extract3DOSCNTexture(int textN, char* FamFileName3DO, int chunkN);
void Extract3DOHorz(char* infilename);
void ExtractObject3D3DOTexture(int textN, char* FamFileName3DO, int offset2SHPIdir, int textureValue);
void ExtractReversedTrack(char *infilename);

extern int size13DObj;
extern int size83DObj;
extern int vt3DOcounter;
VirtRd_t VIRTUALROADARRAY[numVirtRdIndexes];
Object_t OBJECTSARRAY[numOBJIndexes];
ObjectParams_t OBJECTPARAMSARRAY[numOBJParams];
TrafficArray_t TRAFFICARRAY[numSceneryIndexes];
Scenery_t SCENERYARRAY[numSceneryIndexes];
int trackEndVr = 0; // Value for the last non-zero Virtual road data


//#define Swap4Bytes(val) val = ( (((val) >> 24) & 0x000000FF) | (((val) >> 8) & 0x0000FF00) | (((val) <<  8) & 0x00FF0000) | (((val) << 24) & 0xFF000000) )
//#define Swap8Bytes(val) ( (((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) | (((val) >> 24) & 0x0000000000FF0000) | (((val) >>  8) & 0x00000000FF000000) | (((val) <<  8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) | (((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000) )
//#define Swap2Bytes(val) ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

//#define BUFFER_SIZE 0x400000UL
//#define F_EPS 1e-4

static Byte *Buffer; /* File buffer for data transfer */

//	FILE *inputfile, *VirtRdOBJ; // Файл трека
//	char *infilename; // TRI filename
//	char *famfilename; // Textures file

#if 0;
typedef struct {									// Virtual road structure
   
//	unsigned short int VergeLeft; // Distance to left Verge
//	unsigned short int VergeRight; // Distance to right Verge
//	unsigned short int BarrierLeft; // Distance to left Barrier
//	unsigned short int BarrierRight; // Distance to right Barrier

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
	short int Yorientation; // y-orientation
	short int slantB; // slant-B
	short int Xorientation; // x-orientation
	} VirtRd_t;	
#endif;

#if 0;
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
	} Object_t;
#endif;

#if 0;
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
//	Byte fenceType;
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
//----------------
	} Scenery_t;
#endif;

#if 0;
const double pi = 3.1415926535897932384626433832795;
const int SCALE = 1000000; // Scale for the Object file
const int SCALETRACK = 10000; // Scale for the width of the track (Unused)
const int SCALETILT = 120; // Scale for the width of the track
const float SCALEOBJECT = 0.06; // Scale for the objects
const float SCALEROTATEOBJECT = 10; // Scale for the object rotation
const int SCALEOBJ = 3800; // Scale for the object placement ///////////////////CHECK!
const int numVirtRdIndexes = 2400; // Quantity of the Virtual Road records
const int numOBJIndexes = 1000; // Quantity of the Objects records
const float FENCEHEIGHT = 0.12; // Fence height (scenery block)
#endif;

//#define degreesToRadians(angleDegrees) ((angleDegrees) * pi / 180.0)
//#define radiansToDegrees(angleRadians) ((angleRadians) * 180.0 / pi)

#if 0;
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
#endif;

#if 1;
//////// FOR checking intersections
class Vector3{
public:
  float x,y,z;
  Vector3(){}
  Vector3(float _x, float _y, float _z){x=_x; y=_y; z=_z;}
  float len2(){return x*x + y*y + z*z;}
  float len(){return sqrt(len2());}
  void normalize(){float l=sqrt(x*x+y*y+z*z); x/=l; y/=l; z/=l;}
  float operator * (Vector3 arg){return x*arg.x + y*arg.y + z*arg.z;}
  Vector3 operator = (Vector3 src){x=src.x; y=src.y; z=src.z; return *this;}
  Vector3 operator + (Vector3 arg){Vector3 tmp(x+arg.x, y+arg.y, z+arg.z); return tmp;}
  Vector3 operator - (Vector3 arg){Vector3 tmp(x-arg.x, y-arg.y, z-arg.z); return tmp;}
  Vector3 operator * (float l){Vector3 tmp(x*l, y*l, z*l); return tmp;} //scalar multiply
};
 
/*
 * ray_st, ray_dir - start point and direction of the ray
 * sp_cen, sp_rad - center point and radius of the sphere
 * res1, res2 - intersection points (if there is less than 2 intersections - no changes)
 * return value - number of intersections
 */
#endif;


// ф-я вывода
void print(const int * arr, const size_t size = SIZE){
    for (size_t i = 0; i < size; i++)
        cout << "[" << arr[i] << "]";
     cout << endl;
}

// ф-я ищет дубликаты в массивы с позиции (current_posizito)
bool isNextExist(const int * arr, const size_t current_posiziton, const int find_number){
    for (size_t i = current_posiziton + 1; i < SIZE; i++)
        if (arr[i] == find_number)
            return true;
    return false;
}

// ф-я перемещает элемент с указанной позиции в конец массива
void moveInEnd(int * arr, const size_t position){
    int tmp = arr[position];
    for (size_t i = position; i < SIZE; i++)
        arr[i] = arr[i + 1];
    arr[SIZE - 1] = tmp;
}

// ф-я помещает уникальные элементы в начале массива и возвращает их количество
const size_t unique(int * arr){
    size_t len = SIZE;

    for (size_t i = 0; i < len; i++){
        if (isNextExist(arr, i, arr[i])){
            moveInEnd(arr, i);
            //print(arr); // раскоментировать для отладки
            len--;
            i = 0;
            continue;
        }
    }
    return len;
}



// Returns array of vertices based on texture number and 10 points 
//void TexAssign(int tex, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, 
//			   float x5, float y5, float z5, float x6, float y6, float z6, float x7, float y7, float z7, float x8, float y8, float z8, float x9, float y9, float z9, float x10, float y10, float z10) {
void TexAssign(unsigned char tex) {
FILE *outf3, *mtlfile;
bool unique = false;
char str[20], basestr[20], mtlN[20], cuttedStr[2];
unsigned char texN;
int matNfromMTLFile=0;
char *s = new char [1024];

// Store every tex in the array.
arrTextures[iC] = tex;
//cout << arrTextures[iC] << endl;
iC++;

#if 0; //////// *************** /////////////
// Make original string
strcpy(basestr, "newmtl ");
itoa(tex, mtlN, 10);
strcat(basestr, mtlN);
//cout << "origstr = " << basestr << "\n";

//outf3 = fopen("TextureCoords.txt","a");
//mtlfile = fopen("Scenery.mtl","r");
//fseek(mtlfile,0,SEEK_SET);		// Set to the begining

ifstream file("Scenery.mtl");
//if (getline(file, s) != 0) goto endfile;
// Read all file. Make sure that mtl is unique.
while (!file.eof()) {
        file.getline(s, 1024, '\n');
        iC++;
//		cout << i << endl;
//		cout << s << endl;
		if (iC%2 == 0) { // It's even line
//			cout << "It's even line" << endl;
			} else {
//					cout << "It's odd line" << endl;
///*//				if (strncmp(s, basestr, 20) != 0) {
					strncpy(cuttedStr, s+7, 3);
					matNfromMTLFile = atoi(cuttedStr);
//					cout << "cuttedStr = " << cuttedStr << endl;
					arrTextures[iC] = matNfromMTLFile; // Send this tex number to the array.
					cout << iC << endl;
					cout << basestr << endl;
					cout << s << endl;
						// Check others
						#if 1;
						// Compare current tex with every in arrTextures[] array.
						for (int j=0; j<sizeof(arrTextures); j++) {

/////							if (arrTextures[j] == tex) {
								///	unique = true;
								///	goto endfile;
/////								}
							}
						#endif; // Check others
///					unique = true;
///					goto endfile;
//					break;
				}
///*//			}
}
//fclose(mtlfile);
endfile:
file.close();
	#if 1;
	if (unique) {
		// If mtl is unique, add it to the file. And corresponding texture too.
		mtlfile = fopen("Scenery1.mtl","a");
		fprintf(mtlfile, "newmtl %d\n", tex);
		//fprintf(mtlfile, "CurrentNum %d\n", Scenery->CurrentNum);
		//fputs("map_Kd C:\\Users\\Versus\\Desktop\\Tex\n",mtlfile);
		fprintf(mtlfile, "map_Kd C:\\Users\\Versus\\Desktop\\Tex%d.bmp\n", tex);
		fclose(mtlfile);
		unique = false;
		}
	#endif;
//fclose(outf3);
#endif; //////// *************** /////////////
}

#if 1;
int find_intersection(Vector3 ray_st, Vector3 ray_dir, Vector3 sp_cen, float sp_rad, Vector3 &res1, Vector3 &res2){
  Vector3 middle; //middle point between intersections
  Vector3 dist; //vector from center of sphere to ray start
  Vector3 dpos; //vector from ray_st to sp_cen
  float len2; //square of distance from center of sphere to ray
  ray_dir.normalize();
  sp_rad *= sp_rad;
  
  dpos = sp_cen - ray_st;
  middle = ray_st + ray_dir*(ray_dir*dpos);
  len2 = (middle - sp_cen).len2();
  
  if(len2 > sp_rad)return 0;
  if(fabs( len2 - sp_rad )<F_EPS){
    res1 = middle;
    return 1;
  }
  len2 = sqrt(sp_rad - len2);
  //now len2 is distance, not its square
  res1 = middle - ray_dir*len2;
  res2 = middle + ray_dir*len2;
  return 2;
}
#endif;

//int getNum(char ch);

//VirtRd_t *VirtualRdArray[2400];
//Object_t ObjArray[10];
//Scenery_t ScenArray[10];


//function : getNum
//this function will return number corresponding
//0,1,2..,9,A,B,C,D,E,F
 
int getNum(char ch)
{
    int num=0;
    if(ch>='0' && ch<='9')
    {
        num=ch-0x30;
    }
    else
    {
        switch(ch)
        {
            case 'A': case 'a': num=10; break;
            case 'B': case 'b': num=11; break;
            case 'C': case 'c': num=12; break;
            case 'D': case 'd': num=13; break;
            case 'E': case 'e': num=14; break;
            case 'F': case 'f': num=15; break;
            default: num=0;
        }
    }
    return num;
}


// Function to convert hexadecimal to decimal 
int hexadecimalToDecimal(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 



// Initialize data
void InitVars(void) 
{
FILE *outf;
char *outfilename;

NextL = -1;
FileSize = 0;
#if 1;
// Reset VirtRd data:
	VirtRd->VergeLeft = 0; // Distance to left Verge
	VirtRd->VergeRight = 0; // Distance to right Verge
	VirtRd->BarrierLeft = 0; // Distance to left Barrier
	VirtRd->BarrierRight = 0; // Distance to right Barrier
	VirtRd->Flag1 = 0;
	VirtRd->Flag2 = 0;
	VirtRd->Flag3 = 0;
	VirtRd->Flag4 = 0;
	VirtRd->X = 0;
	VirtRd->Z = 0;
	VirtRd->Y = 0;
	VirtRd->slope = 0;
	VirtRd->slantA = 0;
	VirtRd->orientation = 0;
	VirtRd->Yorientation = 0;
	VirtRd->slantB = 0;
	VirtRd->Xorientation = 0;
#endif;
// Reset Object data:
	Object->RefNode = 0; // reference node
	Object->BitmapN = 0; // bitmap number
	Object->Flip = 0;    // flip
	Object->Xcoord = 0;  // relative x coordinate
    Object->Zcoord = 0;  // relative z coordinate
    Object->Ycoord = 0;  // relative y coordinate

// Reset Scenery data:
	Scenery->CurrentNum = 0; // number of this record (0 for the first, then 1, etc...)
    Scenery->Texture1 = 0;  // texture #1
	Scenery->Texture2 = 0;  // texture #2
	Scenery->Texture3 = 0;  // texture #3
	Scenery->Texture4 = 0;  // texture #4
	Scenery->Texture5 = 0;  // texture #5
	Scenery->Texture6 = 0;  // texture #6
    Scenery->Texture7 = 0;  // texture #7
	Scenery->Texture8 = 0;  // texture #8
	Scenery->Texture9 = 0;  // texture #9
	Scenery->Texture10 = 0;  // texture #10
//----------------

	Scenery->PointA0X = 0;  // point A0 X coord
	Scenery->PointA0Z = 0;  // point A0 Z coord
	Scenery->PointA0Y = 0;  // point A0 Y coord

	Scenery->PointA1X = 0;  // point A1 X coord
	Scenery->PointA1Z = 0;  // point A1 Z coord
	Scenery->PointA1Y = 0;  // point A1 Y coord

	Scenery->PointA2X = 0;  // point A2 X coord
	Scenery->PointA2Z = 0;  // point A2 Z coord
	Scenery->PointA2Y = 0;  // point A2 Y coord

	Scenery->PointA3X = 0;  // point A3 X coord
	Scenery->PointA3Z = 0;  // point A3 Z coord
	Scenery->PointA3Y = 0;  // point A3 Y coord

	Scenery->PointA4X = 0;  // point A4 X coord
	Scenery->PointA4Z = 0;  // point A4 Z coord
	Scenery->PointA4Y = 0;  // point A4 Y coord

	Scenery->PointA5X = 0;  // point A5 X coord
	Scenery->PointA5Z = 0;  // point A5 Z coord
	Scenery->PointA5Y = 0;  // point A5 Y coord

	Scenery->PointA6X = 0;  // point A6 X coord
	Scenery->PointA6Z = 0;  // point A6 Z coord
	Scenery->PointA6Y = 0;  // point A6 Y coord

	Scenery->PointA7X = 0;  // point A7 X coord
	Scenery->PointA7Z = 0;  // point A7 Z coord
	Scenery->PointA7Y = 0;  // point A7 Y coord

	Scenery->PointA8X = 0;  // point A8 X coord
	Scenery->PointA8Z = 0;  // point A8 Z coord
	Scenery->PointA8Y = 0;  // point A8 Y coord

	Scenery->PointA9X = 0;  // point A9 X coord
	Scenery->PointA9Z = 0;  // point A9 Z coord
	Scenery->PointA9Y = 0;  // point A9 Y coord

	Scenery->PointA10X = 0;  // point A10 X coord
	Scenery->PointA10Z = 0;  // point A10 Z coord
	Scenery->PointA10Y = 0;  // point A10 Y coord
//----------------

    Scenery->PointB0X = 0;  // point B0 X coord
	Scenery->PointB0Z = 0;  // point B0 Z coord
	Scenery->PointB0Y = 0;  // point B0 Y coord

	Scenery->PointB1X = 0;  // point B1 X coord
	Scenery->PointB1Z = 0;  // point B1 Z coord
	Scenery->PointB1Y = 0;  // point B1 Y coord

	Scenery->PointB2X = 0;  // point B2 X coord
	Scenery->PointB2Z = 0;  // point B2 Z coord
	Scenery->PointB2Y = 0;  // point B2 Y coord

	Scenery->PointB3X = 0;  // point B3 X coord
	Scenery->PointB3Z = 0;  // point B3 Z coord
	Scenery->PointB3Y = 0;  // point B3 Y coord

	Scenery->PointB4X = 0;  // point B4 X coord
	Scenery->PointB4Z = 0;  // point B4 Z coord
	Scenery->PointB4Y = 0;  // point B4 Y coord

	Scenery->PointB5X = 0;  // point B5 X coord
	Scenery->PointB5Z = 0;  // point B5 Z coord
	Scenery->PointB5Y = 0;  // point B5 Y coord

	Scenery->PointB6X = 0;  // point B6 X coord
	Scenery->PointB6Z = 0;  // point B6 Z coord
	Scenery->PointB6Y = 0;  // point B6 Y coord

	Scenery->PointB7X = 0;  // point B7 X coord
	Scenery->PointB7Z = 0;  // point B7 Z coord
	Scenery->PointB7Y = 0;  // point B7 Y coord

	Scenery->PointB8X = 0;  // point B8 X coord
	Scenery->PointB8Z = 0;  // point B8 Z coord
	Scenery->PointB8Y = 0;  // point B8 Y coord

	Scenery->PointB9X = 0;  // point B9 X coord
	Scenery->PointB9Z = 0;  // point B9 Z coord
	Scenery->PointB9Y = 0;  // point B9 Y coord

	Scenery->PointB10X = 0;  // point B10 X coord
	Scenery->PointB10Z = 0;  // point B10 Z coord
	Scenery->PointB10Y = 0;  // point B10 Y coord
//----------------

	Scenery->PointC0X = 0;  // point C0 X coord
	Scenery->PointC0Z = 0;  // point C0 Z coord
	Scenery->PointC0Y = 0;  // point C0 Y coord

	Scenery->PointC1X = 0;  // point C1 X coord
	Scenery->PointC1Z = 0;  // point C1 Z coord
	Scenery->PointC1Y = 0;  // point C1 Y coord

	Scenery->PointC2X = 0;  // point C2 X coord
	Scenery->PointC2Z = 0;  // point C2 Z coord
	Scenery->PointC2Y = 0;  // point C2 Y coord

	Scenery->PointC3X = 0;  // point C3 X coord
	Scenery->PointC3Z = 0;  // point C3 Z coord
	Scenery->PointC3Y = 0;  // point C3 Y coord

	Scenery->PointC4X = 0;  // point C4 X coord
	Scenery->PointC4Z = 0;  // point C4 Z coord
	Scenery->PointC4Y = 0;  // point C4 Y coord

	Scenery->PointC5X = 0;  // point C5 X coord
	Scenery->PointC5Z = 0;  // point C5 Z coord
	Scenery->PointC5Y = 0;  // point C5 Y coord

	Scenery->PointC6X = 0;  // point C6 X coord
	Scenery->PointC6Z = 0;  // point C6 Z coord
	Scenery->PointC6Y = 0;  // point C6 Y coord

	Scenery->PointC7X = 0;  // point C7 X coord
	Scenery->PointC7Z = 0;  // point C7 Z coord
	Scenery->PointC7Y = 0;  // point C7 Y coord

	Scenery->PointC8X = 0;  // point C8 X coord
	Scenery->PointC8Z = 0;  // point C8 Z coord
	Scenery->PointC8Y = 0;  // point C8 Y coord

	Scenery->PointC9X = 0;  // point C9 X coord
	Scenery->PointC9Z = 0;  // point C9 Z coord
	Scenery->PointC9Y = 0;  // point C9 Y coord

	Scenery->PointC10X = 0;  // point C10 X coord
	Scenery->PointC10Z = 0;  // point C10 Z coord
	Scenery->PointC10Y = 0;  // point C10 Y coord
//----------------

	Scenery->PointD0X = 0;  // point D0 X coord
	Scenery->PointD0Z = 0;  // point D0 Z coord
	Scenery->PointD0Y = 0;  // point D0 Y coord

	Scenery->PointD1X = 0;  // point D1 X coord
	Scenery->PointD1Z = 0;  // point D1 Z coord
	Scenery->PointD1Y = 0;  // point D1 Y coord

	Scenery->PointD2X = 0;  // point D2 X coord
	Scenery->PointD2Z = 0;  // point D2 Z coord
	Scenery->PointD2Y = 0;  // point D2 Y coord

	Scenery->PointD3X = 0;  // point D3 X coord
	Scenery->PointD3Z = 0;  // point D3 Z coord
	Scenery->PointD3Y = 0;  // point D3 Y coord

	Scenery->PointD4X = 0;  // point D4 X coord
	Scenery->PointD4Z = 0;  // point D4 Z coord
	Scenery->PointD4Y = 0;  // point D4 Y coord

	Scenery->PointD5X = 0;  // point D5 X coord
	Scenery->PointD5Z = 0;  // point D5 Z coord
	Scenery->PointD5Y = 0;  // point D5 Y coord

	Scenery->PointD6X = 0;  // point D6 X coord
	Scenery->PointD6Z = 0;  // point D6 Z coord
	Scenery->PointD6Y = 0;  // point D6 Y coord

	Scenery->PointD7X = 0;  // point D7 X coord
	Scenery->PointD7Z = 0;  // point D7 Z coord
	Scenery->PointD7Y = 0;  // point D7 Y coord

	Scenery->PointD8X = 0;  // point D8 X coord
	Scenery->PointD8Z = 0;  // point D8 Z coord
	Scenery->PointD8Y = 0;  // point D8 Y coord

	Scenery->PointD9X = 0;  // point D9 X coord
	Scenery->PointD9Z = 0;  // point D9 Z coord
	Scenery->PointD9Y = 0;  // point D9 Y coord

	Scenery->PointD10X = 0;  // point D10 X coord
	Scenery->PointD10Z = 0;  // point D10 Z coord
	Scenery->PointD10Y = 0;  // point D10 Y coord
//----------------

	Scenery->PointE0X = 0;  // point E0 X coord
	Scenery->PointE0Z = 0;  // point E0 Z coord
	Scenery->PointE0Y = 0;  // point E0 Y coord

	Scenery->PointE1X = 0;  // point E1 X coord
	Scenery->PointE1Z = 0;  // point E1 Z coord
	Scenery->PointE1Y = 0;  // point E1 Y coord

	Scenery->PointE2X = 0;  // point E2 X coord
	Scenery->PointE2Z = 0;  // point E2 Z coord
	Scenery->PointE2Y = 0;  // point E2 Y coord

	Scenery->PointE3X = 0;  // point E3 X coord
	Scenery->PointE3Z = 0;  // point E3 Z coord
	Scenery->PointE3Y = 0;  // point E3 Y coord

	Scenery->PointE4X = 0;  // point E4 X coord
	Scenery->PointE4Z = 0;  // point E4 Z coord
	Scenery->PointE4Y = 0;  // point E4 Y coord

	Scenery->PointE5X = 0;  // point E5 X coord
	Scenery->PointE5Z = 0;  // point E5 Z coord
	Scenery->PointE5Y = 0;  // point E5 Y coord

	Scenery->PointE6X = 0;  // point E6 X coord
	Scenery->PointE6Z = 0;  // point E6 Z coord
	Scenery->PointE6Y = 0;  // point E6 Y coord

	Scenery->PointE7X = 0;  // point E7 X coord
	Scenery->PointE7Z = 0;  // point E7 Z coord
	Scenery->PointE7Y = 0;  // point E7 Y coord

	Scenery->PointE8X = 0;  // point E8 X coord
	Scenery->PointE8Z = 0;  // point E8 Z coord
	Scenery->PointE8Y = 0;  // point E8 Y coord

	Scenery->PointE9X = 0;  // point E9 X coord
	Scenery->PointE9Z = 0;  // point E9 Z coord
	Scenery->PointE9Y = 0;  // point E9 Y coord

	Scenery->PointE10X = 0;  // point E10 X coord
	Scenery->PointE10Z = 0;  // point E10 Z coord
	Scenery->PointE10Y = 0;  // point E10 Y coord

// Rewrite VirtRdOBJ file for future use
outfilename = "VirtRdOBJ.obj";
outf = fopen(outfilename,"w");
//fwrite("test",4,1,outf);
fclose(outf);

// Rewrite VirtRdSpline file for future use
outfilename = "VirtRdSpline.obj";
outf = fopen(outfilename,"w");
//fwrite("test",4,1,outf);
fclose(outf);

// Rewrite VirtRdOBJ material file for future use
outfilename = "VirtRdOBJ.mtl";
outf = fopen(outfilename,"w");
//fwrite("test",4,1,outf);
fclose(outf);

// Rewrite VirtRdOBJ flags debug file for future use
outfilename = "VirtRdFlags.txt";
outf = fopen(outfilename,"w");
//fwrite("test",4,1,outf);
fclose(outf);

// Rewrite Test file for future use
outfilename = "Al1_new.trk";
outf = fopen(outfilename,"wb");
fclose(outf);

// Rewrite Objects file for future use
outfilename = "Objects.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Scenery file for future use
outfilename = "Scenery.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Fences file for future use
outfilename = "Fence.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Test file for future use
outfilename = "Objects_test.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Test file for future use
//outfilename = "TextureCoords.txt";
//outf = fopen(outfilename,"w");
//fclose(outf);

// Rewrite Scenery material file for future use
outfilename = "Scenery.mtl";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Fence material file for future use
outfilename = "Fence.mtl";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Objects material file for future use
outfilename = "Objects.mtl";
outf = fopen(outfilename,"w");
fclose(outf);


// Rewrite Horizon file for future use
outfilename = "Horizon.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite Horizon material file for future use
outfilename = "Horizon.mtl";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite 3DObjects file for future use
outfilename = "Object3D.obj";
outf = fopen(outfilename,"w");
fclose(outf);

// Rewrite 3DObjects material file for future use
outfilename = "Obj3D.mtl";
outf = fopen(outfilename,"w");
fclose(outf);
}


// Virtual Road
void ParseVirtualRoad(FILE* inputfile)
{
//
unsigned int temp;
FILE *outf, *outf2, *outf3, *outf4, *outf5, *outfileforNewFlags;
char *outfilename, *outfilename2, *outfilename3, *outfilename4, *outfilename5, *outfilename6;
int i, Length, startOffset;
int version, verBuffer;
int j; // Check for 36 bytes
int k; // For checking array fill
int m; // Quantity of real chars to store in file
int n; // for resetting ForFileRecord array
int CounterBlocks=0;
double XCenter, YCenter, ZCenter, FloatVergeLeft, FloatVergeRight, FloatBarrierLeft, FloatBarrierRight; // Floating params of the VirtRd
double Floatslope, FloatslantA, Floatorientation, FloatYorientation, FloatslantB, FloatXorientation; // Floating params of the VirtRd
double OrientXYAngle; 
double SlantBAngle;
double OrthoTrack; // Perpendicular to the track
double dirVectorX, dirVectorY, dirVectorZ; // Vector direction. Perpendicular to the track
double a0x;
double a0y;
double a0z;
double a1x;
double a1y;
double a1z;
double a2x; // Coords for the road verges and barriers
double a2y;
double a2z;
double a3x;
double a3y;
double a3z;

double A0X;
double A0Y;
double A0Z;
double A1X;
double A1Y;
double A1Z;
double A2X; // Absolute coords for the road verges and barriers
double A2Y;
double A2Z;
double A3X;
double A3Y;
double A3Z;

double XCenterABS; // Absolute coords for the center point
double YCenterABS;
double ZCenterABS;
int b=0; // For centerline file VirtRDSpline.obj
// For polygon store
float A0X_prev, A0Y_prev, A0Z_prev, A1X_prev, A1Y_prev, A1Z_prev, A2X_prev, A2Y_prev, A2Z_prev, A3X_prev, A3Y_prev, A3Z_prev, XCenterABS_Prev, YCenterABS_Prev, ZCenterABS_Prev;

char buffer[128]; /* Buffer fo the fwrite */
int LinesCount = 0; // lines counter
int p = 0; // For the polygon counter
int y = 0; // For the polygon counter
bool flag3DO = false;
//string *arrayRGB = new string [2400];
char *arrayRGB[numVirtRdIndexes];
int arrayRGB2[numVirtRdIndexes];
int arrayRGB3[numVirtRdIndexes];
int arrayRGB4[numVirtRdIndexes];
int arrayRGB5[numVirtRdIndexes];

char flag1 = 3; // Traffic, Fence distance??
char flag2 = 6; // Fence distance??
char flag3 = 6;
char flag4 = 3;
bool testFlags = 1; //////////////////////////////////// TURN THIS ON TO SAVE SEPARATE 3DO TRK FILE WITH NEW FLAGS (SEE ABOVE)
//for (int f4=0; f4<numVirtRdIndexes; f4++) {
//	arrayRGB[f4] = ' ';
//	} 

//float LeftVergeSphereRadius; // For checking intersections between left verge sphere and perpendicular of the track
//float RightVergeSphereRadius; // For checking intersections between right verge sphere and perpendicular of the track
//float LeftBarrierSphereRadius; // For checking intersections between left barrier sphere and perpendicular of the track
//float RightBarrierSphereRadius; // For checking intersections between right barrier sphere and perpendicular of the track

char String[20] = "";
char String2[5] = "";
char ForFileRecord[8] = "";
//char FourBytes[4] = "";
//char TwoBytes[2] = "";


fseek(inputfile, 0, SEEK_END);  
FileSize = ftell(inputfile);
//cout << "FileSize = " << FileSize << "\n";

fseek(inputfile,0,SEEK_SET);

// Reading Virtual Road structure segments
//Records are starting at 0x12F8, finishing at 0x16468. Possible error in Unofficial specs. 36*2400 = 86400 = 0x15180
// For the SE edition of NFS records starts at 0x98C!
// For the 3DO version start at 0x13B4 finish at 0x16530
#if 1;
fread(&verBuffer,4,1,inputfile);
version = verBuffer;
//cout << "\nversion =" << version <<"\n";
	if (version == 0x10) { // NFS original version
			startOffset = 0x12f8;
			cout << "++Original NFS virtual road++\n";
		} 
	if (version == 0x11) {
			startOffset = 0x98C;
			cout << "++SE NFS virtual road++\n";
		}

		if (version == 0xE000000) {
			startOffset = 0x13B4;
			flag3DO = true;
			version3DO = true;
			cout << "++3DO NFS virtual road++\n";
		}
#endif; // version	

if ((flag3DO)&&(testFlags)) {
fseek(inputfile,0,SEEK_SET);		// Set to the begining

// Read and write all data before Virtual road.
outfilename6 = "Al1_new.trk"; // New TRK file
outfileforNewFlags = fopen(outfilename6,"wb");


//Read Virtual road from input file and write it to al1_new.trk file using new Flags.
//fread(&temp,sizeof(char),1,inputfile);
for (int p=0; p<1261; p++) {
	fread(&temp,4,1,inputfile);
	fwrite(&temp, 4, 1, outfileforNewFlags);

	}
}
//fwrite(&flag1, 1, 1, outfileforNewFlags);

//Read and write all data after Virtual road.



// Back to input file Virtual Road section for track export.
fseek(inputfile,0,SEEK_SET);		// Set to the begining
fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
//fseek(inputfile,0x98c,SEEK_CUR); // Setting to the starting position

//Length = 0x16478-0x12f8; // Total data to read
//Debug******** cout << "Length of the object data is " << Length << "\n";


#if 1;
outfilename = "VirtRdVergesAndBarriers.txt"; // File for debuggig. VirtualRd data
outf = fopen(outfilename,"w");

outfilename2 = "VirtRdOBJ.obj"; // File for the VirtualRd data
outf2 = fopen(outfilename2,"a");
fwrite("# Virtual Road data\n",20,1,outf2);

outfilename3 = "VirtRdSpline.obj"; // File for the VirtualRd data (spline)
outf3 = fopen(outfilename3,"a");
fwrite("# Virtual Road Center spline data\n",34,1,outf3);

outfilename4 = "VirtRdOBJ.mtl"; // File for the VirtualRd data (materials)
outf4 = fopen(outfilename4,"a");
fwrite("# Materials based on Virtual road flags\n",40,1,outf4);

outfilename5 = "VirtRdFlags.txt"; // File for the VirtualRd data (flags)
outf5 = fopen(outfilename5,"a");
fwrite("# Virtual road flags\n",21,1,outf5);

//fwrite("o ",2,1,outf2); // TEMP
//fwrite("Name",4,1,outf2); // TEMP

// Create array of VirtRd structs
//VirtRd_t VIRTUALROADARRAY[numVirtRdIndexes]; 

for (i=1; i<=numVirtRdIndexes; i++)	{
//numVirtRdIndexes = 2400
j = 0; // Counter only
// Reading all Virtual Road data
//Debug********				cout << dec << "\n\n\n-= Virtual Road data =- #" << i << "\n";
		VIRTUALROADARRAY[i].refnode = i;

		fread(&temp,sizeof(char),1,inputfile); // Reading boarders
		VirtRd->VergeLeft = temp;
		VIRTUALROADARRAY[i].VergeLeft = VirtRd->VergeLeft; 
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 1, 1, outfileforNewFlags);
		}
//Debug********				cout << "VirtRd->VergeLeft = 0x" << hex << (int)VirtRd->VergeLeft << "\n";

		fread(&temp,sizeof(char),1,inputfile);
		VirtRd->VergeRight = temp;
		VIRTUALROADARRAY[i].VergeRight = VirtRd->VergeRight; 
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 1, 1, outfileforNewFlags);
		}
//Debug********				cout << "VirtRd->VergeRight = 0x" << hex << (int)VirtRd->VergeRight << "\n";

		fread(&temp,sizeof(char),1,inputfile);
		VirtRd->BarrierLeft = temp;
		VIRTUALROADARRAY[i].BarrierLeft = VirtRd->BarrierLeft;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 1, 1, outfileforNewFlags);
		}
//Debug********				cout << "VirtRd->BarrierLeft = 0x" << hex << (int)VirtRd->BarrierLeft << "\n";

		fread(&temp,sizeof(char),1,inputfile);
		VirtRd->BarrierRight = temp;
		VIRTUALROADARRAY[i].BarrierRight = VirtRd->BarrierRight;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 1, 1, outfileforNewFlags);
		}
//Debug********				cout << "VirtRd->BarrierRight = 0x" << hex << (int)VirtRd->BarrierRight << "\n";
j+=4; // Add 4 bytes

		fread(&temp,sizeof(char),1,inputfile); // Reading 4 flags
		VirtRd->Flag1 = temp;
		VIRTUALROADARRAY[i].Flag1 = VirtRd->Flag1;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&flag1, 1, 1, outfileforNewFlags);
		}
			VRArrFlag1[i] = VirtRd->Flag1; // For future use with ParseScenery.
//Debug********		cout << "VirtRd->Flag1 = 0x" << hex << (int)VirtRd->Flag1 << "\n";
		fread(&temp,sizeof(char),1,inputfile); // Reading 4 flags
		VirtRd->Flag2 = temp;
		VIRTUALROADARRAY[i].Flag2 = VirtRd->Flag2;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&flag2, 1, 1, outfileforNewFlags);
		}
			VRArrFlag2[i] = VirtRd->Flag2; // For future use with ParseScenery.
//Debug********		cout << "VirtRd->Flag2 = 0x" << hex << (int)VirtRd->Flag2 << "\n";
		fread(&temp,sizeof(char),1,inputfile); // Reading 4 flags
		VirtRd->Flag3 = temp;
		VIRTUALROADARRAY[i].Flag3 = VirtRd->Flag3;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&flag3, 1, 1, outfileforNewFlags);
		}
			VRArrFlag3[i] = VirtRd->Flag3; // For future use with ParseScenery.
//Debug********		cout << "VirtRd->Flag3 = 0x" << hex << (int)VirtRd->Flag3 << "\n";
		fread(&temp,sizeof(char),1,inputfile); // Reading 4 flags
		VirtRd->Flag4 = temp; // TrackNode property!
		VIRTUALROADARRAY[i].Flag4 = VirtRd->Flag4;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&flag4, 1, 1, outfileforNewFlags);
		}
			VRArrFlag4[i] = VirtRd->Flag4; // For future use with ParseScenery.
//			cout << "i=" << dec << i<< endl;
//			cout << "fl=" << dec << (int)VirtRd->Flag4 << " ";
//Debug********		cout << "VirtRd->Flag4 = 0x" << hex << (int)VirtRd->Flag4 << "\n";
j+=4; // Add 4 bytes


		fread(&temp,sizeof(int),1,inputfile); // Now reading coords
j+=4; // Add 4 bytes
		VirtRd->X = temp;						// Store X coord
		VIRTUALROADARRAY[i].X = VirtRd->X;
//		cout << "VIRTUALROADARRAY[i].X = 0x" << hex << VIRTUALROADARRAY[i].X << "\n\n";
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 4, 1, outfileforNewFlags);
		}
			if (flag3DO) {
				Swap4Bytes(VirtRd->X);
//				VIRTUALROADARRAY[i].X = VirtRd->X;
//				cout << "VIRTUALROADARRAY[i].X = 0x" << hex << VIRTUALROADARRAY[i].X << "\n";
			}
//				cout << "VirtRd->X = " <<  VirtRd->X << endl << endl;	
//				cout << "VirtRd->X = 0x" << hex << VirtRd->X << "\n";
//				cout << "VIRTUALROADARRAY[i].X = 0x" << hex << VIRTUALROADARRAY[i].X << "\n";
		fread(&temp,sizeof(int),1,inputfile);
j+=4; // Add 4 bytes
		VirtRd->Z = temp;						// Store Z coord
		VIRTUALROADARRAY[i].Z = VirtRd->Z;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 4, 1, outfileforNewFlags);
		}
		if (flag3DO) {
//				fwrite(&temp, 4, 1, outfileforNewFlags);
				Swap4Bytes(VirtRd->Z);
		}
//Debug********				cout << "VirtRd->Z = " << VirtRd->Z << "\n";
				//cout << "VirtRd->Z = 0x" << hex << VirtRd->Z << "\n";
		fread(&temp,sizeof(int),1,inputfile);
j+=4; // Add 4 bytes
		VirtRd->Y = temp;						// Store Y coord
		VIRTUALROADARRAY[i].Y = VirtRd->Y;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 4, 1, outfileforNewFlags);
		}
		if (flag3DO) {
//				fwrite(&temp, 4, 1, outfileforNewFlags);
				Swap4Bytes(VirtRd->Y);
		}
//Debug********				cout << "VirtRd->Y = " << VirtRd->Y << "\n";
				//cout << "VirtRd->Y = 0x" << hex << VirtRd->Y << "\n";


		// Now reading slope
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->slope = temp;
		VIRTUALROADARRAY[i].slope = VirtRd->slope;
			if ((flag3DO)&&(testFlags)) {
				fwrite(&temp, 2, 1, outfileforNewFlags);
			}
				Swap2Bytes(VirtRd->slope);
//Debug********				cout << "VirtRd->slope = 0x" << hex << VirtRd->slope << "\n";

		// Now reading slant-A
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->slantA = temp;
		VIRTUALROADARRAY[i].slantA = VirtRd->slantA;
			if ((flag3DO)&&(testFlags)) {
				fwrite(&temp, 2, 1, outfileforNewFlags);
			}
				Swap2Bytes(VirtRd->slantA);
//Debug********				cout << "VirtRd->slantA = 0x" << hex << VirtRd->slantA << "\n";

		// Now reading orientation
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->orientation = temp;
		VIRTUALROADARRAY[i].orientation = VirtRd->orientation;
			if ((flag3DO)&&(testFlags)) {
				fwrite(&temp, 2, 1, outfileforNewFlags);
			}
			if (flag3DO==true) {
//				fwrite(&temp, 2, 1, outfileforNewFlags);
				VirtRd->orientation = Swap2Bytes(VirtRd->orientation);
			}
				Swap2Bytes(VirtRd->orientation);
//Debug********				cout << "VirtRd->orientation = 0x" << hex << VirtRd->orientation << "\n";

		// SKIP 2 bytes
		fread(&temp,sizeof(short int),1,inputfile);
		VIRTUALROADARRAY[i].zero = temp;
		if ((flag3DO)&&(testFlags)) {
		fwrite(&temp, 2, 1, outfileforNewFlags);
		}
j+=2; // Add 2 bytes

		// Now reading y-orientation
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->Yorientation = temp;
		VIRTUALROADARRAY[i].Yorientation = VirtRd->Yorientation;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 2, 1, outfileforNewFlags);
			}
				Swap2Bytes(VirtRd->Yorientation);
//Debug********				cout << "VirtRd->Yorientation = 0x" << hex << VirtRd->Yorientation << "\n";

		// Now reading slant-B
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->slantB = temp;
		VIRTUALROADARRAY[i].slantB = VirtRd->slantB;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 2, 1, outfileforNewFlags);
			}
		if (flag3DO==true) {
//			fwrite(&temp, 2, 1, outfileforNewFlags);
			VirtRd->slantB = Swap2Bytes(VirtRd->slantB);
			}
				Swap2Bytes(VirtRd->slantB);
//Debug********				cout << "VirtRd->slantB = 0x" << hex << VirtRd->slantB << "\n";

		// Now reading x-orientation
		fread(&temp,sizeof(short int),1,inputfile);
j+=2; // Add 2 bytes
		VirtRd->Xorientation = temp;
		VIRTUALROADARRAY[i].Xorientation = VirtRd->Xorientation;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 2, 1, outfileforNewFlags);
			}
				Swap2Bytes(VirtRd->Xorientation);
//Debug********				cout << "VirtRd->Xorientation = 0x" << hex << VirtRd->Xorientation << "\n";

		// SKIP 2 bytes
		fread(&temp,sizeof(short int),1,inputfile);
		VIRTUALROADARRAY[i].zero1 = temp;
		if ((flag3DO)&&(testFlags)) {
			fwrite(&temp, 2, 1, outfileforNewFlags);
			}
j+=2; // Add 2 bytes
//Debug******** cout << dec << "\nBytes in block = " << j << "\n";

fwrite("\n",1,1,outf); // New line between blocks

//cout << "TrackNode #" << i << ". Property = " << VirtRd->Flag4 << endl;
// ************ Truncate zero data:
if ((VirtRd->BarrierLeft == 0) && (VirtRd->BarrierRight == 0)&&(testFlags)) {
		trackEndVr = i;
		cout << "\nVirtual road data #" << i << " has zero values. Truncating zero records below.\n";
		virtRDlengh = i;
		break;
	}




/**//**//**///
// Patch 3DO ISO. Write extended BarrierLeft, BarrierRight, VergeLeft, VergeRight params to iso.
// Open iso, seek Virtual road start + N of block, write new value, close file. ????

/**//**//**///


////////////////////Converting and saving the Virtual Road data to VirtRdOBJ.obj
fwrite("\n",1,1,outf2); // New line in OBJ file
fwrite("o ",2,1,outf2); // Object type
itoa(i,String2,10);
fwrite("VirtRd",6,1,outf2); 
//Checking array fill
#if 1;
m = 0;
for (k=0; k<=4; k++) {
	if (String2[k] == 0) {
		m = k+1;
//		cout << dec << "\nK = " << k << "\n"; // Debug
		break;
	} else {
		m = k+1;
		}
	}
#endif;
fwrite(String2,m-1,1,outf2); // OBJ # 
CounterBlocks++;
//cout << "\nCounterBlocks = " << CounterBlocks << "\n"; // Debug
fwrite("\n",1,1,outf2); // New line in OBJ file
/////////////////////////////
			itoa(VirtRd->VergeLeft,String,16); // Write this value to file
				if (VirtRd->VergeLeft == 0) {
					fwrite("VergeLeft= 0x",13,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("VergeLeft= 0x",13,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->VergeRight,String,16); // Write this value to file
				if (VirtRd->VergeRight == 0) {
					fwrite("VergeRight= 0x",14,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("VergeRight= 0x",14,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->BarrierLeft,String,16); // Write this value to file
				if (VirtRd->BarrierLeft == 0) {
					fwrite("BarrierLeft= 0x",15,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("BarrierLeft= 0x",15,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->BarrierRight,String,16); // Write this value to file
				if (VirtRd->BarrierRight == 0) {
					fwrite("BarrierRight= 0x",16,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("BarrierRight= 0x",16,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

				// Flags
	#if 0;
			itoa(VirtRd->Flag1,String,16); // Write this value to file
				if (VirtRd->Flag1 == 0) {
					fwrite("Flag1= 0x",16,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("Flag1= 0x",16,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->Flag2,String,16); // Write this value to file
				if (VirtRd->Flag2 == 0) {
					fwrite("Flag2= 0x",16,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("Flag2= 0x",16,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->Flag3,String,16); // Write this value to file
				if (VirtRd->Flag3 == 0) {
					fwrite("Flag3= 0x",16,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("Flag3= 0x",16,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}

			itoa(VirtRd->Flag4,String,16); // Write this value to file
				if (VirtRd->Flag4 == 0) {
					fwrite("Flag4= 0x",16,1,outf);
					fwrite(String,1,1,outf);
					fwrite("\n",1,1,outf); // New line
				} else {
			fwrite("Flag4= 0x",16,1,outf);
			fwrite(String,2,1,outf);
			fwrite("\n",1,1,outf); // New line
				}
	#endif;
///////////////////////////////

////------------///////////

// Converting all HEX values to float. 
//XCenter, YCenter, ZCenter, FloatVergeLeft, FloatVergeRight, FloatBarrierLeft, FloatBarrierRight; // Floating params of the VirtRd
//Floatslope, FloatslantA, Floatorientation, FloatYorientation, FloatslantB, FloatXorientation;
	XCenter = VirtRd->X;
	YCenter = VirtRd->Y;
	ZCenter = VirtRd->Z;
//Debug******** cout << "\n-----X = " << XCenter;
//Debug******** cout << "\n-----Y = " << YCenter;
//Debug******** cout << "\n-----Z = " << ZCenter;

FloatVergeLeft = (double)(VirtRd->VergeLeft); 
//Debug******** cout << "\n-----FloatVergeLeft = " << FloatVergeLeft;
FloatVergeRight = (double)(VirtRd->VergeRight); 
//Debug******** cout << "\n-----FloatVergeRight = " << FloatVergeRight;
FloatBarrierLeft = (double)(VirtRd->BarrierLeft); 
//Debug******** cout << "\n-----FloatBarrierLeft = " << FloatBarrierLeft;
FloatBarrierRight = (double)(VirtRd->BarrierRight); 
//Debug******** cout << "\n-----FloatBarrierRight = " << FloatBarrierRight;
Floatslope = (double)(VirtRd->slope); // 
// Shift bytes for matching 14-bit format. 0x4000
	if (Floatslope > 0x2000) {
		Floatslope -= 0x4000;
		}
///cout << "\n-----Floatslope = " << Floatslope << "\n";
FloatslantA = (double)(VirtRd->slantA); 
// Shift bytes for matching 14-bit format. 0x4000
	if (FloatslantA > 0x2000) {
		FloatslantA -= 0x4000;
		}
///cout << "\n-----FloatslantA = " << FloatslantA << "\n"; 
Floatorientation = (double)(VirtRd->orientation); 
// Shift bytes for matching 14-bit format. 0x4000
	if (Floatorientation > 0x2000) {
		Floatorientation -= 0x4000;
		}
//Debug******** cout << "\n-----Floatorientation = " << Floatorientation; 
FloatYorientation = (double)(VirtRd->Yorientation); 
///cout << "\n-----FloatYorientation = " << FloatYorientation << "\n";
FloatslantB = (double)(VirtRd->slantB); 
//Debug******** cout << "\n-----FloatslantB = " << FloatslantB; 
FloatXorientation = (double)(VirtRd->Xorientation); 
///cout << "\n-----FloatXorientation = " << FloatXorientation << "\n";

// Angle in XY direction of the track
//OrientXYAngle = (Floatorientation/16384) * 360;
//cout << "\n+----OrientXYAngle = " << OrientXYAngle;
// Angle in YZ XZ direction ( Slope-B? )
SlantBAngle = (FloatslantB/65353) *360;
			if (flag3DO) {
//				SlantBAngle = SlantBAngle/360;
			}
// cout << "\n-----SlantBAngle = " << SlantBAngle;

#if 0;
// Vector for the perpendicular to the track: 
dirVectorX = FloatVergeRight * sin(degreesToRadians(OrthoTrack));
//Debug******** cout << "\nVectorX " << dirVectorX << "\n";
dirVectorY = FloatVergeRight * cos(degreesToRadians(OrthoTrack));
//Debug******** cout << "\nVectorY " << dirVectorY << "\n";
dirVectorZ = 0; // Just for now...
#endif;
//#if 0;

/////////////////////////////////////////////////////////////////////////////////////////////
//******// a2-a0-center-a1-a3
// Angle in XY direction of the track
//OrientXYAngle=VirtRd->orientation/16384 * 360;
OrientXYAngle = (Floatorientation/16384) * 360; 
//Debug******** cout << "\n+----OrientXYAngle = " << OrientXYAngle;


#if 0;
if (OrientXYAngle > 0) { // Check + or - value
		OrthoTrack = OrientXYAngle + 90;
			// Check if OrthoTrack is more than 180
			if (OrthoTrack > 180) {
				OrthoTrack -= 360;
				}
//Debug******** 		cout << "\n+----OrthoTrack angle = " << OrthoTrack;
	} else {
		OrthoTrack = OrientXYAngle + 90;
			// Check if OrthoTrack is > 0 
			if (OrthoTrack > 0) {
//Debug******** 			cout << "\n-----OrthoTrack angle = " << OrthoTrack;
				} else {			
//Debug******** 			cout << "\n-----OrthoTrack angle = " << OrthoTrack;
			}
		}


//********** New *******////////////
if ((OrthoTrack >= 0)&&(OrthoTrack <= 90)) {
//	cout << "\nI quadrant\n";
			// a0x -, a0y -
			// a1x +, a1y +
			// a2x -, a2y -
			// a3x +, a3y +
	
	}

if ((OrthoTrack > 90)&&(OrthoTrack <= 180)) {
//		cout << "\nII quadrant\n";
			// a0x -, a0y +
			// a1x +, a1y -
			// a2x -, a2y +
			// a3x +, a3y -

	}

if ((OrthoTrack < 0)&&(OrthoTrack >= -90)) {
//		cout << "\nIV quadrant\n";
			// a0x +, a0y -
			// a1x -, a1y +
			// a2x +, a2y -
			// a3x -, a3y +

	}

if ((OrthoTrack < -90)&&(OrthoTrack > -180)) {
//		cout << "\nIII quadrant\n";
			// a0x +, a0y +
			// a1x -, a1y -
			// a2x +, a2y +
			// a3x -, a3y -

	}
#endif;

// Rotating vector in XY plane around Z-axis.
float alpha = 180 + OrientXYAngle; 
//Debug******** cout << "\nalpha = " << alpha << "\n";

alpha = pi *alpha / 180; //  Grad to Rad

float cosA = (float)cos(alpha); //Cos
float sinA = (float)sin(alpha); //Sin

#if 0;
a0x = -cosA * FloatVergeLeft;
a0y = -sinA * FloatVergeLeft;
a0z = ZCenter; 
a1x = cosA * FloatVergeRight;
a1y = sinA * FloatVergeRight;
a1z = ZCenter; 
a2x = -cosA * FloatBarrierLeft;
a2y = -sinA * FloatBarrierLeft;
a2z = ZCenter; 
a3x = cosA * FloatBarrierRight;
a3y = sinA * FloatBarrierRight;
a3z = ZCenter; 


A0X = (FloatVergeLeft + XCenter) / SCALE;
A0Y = - YCenter / SCALE;
A0Z = ZCenter / SCALE;
A1X = (FloatVergeRight + XCenter)/ SCALE;
A1Y = - YCenter / SCALE;
A1Z = ZCenter / SCALE;
A2X = (FloatBarrierLeft + XCenter)/ SCALE;
A2Y = - YCenter / SCALE;
A2Z = ZCenter / SCALE;
A3X = (FloatBarrierRight + XCenter)/ SCALE;
A3Y = - YCenter / SCALE;
A3Z = ZCenter / SCALE;
#endif;

A0X = (cosA * FloatVergeLeft * SCALETRACK + XCenter) / SCALE;
//A0Y = -YCenter / SCALE;
A0Y = -(-sinA * FloatVergeLeft * SCALETRACK + YCenter) / SCALE;
A0Z = ZCenter / SCALE;

A1X = (-cosA * FloatVergeRight * SCALETRACK + XCenter) / SCALE;
//A1Y = -YCenter / SCALE;
A1Y = -(sinA * FloatVergeRight * SCALETRACK + YCenter) / SCALE;
A1Z = ZCenter / SCALE;

A2X = (cosA * FloatBarrierLeft * SCALETRACK + XCenter) / SCALE;
//A2Y = -YCenter / SCALE;
A2Y = -(-sinA * FloatBarrierLeft* SCALETRACK + YCenter) / SCALE;
A2Z = ZCenter / SCALE;

A3X = (-cosA * FloatBarrierRight* SCALETRACK + XCenter) / SCALE;
//A3Y = -YCenter / SCALE;
A3Y = -(sinA * FloatBarrierRight* SCALETRACK + YCenter) / SCALE;
A3Z = ZCenter / SCALE;

XCenterABS = XCenter/SCALE;
YCenterABS = -YCenter/SCALE;
ZCenterABS = ZCenter/SCALE;

// Here I must store this coords for the ParseObjects void
//ArrayXYZ[i] = XCenterABS;
ArrayXYZ[i][0] = XCenterABS;
ArrayXYZ[i][1] = YCenterABS;
ArrayXYZ[i][2] = ZCenterABS;

ArrayAngleXY[i] = OrientXYAngle; // Store absolute angle of the track for the Objects rotation
/////////////////////////////////////////////////////////////////////////////////////////////
//******// a2-a0-center-a1-a3
// Rotating vector OrthoTrack around (OrientXYAngle) -axis.
// Rotating vector A2X,A2Y,A2Z - A3X,A3Y,A3Z around (OrientXYAngle) -axis.
// Rotation angle - SlantBAngle
// Vector's start at: XCenterABS, YCenterABS, ZCenterABS

// Recalculate X,Y,Z values for a0, a1, a2, a3
// Matrix with all coords.
double origMatrix[5][3];
double resultMatrix[5][3];
Vector3 a0_Res, a1_Res, a2_Res, a3_Res, blank;

float zed = SlantBAngle;

zed = pi*zed / 180; //  Grad to Rad
//Debug******** cout << "\nzed = " << zed << "\n";

float cosZ = (float)cos(zed); //Cos
float sinZ = (float)sin(zed); //Sin

////////////////////////////////////////////////////////////********************** FIX cosA, sinA, cosZ *****************///////////////////////////////////////////
// New coords for a0, a1, a2, a3
// IF OFF - track will be without slantB tilt (horizontal)
#if 1;
// New a0 coords: 
find_intersection(Vector3(XCenterABS, YCenterABS, ZCenterABS), Vector3(-cosA, -sinA, sinZ), Vector3(XCenterABS, YCenterABS, ZCenterABS), FloatVergeLeft/SCALETILT, a0_Res, blank);
A0X = a0_Res.x;
A0Y = a0_Res.y;
A0Z = a0_Res.z;
//Debug******** cout << "\nXCenterABS = " << XCenterABS;
//Debug******** cout << "\nYCenterABS = " << YCenterABS;
//Debug******** cout << "\nZCenterABS = " << ZCenterABS;
//Debug******** cout << "\na0_Res X = " << a0_Res.x;
//Debug******** cout << "\na0_Res Y = " << a0_Res.y;
//Debug******** cout << "\na0_Res Z = " << a0_Res.z;

// New a1 coords: 
find_intersection(Vector3(XCenterABS, YCenterABS, ZCenterABS), Vector3(cosA, sinA, -sinZ), Vector3(XCenterABS, YCenterABS, ZCenterABS), FloatVergeRight/SCALETILT, a1_Res, blank);
A1X = a1_Res.x;
A1Y = a1_Res.y;
A1Z = a1_Res.z;
//Debug******** cout << "\na1_Res X = " << a1_Res.x;
//Debug******** cout << "\na1_Res Y = " << a1_Res.y;
//Debug******** cout << "\na1_Res Z = " << a1_Res.z;

// New a2 coords: 
find_intersection(Vector3(XCenterABS, YCenterABS, ZCenterABS), Vector3(-cosA, -sinA, sinZ), Vector3(XCenterABS, YCenterABS, ZCenterABS), FloatBarrierLeft/SCALETILT, a2_Res, blank);
A2X = a2_Res.x;
A2Y = a2_Res.y;
A2Z = a2_Res.z;
//Debug******** cout << "\na2_Res X = " << a2_Res.x;
//Debug******** cout << "\na2_Res Y = " << a2_Res.y;
//Debug******** cout << "\na2_Res Z = " << a2_Res.z;

// New a3 coords:
find_intersection(Vector3(XCenterABS, YCenterABS, ZCenterABS), Vector3(cosA, sinA, -sinZ), Vector3(XCenterABS, YCenterABS, ZCenterABS), FloatBarrierRight/SCALETILT, a3_Res, blank);
A3X = a3_Res.x;
A3Y = a3_Res.y;
A3Z = a3_Res.z;
//Debug******** cout << "\na3_Res X = " << a3_Res.x;
//Debug******** cout << "\na3_Res Y = " << a3_Res.y;
//Debug******** cout << "\na3_Res Z = " << a3_Res.z;
#endif;


// Filling ArrayVirtRd array for future use.
#if 1;
ArrayVirtRd[i][0] = A0X;
ArrayVirtRd[i][1] = A0Y;
ArrayVirtRd[i][2] = A0Z;

ArrayVirtRd[i][3] = A1X;
ArrayVirtRd[i][4] = A1Y;
ArrayVirtRd[i][5] = A1Z;

ArrayVirtRd[i][6] = A2X;
ArrayVirtRd[i][7] = A2Y;
ArrayVirtRd[i][8] = A2Z;

ArrayVirtRd[i][9] = A3X;
ArrayVirtRd[i][10] = A3Y;
ArrayVirtRd[i][11] = A3Z;

ArrayVirtRd[i][12] = XCenterABS;
ArrayVirtRd[i][13] = YCenterABS;
ArrayVirtRd[i][14] = ZCenterABS;
#endif;
////------------///////////



/// Write vertex data: 

//******// a2-a0-center-a1-a3
#if 1;
fwrite("v ",2,1,outf2); // Vertex header
fprintf(outf2, "%f ", A2X); 
fprintf(outf2, "%f ", A2Z); 
fprintf(outf2, "%f ", A2Y);
fwrite("\n",1,1,outf2);// New line

fwrite("v ",2,1,outf2); // Vertex header
fprintf(outf2, "%f ", A0X); 
fprintf(outf2, "%f ", A0Z); 
fprintf(outf2, "%f ", A0Y);
fwrite("\n",1,1,outf2);// New line
#endif;

fwrite("v ",2,1,outf2); // Vertex header
fprintf(outf2, "%f ", XCenterABS); 
fprintf(outf2, "%f ", ZCenterABS); 
fprintf(outf2, "%f ", YCenterABS);
fwrite("\n",1,1,outf2);// New line

// For Center Spline. Write in separate file!
fwrite("v ",2,1,outf3); // Vertex header 
fprintf(outf3, "%f ", XCenterABS); 
fprintf(outf3, "%f ", ZCenterABS); 
fprintf(outf3, "%f ", YCenterABS);
fwrite("\n",1,1,outf3);// New line

#if 1;
fwrite("v ",2,1,outf2); // Vertex header
fprintf(outf2, "%f ", A1X); 
fprintf(outf2, "%f ", A1Z); 
fprintf(outf2, "%f ", A1Y);
fwrite("\n",1,1,outf2);// New line

fwrite("v ",2,1,outf2); // Vertex header
fprintf(outf2, "%f ", A3X); 
fprintf(outf2, "%f ", A3Z); 
fprintf(outf2, "%f ", A3Y);
fwrite("\n",1,1,outf2);// New line
#endif;

// Write lines between center points to a separate file (VirtRdSpline.obj)
//for (int b=0; b<numVirtRdIndexes; b++) {
	fwrite("l ",2,1,outf3);
	fprintf(outf3, "%d ", (b));
	fprintf(outf3, "%d ", (b+1));
	fwrite("\n",1,1,outf3);// New line
	b++;
//}

/// Write 4 lines data:
#if 1;
//LinesCount += 1;
NextL++;
bool flag = false;
for (LinesCount = 1; LinesCount < 5; LinesCount++) {
		fwrite("l ",2,1,outf2);
		NextL++; //Next object
		fprintf(outf2, "%d ", NextL);
		fprintf(outf2, "%d ", (NextL+1));
		fwrite("\n",1,1,outf2);// New line
		if (flag == true) continue;
#if 1;
		// Connect to all previous vertexes.
			if (NextL > 5) { // It's the second block

				fwrite("l ",2,1,outf2);
				fprintf(outf2, "%d ", (NextL-5));
				fprintf(outf2, "%d ", (NextL));
				fwrite("\n",1,1,outf2);// New line

				fwrite("l ",2,1,outf2);
				fprintf(outf2, "%d ", (NextL-4));
				fprintf(outf2, "%d ", (NextL+1));
				fwrite("\n",1,1,outf2);// New line

				fwrite("l ",2,1,outf2);
				fprintf(outf2, "%d ", (NextL-3));
				fprintf(outf2, "%d ", (NextL+2));
				fwrite("\n",1,1,outf2);// New line

				fwrite("l ",2,1,outf2);
				fprintf(outf2, "%d ", (NextL-2));
				fprintf(outf2, "%d ", (NextL+3));
				fwrite("\n",1,1,outf2);// New line

				fwrite("l ",2,1,outf2);
				fprintf(outf2, "%d ", (NextL-1));
				fprintf(outf2, "%d ", (NextL+4));
				fwrite("\n",1,1,outf2);// New line
				flag = true;
				}
			#endif;
	}
#endif;

// Generate materials
float matR, matG, matB = 0;

matR = (float)VirtRd->Flag1/100;
matG = (float)VirtRd->Flag2/100;
matB = (float)VirtRd->Flag4/10;

fwrite("newmtl ",7,1,outf4);
fprintf(outf4, "%d%d%d%d\n", VirtRd->Flag1, VirtRd->Flag2, VirtRd->Flag3, VirtRd->Flag4);

fprintf(outf5, "VirtRd index - %d\n", i); // write debug data
fprintf(outf5, "%d %d %d %d\n\n", VirtRd->Flag1, VirtRd->Flag2, VirtRd->Flag3, VirtRd->Flag4); // write debug data

fwrite("Kd ",3,1,outf4);
//cout << "matR = "<< matR << "\n";
//cout << "matG = "<< matG << "\n";
//cout << "matB = "<< matB << "\n";
fprintf(outf4, "%f %f %f\n", matR, matG, matB);

// Assign material------------------------------------------------------------------------------------
//itoa(VirtRd->Flag1, arrayRGB[i], 10); 
//	cout << tmpbmp << endl;
//arrayRGB[i] = to_string(VirtRd->Flag1);
//arrayRGB[i] = VirtRd->Flag1;
//itoa(VirtRd->Flag1, arrayRGB[i], 10);
//char* numberstring[(((sizeof VirtRd->Flag1) * CHAR_BIT) + 2)/3 + 2];

//arrayRGB[i] = numberstring[VirtRd->Flag1];
//itoa(VirtRd->Flag1, arrayRGB2[i], 10);
int tempR = VirtRd->Flag1;
int tempG = VirtRd->Flag2;
int tempB = VirtRd->Flag3;
int tempSpare = VirtRd->Flag4;
//tmp = to_string(temp333);
arrayRGB2[i] = tempR;
arrayRGB3[i] = tempG;
arrayRGB4[i] = tempB;
arrayRGB5[i] = tempSpare;
//cout << temp333 << endl;
//arrayRGB[i] = "3333";
//cout << VirtRd->Flag1 << endl;
//cout << arrayRGB[i] << endl;

//////////////************END of the 2400 loop **********/////////////////
}

// Write all data while !eof (for 3DO debug file)
if ((flag3DO)&&(testFlags)) {
while (!feof(inputfile)) {
	fread(&temp,4,1,inputfile);
//	cout << temp << endl;
	fwrite(&temp, 4, 1, outfileforNewFlags);
}
}

// Write Polygon data (4 polygons):
#if 1;
int r;
for (r=0; r<CounterBlocks-1; r++) { 
//for (r=0; r<numVirtRdIndexes-322; r++) {


// Assign material------------------------------------------------------------------------------------
fwrite("usemtl ",7,1,outf2);
fprintf(outf2, "%d%d%d%d\n", arrayRGB2[r+1], arrayRGB3[r+1], arrayRGB4[r+1], arrayRGB5[r+1]);

//fwrite("\nuse mtl None\n",14,1,outf2);
//fwrite("s off\n",6,1,outf2);
// Writing 1st polygon 1//1 2//1 7//1 6//1
fwrite("f ",2,1,outf2);
fprintf(outf2, "%d", p+1); 
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+2);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+7);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+6);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);
fwrite("\n",1,1,outf2);

// Second polygon
//fwrite("\nuse mtl None\n",14,1,outf2);
//fwrite("s off\n",6,1,outf2);
// Writing 2nd polygon 2//1 3//1 8//1 7//1
fwrite("f ",2,1,outf2);
fprintf(outf2, "%d", p+2); 
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+3);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+8);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+7);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);
fwrite("\n",1,1,outf2);


// 3rd polygon
//fwrite("\nuse mtl None\n",14,1,outf2);
//fwrite("s off\n",6,1,outf2);
// Writing 3rd polygon 3//1 4//1 9//1 8//1
fwrite("f ",2,1,outf2);
fprintf(outf2, "%d", p+3); 
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+4);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+9);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+8);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);
fwrite("\n",1,1,outf2);

// 4th polygon
//fwrite("\nuse mtl None\n",14,1,outf2);
//fwrite("s off\n",6,1,outf2);
// Writing 4th polygon 4//1 5//1 10//1 9//1
fwrite("f ",2,1,outf2);
fprintf(outf2, "%d", p+4); 
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+5);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+10);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);

fprintf(outf2, "%d", p+9);
fwrite("//",2,1,outf2); // Separator
fprintf(outf2, "%d ", r+1);
fwrite("\n",1,1,outf2);

p += 5;
//y += 1;
}
#endif;

fclose(outf);
fclose(outf2);
fclose(outf3);
fclose(outf4);
fclose(outf5);

if ((flag3DO)&&(testFlags)) {
	fclose(outfileforNewFlags);
	}
//delete[]arrayRGB;
//cout << "File VirtRdOBJ.obj successfuly saved\n";
#endif;
}




//Parse Objects
void ParseObjects(FILE* inputfile)
{
//
int counter3D = 0;
int counterVt = 1;
unsigned int temp, temp2;
unsigned char bitN, flipV, resID, type, flags, resID2;
unsigned char width, height, p1, p5;
short int p2, p3, p4;
FILE *outf, *outf2;
char *outfilename;
int i, Length, startOffset;
int version, verBuffer, paramsStart, trafficStart;
int counter = 0;
float angleABS, XABS, YABS, ZABS;
Vector3 r1, r2, r3, r4;
bool flag3DO = false;
int matCounter = 0;


fseek(inputfile, 0, SEEK_END);  
FileSize = ftell(inputfile);
//Debug******** cout << "\n\n-=Reading Objects data=-\n";

#if 1; // version
fseek(inputfile,0,SEEK_SET);
// Reading Objects records
// Records are starting at 0x16F94, finishing at 0x1B000.
// For the SE edition of NFS records starts at 0x16628
fread(&verBuffer,4,1,inputfile);
version = verBuffer;

	if (version == 0x10) { // NFS original version
			startOffset = 0x16F94;
			Length = 1000;
			paramsStart = 0x16B80;
//			fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
			cout << "++Original NFS objects++\n";
		} 
	if (version == 0x11) {
			startOffset = 0x16628;
			Length = 1000;
			paramsStart = 0x16214;
//			fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
			cout << "++SE NFS objects++\n";
	}
	if (version == 0xE000000) {
			startOffset = 0x17050;
//			paramsStart = 0x16C3C;
			paramsStart = 0x16C3C;
			trafficStart = 0x16534;
			flag3DO = true;
			cout << "++3DO NFS objects++\n";
		}
//cout << "\nversion = 0x" << hex << version << "\n";
//cout << "\nstartOffset = 0x" << hex << startOffset << "\n";
#endif; // version

// Reading width and height for the objects
#if 1;
outf2 = fopen("Objects_test.obj","a");
//fwrite("# Objects params data\n",19,1,outf2);
fprintf(outf2, "# Objects params data\n");

fseek(inputfile,paramsStart-4,SEEK_CUR); // Setting to the starting position
//DEBUG******cout << "\nparamsStart = 0x" << hex << paramsStart << "\n";
fread(&verBuffer,4,1,inputfile); // For test
version = verBuffer;
	if (flag3DO == true) {
//		fread(&verBuffer,4,1,inputfile); // For test
		version = Swap4Bytes(verBuffer);
	}
//cout << "\nversion = 0x" << hex << version << "\n";
//cout << "\nflag3DO = 0x" << flag3DO << "\n";
fread(&verBuffer,4,1,inputfile); // For test
fread(&verBuffer,4,1,inputfile); // For test
fread(&verBuffer,4,1,inputfile); // For test
fread(&verBuffer,4,1,inputfile); // For test
//fread(&verBuffer,4,1,inputfile); // For test
//	cout << "\nObjParams = 0x" << hex << version << "\n";
	for (int h=0; h<version; h++) {
//DEBUG******				cout << "\n--COUNTER  = " << dec << h << "\n";
		fread(&flags,1,1,inputfile); // reading flags // none = 0, animated = 4
//later		Object->SceneryFlags = flags;
		OBJArray[h].SceneryFlags = flags;
		//DEBUG******		cout << "\n--SceneryFlags = " << Object->SceneryFlags << "\n";
		fread(&type,1,1,inputfile); // model = 1, bitmap = 4, twosidedbitmap = 6
//later		Object->SceneryType = type;
		OBJArray[h].SceneryType = type;
		//DEBUG******		cout << "\n--SceneryType = " << Object->SceneryType << "\n";
		fread(&resID,1,1,inputfile); // reading resID
//later		Object->rID = resID;
		OBJArray[h].rID = resID;
			//DEBUG******		cout << "\n--rID  = " << Object->rID << "\n";
		fread(&resID2,1,1,inputfile); // read rID2 for 2-sided bitmap
		OBJArray[h].rID2 = resID2;

		fread(&type,1,1,inputfile); // skip 1 byte
		fread(&width,1,1,inputfile); // skip 1 byte
			if (flag3DO == true) {
				OBJArray[h].Width = width;
				fread(&width,1,1,inputfile); // skip 1 byte
					p1 = width;
					OBJECTPARAMSARRAY[h].p1 = width;
//			cout << "\n--OBJECTPARAMSARRAY p1 = " << OBJECTPARAMSARRAY[h].p1 << "\n";
				goto cont;
			}
		fread(&width,1,1,inputfile); // Width
//later		Object->Width = width; 
		OBJArray[h].Width = width;
cont:
		//DEBUG******	cout << "\n--Width  = " << Object->Width << "\n";
		fread(&p2,2,1,inputfile); // skip 2 bytes
			OBJECTPARAMSARRAY[h].p2 = p2;
		fread(&p3,2,1,inputfile); // skip 2 bytes
			OBJECTPARAMSARRAY[h].p3 = p3;
		fread(&p4,2,1,inputfile); // skip 2 bytes
			OBJECTPARAMSARRAY[h].p4 = p4;
		fread(&height,1,1,inputfile); // skip 1 byte
			OBJECTPARAMSARRAY[h].p5 = height;

			if (flag3DO == true) {
				OBJArray[h].Height = height;
				fread(&height,1,1,inputfile); // skip 1 byte
					OBJECTPARAMSARRAY[h].p5 = height;
				goto cont2;
			}
		fread(&height,1,1,inputfile); // Height
//later		Object->Height = height;
		OBJArray[h].Height = height;
cont2:
		//DEBUG******cout << "\n--Height  = " << Object->Height << "\n\n";
		fread(&type,1,1,inputfile); // skip 1 byte

		OBJECTPARAMSARRAY[h].SceneryFlags = OBJArray[h].SceneryFlags;
		OBJECTPARAMSARRAY[h].SceneryType = OBJArray[h].SceneryType;
		OBJECTPARAMSARRAY[h].rID = OBJArray[h].rID;
		OBJECTPARAMSARRAY[h].rID2 = OBJArray[h].rID2;
		OBJECTPARAMSARRAY[h].Width = OBJArray[h].Width;
		OBJECTPARAMSARRAY[h].Height = OBJArray[h].Height;
		
		// Write debug
				//fwrite("\n",1,1,outf2);// New line
				fprintf(outf2, "Counter %d ", h);
				fprintf(outf2, "SceneryFlags %d ", OBJArray[h].SceneryFlags);
				fprintf(outf2, "SceneryType %d ", OBJArray[h].SceneryType);
				fprintf(outf2, "rID %d ", OBJArray[h].rID);
				fprintf(outf2, "rID2 %d ", OBJArray[h].rID2);
				fprintf(outf2, "Width %f ", OBJArray[h].Width);
				fprintf(outf2, "Height %f ", OBJArray[h].Height);
				fwrite("\n",1,1,outf2);// New line
		}
////fclose(outf2);
#endif;

fseek(inputfile,0,SEEK_SET);		// Set to the begining
fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
outfilename = "Objects.obj"; // File for the Objects
outf = fopen(outfilename,"a");
fwrite("# Objects data\n",15,1,outf);


for (i=1; i<=numOBJIndexes; i++) {
//numOBJIndexes=1000

	if (Object->BitmapN == 19) {
//		cout << "\nRecord #" << dec << i << "\n";
	}

//Debug********					cout << "\nRecord #" << dec << i << "\n";
next:
///int tmp3=0;
		fread(&temp2,4,1,inputfile); // Reference node
		Object->RefNode = temp2;
///		tmp3 = temp2;
///		tmp3 = Swap4Bytes(tmp3);
///		cout << "\nObject->RefNode = 0x" << tmp3 << "\n";
///		if (tmp3 == 0xFFFFFFFF) {	
///			OBJECTSARRAY[i].RefNode = 0;
///			} else {
			OBJECTSARRAY[i].RefNode = Object->RefNode;
///			}
//			cout << "\nflag3DO = 0x" << flag3DO << "\n";
			if (flag3DO == true) {
				Object->RefNode = Swap4Bytes(Object->RefNode);
//				cout << "\nRefnode #" << Object->RefNode << endl;
			}
///			fprintf(outf2, "RefNode %d ", Object->RefNode);
///			fwrite("\n",1,1,outf2);// New line

//Debug********				 		cout << "\nObject->RefNode = " << dec << Object->RefNode << "\n";

		fread(&bitN,1,1,inputfile); // bitmap number
		Object->BitmapN = bitN;
			OBJECTSARRAY[i].BitmapN = Object->BitmapN;
//Debug********		cout << "\nBitmapN = 0x" << hex << Object->BitmapN << "\n";
//		arrObjTex[i] = Object->BitmapN;
		Object->Width = OBJArray[Object->BitmapN].Width;
		Object->Height = OBJArray[Object->BitmapN].Height;
		Object->rID = OBJArray[Object->BitmapN].rID;
		Object->rID2 = OBJArray[Object->BitmapN].rID2;

		int checkpoint = OBJArray[Object->BitmapN].rID;
			if (checkpoint == 124) {
				cout << "\nRefnode with bitmap #" << dec << Object->BitmapN << " = " << Object->RefNode << "\n";
//				cout << "\nFlags #" << VirtRd->Flag2 << "\n";
			}


		arrObjTex[i] = Object->rID/4;
		arrObjTex2[i] = Object->rID2/4;
//		arrObjTex[i] = Object->BitmapN;

///		fprintf(outf2, "Object->Width %f ", Object->Width);
///		fwrite("\n",1,1,outf2);// New line
///		fprintf(outf2, "Object->Height %f ", Object->Height);
///		fwrite("\n",1,1,outf2);// New line
//Debug********		cout << dec << arrObjTex[i] << " ";
//Debug********		cout << arrObjTex[i] << " ";

//Debug********				 		cout << "\narrObjTex[i] = 0x" << hex << arrObjTex[i] << "\n";

		fread(&flipV,1,1,inputfile); // flip
		Object->Flip = flipV;
			OBJECTSARRAY[i].Flip = Object->Flip;
//Debug********				 		cout << "\nObject->Flip = 0x" << hex << Object->Flip << "\n";

		fread(&temp,4,1,inputfile); // Skip 4 bytes
		OBJECTSARRAY[i].flags = temp;

		fread(&temp,2,1,inputfile); // X coord
		Object->Xcoord = temp;
			OBJECTSARRAY[i].Xcoord = Object->Xcoord;
			if (flag3DO == true) {
				Object->Xcoord = Swap2Bytes(Object->Xcoord);
//				cout << "\nXcoord = " << Object->Xcoord << endl;
				}
//		Object->Xcoord = Swap2Bytes(temp);
//cout << "\nObject->Xcoord = 0x" << hex << Object->Xcoord << "\n";

		fread(&temp,2,1,inputfile); // Z coord
		Object->Zcoord = temp;
			OBJECTSARRAY[i].Zcoord = Object->Zcoord;
			if (flag3DO == true) {
				Object->Zcoord = Swap2Bytes(Object->Zcoord);
//				cout << "\nZcoord = " << Object->Zcoord << endl;
				}
//		Object->Zcoord = Swap2Bytes(temp);
//Debug********						cout << "\nObject->Zcoord = 0x" << hex << Object->Zcoord << "\n";

		fread(&temp,2,1,inputfile); // Y coord
		Object->Ycoord = temp;
			OBJECTSARRAY[i].Ycoord = Object->Ycoord;
			if (flag3DO == true) {
				Object->Ycoord = Swap2Bytes(Object->Ycoord);
//				cout << "\nYcoord = " << Object->Ycoord << endl;
				}
//		Object->Ycoord = Swap2Bytes(temp);
//Debug********				 		cout << "\nObject->Ycoord = 0x" << hex << Object->Ycoord << "\n\n";

			if (Object->RefNode == -1) { // Reached the end of the records list or this node is unused FIX unused records defining!!!!!!!!!!!!!!
//				i++;
//				fread(&temp,4,1,inputfile); // Skip 12 bytes
//				fread(&temp,4,1,inputfile); // Skip 12 bytes
//				fread(&temp,4,1,inputfile); // Skip 12 bytes
//				goto next;//skip this record;
//				cout << "\nLast Reference node is: " << i << "\n";
				break;
				}
		Object->SceneryType = OBJArray[Object->BitmapN].SceneryType;


counter++;
//fprintf(outf, "N%d ", i);
fprintf(outf, "o Node%d", Object->RefNode);
fprintf(outf, "Bitmap%d", Object->BitmapN);
fwrite("\n",1,1,outf);// New line
			// Reference node is OK. Absolute coords are stored already in virtual road.
				Object->X = ArrayXYZ[Object->RefNode+1][0];// +1 means that we must bind object next to the "next" RefNode.
				Object->Y = ArrayXYZ[Object->RefNode+1][1];
				Object->Z = ArrayXYZ[Object->RefNode+1][2];
				XABS = Object->X + (float)Object->Xcoord/SCALEOBJ;
				YABS = Object->Y - (float)Object->Ycoord/SCALEOBJ;
				ZABS = Object->Z + (float)Object->Zcoord/SCALEOBJ;

				angleABS = ArrayAngleXY[Object->RefNode] + (float)Object->Flip / 256 * 360; // Calculate object angle placement SCALEROTATEOBJECT
							if (flag3DO == true) {
//								angleABS = angleABS / 8;
							}
///				fprintf(outf2, "ArrayAngleXY %f ", ArrayAngleXY[Object->RefNode]);
///				fwrite("\n",1,1,outf2);// New line
///				fprintf(outf2, "Object->Flip %f ", (float)Object->Flip / 256 * 360);
///				fwrite("\n",1,1,outf2);// New line

					if (angleABS > 360) {
//						angleABS = 360 - angleABS;
//						fwrite("\n",1,1,outf);// New line
//						fprintf(outf, "ANGLE%f", angleABS);
//						fwrite("\n",1,1,outf);// New line
					}


////////////////////Object rotation quadrant start //////////////////////////////////////
// Recalculate angleABS. 
#if 1;
if ((angleABS >= 0)&&(angleABS <= 90)) {
//		angleABS = ArrayAngleXY[Object->RefNode] + (float)Object->Flip / 256 * 360; // Calculate object angle placement SCALEROTATEOBJECT
//		cout << "\nI quadrant\n";
//		cout << "angleABS= " << angleABS << endl << endl;
	}

if ((angleABS > 90)&&(angleABS <= 180)) {
//		cout << "\nII quadrant\n";
//		cout << "angleABS= " << angleABS << endl << endl;
	}

if ((angleABS < 0)&&(angleABS >= -90)) {
//		cout << "\nIV quadrant\n";
//		cout << "angleABS= " << angleABS << endl << endl;
	}

if ((angleABS < -90)&&(angleABS > -180)) {
//		cout << "\nIII quadrant\n";
//		cout << "angleABS= " << angleABS << endl << endl;
	}
//angleABS = ArrayAngleXY[Object->RefNode];

#endif;
////////////////////////////////////// Object rotation quadrant end ///////////////////////////


		if (Object->SceneryType == 1 ) { // It's a 3D model
			//cout << "SceneryType = " << Object->SceneryType << "\n";
			//cout << "RefNode = " << Object->RefNode << "\n";
			// EXTRACT 3D object
				// Define N of ORIP! Maybe based on Object->BitmapN.
				// Count all textures for 2D objects. Count to horizon entry.
				// 
			//cout << "BITMAP # = " << Object->BitmapN << "\n";	
			Extract3D(angleABS, XABS, YABS, ZABS, Object->BitmapN, infilename, counter3D, counterVt, flag3DO, FamFileName3DO); // Seek it in the 4th chunk of FAM file.
			
			if (flag3DO==true) {
				counterVt = vt3DOcounter;
				//cout << "\ncounterVt= " << vt3DOcounter << endl;
				} else {
				counterVt = size13DObj*4+1;
				}
			counter3D = size83DObj;
//			cout << "\nsize13DObj= " << size13DObj << endl;
//			cout << "\nsize83DObj= " << size83DObj << endl;
//			cout << "\nObj3Dparams->size8= " << Obj3Dparams->size8 << endl;
			
			//Skip plane object creation
//			continue;
//			goto afterext;
		}

/////////*********************//////////////////////
#if 0;
     cout << "\n\nObject->RefNode= " << Object->RefNode << endl;
	 cout << "Object->BitmapN= " << Object->BitmapN << endl;
	 cout << "Object->Flip= " << Object->Flip << endl;
//	 cout << "Object->Xcoord= " << Object->Xcoord << endl;
//	 cout << "Object->Zcoord= " << Object->Zcoord << endl;
//	 cout << "Object->Ycoord= " << Object->Ycoord << endl;
//	 cout << "Object->X= " << Object->X << endl;
//	 cout << "Object->Z= " << Object->Z << endl;
//	 cout << "Object->Y= " << Object->Y << endl;
	 cout << "Object->Width= " << Object->Width << endl;
	 cout << "Object->Height= " << Object->Height << endl;
	 cout << "Object->SceneryType= " << Object->SceneryType << endl;
	 cout << "Object->SceneryFlags= " << Object->SceneryFlags << endl;
//	 cout << "Object->rID= " << Object->rID << endl;
//	 cout << "Object->rID2= " << Object->rID2 << endl;
#endif;

/////////*********************//////////////////////

//fprintf(outf, "ANGLE%f\n", angleABS);
//fwrite("\n",1,1,outf);// New line
///cout << "\nLOOP = " << dec << i << "\n";
				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", XABS);
				fprintf(outf, "%f ", ZABS);//////////////// POINT 1
				fprintf(outf, "%f ", YABS);
//				fprintf(outf, "%f ", angleABS);
				fwrite("\n",1,1,outf);// New line
// Write additional points
				if (Object->Width == 0) {
					Object->Width = SCALEOBJECT * 5.6;// It's for the small pillars near road edge.
					}
//find_intersection(Vector3(XABS, YABS, ZABS), Vector3(cos(degreesToRadians(angleABS)), sin(degreesToRadians(angleABS)), 0), Vector3(XABS, YABS, ZABS), 0.3, r1, r2);
find_intersection(Vector3(XABS, YABS, ZABS), Vector3(cos(degreesToRadians(angleABS)), sin(degreesToRadians(angleABS)), 0), Vector3(XABS, YABS, ZABS), Object->Width/2 * SCALEOBJECT, r1, r2);
////cout << "\nObject->Width/2 * SCALEOBJECT = " << dec << Object->Width/2*SCALEOBJECT << "\n";

				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", XABS);
				fprintf(outf, "%f ", ZABS+Object->Height * SCALEOBJECT); //Draw vertical line
				fprintf(outf, "%f ", YABS);//////////////// POINT 2
				fwrite("\n",1,1,outf);// New line
//				fprintf(outf, "%f ", angleABS);

				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", r1.x);
				fprintf(outf, "%f ", r1.z); //////////////// POINT 3
				fprintf(outf, "%f ", r1.y);
				fwrite("\n",1,1,outf);// New line

//					fprintf(outf, "# Object->Width = ");
//					fprintf(outf, "%f \n", Object->Width);
//					fprintf(outf, "# Object->Height = ");
//					fprintf(outf, "%f \n", Object->Height);
				fwrite("v ",2,1,outf); // Object type 
				fprintf(outf, "%f ", r2.x);
				fprintf(outf, "%f ", r2.z); //////////////// POINT 4
				fprintf(outf, "%f ", r2.y);
				fwrite("\n",1,1,outf);// New line

				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", r1.x);
				fprintf(outf, "%f ", r1.z+Object->Height * SCALEOBJECT); //////////////// POINT 5
				fprintf(outf, "%f ", r1.y);
				fwrite("\n",1,1,outf);// New line

				fwrite("v ",2,1,outf); // Object type 
				fprintf(outf, "%f ", r2.x);
				fprintf(outf, "%f ", r2.z+Object->Height * SCALEOBJECT); //////////////// POINT 6
				fprintf(outf, "%f ", r2.y);
				fwrite("\n",1,1,outf);// New line

#if 1;// SceneryType == 6
	if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.
			// Add 2 additional points, 1 polygon and 2 vertex coord (vt).
		find_intersection(Vector3(r2.x, r2.y, r2.z), Vector3(cos(degreesToRadians(angleABS+90)), sin(degreesToRadians(angleABS+90)), 0), Vector3(r2.x, r2.y, r2.z), Object->Width * SCALEOBJECT, r3, r4);
				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", r3.x);
				fprintf(outf, "%f ", r3.z); //////////////// POINT 7
				fprintf(outf, "%f ", r3.y);
				fwrite("\n",1,1,outf);// New line

				fwrite("v ",2,1,outf); // Object type
				fprintf(outf, "%f ", r3.x);
				fprintf(outf, "%f ", r3.z+Object->Height * SCALEOBJECT); //////////////// POINT 8
				fprintf(outf, "%f ", r3.y);
				fwrite("\n",1,1,outf);// New line

				// Add 2 double points 6 and 4.
#if 1; // 2 points
						fwrite("v ",2,1,outf); // Object type 
						fprintf(outf, "%f ", r2.x);
						fprintf(outf, "%f ", r2.z+Object->Height * SCALEOBJECT); //////////////// POINT 6
						fprintf(outf, "%f ", r2.y);
						fwrite("\n",1,1,outf);// New line

						fwrite("v ",2,1,outf); // Object type 
						fprintf(outf, "%f ", r2.x);
						fprintf(outf, "%f ", r2.z); //////////////// POINT 4
						fprintf(outf, "%f ", r2.y);
						fwrite("\n",1,1,outf);// New line

#endif; // 2 points
				}

#endif;//SceneryType == 6

				// Draw lines
				fwrite("l ",2,1,outf); // Object type
				fprintf(outf, "%d ", counter);
				fprintf(outf, "%d ", counter+1);
				fwrite("\n",1,1,outf);// New line

				fwrite("l ",2,1,outf); // Object type
				fprintf(outf, "%d ", counter);
				fprintf(outf, "%d ", counter+2);
				fwrite("\n",1,1,outf);// New line

				fwrite("l ",2,1,outf); // Object type
				fprintf(outf, "%d ", counter);
				fprintf(outf, "%d ", counter+3);
				fwrite("\n",1,1,outf);// New line

				fwrite("l ",2,1,outf); // Object type
				fprintf(outf, "%d ", counter+1);
				fprintf(outf, "%d ", counter+4);
				fwrite("\n",1,1,outf);// New line

				fwrite("l ",2,1,outf); // Object type
				fprintf(outf, "%d ", counter+1);
				fprintf(outf, "%d ", counter+5);
				fwrite("\n",1,1,outf);// New line

			#if 1;// SceneryType == 6
				if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.

					fwrite("l ",2,1,outf); // Object type
						fprintf(outf, "%d ", counter+5);
						fprintf(outf, "%d ", counter+7);
					fwrite("\n",1,1,outf);// New line

					fwrite("l ",2,1,outf); // Object type
						fprintf(outf, "%d ", counter+3);
						fprintf(outf, "%d ", counter+6);
					fwrite("\n",1,1,outf);// New line

					fwrite("l ",2,1,outf); // Object type
						fprintf(outf, "%d ", counter+7);
						fprintf(outf, "%d ", counter+6);
					fwrite("\n",1,1,outf);// New line
				}
			#endif;//SceneryType == 6


#if 1;
// Write vt params. There are 6 vertices. Max uv coord is 1.0!
if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 1.000000\n");
fprintf(outf, "vt 0.000000 1.000000 1.000000\n");
fprintf(outf, "vt 0.500000 0.000000 1.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 1.000000\n");
fprintf(outf, "vt 1.000000 1.000000 1.000000\n");
fprintf(outf, "usemtl %d\n", arrObjTex[i]);

#if 1;// SceneryType == 6
if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.///////////////////// FIX!!!!!!!!!!!!!!!!
	fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
	fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
		fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
		fprintf(outf, "vt 0.000000 1.000000 0.000000\n");

	}
#endif;//SceneryType == 6

	}	else	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrObjTex[i]);


#if 1;// SceneryType == 6
if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.
	fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
	fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
		fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
		fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
	}
#endif;//SceneryType == 6
				}
#endif; // vt

				#if 1; // Write polygons
				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (counter));
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fprintf(outf, "%d", (counter+1));
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fprintf(outf, "%d", (counter+4)); 
					fprintf(outf, "/%d ", matCounter+2);  // vt index
				fprintf(outf, "%d", (counter+2));
					fprintf(outf, "/%d ", matCounter+1);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (counter));
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fprintf(outf, "%d", (counter+3));
					fprintf(outf, "/%d ", matCounter+5);  // vt index
				fprintf(outf, "%d", (counter+5)); 
					fprintf(outf, "/%d ", matCounter+6);  // vt index
				fprintf(outf, "%d", (counter+1));
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fwrite("\n",1,1,outf);// New line
				#endif;
#if 1;// SceneryType == 6
			if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.

				fprintf(outf, "usemtl %d\n", arrObjTex2[i]);
					fwrite("f ",2,1,outf);
					fprintf(outf, "%d", (counter+3));// point 4
						fprintf(outf, "/%d ", matCounter+7);  // vt index
					fprintf(outf, "%d", (counter+5));// point 6
						fprintf(outf, "/%d ", matCounter+8);  // vt index
					fprintf(outf, "%d", (counter+7));// point 8
						fprintf(outf, "/%d ", matCounter+6);  // vt index
					fprintf(outf, "%d", (counter+6));// point 7
						fprintf(outf, "/%d ", matCounter+5);  // vt index
					fwrite("\n",1,1,outf);// New line
				}
#endif;//SceneryType == 6

			if (Object->SceneryType == 6 ) { // It's a TwoSided bitmap.
//				counter += 7;
//				matCounter += 8;
				counter += 9;
				matCounter += 10;
			} else {

			counter += 5;
			matCounter += 6;
			
			}
//afterext:
}	// End of OBJ reading loop 

fclose(outf);
fclose(outf2);// Close Objects_test.obj file
/////////////**************************///////////////////////////////////
#if 1; // Objects material file
FILE *mtlfile;
mtlfile = fopen("Objects.mtl","a");
fwrite("# Objects mtl data\n",19,1,mtlfile);
int c, count;

// New array size
size_t size = sizeof(arrObjTex) / sizeof(arrObjTex[0]); 
sort(arrObjTex, arrObjTex + size);
count = distance(arrObjTex, unique(arrObjTex, arrObjTex + size)); // unique values quantity.

//for (size_t f = 0; f < size-14300; ++f) {
  for (size_t f = 0; f < size; ++f) { // Show sorted list
//   cout << dec << arrObjTex[f] << ' ';
  }
//cout << endl << "Unique object numbers = " << count << endl;
c = count;


for (c=0; c<count; c++) {
// Add all unique array elements to the materials file.
//		if (c == 0) continue;
		fprintf(mtlfile, "newmtl %d\n", arrObjTex[c]);
//		fprintf(mtlfile, "map_Kd C:\\Users\\Versus\\Desktop\\Tex%d.bmp\n", arrObjTex[c]);
//		fprintf(mtlfile, "map_Kd ObjectTextures\\Tex%d.bmp\n", arrObjTex[c]);
			if (flag3DO == true) {
				fprintf(mtlfile, "map_Kd ThreeDOObjTextures\\Tex%d.bmp\n", arrObjTex[c]);
//				cout << "ObjTexture # = " << arrObjTex[c] << endl;
				Extract3DOObjTexture(arrObjTex[c], FamFileName3DO, 2);
			} else {
//				fprintf(mtlfile, "map_Kd ObjectTextures\\Tex%d.bmp\n", arrObjTex[c]);
//				ExtractObjectTexture(arrObjTex[c], infilename);
				fprintf(mtlfile, "map_Kd ObjectTextures\\Tex%d.bmp\n", arrObjTex[c]);
				ExtractObjectTexture(arrObjTex[c], infilename);
			}
}
fclose(mtlfile);
///////////////******************///////////////////
// Add some arrObjTex2[] textures if they doesn't exist in mtlfile.
//ExtractObjectTexture(arrObjTex2[c], infilename);
   ifstream file("Objects.mtl"); 
    string s;
	char find[256] = {0};
	char find2[256] = {0};
    char c1, c4, c5;
	bool found = false;

	 while (!file.eof()){ // Fill s string
        file.get(c1);
        s.push_back(c1);
    }
    file.close();

//////****************************************************
#if 0;
int c2, count2;

// New array size
size_t size2 = sizeof(arrObjTex2) / sizeof(arrObjTex2[0]); 
sort(arrObjTex2, arrObjTex2 + size2);
count2 = distance(arrObjTex2, unique(arrObjTex2, arrObjTex2 + size2)); // unique values quantity.

  for (size_t f = 0; f < size2; ++f) { // Show sorted list
   cout << dec << arrObjTex[f] << ' ';
  }
cout << endl << "Unique object numbers = " << count2 << endl;
c2 = count2;
#endif;
//////**************************************************


// Seek some kind of "Tex19.bmp"
//for (c=0; c<count; c++) {
for (int c=0; c < sizeof(arrObjTex2); c++) { ///////////////////////// FIX!!!!!!!!!!!!!!!!!!!!

//if (arrObjTex2[c] == 0) continue;

//	strncpy(find, "Tex", 3);
//	strcpy(find2,itoa(arrObjTex2[c],find2,10)); 
//	strcat(find, find2);
///cout << "find = " << find << endl;

	strcpy(find,itoa(arrObjTex2[c],find,10)); ////////////////////// CHECK search number only (without "Tex") and fix if necessary.
int pos;
		pos = s.find(find);

//cout << "s = " << s << endl;
	if (pos == -1) {
//		cout << "Material entry for 2nd side is not found. Adding material to mtlfile." << endl;
	if (flag3DO == true) {
//			cout << "3DO!3DO!3DO!3DO!3DO!3DO!3DO!3DO!3DO!" << endl;
			Extract3DOObjTexture(arrObjTex2[c], FamFileName3DO, 2);
			mtlfile = fopen("Objects.mtl","a");
				fprintf(mtlfile, "newmtl %d\n", arrObjTex2[c]);
				fprintf(mtlfile, "map_Kd ThreeDOObjTextures\\Tex%d.bmp\n", arrObjTex2[c]);
			fclose(mtlfile);
			// Regen s string. 
				ifstream file("Objects.mtl"); 
				while (!file.eof()){ // Fill s string
					file.get(c5);
					s.push_back(c5);
				    }
				file.close();
		} else 
	{
		ExtractObjectTexture(arrObjTex2[c], infilename);
		mtlfile = fopen("Objects.mtl","a");
			fprintf(mtlfile, "newmtl %d\n", arrObjTex2[c]);
			fprintf(mtlfile, "map_Kd ObjectTextures\\Tex%d.bmp\n", arrObjTex2[c]);
		fclose(mtlfile);
			// Regen s string. 
				ifstream file("Objects.mtl"); 
				while (!file.eof()){ // Fill s string
					file.get(c4);
					s.push_back(c4);
				    }
				file.close();
			}
	}
	else {
		continue;
//		found = true;
//		cout << "Found! Position = " << pos << endl;
	}
}
////////////////**********************///////////////////////



#endif; // Objects material file


fseek(inputfile, trafficStart-3, SEEK_SET); // Setting to the traffic starting position
//unsigned int ttt = 0;
unsigned char ttt = 0;
//fread(&ttt,4,1,inputfile);
//	cout << "ttt = 0x" << hex << ttt << endl;
//	cout << "trafficStart = 0x" << hex << trafficStart << endl;
for (int ad=0; ad<=numSceneryIndexes; ad++) {
	fread(&ttt,1,1,inputfile); 
		TRAFFICARRAY[ad].Speed = ttt;
//			cout << "TRAFFICARRAY[ad].Speed = 0x" << hex << TRAFFICARRAY[ad].Speed << endl;
	fread(&ttt,1,1,inputfile); 
//			cout << "TRAFFICARRAY[ad].CopsSpeed = 0x" << hex << TRAFFICARRAY[ad].CopsSpeed << endl;
		TRAFFICARRAY[ad].CopsSpeed = ttt;
	fread(&ttt,1,1,inputfile); 
//			cout << "TRAFFICARRAY[ad].Speed2 = 0x" << hex << TRAFFICARRAY[ad].Speed2 << endl;
		TRAFFICARRAY[ad].Speed2 = ttt;
	}


/////////////**************************///////////////////////////////////
//cout << "File Objects.obj successfuly saved\n";
//cout << "File Objects.mtl successfuly saved\n";
}




void ParseScenery(FILE* inputfile)
{
#if 1;
//long int pos;
	bool tunnelRight = false;
	bool tunnelLeft = false;
fpos_t pos;
int matCounter2 = 0;
int linesCounter = 1;
int p = 0;
unsigned int temp;
unsigned short int temp2;
FILE *outf, *outf4;
char *outfilename, *outfilename4;
int i, j, k, f, s, startOffset, version, verBuffer;
int z = 0;
int connector;
int NextL = 1;
unsigned char t1;
double ArrayCoord[100000] = {0};
unsigned short int arrTex1[5000], arrTex2[5000], arrTex3[5000], arrTex4[5000], arrTex5[5000], arrTex6[5000], arrTex7[5000], arrTex8[5000], arrTex9[5000], arrTex10[5000];
char mtlN[30];
bool flag3DO = false;
bool hasFence = false;
//int FenceRight, FenceLeft, FenceTexture;
double a0x, a0y, a0z, a1x, a1y, a1z, a2x, a2y, a2z, a3x, a3y, a3z, a4x, a4y, a4z, a5x, a5y, a5z, a6x, a6y, a6z, a7x, a7y, a7z, a8x, a8y, a8z, a9x, a9y, a9z, a10x, a10y, a10z; // Float coords
double b0x, b0y, b0z, b1x, b1y, b1z, b2x, b2y, b2z, b3x, b3y, b3z, b4x, b4y, b4z, b5x, b5y, b5z, b6x, b6y, b6z, b7x, b7y, b7z, b8x, b8y, b8z, b9x, b9y, b9z, b10x, b10y, b10z;
double c0x, c0y, c0z, c1x, c1y, c1z, c2x, c2y, c2z, c3x, c3y, c3z, c4x, c4y, c4z, c5x, c5y, c5z, c6x, c6y, c6z, c7x, c7y, c7z, c8x, c8y, c8z, c9x, c9y, c9z, c10x, c10y, c10z;
double d0x, d0y, d0z, d1x, d1y, d1z, d2x, d2y, d2z, d3x, d3y, d3z, d4x, d4y, d4z, d5x, d5y, d5z, d6x, d6y, d6z, d7x, d7y, d7z, d8x, d8y, d8z, d9x, d9y, d9z, d10x, d10y, d10z;
double e0x, e0y, e0z, e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z, e4x, e4y, e4z, e5x, e5y, e5z, e6x, e6y, e6z, e7x, e7y, e7z, e8x, e8y, e8z, e9x, e9y, e9z, e10x, e10y, e10z;

fseek(inputfile, 0, SEEK_END);  
FileSize = ftell(inputfile);
//Debug******** cout << "\n\n-=Reading Objects data=-\n";

#if 1; // version/////////////////////// WRITE NEW ALGORTIM FOR THE SE VERSION SCENERY!
// Reading Scenery records
// Records are starting at 0x1B000, finishing at eof.
// For the SE edition of NFS records starts at 0x1A4A0
fseek(inputfile,0,SEEK_SET);
// Reading Scenery records

fread(&verBuffer,4,1,inputfile);
version = verBuffer;

	if (version == 0x10) { // NFS original version
			startOffset = 0x1B000;
//			Length = 1000;
			cout << "++Original NFS scenery++\n";
		} 
	if (version == 0x11) {
			startOffset = 0x1A4A0;
			cout << "++SE NFS scenery++\n";
	}
	if (version == 0xE000000) {
			startOffset = 0x1B000;
			flag3DO = true;
			cout << "++3DO NFS scenery++\n";
		}
//cout << "\nversion = 0x" << hex << version << "\n";
//cout << "\nstartOffset = 0x" << hex << startOffset << "\n";
#endif; // version

//version = fread(&version,sizeof(char),1,inputfile);
//	if (version == 0x01) { // NFS original version
//			startOffset = 0x1B000;
//			cout << "\nOriginal NFS version\n";
//		} else {
//			startOffset = 0x1A4A0;
//			cout << "\nSE NFS version\n";
//		}

//fseek(inputfile,0,SEEK_SET);


fseek(inputfile,0,SEEK_SET);		// Set to the begining

fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
outfilename = "Scenery.obj"; // File for the Scenery
outf = fopen(outfilename,"a");
fwrite("# Scenery data\n",15,1,outf);
fprintf(outf, "mtllib Scenery.mtl\n\n");

outfilename4 = "Fence.obj"; // File for the Scenery
outf4 = fopen(outfilename4,"a");
fwrite("# Fence data\n",13,1,outf4);

i = 0;
connector = 55; // Starting number
while (!feof(inputfile)) {////////////////////////////////////// TURN ON FOR THE WHOLE SCENERY SAVE!!!!!!!!!!!!!!!
//while (i<264) {//////////////////////////////
//for (i=1; i<=50; i++) {
//i++;
fread(&temp,4,1,inputfile); // Skip 4 bytes
//cout << "\nTRKD = " << hex << temp << "\n";
//fread(&temp,4,1,inputfile); // Skip 4 bytes
//cout << "\nTRKD2 = " << hex << temp << "\n";
Scenery->PointA0X = temp; // TRKD check. This value will be overwritten later. 
	if (temp != 0x444B5254) {
		break;
	}
//Debug******** cout << "\nTRKD = 0x" << hex << Scenery->PointA0X << "\n";
//Debug******** fprintf(outf, "TRKD %d \n", Scenery->PointA0X);
fread(&temp,4,1,inputfile); // Skip 4 bytes ******************************* READ LENGTH OF THE TABLE CONTENTS FOR 3DO VERSION!!!!***************

fread(&temp,4,1,inputfile); // Record # 
Scenery->CurrentNum = temp; // Record #
SCENERYARRAY[i].CurrentNum = Scenery->CurrentNum;
// 3DO specific:
		if (flag3DO == true) {
			Scenery->CurrentNum = Swap4Bytes(Scenery->CurrentNum);
		} 

//Debug******** cout << "\nCurrentNum = " << dec << Scenery->CurrentNum << "\n";
//Debug******** fprintf(outf, "CurrentNum %d \n", Scenery->CurrentNum);
// The following second byte is FENCE.
//fread(&temp,2,1,inputfile); // Skip 2 bytes (1 byte skip, 2nd byte fence)
#if 1;
hasFence = false; // Not found the fence yet.
fread(&temp,1,1,inputfile); // Skip 1 byte
fread(&temp2,1,1,inputfile); // Read fence type and texture
SCENERYARRAY[i].RawFence = temp2;

				//fenceType stores the sides of the road the fence lives, and the textureId to use for it.
				// If the top bit is set, fence on the left exists, if next bit is set, fence is on the right.  Both can also be set. 
				//The other 6 bits seem to the texture number
	if (temp2 != 0) {
//		cout << "\nFence byte = 0x" << hex << temp << "\n";
		bool bit7 = (temp2 & (0x1 << 7)) != 0;
		bool bit6 = (temp2 & (0x1 << 6)) != 0;
		Scenery->HasLeftFence = bit7;
		Scenery->HasRightFence = bit6;
//		cout << "\nHasLeftFence = " << Scenery->HasLeftFence << "\n";
//		cout << "\nHasRightFence = " << Scenery->HasRightFence << "\n";
		// Ignore the top 2 bits to find the texture to use
		Scenery->FenceTexID = temp2 & (0xff >> 2);
		if (Scenery->FenceTexID == 16) {Scenery->FenceTexID = 48;} // Nasty hack for Alpine 1 track
		arrFenceTex[i] = Scenery->FenceTexID;
//		cout << Scenery->FenceTexID << " ";
//		cout << "\nFenceTexID = " << arrFenceTex[i] << " i=" << i << "\n";
		hasFence = true;
		}	

#endif;

#if 1;
fread(&temp,1,1,inputfile); // Reading texture #1
	Scenery->Texture1 = temp; 
		SCENERYARRAY[i].Texture1 = Scenery->Texture1;
	arrTex1[i] = Scenery->Texture1;
//Debug********	cout << "\nTexture #1 = 0x" << hex << Scenery->Texture1 << "\n";
//	cout << "\nTexture #1 = 0x" << hex << arrTex1[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #2
	Scenery->Texture2 = temp;
		SCENERYARRAY[i].Texture2 = Scenery->Texture2;
	arrTex2[i] = Scenery->Texture2;
//Debug********	cout << "\nTexture #2 = 0x" << hex << Scenery->Texture2 << "\n";
//	cout << "\nTexture #2 = 0x" << hex << arrTex2[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #3
	Scenery->Texture3 = temp;
		SCENERYARRAY[i].Texture3 = Scenery->Texture3;
	arrTex3[i] = Scenery->Texture3;
//Debug********	cout << "\nTexture #3 = 0x" << hex << Scenery->Texture3 << "\n";
//	cout << "\nTexture #3 = 0x" << hex << arrTex3[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #4
	Scenery->Texture4 = temp;
		SCENERYARRAY[i].Texture4 = Scenery->Texture4;
	arrTex4[i] = Scenery->Texture4;
//Debug********	cout << "\nTexture #4 = 0x" << hex << Scenery->Texture4 << "\n";
//	cout << "\nTexture #4 = 0x" << hex << arrTex4[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #5
	Scenery->Texture5 = temp; 
		SCENERYARRAY[i].Texture5 = Scenery->Texture5;
	arrTex5[i] = Scenery->Texture5;
//Debug********	cout << "\nTexture #5 = 0x" << hex << Scenery->Texture5 << "\n";
//	cout << "\nTexture #5 = 0x" << hex << arrTex5[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #6
	Scenery->Texture6 = temp; 
		SCENERYARRAY[i].Texture6 = Scenery->Texture6;
	arrTex6[i] = Scenery->Texture6;
//Debug********	cout << "\nTexture #6 = 0x" << hex << Scenery->Texture6 << "\n";
//	cout << "\nTexture #6 = 0x" << hex << arrTex6[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #7
	Scenery->Texture7 = temp; 
		SCENERYARRAY[i].Texture7 = Scenery->Texture7;
	arrTex7[i] = Scenery->Texture7;
//Debug********	cout << "\nTexture #7 = 0x" << hex << Scenery->Texture7 << "\n";
//	cout << "\nTexture #7 = 0x" << hex << arrTex7[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #8
	Scenery->Texture8 = temp;
		SCENERYARRAY[i].Texture8 = Scenery->Texture8;
	arrTex8[i] = Scenery->Texture8;
//Debug********	cout << "\nTexture #8 = 0x" << hex << Scenery->Texture8 << "\n";
//	cout << "\nTexture #8 = 0x" << hex << arrTex8[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #9
	Scenery->Texture9 = temp;
		SCENERYARRAY[i].Texture9 = Scenery->Texture9;
	arrTex9[i] = Scenery->Texture9;
//Debug********	cout << "\nTexture #9 = 0x" << hex << Scenery->Texture9 << "\n";
//	cout << "\nTexture #9 = 0x" << hex << arrTex9[i] << "\n";
fread(&temp,1,1,inputfile); // Reading texture #10
	Scenery->Texture10 = temp; 
		SCENERYARRAY[i].Texture10 = Scenery->Texture10;
	arrTex10[i] = Scenery->Texture10;
//Debug********	cout << "\nTexture #10 = 0x" << hex << Scenery->Texture10 << "\n";
//	cout << "\nTexture #10 = 0x" << hex << arrTex10[i] << "\n";

#if 0;
FILE *tmpfile;
char tmpname[20];

//itoa(Scenery->Texture1, tmpname, 10);
//strcat(tmpname, ".txt");
//tmpfile = fopen(tmpname, "a");

tmpfile = fopen("Textures.txt", "a");
//	fprintf(tmpfile, "Textures 1");
	fprintf(tmpfile, "CurrentNum = %d\n", Scenery->CurrentNum);
	fprintf(tmpfile, "tex1=%d\n", arrTex1[i]);
	fprintf(tmpfile, "tex2=%d\n", arrTex2[i]);
	fprintf(tmpfile, "tex3=%d\n", arrTex3[i]);
	fprintf(tmpfile, "tex4=%d\n", arrTex4[i]);
	fprintf(tmpfile, "tex5=%d\n", arrTex5[i]);
	fprintf(tmpfile, "tex6=%d\n", arrTex6[i]);
	fprintf(tmpfile, "tex7=%d\n", arrTex7[i]);
	fprintf(tmpfile, "tex8=%d\n", arrTex8[i]);
	fprintf(tmpfile, "tex9=%d\n", arrTex9[i]);
	fprintf(tmpfile, "tex10=%d\n", arrTex10[i]);
	fwrite("\n\n", 2, 1, tmpfile);
fclose(tmpfile);
#endif;

#if 0;
int t1=0;
	for (int x=0; x<= sizeof(arrTex1); x++) {
		t1 = arrTex1[x];
		if (t1 == 44) {
//			cout << "\nTexture #1 0x" << hex << arrTex1[x] << "\n";
//			cout << "\nCurrentNum = " << dec << Scenery->CurrentNum << "\n";
			}
		}
#endif;

fread(&temp,4,1,inputfile); // Skip 4 bytes
		SCENERYARRAY[i].refpointX24 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip reference point
		SCENERYARRAY[i].refpointZ24 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes
		SCENERYARRAY[i].refpointY24 = Swap4Bytes(temp);

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes
// 3DO Specific code here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
fread(&temp,4,1,inputfile); // a0x 
	Scenery->PointA0X = temp;
		SCENERYARRAY[i].PointA0X = Scenery->PointA0X;
		if (flag3DO == true) {
			Scenery->PointA0X = Swap4Bytes(Scenery->PointA0X);
		} 
//	cout << "\nScenery->PointA0X = 0x" << hex << Scenery->PointA0X << "\n";
fread(&temp,4,1,inputfile); // a0z
	Scenery->PointA0Z = temp;
		SCENERYARRAY[i].PointA0Z = Scenery->PointA0Z;
		// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA0Z = Swap4Bytes(Scenery->PointA0Z);
		} 
//	cout << "\nScenery->PointA0Z = 0x" << hex << Scenery->PointA0Z << "\n";

fread(&temp,4,1,inputfile); // a0y
	Scenery->PointA0Y = temp;
		SCENERYARRAY[i].PointA0Y = Scenery->PointA0Y;
		// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA0Y = Swap4Bytes(Scenery->PointA0Y);
		} 
//	cout << "\nScenery->PointA0Y = 0x" << hex << Scenery->PointA0Y << "\n";

fread(&temp,4,1,inputfile); // a1x
	Scenery->PointA1X = temp;
		SCENERYARRAY[i].PointA1X = Scenery->PointA1X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA1X = Swap4Bytes(Scenery->PointA1X);
		} 
//	cout << "\nScenery->PointA1X = 0x" << hex << Scenery->PointA1X << "\n";
fread(&temp,4,1,inputfile); // a1z
	Scenery->PointA1Z = temp;
		SCENERYARRAY[i].PointA1Z = Scenery->PointA1Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA1Z = Swap4Bytes(Scenery->PointA1Z);
		} 
//	cout << "\nScenery->PointA1Z = 0x" << hex << Scenery->PointA1Z << "\n";
fread(&temp,4,1,inputfile); // a1y
	Scenery->PointA1Y = temp;
		SCENERYARRAY[i].PointA1Y = Scenery->PointA1Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA1Y = Swap4Bytes(Scenery->PointA1Y);
		} 
//	cout << "\nScenery->PointA1Y = 0x" << hex << Scenery->PointA1Y << "\n";

fread(&temp,4,1,inputfile); // a2x
	Scenery->PointA2X = temp;
		SCENERYARRAY[i].PointA2X = Scenery->PointA2X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA2X = Swap4Bytes(Scenery->PointA2X);
		} 
//	cout << "\nScenery->PointA2X = 0x" << hex << Scenery->PointA2X << "\n";
fread(&temp,4,1,inputfile); // a2z
	Scenery->PointA2Z = temp;
		SCENERYARRAY[i].PointA2Z = Scenery->PointA2Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA2Z = Swap4Bytes(Scenery->PointA2Z);
		} 
//	cout << "\nScenery->PointA2Z = 0x" << hex << Scenery->PointA2Z << "\n";
fread(&temp,4,1,inputfile); // a2y
	Scenery->PointA2Y = temp; 
		SCENERYARRAY[i].PointA2Y = Scenery->PointA2Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA2Y = Swap4Bytes(Scenery->PointA2Y);
		} 
//	cout << "\nScenery->PointA2Y = 0x" << hex << Scenery->PointA2Y << "\n";

fread(&temp,4,1,inputfile); // a3x 
	Scenery->PointA3X = temp; 		// 3DO specific:
		SCENERYARRAY[i].PointA3X = Scenery->PointA3X;
		if (flag3DO == true) {
			Scenery->PointA3X = Swap4Bytes(Scenery->PointA3X);
		} 
//	cout << "\nScenery->PointA3X = 0x" << hex << Scenery->PointA3X << "\n";
fread(&temp,4,1,inputfile); // a3z
	Scenery->PointA3Z = temp;
		SCENERYARRAY[i].PointA3Z = Scenery->PointA3Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA3Z = Swap4Bytes(Scenery->PointA3Z);
		} 
//	cout << "\nScenery->PointA3Z = 0x" << hex << Scenery->PointA3Z << "\n";
fread(&temp,4,1,inputfile); // a3y
	Scenery->PointA3Y = temp; 
		SCENERYARRAY[i].PointA3Y = Scenery->PointA3Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA3Y = Swap4Bytes(Scenery->PointA3Y);
		} 
//	cout << "\nScenery->PointA3Y = 0x" << hex << Scenery->PointA3Y << "\n";

fread(&temp,4,1,inputfile); // a4x
	Scenery->PointA4X = temp;
		SCENERYARRAY[i].PointA4X = Scenery->PointA4X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA4X = Swap4Bytes(Scenery->PointA4X);
		} 
//	cout << "\nScenery->PointA4X = 0x" << hex << Scenery->PointA4X << "\n";
fread(&temp,4,1,inputfile); // a4z
	Scenery->PointA4Z = temp; 
		SCENERYARRAY[i].PointA4Z = Scenery->PointA4Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA4Z = Swap4Bytes(Scenery->PointA4Z);
		} 
//	cout << "\nScenery->PointA4Z = 0x" << hex << Scenery->PointA4Z << "\n";
fread(&temp,4,1,inputfile); // a4y
	Scenery->PointA4Y = temp;
		SCENERYARRAY[i].PointA4Y = Scenery->PointA4Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA4Y = Swap4Bytes(Scenery->PointA4Y);
		} 
//	cout << "\nScenery->PointA4Y = 0x" << hex << Scenery->PointA4Y << "\n";

fread(&temp,4,1,inputfile); // a5x 
	Scenery->PointA5X = temp;
		SCENERYARRAY[i].PointA5X = Scenery->PointA5X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA5X = Swap4Bytes(Scenery->PointA5X);
		} 
//	cout << "\nScenery->PointA5X = 0x" << hex << Scenery->PointA5X << "\n";
fread(&temp,4,1,inputfile); // a5z
	Scenery->PointA5Z = temp; 
		SCENERYARRAY[i].PointA5Z = Scenery->PointA5Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA5Z = Swap4Bytes(Scenery->PointA5Z);
		} 
//	cout << "\nScenery->PointA5Z = 0x" << hex << Scenery->PointA5Z << "\n";
fread(&temp,4,1,inputfile); // a5y
	Scenery->PointA5Y = temp; 
		SCENERYARRAY[i].PointA5Y = Scenery->PointA5Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA5Y = Swap4Bytes(Scenery->PointA5Y);
		} 
//	cout << "\nScenery->PointA5Y = 0x" << hex << Scenery->PointA5Y << "\n";

fread(&temp,4,1,inputfile); // a6x 
	Scenery->PointA6X = temp;
		SCENERYARRAY[i].PointA6X = Scenery->PointA6X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA6X = Swap4Bytes(Scenery->PointA6X);
		} 
//	cout << "\nScenery->PointA6X = 0x" << hex << Scenery->PointA6X << "\n";
fread(&temp,4,1,inputfile); // a6z
	Scenery->PointA6Z = temp;
		SCENERYARRAY[i].PointA6Z = Scenery->PointA6Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA6Z = Swap4Bytes(Scenery->PointA6Z);
		} 
//	cout << "\nScenery->PointA6Z = 0x" << hex << Scenery->PointA6Z << "\n";
fread(&temp,4,1,inputfile); // a6y
	Scenery->PointA6Y = temp; 
		SCENERYARRAY[i].PointA6Y = Scenery->PointA6Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA6Y = Swap4Bytes(Scenery->PointA6Y);
		} 
//	cout << "\nScenery->PointA6Y = 0x" << hex << Scenery->PointA6Y << "\n";

fread(&temp,4,1,inputfile); // a7x
	Scenery->PointA7X = temp; 
		SCENERYARRAY[i].PointA7X = Scenery->PointA7X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA7X = Swap4Bytes(Scenery->PointA7X);
		} 
//	cout << "\nScenery->PointA7X = 0x" << hex << Scenery->PointA7X << "\n";
fread(&temp,4,1,inputfile); // a7z
	Scenery->PointA7Z = temp;
		SCENERYARRAY[i].PointA7Z = Scenery->PointA7Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA7Z = Swap4Bytes(Scenery->PointA7Z);
		} 
//	cout << "\nScenery->PointA7Z = 0x" << hex << Scenery->PointA7Z << "\n";
fread(&temp,4,1,inputfile); // a7y
	Scenery->PointA7Y = temp; 
		SCENERYARRAY[i].PointA7Y = Scenery->PointA7Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA7Y = Swap4Bytes(Scenery->PointA7Y);
		} 
//	cout << "\nScenery->PointA7Y = 0x" << hex << Scenery->PointA7Y << "\n";

fread(&temp,4,1,inputfile); // a8x
	Scenery->PointA8X = temp;
		SCENERYARRAY[i].PointA8X = Scenery->PointA8X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA8X = Swap4Bytes(Scenery->PointA8X);
		} 
//	cout << "\nScenery->PointA8X = 0x" << hex << Scenery->PointA8X << "\n";
fread(&temp,4,1,inputfile); // a8z
	Scenery->PointA8Z = temp;
		SCENERYARRAY[i].PointA8Z = Scenery->PointA8Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA8Z = Swap4Bytes(Scenery->PointA8Z);
		} 
//	cout << "\nScenery->PointA8Z = 0x" << hex << Scenery->PointA8Z << "\n";
fread(&temp,4,1,inputfile); // a8y
	Scenery->PointA8Y = temp; 
		SCENERYARRAY[i].PointA8Y = Scenery->PointA8Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA8Y = Swap4Bytes(Scenery->PointA8Y);
		} 
//	cout << "\nScenery->PointA8Y = 0x" << hex << Scenery->PointA8Y << "\n";

fread(&temp,4,1,inputfile); // a9x
	Scenery->PointA9X = temp;
		SCENERYARRAY[i].PointA9X = Scenery->PointA9X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA9X = Swap4Bytes(Scenery->PointA9X);
		} 
//	cout << "\nScenery->PointA9X = 0x" << hex << Scenery->PointA9X << "\n";
fread(&temp,4,1,inputfile); // a9z
	Scenery->PointA9Z = temp; 
		SCENERYARRAY[i].PointA9Z = Scenery->PointA9Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA9Z = Swap4Bytes(Scenery->PointA9Z);
		} 
//	cout << "\nScenery->PointA9Z = 0x" << hex << Scenery->PointA9Z << "\n";
fread(&temp,4,1,inputfile); // a9y
	Scenery->PointA9Y = temp;
		SCENERYARRAY[i].PointA9Y = Scenery->PointA9Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA9Y = Swap4Bytes(Scenery->PointA9Y);
		} 
//	cout << "\nScenery->PointA9Y = 0x" << hex << Scenery->PointA9Y << "\n";

fread(&temp,4,1,inputfile); // a10x 
	Scenery->PointA10X = temp;
		SCENERYARRAY[i].PointA10X = Scenery->PointA10X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA10X = Swap4Bytes(Scenery->PointA10X);
		} 
//	cout << "\nScenery->PointA10X = 0x" << hex << Scenery->PointA10X << "\n";
fread(&temp,4,1,inputfile); // a10z
	Scenery->PointA10Z = temp;
		SCENERYARRAY[i].PointA10Z = Scenery->PointA10Z;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA10Z = Swap4Bytes(Scenery->PointA10Z);
		} 
//	cout << "\nScenery->PointA10Z = 0x" << hex << Scenery->PointA10Z << "\n";
fread(&temp,4,1,inputfile); // a10y
	Scenery->PointA10Y = temp;
		SCENERYARRAY[i].PointA10Y = Scenery->PointA10Y;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointA10Y = Swap4Bytes(Scenery->PointA10Y);
		} 
//	cout << "\nScenery->PointA10Y = 0x" << hex << Scenery->PointA10Y << "\n";

fread(&temp,4,1,inputfile); // Skip 4 bytes
		SCENERYARRAY[i].refpointXB4 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
		SCENERYARRAY[i].refpointZB4 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes
		SCENERYARRAY[i].refpointYB4 = Swap4Bytes(temp);

fread(&temp,4,1,inputfile); // b0x 
	Scenery->PointB0X = temp;
		SCENERYARRAY[i].PointB0X = Scenery->PointB0X;
			// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB0X = Swap4Bytes(Scenery->PointB0X);
		} 
//	cout << "\nScenery->PointB0X = 0x" << hex << Scenery->PointB0X << "\n";
fread(&temp,4,1,inputfile); // b0z
	Scenery->PointB0Z = temp;
		SCENERYARRAY[i].PointB0Z = Scenery->PointB0Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB0Z = Swap4Bytes(Scenery->PointB0Z);
		} 
//	cout << "\nScenery->PointB0Z = 0x" << hex << Scenery->PointB0Z << "\n";
fread(&temp,4,1,inputfile); // b0y
	Scenery->PointB0Y = temp; 
		SCENERYARRAY[i].PointB0Y = Scenery->PointB0Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB0Y = Swap4Bytes(Scenery->PointB0Y);
		} 
//	cout << "\nScenery->PointB0Y = 0x" << hex << Scenery->PointB0Y << "\n";

fread(&temp,4,1,inputfile); // b1x 
	Scenery->PointB1X = temp; 
		SCENERYARRAY[i].PointB1X = Scenery->PointB1X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB1X = Swap4Bytes(Scenery->PointB1X);
		} 
//	cout << "\nScenery->PointB1X = 0x" << hex << Scenery->PointB1X << "\n";
fread(&temp,4,1,inputfile); // b1z
	Scenery->PointB1Z = temp;
		SCENERYARRAY[i].PointB1Z = Scenery->PointB1Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB1Z = Swap4Bytes(Scenery->PointB1Z);
		} 
//	cout << "\nScenery->PointB1Z = 0x" << hex << Scenery->PointB1Z << "\n";
fread(&temp,4,1,inputfile); // b1y
	Scenery->PointB1Y = temp; 
		SCENERYARRAY[i].PointB1Y = Scenery->PointB1Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB1Y = Swap4Bytes(Scenery->PointB1Y);
		} 
//	cout << "\nScenery->PointB1Y = 0x" << hex << Scenery->PointB1Y << "\n";

fread(&temp,4,1,inputfile); // b2x 
	Scenery->PointB2X = temp;
		SCENERYARRAY[i].PointB2X = Scenery->PointB2X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB2X = Swap4Bytes(Scenery->PointB2X);
		} 
//	cout << "\nScenery->PointB2X = 0x" << hex << Scenery->PointB2X << "\n";
fread(&temp,4,1,inputfile); // b2z
	Scenery->PointB2Z = temp; 
		SCENERYARRAY[i].PointB2Z = Scenery->PointB2Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB2Z = Swap4Bytes(Scenery->PointB2Z);
		} 
//	cout << "\nScenery->PointB2Z = 0x" << hex << Scenery->PointB2Z << "\n";
fread(&temp,4,1,inputfile); // b2y
	Scenery->PointB2Y = temp; 
		SCENERYARRAY[i].PointB2Y = Scenery->PointB2Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB2Y = Swap4Bytes(Scenery->PointB2Y);
		} 
//	cout << "\nScenery->PointB2Y = 0x" << hex << Scenery->PointB2Y << "\n";

fread(&temp,4,1,inputfile); // b3x 
	Scenery->PointB3X = temp;
		SCENERYARRAY[i].PointB3X = Scenery->PointB3X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB3X = Swap4Bytes(Scenery->PointB3X);
		} 
//	cout << "\nScenery->PointB3X = 0x" << hex << Scenery->PointB3X << "\n";
fread(&temp,4,1,inputfile); // b3z
	Scenery->PointB3Z = temp;
		SCENERYARRAY[i].PointB3Z = Scenery->PointB3Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB3Z = Swap4Bytes(Scenery->PointB3Z);
		} 
//	cout << "\nScenery->PointB3Z = 0x" << hex << Scenery->PointB3Z << "\n";
fread(&temp,4,1,inputfile); // b3y
	Scenery->PointB3Y = temp;
		SCENERYARRAY[i].PointB3Y = Scenery->PointB3Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB3Y = Swap4Bytes(Scenery->PointB3Y);
		} 
//	cout << "\nScenery->PointB3Y = 0x" << hex << Scenery->PointB3Y << "\n";

fread(&temp,4,1,inputfile); // b4x
	Scenery->PointB4X = temp; 
		SCENERYARRAY[i].PointB4X = Scenery->PointB4X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB4X = Swap4Bytes(Scenery->PointB4X);
		} 
//	cout << "\nScenery->PointB4X = 0x" << hex << Scenery->PointB4X << "\n";
fread(&temp,4,1,inputfile); // b4z
	Scenery->PointB4Z = temp; 
		SCENERYARRAY[i].PointB4Z = Scenery->PointB4Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB4Z = Swap4Bytes(Scenery->PointB4Z);
		} 
//	cout << "\nScenery->PointB4Z = 0x" << hex << Scenery->PointB4Z << "\n";
fread(&temp,4,1,inputfile); // b4y
	Scenery->PointB4Y = temp;
		SCENERYARRAY[i].PointB4Y = Scenery->PointB4Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB4Y = Swap4Bytes(Scenery->PointB4Y);
		} 
//	cout << "\nScenery->PointB4Y = 0x" << hex << Scenery->PointB4Y << "\n";

fread(&temp,4,1,inputfile); // b5x 
	Scenery->PointB5X = temp;
		SCENERYARRAY[i].PointB5X = Scenery->PointB5X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB5X = Swap4Bytes(Scenery->PointB5X);
		} 
//	cout << "\nScenery->PointB5X = 0x" << hex << Scenery->PointB5X << "\n";
fread(&temp,4,1,inputfile); // b5z
	Scenery->PointB5Z = temp;
		SCENERYARRAY[i].PointB5Z = Scenery->PointB5Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB5Z = Swap4Bytes(Scenery->PointB5Z);
		} 
//	cout << "\nScenery->PointB5Z = 0x" << hex << Scenery->PointB5Z << "\n";
fread(&temp,4,1,inputfile); // b5y
	Scenery->PointB5Y = temp; 
		SCENERYARRAY[i].PointB5Y = Scenery->PointB5Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB5Y = Swap4Bytes(Scenery->PointB5Y);
		} 
//	cout << "\nScenery->PointB5Y = 0x" << hex << Scenery->PointB5Y << "\n";

fread(&temp,4,1,inputfile); // b6x
	Scenery->PointB6X = temp; 
		SCENERYARRAY[i].PointB6X = Scenery->PointB6X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB6X = Swap4Bytes(Scenery->PointB6X);
		} 
//	cout << "\nScenery->PointB6X = 0x" << hex << Scenery->PointB6X << "\n";
fread(&temp,4,1,inputfile); // b6z
	Scenery->PointB6Z = temp;
		SCENERYARRAY[i].PointB6Z = Scenery->PointB6Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB6Z = Swap4Bytes(Scenery->PointB6Z);
		} 
//	cout << "\nScenery->PointB6Z = 0x" << hex << Scenery->PointB6Z << "\n";
fread(&temp,4,1,inputfile); // b6y
	Scenery->PointB6Y = temp;
		SCENERYARRAY[i].PointB6Y = Scenery->PointB6Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB6Y = Swap4Bytes(Scenery->PointB6Y);
		} 
//	cout << "\nScenery->PointB6Y = 0x" << hex << Scenery->PointB6Y << "\n";

fread(&temp,4,1,inputfile); // b7x
	Scenery->PointB7X = temp;
		SCENERYARRAY[i].PointB7X = Scenery->PointB7X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB7X = Swap4Bytes(Scenery->PointB7X);
		} 
//	cout << "\nScenery->PointB7X = 0x" << hex << Scenery->PointB7X << "\n";
fread(&temp,4,1,inputfile); // b7z
	Scenery->PointB7Z = temp; 
		SCENERYARRAY[i].PointB7Z = Scenery->PointB7Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB7Z = Swap4Bytes(Scenery->PointB7Z);
		} 
//	cout << "\nScenery->PointB7Z = 0x" << hex << Scenery->PointB7Z << "\n";
fread(&temp,4,1,inputfile); // b7y
	Scenery->PointB7Y = temp; 
		SCENERYARRAY[i].PointB7Y = Scenery->PointB7Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB7Y = Swap4Bytes(Scenery->PointB7Y);
		} 
//	cout << "\nScenery->PointB7Y = 0x" << hex << Scenery->PointB7Y << "\n";

fread(&temp,4,1,inputfile); // b8x
	Scenery->PointB8X = temp;
		SCENERYARRAY[i].PointB8X = Scenery->PointB8X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB8X = Swap4Bytes(Scenery->PointB8X);
		} 
//	cout << "\nScenery->PointB8X = 0x" << hex << Scenery->PointB8X << "\n";
fread(&temp,4,1,inputfile); // b8z
	Scenery->PointB8Z = temp;
		SCENERYARRAY[i].PointB8Z = Scenery->PointB8Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB8Z = Swap4Bytes(Scenery->PointB8Z);
		} 
//	cout << "\nScenery->PointB8Z = 0x" << hex << Scenery->PointB8Z << "\n";
fread(&temp,4,1,inputfile); // b8y
	Scenery->PointB8Y = temp; 
		SCENERYARRAY[i].PointB8Y = Scenery->PointB8Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB8Y = Swap4Bytes(Scenery->PointB8Y);
		} 
//	cout << "\nScenery->PointB8Y = 0x" << hex << Scenery->PointB8Y << "\n";

fread(&temp,4,1,inputfile); // b9x 
	Scenery->PointB9X = temp; 
		SCENERYARRAY[i].PointB9X = Scenery->PointB9X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB9X = Swap4Bytes(Scenery->PointB9X);
		} 
//	cout << "\nScenery->PointB9X = 0x" << hex << Scenery->PointB9X << "\n";
fread(&temp,4,1,inputfile); // b9z
	Scenery->PointB9Z = temp; 
		SCENERYARRAY[i].PointB9Z = Scenery->PointB9Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB9Z = Swap4Bytes(Scenery->PointB9Z);
		} 
//	cout << "\nScenery->PointB9Z = 0x" << hex << Scenery->PointB9Z << "\n";
fread(&temp,4,1,inputfile); // b9y
	Scenery->PointB9Y = temp;
		SCENERYARRAY[i].PointB9Y = Scenery->PointB9Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB9Y = Swap4Bytes(Scenery->PointB9Y);
		} 
//	cout << "\nScenery->PointB9Y = 0x" << hex << Scenery->PointB9Y << "\n";

fread(&temp,4,1,inputfile); // b10x
	Scenery->PointB10X = temp;
		SCENERYARRAY[i].PointB10X = Scenery->PointB10X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB10X = Swap4Bytes(Scenery->PointB10X);
		} 
//	cout << "\nScenery->PointB10X = 0x" << hex << Scenery->PointB10X << "\n";
fread(&temp,4,1,inputfile); // b10z
	Scenery->PointB10Z = temp;
		SCENERYARRAY[i].PointB10Z = Scenery->PointB10Z;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB10Z = Swap4Bytes(Scenery->PointB10Z);
		} 
//	cout << "\nScenery->PointB10Z = 0x" << hex << Scenery->PointB10Z << "\n";
fread(&temp,4,1,inputfile); // b10y
	Scenery->PointB10Y = temp;
		SCENERYARRAY[i].PointB10Y = Scenery->PointB10Y;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointB10Y = Swap4Bytes(Scenery->PointB10Y);
		} 
//	cout << "\nScenery->PointB10Y = 0x" << hex << Scenery->PointB10Y << "\n";

fread(&temp,4,1,inputfile); // Skip 4 bytes
	SCENERYARRAY[i].refpointX144 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
	SCENERYARRAY[i].refpointZ144 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes
	SCENERYARRAY[i].refpointY144 = Swap4Bytes(temp);


fread(&temp,4,1,inputfile); // c0x 
	Scenery->PointC0X = temp;
		SCENERYARRAY[i].PointC0X = Scenery->PointC0X;
				// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC0X = Swap4Bytes(Scenery->PointC0X);
		} 
fread(&temp,4,1,inputfile); // c0z
	Scenery->PointC0Z = temp;
		SCENERYARRAY[i].PointC0Z = Scenery->PointC0Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC0Z = Swap4Bytes(Scenery->PointC0Z);
		} 
fread(&temp,4,1,inputfile); // c0y
	Scenery->PointC0Y = temp;
		SCENERYARRAY[i].PointC0Y = Scenery->PointC0Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC0Y = Swap4Bytes(Scenery->PointC0Y);
		} 

fread(&temp,4,1,inputfile); // c1x
	Scenery->PointC1X = temp;
		SCENERYARRAY[i].PointC1X = Scenery->PointC1X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC1X = Swap4Bytes(Scenery->PointC1X);
		} 
fread(&temp,4,1,inputfile); // c1z
	Scenery->PointC1Z = temp;
		SCENERYARRAY[i].PointC1Z = Scenery->PointC1Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC1Z = Swap4Bytes(Scenery->PointC1Z);
		} 
fread(&temp,4,1,inputfile); // c1y
	Scenery->PointC1Y = temp;
		SCENERYARRAY[i].PointC1Y = Scenery->PointC1Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC1Y = Swap4Bytes(Scenery->PointC1Y);
		} 

fread(&temp,4,1,inputfile); // c2x 
	Scenery->PointC2X = temp;
		SCENERYARRAY[i].PointC2X = Scenery->PointC2X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC2X = Swap4Bytes(Scenery->PointC2X);
		} 
fread(&temp,4,1,inputfile); // c2z
	Scenery->PointC2Z = temp;
		SCENERYARRAY[i].PointC2Z = Scenery->PointC2Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC2Z = Swap4Bytes(Scenery->PointC2Z);
		} 
fread(&temp,4,1,inputfile); // c2y
	Scenery->PointC2Y = temp;
		SCENERYARRAY[i].PointC2Y = Scenery->PointC2Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC2Y = Swap4Bytes(Scenery->PointC2Y);
		} 

fread(&temp,4,1,inputfile); // c3x 
	Scenery->PointC3X = temp;
		SCENERYARRAY[i].PointC3X = Scenery->PointC3X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC3X = Swap4Bytes(Scenery->PointC3X);
		} 
fread(&temp,4,1,inputfile); // c3z
	Scenery->PointC3Z = temp;
		SCENERYARRAY[i].PointC3Z = Scenery->PointC3Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC3Z = Swap4Bytes(Scenery->PointC3Z);
		} 
fread(&temp,4,1,inputfile); // c3y
	Scenery->PointC3Y = temp;
		SCENERYARRAY[i].PointC3Y = Scenery->PointC3Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC3Y = Swap4Bytes(Scenery->PointC3Y);
		} 

fread(&temp,4,1,inputfile); // c4x
	Scenery->PointC4X = temp;
		SCENERYARRAY[i].PointC4X = Scenery->PointC4X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC4X = Swap4Bytes(Scenery->PointC4X);
		} 
fread(&temp,4,1,inputfile); // c4z
	Scenery->PointC4Z = temp;
		SCENERYARRAY[i].PointC4Z = Scenery->PointC4Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC4Z = Swap4Bytes(Scenery->PointC4Z);
		} 
fread(&temp,4,1,inputfile); // c4y
	Scenery->PointC4Y = temp;
		SCENERYARRAY[i].PointC4Y = Scenery->PointC4Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC4Y = Swap4Bytes(Scenery->PointC4Y);
		} 

fread(&temp,4,1,inputfile); // c5x 
	Scenery->PointC5X = temp;
		SCENERYARRAY[i].PointC5X = Scenery->PointC5X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC5X = Swap4Bytes(Scenery->PointC5X);
		} 
fread(&temp,4,1,inputfile); // c5z
	Scenery->PointC5Z = temp;
		SCENERYARRAY[i].PointC5Z = Scenery->PointC5Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC5Z = Swap4Bytes(Scenery->PointC5Z);
		} 
fread(&temp,4,1,inputfile); // c5y
	Scenery->PointC5Y = temp;
		SCENERYARRAY[i].PointC5Y = Scenery->PointC5Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC5Y = Swap4Bytes(Scenery->PointC5Y);
		} 

fread(&temp,4,1,inputfile); // c6x
	Scenery->PointC6X = temp;
	SCENERYARRAY[i].PointC6X = Scenery->PointC6X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC6X = Swap4Bytes(Scenery->PointC6X);
		} 
fread(&temp,4,1,inputfile); // c6z
	Scenery->PointC6Z = temp;
		SCENERYARRAY[i].PointC6Z = Scenery->PointC6Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC6Z = Swap4Bytes(Scenery->PointC6Z);
		} 
fread(&temp,4,1,inputfile); // c6y
	Scenery->PointC6Y = temp;
		SCENERYARRAY[i].PointC6Y = Scenery->PointC6Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC6Y = Swap4Bytes(Scenery->PointC6Y);
		} 

fread(&temp,4,1,inputfile); // c7x 
	Scenery->PointC7X = temp;
		SCENERYARRAY[i].PointC7X = Scenery->PointC7X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC7X = Swap4Bytes(Scenery->PointC7X);
		} 
fread(&temp,4,1,inputfile); // c7z
	Scenery->PointC7Z = temp;
		SCENERYARRAY[i].PointC7Z = Scenery->PointC7Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC7Z = Swap4Bytes(Scenery->PointC7Z);
		} 
fread(&temp,4,1,inputfile); // c7y
	Scenery->PointC7Y = temp;
		SCENERYARRAY[i].PointC7Y = Scenery->PointC7Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC7Y = Swap4Bytes(Scenery->PointC7Y);
		} 

fread(&temp,4,1,inputfile); // c8x 
	Scenery->PointC8X = temp;
		SCENERYARRAY[i].PointC8X = Scenery->PointC8X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC8X = Swap4Bytes(Scenery->PointC8X);
		} 
fread(&temp,4,1,inputfile); // c8z
	Scenery->PointC8Z = temp;
		SCENERYARRAY[i].PointC8Z = Scenery->PointC8Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC8Z = Swap4Bytes(Scenery->PointC8Z);
		} 
fread(&temp,4,1,inputfile); // c8y
	Scenery->PointC8Y = temp;
		SCENERYARRAY[i].PointC8Y = Scenery->PointC8Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC8Y = Swap4Bytes(Scenery->PointC8Y);
		} 

fread(&temp,4,1,inputfile); // c9x
	Scenery->PointC9X = temp;
		SCENERYARRAY[i].PointC9X = Scenery->PointC9X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC9X = Swap4Bytes(Scenery->PointC9X);
		} 
fread(&temp,4,1,inputfile); // c9z
	Scenery->PointC9Z = temp;
		SCENERYARRAY[i].PointC9Z = Scenery->PointC9Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC9Z = Swap4Bytes(Scenery->PointC9Z);
		} 
fread(&temp,4,1,inputfile); // c9y
	Scenery->PointC9Y = temp;
		SCENERYARRAY[i].PointC9Y = Scenery->PointC9Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC9Y = Swap4Bytes(Scenery->PointC9Y);
		} 

fread(&temp,4,1,inputfile); // c10x
	Scenery->PointC10X = temp;
		SCENERYARRAY[i].PointC10X = Scenery->PointC10X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC10X = Swap4Bytes(Scenery->PointC10X);
		} 
fread(&temp,4,1,inputfile); // c10z
	Scenery->PointC10Z = temp;
		SCENERYARRAY[i].PointC10Z = Scenery->PointC10Z;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC10Z = Swap4Bytes(Scenery->PointC10Z);
		} 
fread(&temp,4,1,inputfile); // c10y
	Scenery->PointC10Y = temp;
		SCENERYARRAY[i].PointC10Y = Scenery->PointC10Y;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointC10Y = Swap4Bytes(Scenery->PointC10Y);
		} 

fread(&temp,4,1,inputfile); // Skip 4 bytes
	SCENERYARRAY[i].refpointX1D4 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
	SCENERYARRAY[i].refpointZ1D4 = Swap4Bytes(temp);
fread(&temp,4,1,inputfile); // Skip 4 bytes
	SCENERYARRAY[i].refpointY1D4 = Swap4Bytes(temp);

fread(&temp,4,1,inputfile); // d0x 
	Scenery->PointD0X = temp;
		SCENERYARRAY[i].PointD0X = Scenery->PointD0X;
					// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD0X = Swap4Bytes(Scenery->PointD0X);
		} 
//	cout << "\nScenery->PointD0X = 0x" << hex << Scenery->PointD0X << "\n";
fread(&temp,4,1,inputfile); // d0z
	Scenery->PointD0Z = temp;
		SCENERYARRAY[i].PointD0Z = Scenery->PointD0Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD0Z = Swap4Bytes(Scenery->PointD0Z);
		} 
//	cout << "\nScenery->PointD0Z = 0x" << hex << Scenery->PointD0Z << "\n";
fread(&temp,4,1,inputfile); // d0y
	Scenery->PointD0Y = temp;
		SCENERYARRAY[i].PointD0Y = Scenery->PointD0Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD0Y = Swap4Bytes(Scenery->PointD0Y);
		} 
//	cout << "\nScenery->PointD0Y = 0x" << hex << Scenery->PointD0Y << "\n";

fread(&temp,4,1,inputfile); // d1x 
	Scenery->PointD1X = temp;
		SCENERYARRAY[i].PointD1X = Scenery->PointD1X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD1X = Swap4Bytes(Scenery->PointD1X);
		} 
fread(&temp,4,1,inputfile); // d1z
	Scenery->PointD1Z = temp;
		SCENERYARRAY[i].PointD1Z = Scenery->PointD1Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD1Z = Swap4Bytes(Scenery->PointD1Z);
		} 
fread(&temp,4,1,inputfile); // d1y
	Scenery->PointD1Y = temp;
		SCENERYARRAY[i].PointD1Y = Scenery->PointD1Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD1Y = Swap4Bytes(Scenery->PointD1Y);
		} 

fread(&temp,4,1,inputfile); // d2x 
	Scenery->PointD2X = temp;
		SCENERYARRAY[i].PointD2X = Scenery->PointD2X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD2X = Swap4Bytes(Scenery->PointD2X);
		} 
fread(&temp,4,1,inputfile); // d2z
	Scenery->PointD2Z = temp;
		SCENERYARRAY[i].PointD2Z = Scenery->PointD2Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD2Z = Swap4Bytes(Scenery->PointD2Z);
		} 
fread(&temp,4,1,inputfile); // d2y
	Scenery->PointD2Y = temp;
		SCENERYARRAY[i].PointD2Y = Scenery->PointD2Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD2Y = Swap4Bytes(Scenery->PointD2Y);
		} 

fread(&temp,4,1,inputfile); // d3x
	Scenery->PointD3X = temp;
		SCENERYARRAY[i].PointD3X = Scenery->PointD3X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD3X = Swap4Bytes(Scenery->PointD3X);
		} 
fread(&temp,4,1,inputfile); // d3z
	Scenery->PointD3Z = temp;
		SCENERYARRAY[i].PointD3Z = Scenery->PointD3Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD3Z = Swap4Bytes(Scenery->PointD3Z);
		} 
fread(&temp,4,1,inputfile); // d3y
	Scenery->PointD3Y = temp;
		SCENERYARRAY[i].PointD3Y = Scenery->PointD3Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD3Y = Swap4Bytes(Scenery->PointD3Y);
		} 

fread(&temp,4,1,inputfile); // d4x 
	Scenery->PointD4X = temp;
		SCENERYARRAY[i].PointD4X = Scenery->PointD4X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD4X = Swap4Bytes(Scenery->PointD4X);
		} 
fread(&temp,4,1,inputfile); // d4z
	Scenery->PointD4Z = temp;
		SCENERYARRAY[i].PointD4Z = Scenery->PointD4Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD4Z = Swap4Bytes(Scenery->PointD4Z);
		} 
fread(&temp,4,1,inputfile); // d4y
	Scenery->PointD4Y = temp;
		SCENERYARRAY[i].PointD4Y = Scenery->PointD4Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD4Y = Swap4Bytes(Scenery->PointD4Y);
		} 

fread(&temp,4,1,inputfile); // d5x 
	Scenery->PointD5X = temp;
		SCENERYARRAY[i].PointD5X = Scenery->PointD5X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD5X = Swap4Bytes(Scenery->PointD5X);
		} 
fread(&temp,4,1,inputfile); // d5z
	Scenery->PointD5Z = temp;		
		SCENERYARRAY[i].PointD5Z = Scenery->PointD5Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD5Z = Swap4Bytes(Scenery->PointD5Z);
		} 
fread(&temp,4,1,inputfile); // d5y
	Scenery->PointD5Y = temp;
		SCENERYARRAY[i].PointD5Y = Scenery->PointD5Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD5Y = Swap4Bytes(Scenery->PointD5Y);
		} 

fread(&temp,4,1,inputfile); // d6x
	Scenery->PointD6X = temp;
		SCENERYARRAY[i].PointD6X = Scenery->PointD6X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD6X = Swap4Bytes(Scenery->PointD6X);
		} 
fread(&temp,4,1,inputfile); // d6z
	Scenery->PointD6Z = temp;
		SCENERYARRAY[i].PointD6Z = Scenery->PointD6Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD6Z = Swap4Bytes(Scenery->PointD6Z);
		} 
fread(&temp,4,1,inputfile); // d6y
	Scenery->PointD6Y = temp;
		SCENERYARRAY[i].PointD6Y = Scenery->PointD6Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD6Y = Swap4Bytes(Scenery->PointD6Y);
		} 

fread(&temp,4,1,inputfile); // d7x
	Scenery->PointD7X = temp;
			SCENERYARRAY[i].PointD7X = Scenery->PointD7X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD7X = Swap4Bytes(Scenery->PointD7X);
		} 
fread(&temp,4,1,inputfile); // d7z
	Scenery->PointD7Z = temp;
			SCENERYARRAY[i].PointD7Z = Scenery->PointD7Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD7Z = Swap4Bytes(Scenery->PointD7Z);
		} 
fread(&temp,4,1,inputfile); // d7y
	Scenery->PointD7Y = temp;
			SCENERYARRAY[i].PointD7Y = Scenery->PointD7Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD7Y = Swap4Bytes(Scenery->PointD7Y);
		} 

fread(&temp,4,1,inputfile); // d8x
	Scenery->PointD8X = temp;
			SCENERYARRAY[i].PointD8X = Scenery->PointD8X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD8X = Swap4Bytes(Scenery->PointD8X);
		} 
fread(&temp,4,1,inputfile); // d8z
	Scenery->PointD8Z = temp;
			SCENERYARRAY[i].PointD8Z = Scenery->PointD8Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD8Z = Swap4Bytes(Scenery->PointD8Z);
		} 
fread(&temp,4,1,inputfile); // d8y
	Scenery->PointD8Y = temp;
			SCENERYARRAY[i].PointD8Y = Scenery->PointD8Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD8Y = Swap4Bytes(Scenery->PointD8Y);
		} 

fread(&temp,4,1,inputfile); // d9x
	Scenery->PointD9X = temp;
			SCENERYARRAY[i].PointD9X = Scenery->PointD9X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD9X = Swap4Bytes(Scenery->PointD9X);
		} 
fread(&temp,4,1,inputfile); // d9z
	Scenery->PointD9Z = temp;
			SCENERYARRAY[i].PointD9Z = Scenery->PointD9Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD9Z = Swap4Bytes(Scenery->PointD9Z);
		} 
fread(&temp,4,1,inputfile); // d9y
	Scenery->PointD9Y = temp;
			SCENERYARRAY[i].PointD9Y = Scenery->PointD9Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD9Y = Swap4Bytes(Scenery->PointD9Y);
		} 

fread(&temp,4,1,inputfile); // d10x 
	Scenery->PointD10X = temp;
			SCENERYARRAY[i].PointD10X = Scenery->PointD10X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD10X = Swap4Bytes(Scenery->PointD10X);
		} 
//	cout << "\nScenery->PointD10X = 0x" << hex << Scenery->PointD10X << "\n";

fread(&temp,4,1,inputfile); // d10z
	Scenery->PointD10Z = temp;
			SCENERYARRAY[i].PointD10Z = Scenery->PointD10Z;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD10Z = Swap4Bytes(Scenery->PointD10Z);
		} 
//	cout << "\nScenery->PointD10Z = 0x" << hex << Scenery->PointD10Z << "\n";

fread(&temp,4,1,inputfile); // d10y
	Scenery->PointD10Y = temp;
			SCENERYARRAY[i].PointD10Y = Scenery->PointD10Y;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointD10Y = Swap4Bytes(Scenery->PointD10Y);
		} 
//	cout << "\nScenery->PointD10Y = 0x" << hex << Scenery->PointD10Y << "\n";

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

#if 1; // E0 3DO reading
// 3DO Specific: Don't skip next 12 bytes. Read E0X, E0Y, E0Z now!
	if (flag3DO == true) {
		fread(&temp,4,1,inputfile); // e0x
//		cout << "\ni = " << dec << Scenery->CurrentNum << "\n";
			Scenery->PointE0X = temp;
				SCENERYARRAY[i].PointE0X = Scenery->PointE0X;
//			cout << "\nNot swaped Scenery->PointE0X = 0x" << hex << Scenery->PointE0X << "\n";
			Scenery->PointE0X = Swap4Bytes(Scenery->PointE0X);
//			cout << "\nScenery->PointE0X = 0x" << hex << Scenery->PointE0X << "\n";
		fread(&temp,4,1,inputfile); // e0z
			Scenery->PointE0Z = temp;
				SCENERYARRAY[i].PointE0Z = Scenery->PointE0Z;
//			cout << "\nNot swaped Scenery->PointE0Z = 0x" << hex << Scenery->PointE0Z << "\n";
			Scenery->PointE0Z = Swap4Bytes(Scenery->PointE0Z);
//			cout << "\nScenery->PointE0Z = 0x" << hex << Scenery->PointE0Z << "\n";
		fread(&temp,4,1,inputfile); // e0y
			Scenery->PointE0Y = temp;
				SCENERYARRAY[i].PointE0Y = Scenery->PointE0Y;
//			cout << "\nNot swaped Scenery->PointE0Y = 0x" << hex << Scenery->PointE0Y << "\n";
			Scenery->PointE0Y = Swap4Bytes(Scenery->PointE0Y);
//			cout << "\nScenery->PointE0Y = 0x" << hex << Scenery->PointE0Y << "\n";
				fread(&temp,4,1,inputfile); // Skip 4 bytes
				fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
				fread(&temp,4,1,inputfile); // Skip 4 bytes
				fread(&temp,4,1,inputfile); // Skip 4 bytes
				fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
				fread(&temp,4,1,inputfile); // Skip 4 bytes
//				cout << "+++++++++++++++++++++" << Scenery->PointD10Z << "\n";
				int a, b, c;
					a = Scenery->PointE0X - Scenery->PointD0X;
					b = Scenery->PointE0Y - Scenery->PointD0Y;
					c = Scenery->PointE0Z - Scenery->PointD0Z;
					if ((Scenery->CurrentNum>100)&&(a<10)||(b<10)||(c<10)) {
//				if ((Scenery->CurrentNum>100)&&(Scenery->PointE0X==0)||(Scenery->PointE0Y==0)||(Scenery->PointE0Z==0)) {
//				if ((Scenery->CurrentNum>100)&&(Scenery->PointE0X<1)||(Scenery->PointE0Y<1)||(Scenery->PointE0Z<1)) {
//				if ((Scenery->CurrentNum>100)&&(Scenery->PointE0X<10)) {
	// 			if (Scenery->CurrentNum>258) {///////BRUTE HACK!/////////////////// CHECK THIS AND FIX!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///					Scenery->PointE0X = ArrayXYZ[Scenery->CurrentNum*4+5][0]*SCALE;
///					Scenery->PointE0Y = -ArrayXYZ[Scenery->CurrentNum*4+5][1]*SCALE;
///					Scenery->PointE0Z = ArrayXYZ[Scenery->CurrentNum*4+5][2]*SCALE;
#if 1; // get pos
						fgetpos(inputfile, &pos);
						fseek(inputfile, pos-12, SEEK_SET);	// step back for 12 bytes

					fread(&temp,4,1,inputfile); // Skip 4 bytes
							Scenery->PointE0X = temp;
							Scenery->PointE0X = Swap4Bytes(Scenery->PointE0X);
//							cout << "\nScenery->PointE0X = 0x" << hex << Scenery->PointE0X << "\n";
					fread(&temp,4,1,inputfile); // Skip 4 bytes
							Scenery->PointE0Z = temp;
							Scenery->PointE0Z = Swap4Bytes(Scenery->PointE0Z);
//							cout << "\nScenery->PointE0Z = 0x" << hex << Scenery->PointE0Z << "\n";
					fread(&temp,4,1,inputfile); // Skip 4 bytes
							Scenery->PointE0Y = temp;
							Scenery->PointE0Y = Swap4Bytes(Scenery->PointE0Y);
//							cout << "\nScenery->PointE0Y = 0x" << hex << Scenery->PointE0Y << "\n";
#endif; // get pos
//					cout << "\nScenery->PointE0X corr = 0x" << hex << Scenery->PointE0X << "\n";
//					cout << "\nScenery->PointE0Z corr = 0x" << hex << Scenery->PointE0Z << "\n";
//					cout << "\nScenery->PointE0Y corr = 0x" << hex << Scenery->PointE0Y << "\n";
					}
				goto readE1;
		}
// Skip 0x18 and read E01
#endif;// E0 3DO reading


fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // e0x
	Scenery->PointE0X = temp;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE0X = Swap4Bytes(Scenery->PointE0X);
		}
fread(&temp,4,1,inputfile); // e0z
	Scenery->PointE0Z = temp;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE0Z = Swap4Bytes(Scenery->PointE0Z);
		}
fread(&temp,4,1,inputfile); // e0y
	Scenery->PointE0Y = temp;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE0Y = Swap4Bytes(Scenery->PointE0Y);
		}

readE1: // Read 3DO E1
//cout << "+++++++++++++++++++++" << Scenery->PointE0X << "\n";
fread(&temp,4,1,inputfile); // e1x 
	Scenery->PointE1X = temp;
		SCENERYARRAY[i].PointE1X = Scenery->PointE1X;
						// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE1X = Swap4Bytes(Scenery->PointE1X);
		} 
fread(&temp,4,1,inputfile); // e1z
	Scenery->PointE1Z = temp;
		SCENERYARRAY[i].PointE1Z = Scenery->PointE1Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE1Z = Swap4Bytes(Scenery->PointE1Z);
		} 
fread(&temp,4,1,inputfile); // e1y
	Scenery->PointE1Y = temp;
		SCENERYARRAY[i].PointE1Y = Scenery->PointE1Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE1Y = Swap4Bytes(Scenery->PointE1Y);
		} 

fread(&temp,4,1,inputfile); // e2x
	Scenery->PointE2X = temp;
		SCENERYARRAY[i].PointE2X = Scenery->PointE2X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE2X = Swap4Bytes(Scenery->PointE2X);
		} 
fread(&temp,4,1,inputfile); // e2z
	Scenery->PointE2Z = temp;
		SCENERYARRAY[i].PointE2Z = Scenery->PointE2Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE2Z = Swap4Bytes(Scenery->PointE2Z);
		} 
fread(&temp,4,1,inputfile); // e2y
	Scenery->PointE2Y = temp;
		SCENERYARRAY[i].PointE2Y = Scenery->PointE2Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE2Y = Swap4Bytes(Scenery->PointE2Y);
		} 

fread(&temp,4,1,inputfile); // e3x 
	Scenery->PointE3X = temp;
		SCENERYARRAY[i].PointE3X = Scenery->PointE3X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE3X = Swap4Bytes(Scenery->PointE3X);
		} 
//cout << "\nScenery->PointE3X = 0x" << hex << Scenery->PointE3X << "\n";
fread(&temp,4,1,inputfile); // e3z
	Scenery->PointE3Z = temp;
		SCENERYARRAY[i].PointE3Z = Scenery->PointE3Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE3Z = Swap4Bytes(Scenery->PointE3Z);
		} 
//cout << "\nScenery->PointE3Z = 0x" << hex << Scenery->PointE3Z << "\n";
fread(&temp,4,1,inputfile); // e3y
	Scenery->PointE3Y = temp;
		SCENERYARRAY[i].PointE3Y = Scenery->PointE3Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE3Y = Swap4Bytes(Scenery->PointE3Y);
		} 
//cout << "\nScenery->PointE3Y = 0x" << hex << Scenery->PointE3Y << "\n";

fread(&temp,4,1,inputfile); // e4x
	Scenery->PointE4X = temp;
		SCENERYARRAY[i].PointE4X = Scenery->PointE4X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE4X = Swap4Bytes(Scenery->PointE4X);
		} 
//cout << "\nScenery->PointE4X = 0x" << hex << Scenery->PointE4X << "\n";
fread(&temp,4,1,inputfile); // e4z
	Scenery->PointE4Z = temp;
		SCENERYARRAY[i].PointE4Z = Scenery->PointE4Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE4Z = Swap4Bytes(Scenery->PointE4Z);
		} 
//cout << "\nScenery->PointE4Z = 0x" << hex << Scenery->PointE4Z << "\n";
fread(&temp,4,1,inputfile); // e4y
	Scenery->PointE4Y = temp;
		SCENERYARRAY[i].PointE4Y = Scenery->PointE4Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE4Y = Swap4Bytes(Scenery->PointE4Y);
		} 
//cout << "\nScenery->PointE4Y = 0x" << hex << Scenery->PointE4Y << "\n";

fread(&temp,4,1,inputfile); // e5x 
	Scenery->PointE5X = temp;
		SCENERYARRAY[i].PointE5X = Scenery->PointE5X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE5X = Swap4Bytes(Scenery->PointE5X);
		} 
fread(&temp,4,1,inputfile); // e5z
	Scenery->PointE5Z = temp;
		SCENERYARRAY[i].PointE5Z = Scenery->PointE5Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE5Z = Swap4Bytes(Scenery->PointE5Z);
		} 
fread(&temp,4,1,inputfile); // e5y
	Scenery->PointE5Y = temp;
		SCENERYARRAY[i].PointE5Y = Scenery->PointE5Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE5Y = Swap4Bytes(Scenery->PointE5Y);
		} 

fread(&temp,4,1,inputfile); // e6x
	Scenery->PointE6X = temp;
			SCENERYARRAY[i].PointE6X = Scenery->PointE6X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE6X = Swap4Bytes(Scenery->PointE6X);
		} 
fread(&temp,4,1,inputfile); // e6z
	Scenery->PointE6Z = temp;
			SCENERYARRAY[i].PointE6Z = Scenery->PointE6Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE6Z = Swap4Bytes(Scenery->PointE6Z);
		} 
fread(&temp,4,1,inputfile); // e6y
	Scenery->PointE6Y = temp;
			SCENERYARRAY[i].PointE6Y = Scenery->PointE6Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE6Y = Swap4Bytes(Scenery->PointE6Y);
		} 

fread(&temp,4,1,inputfile); // e7x
	Scenery->PointE7X = temp;
			SCENERYARRAY[i].PointE7X = Scenery->PointE7X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE7X = Swap4Bytes(Scenery->PointE7X);
		} 
fread(&temp,4,1,inputfile); // e7z
	Scenery->PointE7Z = temp;
			SCENERYARRAY[i].PointE7Z = Scenery->PointE7Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE7Z = Swap4Bytes(Scenery->PointE7Z);
		} 
fread(&temp,4,1,inputfile); // e7y
	Scenery->PointE7Y = temp;
			SCENERYARRAY[i].PointE7Y = Scenery->PointE7Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE7Y = Swap4Bytes(Scenery->PointE7Y);
		} 

fread(&temp,4,1,inputfile); // e8x
	Scenery->PointE8X = temp;
			SCENERYARRAY[i].PointE8X = Scenery->PointE8X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE8X = Swap4Bytes(Scenery->PointE8X);
		} 
fread(&temp,4,1,inputfile); // e8z
	Scenery->PointE8Z = temp;
			SCENERYARRAY[i].PointE8Z = Scenery->PointE8Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE8Z = Swap4Bytes(Scenery->PointE8Z);
		} 
fread(&temp,4,1,inputfile); // e8y
	Scenery->PointE8Y = temp;
			SCENERYARRAY[i].PointE8Y = Scenery->PointE8Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE8Y = Swap4Bytes(Scenery->PointE8Y);
		} 

fread(&temp,4,1,inputfile); // e9x 
	Scenery->PointE9X = temp;
			SCENERYARRAY[i].PointE9X = Scenery->PointE9X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE9X = Swap4Bytes(Scenery->PointE9X);
		} 
fread(&temp,4,1,inputfile); // e9z
	Scenery->PointE9Z = temp;
			SCENERYARRAY[i].PointE9Z = Scenery->PointE9Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE9Z = Swap4Bytes(Scenery->PointE9Z);
		} 
fread(&temp,4,1,inputfile); // e9y
	Scenery->PointE9Y = temp;
			SCENERYARRAY[i].PointE9Y = Scenery->PointE9Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE9Y = Swap4Bytes(Scenery->PointE9Y);
		} 

fread(&temp,4,1,inputfile); // e10x
	Scenery->PointE10X = temp;
			SCENERYARRAY[i].PointE10X = Scenery->PointE10X;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE10X = Swap4Bytes(Scenery->PointE10X);
		} 
fread(&temp,4,1,inputfile); // e10z
	Scenery->PointE10Z = temp;
			SCENERYARRAY[i].PointE10Z = Scenery->PointE10Z;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE10Z = Swap4Bytes(Scenery->PointE10Z);
		} 
fread(&temp,4,1,inputfile); // e10y
	Scenery->PointE10Y = temp;
			SCENERYARRAY[i].PointE10Y = Scenery->PointE10Y;
							// 3DO specific:
		if (flag3DO == true) {
			Scenery->PointE10Y = Swap4Bytes(Scenery->PointE10Y);
		} 
//cout << "+++++++++++++++++++++" << hex << Scenery->PointE9X << "\n";
#endif; // Reading coords

// 3DO Specific code here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// If 3DO flag was set, proceed to export.
	if (flag3DO == true) {
		goto cont3DO;
		}

	for	(j=1; j<=149; j++) {
		fread(&temp,4,1,inputfile); // Skip 4 bytes
		}

cont3DO:
#if 0;
a0x, a0y, a0z, a1x, a1y, a1z, a2x, a2y, a2z, a3x, a3y, a3z, a4x, a4y, a4z, a5x, a5y, a5z, a6x, a6y, a6z, a7x, a7y, a7z, a8x, a8y, a8z, a9x, a9y, a9z, a10x, a10y, a10z; // Float coords
b0x, b0y, b0z, b1x, b1y, b1z, b2x, b2y, b2z, b3x, b3y, b3z, b4x, b4y, b4z, b5x, b5y, b5z, b6x, b6y, b6z, b7x, b7y, b7z, b8x, b8y, b8z, b9x, b9y, b9z, b10x, b10y, b10z;
c0x, c0y, c0z, c1x, c1y, c1z, c2x, c2y, c2z, c3x, c3y, c3z, c4x, c4y, c4z, c5x, c5y, c5z, c6x, c6y, c6z, c7x, c7y, c7z, c8x, c8y, c8z, c9x, c9y, c9z, c10x, c10y, c10z;
d0x, d0y, d0z, d1x, d1y, d1z, d2x, d2y, d2z, d3x, d3y, d3z, d4x, d4y, d4z, d5x, d5y, d5z, d6x, d6y, d6z, d7x, d7y, d7z, d8x, d8y, d8z, d9x, d9y, d9z, d10x, d10y, d10z;
e0x, e0y, e0z, e1x, e1y, e1z, e2x, e2y, e2z, e3x, e3y, e3z, e4x, e4y, e4z, e5x, e5y, e5z, e6x, e6y, e6z, e7x, e7y, e7z, e8x, e8y, e8z, e9x, e9y, e9z, e10x, e10y, e10z;
#endif;

////******************/////////////////******************
////******************FENCES///////////******************
/////////////////////////////////////////////////////////
#if 1; // FENCES BASED ON SCENERY
	if (hasFence == true) { // Fence was found previously 
		// Writing fences.
		if (Scenery->HasLeftFence != 0) {

			if ((flag3DO==true)&&(VRArrFlag4[i*4]==1)) { // It's only for the Alpine segment 1 track. Fence on the left near track start.
				// If lanes are splited and fence on the left exists. Nasty hack, eh?.	MAKE FOR REVERSED TRACK!!!
				// Add vertices above A8, B8, C8, D8, E8 instead of *7.
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA8Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointA8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB8Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointB8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC8Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointC8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD8Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointD8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE8Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointE8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

						// Add REAL vertices A8, B8, C8, D8, E8 instead of *7.
//			#if 1; //reaL v
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA8Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointA8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB8Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointB8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC8Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointC8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD8Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointD8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE8X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE8Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointE8Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

				} else {

			// Add vertices above A7, B7, C7, D7, E7.
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA7Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointA7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB7Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointB7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC7Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointC7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD7Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointD7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE7Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointE7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

						// Add REAL vertices A7, B7, C7, D7, E7.
			#if 1; //reaL v
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA7Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointA7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB7Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointB7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC7Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointC7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD7Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointD7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE7X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE7Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointE7Y / SCALE);
			fwrite("\n",1,1,outf4); // New line
			}

			#endif; // real v

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter += 2;
			fwrite("\n",1,1,outf4); // New line

			// Bottom lines
			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter += 2;
			fwrite("\n",1,1,outf4); // New line

#if 1;
// Write vt params. There are 10 vertices. Max uv coord is 1.0!
			if (flag3DO == true) {
			fprintf(outf4, "\n");
fprintf(outf4, "vt 0.000000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.000000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.250000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.250000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.500000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.500000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.750000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.750000 1.000000 1.000000\n");
fprintf(outf4, "vt 1.000000 0.000000 1.000000\n");
fprintf(outf4, "vt 1.000000 1.000000 1.000000\n");
			fprintf(outf4, "usemtl %d\n", arrFenceTex[i]);
				} else {

fprintf(outf4, "\n");
fprintf(outf4, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf4, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf4, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf4, "usemtl %d\n", arrFenceTex[i]);
			}
#endif; // vt

			#if 1; // Write polygons
//			for (p=0; p<4; p++) {
				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+1)); 
					fprintf(outf4, "/%d ", matCounter2+1);  // vt index
				fprintf(outf4, "%d", (p+2));
					fprintf(outf4, "/%d ", matCounter2+4);  // vt index
				fprintf(outf4, "%d", (p+7));
					fprintf(outf4, "/%d ", matCounter2+3);  // vt index
				fprintf(outf4, "%d", (p+6));
					fprintf(outf4, "/%d ", matCounter2+2);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+2)); 
					fprintf(outf4, "/%d ", matCounter2+4);  // vt index
				fprintf(outf4, "%d", (p+3));
					fprintf(outf4, "/%d ", matCounter2+6);  // vt index
				fprintf(outf4, "%d", (p+8));
					fprintf(outf4, "/%d ", matCounter2+5);  // vt index
				fprintf(outf4, "%d", (p+7));
					fprintf(outf4, "/%d ", matCounter2+3);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+3)); 
					fprintf(outf4, "/%d ", matCounter2+6);  // vt index
				fprintf(outf4, "%d", (p+4));
					fprintf(outf4, "/%d ", matCounter2+8);  // vt index
				fprintf(outf4, "%d", (p+9)); 
					fprintf(outf4, "/%d ", matCounter2+7);  // vt index
				fprintf(outf4, "%d", (p+8));
					fprintf(outf4, "/%d ", matCounter2+5);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+4));
					fprintf(outf4, "/%d ", matCounter2+8);  // vt index
				fprintf(outf4, "%d", (p+5));
					fprintf(outf4, "/%d ", matCounter2+10);  // vt index
				fprintf(outf4, "%d", (p+10)); 
					fprintf(outf4, "/%d ", matCounter2+9);  // vt index
				fprintf(outf4, "%d", (p+9));
					fprintf(outf4, "/%d ", matCounter2+7);  // vt index
				fwrite("\n",1,1,outf4);// New line
				p += 10;
				matCounter2 += 10;
//				}
			//p++;
			#endif; // polygons

				// Make vertical lines: 1-6, 2-7, 3-8, 4-9, 5-10
//			fprintf(outf4, "l ");
//			fprintf(outf4, "%d ", linesCounter);
//			fprintf(outf4, "%d ", linesCounter+5);
//			fwrite("\n",1,1,outf4); // New line

			}

		if (Scenery->HasRightFence != 0) {
			// Add vertices above A2, B2, C2, D2, E2.
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA2Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointA2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB2Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointB2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC2Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointC2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD2Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointD2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE2Z / SCALE + FENCEHEIGHT);
			fprintf(outf4, "%f ", -(float)Scenery->PointE2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

						// Add REAL vertices A2, B2, C2, D2, E2.
			#if 1; //reaL v
			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointA2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointA2Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointA2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointB2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointB2Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointB2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointC2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointC2Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointC2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointD2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointD2Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointD2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "v ");
			fprintf(outf4, "%f ", (float)Scenery->PointE2X / SCALE);
			fprintf(outf4, "%f ", (float)Scenery->PointE2Z / SCALE);
			fprintf(outf4, "%f ", -(float)Scenery->PointE2Y / SCALE);
			fwrite("\n",1,1,outf4); // New line
			#endif; // real v


			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter +=2;
			fwrite("\n",1,1,outf4); // New line

			// Bottom lines
			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf4); // New line

			fprintf(outf4, "l ");
			fprintf(outf4, "%d ", linesCounter);
			fprintf(outf4, "%d ", linesCounter+1);
			linesCounter +=2;
			fwrite("\n",1,1,outf4); // New line

#if 1;
// Write vt params. There are 10 vertices. Max uv coord is 1.0!
	if (flag3DO == true) {
			fprintf(outf4, "\n");
fprintf(outf4, "vt 0.000000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.000000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.250000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.250000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.500000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.500000 1.000000 1.000000\n");
fprintf(outf4, "vt 0.750000 0.000000 1.000000\n");
fprintf(outf4, "vt 0.750000 1.000000 1.000000\n");
fprintf(outf4, "vt 1.000000 0.000000 1.000000\n");
fprintf(outf4, "vt 1.000000 1.000000 1.000000\n");
			fprintf(outf4, "usemtl %d\n", arrFenceTex[i]);
				} else {

fprintf(outf4, "\n");
fprintf(outf4, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf4, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf4, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf4, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf4, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf4, "usemtl %d\n", arrFenceTex[i]);
	}
#endif; // vt

			#if 1; // Write polygons
//			for (p=0; p<4; p++) {
				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+1)); 
					fprintf(outf4, "/%d ", matCounter2+1);  // vt index
				fprintf(outf4, "%d", (p+2));
					fprintf(outf4, "/%d ", matCounter2+4);  // vt index
				fprintf(outf4, "%d", (p+7)); 
					fprintf(outf4, "/%d ", matCounter2+3);  // vt index
				fprintf(outf4, "%d", (p+6));
					fprintf(outf4, "/%d ", matCounter2+2);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+2));
					fprintf(outf4, "/%d ", matCounter2+4);  // vt index
				fprintf(outf4, "%d", (p+3));
					fprintf(outf4, "/%d ", matCounter2+6);  // vt index
				fprintf(outf4, "%d", (p+8));
					fprintf(outf4, "/%d ", matCounter2+5);  // vt index
				fprintf(outf4, "%d", (p+7));
					fprintf(outf4, "/%d ", matCounter2+3);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+3));
					fprintf(outf4, "/%d ", matCounter2+6);  // vt index
				fprintf(outf4, "%d", (p+4));
					fprintf(outf4, "/%d ", matCounter2+8);  // vt index
				fprintf(outf4, "%d", (p+9));
					fprintf(outf4, "/%d ", matCounter2+7);  // vt index
				fprintf(outf4, "%d", (p+8));
					fprintf(outf4, "/%d ", matCounter2+5);  // vt index
				fwrite("\n",1,1,outf4);// New line

				fwrite("f ",2,1,outf4);
				fprintf(outf4, "%d", (p+4)); 
					fprintf(outf4, "/%d ", matCounter2+8);  // vt index
				fprintf(outf4, "%d", (p+5));
					fprintf(outf4, "/%d ", matCounter2+10);  // vt index
				fprintf(outf4, "%d", (p+10));
					fprintf(outf4, "/%d ", matCounter2+9);  // vt index
				fprintf(outf4, "%d", (p+9));
					fprintf(outf4, "/%d ", matCounter2+7);  // vt index
				fwrite("\n",1,1,outf4);// New line
				p += 10;
//				}
			//p++;
			#endif; // polygons
			}
		}
#endif; // FENCES BASED ON SCENERY

////*******************////////////////******************
/////////////////////FENCES////////////******************
/////////////////////////////////////////////////////////


// Write using VirtRd flag4 here:
// Redefine some coords depending on Flag4.
		// Check E coords below!
		// Remove polys near tunnel wall!
#if 1; // Flag4 //////////// TURN OFF TO REWRAP TUNNELS
		if (VRArrFlag4[i*4+4] == LEFT_TUNNEL) {// Left tunnel
//				tunnelLeft = true; // Set the flag
			Scenery->PointA9X = Scenery->PointA4X;
			Scenery->PointA9Y = Scenery->PointA4Y;
			Scenery->PointA9Z = Scenery->PointA4Z;

			Scenery->PointB9X = Scenery->PointB4X;
			Scenery->PointB9Y = Scenery->PointB4Y;
			Scenery->PointB9Z = Scenery->PointB4Z;

			Scenery->PointC9X = Scenery->PointC4X;
			Scenery->PointC9Y = Scenery->PointC4Y;
			Scenery->PointC9Z = Scenery->PointC4Z;

			Scenery->PointD9X = Scenery->PointD4X;
			Scenery->PointD9Y = Scenery->PointD4Y;
			Scenery->PointD9Z = Scenery->PointD4Z;

			Scenery->PointE9X = Scenery->PointE4X;
			Scenery->PointE9Y = Scenery->PointE4Y;
			Scenery->PointE9Z = Scenery->PointE4Z;
////
			Scenery->PointA10X = Scenery->PointA7X;
			Scenery->PointA10Y = Scenery->PointA7Y;
			Scenery->PointA10Z = Scenery->PointA7Z;

			Scenery->PointB10X = Scenery->PointB7X;
			Scenery->PointB10Y = Scenery->PointB7Y;
			Scenery->PointB10Z = Scenery->PointB7Z;

			Scenery->PointC10X = Scenery->PointC7X;
			Scenery->PointC10Y = Scenery->PointC7Y;
			Scenery->PointC10Z = Scenery->PointC7Z;

			Scenery->PointD10X = Scenery->PointD7X;
			Scenery->PointD10Y = Scenery->PointD7Y;
			Scenery->PointD10Z = Scenery->PointD7Z;

			Scenery->PointE10X = Scenery->PointE7X;
			Scenery->PointE10Y = Scenery->PointE7Y;
			Scenery->PointE10Z = Scenery->PointE7Z;
		
		}
#if 1;//////////// TURN OFF TO REWRAP TUNNELS
		if (VRArrFlag4[i*4+4] == RIGHT_TUNNEL) { // Right tunnel
//				tunnelRight = true; // Set the flag
			Scenery->PointA4X = Scenery->PointA9X;
			Scenery->PointA4Y = Scenery->PointA9Y;
			Scenery->PointA4Z = Scenery->PointA9Z;

			Scenery->PointB4X = Scenery->PointB9X;
			Scenery->PointB4Y = Scenery->PointB9Y;
			Scenery->PointB4Z = Scenery->PointB9Z;

			Scenery->PointC4X = Scenery->PointC9X;
			Scenery->PointC4Y = Scenery->PointC9Y;
			Scenery->PointC4Z = Scenery->PointC9Z;

			Scenery->PointD4X = Scenery->PointD9X;
			Scenery->PointD4Y = Scenery->PointD9Y;
			Scenery->PointD4Z = Scenery->PointD9Z;

			Scenery->PointE4X = Scenery->PointE9X;
			Scenery->PointE4Y = Scenery->PointE9Y;
			Scenery->PointE4Z = Scenery->PointE9Z;
////
			Scenery->PointA5X = Scenery->PointA2X;
			Scenery->PointA5Y = Scenery->PointA2Y;
			Scenery->PointA5Z = Scenery->PointA2Z;

			Scenery->PointB5X = Scenery->PointB2X;
			Scenery->PointB5Y = Scenery->PointB2Y;
			Scenery->PointB5Z = Scenery->PointB2Z;

			Scenery->PointC5X = Scenery->PointC2X;
			Scenery->PointC5Y = Scenery->PointC2Y;
			Scenery->PointC5Z = Scenery->PointC2Z;

			Scenery->PointD5X = Scenery->PointD2X;
			Scenery->PointD5Y = Scenery->PointD2Y;
			Scenery->PointD5Z = Scenery->PointD2Z;

			Scenery->PointE5X = Scenery->PointE2X;
			Scenery->PointE5Y = Scenery->PointE2Y;
			Scenery->PointE5Z = Scenery->PointE2Z;
		
		}
#endif;
#endif; // Flag4

#if 1;
		if ((VRArrFlag4[i*4+1] == LANE_SPLIT)&&(i != 0)) {
//				cout << i << " " << endl;
		// Move A6, A0, A1 to the positions A0, A1, A2

			Scenery->PointA7X = Scenery->PointA6X;
			Scenery->PointA7Y = Scenery->PointA6Y;
			Scenery->PointA7Z = Scenery->PointA6Z;

			Scenery->PointA6X = Scenery->PointA0X;
			Scenery->PointA6Y = Scenery->PointA0Y;
			Scenery->PointA6Z = Scenery->PointA0Z;

			Scenery->PointA0X = Scenery->PointA1X;
			Scenery->PointA0Y = Scenery->PointA1Y;
			Scenery->PointA0Z = Scenery->PointA1Z;

		} 

		if ((VRArrFlag4[i*4+1] == LANE_REJOIN)&&(i != 0)) {
//				cout << i << " " << endl;
		// Move E6, E0, E1 to the positions E0, E1, E2
			Scenery->PointE7X = Scenery->PointE6X;
			Scenery->PointE7Y = Scenery->PointE6Y;
			Scenery->PointE7Z = Scenery->PointE6Z;

			Scenery->PointE6X = Scenery->PointE0X;
			Scenery->PointE6Y = Scenery->PointE0Y;
			Scenery->PointE6Z = Scenery->PointE0Z;

			Scenery->PointE0X = Scenery->PointE1X;
			Scenery->PointE0Y = Scenery->PointE1Y;
			Scenery->PointE0Z = Scenery->PointE1Z;
		} 


#endif;

// Saving data to Scenery.obj file:
fwrite("o ",2,1,outf); // Object type
fwrite("Scenery",7,1,outf);
fprintf(outf, "%d\n", Scenery->CurrentNum);

//	for (k=0; k<165; k++) {	// Write coords data
//		fprintf(outf, "v ");
//		fprintf(outf, "%f ", ArrayCoord[k+1]);
//		fprintf(outf, "%f ", ArrayCoord[k+1]);
//		fprintf(outf, "%f ", ArrayCoord[k+2]);
//		fwrite("\n",1,1,outf); // New line
//	}

// Scaling values:
#if 1;
	ArrayCoord[i] =   (float)Scenery->PointA0X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i]);
	ArrayCoord[i+1] = (float)Scenery->PointA0Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+1]);
	ArrayCoord[i+2] = -(float)Scenery->PointA0Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+2]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+3] = (float)Scenery->PointA1X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+3]);
	ArrayCoord[i+4] = (float)Scenery->PointA1Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+4]);
	ArrayCoord[i+5] = -(float)Scenery->PointA1Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+5]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+6] = (float)Scenery->PointA2X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+6]);
	ArrayCoord[i+7] = (float)Scenery->PointA2Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+7]);
	ArrayCoord[i+8] = -(float)Scenery->PointA2Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+8]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+9] = (float)Scenery->PointA3X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+9]);
	ArrayCoord[i+10] = (float)Scenery->PointA3Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+10]);
	ArrayCoord[i+11] = -(float)Scenery->PointA3Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+11]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+12] = (float)Scenery->PointA4X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+12]);
	ArrayCoord[i+13] = (float)Scenery->PointA4Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+13]);
	ArrayCoord[i+14] = -(float)Scenery->PointA4Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+14]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+15] = (float)Scenery->PointA5X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+15]);
	ArrayCoord[i+16] = (float)Scenery->PointA5Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+16]);
	ArrayCoord[i+17] = -(float)Scenery->PointA5Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+17]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+18] = (float)Scenery->PointA6X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+18]);
	ArrayCoord[i+19] = (float)Scenery->PointA6Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+19]);
	ArrayCoord[i+20] = -(float)Scenery->PointA6Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+20]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+21] = (float)Scenery->PointA7X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+21]);
	ArrayCoord[i+22] = (float)Scenery->PointA7Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+22]);
	ArrayCoord[i+23] = -(float)Scenery->PointA7Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+23]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+24] = (float)Scenery->PointA8X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+24]);
	ArrayCoord[i+25] = (float)Scenery->PointA8Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+25]);
	ArrayCoord[i+26] = -(float)Scenery->PointA8Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+26]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+27] = (float)Scenery->PointA9X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+27]);
	ArrayCoord[i+28] = (float)Scenery->PointA9Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+28]);
	ArrayCoord[i+29] = -(float)Scenery->PointA9Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+29]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+30] = (float)Scenery->PointA10X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+30]);
	ArrayCoord[i+31] = (float)Scenery->PointA10Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+31]);
	ArrayCoord[i+32] = -(float)Scenery->PointA10Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+32]);
		fwrite("\n",1,1,outf); // New line
#endif; // A off

#if 1;
		ArrayCoord[i+33] = (float)Scenery->PointB0X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+33]);
	ArrayCoord[i+34] = (float)Scenery->PointB0Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+34]);
	ArrayCoord[i+35] = -(float)Scenery->PointB0Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+35]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+36] = (float)Scenery->PointB1X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+36]);
	ArrayCoord[i+37] = (float)Scenery->PointB1Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+37]);
	ArrayCoord[i+38] = -(float)Scenery->PointB1Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+38]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+39] = (float)Scenery->PointB2X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+39]);
	ArrayCoord[i+40] = (float)Scenery->PointB2Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+40]);
	ArrayCoord[i+41] = -(float)Scenery->PointB2Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+41]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+42] = (float)Scenery->PointB3X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+42]);
	ArrayCoord[i+43] = (float)Scenery->PointB3Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+43]);
	ArrayCoord[i+44] = -(float)Scenery->PointB3Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+44]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+45] = (float)Scenery->PointB4X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+45]);
	ArrayCoord[i+46] = (float)Scenery->PointB4Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+46]);
	ArrayCoord[i+47] = -(float)Scenery->PointB4Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+47]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+48] = (float)Scenery->PointB5X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+48]);
	ArrayCoord[i+49] = (float)Scenery->PointB5Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+49]);
	ArrayCoord[i+50] = -(float)Scenery->PointB5Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+50]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+51] = (float)Scenery->PointB6X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+51]);
	ArrayCoord[i+52] = (float)Scenery->PointB6Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+52]);
	ArrayCoord[i+53] = -(float)Scenery->PointB6Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+53]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+54] = (float)Scenery->PointB7X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+54]);
	ArrayCoord[i+55] = (float)Scenery->PointB7Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+55]);
	ArrayCoord[i+56] = -(float)Scenery->PointB7Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+56]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+57] = (float)Scenery->PointB8X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+57]);
	ArrayCoord[i+58] = (float)Scenery->PointB8Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+58]);
	ArrayCoord[i+59] = -(float)Scenery->PointB8Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+59]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+60] = (float)Scenery->PointB9X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+60]);
	ArrayCoord[i+61] = (float)Scenery->PointB9Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+61]);
	ArrayCoord[i+62] = -(float)Scenery->PointB9Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+62]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+63] = (float)Scenery->PointB10X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+63]);
	ArrayCoord[i+64] = (float)Scenery->PointB10Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+64]);
	ArrayCoord[i+65] = -(float)Scenery->PointB10Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+65]);
		fwrite("\n",1,1,outf); // New line
#endif; // B off

#if 1;
//**********************
	ArrayCoord[i+66] = (float)Scenery->PointC0X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+66]);
	ArrayCoord[i+67] = (float)Scenery->PointC0Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+67]);
	ArrayCoord[i+68] = -(float)Scenery->PointC0Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+68]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+69] = (float)Scenery->PointC1X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+69]);
	ArrayCoord[i+70] = (float)Scenery->PointC1Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+70]);
	ArrayCoord[i+71] = -(float)Scenery->PointC1Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+71]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+72] = (float)Scenery->PointC2X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+72]);
	ArrayCoord[i+73] = (float)Scenery->PointC2Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+73]);
	ArrayCoord[i+74] = -(float)Scenery->PointC2Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+74]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+75] = (float)Scenery->PointC3X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+75]);
	ArrayCoord[i+76] = (float)Scenery->PointC3Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+76]);
	ArrayCoord[i+77] = -(float)Scenery->PointC3Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+77]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+78] = (float)Scenery->PointC4X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+78]);
	ArrayCoord[i+79] = (float)Scenery->PointC4Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+79]);
	ArrayCoord[i+80] = -(float)Scenery->PointC4Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+80]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+81] = (float)Scenery->PointC5X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+81]);
	ArrayCoord[i+82] = (float)Scenery->PointC5Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+82]);
	ArrayCoord[i+83] = -(float)Scenery->PointC5Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+83]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+84] = (float)Scenery->PointC6X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+84]);
	ArrayCoord[i+85] = (float)Scenery->PointC6Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+85]);
	ArrayCoord[i+86] = -(float)Scenery->PointC6Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+86]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+87] = (float)Scenery->PointC7X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+87]);
	ArrayCoord[i+88] = (float)Scenery->PointC7Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+88]);
	ArrayCoord[i+89] = -(float)Scenery->PointC7Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+89]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+90] = (float)Scenery->PointC8X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+90]);
	ArrayCoord[i+91] = (float)Scenery->PointC8Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+91]);
	ArrayCoord[i+92] = -(float)Scenery->PointC8Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+92]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+93] = (float)Scenery->PointC9X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+93]);
	ArrayCoord[i+94] = (float)Scenery->PointC9Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+94]);
	ArrayCoord[i+95] = -(float)Scenery->PointC9Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+95]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+96] = (float)Scenery->PointC10X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+96]);
	ArrayCoord[i+97] = (float)Scenery->PointC10Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+97]);
	ArrayCoord[i+98] = -(float)Scenery->PointC10Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+98]);
		fwrite("\n",1,1,outf); // New line
#endif; // C off

#if 1;
	ArrayCoord[i+99] = (float)Scenery->PointD0X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+99]);
	ArrayCoord[i+100] = (float)Scenery->PointD0Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+100]);
	ArrayCoord[i+101] = -(float)Scenery->PointD0Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+101]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+102] = (float)Scenery->PointD1X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+102]);
	ArrayCoord[i+103] = (float)Scenery->PointD1Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+103]);
	ArrayCoord[i+104] = -(float)Scenery->PointD1Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+104]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+105] = (float)Scenery->PointD2X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+105]);
	ArrayCoord[i+106] = (float)Scenery->PointD2Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+106]);
	ArrayCoord[i+107] = -(float)Scenery->PointD2Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+107]);
		fwrite("\n",1,1,outf); // New line


	ArrayCoord[i+108] = (float)Scenery->PointD3X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+108]);
	ArrayCoord[i+109] = (float)Scenery->PointD3Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+109]);
	ArrayCoord[i+110] = -(float)Scenery->PointD3Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+110]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+111] = (float)Scenery->PointD4X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+111]);
	ArrayCoord[i+112] = (float)Scenery->PointD4Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+112]);
	ArrayCoord[i+113] = -(float)Scenery->PointD4Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+113]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+114] = (float)Scenery->PointD5X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+114]);
	ArrayCoord[i+115] = (float)Scenery->PointD5Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+115]);
	ArrayCoord[i+116] = -(float)Scenery->PointD5Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+116]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+117] = (float)Scenery->PointD6X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+117]);
	ArrayCoord[i+118] = (float)Scenery->PointD6Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+118]);
	ArrayCoord[i+119] = -(float)Scenery->PointD6Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+119]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+120] = (float)Scenery->PointD7X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+120]);
	ArrayCoord[i+121] = (float)Scenery->PointD7Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+121]);
	ArrayCoord[i+122] = -(float)Scenery->PointD7Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+122]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+123] = (float)Scenery->PointD8X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+123]);
	ArrayCoord[i+124] = (float)Scenery->PointD8Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+124]);
	ArrayCoord[i+125] = -(float)Scenery->PointD8Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+125]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+126] = (float)Scenery->PointD9X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+126]);
	ArrayCoord[i+127] = (float)Scenery->PointD9Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+127]);
	ArrayCoord[i+128] = -(float)Scenery->PointD9Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+128]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+129] = (float)Scenery->PointD10X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+129]);
	ArrayCoord[i+130] = (float)Scenery->PointD10Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+130]);
	ArrayCoord[i+131] = -(float)Scenery->PointD10Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+131]);
		fwrite("\n",1,1,outf); // New line

#endif; // D off

#if 1;
//**********************
// If it's a first record and all values are zeros, read next records (A10...A5) and store them as E10...E5 for the first record.
//if ((Scenery->CurrentNum == 0)&&( Scenery->PointE0X ==0 ) &&( Scenery->PointE0Y ==0 )&&( Scenery->PointE0Z ==0 )&&( Scenery->PointE1X ==0 )&&(flag3DO==false)){
if ((Scenery->CurrentNum == 0)&&( Scenery->PointE0X ==0 ) &&( Scenery->PointE0Y ==0 )&&( Scenery->PointE0Z ==0 )&&( Scenery->PointE1X ==0 )){

#if 1; // Reading E coords
int offset=0;
			// Scenery record size is 554 bytes. Now we are at the beginning of the 2nd block.
			// Set to E line on the second TRKD block (+636 bytes)
			fseek(inputfile,0,SEEK_SET);		// Set to the begining
			offset = 0x1B000+0x22A+0x27C+0xDE;
			fseek(inputfile, offset, SEEK_CUR); // Setting to the starting position
//			cout << "\nStarting offset = 0x" << hex << offset << "\n";


			// Reading 132 bytes (11*12 bytes = 132) ("E" line, second block)
fread(&temp,4,1,inputfile); // e0x
	Scenery->PointE0X = temp;
//cout << "\nScenery->PointE0X = 0x" << hex << Scenery->PointE0X << "\n";
fread(&temp,4,1,inputfile); // e0z
	Scenery->PointE0Z = temp;
fread(&temp,4,1,inputfile); // e0y
	Scenery->PointE0Y = temp;

fread(&temp,4,1,inputfile); // e1x 
	Scenery->PointE1X = temp;
fread(&temp,4,1,inputfile); // e1z
	Scenery->PointE1Z = temp;
fread(&temp,4,1,inputfile); // e1y
	Scenery->PointE1Y = temp;

fread(&temp,4,1,inputfile); // e2x
	Scenery->PointE2X = temp;
fread(&temp,4,1,inputfile); // e2z
	Scenery->PointE2Z = temp;
fread(&temp,4,1,inputfile); // e2y
	Scenery->PointE2Y = temp;

fread(&temp,4,1,inputfile); // e3x 
	Scenery->PointE3X = temp;
//cout << "\nScenery->PointE3X = 0x" << hex << Scenery->PointE3X << "\n";
fread(&temp,4,1,inputfile); // e3z
	Scenery->PointE3Z = temp;
//cout << "\nScenery->PointE3Z = 0x" << hex << Scenery->PointE3Z << "\n";
fread(&temp,4,1,inputfile); // e3y
	Scenery->PointE3Y = temp;
//cout << "\nScenery->PointE3Y = 0x" << hex << Scenery->PointE3Y << "\n";

fread(&temp,4,1,inputfile); // e4x
	Scenery->PointE4X = temp;
//cout << "\nScenery->PointE4X = 0x" << hex << Scenery->PointE4X << "\n";
fread(&temp,4,1,inputfile); // e4z
	Scenery->PointE4Z = temp;
//cout << "\nScenery->PointE4Z = 0x" << hex << Scenery->PointE4Z << "\n";
fread(&temp,4,1,inputfile); // e4y
	Scenery->PointE4Y = temp;
//cout << "\nScenery->PointE4Y = 0x" << hex << Scenery->PointE4Y << "\n";

fread(&temp,4,1,inputfile); // e5x 
	Scenery->PointE5X = temp;
fread(&temp,4,1,inputfile); // e5z
	Scenery->PointE5Z = temp;
fread(&temp,4,1,inputfile); // e5y
	Scenery->PointE5Y = temp;

fread(&temp,4,1,inputfile); // e6x
	Scenery->PointE6X = temp;
fread(&temp,4,1,inputfile); // e6z
	Scenery->PointE6Z = temp;
fread(&temp,4,1,inputfile); // e6y
	Scenery->PointE6Y = temp;

fread(&temp,4,1,inputfile); // e7x
	Scenery->PointE7X = temp;
fread(&temp,4,1,inputfile); // e7z
	Scenery->PointE7Z = temp;
fread(&temp,4,1,inputfile); // e7y
	Scenery->PointE7Y = temp;

fread(&temp,4,1,inputfile); // e8x
	Scenery->PointE8X = temp;
fread(&temp,4,1,inputfile); // e8z
	Scenery->PointE8Z = temp;
fread(&temp,4,1,inputfile); // e8y
	Scenery->PointE8Y = temp;

fread(&temp,4,1,inputfile); // e9x 
	Scenery->PointE9X = temp;
fread(&temp,4,1,inputfile); // e9z
	Scenery->PointE9Z = temp;
fread(&temp,4,1,inputfile); // e9y
	Scenery->PointE9Y = temp;

fread(&temp,4,1,inputfile); // e10x
	Scenery->PointE10X = temp;
fread(&temp,4,1,inputfile); // e10z
	Scenery->PointE10Z = temp;
fread(&temp,4,1,inputfile); // e10y
	Scenery->PointE10Y = temp;

// Get back to the beginning of the 2nd block -768 bytes
fseek(inputfile,0,SEEK_SET);		// Set to the begining
fseek(inputfile,0x1B554,SEEK_CUR); // Setting to the starting position
#endif; // Reading E coords



			#if 0;// Double line D
			// Make double line "D"
			Scenery->PointE0X = Scenery->PointD0X;
			Scenery->PointE0Y = Scenery->PointD0Y;
			Scenery->PointE0Z = Scenery->PointD0Z;

			Scenery->PointE1X = Scenery->PointD1X;
			Scenery->PointE1Y = Scenery->PointD1Y;
			Scenery->PointE1Z = Scenery->PointD1Z;

			Scenery->PointE2X = Scenery->PointD2X;
			Scenery->PointE2Y = Scenery->PointD2Y;
			Scenery->PointE2Z = Scenery->PointD2Z;

			Scenery->PointE3X = Scenery->PointD3X;
			Scenery->PointE3Y = Scenery->PointD3Y;
			Scenery->PointE3Z = Scenery->PointD3Z;

			Scenery->PointE4X = Scenery->PointD4X;
			Scenery->PointE4Y = Scenery->PointD4Y;
			Scenery->PointE4Z = Scenery->PointD4Z;

			Scenery->PointE5X = Scenery->PointD5X;
			Scenery->PointE5Y = Scenery->PointD5Y;
			Scenery->PointE5Z = Scenery->PointD5Z;

			Scenery->PointE6X = Scenery->PointD6X;
			Scenery->PointE6Y = Scenery->PointD6Y;
			Scenery->PointE6Z = Scenery->PointD6Z;

			Scenery->PointE7X = Scenery->PointD7X;
			Scenery->PointE7Y = Scenery->PointD7Y;
			Scenery->PointE7Z = Scenery->PointD7Z;

			Scenery->PointE8X = Scenery->PointD8X;
			Scenery->PointE8Y = Scenery->PointD8Y;
			Scenery->PointE8Z = Scenery->PointD8Z;

			Scenery->PointE9X = Scenery->PointD9X;
			Scenery->PointE9Y = Scenery->PointD9Y;
			Scenery->PointE9Z = Scenery->PointD9Z;

			Scenery->PointE10X = Scenery->PointD10X;
			Scenery->PointE10Y = Scenery->PointD10Y;
			Scenery->PointE10Z = Scenery->PointD10Z;
			#endif; // Double line D
			// Writing values
		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE0X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE0Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE0Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE1X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE1Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE1Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE2X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE2Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE2Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE3X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE3Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE3Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE4X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE4Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE4Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE5X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE5Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE5Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE6X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE6Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE6Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE7X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE7Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE7Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE8X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE8Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE8Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ",(float)Scenery->PointE9X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE9Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE9Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		fprintf(outf, "v ");
		fprintf(outf, "%f ", (float)Scenery->PointE10X / SCALE);
		fprintf(outf, "%f ", (float)Scenery->PointE10Z / SCALE);
		fprintf(outf, "%f ", -(float)Scenery->PointE10Y / SCALE);
		fwrite("\n",1,1,outf); // New line

		} else {

	ArrayCoord[i+132] = (float)Scenery->PointE0X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+132]);
	ArrayCoord[i+133] = (float)Scenery->PointE0Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+133]);
	ArrayCoord[i+134] = -(float)Scenery->PointE0Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+134]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+135] = (float)Scenery->PointE1X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+135]);
	ArrayCoord[i+136] = (float)Scenery->PointE1Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+136]);
	ArrayCoord[i+137] = -(float)Scenery->PointE1Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+137]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+138] = (float)Scenery->PointE2X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+138]);
	ArrayCoord[i+139] = (float)Scenery->PointE2Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+139]);
	ArrayCoord[i+140] = -(float)Scenery->PointE2Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+140]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+141] = (float)Scenery->PointE3X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+141]);
	ArrayCoord[i+142] = (float)Scenery->PointE3Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+142]);
	ArrayCoord[i+143] = -(float)Scenery->PointE3Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+143]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+144] = (float)Scenery->PointE4X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+144]);
	ArrayCoord[i+145] = (float)Scenery->PointE4Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+145]);
	ArrayCoord[i+146] = -(float)Scenery->PointE4Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+146]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+147] = (float)Scenery->PointE5X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+147]);
	ArrayCoord[i+148] = (float)Scenery->PointE5Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+148]);
	ArrayCoord[i+149] = -(float)Scenery->PointE5Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+149]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+150] = (float)Scenery->PointE6X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+150]);
	ArrayCoord[i+151] = (float)Scenery->PointE6Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+151]);
	ArrayCoord[i+152] = -(float)Scenery->PointE6Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+152]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+153] = (float)Scenery->PointE7X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+153]);
	ArrayCoord[i+154] = (float)Scenery->PointE7Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+154]);
	ArrayCoord[i+155] = -(float)Scenery->PointE7Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+155]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+156] = (float)Scenery->PointE8X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+156]);
	ArrayCoord[i+157] = (float)Scenery->PointE8Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+157]);
	ArrayCoord[i+158] = -(float)Scenery->PointE8Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+158]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+159] = (float)Scenery->PointE9X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+159]);
	ArrayCoord[i+160] = (float)Scenery->PointE9Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+160]);
	ArrayCoord[i+161] = -(float)Scenery->PointE9Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+161]);
		fwrite("\n",1,1,outf); // New line

	ArrayCoord[i+162] = (float)Scenery->PointE10X / SCALE;
		fprintf(outf, "v ");
		fprintf(outf, "%f ", ArrayCoord[i+162]);
	ArrayCoord[i+163] = (float)Scenery->PointE10Z / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+163]);
	ArrayCoord[i+164] = -(float)Scenery->PointE10Y / SCALE;
		fprintf(outf, "%f ", ArrayCoord[i+164]);
		fwrite("\n",1,1,outf); // New line
		}

fwrite("\n",1,1,outf); // New line
#endif; // E off


	// Assign texture to polygon
	// Material name

#if 0; // prepare polygons
	// Prepare point arrays
	float ***ArrScenPoints;
		ArrScenPoints = new float**[Scenery->CurrentNum+1]; 
		for (int s = 0; s < (Scenery->CurrentNum+1); ++s) {
		  ArrScenPoints[s] = new float*[10];

		  for (int f = 0; f < 10; ++f)
		  ArrScenPoints[s][f] = new float[3];
		}

#endif;// prepare polygons

#if 0; // TexAssign
			TexAssign(Scenery->Texture1, 
			Scenery->PointA0X, Scenery->PointA0Y, Scenery->PointA0Z, Scenery->PointA1X, Scenery->PointA1Y, Scenery->PointA1Z, Scenery->PointB1X, Scenery->PointB1Y, Scenery->PointB1Z, 
			Scenery->PointC1X, Scenery->PointC1Y, Scenery->PointC1Z, Scenery->PointD1X, Scenery->PointD1Y, Scenery->PointD1Z, Scenery->PointE1X, Scenery->PointE1Y, Scenery->PointE1Z,
			Scenery->PointE0X, Scenery->PointE0Y, Scenery->PointE0Z, Scenery->PointD0X, Scenery->PointD0Y, Scenery->PointD0Z, Scenery->PointC0X, Scenery->PointC0Y, Scenery->PointC0Z, 
			Scenery->PointB0X, Scenery->PointB0Y, Scenery->PointB0Z);

			TexAssign(Scenery->Texture2, 
			Scenery->PointA1X, Scenery->PointA1Y, Scenery->PointA1Z, Scenery->PointA2X, Scenery->PointA2Y, Scenery->PointA2Z, Scenery->PointB2X, Scenery->PointB2Y, Scenery->PointB2Z, 
			Scenery->PointC2X, Scenery->PointC2Y, Scenery->PointC2Z, Scenery->PointD2X, Scenery->PointD2Y, Scenery->PointD2Z, Scenery->PointE2X, Scenery->PointE2Y, Scenery->PointE2Z,
			Scenery->PointE1X, Scenery->PointE1Y, Scenery->PointE1Z, Scenery->PointD1X, Scenery->PointD1Y, Scenery->PointD1Z, Scenery->PointC1X, Scenery->PointC1Y, Scenery->PointC1Z, 
			Scenery->PointB1X, Scenery->PointB1Y, Scenery->PointB1Z);

			TexAssign(Scenery->Texture3, 
			Scenery->PointA2X, Scenery->PointA2Y, Scenery->PointA2Z, Scenery->PointA3X, Scenery->PointA3Y, Scenery->PointA3Z, Scenery->PointB3X, Scenery->PointB3Y, Scenery->PointB3Z, 
			Scenery->PointC3X, Scenery->PointC3Y, Scenery->PointC3Z, Scenery->PointD3X, Scenery->PointD3Y, Scenery->PointD3Z, Scenery->PointE3X, Scenery->PointE3Y, Scenery->PointE3Z,
			Scenery->PointE2X, Scenery->PointE2Y, Scenery->PointE2Z, Scenery->PointD2X, Scenery->PointD2Y, Scenery->PointD2Z, Scenery->PointC2X, Scenery->PointC2Y, Scenery->PointC2Z, 
			Scenery->PointB2X, Scenery->PointB2Y, Scenery->PointB2Z);

			TexAssign(Scenery->Texture4, 
			Scenery->PointA3X, Scenery->PointA3Y, Scenery->PointA3Z, Scenery->PointA4X, Scenery->PointA4Y, Scenery->PointA4Z, Scenery->PointB4X, Scenery->PointB4Y, Scenery->PointB4Z, 
			Scenery->PointC4X, Scenery->PointC4Y, Scenery->PointC4Z, Scenery->PointD4X, Scenery->PointD4Y, Scenery->PointD4Z, Scenery->PointE4X, Scenery->PointE4Y, Scenery->PointE4Z,
			Scenery->PointE3X, Scenery->PointE3Y, Scenery->PointE3Z, Scenery->PointD3X, Scenery->PointD3Y, Scenery->PointD3Z, Scenery->PointC3X, Scenery->PointC3Y, Scenery->PointC3Z, 
			Scenery->PointB3X, Scenery->PointB3Y, Scenery->PointB3Z);

			TexAssign(Scenery->Texture5, 
			Scenery->PointA4X, Scenery->PointA4Y, Scenery->PointA4Z, Scenery->PointA5X, Scenery->PointA5Y, Scenery->PointA5Z, Scenery->PointB5X, Scenery->PointB5Y, Scenery->PointB5Z, 
			Scenery->PointC5X, Scenery->PointC5Y, Scenery->PointC5Z, Scenery->PointD5X, Scenery->PointD5Y, Scenery->PointD5Z, Scenery->PointE5X, Scenery->PointE5Y, Scenery->PointE5Z,
			Scenery->PointE4X, Scenery->PointE4Y, Scenery->PointE4Z, Scenery->PointD4X, Scenery->PointD4Y, Scenery->PointD4Z, Scenery->PointC4X, Scenery->PointC4Y, Scenery->PointC4Z, 
			Scenery->PointB4X, Scenery->PointB4Y, Scenery->PointB4Z);

			TexAssign(Scenery->Texture6, 
			Scenery->PointA6X, Scenery->PointA6Y, Scenery->PointA6Z, Scenery->PointA0X, Scenery->PointA0Y, Scenery->PointA0Z, Scenery->PointB0X, Scenery->PointB0Y, Scenery->PointB0Z, 
			Scenery->PointC0X, Scenery->PointC0Y, Scenery->PointC0Z, Scenery->PointD0X, Scenery->PointD0Y, Scenery->PointD0Z, Scenery->PointE0X, Scenery->PointE0Y, Scenery->PointE0Z,
			Scenery->PointE6X, Scenery->PointE6Y, Scenery->PointE6Z, Scenery->PointD6X, Scenery->PointD6Y, Scenery->PointD6Z, Scenery->PointC6X, Scenery->PointC6Y, Scenery->PointC6Z, 
			Scenery->PointB6X, Scenery->PointB6Y, Scenery->PointB6Z);

			TexAssign(Scenery->Texture7, 
			Scenery->PointA7X, Scenery->PointA7Y, Scenery->PointA7Z, Scenery->PointA6X, Scenery->PointA6Y, Scenery->PointA6Z, Scenery->PointB6X, Scenery->PointB6Y, Scenery->PointB6Z, 
			Scenery->PointC6X, Scenery->PointC6Y, Scenery->PointC6Z, Scenery->PointD6X, Scenery->PointD6Y, Scenery->PointD6Z, Scenery->PointE6X, Scenery->PointE6Y, Scenery->PointE6Z,
			Scenery->PointE7X, Scenery->PointE7Y, Scenery->PointE7Z, Scenery->PointD7X, Scenery->PointD7Y, Scenery->PointD7Z, Scenery->PointC7X, Scenery->PointC7Y, Scenery->PointC7Z, 
			Scenery->PointB7X, Scenery->PointB7Y, Scenery->PointB7Z);

			TexAssign(Scenery->Texture8, 
			Scenery->PointA8X, Scenery->PointA8Y, Scenery->PointA8Z, Scenery->PointA7X, Scenery->PointA7Y, Scenery->PointA7Z, Scenery->PointB7X, Scenery->PointB7Y, Scenery->PointB7Z, 
			Scenery->PointC7X, Scenery->PointC7Y, Scenery->PointC7Z, Scenery->PointD7X, Scenery->PointD7Y, Scenery->PointD7Z, Scenery->PointE7X, Scenery->PointE7Y, Scenery->PointE7Z,
			Scenery->PointE8X, Scenery->PointE8Y, Scenery->PointE8Z, Scenery->PointD8X, Scenery->PointD8Y, Scenery->PointD8Z, Scenery->PointC8X, Scenery->PointC8Y, Scenery->PointC8Z, 
			Scenery->PointB8X, Scenery->PointB8Y, Scenery->PointB8Z);

			TexAssign(Scenery->Texture9, 
			Scenery->PointA9X, Scenery->PointA9Y, Scenery->PointA9Z, Scenery->PointA8X, Scenery->PointA8Y, Scenery->PointA8Z, Scenery->PointB8X, Scenery->PointB8Y, Scenery->PointB8Z, 
			Scenery->PointC8X, Scenery->PointC8Y, Scenery->PointC8Z, Scenery->PointD8X, Scenery->PointD8Y, Scenery->PointD8Z, Scenery->PointE8X, Scenery->PointE8Y, Scenery->PointE8Z,
			Scenery->PointE9X, Scenery->PointE9Y, Scenery->PointE9Z, Scenery->PointD9X, Scenery->PointD9Y, Scenery->PointD9Z, Scenery->PointC9X, Scenery->PointC9Y, Scenery->PointC9Z, 
			Scenery->PointB9X, Scenery->PointB9Y, Scenery->PointB9Z);

			TexAssign(Scenery->Texture10, 
			Scenery->PointA10X, Scenery->PointA10Y, Scenery->PointA10Z, Scenery->PointA9X, Scenery->PointA9Y, Scenery->PointA9Z, Scenery->PointB9X, Scenery->PointB9Y, Scenery->PointB9Z, 
			Scenery->PointC9X, Scenery->PointC9Y, Scenery->PointC9Z, Scenery->PointD9X, Scenery->PointD9Y, Scenery->PointD9Z, Scenery->PointE9X, Scenery->PointE9Y, Scenery->PointE9Z,
			Scenery->PointE10X, Scenery->PointE10Y, Scenery->PointE10Z, Scenery->PointD10X, Scenery->PointD10Y, Scenery->PointD10Z, Scenery->PointC10X, Scenery->PointC10Y, Scenery->PointC10Z, 
			Scenery->PointB10X, Scenery->PointB10Y, Scenery->PointB10Z);

#endif;// TexAssign

#if 1;
// Add materials to mtl file
TexAssign(Scenery->Texture1);
TexAssign(Scenery->Texture2);
TexAssign(Scenery->Texture3);
TexAssign(Scenery->Texture4);
TexAssign(Scenery->Texture5);
TexAssign(Scenery->Texture6);
TexAssign(Scenery->Texture7);
TexAssign(Scenery->Texture8);
TexAssign(Scenery->Texture9);
TexAssign(Scenery->Texture10);
#endif;

// Write lines connections
// 1-2, 2-3, 3-4, 4-5, 5-6, 1-7, 7-8, 8-9, 9-10, 10-11
// 12-13, 13-14, 14-15, 15-16, 16-17, 12-18, 18-19, 19-20, 20-21, 21-22 (i.e. +11)
#if 1;
int d;
bool flag = false;

for (d=1; d<=5; d++) {		// Main lines loop. Writing all lines for A...E points.
//	if (flag == true) {
		connector = NextL;	// Add counter for line connectors
//	}
	for (f=1; f<=5; f++) {	// Write lines 1-2, 2-3, 3-4, 4-5, 5-6
		fwrite("l ",2,1,outf);
		fprintf(outf, "%d ", NextL);
		fprintf(outf, "%d ", (NextL+1));
		NextL++;
		fwrite("\n",1,1,outf);// New line
		}

		fwrite("l ",2,1,outf);			// Write line 1-7
		fprintf(outf, "%d ", NextL-5);
		fprintf(outf, "%d ", NextL+1);
		fwrite("\n",1,1,outf);// New line
		NextL++;

	for (f=1; f<5; f++) {	// Write lines 7-8, 8-9, 9-10, 10-11
		fwrite("l ",2,1,outf);
		fprintf(outf, "%d ", NextL);
		fprintf(outf, "%d ", (NextL+1));
		NextL++;
		fwrite("\n",1,1,outf);// New line
		}
		fwrite("\n",1,1,outf);// New line


fwrite("\n",1,1,outf);// New line

#if 1; // "letter" lines connections
//if (d==1) {
//	goto skip;
//	} // Skip adding "letter" lines connections

if (flag == true) goto skip; //Lines added already
// Connect all "letter" lines. Line A10-A5 to line B10-B5 and so on.
for (s=1; s<=4; s++) {
	// 11-22, 10-21, 9-20, 8-19, 7-18, 1-12, 2-13, 3-14, 4-15, 5-16, 6-17
	// Starting from second "letter" line. Repeat 4 times.
	fwrite("# 'Letter lines' connections\n",29,1,outf);
	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+10));// 11-22
	fprintf(outf, "%d ", (connector+21));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+9));// 10-21
	fprintf(outf, "%d ", (connector+20));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+8)); // 9-20
	fprintf(outf, "%d ", (connector+19));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+7)); // 8-19
	fprintf(outf, "%d ", (connector+18));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+6)); // 7-18
	fprintf(outf, "%d ", (connector+17));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector)); // 1-12
	fprintf(outf, "%d ", (connector+11));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+1)); // 2-13
	fprintf(outf, "%d ", (connector+12));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+2)); // 3-14
	fprintf(outf, "%d ", (connector+13));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+3)); // 4-15
	fprintf(outf, "%d ", (connector+14));
	fwrite("\n",1,1,outf);// New line
	
	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+4)); // 5-16
	fprintf(outf, "%d ", (connector+15));
	fwrite("\n",1,1,outf);// New line

	fwrite("l ",2,1,outf);
	fprintf(outf, "%d ", (connector+5)); // 6-17
	fprintf(outf, "%d ", (connector+16));

	fwrite("\n",1,1,outf);// New line
	fwrite("\n",1,1,outf);// New line

	// All lines are drawn.
	flag = true;
	connector += 11;
	}
#endif; // "letter lines"

skip:
NextL++; //Next scenery record
}

#endif; // Lines records 

// Draw polygons sorted by materials. There are 10 materials, 4 polygons in each.
// Repeat loop 10 times, draw 4 polygons with "vt" params.
#if 0; // DRAW POLYGONS BY MATERIALS
for (z; z<=Scenery->CurrentNum; z+=11) {
fwrite("\n",1,1,outf);// New line
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+1));  // Line A0-A1 ... E0-E1.
	fprintf(outf, "%d ", (z+2));
	fprintf(outf, "%d ", (z+13)); 
	fprintf(outf, "%d ", (z+12));
	fwrite("\n",1,1,outf);// New line

	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+12));  // Line A0-A1 ... E0-E1.
	fprintf(outf, "%d ", (z+13));
	fprintf(outf, "%d ", (z+24)); 
	fprintf(outf, "%d ", (z+23));
	fwrite("\n",1,1,outf);// New line

	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+23));  // Line A0-A1 ... E0-E1.
	fprintf(outf, "%d ", (z+24));
	fprintf(outf, "%d ", (z+35)); 
	fprintf(outf, "%d ", (z+34));
	fwrite("\n",1,1,outf);// New line

	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+34));  // Line A0-A1 ... E0-E1.
	fprintf(outf, "%d ", (z+35));
	fprintf(outf, "%d ", (z+46)); 
	fprintf(outf, "%d ", (z+45));
	fwrite("\n",1,1,outf);// New line
	#endif;
//z += 11; 
	}
#endif; // Polygons draw by materials

// If 3DO flag was set, jump to next TRKD.
	if (flag3DO == true) {
		int trkdCheck = 0;
		short int value;
		int value2;
		// Seek next TRKD and step back to 4 bytes.
		// Read file while TRKD is found
		while (!feof(inputfile)) {  
		fread(&temp,4,1,inputfile); 
		trkdCheck = temp;
			if (trkdCheck == 0x444B5254) { // if TRKD was found
//				cout << "trkdCheck = 0x" << hex << trkdCheck << " next i = " << dec << i+1 << endl;
				fgetpos(inputfile, &pos);
//				pos = ftell(inputfile);
//				cout << "pos = " << pos << endl;
//		fread(&value2,4,1,inputfile);
//			cout << "value = 0x" << hex << value2 << endl;
//		fread(&value2,4,1,inputfile);
//			cout << "value = 0x" << hex << value2 << endl;
//		fread(&value2,4,1,inputfile);
//			cout << "value = 0x" << hex << value2 << endl;
//		fread(&value2,4,1,inputfile);
//			cout << "value = 0x" << hex << value2 << endl;

				fseek(inputfile, pos-4, SEEK_SET);	// step back for 4 bytes
				//continue;
				goto cont3DO2;
			}
		  }
		}

cont3DO2:
i++;
} // End of file (END OF SCENERY reading loop )

////////////////**********************FENCES********************///////////////////
#if 1; // Fences material file
FILE *mtlfile2;
int count2, c2;

mtlfile2 = fopen("Fence.mtl","a");
fwrite("# Fence mtl data\n",17,1,mtlfile2);

// New array size
size_t size2 = sizeof(arrFenceTex) / sizeof(arrFenceTex[0]); 
sort(arrFenceTex, arrFenceTex + size2);
//count = distance(arrTextures, unique(arrTextures, arrTextures + size)); // unique values quantity.
count2 = distance(arrFenceTex, unique(arrFenceTex, arrFenceTex + size2)); // unique values quantity.

//for (size_t f = 0; f < size-14300; ++f) {
  for (size_t f2 = 0; f2 < size2; ++f2) { // Show sorted list
//    cout << arrFenceTex[f] << ' ';
  }
//cout << endl << "Unique fence numbers = " << count << endl;
c2 = count2;


for (c2=0; c2<count2; c2++) {
// Add all unique array elements to the materials file.
		if (c2 == 0) continue;
		fprintf(mtlfile2, "newmtl %d\n", arrFenceTex[c2]);
//		fprintf(mtlfile2, "map_Kd C:\\Users\\Versus\\Desktop\\Tex%d.bmp\n", arrFenceTex[c]);

		if (flag3DO == true) {	
			fprintf(mtlfile2, "map_Kd ThreeDOSceneryTextures\\Tex%d.bmp\n", arrFenceTex[c2]);
			Extract3DOSCNTexture(arrFenceTex[c2], FamFileName3DO, 1);
			} else {


		fprintf(mtlfile2, "map_Kd SceneryTextures\\ScnTex%d.bmp\n", arrFenceTex[c2]);
		ExtractScenTexture(arrFenceTex[c2], infilename);
		}
}
fclose(mtlfile2);
#endif; // Fences material file
////////////////**********************FENCES********************///////////////////




// Add connections between first and the second record (because of zero values in points E10...E5 in original file)
	fwrite("# 'Letter lines' connections 0 to 1\n",36,1,outf);
	fwrite("l 55 66",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 54 65",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 53 64",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 52 63",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 51 62",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 45 56",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 46 57",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 47 58",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 48 59",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 49 60",7,1,outf);
	fwrite("\n",1,1,outf);// New line
	fwrite("l 50 61",7,1,outf);

	fwrite("\n",1,1,outf);// New line


// Write polygon data. There is 10 polygons in a row and 5 rows in scenery block. A-B row, B-C row, C-D row, D-E row.
// A-B row 11//1 10//1 21//1 22//1.
#if 0; // DRAW POLYGONS BY ROWS
	for (int v=0; v < i*5-1; v++) {
fwrite("\n",1,1,outf);// New line
#if 1;
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+11));  // Line A10-A9 ... B10-B9 and so on.
	fprintf(outf, "%d ", (z+10));
	fprintf(outf, "%d ", (z+21));
	fprintf(outf, "%d ", (z+22));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+10));  // Line A9-A8 ... B9-B8 and so on.
	fprintf(outf, "%d ", (z+9));
	fprintf(outf, "%d ", (z+20)); 
	fprintf(outf, "%d ", (z+21));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+9));  // Line A8-A7 ... B8-B7 and so on.
	fprintf(outf, "%d ", (z+8));
	fprintf(outf, "%d ", (z+19)); 
	fprintf(outf, "%d ", (z+20));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+8));  // Line A7-A6 ... B7-B6 and so on.
	fprintf(outf, "%d ", (z+7));
	fprintf(outf, "%d ", (z+18)); 
	fprintf(outf, "%d ", (z+19));
	fwrite("\n",1,1,outf);// New line
	#endif;

#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+7));  // Line A6-A0 ... B6-B0 and so on. 
	fprintf(outf, "%d ", (z+1));
	fprintf(outf, "%d ", (z+12)); 
	fprintf(outf, "%d ", (z+18));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+1));  // Line A0-A1 ... B0-B1 and so on.
	fprintf(outf, "%d ", (z+2));
	fprintf(outf, "%d ", (z+13)); 
	fprintf(outf, "%d ", (z+12));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+2));  // Line A1-A2 ... B1-B2 and so on.
	fprintf(outf, "%d ", (z+3));
	fprintf(outf, "%d ", (z+14)); 
	fprintf(outf, "%d ", (z+13));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+3));  // Line A2-A3 ... B2-B3 and so on.
	fprintf(outf, "%d ", (z+4));
	fprintf(outf, "%d ", (z+15)); 
	fprintf(outf, "%d ", (z+14));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+4));  // Line A3-A4 ... B3-B4 and so on.
	fprintf(outf, "%d ", (z+5));
	fprintf(outf, "%d ", (z+16)); 
	fprintf(outf, "%d ", (z+15));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1; 
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+5));  // Line A4-A5 ... B4-B5 and so on.
	fprintf(outf, "%d ", (z+6));
	fprintf(outf, "%d ", (z+17)); 
	fprintf(outf, "%d ", (z+16));
	fwrite("\n",1,1,outf);// New line
	#endif;

z += 11; 
	}
#endif; // DRAW POLYGONS BY ROWS



#if 1; // DRAW POLYGONS BY COLUMNS

int matCounter = 0;
	for (int v=0; v < i; v++) {
fwrite("\n",1,1,outf);// New line
#if 1; //// Line A0-A1...E1-E0.

#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex1[v]);
		} else {
#endif;

fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex1[v]);
	}
#endif; // vt

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+1));  // Line A0-A1...E1-E0.
			fprintf(outf, "/%d ", matCounter+1);  // vt index
//			fprintf(outf, "/1 ");  // vt index
	fprintf(outf, "%d", (z+2));
			fprintf(outf, "/%d ", matCounter+2);  // vt index
//			fprintf(outf, "/2 ");  // vt index
	fprintf(outf, "%d", (z+13)); 
			fprintf(outf, "/%d ", matCounter+3);  // vt index
//			fprintf(outf, "/3 ");  // vt index
	fprintf(outf, "%d", (z+12));
			fprintf(outf, "/%d ", matCounter+4);  // vt index
//			fprintf(outf, "/4 ");  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+12));  // Line A0-A1...E1-E0.
			fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+13));
			fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+24)); 
			fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+23));
			fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+23));  // Line A0-A1...E1-E0.
			fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+24));
			fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+35)); 
			fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+34));
			fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+34));  // Line A0-A1...E1-E0.
			fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+35));
			fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+46)); 
			fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+45));
			fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;
#endif;//// Line A0-A1...E1-E0.

#if 1; //// Line А1-A2...E2-E1.
	matCounter = 10;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex2[v]);
		} else {
#endif;

fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex2[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+2));  
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+3));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+14)); 
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+13));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+13));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+14));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+25));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+24));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+24));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+25));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+36));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+35));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+35));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+36));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+47)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+46));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А1-A2...E2-E1. 

#if 1; //// Line А2-A3...E3-E2. 
		matCounter = 20;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex3[v]);
		} else {
#endif;

fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex3[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+3));  
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+4));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+15));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+14));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+14));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+15));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+26)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+25));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+25)); 
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+26));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+37)); 
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+36));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+36));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+37));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+48)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+47));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А2-A3...E3-E2. 

#if 1; //// Line А3-A4...E4-E3. 
		matCounter = 30;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
		// If it's a tunnel on the right, skip Polygon #4.
		if (tunnelRight == true) goto tunnelskip;
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex4[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex4[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+4));
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+5));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+16));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+15));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+15)); 
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+16));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+27)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+26));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+26)); 
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+27));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+38));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+37));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+37));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+38));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+49)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+48));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А2-A3...E3-E2. 
tunnelskip:

#if 1; //// Line А4-A5...E5-E4.
		matCounter = 40;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex5[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex5[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+5)); 
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+6));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+17));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+16));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+16)); 
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+17));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+28)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+27));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+27)); 
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+28));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+39));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+38));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+38)); 
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+39));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+50)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+49));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А4-A5...E5-E4. 
////////////////**********************************************
#if 1; //// Line А6-A0...E0-E6.
		matCounter = 50;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex6[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex6[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+7));
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+1));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+12));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+18));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+18));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+12));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+23)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+29));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+29)); 
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+23));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+34)); 
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+40));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+40));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+34));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+45)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+51));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;
#endif;//// Line А6-A0...E0-E6.

#if 1; //// Line А7-A6...E6-E7.
		matCounter = 60;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex7[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex7[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+8)); 
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+7));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+18));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+19));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+19));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+18));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+29)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+30));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+30)); 
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+29));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+40));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+41));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+41));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+40));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+51));
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+52));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif;  //// Line А7-A6...E6-E7. 

#if 1; //// Line А8-A7...E7-E8. 
		matCounter = 70;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex8[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex8[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+9));
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+8));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+19)); 
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+20));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+20));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+19));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+30)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+31));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+31));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+30));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+41));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+42));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+42));  
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+41));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+52)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+53));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А8-A7...E7-E8. 

#if 1; //// Line А9-A8...E8-E9.
		matCounter = 80;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
			// If it's a tunnel on the left, skip Polygon #9.
		if (tunnelLeft == true) goto tunnelskip2;
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex9[v]);
		} else {
#endif;


fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex9[v]);
	}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+10));
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+9));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+20));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+21));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+21)); 
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+20));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+31)); 
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+32));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+32));  
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+31));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+42)); 
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+43));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+43)); 
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+42));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+53));
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+54));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif;  //// Line А9-A8...E8-E9.  
tunnelskip2:
#if 1; //// Line А10-A9...E10-E9.
		matCounter = 90;
#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
#if 1;
		if (flag3DO==true)	{
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex10[v]);
		} else {
#endif;

fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrTex10[v]);
		}
#endif; // vt
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+11)); 
		fprintf(outf, "/%d ", matCounter+1);  // vt index
	fprintf(outf, "%d", (z+10));
		fprintf(outf, "/%d ", matCounter+2);  // vt index
	fprintf(outf, "%d", (z+21));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+22));
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+22)); 
		fprintf(outf, "/%d ", matCounter+4);  // vt index
	fprintf(outf, "%d", (z+21));
		fprintf(outf, "/%d ", matCounter+3);  // vt index
	fprintf(outf, "%d", (z+32));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+33));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+33));
		fprintf(outf, "/%d ", matCounter+6);  // vt index
	fprintf(outf, "%d", (z+32));
		fprintf(outf, "/%d ", matCounter+5);  // vt index
	fprintf(outf, "%d", (z+43)); 
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+44));
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d", (z+44)); 
		fprintf(outf, "/%d ", matCounter+8);  // vt index
	fprintf(outf, "%d", (z+43));
		fprintf(outf, "/%d ", matCounter+7);  // vt index
	fprintf(outf, "%d", (z+54)); 
		fprintf(outf, "/%d ", matCounter+9);  // vt index
	fprintf(outf, "%d", (z+55));
		fprintf(outf, "/%d ", matCounter+10);  // vt index
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А10-A9...E10-E9. 

matCounter = 100;
z += 55; 
//#endif;
	}
// Draw polygons between D and E lines, FIRST block
#if 0; 
	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+44));  
	fprintf(outf, "%d ", (z+43));
	fprintf(outf, "%d ", (z+54)); 
	fprintf(outf, "%d ", (z+55));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+43));  
	fprintf(outf, "%d ", (z+42));
	fprintf(outf, "%d ", (z+53)); 
	fprintf(outf, "%d ", (z+54));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+42));  
	fprintf(outf, "%d ", (z+41));
	fprintf(outf, "%d ", (z+52)); 
	fprintf(outf, "%d ", (z+53));
	fwrite("\n",1,1,outf);// New line
	#endif;

	#if 1;
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d ", (z+41));  
	fprintf(outf, "%d ", (z+40));
	fprintf(outf, "%d ", (z+51)); 
	fprintf(outf, "%d ", (z+52));
	fwrite("\n",1,1,outf);// New line
	#endif; 
#endif; //// Line А10-A9...E10-E9. 
tunnelLeft = false;
tunnelRight = false;
#endif; // Polygons draw 2


///// Make MATERIALS file
#if 1;	// materials
FILE *mtlfile;
int count, c;

#if 0; // len
size_t len;

// Sort array arrTextures.
//cout << "\n"<< iC <<"\n";
print(arrTextures);
len = unique(arrTextures);
cout << "\n"<< len <<"\n";
print(arrTextures, len);
#endif; //len


#if 1; // Previous sort
// New array size
size_t size = sizeof(arrTextures) / sizeof(arrTextures[0]); 
sort(arrTextures, arrTextures + size);
//count = distance(arrTextures, unique(arrTextures, arrTextures + size)); // unique values quantity.
count = distance(arrTextures, unique(arrTextures, arrTextures + size)); // unique values quantity.

//  for (size_t f = 0; f < size-14300; ++f) {
  for (size_t f = 0; f < size; ++f) { // Show sorted list
//    cout << arrTextures[f] << ' ';
  }
//cout << endl << "Unique scenery numbers = " << count << endl;
c = count;

for (c=0; c<count; c++) {
// Add all unique array elements to the materials file.
		if (c == 0) continue;
		mtlfile = fopen("Scenery.mtl","a");
		fprintf(mtlfile, "newmtl %d\n", arrTextures[c]);
//		fprintf(mtlfile, "map_Kd C:\\Users\\Versus\\Desktop\\Tex%d.bmp\n", arrTextures[c]);

		if (flag3DO == true) {	
			fprintf(mtlfile, "map_Kd ThreeDOSceneryTextures\\Tex%d.bmp\n", arrTextures[c]);
			Extract3DOSCNTexture(arrTextures[c], FamFileName3DO, 1);
			} else {

		fprintf(mtlfile, "map_Kd SceneryTextures\\ScnTex%d.bmp\n", arrTextures[c]);
		// Scenery textures 
		ExtractScenTexture(arrTextures[c], infilename);
		}
		fclose(mtlfile);
}
#endif; // Previous sort

#endif; // materials

fclose(outf);
//cout << "File Scenery.obj successfuly saved\n";
//cout << "File Scenery.mtl successfuly saved\n";
fclose(outf4);
//cout << "File Fence.obj successfuly saved\n";
//cout << "File Fence.mtl successfuly saved\n";
#endif;

Extract3DOTextures(FamFileName3DO); //Just extract all 3DO textures
ExtractTRKDs3DO(infilename); // Just extract TRKD backgrounds for future use with ExtractReversedTrack(infilename);

	for (int d=0; d<=numVirtRdIndexes; d++) {// For test
//		cout << VRArrFlag4[d] << " ";
		}

}



//---------------------------------------------------
void ParseFences(FILE* inputfile)
{
//
unsigned int temp;
FILE *outf, *mtlfile;
char *outfilename;
int linesCounter = 1; 
int i, j, startOffset, version, verBuffer;
int p = 0;
unsigned short int temp2;
int connector;
bool hasFence = false;
//int NextL = 1;
double ArrayCoord[100000] = {0};
int matCounter = 0;
bool flag3DO=false;

fseek(inputfile, 0, SEEK_END);  
FileSize = ftell(inputfile);
//cout << "\n\n-=Reading Fences data=-\n";

fseek(inputfile,0,SEEK_SET);
// Reading Scenery records
// Records are starting at 0x1B000, finishing at eof.
// For the SE edition of NFS records starts at 0x1A4A0
#if 1;
version = fread(&version,sizeof(char),1,inputfile);
	if (version == 0x01) { // NFS original version
			startOffset = 0x1B000;
//			cout << "\nOriginal NFS version\n";
		} else {
			startOffset = 0x1A4A0;
//			cout << "\nSE NFS version\n";
		}
#endif;
#if 0;
fread(&verBuffer,4,1,inputfile);
version = verBuffer;

	if (version == 0x10) { // NFS original version
			startOffset = 0x1B000;
//			Length = 1000;
			cout << "Original NFS fences\n";
		} 
	if (version == 0x11) {
			startOffset = 0x1A4A0;
			cout << "SE NFS fences\n";
	}
	if (version == 0xE000000) {
			startOffset = 0x1B000;
			flag3DO = true;
			cout << "3DO NFS fences\n";
		}
#endif;
fseek(inputfile,0,SEEK_SET);		// Set to the begining

fseek(inputfile,startOffset,SEEK_CUR); // Setting to the starting position
outfilename = "Fence.obj"; // File for the Scenery
outf = fopen(outfilename,"a");
fwrite("# Fence data\n",13,1,outf);

i = 0;
//connector = 55; // Starting number
while (!feof(inputfile)) {
//while (i<2) {
//for (i=1; i<=50; i++) {
i++;
fread(&temp,4,1,inputfile); // Skip 4 bytes
Scenery->PointA0X = temp; // TRKD check. This value will be overwritten later. 
	if (temp != 0x444B5254) {
		break;
	}
//Debug******** cout << "\nTRKD = 0x" << hex << Scenery->PointA0X << "\n";
//Debug******** fprintf(outf, "TRKD %d \n", Scenery->PointA0X);
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // Record # 
Scenery->CurrentNum = temp; // Record #
//Debug******** cout << "\nCurrentNum = " << dec << Scenery->CurrentNum << "\n";
//Debug******** fprintf(outf, "CurrentNum %d \n", Scenery->CurrentNum);
// The following second byte is FENCE.
//fread(&temp,2,1,inputfile); // Skip 2 bytes (1 byte skip, 2nd byte fence)
#if 1;
hasFence = false; // Not found the fence yet.
fread(&temp,1,1,inputfile); // Skip 1 byte
fread(&temp2,1,1,inputfile); // Read fence type and texture

				//fenceType stores the sides of the road the fence lives, and the textureId to use for it.
				// If the top bit is set, fence on the left exists, if next bit is set, fence is on the right.  Both can also be set. 
				//The other 6 bits seem to the texture number
	if (temp2 != 0) {
//		cout << "\nFence byte = 0x" << hex << temp << "\n";
		bool bit7 = (temp2 & (0x1 << 7)) != 0;
		bool bit6 = (temp2 & (0x1 << 6)) != 0;
		Scenery->HasLeftFence = bit7;
		Scenery->HasRightFence = bit6;
//		cout << "\nHasLeftFence = " << Scenery->HasLeftFence << "\n";
//		cout << "\nHasRightFence = " << Scenery->HasRightFence << "\n";
		// Ignore the top 2 bits to find the texture to use
		Scenery->FenceTexID = temp2 & (0xff >> 2);
		if (Scenery->FenceTexID == 16) {Scenery->FenceTexID = 48;} // Nasty hack for Alpine 1 track
		arrFenceTex[i] = Scenery->FenceTexID;
//		cout << Scenery->FenceTexID << " ";
//		cout << "\nFenceTexID = " << arrFenceTex[i] << " i=" << i << "\n";
		#if 0; // start writing to file
		fprintf(outf, "%d ", Scenery->CurrentNum);
			fwrite("\n",1,1,outf); // New line
		fprintf(outf, "Fence byte = ");
		fprintf(outf, "%d ", temp2);
			fwrite("\n",1,1,outf); // New line
		fprintf(outf, "HasLeftFence ");
		fprintf(outf, "%d ", Scenery->HasLeftFence);
			fwrite("\n",1,1,outf); // New line
		fprintf(outf, "HasRightFence ");
		fprintf(outf, "%d ", Scenery->HasRightFence);
			fwrite("\n",1,1,outf); // New line
		fprintf(outf, "FenceTexID ");
		fprintf(outf, "%d ", Scenery->FenceTexID);
			fwrite("\n",1,1,outf); // New line
			fwrite("\n",1,1,outf); // New line
		#endif; // end writing to file
		hasFence = true;
		}	

#endif;

#if 1;
fread(&temp,1,1,inputfile); // Reading texture #1
	Scenery->Texture1 = temp; 
//Debug********	cout << "\nTexture #1 = 0x" << hex << Scenery->Texture1 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #2
	Scenery->Texture2 = temp; 
//Debug********	cout << "\nTexture #2 = 0x" << hex << Scenery->Texture2 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #3
	Scenery->Texture3 = temp; 
//Debug********	cout << "\nTexture #3 = 0x" << hex << Scenery->Texture3 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #4
	Scenery->Texture4 = temp; 
//Debug********	cout << "\nTexture #4 = 0x" << hex << Scenery->Texture4 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #5
	Scenery->Texture5 = temp; 
//Debug********	cout << "\nTexture #5 = 0x" << hex << Scenery->Texture5 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #6
	Scenery->Texture6 = temp; 
//Debug********	cout << "\nTexture #6 = 0x" << hex << Scenery->Texture6 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #7
	Scenery->Texture7 = temp; 
//Debug********	cout << "\nTexture #7 = 0x" << hex << Scenery->Texture7 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #8
	Scenery->Texture8 = temp; 
//Debug********	cout << "\nTexture #8 = 0x" << hex << Scenery->Texture8 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #9
	Scenery->Texture9 = temp; 
//Debug********	cout << "\nTexture #9 = 0x" << hex << Scenery->Texture9 << "\n";
fread(&temp,1,1,inputfile); // Reading texture #10
	Scenery->Texture10 = temp; 
//Debug********	cout << "\nTexture #10 = 0x" << hex << Scenery->Texture10 << "\n";


fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip reference point
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // a0x 
	Scenery->PointA0X = temp;
//	cout << "\nScenery->PointA0X = 0x" << hex << Scenery->PointA0X << "\n";
fread(&temp,4,1,inputfile); // a0z
	Scenery->PointA0Z = temp; 
//	cout << "\nScenery->PointA0Z = 0x" << hex << Scenery->PointA0Z << "\n";
fread(&temp,4,1,inputfile); // a0y
	Scenery->PointA0Y = temp; 
//	cout << "\nScenery->PointA0Y = 0x" << hex << Scenery->PointA0Y << "\n";

fread(&temp,4,1,inputfile); // a1x
	Scenery->PointA1X = temp; 
//	cout << "\nScenery->PointA1X = 0x" << hex << Scenery->PointA1X << "\n";
fread(&temp,4,1,inputfile); // a1z
	Scenery->PointA1Z = temp; 
//	cout << "\nScenery->PointA1Z = 0x" << hex << Scenery->PointA1Z << "\n";
fread(&temp,4,1,inputfile); // a1y
	Scenery->PointA1Y = temp; 
//	cout << "\nScenery->PointA1Y = 0x" << hex << Scenery->PointA1Y << "\n";

fread(&temp,4,1,inputfile); // a2x
	Scenery->PointA2X = temp; 
//	cout << "\nScenery->PointA2X = 0x" << hex << Scenery->PointA2X << "\n";
fread(&temp,4,1,inputfile); // a2z
	Scenery->PointA2Z = temp; 
//	cout << "\nScenery->PointA2Z = 0x" << hex << Scenery->PointA2Z << "\n";
fread(&temp,4,1,inputfile); // a2y
	Scenery->PointA2Y = temp; 
//	cout << "\nScenery->PointA2Y = 0x" << hex << Scenery->PointA2Y << "\n";

fread(&temp,4,1,inputfile); // a3x 
	Scenery->PointA3X = temp; 
//	cout << "\nScenery->PointA3X = 0x" << hex << Scenery->PointA3X << "\n";
fread(&temp,4,1,inputfile); // a3z
	Scenery->PointA3Z = temp; 
//	cout << "\nScenery->PointA3Z = 0x" << hex << Scenery->PointA3Z << "\n";
fread(&temp,4,1,inputfile); // a3y
	Scenery->PointA3Y = temp; 
//	cout << "\nScenery->PointA3Y = 0x" << hex << Scenery->PointA3Y << "\n";

fread(&temp,4,1,inputfile); // a4x
	Scenery->PointA4X = temp; 
//	cout << "\nScenery->PointA4X = 0x" << hex << Scenery->PointA4X << "\n";
fread(&temp,4,1,inputfile); // a4z
	Scenery->PointA4Z = temp; 
//	cout << "\nScenery->PointA4Z = 0x" << hex << Scenery->PointA4Z << "\n";
fread(&temp,4,1,inputfile); // a4y
	Scenery->PointA4Y = temp; 
//	cout << "\nScenery->PointA4Y = 0x" << hex << Scenery->PointA4Y << "\n";

fread(&temp,4,1,inputfile); // a5x 
	Scenery->PointA5X = temp; 
//	cout << "\nScenery->PointA5X = 0x" << hex << Scenery->PointA5X << "\n";
fread(&temp,4,1,inputfile); // a5z
	Scenery->PointA5Z = temp; 
//	cout << "\nScenery->PointA5Z = 0x" << hex << Scenery->PointA5Z << "\n";
fread(&temp,4,1,inputfile); // a5y
	Scenery->PointA5Y = temp; 
//	cout << "\nScenery->PointA5Y = 0x" << hex << Scenery->PointA5Y << "\n";

fread(&temp,4,1,inputfile); // a6x 
	Scenery->PointA6X = temp; 
//	cout << "\nScenery->PointA6X = 0x" << hex << Scenery->PointA6X << "\n";
fread(&temp,4,1,inputfile); // a6z
	Scenery->PointA6Z = temp; 
//	cout << "\nScenery->PointA6Z = 0x" << hex << Scenery->PointA6Z << "\n";
fread(&temp,4,1,inputfile); // a6y
	Scenery->PointA6Y = temp; 
//	cout << "\nScenery->PointA6Y = 0x" << hex << Scenery->PointA6Y << "\n";

fread(&temp,4,1,inputfile); // a7x
	Scenery->PointA7X = temp; 
//	cout << "\nScenery->PointA7X = 0x" << hex << Scenery->PointA7X << "\n";
fread(&temp,4,1,inputfile); // a7z
	Scenery->PointA7Z = temp; 
//	cout << "\nScenery->PointA7Z = 0x" << hex << Scenery->PointA7Z << "\n";
fread(&temp,4,1,inputfile); // a7y
	Scenery->PointA7Y = temp; 
//	cout << "\nScenery->PointA7Y = 0x" << hex << Scenery->PointA7Y << "\n";

fread(&temp,4,1,inputfile); // a8x
	Scenery->PointA8X = temp; 
//	cout << "\nScenery->PointA8X = 0x" << hex << Scenery->PointA8X << "\n";
fread(&temp,4,1,inputfile); // a8z
	Scenery->PointA8Z = temp; 
//	cout << "\nScenery->PointA8Z = 0x" << hex << Scenery->PointA8Z << "\n";
fread(&temp,4,1,inputfile); // a8y
	Scenery->PointA8Y = temp; 
//	cout << "\nScenery->PointA8Y = 0x" << hex << Scenery->PointA8Y << "\n";

fread(&temp,4,1,inputfile); // a9x
	Scenery->PointA9X = temp; 
//	cout << "\nScenery->PointA9X = 0x" << hex << Scenery->PointA9X << "\n";
fread(&temp,4,1,inputfile); // a9z
	Scenery->PointA9Z = temp; 
//	cout << "\nScenery->PointA9Z = 0x" << hex << Scenery->PointA9Z << "\n";
fread(&temp,4,1,inputfile); // a9y
	Scenery->PointA9Y = temp; 
//	cout << "\nScenery->PointA9Y = 0x" << hex << Scenery->PointA9Y << "\n";

fread(&temp,4,1,inputfile); // a10x 
	Scenery->PointA10X = temp; 
//	cout << "\nScenery->PointA10X = 0x" << hex << Scenery->PointA10X << "\n";
fread(&temp,4,1,inputfile); // a10z
	Scenery->PointA10Z = temp; 
//	cout << "\nScenery->PointA10Z = 0x" << hex << Scenery->PointA10Z << "\n";
fread(&temp,4,1,inputfile); // a10y
	Scenery->PointA10Y = temp; 
//	cout << "\nScenery->PointA10Y = 0x" << hex << Scenery->PointA10Y << "\n";

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // b0x 
	Scenery->PointB0X = temp; 
//	cout << "\nScenery->PointB0X = 0x" << hex << Scenery->PointB0X << "\n";
fread(&temp,4,1,inputfile); // b0z
	Scenery->PointB0Z = temp; 
//	cout << "\nScenery->PointB0Z = 0x" << hex << Scenery->PointB0Z << "\n";
fread(&temp,4,1,inputfile); // b0y
	Scenery->PointB0Y = temp; 
//	cout << "\nScenery->PointB0Y = 0x" << hex << Scenery->PointB0Y << "\n";

fread(&temp,4,1,inputfile); // b1x 
	Scenery->PointB1X = temp; 
//	cout << "\nScenery->PointB1X = 0x" << hex << Scenery->PointB1X << "\n";
fread(&temp,4,1,inputfile); // b1z
	Scenery->PointB1Z = temp; 
//	cout << "\nScenery->PointB1Z = 0x" << hex << Scenery->PointB1Z << "\n";
fread(&temp,4,1,inputfile); // b1y
	Scenery->PointB1Y = temp; 
//	cout << "\nScenery->PointB1Y = 0x" << hex << Scenery->PointB1Y << "\n";

fread(&temp,4,1,inputfile); // b2x 
	Scenery->PointB2X = temp; 
//	cout << "\nScenery->PointB2X = 0x" << hex << Scenery->PointB2X << "\n";
fread(&temp,4,1,inputfile); // b2z
	Scenery->PointB2Z = temp; 
//	cout << "\nScenery->PointB2Z = 0x" << hex << Scenery->PointB2Z << "\n";
fread(&temp,4,1,inputfile); // b2y
	Scenery->PointB2Y = temp; 
//	cout << "\nScenery->PointB2Y = 0x" << hex << Scenery->PointB2Y << "\n";

fread(&temp,4,1,inputfile); // b3x 
	Scenery->PointB3X = temp; 
//	cout << "\nScenery->PointB3X = 0x" << hex << Scenery->PointB3X << "\n";
fread(&temp,4,1,inputfile); // b3z
	Scenery->PointB3Z = temp; 
//	cout << "\nScenery->PointB3Z = 0x" << hex << Scenery->PointB3Z << "\n";
fread(&temp,4,1,inputfile); // b3y
	Scenery->PointB3Y = temp; 
//	cout << "\nScenery->PointB3Y = 0x" << hex << Scenery->PointB3Y << "\n";

fread(&temp,4,1,inputfile); // b4x
	Scenery->PointB4X = temp; 
//	cout << "\nScenery->PointB4X = 0x" << hex << Scenery->PointB4X << "\n";
fread(&temp,4,1,inputfile); // b4z
	Scenery->PointB4Z = temp; 
//	cout << "\nScenery->PointB4Z = 0x" << hex << Scenery->PointB4Z << "\n";
fread(&temp,4,1,inputfile); // b4y
	Scenery->PointB4Y = temp; 
//	cout << "\nScenery->PointB4Y = 0x" << hex << Scenery->PointB4Y << "\n";

fread(&temp,4,1,inputfile); // b5x 
	Scenery->PointB5X = temp; 
//	cout << "\nScenery->PointB5X = 0x" << hex << Scenery->PointB5X << "\n";
fread(&temp,4,1,inputfile); // b5z
	Scenery->PointB5Z = temp; 
//	cout << "\nScenery->PointB5Z = 0x" << hex << Scenery->PointB5Z << "\n";
fread(&temp,4,1,inputfile); // b5y
	Scenery->PointB5Y = temp; 
//	cout << "\nScenery->PointB5Y = 0x" << hex << Scenery->PointB5Y << "\n";

fread(&temp,4,1,inputfile); // b6x
	Scenery->PointB6X = temp; 
//	cout << "\nScenery->PointB6X = 0x" << hex << Scenery->PointB6X << "\n";
fread(&temp,4,1,inputfile); // b6z
	Scenery->PointB6Z = temp; 
//	cout << "\nScenery->PointB6Z = 0x" << hex << Scenery->PointB6Z << "\n";
fread(&temp,4,1,inputfile); // b6y
	Scenery->PointB6Y = temp; 
//	cout << "\nScenery->PointB6Y = 0x" << hex << Scenery->PointB6Y << "\n";

fread(&temp,4,1,inputfile); // b7x
	Scenery->PointB7X = temp; 
//	cout << "\nScenery->PointB7X = 0x" << hex << Scenery->PointB7X << "\n";
fread(&temp,4,1,inputfile); // b7z
	Scenery->PointB7Z = temp; 
//	cout << "\nScenery->PointB7Z = 0x" << hex << Scenery->PointB7Z << "\n";
fread(&temp,4,1,inputfile); // b7y
	Scenery->PointB7Y = temp; 
//	cout << "\nScenery->PointB7Y = 0x" << hex << Scenery->PointB7Y << "\n";

fread(&temp,4,1,inputfile); // b8x
	Scenery->PointB8X = temp; 
//	cout << "\nScenery->PointB8X = 0x" << hex << Scenery->PointB8X << "\n";
fread(&temp,4,1,inputfile); // b8z
	Scenery->PointB8Z = temp; 
//	cout << "\nScenery->PointB8Z = 0x" << hex << Scenery->PointB8Z << "\n";
fread(&temp,4,1,inputfile); // b8y
	Scenery->PointB8Y = temp; 
//	cout << "\nScenery->PointB8Y = 0x" << hex << Scenery->PointB8Y << "\n";

fread(&temp,4,1,inputfile); // b9x 
	Scenery->PointB9X = temp; 
//	cout << "\nScenery->PointB9X = 0x" << hex << Scenery->PointB9X << "\n";
fread(&temp,4,1,inputfile); // b9z
	Scenery->PointB9Z = temp; 
//	cout << "\nScenery->PointB9Z = 0x" << hex << Scenery->PointB9Z << "\n";
fread(&temp,4,1,inputfile); // b9y
	Scenery->PointB9Y = temp; 
//	cout << "\nScenery->PointB9Y = 0x" << hex << Scenery->PointB9Y << "\n";

fread(&temp,4,1,inputfile); // b10x
	Scenery->PointB10X = temp; 
//	cout << "\nScenery->PointB10X = 0x" << hex << Scenery->PointB10X << "\n";
fread(&temp,4,1,inputfile); // b10z
	Scenery->PointB10Z = temp; 
//	cout << "\nScenery->PointB10Z = 0x" << hex << Scenery->PointB10Z << "\n";
fread(&temp,4,1,inputfile); // b10y
	Scenery->PointB10Y = temp; 
//	cout << "\nScenery->PointB10Y = 0x" << hex << Scenery->PointB10Y << "\n";

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes


fread(&temp,4,1,inputfile); // c0x 
	Scenery->PointC0X = temp; 
fread(&temp,4,1,inputfile); // c0z
	Scenery->PointC0Z = temp;
fread(&temp,4,1,inputfile); // c0y
	Scenery->PointC0Y = temp;

fread(&temp,4,1,inputfile); // c1x
	Scenery->PointC1X = temp;
fread(&temp,4,1,inputfile); // c1z
	Scenery->PointC1Z = temp;
fread(&temp,4,1,inputfile); // c1y
	Scenery->PointC1Y = temp;

fread(&temp,4,1,inputfile); // c2x 
	Scenery->PointC2X = temp;
fread(&temp,4,1,inputfile); // c2z
	Scenery->PointC2Z = temp;
fread(&temp,4,1,inputfile); // c2y
	Scenery->PointC2Y = temp;

fread(&temp,4,1,inputfile); // c3x 
	Scenery->PointC3X = temp;
fread(&temp,4,1,inputfile); // c3z
	Scenery->PointC3Z = temp;
fread(&temp,4,1,inputfile); // c3y
	Scenery->PointC3Y = temp;

fread(&temp,4,1,inputfile); // c4x
	Scenery->PointC4X = temp;
fread(&temp,4,1,inputfile); // c4z
	Scenery->PointC4Z = temp;
fread(&temp,4,1,inputfile); // c4y
	Scenery->PointC4Y = temp;

fread(&temp,4,1,inputfile); // c5x 
	Scenery->PointC5X = temp;
fread(&temp,4,1,inputfile); // c5z
	Scenery->PointC5Z = temp;
fread(&temp,4,1,inputfile); // c5y
	Scenery->PointC5Y = temp;

fread(&temp,4,1,inputfile); // c6x
	Scenery->PointC6X = temp;
fread(&temp,4,1,inputfile); // c6z
	Scenery->PointC6Z = temp;
fread(&temp,4,1,inputfile); // c6y
	Scenery->PointC6Y = temp;

fread(&temp,4,1,inputfile); // c7x 
	Scenery->PointC7X = temp;
fread(&temp,4,1,inputfile); // c7z
	Scenery->PointC7Z = temp;
fread(&temp,4,1,inputfile); // c7y
	Scenery->PointC7Y = temp;

fread(&temp,4,1,inputfile); // c8x 
	Scenery->PointC8X = temp;
fread(&temp,4,1,inputfile); // c8z
	Scenery->PointC8Z = temp;
fread(&temp,4,1,inputfile); // c8y
	Scenery->PointC8Y = temp;

fread(&temp,4,1,inputfile); // c9x
	Scenery->PointC9X = temp;
fread(&temp,4,1,inputfile); // c9z
	Scenery->PointC9Z = temp;
fread(&temp,4,1,inputfile); // c9y
	Scenery->PointC9Y = temp;

fread(&temp,4,1,inputfile); // c10x
	Scenery->PointC10X = temp;
fread(&temp,4,1,inputfile); // c10z
	Scenery->PointC10Z = temp;
fread(&temp,4,1,inputfile); // c10y
	Scenery->PointC10Y = temp;

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // d0x 
	Scenery->PointD0X = temp;
fread(&temp,4,1,inputfile); // d0z
	Scenery->PointD0Z = temp;
fread(&temp,4,1,inputfile); // d0y
	Scenery->PointD0Y = temp;

fread(&temp,4,1,inputfile); // d1x 
	Scenery->PointD1X = temp;
fread(&temp,4,1,inputfile); // d1z
	Scenery->PointD1Z = temp;
fread(&temp,4,1,inputfile); // d1y
	Scenery->PointD1Y = temp;

fread(&temp,4,1,inputfile); // d2x 
	Scenery->PointD2X = temp;
fread(&temp,4,1,inputfile); // d2z
	Scenery->PointD2Z = temp;
fread(&temp,4,1,inputfile); // d2y
	Scenery->PointD2Y = temp;

fread(&temp,4,1,inputfile); // d3x
	Scenery->PointD3X = temp;
fread(&temp,4,1,inputfile); // d3z
	Scenery->PointD3Z = temp;
fread(&temp,4,1,inputfile); // d3y
	Scenery->PointD3Y = temp;

fread(&temp,4,1,inputfile); // d4x 
	Scenery->PointD4X = temp;
fread(&temp,4,1,inputfile); // d4z
	Scenery->PointD4Z = temp;
fread(&temp,4,1,inputfile); // d4y
	Scenery->PointD4Y = temp;

fread(&temp,4,1,inputfile); // d5x 
	Scenery->PointD5X = temp;
fread(&temp,4,1,inputfile); // d5z
	Scenery->PointD5Z = temp;
fread(&temp,4,1,inputfile); // d5y
	Scenery->PointD5Y = temp;

fread(&temp,4,1,inputfile); // d6x
	Scenery->PointD6X = temp;
fread(&temp,4,1,inputfile); // d6z
	Scenery->PointD6Z = temp;
fread(&temp,4,1,inputfile); // d6y
	Scenery->PointD6Y = temp;

fread(&temp,4,1,inputfile); // d7x
	Scenery->PointD7X = temp;
fread(&temp,4,1,inputfile); // d7z
	Scenery->PointD7Z = temp;
fread(&temp,4,1,inputfile); // d7y
	Scenery->PointD7Y = temp;

fread(&temp,4,1,inputfile); // d8x
	Scenery->PointD8X = temp;
fread(&temp,4,1,inputfile); // d8z
	Scenery->PointD8Z = temp;
fread(&temp,4,1,inputfile); // d8y
	Scenery->PointD8Y = temp;

fread(&temp,4,1,inputfile); // d9x
	Scenery->PointD9X = temp;
fread(&temp,4,1,inputfile); // d9z
	Scenery->PointD9Z = temp;
fread(&temp,4,1,inputfile); // d9y
	Scenery->PointD9Y = temp;

fread(&temp,4,1,inputfile); // d10x 
	Scenery->PointD10X = temp;
fread(&temp,4,1,inputfile); // d10z
	Scenery->PointD10Z = temp;
fread(&temp,4,1,inputfile); // d10y
	Scenery->PointD10Y = temp;

fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes
fread(&temp,4,1,inputfile); // Skip 4 bytes  // Skip unknown
fread(&temp,4,1,inputfile); // Skip 4 bytes

fread(&temp,4,1,inputfile); // e0x
	Scenery->PointE0X = temp;
fread(&temp,4,1,inputfile); // e0z
	Scenery->PointE0Z = temp;
fread(&temp,4,1,inputfile); // e0y
	Scenery->PointE0Y = temp;

fread(&temp,4,1,inputfile); // e1x 
	Scenery->PointE1X = temp;
fread(&temp,4,1,inputfile); // e1z
	Scenery->PointE1Z = temp;
fread(&temp,4,1,inputfile); // e1y
	Scenery->PointE1Y = temp;

fread(&temp,4,1,inputfile); // e2x
	Scenery->PointE2X = temp;
fread(&temp,4,1,inputfile); // e2z
	Scenery->PointE2Z = temp;
fread(&temp,4,1,inputfile); // e2y
	Scenery->PointE2Y = temp;

fread(&temp,4,1,inputfile); // e3x 
	Scenery->PointE3X = temp;
//cout << "\nScenery->PointE3X = 0x" << hex << Scenery->PointE3X << "\n";
fread(&temp,4,1,inputfile); // e3z
	Scenery->PointE3Z = temp;
//cout << "\nScenery->PointE3Z = 0x" << hex << Scenery->PointE3Z << "\n";
fread(&temp,4,1,inputfile); // e3y
	Scenery->PointE3Y = temp;
//cout << "\nScenery->PointE3Y = 0x" << hex << Scenery->PointE3Y << "\n";

fread(&temp,4,1,inputfile); // e4x
	Scenery->PointE4X = temp;
//cout << "\nScenery->PointE4X = 0x" << hex << Scenery->PointE4X << "\n";
fread(&temp,4,1,inputfile); // e4z
	Scenery->PointE4Z = temp;
//cout << "\nScenery->PointE4Z = 0x" << hex << Scenery->PointE4Z << "\n";
fread(&temp,4,1,inputfile); // e4y
	Scenery->PointE4Y = temp;
//cout << "\nScenery->PointE4Y = 0x" << hex << Scenery->PointE4Y << "\n";

fread(&temp,4,1,inputfile); // e5x 
	Scenery->PointE5X = temp;
fread(&temp,4,1,inputfile); // e5z
	Scenery->PointE5Z = temp;
fread(&temp,4,1,inputfile); // e5y
	Scenery->PointE5Y = temp;

fread(&temp,4,1,inputfile); // e6x
	Scenery->PointE6X = temp;
fread(&temp,4,1,inputfile); // e6z
	Scenery->PointE6Z = temp;
fread(&temp,4,1,inputfile); // e6y
	Scenery->PointE6Y = temp;

fread(&temp,4,1,inputfile); // e7x
	Scenery->PointE7X = temp;
fread(&temp,4,1,inputfile); // e7z
	Scenery->PointE7Z = temp;
fread(&temp,4,1,inputfile); // e7y
	Scenery->PointE7Y = temp;

fread(&temp,4,1,inputfile); // e8x
	Scenery->PointE8X = temp;
fread(&temp,4,1,inputfile); // e8z
	Scenery->PointE8Z = temp;
fread(&temp,4,1,inputfile); // e8y
	Scenery->PointE8Y = temp;

fread(&temp,4,1,inputfile); // e9x 
	Scenery->PointE9X = temp;
fread(&temp,4,1,inputfile); // e9z
	Scenery->PointE9Z = temp;
fread(&temp,4,1,inputfile); // e9y
	Scenery->PointE9Y = temp;

fread(&temp,4,1,inputfile); // e10x
	Scenery->PointE10X = temp;
fread(&temp,4,1,inputfile); // e10z
	Scenery->PointE10Z = temp;
fread(&temp,4,1,inputfile); // e10y
	Scenery->PointE10Y = temp;
#endif; // Reading coords

	for	(j=1; j<=149; j++) {
		fread(&temp,4,1,inputfile); // Skip 4 bytes
		}


#if 0; // FENCES BASED ON VIRTUAL ROAD
	if (hasFence == true) { // Fence was found previously 
		// Writing fences.
		if (Scenery->HasLeftFence != 0) {
					fprintf(outf, "o Fence%d", Scenery->CurrentNum);
					fprintf(outf, "TexID%d", Scenery->FenceTexID);
					fwrite("\n",1,1,outf);// New line
			// Add vertices above VirtRd left barrier. Consider 5 VirtRd blocks.
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][8]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][7]);
			fwrite("\n",1,1,outf); // New line
#if 0; // more points left
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][8]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][7]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][8]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][7]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][8]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][7]);
			fwrite("\n",1,1,outf); // New line
#endif;	// more points left					
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][8]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][7]);
			fwrite("\n",1,1,outf); // New line
		#if 1;
			fprintf(outf, "v "); // Verts on the road.
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][8]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][7]);
			fwrite("\n",1,1,outf); // New line

#if 0; // more points left (on the road)
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][8]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][7]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][8]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][7]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][8]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][7]);
			fwrite("\n",1,1,outf); // New line
#endif;	// more points left	(on the road)

			fprintf(outf, "v ");// Verts on the road.
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][6]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][8]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][7]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");//////////////////// ADD 3 MORE LINES FOR ADDITIONAL POINTS
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line
		#endif;
			#if 0; //More lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line
			#endif;//More lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			fwrite("\n",1,1,outf); // New line
//			#endif;
				#if 1; // Write polygons//////////////////// ADD 3 MORE POLYGONS FOR ADDITIONAL POINTS
				fwrite("f ",2,1,outf);// 1-2-9-10
				fprintf(outf, "%d ", (linesCounter-2)); //1
				fprintf(outf, "%d ", (linesCounter-1));//2
				fprintf(outf, "%d ", (linesCounter)); //3
				fprintf(outf, "%d ", (linesCounter+1));//4
				fwrite("\n",1,1,outf);// New line
				#endif;
			#if 0; //More polygons
				fwrite("f ",2,1,outf);// 1-2-9-10
				fprintf(outf, "%d ", (linesCounter-2)); //1
				fprintf(outf, "%d ", (linesCounter-1));//2
				fprintf(outf, "%d ", (linesCounter+6)); //9
				fprintf(outf, "%d ", (linesCounter+7));//10
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);// 2-3-8-9
				fprintf(outf, "%d ", (linesCounter-1)); 
				fprintf(outf, "%d ", (linesCounter));
				fprintf(outf, "%d ", (linesCounter+5)); 
				fprintf(outf, "%d ", (linesCounter+6));
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);// 3-4-7-8
				fprintf(outf, "%d ", (linesCounter)); 
				fprintf(outf, "%d ", (linesCounter+1));
				fprintf(outf, "%d ", (linesCounter+4)); 
				fprintf(outf, "%d ", (linesCounter+5));
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);// 4-5-6-7
				fprintf(outf, "%d ", (linesCounter+1)); 
				fprintf(outf, "%d ", (linesCounter+2));
				fprintf(outf, "%d ", (linesCounter+3)); 
				fprintf(outf, "%d ", (linesCounter+4));
				fwrite("\n",1,1,outf);// New line
			#endif;//More polygons		
			linesCounter += 2;
			}

		if (Scenery->HasRightFence != 0) {
					fprintf(outf, "o Fence%d", Scenery->CurrentNum);
					fprintf(outf, "TexID%d", Scenery->FenceTexID);
					fwrite("\n",1,1,outf);// New line
			// Add vertices above VirtRd right barrier. Consider 5 VirtRd blocks.
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][11]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][10]);
			fwrite("\n",1,1,outf); // New line
#if 0; // more points right
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][11]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][10]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][11]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][10]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][11]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][10]);
			fwrite("\n",1,1,outf); // New line
#endif;	// more points right		
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][11]+FENCEHEIGHT);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][10]);
			fwrite("\n",1,1,outf); // New line
		#if 1;
			fprintf(outf, "v "); // Verts on the road.
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][11]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+5][10]);
			fwrite("\n",1,1,outf); // New line

#if 0; // more points right (on the road)
			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][11]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+4][10]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][11]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+3][10]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][11]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+2][10]);
			fwrite("\n",1,1,outf); // New line
#endif;	// more points right (on the road)

			fprintf(outf, "v ");// Verts on the road.
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][9]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][11]);
			fprintf(outf, "%f ", ArrayVirtRd[Scenery->CurrentNum*4+1][10]);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");//////////////////// ADD 3 MORE LINES FOR ADDITIONAL POINTS
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1); 
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line
		#endif;
			#if 0; // More lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line
			#endif;//More lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			fwrite("\n",1,1,outf); // New line
//			#endif;

				#if 1; // Write polygons//////////////////// ADD 3 MORE POLYGONS FOR ADDITIONAL POINTS
				fwrite("f ",2,1,outf);
				fprintf(outf, "%d ", (linesCounter-2)); 
				fprintf(outf, "%d ", (linesCounter-1));
				fprintf(outf, "%d ", (linesCounter)); 
				fprintf(outf, "%d ", (linesCounter+1));
				fwrite("\n",1,1,outf);// New line
				#endif;
			linesCounter += 2;
			}
		}
#endif; // FENCES VIRTUAL ROAD


#if 1; // FENCES BASED ON SCENERY
	if (hasFence == true) { // Fence was found previously 
		// Writing fences.
		if (Scenery->HasLeftFence != 0) {
			// Add vertices above A7, B7, C7, D7, E7.
			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointA7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointA7Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointA7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointB7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointB7Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointB7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointC7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointC7Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointC7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointD7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointD7Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointD7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointE7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointE7Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointE7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

						// Add REAL vertices A7, B7, C7, D7, E7.
			#if 1; //reaL v
			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointA7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointA7Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointA7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointB7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointB7Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointB7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointC7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointC7Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointC7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointD7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointD7Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointD7Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointE7X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointE7Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointE7Y / SCALE);
			fwrite("\n",1,1,outf); // New line
			#endif; // real v

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter += 2;
			fwrite("\n",1,1,outf); // New line

			// Bottom lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter += 2;
			fwrite("\n",1,1,outf); // New line

#if 1;
// Write vt params. There are 10 vertices. Max uv coord is 1.0!
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrFenceTex[i]);
#endif; // vt

			#if 1; // Write polygons
//			for (p=0; p<4; p++) {
				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+1)); 
					fprintf(outf, "/%d ", matCounter+1);  // vt index
				fprintf(outf, "%d", (p+2));
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fprintf(outf, "%d", (p+7));
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fprintf(outf, "%d", (p+6));
					fprintf(outf, "/%d ", matCounter+2);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+2)); 
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fprintf(outf, "%d", (p+3));
					fprintf(outf, "/%d ", matCounter+6);  // vt index
				fprintf(outf, "%d", (p+8));
					fprintf(outf, "/%d ", matCounter+5);  // vt index
				fprintf(outf, "%d", (p+7));
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+3)); 
					fprintf(outf, "/%d ", matCounter+6);  // vt index
				fprintf(outf, "%d", (p+4));
					fprintf(outf, "/%d ", matCounter+8);  // vt index
				fprintf(outf, "%d", (p+9)); 
					fprintf(outf, "/%d ", matCounter+7);  // vt index
				fprintf(outf, "%d", (p+8));
					fprintf(outf, "/%d ", matCounter+5);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+4));
					fprintf(outf, "/%d ", matCounter+8);  // vt index
				fprintf(outf, "%d", (p+5));
					fprintf(outf, "/%d ", matCounter+10);  // vt index
				fprintf(outf, "%d", (p+10)); 
					fprintf(outf, "/%d ", matCounter+9);  // vt index
				fprintf(outf, "%d", (p+9));
					fprintf(outf, "/%d ", matCounter+7);  // vt index
				fwrite("\n",1,1,outf);// New line
				p += 10;
				matCounter += 10;
//				}
			//p++;
			#endif; // polygons

				// Make vertical lines: 1-6, 2-7, 3-8, 4-9, 5-10
//			fprintf(outf, "l ");
//			fprintf(outf, "%d ", linesCounter);
//			fprintf(outf, "%d ", linesCounter+5);
//			fwrite("\n",1,1,outf); // New line

			}

		if (Scenery->HasRightFence != 0) {
			// Add vertices above A2, B2, C2, D2, E2.
			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointA2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointA2Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointA2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointB2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointB2Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointB2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointC2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointC2Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointC2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointD2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointD2Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointD2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointE2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointE2Z / SCALE + FENCEHEIGHT);
			fprintf(outf, "%f ", -(float)Scenery->PointE2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

						// Add REAL vertices A2, B2, C2, D2, E2.
			#if 1; //reaL v
			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointA2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointA2Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointA2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointB2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointB2Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointB2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointC2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointC2Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointC2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointD2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointD2Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointD2Y / SCALE);
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "v ");
			fprintf(outf, "%f ", (float)Scenery->PointE2X / SCALE);
			fprintf(outf, "%f ", (float)Scenery->PointE2Z / SCALE);
			fprintf(outf, "%f ", -(float)Scenery->PointE2Y / SCALE);
			fwrite("\n",1,1,outf); // New line
			#endif; // real v


			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter +=2;
			fwrite("\n",1,1,outf); // New line

			// Bottom lines
			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter++;
			fwrite("\n",1,1,outf); // New line

			fprintf(outf, "l ");
			fprintf(outf, "%d ", linesCounter);
			fprintf(outf, "%d ", linesCounter+1);
			linesCounter +=2;
			fwrite("\n",1,1,outf); // New line

#if 1;
// Write vt params. There are 10 vertices. Max uv coord is 1.0!
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 1.000000 0.000000\n");
fprintf(outf, "vt 0.250000 0.000000 0.000000\n");
fprintf(outf, "vt 0.500000 1.000000 0.000000\n");
fprintf(outf, "vt 0.500000 0.000000 0.000000\n");
fprintf(outf, "vt 0.750000 1.000000 0.000000\n");
fprintf(outf, "vt 0.750000 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000 0.000000\n");
fprintf(outf, "usemtl %d\n", arrFenceTex[i]);
#endif; // vt

			#if 1; // Write polygons
//			for (p=0; p<4; p++) {
				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+1)); 
					fprintf(outf, "/%d ", matCounter+1);  // vt index
				fprintf(outf, "%d", (p+2));
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fprintf(outf, "%d", (p+7)); 
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fprintf(outf, "%d", (p+6));
					fprintf(outf, "/%d ", matCounter+2);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+2));
					fprintf(outf, "/%d ", matCounter+4);  // vt index
				fprintf(outf, "%d", (p+3));
					fprintf(outf, "/%d ", matCounter+6);  // vt index
				fprintf(outf, "%d", (p+8));
					fprintf(outf, "/%d ", matCounter+5);  // vt index
				fprintf(outf, "%d", (p+7));
					fprintf(outf, "/%d ", matCounter+3);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+3));
					fprintf(outf, "/%d ", matCounter+6);  // vt index
				fprintf(outf, "%d", (p+4));
					fprintf(outf, "/%d ", matCounter+8);  // vt index
				fprintf(outf, "%d", (p+9));
					fprintf(outf, "/%d ", matCounter+7);  // vt index
				fprintf(outf, "%d", (p+8));
					fprintf(outf, "/%d ", matCounter+5);  // vt index
				fwrite("\n",1,1,outf);// New line

				fwrite("f ",2,1,outf);
				fprintf(outf, "%d", (p+4)); 
					fprintf(outf, "/%d ", matCounter+8);  // vt index
				fprintf(outf, "%d", (p+5));
					fprintf(outf, "/%d ", matCounter+10);  // vt index
				fprintf(outf, "%d", (p+10));
					fprintf(outf, "/%d ", matCounter+9);  // vt index
				fprintf(outf, "%d", (p+9));
					fprintf(outf, "/%d ", matCounter+7);  // vt index
				fwrite("\n",1,1,outf);// New line
				p += 10;
//				}
			//p++;
			#endif; // polygons
			}
		}
#endif; // FENCES BASED ON SCENERY
	}

#if 1; // Fences material file
mtlfile = fopen("Fence.mtl","a");
fwrite("# Fence mtl data\n",17,1,mtlfile);
int c, count;

// New array size
size_t size = sizeof(arrFenceTex) / sizeof(arrFenceTex[0]); 
sort(arrFenceTex, arrFenceTex + size);
//count = distance(arrTextures, unique(arrTextures, arrTextures + size)); // unique values quantity.
count = distance(arrFenceTex, unique(arrFenceTex, arrFenceTex + size)); // unique values quantity.

//for (size_t f = 0; f < size-14300; ++f) {
  for (size_t f = 0; f < size; ++f) { // Show sorted list
//    cout << arrFenceTex[f] << ' ';
  }
//cout << endl << "Unique fence numbers = " << count << endl;
c = count;


for (c=0; c<count; c++) {
// Add all unique array elements to the materials file.
		if (c == 0) continue;
		fprintf(mtlfile, "newmtl %d\n", arrFenceTex[c]);
//		fprintf(mtlfile, "map_Kd C:\\Users\\Versus\\Desktop\\Tex%d.bmp\n", arrFenceTex[c]);
		fprintf(mtlfile, "map_Kd SceneryTextures\\ScnTex%d.bmp\n", arrFenceTex[c]);
		ExtractScenTexture(arrFenceTex[c], infilename);
}
fclose(mtlfile);
#endif; // Fences material file

fclose(outf);
//cout << "File Fence.obj successfuly saved\n";
//cout << "File Fence.mtl successfuly saved\n";
}



void ExtractReversedTrack(char *infilename) {
int i = 20;
int j1 = 10;
FILE *invtrackfile, *invtrackfile2, *invtrackfile3, *invtrackfile4;
FILE *refFile;
char *invtrackfilename;
char path[30], path2[30], path3[30], path4[30], refname[40];
char byte1;
short int bytes2;
int bytes4;
char tmpstrREV[17];
unsigned int skipcorrupted = 2;
//bool backwards = false;
bool straight = true; // ALSO unset all //***// commented lined in this function and comment lines nearby.


// Generating filename Al1_reversed.trk
	strncpy(tmpstrREV, infilename, 3); //скопировать 3 символа строки в строку tmpstr77. Получилось "al1"
	strcat(tmpstrREV, "_reversed.trk");
//	strncpy(tmpstr88, tmpstr77, 11);
//cout << "Filename for reversed track = " << tmpstrREV << endl;

#if 0; // Virtual road test
cout << "VergeLeft " << VIRTUALROADARRAY[i].VergeLeft <<"\n"; // Just for test!!!
cout << "VergeRight " << VIRTUALROADARRAY[i].VergeRight <<"\n";
cout << "BarrierLeft " << VIRTUALROADARRAY[i].BarrierLeft <<"\n";
cout << "BarrierRight " << VIRTUALROADARRAY[i].BarrierRight <<"\n";
cout << "Flag1 " << VIRTUALROADARRAY[i].Flag1 <<"\n";
cout << "Flag2 " << VIRTUALROADARRAY[i].Flag2 <<"\n";
cout << "Flag3 " << VIRTUALROADARRAY[i].Flag3 <<"\n";
cout << "Flag4 " << VIRTUALROADARRAY[i].Flag4 <<"\n";
cout << "X " << VIRTUALROADARRAY[i].X <<"\n";
cout << "Y " << VIRTUALROADARRAY[i].Y <<"\n";
cout << "Z " << VIRTUALROADARRAY[i].Z <<"\n";
cout << "slope " << VIRTUALROADARRAY[i].slope <<"\n";
cout << "slantA " << VIRTUALROADARRAY[i].slantA <<"\n";
cout << "orientation " << VIRTUALROADARRAY[i].orientation <<"\n";
cout << "zero " << VIRTUALROADARRAY[i].zero <<"\n";
cout << "Yorientation " << VIRTUALROADARRAY[i].Yorientation <<"\n";
cout << "slantB " << VIRTUALROADARRAY[i].slantB <<"\n";
cout << "Xorientation " << VIRTUALROADARRAY[i].Xorientation <<"\n";
cout << "zero1 " << VIRTUALROADARRAY[i].zero1 <<"\n";
#endif;

#if 0;// Objects test
cout << "Object->RefNode " << OBJECTSARRAY[i].RefNode <<"\n";
cout << "Object->BitmapN " << OBJECTSARRAY[i].BitmapN <<"\n";
cout << "Object->Flip " << OBJECTSARRAY[i].Flip <<"\n";
cout << "Object->Xcoord " << OBJECTSARRAY[i].Xcoord <<"\n";
cout << "Object->Zcoord " << OBJECTSARRAY[i].Zcoord <<"\n";
cout << "Object->Ycoord " << OBJECTSARRAY[i].Ycoord <<"\n";

//cout << "Object->Width " << OBJECTPARAMSARRAY[j1].Width <<"\n";
//cout << "Object->Height " << OBJECTPARAMSARRAY[j1].Height <<"\n";
//cout << "Object->SceneryType " << OBJECTPARAMSARRAY[j1].SceneryType <<"\n";
//cout << "Object->SceneryFlags " << OBJECTPARAMSARRAY[j1].SceneryFlags <<"\n";
//cout << "Object->rID " << OBJECTPARAMSARRAY[j1].rID <<"\n";
//cout << "Object->rID2 " << OBJECTPARAMSARRAY[j1].rID2 <<"\n";
#endif;

#if 0;// Traffic test
for (int ad=0; ad<numSceneryIndexes; ad++) {
cout << "Speed " << TRAFFICARRAY[ad].Speed <<"\n";
cout << "CopsSpeed " << TRAFFICARRAY[ad].CopsSpeed <<"\n";
cout << "Speed2 " << TRAFFICARRAY[ad].Speed2 <<"\n";
	}
#endif;


// SAVE VIRTUAL ROAD
#if 1;
// Write entries from the track's ending
//invtrackfilename = "VirtualRoad";
strcpy(path, "\Inverted_track\\VirtualRoad");
invtrackfile = fopen(path, "wb");
int j, j2;
j2=numVirtRdIndexes;
//temp = 0x44556677;
int deltaX = 0;
int deltaZ = 0;
int deltaY = 0;

// Calculate Delta x, y, z for all the coordinates. 
// It means to move the track's ending to the 0,0,0 (starting position)
// Select last VirtualRoad entry
//cout << "\ntrackEndVr-2 = " << dec << trackEndVr-3 << endl;

deltaX = Swap4Bytes(VIRTUALROADARRAY[trackEndVr-skipcorrupted].X); // Step forward to select a non-corrupt value
deltaZ = Swap4Bytes(VIRTUALROADARRAY[trackEndVr-skipcorrupted].Z); // CHECK and FIX skipcorrupted!!!
deltaY = Swap4Bytes(VIRTUALROADARRAY[trackEndVr-skipcorrupted].Y);

//cout << "\ndeltaX = 0x" << hex << deltaX << endl;
//cout << "\ndeltaZ = 0x" << hex << deltaZ << endl;
//cout << "\ndeltaY = 0x" << hex << deltaY << endl;

// Recalculate all coords
if (!straight) {
#if 1;
for (j=0; j<numVirtRdIndexes; j++) {
	if (deltaX<0) {
		VIRTUALROADARRAY[j].X = Swap4Bytes(VIRTUALROADARRAY[j].X) + abs(deltaX);
//		cout << "\nj = " << dec << j << endl;
//		cout << "\nvrX = 0x" << hex << VIRTUALROADARRAY[j].X << endl;
		VIRTUALROADARRAY[j].X = Swap4Bytes(VIRTUALROADARRAY[j].X);
	} else {
		VIRTUALROADARRAY[j].X = Swap4Bytes(VIRTUALROADARRAY[j].X) - abs(deltaX);
//		cout << "\nj = " << dec << j << endl;
//		cout << "vrX = 0x" << hex << VIRTUALROADARRAY[j].X << endl;
		VIRTUALROADARRAY[j].X = Swap4Bytes(VIRTUALROADARRAY[j].X);
		}
	if (deltaZ<0) {
		VIRTUALROADARRAY[j].Z = Swap4Bytes(VIRTUALROADARRAY[j].Z) + abs(deltaZ);
//		cout << "\nj = " << dec << j << endl;
//		cout << "vrZ = 0x" << hex << VIRTUALROADARRAY[j].Z << endl;
		VIRTUALROADARRAY[j].Z = Swap4Bytes(VIRTUALROADARRAY[j].Z);
	} else {
		VIRTUALROADARRAY[j].Z = Swap4Bytes(VIRTUALROADARRAY[j].Z) - abs(deltaZ);
//		cout << "\nj = " << dec << j << endl;
//		cout << "vrZ = 0x" << hex << VIRTUALROADARRAY[j].Z << endl;
		VIRTUALROADARRAY[j].Z = Swap4Bytes(VIRTUALROADARRAY[j].Z);
		}
	if (deltaY<0) {
		VIRTUALROADARRAY[j].Y = Swap4Bytes(VIRTUALROADARRAY[j].Y) + abs(deltaY);
		VIRTUALROADARRAY[j].Y = Swap4Bytes(VIRTUALROADARRAY[j].Y);
	} else {
		VIRTUALROADARRAY[j].Y = Swap4Bytes(VIRTUALROADARRAY[j].Y) - abs(deltaY);
		VIRTUALROADARRAY[j].Y = Swap4Bytes(VIRTUALROADARRAY[j].Y);
		}
}
#endif;
}
// Turn this on to backwards save
//***//for (j=numVirtRdIndexes-1; j>0; j--) {// -1: Skip corrupted value on the end of the track
for (j=0; j<numVirtRdIndexes-1; j++) {// Turn this on to straight save

	if ((VIRTUALROADARRAY[j].BarrierLeft == 0) && (VIRTUALROADARRAY[j].BarrierRight == 0)) { 
//		cout << "\nVirtual road data #" << j << " has zero values.\n";
		continue;
	}

	byte1 =  VIRTUALROADARRAY[j].VergeLeft;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].VergeRight;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].BarrierLeft;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].BarrierRight;
		fwrite(&byte1, 1, 1, invtrackfile); 
	
	byte1 =  VIRTUALROADARRAY[j].Flag1;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].Flag2;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].Flag3;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 =  VIRTUALROADARRAY[j].Flag4;
		fwrite(&byte1, 1, 1, invtrackfile); 

	bytes4 =  VIRTUALROADARRAY[j].X;
		fwrite(&bytes4, 4, 1, invtrackfile); 
	bytes4 =  VIRTUALROADARRAY[j].Z;
		fwrite(&bytes4, 4, 1, invtrackfile); 
	bytes4 =  VIRTUALROADARRAY[j].Y;
		fwrite(&bytes4, 4, 1, invtrackfile); 

	bytes2 =  VIRTUALROADARRAY[j].slope;
		fwrite(&bytes2, 2, 1, invtrackfile); 
	bytes2 =  VIRTUALROADARRAY[j].slantA;
		fwrite(&bytes2, 2, 1, invtrackfile); 

	bytes2 =  VIRTUALROADARRAY[j].orientation;
		fwrite(&bytes2, 2, 1, invtrackfile); 
	bytes2 =  VIRTUALROADARRAY[j].zero;
		fwrite(&bytes2, 2, 1, invtrackfile); 

	bytes2 =  VIRTUALROADARRAY[j].Yorientation;
		fwrite(&bytes2, 2, 1, invtrackfile); 
	bytes2 =  VIRTUALROADARRAY[j].slantB;
		fwrite(&bytes2, 2, 1, invtrackfile); 

	bytes2 =  VIRTUALROADARRAY[j].Xorientation;
		fwrite(&bytes2, 2, 1, invtrackfile); 
	bytes2 =  VIRTUALROADARRAY[j].zero1;
		fwrite(&bytes2, 2, 1, invtrackfile); 
	j2--; 
}

int temp = 0;
if (j2!=0) { // Fill rest of the road data with zero values.

	for (int j3=0; j3<j2*9; j3++) {
		fwrite(&temp, 4, 1, invtrackfile);
		}
	}
fclose(invtrackfile);
#endif; // Virtualroad


//SAVE TRAFFIC AND OBJECTS DATA 
#if 1;
unsigned int mainBlocksize = 0x3E8;
int objs = 0x4F424A53;
unsigned int totalLength = 0x428C;

strcpy(path, "\Inverted_track\\TrafficAndObjects");
invtrackfile = fopen(path, "wb");


// Write traffic data from the track's ending
// Turn this on to backwards save
//***//for (int a=numSceneryIndexes; a>0; a--) { // -1: Skip corrupted value on the end of the track
for (int a=1; a<=numSceneryIndexes; a++) { // Turn this on to straight save
	byte1 = TRAFFICARRAY[a].Speed;
		fwrite(&byte1, 1, 1, invtrackfile); 
	byte1 = TRAFFICARRAY[a].CopsSpeed;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = TRAFFICARRAY[a].Speed2;
		fwrite(&byte1, 1, 1, invtrackfile); 
	}


// Write header 
temp = numOBJParams;
temp = Swap4Bytes(temp);
	fwrite(&temp, 4, 1, invtrackfile);
temp = Swap4Bytes(mainBlocksize);
	fwrite(&temp, 4, 1, invtrackfile);
temp = Swap4Bytes(objs);// 'OBJS'
	fwrite(&temp, 4, 1, invtrackfile);
temp = Swap4Bytes(totalLength);
	fwrite(&temp, 4, 1, invtrackfile);
temp = 0;
	fwrite(&temp, 4, 1, invtrackfile);

// Write objects parameters data
	for (int r=0; r<numOBJParams; r++) {
// Write 16 bytes
	byte1 = OBJECTPARAMSARRAY[r].SceneryFlags;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = OBJECTPARAMSARRAY[r].SceneryType;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = OBJECTPARAMSARRAY[r].rID;
		fwrite(&byte1, 1, 1, invtrackfile);	
	byte1 = OBJECTPARAMSARRAY[r].rID2;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = 0;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = OBJECTPARAMSARRAY[r].Width;
		fwrite(&byte1, 1, 1, invtrackfile);

//	byte1 = 0;
	byte1 = OBJECTPARAMSARRAY[r].p1;
		fwrite(&byte1, 1, 1, invtrackfile);//Unknown bytes. Write just in case and for test
//	bytes2 = 0;
	bytes2 = OBJECTPARAMSARRAY[r].p2;
		fwrite(&bytes2, 2, 1, invtrackfile);//Unknown bytes. Write just in case and for test
	bytes2 = OBJECTPARAMSARRAY[r].p3;
		fwrite(&bytes2, 2, 1, invtrackfile);//Unknown bytes. Write just in case and for test
	bytes2 = OBJECTPARAMSARRAY[r].p4;
		fwrite(&bytes2, 2, 1, invtrackfile);//Unknown bytes. Write just in case and for test

	byte1 = OBJECTPARAMSARRAY[r].Height;
		fwrite(&byte1, 1, 1, invtrackfile);
//	byte1 = 0;
	byte1 = OBJECTPARAMSARRAY[r].p5;
		fwrite(&byte1, 1, 1, invtrackfile);
	byte1 = 0;
		fwrite(&byte1, 1, 1, invtrackfile);
}


// Write objects data from the track's end
unsigned int tmp = 0;

//***//for (int d=numOBJIndexes; d>0; d--) {// Turn this on to backwards save
for (int d=1; d<=numOBJIndexes; d++) {// Turn this on to straight save

//	tmp = Swap4Bytes(OBJECTSARRAY[d].RefNode); 	// Must be recalculated backwards
if (straight) {
	tmp = OBJECTSARRAY[d].RefNode; 	// Turn this on to straight save
	tmp = Swap4Bytes(tmp);
} else {
//	tmp = numOBJIndexes - tmp + 1079;
	tmp = Swap4Bytes(OBJECTSARRAY[d].RefNode);
	tmp = virtRDlengh - tmp - skipcorrupted;
	if (tmp < 0) {
		tmp = 0;
		}
//		cout << "ObjRefNode = " << tmp <<"\n"; // Just for test
}
	tmp = Swap4Bytes(tmp);
//		cout << "ObjRefNode = " << tmp <<"\n"; // Just for test
//	cout << "RefNode 0x" << hex << tmp <<"\n"; // Just for test
		fwrite(&tmp, 4, 1, invtrackfile);
	byte1 = OBJECTSARRAY[d].BitmapN;
		fwrite(&byte1, 1, 1, invtrackfile);
if (straight) {
	byte1 = OBJECTSARRAY[d].Flip;			// Turn this on to straight save
} else {
	byte1 = OBJECTSARRAY[d].Flip + 128;		// Turn this on to backwards save FIX!!!
	if (byte1 > 256) {
		byte1 = -OBJECTSARRAY[d].Flip;
		}
	}
		fwrite(&byte1, 1, 1, invtrackfile);
	bytes4 = OBJECTSARRAY[i].flags;
		fwrite(&bytes4, 4, 1, invtrackfile);
	bytes2 = OBJECTSARRAY[d].Xcoord;
		fwrite(&bytes2, 2, 1, invtrackfile);
	bytes2 = OBJECTSARRAY[d].Zcoord;
		fwrite(&bytes2, 2, 1, invtrackfile);
	bytes2 = OBJECTSARRAY[d].Ycoord;
		fwrite(&bytes2, 2, 1, invtrackfile);
	}
// Write zeros to fill the totalLength size. OR to the first TRKD?
tmp=0;
for (int tm=0; tm<76; tm++) {
	fwrite(&tmp, 4, 1, invtrackfile);
	}

fclose(invtrackfile);
#endif; // Traffic and objects




// Write TRKD sections (without CCB for now)
#if 1; 
unsigned int lengthTRKD = 0;
unsigned int CCBsize = 0; // Must be precalculated for all TRKDs.
unsigned int filler = 0; // Filler before CCB
int modulus = 0x3000; // If size > modulus, it will be increased later if necessary
int size2CCB = 0x21E0; // Size to CCB beginning.
unsigned int TRACKDEPTH = 12; // Quantity of rendered 12 scenery blocks for 3DO inverted track. For each TRKD!
unsigned int trkdArray[numSceneryIndexes] = {0};
unsigned int modulusArray[numSceneryIndexes] = {0};


// Write all unknown points
strcpy(refname, "refPoints024.obj");
refFile = fopen(refname, "w");


// Recalculate all coordinates (Move track's end to the 0,0,0)
if (!straight) {
#if 1;
for (j=0; j<numSceneryIndexes; j++) {
// DeltaX
	if (deltaX<0) {
		SCENERYARRAY[j].PointA0X = Swap4Bytes(SCENERYARRAY[j].PointA0X) + abs(deltaX);
		SCENERYARRAY[j].PointA1X = Swap4Bytes(SCENERYARRAY[j].PointA1X) + abs(deltaX);
		SCENERYARRAY[j].PointA2X = Swap4Bytes(SCENERYARRAY[j].PointA2X) + abs(deltaX);
		SCENERYARRAY[j].PointA3X = Swap4Bytes(SCENERYARRAY[j].PointA3X) + abs(deltaX);
		SCENERYARRAY[j].PointA4X = Swap4Bytes(SCENERYARRAY[j].PointA4X) + abs(deltaX);
		SCENERYARRAY[j].PointA5X = Swap4Bytes(SCENERYARRAY[j].PointA5X) + abs(deltaX);
		SCENERYARRAY[j].PointA6X = Swap4Bytes(SCENERYARRAY[j].PointA6X) + abs(deltaX);
		SCENERYARRAY[j].PointA7X = Swap4Bytes(SCENERYARRAY[j].PointA7X) + abs(deltaX);
		SCENERYARRAY[j].PointA8X = Swap4Bytes(SCENERYARRAY[j].PointA8X) + abs(deltaX);
		SCENERYARRAY[j].PointA9X = Swap4Bytes(SCENERYARRAY[j].PointA9X) + abs(deltaX);
		SCENERYARRAY[j].PointA10X = Swap4Bytes(SCENERYARRAY[j].PointA10X) + abs(deltaX);

		SCENERYARRAY[j].PointB0X = Swap4Bytes(SCENERYARRAY[j].PointB0X) + abs(deltaX);
		SCENERYARRAY[j].PointB1X = Swap4Bytes(SCENERYARRAY[j].PointB1X) + abs(deltaX);
		SCENERYARRAY[j].PointB2X = Swap4Bytes(SCENERYARRAY[j].PointB2X) + abs(deltaX);
		SCENERYARRAY[j].PointB3X = Swap4Bytes(SCENERYARRAY[j].PointB3X) + abs(deltaX);
		SCENERYARRAY[j].PointB4X = Swap4Bytes(SCENERYARRAY[j].PointB4X) + abs(deltaX);
		SCENERYARRAY[j].PointB5X = Swap4Bytes(SCENERYARRAY[j].PointB5X) + abs(deltaX);
		SCENERYARRAY[j].PointB6X = Swap4Bytes(SCENERYARRAY[j].PointB6X) + abs(deltaX);
		SCENERYARRAY[j].PointB7X = Swap4Bytes(SCENERYARRAY[j].PointB7X) + abs(deltaX);
		SCENERYARRAY[j].PointB8X = Swap4Bytes(SCENERYARRAY[j].PointB8X) + abs(deltaX);
		SCENERYARRAY[j].PointB9X = Swap4Bytes(SCENERYARRAY[j].PointB9X) + abs(deltaX);
		SCENERYARRAY[j].PointB10X = Swap4Bytes(SCENERYARRAY[j].PointB10X) + abs(deltaX);

		SCENERYARRAY[j].PointC0X = Swap4Bytes(SCENERYARRAY[j].PointC0X) + abs(deltaX);
		SCENERYARRAY[j].PointC1X = Swap4Bytes(SCENERYARRAY[j].PointC1X) + abs(deltaX);
		SCENERYARRAY[j].PointC2X = Swap4Bytes(SCENERYARRAY[j].PointC2X) + abs(deltaX);
		SCENERYARRAY[j].PointC3X = Swap4Bytes(SCENERYARRAY[j].PointC3X) + abs(deltaX);
		SCENERYARRAY[j].PointC4X = Swap4Bytes(SCENERYARRAY[j].PointC4X) + abs(deltaX);
		SCENERYARRAY[j].PointC5X = Swap4Bytes(SCENERYARRAY[j].PointC5X) + abs(deltaX);
		SCENERYARRAY[j].PointC6X = Swap4Bytes(SCENERYARRAY[j].PointC6X) + abs(deltaX);
		SCENERYARRAY[j].PointC7X = Swap4Bytes(SCENERYARRAY[j].PointC7X) + abs(deltaX);
		SCENERYARRAY[j].PointC8X = Swap4Bytes(SCENERYARRAY[j].PointC8X) + abs(deltaX);
		SCENERYARRAY[j].PointC9X = Swap4Bytes(SCENERYARRAY[j].PointC9X) + abs(deltaX);
		SCENERYARRAY[j].PointC10X = Swap4Bytes(SCENERYARRAY[j].PointC10X) + abs(deltaX);

		SCENERYARRAY[j].PointD0X = Swap4Bytes(SCENERYARRAY[j].PointD0X) + abs(deltaX);
		SCENERYARRAY[j].PointD1X = Swap4Bytes(SCENERYARRAY[j].PointD1X) + abs(deltaX);
		SCENERYARRAY[j].PointD2X = Swap4Bytes(SCENERYARRAY[j].PointD2X) + abs(deltaX);
		SCENERYARRAY[j].PointD3X = Swap4Bytes(SCENERYARRAY[j].PointD3X) + abs(deltaX);
		SCENERYARRAY[j].PointD4X = Swap4Bytes(SCENERYARRAY[j].PointD4X) + abs(deltaX);
		SCENERYARRAY[j].PointD5X = Swap4Bytes(SCENERYARRAY[j].PointD5X) + abs(deltaX);
		SCENERYARRAY[j].PointD6X = Swap4Bytes(SCENERYARRAY[j].PointD6X) + abs(deltaX);
		SCENERYARRAY[j].PointD7X = Swap4Bytes(SCENERYARRAY[j].PointD7X) + abs(deltaX);
		SCENERYARRAY[j].PointD8X = Swap4Bytes(SCENERYARRAY[j].PointD8X) + abs(deltaX);
		SCENERYARRAY[j].PointD9X = Swap4Bytes(SCENERYARRAY[j].PointD9X) + abs(deltaX);
		SCENERYARRAY[j].PointD10X = Swap4Bytes(SCENERYARRAY[j].PointD10X) + abs(deltaX);

		SCENERYARRAY[j].PointE0X = Swap4Bytes(SCENERYARRAY[j].PointE0X) + abs(deltaX);
		SCENERYARRAY[j].PointE1X = Swap4Bytes(SCENERYARRAY[j].PointE1X) + abs(deltaX);
		SCENERYARRAY[j].PointE2X = Swap4Bytes(SCENERYARRAY[j].PointE2X) + abs(deltaX);
		SCENERYARRAY[j].PointE3X = Swap4Bytes(SCENERYARRAY[j].PointE3X) + abs(deltaX);
		SCENERYARRAY[j].PointE4X = Swap4Bytes(SCENERYARRAY[j].PointE4X) + abs(deltaX);
		SCENERYARRAY[j].PointE5X = Swap4Bytes(SCENERYARRAY[j].PointE5X) + abs(deltaX);
		SCENERYARRAY[j].PointE6X = Swap4Bytes(SCENERYARRAY[j].PointE6X) + abs(deltaX);
		SCENERYARRAY[j].PointE7X = Swap4Bytes(SCENERYARRAY[j].PointE7X) + abs(deltaX);
		SCENERYARRAY[j].PointE8X = Swap4Bytes(SCENERYARRAY[j].PointE8X) + abs(deltaX);
		SCENERYARRAY[j].PointE9X = Swap4Bytes(SCENERYARRAY[j].PointE9X) + abs(deltaX);
		SCENERYARRAY[j].PointE10X = Swap4Bytes(SCENERYARRAY[j].PointE10X) + abs(deltaX);
//		cout << "\nj = " << dec << j << endl;
//		cout << "\nSCENERYARRAY[j].PointA0X = 0x" << hex << SCENERYARRAY[j].PointA0X << endl;
		SCENERYARRAY[j].PointA0X = Swap4Bytes(SCENERYARRAY[j].PointA0X);
		SCENERYARRAY[j].PointA1X = Swap4Bytes(SCENERYARRAY[j].PointA1X);
		SCENERYARRAY[j].PointA2X = Swap4Bytes(SCENERYARRAY[j].PointA2X);
		SCENERYARRAY[j].PointA3X = Swap4Bytes(SCENERYARRAY[j].PointA3X);
		SCENERYARRAY[j].PointA4X = Swap4Bytes(SCENERYARRAY[j].PointA4X);
		SCENERYARRAY[j].PointA5X = Swap4Bytes(SCENERYARRAY[j].PointA5X);
		SCENERYARRAY[j].PointA6X = Swap4Bytes(SCENERYARRAY[j].PointA6X);
		SCENERYARRAY[j].PointA7X = Swap4Bytes(SCENERYARRAY[j].PointA7X);
		SCENERYARRAY[j].PointA8X = Swap4Bytes(SCENERYARRAY[j].PointA8X);
		SCENERYARRAY[j].PointA9X = Swap4Bytes(SCENERYARRAY[j].PointA9X);
		SCENERYARRAY[j].PointA10X = Swap4Bytes(SCENERYARRAY[j].PointA10X);

		SCENERYARRAY[j].PointB0X = Swap4Bytes(SCENERYARRAY[j].PointB0X);
		SCENERYARRAY[j].PointB1X = Swap4Bytes(SCENERYARRAY[j].PointB1X);
		SCENERYARRAY[j].PointB2X = Swap4Bytes(SCENERYARRAY[j].PointB2X);
		SCENERYARRAY[j].PointB3X = Swap4Bytes(SCENERYARRAY[j].PointB3X);
		SCENERYARRAY[j].PointB4X = Swap4Bytes(SCENERYARRAY[j].PointB4X);
		SCENERYARRAY[j].PointB5X = Swap4Bytes(SCENERYARRAY[j].PointB5X);
		SCENERYARRAY[j].PointB6X = Swap4Bytes(SCENERYARRAY[j].PointB6X);
		SCENERYARRAY[j].PointB7X = Swap4Bytes(SCENERYARRAY[j].PointB7X);
		SCENERYARRAY[j].PointB8X = Swap4Bytes(SCENERYARRAY[j].PointB8X);
		SCENERYARRAY[j].PointB9X = Swap4Bytes(SCENERYARRAY[j].PointB9X);
		SCENERYARRAY[j].PointB10X = Swap4Bytes(SCENERYARRAY[j].PointB10X);

		SCENERYARRAY[j].PointC0X = Swap4Bytes(SCENERYARRAY[j].PointC0X);
		SCENERYARRAY[j].PointC1X = Swap4Bytes(SCENERYARRAY[j].PointC1X);
		SCENERYARRAY[j].PointC2X = Swap4Bytes(SCENERYARRAY[j].PointC2X);
		SCENERYARRAY[j].PointC3X = Swap4Bytes(SCENERYARRAY[j].PointC3X);
		SCENERYARRAY[j].PointC4X = Swap4Bytes(SCENERYARRAY[j].PointC4X);
		SCENERYARRAY[j].PointC5X = Swap4Bytes(SCENERYARRAY[j].PointC5X);
		SCENERYARRAY[j].PointC6X = Swap4Bytes(SCENERYARRAY[j].PointC6X);
		SCENERYARRAY[j].PointC7X = Swap4Bytes(SCENERYARRAY[j].PointC7X);
		SCENERYARRAY[j].PointC8X = Swap4Bytes(SCENERYARRAY[j].PointC8X);
		SCENERYARRAY[j].PointC9X = Swap4Bytes(SCENERYARRAY[j].PointC9X);
		SCENERYARRAY[j].PointC10X = Swap4Bytes(SCENERYARRAY[j].PointC10X);

		SCENERYARRAY[j].PointD0X = Swap4Bytes(SCENERYARRAY[j].PointD0X);
		SCENERYARRAY[j].PointD1X = Swap4Bytes(SCENERYARRAY[j].PointD1X);
		SCENERYARRAY[j].PointD2X = Swap4Bytes(SCENERYARRAY[j].PointD2X);
		SCENERYARRAY[j].PointD3X = Swap4Bytes(SCENERYARRAY[j].PointD3X);
		SCENERYARRAY[j].PointD4X = Swap4Bytes(SCENERYARRAY[j].PointD4X);
		SCENERYARRAY[j].PointD5X = Swap4Bytes(SCENERYARRAY[j].PointD5X);
		SCENERYARRAY[j].PointD6X = Swap4Bytes(SCENERYARRAY[j].PointD6X);
		SCENERYARRAY[j].PointD7X = Swap4Bytes(SCENERYARRAY[j].PointD7X);
		SCENERYARRAY[j].PointD8X = Swap4Bytes(SCENERYARRAY[j].PointD8X);
		SCENERYARRAY[j].PointD9X = Swap4Bytes(SCENERYARRAY[j].PointD9X);
		SCENERYARRAY[j].PointD10X = Swap4Bytes(SCENERYARRAY[j].PointD10X);

		SCENERYARRAY[j].PointE0X = Swap4Bytes(SCENERYARRAY[j].PointE0X);
		SCENERYARRAY[j].PointE1X = Swap4Bytes(SCENERYARRAY[j].PointE1X);
		SCENERYARRAY[j].PointE2X = Swap4Bytes(SCENERYARRAY[j].PointE2X);
		SCENERYARRAY[j].PointE3X = Swap4Bytes(SCENERYARRAY[j].PointE3X);
		SCENERYARRAY[j].PointE4X = Swap4Bytes(SCENERYARRAY[j].PointE4X);
		SCENERYARRAY[j].PointE5X = Swap4Bytes(SCENERYARRAY[j].PointE5X);
		SCENERYARRAY[j].PointE6X = Swap4Bytes(SCENERYARRAY[j].PointE6X);
		SCENERYARRAY[j].PointE7X = Swap4Bytes(SCENERYARRAY[j].PointE7X);
		SCENERYARRAY[j].PointE8X = Swap4Bytes(SCENERYARRAY[j].PointE8X);
		SCENERYARRAY[j].PointE9X = Swap4Bytes(SCENERYARRAY[j].PointE9X);
		SCENERYARRAY[j].PointE10X = Swap4Bytes(SCENERYARRAY[j].PointE10X);
	} else {
		SCENERYARRAY[j].PointA0X = Swap4Bytes(SCENERYARRAY[j].PointA0X) - abs(deltaX);
		SCENERYARRAY[j].PointA1X = Swap4Bytes(SCENERYARRAY[j].PointA1X) - abs(deltaX);
		SCENERYARRAY[j].PointA2X = Swap4Bytes(SCENERYARRAY[j].PointA2X) - abs(deltaX);
		SCENERYARRAY[j].PointA3X = Swap4Bytes(SCENERYARRAY[j].PointA3X) - abs(deltaX);
		SCENERYARRAY[j].PointA4X = Swap4Bytes(SCENERYARRAY[j].PointA4X) - abs(deltaX);
		SCENERYARRAY[j].PointA5X = Swap4Bytes(SCENERYARRAY[j].PointA5X) - abs(deltaX);
		SCENERYARRAY[j].PointA6X = Swap4Bytes(SCENERYARRAY[j].PointA6X) - abs(deltaX);
		SCENERYARRAY[j].PointA7X = Swap4Bytes(SCENERYARRAY[j].PointA7X) - abs(deltaX);
		SCENERYARRAY[j].PointA8X = Swap4Bytes(SCENERYARRAY[j].PointA8X) - abs(deltaX);
		SCENERYARRAY[j].PointA9X = Swap4Bytes(SCENERYARRAY[j].PointA9X) - abs(deltaX);
		SCENERYARRAY[j].PointA10X = Swap4Bytes(SCENERYARRAY[j].PointA10X) - abs(deltaX);

		SCENERYARRAY[j].PointB0X = Swap4Bytes(SCENERYARRAY[j].PointB0X) - abs(deltaX);
		SCENERYARRAY[j].PointB1X = Swap4Bytes(SCENERYARRAY[j].PointB1X) - abs(deltaX);
		SCENERYARRAY[j].PointB2X = Swap4Bytes(SCENERYARRAY[j].PointB2X) - abs(deltaX);
		SCENERYARRAY[j].PointB3X = Swap4Bytes(SCENERYARRAY[j].PointB3X) - abs(deltaX);
		SCENERYARRAY[j].PointB4X = Swap4Bytes(SCENERYARRAY[j].PointB4X) - abs(deltaX);
		SCENERYARRAY[j].PointB5X = Swap4Bytes(SCENERYARRAY[j].PointB5X) - abs(deltaX);
		SCENERYARRAY[j].PointB6X = Swap4Bytes(SCENERYARRAY[j].PointB6X) - abs(deltaX);
		SCENERYARRAY[j].PointB7X = Swap4Bytes(SCENERYARRAY[j].PointB7X) - abs(deltaX);
		SCENERYARRAY[j].PointB8X = Swap4Bytes(SCENERYARRAY[j].PointB8X) - abs(deltaX);
		SCENERYARRAY[j].PointB9X = Swap4Bytes(SCENERYARRAY[j].PointB9X) - abs(deltaX);
		SCENERYARRAY[j].PointB10X = Swap4Bytes(SCENERYARRAY[j].PointB10X) - abs(deltaX);

		SCENERYARRAY[j].PointC0X = Swap4Bytes(SCENERYARRAY[j].PointC0X) - abs(deltaX);
		SCENERYARRAY[j].PointC1X = Swap4Bytes(SCENERYARRAY[j].PointC1X) - abs(deltaX);
		SCENERYARRAY[j].PointC2X = Swap4Bytes(SCENERYARRAY[j].PointC2X) - abs(deltaX);
		SCENERYARRAY[j].PointC3X = Swap4Bytes(SCENERYARRAY[j].PointC3X) - abs(deltaX);
		SCENERYARRAY[j].PointC4X = Swap4Bytes(SCENERYARRAY[j].PointC4X) - abs(deltaX);
		SCENERYARRAY[j].PointC5X = Swap4Bytes(SCENERYARRAY[j].PointC5X) - abs(deltaX);
		SCENERYARRAY[j].PointC6X = Swap4Bytes(SCENERYARRAY[j].PointC6X) - abs(deltaX);
		SCENERYARRAY[j].PointC7X = Swap4Bytes(SCENERYARRAY[j].PointC7X) - abs(deltaX);
		SCENERYARRAY[j].PointC8X = Swap4Bytes(SCENERYARRAY[j].PointC8X) - abs(deltaX);
		SCENERYARRAY[j].PointC9X = Swap4Bytes(SCENERYARRAY[j].PointC9X) - abs(deltaX);
		SCENERYARRAY[j].PointC10X = Swap4Bytes(SCENERYARRAY[j].PointC10X) - abs(deltaX);

		SCENERYARRAY[j].PointD0X = Swap4Bytes(SCENERYARRAY[j].PointD0X) - abs(deltaX);
		SCENERYARRAY[j].PointD1X = Swap4Bytes(SCENERYARRAY[j].PointD1X) - abs(deltaX);
		SCENERYARRAY[j].PointD2X = Swap4Bytes(SCENERYARRAY[j].PointD2X) - abs(deltaX);
		SCENERYARRAY[j].PointD3X = Swap4Bytes(SCENERYARRAY[j].PointD3X) - abs(deltaX);
		SCENERYARRAY[j].PointD4X = Swap4Bytes(SCENERYARRAY[j].PointD4X) - abs(deltaX);
		SCENERYARRAY[j].PointD5X = Swap4Bytes(SCENERYARRAY[j].PointD5X) - abs(deltaX);
		SCENERYARRAY[j].PointD6X = Swap4Bytes(SCENERYARRAY[j].PointD6X) - abs(deltaX);
		SCENERYARRAY[j].PointD7X = Swap4Bytes(SCENERYARRAY[j].PointD7X) - abs(deltaX);
		SCENERYARRAY[j].PointD8X = Swap4Bytes(SCENERYARRAY[j].PointD8X) - abs(deltaX);
		SCENERYARRAY[j].PointD9X = Swap4Bytes(SCENERYARRAY[j].PointD9X) - abs(deltaX);
		SCENERYARRAY[j].PointD10X = Swap4Bytes(SCENERYARRAY[j].PointD10X) - abs(deltaX);

		SCENERYARRAY[j].PointE0X = Swap4Bytes(SCENERYARRAY[j].PointE0X) - abs(deltaX);
		SCENERYARRAY[j].PointE1X = Swap4Bytes(SCENERYARRAY[j].PointE1X) - abs(deltaX);
		SCENERYARRAY[j].PointE2X = Swap4Bytes(SCENERYARRAY[j].PointE2X) - abs(deltaX);
		SCENERYARRAY[j].PointE3X = Swap4Bytes(SCENERYARRAY[j].PointE3X) - abs(deltaX);
		SCENERYARRAY[j].PointE4X = Swap4Bytes(SCENERYARRAY[j].PointE4X) - abs(deltaX);
		SCENERYARRAY[j].PointE5X = Swap4Bytes(SCENERYARRAY[j].PointE5X) - abs(deltaX);
		SCENERYARRAY[j].PointE6X = Swap4Bytes(SCENERYARRAY[j].PointE6X) - abs(deltaX);
		SCENERYARRAY[j].PointE7X = Swap4Bytes(SCENERYARRAY[j].PointE7X) - abs(deltaX);
		SCENERYARRAY[j].PointE8X = Swap4Bytes(SCENERYARRAY[j].PointE8X) - abs(deltaX);
		SCENERYARRAY[j].PointE9X = Swap4Bytes(SCENERYARRAY[j].PointE9X) - abs(deltaX);
		SCENERYARRAY[j].PointE10X = Swap4Bytes(SCENERYARRAY[j].PointE10X) - abs(deltaX);
		////////// Swap for file writing
		SCENERYARRAY[j].PointA0X = Swap4Bytes(SCENERYARRAY[j].PointA0X);
		SCENERYARRAY[j].PointA1X = Swap4Bytes(SCENERYARRAY[j].PointA1X);
		SCENERYARRAY[j].PointA2X = Swap4Bytes(SCENERYARRAY[j].PointA2X);
		SCENERYARRAY[j].PointA3X = Swap4Bytes(SCENERYARRAY[j].PointA3X);
		SCENERYARRAY[j].PointA4X = Swap4Bytes(SCENERYARRAY[j].PointA4X);
		SCENERYARRAY[j].PointA5X = Swap4Bytes(SCENERYARRAY[j].PointA5X);
		SCENERYARRAY[j].PointA6X = Swap4Bytes(SCENERYARRAY[j].PointA6X);
		SCENERYARRAY[j].PointA7X = Swap4Bytes(SCENERYARRAY[j].PointA7X);
		SCENERYARRAY[j].PointA8X = Swap4Bytes(SCENERYARRAY[j].PointA8X);
		SCENERYARRAY[j].PointA9X = Swap4Bytes(SCENERYARRAY[j].PointA9X);
		SCENERYARRAY[j].PointA10X = Swap4Bytes(SCENERYARRAY[j].PointA10X);

		SCENERYARRAY[j].PointB0X = Swap4Bytes(SCENERYARRAY[j].PointB0X);
		SCENERYARRAY[j].PointB1X = Swap4Bytes(SCENERYARRAY[j].PointB1X);
		SCENERYARRAY[j].PointB2X = Swap4Bytes(SCENERYARRAY[j].PointB2X);
		SCENERYARRAY[j].PointB3X = Swap4Bytes(SCENERYARRAY[j].PointB3X);
		SCENERYARRAY[j].PointB4X = Swap4Bytes(SCENERYARRAY[j].PointB4X);
		SCENERYARRAY[j].PointB5X = Swap4Bytes(SCENERYARRAY[j].PointB5X);
		SCENERYARRAY[j].PointB6X = Swap4Bytes(SCENERYARRAY[j].PointB6X);
		SCENERYARRAY[j].PointB7X = Swap4Bytes(SCENERYARRAY[j].PointB7X);
		SCENERYARRAY[j].PointB8X = Swap4Bytes(SCENERYARRAY[j].PointB8X);
		SCENERYARRAY[j].PointB9X = Swap4Bytes(SCENERYARRAY[j].PointB9X);
		SCENERYARRAY[j].PointB10X = Swap4Bytes(SCENERYARRAY[j].PointB10X);

		SCENERYARRAY[j].PointC0X = Swap4Bytes(SCENERYARRAY[j].PointC0X);
		SCENERYARRAY[j].PointC1X = Swap4Bytes(SCENERYARRAY[j].PointC1X);
		SCENERYARRAY[j].PointC2X = Swap4Bytes(SCENERYARRAY[j].PointC2X);
		SCENERYARRAY[j].PointC3X = Swap4Bytes(SCENERYARRAY[j].PointC3X);
		SCENERYARRAY[j].PointC4X = Swap4Bytes(SCENERYARRAY[j].PointC4X);
		SCENERYARRAY[j].PointC5X = Swap4Bytes(SCENERYARRAY[j].PointC5X);
		SCENERYARRAY[j].PointC6X = Swap4Bytes(SCENERYARRAY[j].PointC6X);
		SCENERYARRAY[j].PointC7X = Swap4Bytes(SCENERYARRAY[j].PointC7X);
		SCENERYARRAY[j].PointC8X = Swap4Bytes(SCENERYARRAY[j].PointC8X);
		SCENERYARRAY[j].PointC9X = Swap4Bytes(SCENERYARRAY[j].PointC9X);
		SCENERYARRAY[j].PointC10X = Swap4Bytes(SCENERYARRAY[j].PointC10X);

		SCENERYARRAY[j].PointD0X = Swap4Bytes(SCENERYARRAY[j].PointD0X);
		SCENERYARRAY[j].PointD1X = Swap4Bytes(SCENERYARRAY[j].PointD1X);
		SCENERYARRAY[j].PointD2X = Swap4Bytes(SCENERYARRAY[j].PointD2X);
		SCENERYARRAY[j].PointD3X = Swap4Bytes(SCENERYARRAY[j].PointD3X);
		SCENERYARRAY[j].PointD4X = Swap4Bytes(SCENERYARRAY[j].PointD4X);
		SCENERYARRAY[j].PointD5X = Swap4Bytes(SCENERYARRAY[j].PointD5X);
		SCENERYARRAY[j].PointD6X = Swap4Bytes(SCENERYARRAY[j].PointD6X);
		SCENERYARRAY[j].PointD7X = Swap4Bytes(SCENERYARRAY[j].PointD7X);
		SCENERYARRAY[j].PointD8X = Swap4Bytes(SCENERYARRAY[j].PointD8X);
		SCENERYARRAY[j].PointD9X = Swap4Bytes(SCENERYARRAY[j].PointD9X);
		SCENERYARRAY[j].PointD10X = Swap4Bytes(SCENERYARRAY[j].PointD10X);

		SCENERYARRAY[j].PointE0X = Swap4Bytes(SCENERYARRAY[j].PointE0X);
		SCENERYARRAY[j].PointE1X = Swap4Bytes(SCENERYARRAY[j].PointE1X);
		SCENERYARRAY[j].PointE2X = Swap4Bytes(SCENERYARRAY[j].PointE2X);
		SCENERYARRAY[j].PointE3X = Swap4Bytes(SCENERYARRAY[j].PointE3X);
		SCENERYARRAY[j].PointE4X = Swap4Bytes(SCENERYARRAY[j].PointE4X);
		SCENERYARRAY[j].PointE5X = Swap4Bytes(SCENERYARRAY[j].PointE5X);
		SCENERYARRAY[j].PointE6X = Swap4Bytes(SCENERYARRAY[j].PointE6X);
		SCENERYARRAY[j].PointE7X = Swap4Bytes(SCENERYARRAY[j].PointE7X);
		SCENERYARRAY[j].PointE8X = Swap4Bytes(SCENERYARRAY[j].PointE8X);
		SCENERYARRAY[j].PointE9X = Swap4Bytes(SCENERYARRAY[j].PointE9X);
		SCENERYARRAY[j].PointE10X = Swap4Bytes(SCENERYARRAY[j].PointE10X);
		}

// DeltaZ
	if (deltaZ<0) {
		SCENERYARRAY[j].PointA0Z = Swap4Bytes(SCENERYARRAY[j].PointA0Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA1Z = Swap4Bytes(SCENERYARRAY[j].PointA1Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA2Z = Swap4Bytes(SCENERYARRAY[j].PointA2Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA3Z = Swap4Bytes(SCENERYARRAY[j].PointA3Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA4Z = Swap4Bytes(SCENERYARRAY[j].PointA4Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA5Z = Swap4Bytes(SCENERYARRAY[j].PointA5Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA6Z = Swap4Bytes(SCENERYARRAY[j].PointA6Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA7Z = Swap4Bytes(SCENERYARRAY[j].PointA7Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA8Z = Swap4Bytes(SCENERYARRAY[j].PointA8Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA9Z = Swap4Bytes(SCENERYARRAY[j].PointA9Z) + abs(deltaZ);
		SCENERYARRAY[j].PointA10Z = Swap4Bytes(SCENERYARRAY[j].PointA10Z) + abs(deltaZ);

		SCENERYARRAY[j].PointB0Z = Swap4Bytes(SCENERYARRAY[j].PointB0Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB1Z = Swap4Bytes(SCENERYARRAY[j].PointB1Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB2Z = Swap4Bytes(SCENERYARRAY[j].PointB2Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB3Z = Swap4Bytes(SCENERYARRAY[j].PointB3Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB4Z = Swap4Bytes(SCENERYARRAY[j].PointB4Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB5Z = Swap4Bytes(SCENERYARRAY[j].PointB5Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB6Z = Swap4Bytes(SCENERYARRAY[j].PointB6Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB7Z = Swap4Bytes(SCENERYARRAY[j].PointB7Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB8Z = Swap4Bytes(SCENERYARRAY[j].PointB8Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB9Z = Swap4Bytes(SCENERYARRAY[j].PointB9Z) + abs(deltaZ);
		SCENERYARRAY[j].PointB10Z = Swap4Bytes(SCENERYARRAY[j].PointB10Z) + abs(deltaZ);

		SCENERYARRAY[j].PointC0Z = Swap4Bytes(SCENERYARRAY[j].PointC0Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC1Z = Swap4Bytes(SCENERYARRAY[j].PointC1Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC2Z = Swap4Bytes(SCENERYARRAY[j].PointC2Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC3Z = Swap4Bytes(SCENERYARRAY[j].PointC3Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC4Z = Swap4Bytes(SCENERYARRAY[j].PointC4Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC5Z = Swap4Bytes(SCENERYARRAY[j].PointC5Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC6Z = Swap4Bytes(SCENERYARRAY[j].PointC6Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC7Z = Swap4Bytes(SCENERYARRAY[j].PointC7Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC8Z = Swap4Bytes(SCENERYARRAY[j].PointC8Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC9Z = Swap4Bytes(SCENERYARRAY[j].PointC9Z) + abs(deltaZ);
		SCENERYARRAY[j].PointC10Z = Swap4Bytes(SCENERYARRAY[j].PointC10Z) + abs(deltaZ);

		SCENERYARRAY[j].PointD0Z = Swap4Bytes(SCENERYARRAY[j].PointD0Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD1Z = Swap4Bytes(SCENERYARRAY[j].PointD1Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD2Z = Swap4Bytes(SCENERYARRAY[j].PointD2Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD3Z = Swap4Bytes(SCENERYARRAY[j].PointD3Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD4Z = Swap4Bytes(SCENERYARRAY[j].PointD4Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD5Z = Swap4Bytes(SCENERYARRAY[j].PointD5Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD6Z = Swap4Bytes(SCENERYARRAY[j].PointD6Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD7Z = Swap4Bytes(SCENERYARRAY[j].PointD7Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD8Z = Swap4Bytes(SCENERYARRAY[j].PointD8Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD9Z = Swap4Bytes(SCENERYARRAY[j].PointD9Z) + abs(deltaZ);
		SCENERYARRAY[j].PointD10Z = Swap4Bytes(SCENERYARRAY[j].PointD10Z) + abs(deltaZ);

		SCENERYARRAY[j].PointE0Z = Swap4Bytes(SCENERYARRAY[j].PointE0Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE1Z = Swap4Bytes(SCENERYARRAY[j].PointE1Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE2Z = Swap4Bytes(SCENERYARRAY[j].PointE2Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE3Z = Swap4Bytes(SCENERYARRAY[j].PointE3Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE4Z = Swap4Bytes(SCENERYARRAY[j].PointE4Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE5Z = Swap4Bytes(SCENERYARRAY[j].PointE5Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE6Z = Swap4Bytes(SCENERYARRAY[j].PointE6Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE7Z = Swap4Bytes(SCENERYARRAY[j].PointE7Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE8Z = Swap4Bytes(SCENERYARRAY[j].PointE8Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE9Z = Swap4Bytes(SCENERYARRAY[j].PointE9Z) + abs(deltaZ);
		SCENERYARRAY[j].PointE10Z = Swap4Bytes(SCENERYARRAY[j].PointE10Z) + abs(deltaX);
//		cout << "\nj = " << dec << j << endl;
//		cout << "\nSCENERYARRAY[j].PointA0X = 0x" << hex << SCENERYARRAY[j].PointA0X << endl;
		SCENERYARRAY[j].PointA0Z = Swap4Bytes(SCENERYARRAY[j].PointA0Z);
		SCENERYARRAY[j].PointA1Z = Swap4Bytes(SCENERYARRAY[j].PointA1Z);
		SCENERYARRAY[j].PointA2Z = Swap4Bytes(SCENERYARRAY[j].PointA2Z);
		SCENERYARRAY[j].PointA3Z = Swap4Bytes(SCENERYARRAY[j].PointA3Z);
		SCENERYARRAY[j].PointA4Z = Swap4Bytes(SCENERYARRAY[j].PointA4Z);
		SCENERYARRAY[j].PointA5Z = Swap4Bytes(SCENERYARRAY[j].PointA5Z);
		SCENERYARRAY[j].PointA6Z = Swap4Bytes(SCENERYARRAY[j].PointA6Z);
		SCENERYARRAY[j].PointA7Z = Swap4Bytes(SCENERYARRAY[j].PointA7Z);
		SCENERYARRAY[j].PointA8Z = Swap4Bytes(SCENERYARRAY[j].PointA8Z);
		SCENERYARRAY[j].PointA9Z = Swap4Bytes(SCENERYARRAY[j].PointA9Z);
		SCENERYARRAY[j].PointA10Z = Swap4Bytes(SCENERYARRAY[j].PointA10Z);

		SCENERYARRAY[j].PointB0Z = Swap4Bytes(SCENERYARRAY[j].PointB0Z);
		SCENERYARRAY[j].PointB1Z = Swap4Bytes(SCENERYARRAY[j].PointB1Z);
		SCENERYARRAY[j].PointB2Z = Swap4Bytes(SCENERYARRAY[j].PointB2Z);
		SCENERYARRAY[j].PointB3Z = Swap4Bytes(SCENERYARRAY[j].PointB3Z);
		SCENERYARRAY[j].PointB4Z = Swap4Bytes(SCENERYARRAY[j].PointB4Z);
		SCENERYARRAY[j].PointB5Z = Swap4Bytes(SCENERYARRAY[j].PointB5Z);
		SCENERYARRAY[j].PointB6Z = Swap4Bytes(SCENERYARRAY[j].PointB6Z);
		SCENERYARRAY[j].PointB7Z = Swap4Bytes(SCENERYARRAY[j].PointB7Z);
		SCENERYARRAY[j].PointB8Z = Swap4Bytes(SCENERYARRAY[j].PointB8Z);
		SCENERYARRAY[j].PointB9Z = Swap4Bytes(SCENERYARRAY[j].PointB9Z);
		SCENERYARRAY[j].PointB10Z = Swap4Bytes(SCENERYARRAY[j].PointB10Z);

		SCENERYARRAY[j].PointC0Z = Swap4Bytes(SCENERYARRAY[j].PointC0Z);
		SCENERYARRAY[j].PointC1Z = Swap4Bytes(SCENERYARRAY[j].PointC1Z);
		SCENERYARRAY[j].PointC2Z = Swap4Bytes(SCENERYARRAY[j].PointC2Z);
		SCENERYARRAY[j].PointC3Z = Swap4Bytes(SCENERYARRAY[j].PointC3Z);
		SCENERYARRAY[j].PointC4Z = Swap4Bytes(SCENERYARRAY[j].PointC4Z);
		SCENERYARRAY[j].PointC5Z = Swap4Bytes(SCENERYARRAY[j].PointC5Z);
		SCENERYARRAY[j].PointC6Z = Swap4Bytes(SCENERYARRAY[j].PointC6Z);
		SCENERYARRAY[j].PointC7Z = Swap4Bytes(SCENERYARRAY[j].PointC7Z);
		SCENERYARRAY[j].PointC8Z = Swap4Bytes(SCENERYARRAY[j].PointC8Z);
		SCENERYARRAY[j].PointC9Z = Swap4Bytes(SCENERYARRAY[j].PointC9Z);
		SCENERYARRAY[j].PointC10Z = Swap4Bytes(SCENERYARRAY[j].PointC10Z);

		SCENERYARRAY[j].PointD0Z = Swap4Bytes(SCENERYARRAY[j].PointD0Z);
		SCENERYARRAY[j].PointD1Z = Swap4Bytes(SCENERYARRAY[j].PointD1Z);
		SCENERYARRAY[j].PointD2Z = Swap4Bytes(SCENERYARRAY[j].PointD2Z);
		SCENERYARRAY[j].PointD3Z = Swap4Bytes(SCENERYARRAY[j].PointD3Z);
		SCENERYARRAY[j].PointD4Z = Swap4Bytes(SCENERYARRAY[j].PointD4Z);
		SCENERYARRAY[j].PointD5Z = Swap4Bytes(SCENERYARRAY[j].PointD5Z);
		SCENERYARRAY[j].PointD6Z = Swap4Bytes(SCENERYARRAY[j].PointD6Z);
		SCENERYARRAY[j].PointD7Z = Swap4Bytes(SCENERYARRAY[j].PointD7Z);
		SCENERYARRAY[j].PointD8Z = Swap4Bytes(SCENERYARRAY[j].PointD8Z);
		SCENERYARRAY[j].PointD9Z = Swap4Bytes(SCENERYARRAY[j].PointD9Z);
		SCENERYARRAY[j].PointD10Z = Swap4Bytes(SCENERYARRAY[j].PointD10Z);

		SCENERYARRAY[j].PointE0Z = Swap4Bytes(SCENERYARRAY[j].PointE0Z);
		SCENERYARRAY[j].PointE1Z = Swap4Bytes(SCENERYARRAY[j].PointE1Z);
		SCENERYARRAY[j].PointE2Z = Swap4Bytes(SCENERYARRAY[j].PointE2Z);
		SCENERYARRAY[j].PointE3Z = Swap4Bytes(SCENERYARRAY[j].PointE3Z);
		SCENERYARRAY[j].PointE4Z = Swap4Bytes(SCENERYARRAY[j].PointE4Z);
		SCENERYARRAY[j].PointE5Z = Swap4Bytes(SCENERYARRAY[j].PointE5Z);
		SCENERYARRAY[j].PointE6Z = Swap4Bytes(SCENERYARRAY[j].PointE6Z);
		SCENERYARRAY[j].PointE7Z = Swap4Bytes(SCENERYARRAY[j].PointE7Z);
		SCENERYARRAY[j].PointE8Z = Swap4Bytes(SCENERYARRAY[j].PointE8Z);
		SCENERYARRAY[j].PointE9Z = Swap4Bytes(SCENERYARRAY[j].PointE9Z);
		SCENERYARRAY[j].PointE10Z = Swap4Bytes(SCENERYARRAY[j].PointE10Z);
	} else {
		SCENERYARRAY[j].PointA0Z = Swap4Bytes(SCENERYARRAY[j].PointA0Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA1Z = Swap4Bytes(SCENERYARRAY[j].PointA1Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA2Z = Swap4Bytes(SCENERYARRAY[j].PointA2Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA3Z = Swap4Bytes(SCENERYARRAY[j].PointA3Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA4Z = Swap4Bytes(SCENERYARRAY[j].PointA4Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA5Z = Swap4Bytes(SCENERYARRAY[j].PointA5Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA6Z = Swap4Bytes(SCENERYARRAY[j].PointA6Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA7Z = Swap4Bytes(SCENERYARRAY[j].PointA7Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA8Z = Swap4Bytes(SCENERYARRAY[j].PointA8Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA9Z = Swap4Bytes(SCENERYARRAY[j].PointA9Z) - abs(deltaZ);
		SCENERYARRAY[j].PointA10Z = Swap4Bytes(SCENERYARRAY[j].PointA10Z) - abs(deltaZ);

		SCENERYARRAY[j].PointB0Z = Swap4Bytes(SCENERYARRAY[j].PointB0Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB1Z = Swap4Bytes(SCENERYARRAY[j].PointB1Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB2Z = Swap4Bytes(SCENERYARRAY[j].PointB2Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB3Z = Swap4Bytes(SCENERYARRAY[j].PointB3Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB4Z = Swap4Bytes(SCENERYARRAY[j].PointB4Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB5Z = Swap4Bytes(SCENERYARRAY[j].PointB5Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB6Z = Swap4Bytes(SCENERYARRAY[j].PointB6Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB7Z = Swap4Bytes(SCENERYARRAY[j].PointB7Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB8Z = Swap4Bytes(SCENERYARRAY[j].PointB8Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB9Z = Swap4Bytes(SCENERYARRAY[j].PointB9Z) - abs(deltaZ);
		SCENERYARRAY[j].PointB10Z = Swap4Bytes(SCENERYARRAY[j].PointB10Z) - abs(deltaZ);

		SCENERYARRAY[j].PointC0Z = Swap4Bytes(SCENERYARRAY[j].PointC0Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC1Z = Swap4Bytes(SCENERYARRAY[j].PointC1Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC2Z = Swap4Bytes(SCENERYARRAY[j].PointC2Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC3Z = Swap4Bytes(SCENERYARRAY[j].PointC3Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC4Z = Swap4Bytes(SCENERYARRAY[j].PointC4Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC5Z = Swap4Bytes(SCENERYARRAY[j].PointC5Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC6Z = Swap4Bytes(SCENERYARRAY[j].PointC6Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC7Z = Swap4Bytes(SCENERYARRAY[j].PointC7Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC8Z = Swap4Bytes(SCENERYARRAY[j].PointC8Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC9Z = Swap4Bytes(SCENERYARRAY[j].PointC9Z) - abs(deltaZ);
		SCENERYARRAY[j].PointC10Z = Swap4Bytes(SCENERYARRAY[j].PointC10Z) - abs(deltaZ);

		SCENERYARRAY[j].PointD0Z = Swap4Bytes(SCENERYARRAY[j].PointD0Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD1Z = Swap4Bytes(SCENERYARRAY[j].PointD1Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD2Z = Swap4Bytes(SCENERYARRAY[j].PointD2Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD3Z = Swap4Bytes(SCENERYARRAY[j].PointD3Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD4Z = Swap4Bytes(SCENERYARRAY[j].PointD4Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD5Z = Swap4Bytes(SCENERYARRAY[j].PointD5Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD6Z = Swap4Bytes(SCENERYARRAY[j].PointD6Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD7Z = Swap4Bytes(SCENERYARRAY[j].PointD7Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD8Z = Swap4Bytes(SCENERYARRAY[j].PointD8Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD9Z = Swap4Bytes(SCENERYARRAY[j].PointD9Z) - abs(deltaZ);
		SCENERYARRAY[j].PointD10Z = Swap4Bytes(SCENERYARRAY[j].PointD10Z) - abs(deltaZ);

		SCENERYARRAY[j].PointE0Z = Swap4Bytes(SCENERYARRAY[j].PointE0Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE1Z = Swap4Bytes(SCENERYARRAY[j].PointE1Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE2Z = Swap4Bytes(SCENERYARRAY[j].PointE2Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE3Z = Swap4Bytes(SCENERYARRAY[j].PointE3Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE4Z = Swap4Bytes(SCENERYARRAY[j].PointE4Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE5Z = Swap4Bytes(SCENERYARRAY[j].PointE5Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE6Z = Swap4Bytes(SCENERYARRAY[j].PointE6Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE7Z = Swap4Bytes(SCENERYARRAY[j].PointE7Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE8Z = Swap4Bytes(SCENERYARRAY[j].PointE8Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE9Z = Swap4Bytes(SCENERYARRAY[j].PointE9Z) - abs(deltaZ);
		SCENERYARRAY[j].PointE10Z = Swap4Bytes(SCENERYARRAY[j].PointE10Z) - abs(deltaZ);
		////////// Swap for file writing
		SCENERYARRAY[j].PointA0Z = Swap4Bytes(SCENERYARRAY[j].PointA0Z);
		SCENERYARRAY[j].PointA1Z = Swap4Bytes(SCENERYARRAY[j].PointA1Z);
		SCENERYARRAY[j].PointA2Z = Swap4Bytes(SCENERYARRAY[j].PointA2Z);
		SCENERYARRAY[j].PointA3Z = Swap4Bytes(SCENERYARRAY[j].PointA3Z);
		SCENERYARRAY[j].PointA4Z = Swap4Bytes(SCENERYARRAY[j].PointA4Z);
		SCENERYARRAY[j].PointA5Z = Swap4Bytes(SCENERYARRAY[j].PointA5Z);
		SCENERYARRAY[j].PointA6Z = Swap4Bytes(SCENERYARRAY[j].PointA6Z);
		SCENERYARRAY[j].PointA7Z = Swap4Bytes(SCENERYARRAY[j].PointA7Z);
		SCENERYARRAY[j].PointA8Z = Swap4Bytes(SCENERYARRAY[j].PointA8Z);
		SCENERYARRAY[j].PointA9Z = Swap4Bytes(SCENERYARRAY[j].PointA9Z);
		SCENERYARRAY[j].PointA10Z = Swap4Bytes(SCENERYARRAY[j].PointA10Z);

		SCENERYARRAY[j].PointB0Z = Swap4Bytes(SCENERYARRAY[j].PointB0Z);
		SCENERYARRAY[j].PointB1Z = Swap4Bytes(SCENERYARRAY[j].PointB1Z);
		SCENERYARRAY[j].PointB2Z = Swap4Bytes(SCENERYARRAY[j].PointB2Z);
		SCENERYARRAY[j].PointB3Z = Swap4Bytes(SCENERYARRAY[j].PointB3Z);
		SCENERYARRAY[j].PointB4Z = Swap4Bytes(SCENERYARRAY[j].PointB4Z);
		SCENERYARRAY[j].PointB5Z = Swap4Bytes(SCENERYARRAY[j].PointB5Z);
		SCENERYARRAY[j].PointB6Z = Swap4Bytes(SCENERYARRAY[j].PointB6Z);
		SCENERYARRAY[j].PointB7Z = Swap4Bytes(SCENERYARRAY[j].PointB7Z);
		SCENERYARRAY[j].PointB8Z = Swap4Bytes(SCENERYARRAY[j].PointB8Z);
		SCENERYARRAY[j].PointB9Z = Swap4Bytes(SCENERYARRAY[j].PointB9Z);
		SCENERYARRAY[j].PointB10Z = Swap4Bytes(SCENERYARRAY[j].PointB10Z);

		SCENERYARRAY[j].PointC0Z = Swap4Bytes(SCENERYARRAY[j].PointC0Z);
		SCENERYARRAY[j].PointC1Z = Swap4Bytes(SCENERYARRAY[j].PointC1Z);
		SCENERYARRAY[j].PointC2Z = Swap4Bytes(SCENERYARRAY[j].PointC2Z);
		SCENERYARRAY[j].PointC3Z = Swap4Bytes(SCENERYARRAY[j].PointC3Z);
		SCENERYARRAY[j].PointC4Z = Swap4Bytes(SCENERYARRAY[j].PointC4Z);
		SCENERYARRAY[j].PointC5Z = Swap4Bytes(SCENERYARRAY[j].PointC5Z);
		SCENERYARRAY[j].PointC6Z = Swap4Bytes(SCENERYARRAY[j].PointC6Z);
		SCENERYARRAY[j].PointC7Z = Swap4Bytes(SCENERYARRAY[j].PointC7Z);
		SCENERYARRAY[j].PointC8Z = Swap4Bytes(SCENERYARRAY[j].PointC8Z);
		SCENERYARRAY[j].PointC9Z = Swap4Bytes(SCENERYARRAY[j].PointC9Z);
		SCENERYARRAY[j].PointC10Z = Swap4Bytes(SCENERYARRAY[j].PointC10Z);

		SCENERYARRAY[j].PointD0Z = Swap4Bytes(SCENERYARRAY[j].PointD0Z);
		SCENERYARRAY[j].PointD1Z = Swap4Bytes(SCENERYARRAY[j].PointD1Z);
		SCENERYARRAY[j].PointD2Z = Swap4Bytes(SCENERYARRAY[j].PointD2Z);
		SCENERYARRAY[j].PointD3Z = Swap4Bytes(SCENERYARRAY[j].PointD3Z);
		SCENERYARRAY[j].PointD4Z = Swap4Bytes(SCENERYARRAY[j].PointD4Z);
		SCENERYARRAY[j].PointD5Z = Swap4Bytes(SCENERYARRAY[j].PointD5Z);
		SCENERYARRAY[j].PointD6Z = Swap4Bytes(SCENERYARRAY[j].PointD6Z);
		SCENERYARRAY[j].PointD7Z = Swap4Bytes(SCENERYARRAY[j].PointD7Z);
		SCENERYARRAY[j].PointD8Z = Swap4Bytes(SCENERYARRAY[j].PointD8Z);
		SCENERYARRAY[j].PointD9Z = Swap4Bytes(SCENERYARRAY[j].PointD9Z);
		SCENERYARRAY[j].PointD10Z = Swap4Bytes(SCENERYARRAY[j].PointD10Z);

		SCENERYARRAY[j].PointE0Z = Swap4Bytes(SCENERYARRAY[j].PointE0Z);
		SCENERYARRAY[j].PointE1Z = Swap4Bytes(SCENERYARRAY[j].PointE1Z);
		SCENERYARRAY[j].PointE2Z = Swap4Bytes(SCENERYARRAY[j].PointE2Z);
		SCENERYARRAY[j].PointE3Z = Swap4Bytes(SCENERYARRAY[j].PointE3Z);
		SCENERYARRAY[j].PointE4Z = Swap4Bytes(SCENERYARRAY[j].PointE4Z);
		SCENERYARRAY[j].PointE5Z = Swap4Bytes(SCENERYARRAY[j].PointE5Z);
		SCENERYARRAY[j].PointE6Z = Swap4Bytes(SCENERYARRAY[j].PointE6Z);
		SCENERYARRAY[j].PointE7Z = Swap4Bytes(SCENERYARRAY[j].PointE7Z);
		SCENERYARRAY[j].PointE8Z = Swap4Bytes(SCENERYARRAY[j].PointE8Z);
		SCENERYARRAY[j].PointE9Z = Swap4Bytes(SCENERYARRAY[j].PointE9Z);
		SCENERYARRAY[j].PointE10Z = Swap4Bytes(SCENERYARRAY[j].PointE10Z);
		}
// DeltaY
	if (deltaY<0) {
		SCENERYARRAY[j].PointA0Y = Swap4Bytes(SCENERYARRAY[j].PointA0Y) + abs(deltaY);
		SCENERYARRAY[j].PointA1Y = Swap4Bytes(SCENERYARRAY[j].PointA1Y) + abs(deltaY);
		SCENERYARRAY[j].PointA2Y = Swap4Bytes(SCENERYARRAY[j].PointA2Y) + abs(deltaY);
		SCENERYARRAY[j].PointA3Y = Swap4Bytes(SCENERYARRAY[j].PointA3Y) + abs(deltaY);
		SCENERYARRAY[j].PointA4Y = Swap4Bytes(SCENERYARRAY[j].PointA4Y) + abs(deltaY);
		SCENERYARRAY[j].PointA5Y = Swap4Bytes(SCENERYARRAY[j].PointA5Y) + abs(deltaY);
		SCENERYARRAY[j].PointA6Y = Swap4Bytes(SCENERYARRAY[j].PointA6Y) + abs(deltaY);
		SCENERYARRAY[j].PointA7Y = Swap4Bytes(SCENERYARRAY[j].PointA7Y) + abs(deltaY);
		SCENERYARRAY[j].PointA8Y = Swap4Bytes(SCENERYARRAY[j].PointA8Y) + abs(deltaY);
		SCENERYARRAY[j].PointA9Y = Swap4Bytes(SCENERYARRAY[j].PointA9Y) + abs(deltaY);
		SCENERYARRAY[j].PointA10Y = Swap4Bytes(SCENERYARRAY[j].PointA10Y) + abs(deltaY);

		SCENERYARRAY[j].PointB0Y = Swap4Bytes(SCENERYARRAY[j].PointB0Y) + abs(deltaY);
		SCENERYARRAY[j].PointB1Y = Swap4Bytes(SCENERYARRAY[j].PointB1Y) + abs(deltaY);
		SCENERYARRAY[j].PointB2Y = Swap4Bytes(SCENERYARRAY[j].PointB2Y) + abs(deltaY);
		SCENERYARRAY[j].PointB3Y = Swap4Bytes(SCENERYARRAY[j].PointB3Y) + abs(deltaY);
		SCENERYARRAY[j].PointB4Y = Swap4Bytes(SCENERYARRAY[j].PointB4Y) + abs(deltaY);
		SCENERYARRAY[j].PointB5Y = Swap4Bytes(SCENERYARRAY[j].PointB5Y) + abs(deltaY);
		SCENERYARRAY[j].PointB6Y = Swap4Bytes(SCENERYARRAY[j].PointB6Y) + abs(deltaY);
		SCENERYARRAY[j].PointB7Y = Swap4Bytes(SCENERYARRAY[j].PointB7Y) + abs(deltaY);
		SCENERYARRAY[j].PointB8Y = Swap4Bytes(SCENERYARRAY[j].PointB8Y) + abs(deltaY);
		SCENERYARRAY[j].PointB9Y = Swap4Bytes(SCENERYARRAY[j].PointB9Y) + abs(deltaY);
		SCENERYARRAY[j].PointB10Y = Swap4Bytes(SCENERYARRAY[j].PointB10Y) + abs(deltaY);

		SCENERYARRAY[j].PointC0Y = Swap4Bytes(SCENERYARRAY[j].PointC0Y) + abs(deltaY);
		SCENERYARRAY[j].PointC1Y = Swap4Bytes(SCENERYARRAY[j].PointC1Y) + abs(deltaY);
		SCENERYARRAY[j].PointC2Y = Swap4Bytes(SCENERYARRAY[j].PointC2Y) + abs(deltaY);
		SCENERYARRAY[j].PointC3Y = Swap4Bytes(SCENERYARRAY[j].PointC3Y) + abs(deltaY);
		SCENERYARRAY[j].PointC4Y = Swap4Bytes(SCENERYARRAY[j].PointC4Y) + abs(deltaY);
		SCENERYARRAY[j].PointC5Y = Swap4Bytes(SCENERYARRAY[j].PointC5Y) + abs(deltaY);
		SCENERYARRAY[j].PointC6Y = Swap4Bytes(SCENERYARRAY[j].PointC6Y) + abs(deltaY);
		SCENERYARRAY[j].PointC7Y = Swap4Bytes(SCENERYARRAY[j].PointC7Y) + abs(deltaY);
		SCENERYARRAY[j].PointC8Y = Swap4Bytes(SCENERYARRAY[j].PointC8Y) + abs(deltaY);
		SCENERYARRAY[j].PointC9Y = Swap4Bytes(SCENERYARRAY[j].PointC9Y) + abs(deltaY);
		SCENERYARRAY[j].PointC10Y = Swap4Bytes(SCENERYARRAY[j].PointC10Y) + abs(deltaY);

		SCENERYARRAY[j].PointD0Y = Swap4Bytes(SCENERYARRAY[j].PointD0Y) + abs(deltaY);
		SCENERYARRAY[j].PointD1Y = Swap4Bytes(SCENERYARRAY[j].PointD1Y) + abs(deltaY);
		SCENERYARRAY[j].PointD2Y = Swap4Bytes(SCENERYARRAY[j].PointD2Y) + abs(deltaY);
		SCENERYARRAY[j].PointD3Y = Swap4Bytes(SCENERYARRAY[j].PointD3Y) + abs(deltaY);
		SCENERYARRAY[j].PointD4Y = Swap4Bytes(SCENERYARRAY[j].PointD4Y) + abs(deltaY);
		SCENERYARRAY[j].PointD5Y = Swap4Bytes(SCENERYARRAY[j].PointD5Y) + abs(deltaY);
		SCENERYARRAY[j].PointD6Y = Swap4Bytes(SCENERYARRAY[j].PointD6Y) + abs(deltaY);
		SCENERYARRAY[j].PointD7Y = Swap4Bytes(SCENERYARRAY[j].PointD7Y) + abs(deltaY);
		SCENERYARRAY[j].PointD8Y = Swap4Bytes(SCENERYARRAY[j].PointD8Y) + abs(deltaY);
		SCENERYARRAY[j].PointD9Y = Swap4Bytes(SCENERYARRAY[j].PointD9Y) + abs(deltaY);
		SCENERYARRAY[j].PointD10Y = Swap4Bytes(SCENERYARRAY[j].PointD10Y) + abs(deltaY);

		SCENERYARRAY[j].PointE0Y = Swap4Bytes(SCENERYARRAY[j].PointE0Y) + abs(deltaY);
		SCENERYARRAY[j].PointE1Y = Swap4Bytes(SCENERYARRAY[j].PointE1Y) + abs(deltaY);
		SCENERYARRAY[j].PointE2Y = Swap4Bytes(SCENERYARRAY[j].PointE2Y) + abs(deltaY);
		SCENERYARRAY[j].PointE3Y = Swap4Bytes(SCENERYARRAY[j].PointE3Y) + abs(deltaY);
		SCENERYARRAY[j].PointE4Y = Swap4Bytes(SCENERYARRAY[j].PointE4Y) + abs(deltaY);
		SCENERYARRAY[j].PointE5Y = Swap4Bytes(SCENERYARRAY[j].PointE5Y) + abs(deltaY);
		SCENERYARRAY[j].PointE6Y = Swap4Bytes(SCENERYARRAY[j].PointE6Y) + abs(deltaY);
		SCENERYARRAY[j].PointE7Y = Swap4Bytes(SCENERYARRAY[j].PointE7Y) + abs(deltaY);
		SCENERYARRAY[j].PointE8Y = Swap4Bytes(SCENERYARRAY[j].PointE8Y) + abs(deltaY);
		SCENERYARRAY[j].PointE9Y = Swap4Bytes(SCENERYARRAY[j].PointE9Y) + abs(deltaY);
		SCENERYARRAY[j].PointE10Y = Swap4Bytes(SCENERYARRAY[j].PointE10Y) + abs(deltaY);
//		cout << "\nj = " << dec << j << endl;
//		cout << "\nSCENERYARRAY[j].PointA0X = 0x" << hex << SCENERYARRAY[j].PointA0X << endl;
		SCENERYARRAY[j].PointA0Y = Swap4Bytes(SCENERYARRAY[j].PointA0Y);
		SCENERYARRAY[j].PointA1Y = Swap4Bytes(SCENERYARRAY[j].PointA1Y);
		SCENERYARRAY[j].PointA2Y = Swap4Bytes(SCENERYARRAY[j].PointA2Y);
		SCENERYARRAY[j].PointA3Y = Swap4Bytes(SCENERYARRAY[j].PointA3Y);
		SCENERYARRAY[j].PointA4Y = Swap4Bytes(SCENERYARRAY[j].PointA4Y);
		SCENERYARRAY[j].PointA5Y = Swap4Bytes(SCENERYARRAY[j].PointA5Y);
		SCENERYARRAY[j].PointA6Y = Swap4Bytes(SCENERYARRAY[j].PointA6Y);
		SCENERYARRAY[j].PointA7Y = Swap4Bytes(SCENERYARRAY[j].PointA7Y);
		SCENERYARRAY[j].PointA8Y = Swap4Bytes(SCENERYARRAY[j].PointA8Y);
		SCENERYARRAY[j].PointA9Y = Swap4Bytes(SCENERYARRAY[j].PointA9Y);
		SCENERYARRAY[j].PointA10Y = Swap4Bytes(SCENERYARRAY[j].PointA10Y);

		SCENERYARRAY[j].PointB0Y = Swap4Bytes(SCENERYARRAY[j].PointB0Y);
		SCENERYARRAY[j].PointB1Y = Swap4Bytes(SCENERYARRAY[j].PointB1Y);
		SCENERYARRAY[j].PointB2Y = Swap4Bytes(SCENERYARRAY[j].PointB2Y);
		SCENERYARRAY[j].PointB3Y = Swap4Bytes(SCENERYARRAY[j].PointB3Y);
		SCENERYARRAY[j].PointB4Y = Swap4Bytes(SCENERYARRAY[j].PointB4Y);
		SCENERYARRAY[j].PointB5Y = Swap4Bytes(SCENERYARRAY[j].PointB5Y);
		SCENERYARRAY[j].PointB6Y = Swap4Bytes(SCENERYARRAY[j].PointB6Y);
		SCENERYARRAY[j].PointB7Y = Swap4Bytes(SCENERYARRAY[j].PointB7Y);
		SCENERYARRAY[j].PointB8Y = Swap4Bytes(SCENERYARRAY[j].PointB8Y);
		SCENERYARRAY[j].PointB9Y = Swap4Bytes(SCENERYARRAY[j].PointB9Y);
		SCENERYARRAY[j].PointB10Y = Swap4Bytes(SCENERYARRAY[j].PointB10Y);

		SCENERYARRAY[j].PointC0Y = Swap4Bytes(SCENERYARRAY[j].PointC0Y);
		SCENERYARRAY[j].PointC1Y = Swap4Bytes(SCENERYARRAY[j].PointC1Y);
		SCENERYARRAY[j].PointC2Y = Swap4Bytes(SCENERYARRAY[j].PointC2Y);
		SCENERYARRAY[j].PointC3Y = Swap4Bytes(SCENERYARRAY[j].PointC3Y);
		SCENERYARRAY[j].PointC4Y = Swap4Bytes(SCENERYARRAY[j].PointC4Y);
		SCENERYARRAY[j].PointC5Y = Swap4Bytes(SCENERYARRAY[j].PointC5Y);
		SCENERYARRAY[j].PointC6Y = Swap4Bytes(SCENERYARRAY[j].PointC6Y);
		SCENERYARRAY[j].PointC7Y = Swap4Bytes(SCENERYARRAY[j].PointC7Y);
		SCENERYARRAY[j].PointC8Y = Swap4Bytes(SCENERYARRAY[j].PointC8Y);
		SCENERYARRAY[j].PointC9Y = Swap4Bytes(SCENERYARRAY[j].PointC9Y);
		SCENERYARRAY[j].PointC10Y = Swap4Bytes(SCENERYARRAY[j].PointC10Y);

		SCENERYARRAY[j].PointD0Y = Swap4Bytes(SCENERYARRAY[j].PointD0Y);
		SCENERYARRAY[j].PointD1Y = Swap4Bytes(SCENERYARRAY[j].PointD1Y);
		SCENERYARRAY[j].PointD2Y = Swap4Bytes(SCENERYARRAY[j].PointD2Y);
		SCENERYARRAY[j].PointD3Y = Swap4Bytes(SCENERYARRAY[j].PointD3Y);
		SCENERYARRAY[j].PointD4Y = Swap4Bytes(SCENERYARRAY[j].PointD4Y);
		SCENERYARRAY[j].PointD5Y = Swap4Bytes(SCENERYARRAY[j].PointD5Y);
		SCENERYARRAY[j].PointD6Y = Swap4Bytes(SCENERYARRAY[j].PointD6Y);
		SCENERYARRAY[j].PointD7Y = Swap4Bytes(SCENERYARRAY[j].PointD7Y);
		SCENERYARRAY[j].PointD8Y = Swap4Bytes(SCENERYARRAY[j].PointD8Y);
		SCENERYARRAY[j].PointD9Y = Swap4Bytes(SCENERYARRAY[j].PointD9Y);
		SCENERYARRAY[j].PointD10Y = Swap4Bytes(SCENERYARRAY[j].PointD10Y);

		SCENERYARRAY[j].PointE0Y = Swap4Bytes(SCENERYARRAY[j].PointE0Y);
		SCENERYARRAY[j].PointE1Y = Swap4Bytes(SCENERYARRAY[j].PointE1Y);
		SCENERYARRAY[j].PointE2Y = Swap4Bytes(SCENERYARRAY[j].PointE2Y);
		SCENERYARRAY[j].PointE3Y = Swap4Bytes(SCENERYARRAY[j].PointE3Y);
		SCENERYARRAY[j].PointE4Y = Swap4Bytes(SCENERYARRAY[j].PointE4Y);
		SCENERYARRAY[j].PointE5Y = Swap4Bytes(SCENERYARRAY[j].PointE5Y);
		SCENERYARRAY[j].PointE6Y = Swap4Bytes(SCENERYARRAY[j].PointE6Y);
		SCENERYARRAY[j].PointE7Y = Swap4Bytes(SCENERYARRAY[j].PointE7Y);
		SCENERYARRAY[j].PointE8Y = Swap4Bytes(SCENERYARRAY[j].PointE8Y);
		SCENERYARRAY[j].PointE9Y = Swap4Bytes(SCENERYARRAY[j].PointE9Y);
		SCENERYARRAY[j].PointE10Y = Swap4Bytes(SCENERYARRAY[j].PointE10Y);
	} else {
		SCENERYARRAY[j].PointA0Y = Swap4Bytes(SCENERYARRAY[j].PointA0Y) - abs(deltaY);
		SCENERYARRAY[j].PointA1Y = Swap4Bytes(SCENERYARRAY[j].PointA1Y) - abs(deltaY);
		SCENERYARRAY[j].PointA2Y = Swap4Bytes(SCENERYARRAY[j].PointA2Y) - abs(deltaY);
		SCENERYARRAY[j].PointA3Y = Swap4Bytes(SCENERYARRAY[j].PointA3Y) - abs(deltaY);
		SCENERYARRAY[j].PointA4Y = Swap4Bytes(SCENERYARRAY[j].PointA4Y) - abs(deltaY);
		SCENERYARRAY[j].PointA5Y = Swap4Bytes(SCENERYARRAY[j].PointA5Y) - abs(deltaY);
		SCENERYARRAY[j].PointA6Y = Swap4Bytes(SCENERYARRAY[j].PointA6Y) - abs(deltaY);
		SCENERYARRAY[j].PointA7Y = Swap4Bytes(SCENERYARRAY[j].PointA7Y) - abs(deltaY);
		SCENERYARRAY[j].PointA8Y = Swap4Bytes(SCENERYARRAY[j].PointA8Y) - abs(deltaY);
		SCENERYARRAY[j].PointA9Y = Swap4Bytes(SCENERYARRAY[j].PointA9Y) - abs(deltaY);
		SCENERYARRAY[j].PointA10Y = Swap4Bytes(SCENERYARRAY[j].PointA10Y) - abs(deltaY);

		SCENERYARRAY[j].PointB0Y = Swap4Bytes(SCENERYARRAY[j].PointB0Y) - abs(deltaY);
		SCENERYARRAY[j].PointB1Y = Swap4Bytes(SCENERYARRAY[j].PointB1Y) - abs(deltaY);
		SCENERYARRAY[j].PointB2Y = Swap4Bytes(SCENERYARRAY[j].PointB2Y) - abs(deltaY);
		SCENERYARRAY[j].PointB3Y = Swap4Bytes(SCENERYARRAY[j].PointB3Y) - abs(deltaY);
		SCENERYARRAY[j].PointB4Y = Swap4Bytes(SCENERYARRAY[j].PointB4Y) - abs(deltaY);
		SCENERYARRAY[j].PointB5Y = Swap4Bytes(SCENERYARRAY[j].PointB5Y) - abs(deltaY);
		SCENERYARRAY[j].PointB6Y = Swap4Bytes(SCENERYARRAY[j].PointB6Y) - abs(deltaY);
		SCENERYARRAY[j].PointB7Y = Swap4Bytes(SCENERYARRAY[j].PointB7Y) - abs(deltaY);
		SCENERYARRAY[j].PointB8Y = Swap4Bytes(SCENERYARRAY[j].PointB8Y) - abs(deltaY);
		SCENERYARRAY[j].PointB9Y = Swap4Bytes(SCENERYARRAY[j].PointB9Y) - abs(deltaY);
		SCENERYARRAY[j].PointB10Y = Swap4Bytes(SCENERYARRAY[j].PointB10Y) - abs(deltaY);

		SCENERYARRAY[j].PointC0Y = Swap4Bytes(SCENERYARRAY[j].PointC0Y) - abs(deltaY);
		SCENERYARRAY[j].PointC1Y = Swap4Bytes(SCENERYARRAY[j].PointC1Y) - abs(deltaY);
		SCENERYARRAY[j].PointC2Y = Swap4Bytes(SCENERYARRAY[j].PointC2Y) - abs(deltaY);
		SCENERYARRAY[j].PointC3Y = Swap4Bytes(SCENERYARRAY[j].PointC3Y) - abs(deltaY);
		SCENERYARRAY[j].PointC4Y = Swap4Bytes(SCENERYARRAY[j].PointC4Y) - abs(deltaY);
		SCENERYARRAY[j].PointC5Y = Swap4Bytes(SCENERYARRAY[j].PointC5Y) - abs(deltaY);
		SCENERYARRAY[j].PointC6Y = Swap4Bytes(SCENERYARRAY[j].PointC6Y) - abs(deltaY);
		SCENERYARRAY[j].PointC7Y = Swap4Bytes(SCENERYARRAY[j].PointC7Y) - abs(deltaY);
		SCENERYARRAY[j].PointC8Y = Swap4Bytes(SCENERYARRAY[j].PointC8Y) - abs(deltaY);
		SCENERYARRAY[j].PointC9Y = Swap4Bytes(SCENERYARRAY[j].PointC9Y) - abs(deltaY);
		SCENERYARRAY[j].PointC10Y = Swap4Bytes(SCENERYARRAY[j].PointC10Y) - abs(deltaY);

		SCENERYARRAY[j].PointD0Y = Swap4Bytes(SCENERYARRAY[j].PointD0Y) - abs(deltaY);
		SCENERYARRAY[j].PointD1Y = Swap4Bytes(SCENERYARRAY[j].PointD1Y) - abs(deltaY);
		SCENERYARRAY[j].PointD2Y = Swap4Bytes(SCENERYARRAY[j].PointD2Y) - abs(deltaY);
		SCENERYARRAY[j].PointD3Y = Swap4Bytes(SCENERYARRAY[j].PointD3Y) - abs(deltaY);
		SCENERYARRAY[j].PointD4Y = Swap4Bytes(SCENERYARRAY[j].PointD4Y) - abs(deltaY);
		SCENERYARRAY[j].PointD5Y = Swap4Bytes(SCENERYARRAY[j].PointD5Y) - abs(deltaY);
		SCENERYARRAY[j].PointD6Y = Swap4Bytes(SCENERYARRAY[j].PointD6Y) - abs(deltaY);
		SCENERYARRAY[j].PointD7Y = Swap4Bytes(SCENERYARRAY[j].PointD7Y) - abs(deltaY);
		SCENERYARRAY[j].PointD8Y = Swap4Bytes(SCENERYARRAY[j].PointD8Y) - abs(deltaY);
		SCENERYARRAY[j].PointD9Y = Swap4Bytes(SCENERYARRAY[j].PointD9Y) - abs(deltaY);
		SCENERYARRAY[j].PointD10Y = Swap4Bytes(SCENERYARRAY[j].PointD10Y) - abs(deltaY);

		SCENERYARRAY[j].PointE0Y = Swap4Bytes(SCENERYARRAY[j].PointE0Y) - abs(deltaY);
		SCENERYARRAY[j].PointE1Y = Swap4Bytes(SCENERYARRAY[j].PointE1Y) - abs(deltaY);
		SCENERYARRAY[j].PointE2Y = Swap4Bytes(SCENERYARRAY[j].PointE2Y) - abs(deltaY);
		SCENERYARRAY[j].PointE3Y = Swap4Bytes(SCENERYARRAY[j].PointE3Y) - abs(deltaY);
		SCENERYARRAY[j].PointE4Y = Swap4Bytes(SCENERYARRAY[j].PointE4Y) - abs(deltaY);
		SCENERYARRAY[j].PointE5Y = Swap4Bytes(SCENERYARRAY[j].PointE5Y) - abs(deltaY);
		SCENERYARRAY[j].PointE6Y = Swap4Bytes(SCENERYARRAY[j].PointE6Y) - abs(deltaY);
		SCENERYARRAY[j].PointE7Y = Swap4Bytes(SCENERYARRAY[j].PointE7Y) - abs(deltaY);
		SCENERYARRAY[j].PointE8Y = Swap4Bytes(SCENERYARRAY[j].PointE8Y) - abs(deltaY);
		SCENERYARRAY[j].PointE9Y = Swap4Bytes(SCENERYARRAY[j].PointE9Y) - abs(deltaY);
		SCENERYARRAY[j].PointE10Y = Swap4Bytes(SCENERYARRAY[j].PointE10Y) - abs(deltaY);
		////////// Swap for file writing
		SCENERYARRAY[j].PointA0Y = Swap4Bytes(SCENERYARRAY[j].PointA0Y);
		SCENERYARRAY[j].PointA1Y = Swap4Bytes(SCENERYARRAY[j].PointA1Y);
		SCENERYARRAY[j].PointA2Y = Swap4Bytes(SCENERYARRAY[j].PointA2Y);
		SCENERYARRAY[j].PointA3Y = Swap4Bytes(SCENERYARRAY[j].PointA3Y);
		SCENERYARRAY[j].PointA4Y = Swap4Bytes(SCENERYARRAY[j].PointA4Y);
		SCENERYARRAY[j].PointA5Y = Swap4Bytes(SCENERYARRAY[j].PointA5Y);
		SCENERYARRAY[j].PointA6Y = Swap4Bytes(SCENERYARRAY[j].PointA6Y);
		SCENERYARRAY[j].PointA7Y = Swap4Bytes(SCENERYARRAY[j].PointA7Y);
		SCENERYARRAY[j].PointA8Y = Swap4Bytes(SCENERYARRAY[j].PointA8Y);
		SCENERYARRAY[j].PointA9Y = Swap4Bytes(SCENERYARRAY[j].PointA9Y);
		SCENERYARRAY[j].PointA10Y = Swap4Bytes(SCENERYARRAY[j].PointA10Y);

		SCENERYARRAY[j].PointB0Y = Swap4Bytes(SCENERYARRAY[j].PointB0Y);
		SCENERYARRAY[j].PointB1Y = Swap4Bytes(SCENERYARRAY[j].PointB1Y);
		SCENERYARRAY[j].PointB2Y = Swap4Bytes(SCENERYARRAY[j].PointB2Y);
		SCENERYARRAY[j].PointB3Y = Swap4Bytes(SCENERYARRAY[j].PointB3Y);
		SCENERYARRAY[j].PointB4Y = Swap4Bytes(SCENERYARRAY[j].PointB4Y);
		SCENERYARRAY[j].PointB5Y = Swap4Bytes(SCENERYARRAY[j].PointB5Y);
		SCENERYARRAY[j].PointB6Y = Swap4Bytes(SCENERYARRAY[j].PointB6Y);
		SCENERYARRAY[j].PointB7Y = Swap4Bytes(SCENERYARRAY[j].PointB7Y);
		SCENERYARRAY[j].PointB8Y = Swap4Bytes(SCENERYARRAY[j].PointB8Y);
		SCENERYARRAY[j].PointB9Y = Swap4Bytes(SCENERYARRAY[j].PointB9Y);
		SCENERYARRAY[j].PointB10Y = Swap4Bytes(SCENERYARRAY[j].PointB10Y);

		SCENERYARRAY[j].PointC0Y = Swap4Bytes(SCENERYARRAY[j].PointC0Y);
		SCENERYARRAY[j].PointC1Y = Swap4Bytes(SCENERYARRAY[j].PointC1Y);
		SCENERYARRAY[j].PointC2Y = Swap4Bytes(SCENERYARRAY[j].PointC2Y);
		SCENERYARRAY[j].PointC3Y = Swap4Bytes(SCENERYARRAY[j].PointC3Y);
		SCENERYARRAY[j].PointC4Y = Swap4Bytes(SCENERYARRAY[j].PointC4Y);
		SCENERYARRAY[j].PointC5Y = Swap4Bytes(SCENERYARRAY[j].PointC5Y);
		SCENERYARRAY[j].PointC6Y = Swap4Bytes(SCENERYARRAY[j].PointC6Y);
		SCENERYARRAY[j].PointC7Y = Swap4Bytes(SCENERYARRAY[j].PointC7Y);
		SCENERYARRAY[j].PointC8Y = Swap4Bytes(SCENERYARRAY[j].PointC8Y);
		SCENERYARRAY[j].PointC9Y = Swap4Bytes(SCENERYARRAY[j].PointC9Y);
		SCENERYARRAY[j].PointC10Y = Swap4Bytes(SCENERYARRAY[j].PointC10Y);

		SCENERYARRAY[j].PointD0Y = Swap4Bytes(SCENERYARRAY[j].PointD0Y);
		SCENERYARRAY[j].PointD1Y = Swap4Bytes(SCENERYARRAY[j].PointD1Y);
		SCENERYARRAY[j].PointD2Y = Swap4Bytes(SCENERYARRAY[j].PointD2Y);
		SCENERYARRAY[j].PointD3Y = Swap4Bytes(SCENERYARRAY[j].PointD3Y);
		SCENERYARRAY[j].PointD4Y = Swap4Bytes(SCENERYARRAY[j].PointD4Y);
		SCENERYARRAY[j].PointD5Y = Swap4Bytes(SCENERYARRAY[j].PointD5Y);
		SCENERYARRAY[j].PointD6Y = Swap4Bytes(SCENERYARRAY[j].PointD6Y);
		SCENERYARRAY[j].PointD7Y = Swap4Bytes(SCENERYARRAY[j].PointD7Y);
		SCENERYARRAY[j].PointD8Y = Swap4Bytes(SCENERYARRAY[j].PointD8Y);
		SCENERYARRAY[j].PointD9Y = Swap4Bytes(SCENERYARRAY[j].PointD9Y);
		SCENERYARRAY[j].PointD10Y = Swap4Bytes(SCENERYARRAY[j].PointD10Y);

		SCENERYARRAY[j].PointE0Y = Swap4Bytes(SCENERYARRAY[j].PointE0Y);
		SCENERYARRAY[j].PointE1Y = Swap4Bytes(SCENERYARRAY[j].PointE1Y);
		SCENERYARRAY[j].PointE2Y = Swap4Bytes(SCENERYARRAY[j].PointE2Y);
		SCENERYARRAY[j].PointE3Y = Swap4Bytes(SCENERYARRAY[j].PointE3Y);
		SCENERYARRAY[j].PointE4Y = Swap4Bytes(SCENERYARRAY[j].PointE4Y);
		SCENERYARRAY[j].PointE5Y = Swap4Bytes(SCENERYARRAY[j].PointE5Y);
		SCENERYARRAY[j].PointE6Y = Swap4Bytes(SCENERYARRAY[j].PointE6Y);
		SCENERYARRAY[j].PointE7Y = Swap4Bytes(SCENERYARRAY[j].PointE7Y);
		SCENERYARRAY[j].PointE8Y = Swap4Bytes(SCENERYARRAY[j].PointE8Y);
		SCENERYARRAY[j].PointE9Y = Swap4Bytes(SCENERYARRAY[j].PointE9Y);
		SCENERYARRAY[j].PointE10Y = Swap4Bytes(SCENERYARRAY[j].PointE10Y);
		}
// Flip points. tmpE=A, A=E, E=tmpE,    tmpD=B, B=D, D=tmpD.
#if 0;
int tmpE0X, tmpE0Z, tmpE0Y, tmpE1X, tmpE1Z, tmpE1Y, tmpE2X, tmpE2Z, tmpE2Y, tmpE3X, tmpE3Z, tmpE3Y, tmpE4X, tmpE4Z, tmpE4Y, tmpE5X, tmpE5Z, tmpE5Y, tmpE6X, tmpE6Z, tmpE6Y, tmpE7X, tmpE7Z, tmpE7Y, tmpE8X, tmpE8Z, tmpE8Y, tmpE9X, tmpE9Z, tmpE9Y, tmpE10X, tmpE10Z, tmpE10Y;
int tmpD0X, tmpD0Z, tmpD0Y, tmpD1X, tmpD1Z, tmpD1Y, tmpD2X, tmpD2Z, tmpD2Y, tmpD3X, tmpD3Z, tmpD3Y, tmpD4X, tmpD4Z, tmpD4Y, tmpD5X, tmpD5Z, tmpD5Y, tmpD6X, tmpD6Z, tmpD6Y, tmpD7X, tmpD7Z, tmpD7Y, tmpD8X, tmpD8Z, tmpD8Y, tmpD9X, tmpD9Z, tmpD9Y, tmpD10X, tmpD10Z, tmpD10Y;

#if 0;
// A=E
tmpE0X = SCENERYARRAY[j].PointA0X;
tmpE0Z = SCENERYARRAY[j].PointA0Z;
tmpE0Y = SCENERYARRAY[j].PointA0Y;
tmpE1X = SCENERYARRAY[j].PointA1X;
tmpE1Z = SCENERYARRAY[j].PointA1Z;
tmpE1Y = SCENERYARRAY[j].PointA1Y;
tmpE2X = SCENERYARRAY[j].PointA2X;
tmpE2Z = SCENERYARRAY[j].PointA2Z;
tmpE2Y = SCENERYARRAY[j].PointA2Y;
tmpE3X = SCENERYARRAY[j].PointA3X;
tmpE3Z = SCENERYARRAY[j].PointA3Z;
tmpE3Y = SCENERYARRAY[j].PointA3Y;
tmpE4X = SCENERYARRAY[j].PointA4X;
tmpE4Z = SCENERYARRAY[j].PointA4Z;
tmpE4Y = SCENERYARRAY[j].PointA4Y;
tmpE5X = SCENERYARRAY[j].PointA5X;
tmpE5Z = SCENERYARRAY[j].PointA5Z;
tmpE5Y = SCENERYARRAY[j].PointA5Y;
tmpE6X = SCENERYARRAY[j].PointA6X;
tmpE6Z = SCENERYARRAY[j].PointA6Z;
tmpE6Y = SCENERYARRAY[j].PointA6Y;
tmpE7X = SCENERYARRAY[j].PointA7X;
tmpE7Z = SCENERYARRAY[j].PointA7Z;
tmpE7Y = SCENERYARRAY[j].PointA7Y;
tmpE8X = SCENERYARRAY[j].PointA8X;
tmpE8Z = SCENERYARRAY[j].PointA8Z;
tmpE8Y = SCENERYARRAY[j].PointA8Y;
tmpE9X = SCENERYARRAY[j].PointA9X;
tmpE9Z = SCENERYARRAY[j].PointA9Z;
tmpE9Y = SCENERYARRAY[j].PointA9Y;
tmpE10X = SCENERYARRAY[j].PointA10X;
tmpE10Z = SCENERYARRAY[j].PointA10Z;
tmpE10Y = SCENERYARRAY[j].PointA10Y;

SCENERYARRAY[j].PointA0X = SCENERYARRAY[j].PointE0X;
SCENERYARRAY[j].PointA0Z = SCENERYARRAY[j].PointE0Z;
SCENERYARRAY[j].PointA0Y = SCENERYARRAY[j].PointE0Y;
SCENERYARRAY[j].PointA1X = SCENERYARRAY[j].PointE1X;
SCENERYARRAY[j].PointA1Z = SCENERYARRAY[j].PointE1Z;
SCENERYARRAY[j].PointA1Y = SCENERYARRAY[j].PointE1Y;
SCENERYARRAY[j].PointA2X = SCENERYARRAY[j].PointE2X;
SCENERYARRAY[j].PointA2Z = SCENERYARRAY[j].PointE2Z;
SCENERYARRAY[j].PointA2Y = SCENERYARRAY[j].PointE2Y;
SCENERYARRAY[j].PointA3X = SCENERYARRAY[j].PointE3X;
SCENERYARRAY[j].PointA3Z = SCENERYARRAY[j].PointE3Z;
SCENERYARRAY[j].PointA3Y = SCENERYARRAY[j].PointE3Y;
SCENERYARRAY[j].PointA4X = SCENERYARRAY[j].PointE4X;
SCENERYARRAY[j].PointA4Z = SCENERYARRAY[j].PointE4Z;
SCENERYARRAY[j].PointA4Y = SCENERYARRAY[j].PointE4Y;
SCENERYARRAY[j].PointA5X = SCENERYARRAY[j].PointE5X;
SCENERYARRAY[j].PointA5Z = SCENERYARRAY[j].PointE5Z;
SCENERYARRAY[j].PointA5Y = SCENERYARRAY[j].PointE5Y;
SCENERYARRAY[j].PointA6X = SCENERYARRAY[j].PointE6X;
SCENERYARRAY[j].PointA6Z = SCENERYARRAY[j].PointE6Z;
SCENERYARRAY[j].PointA6Y = SCENERYARRAY[j].PointE6Y;
SCENERYARRAY[j].PointA7X = SCENERYARRAY[j].PointE7X;
SCENERYARRAY[j].PointA7Z = SCENERYARRAY[j].PointE7Z;
SCENERYARRAY[j].PointA7Y = SCENERYARRAY[j].PointE7Y;
SCENERYARRAY[j].PointA8X = SCENERYARRAY[j].PointE8X;
SCENERYARRAY[j].PointA8Z = SCENERYARRAY[j].PointE8Z;
SCENERYARRAY[j].PointA8Y = SCENERYARRAY[j].PointE8Y;
SCENERYARRAY[j].PointA9X = SCENERYARRAY[j].PointE9X;
SCENERYARRAY[j].PointA9Z = SCENERYARRAY[j].PointE9Z;
SCENERYARRAY[j].PointA9Y = SCENERYARRAY[j].PointE9Y;
SCENERYARRAY[j].PointA10X = SCENERYARRAY[j].PointE10X;
SCENERYARRAY[j].PointA10Z = SCENERYARRAY[j].PointE10Z;
SCENERYARRAY[j].PointA10Y = SCENERYARRAY[j].PointE10Y;

SCENERYARRAY[j].PointE0X = tmpE0X;
SCENERYARRAY[j].PointE0Z = tmpE0Z;
SCENERYARRAY[j].PointE0Y = tmpE0Y;
SCENERYARRAY[j].PointE1X = tmpE1X;
SCENERYARRAY[j].PointE1Z = tmpE1Z;
SCENERYARRAY[j].PointE1Y = tmpE1Y;
SCENERYARRAY[j].PointE2X = tmpE2X;
SCENERYARRAY[j].PointE2Z = tmpE2Z;
SCENERYARRAY[j].PointE2Y = tmpE2Y;
SCENERYARRAY[j].PointE3X = tmpE3X;
SCENERYARRAY[j].PointE3Z = tmpE3Z;
SCENERYARRAY[j].PointE3Y = tmpE3Y;
SCENERYARRAY[j].PointE4X = tmpE4X;
SCENERYARRAY[j].PointE4Z = tmpE4Z;
SCENERYARRAY[j].PointE4Y = tmpE4Y;
SCENERYARRAY[j].PointE5X = tmpE5X;
SCENERYARRAY[j].PointE5Z = tmpE5Z;
SCENERYARRAY[j].PointE5Y = tmpE5Y;
SCENERYARRAY[j].PointE6X = tmpE6X;
SCENERYARRAY[j].PointE6Z = tmpE6Z;
SCENERYARRAY[j].PointE6Y = tmpE6Y;
SCENERYARRAY[j].PointE7X = tmpE7X;
SCENERYARRAY[j].PointE7Z = tmpE7Z;
SCENERYARRAY[j].PointE7Y = tmpE7Y;
SCENERYARRAY[j].PointE8X = tmpE8X;
SCENERYARRAY[j].PointE8Z = tmpE8Z;
SCENERYARRAY[j].PointE8Y = tmpE8Y;
SCENERYARRAY[j].PointE9X = tmpE9X;
SCENERYARRAY[j].PointE9Z = tmpE9Z;
SCENERYARRAY[j].PointE9Y = tmpE9Y;
SCENERYARRAY[j].PointE10X = tmpE10X;
SCENERYARRAY[j].PointE10Z = tmpE10Z;
SCENERYARRAY[j].PointE10Y = tmpE10Y;
#endif;

// B=D
// tmpD=B, B=D, D=tmpD.
tmpD0X = SCENERYARRAY[j].PointB0X;
tmpD0Z = SCENERYARRAY[j].PointB0Z;
tmpD0Y = SCENERYARRAY[j].PointB0Y;
tmpD1X = SCENERYARRAY[j].PointB1X;
tmpD1Z = SCENERYARRAY[j].PointB1Z;
tmpD1Y = SCENERYARRAY[j].PointB1Y;
tmpD2X = SCENERYARRAY[j].PointB2X;
tmpD2Z = SCENERYARRAY[j].PointB2Z;
tmpD2Y = SCENERYARRAY[j].PointB2Y;
tmpD3X = SCENERYARRAY[j].PointB3X;
tmpD3Z = SCENERYARRAY[j].PointB3Z;
tmpD3Y = SCENERYARRAY[j].PointB3Y;
tmpD4X = SCENERYARRAY[j].PointB4X;
tmpD4Z = SCENERYARRAY[j].PointB4Z;
tmpD4Y = SCENERYARRAY[j].PointB4Y;
tmpD5X = SCENERYARRAY[j].PointB5X;
tmpD5Z = SCENERYARRAY[j].PointB5Z;
tmpD5Y = SCENERYARRAY[j].PointB5Y;
tmpD6X = SCENERYARRAY[j].PointB6X;
tmpD6Z = SCENERYARRAY[j].PointB6Z;
tmpD6Y = SCENERYARRAY[j].PointB6Y;
tmpD7X = SCENERYARRAY[j].PointB7X;
tmpD7Z = SCENERYARRAY[j].PointB7Z;
tmpD7Y = SCENERYARRAY[j].PointB7Y;
tmpD8X = SCENERYARRAY[j].PointB8X;
tmpD8Z = SCENERYARRAY[j].PointB8Z;
tmpD8Y = SCENERYARRAY[j].PointB8Y;
tmpD9X = SCENERYARRAY[j].PointB9X;
tmpD9Z = SCENERYARRAY[j].PointB9Z;
tmpD9Y = SCENERYARRAY[j].PointB9Y;
tmpD10X = SCENERYARRAY[j].PointB10X;
tmpD10Z = SCENERYARRAY[j].PointB10Z;
tmpD10Y = SCENERYARRAY[j].PointB10Y;

SCENERYARRAY[j].PointB0X = SCENERYARRAY[j].PointD0X;
SCENERYARRAY[j].PointB0Z = SCENERYARRAY[j].PointD0Z;
SCENERYARRAY[j].PointB0Y = SCENERYARRAY[j].PointD0Y;
SCENERYARRAY[j].PointB1X = SCENERYARRAY[j].PointD1X;
SCENERYARRAY[j].PointB1Z = SCENERYARRAY[j].PointD1Z;
SCENERYARRAY[j].PointB1Y = SCENERYARRAY[j].PointD1Y;
SCENERYARRAY[j].PointB2X = SCENERYARRAY[j].PointD2X;
SCENERYARRAY[j].PointB2Z = SCENERYARRAY[j].PointD2Z;
SCENERYARRAY[j].PointB2Y = SCENERYARRAY[j].PointD2Y;
SCENERYARRAY[j].PointB3X = SCENERYARRAY[j].PointD3X;
SCENERYARRAY[j].PointB3Z = SCENERYARRAY[j].PointD3Z;
SCENERYARRAY[j].PointB3Y = SCENERYARRAY[j].PointD3Y;
SCENERYARRAY[j].PointB4X = SCENERYARRAY[j].PointD4X;
SCENERYARRAY[j].PointB4Z = SCENERYARRAY[j].PointD4Z;
SCENERYARRAY[j].PointB4Y = SCENERYARRAY[j].PointD4Y;
SCENERYARRAY[j].PointB5X = SCENERYARRAY[j].PointD5X;
SCENERYARRAY[j].PointB5Z = SCENERYARRAY[j].PointD5Z;
SCENERYARRAY[j].PointB5Y = SCENERYARRAY[j].PointD5Y;
SCENERYARRAY[j].PointB6X = SCENERYARRAY[j].PointD6X;
SCENERYARRAY[j].PointB6Z = SCENERYARRAY[j].PointD6Z;
SCENERYARRAY[j].PointB6Y = SCENERYARRAY[j].PointD6Y;
SCENERYARRAY[j].PointB7X = SCENERYARRAY[j].PointD7X;
SCENERYARRAY[j].PointB7Z = SCENERYARRAY[j].PointD7Z;
SCENERYARRAY[j].PointB7Y = SCENERYARRAY[j].PointD7Y;
SCENERYARRAY[j].PointB8X = SCENERYARRAY[j].PointD8X;
SCENERYARRAY[j].PointB8Z = SCENERYARRAY[j].PointD8Z;
SCENERYARRAY[j].PointB8Y = SCENERYARRAY[j].PointD8Y;
SCENERYARRAY[j].PointB9X = SCENERYARRAY[j].PointD9X;
SCENERYARRAY[j].PointB9Z = SCENERYARRAY[j].PointD9Z;
SCENERYARRAY[j].PointB9Y = SCENERYARRAY[j].PointD9Y;
SCENERYARRAY[j].PointB10X = SCENERYARRAY[j].PointD10X;
SCENERYARRAY[j].PointB10Z = SCENERYARRAY[j].PointD10Z;
SCENERYARRAY[j].PointB10Y = SCENERYARRAY[j].PointD10Y;

SCENERYARRAY[j].PointD0X = tmpD0X;
SCENERYARRAY[j].PointD0Z = tmpD0Z;
SCENERYARRAY[j].PointD0Y = tmpD0Y;
SCENERYARRAY[j].PointD1X = tmpD1X;
SCENERYARRAY[j].PointD1Z = tmpD1Z;
SCENERYARRAY[j].PointD1Y = tmpD1Y;
SCENERYARRAY[j].PointD2X = tmpD2X;
SCENERYARRAY[j].PointD2Z = tmpD2Z;
SCENERYARRAY[j].PointD2Y = tmpD2Y;
SCENERYARRAY[j].PointD3X = tmpD3X;
SCENERYARRAY[j].PointD3Z = tmpD3Z;
SCENERYARRAY[j].PointD3Y = tmpD3Y;
SCENERYARRAY[j].PointD4X = tmpD4X;
SCENERYARRAY[j].PointD4Z = tmpD4Z;
SCENERYARRAY[j].PointD4Y = tmpD4Y;
SCENERYARRAY[j].PointD5X = tmpD5X;
SCENERYARRAY[j].PointD5Z = tmpD5Z;
SCENERYARRAY[j].PointD5Y = tmpD5Y;
SCENERYARRAY[j].PointD6X = tmpD6X;
SCENERYARRAY[j].PointD6Z = tmpD6Z;
SCENERYARRAY[j].PointD6Y = tmpD6Y;
SCENERYARRAY[j].PointD7X = tmpD7X;
SCENERYARRAY[j].PointD7Z = tmpD7Z;
SCENERYARRAY[j].PointD7Y = tmpD7Y;
SCENERYARRAY[j].PointD8X = tmpD8X;
SCENERYARRAY[j].PointD8Z = tmpD8Z;
SCENERYARRAY[j].PointD8Y = tmpD8Y;
SCENERYARRAY[j].PointD9X = tmpD9X;
SCENERYARRAY[j].PointD9Z = tmpD9Z;
SCENERYARRAY[j].PointD9Y = tmpD9Y;
SCENERYARRAY[j].PointD10X = tmpD10X;
SCENERYARRAY[j].PointD10Z = tmpD10Z;
SCENERYARRAY[j].PointD10Y = tmpD10Y;
#endif;
// END of flipping points.
}
#endif; // TRKD coords conversion end
}

////////////////////////////////////
//**************
////////////////////////////////////
// Main TRKD loop. There are numSceneryIndexes entries
//bool straight = true;

if (straight) {

for (int i=0; i<numSceneryIndexes; i++) {/// Turn this on to straight save
//for (int i=numSceneryIndexes-1; i>=0; i--) {

//	cout << "i = 0x" << hex << i << endl;
//for (int i=numSceneryIndexes-1; i>=numSceneryIndexes-10; i--) {


// Write unknown points.
float xx, yy, zz;

//xx = (float)SCENERYARRAY[i].refpointX24/SCALE;
//zz = (float)SCENERYARRAY[i].refpointZ24/SCALE; 
//yy = (float)SCENERYARRAY[i].refpointY24/SCALE;

//fprintf(refFile, "v ");
//fprintf(refFile, "%f %f %f\n", (float)xx, (float)zz, -(float)yy);

xx = (float)SCENERYARRAY[i].refpointXB4/SCALE;
zz = (float)SCENERYARRAY[i].refpointZB4/SCALE;
yy = (float)SCENERYARRAY[i].refpointYB4/SCALE;

//fprintf(refFile, "v ");
//fprintf(refFile, "%f %f %f\n", (float)xx, (float)zz, -(float)yy);

//xx = (float)SCENERYARRAY[i].refpointX144/SCALE;
//zz = (float)SCENERYARRAY[i].refpointZ144/SCALE;
//yy = (float)SCENERYARRAY[i].refpointY144/SCALE;

//fprintf(refFile, "v ");
//fprintf(refFile, "%f %f %f\n", (float)xx, (float)zz, -(float)yy);

//xx = (float)SCENERYARRAY[i].refpointX1D4/SCALE;
//zz = (float)SCENERYARRAY[i].refpointZ1D4/SCALE;
//yy = (float)SCENERYARRAY[i].refpointY1D4/SCALE;

fprintf(refFile, "v ");
fprintf(refFile, "%f %f %f\n", (float)xx, (float)zz, -(float)yy);

char trkd[40];
int temp1; 

	if (straight) { 
	temp1 = i;/// Turn this on to straight save
	} else {
	temp1 = numSceneryIndexes-i-1; /// Turn this on to backwards save FIX????????
	}

itoa(temp1, trkd, 10);
strcpy(path, "\Inverted_track\\TRKDs");
strcat(path, trkd);
invtrackfile = fopen(path, "wb");

strcpy(path3, "\Inverted_track\\TRKDheaders");
char trkdN[40];
itoa(temp1, trkdN, 10);
strcat(path3, trkdN);
invtrackfile3 = fopen(path3, "wb");


////////////////////// MAIN COORDS/////////////////
#if 1;
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].RawFence; // Fence. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);
#if 1; 
				if (i>100) {// CHECK and FIX if necessary for all tracks!!!
			bytes4 = SCENERYARRAY[i-1].PointE0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);			
				} else {
			bytes4 = SCENERYARRAY[i].PointA0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
				}
				// Write mystic 12 bytes.////////// FIND OUT WHAT IS IT!
			bytes4 = 0;
//			bytes4 = SCENERYARRAY[i].PointE0X;
			fwrite(&bytes4, 4, 1, invtrackfile);
//			bytes4 = SCENERYARRAY[i].PointE0Z;
			fwrite(&bytes4, 4, 1, invtrackfile);
//			bytes4 = SCENERYARRAY[i].PointE0Y;
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA0X;  // Write A points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0xB4 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB0X;  // Write B points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0x144 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC0X;  			// Write C points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0x1D4 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD0X;  			// Write D points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#endif; 
#endif;
////////////////////// MAIN COORDS END //////////////




// Start subloop here. It's TRACKDEPTH entries. Size 0x258
#if 1;
			for (int j=0; j<TRACKDEPTH; j++) {

			byte1 = 0; // subloop start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].RawFence; // Fence. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);
#if 1; 
			if (i>100) {// CHECK and FIX if necessary for all tracks!!!
			bytes4 = SCENERYARRAY[i+j].PointE0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+j].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+j].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);			
				} else {
				
			bytes4 = SCENERYARRAY[i+1+j].PointA0X;  // Write reference point (A0=E0 point?)
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			}
				// Write mystic 12 bytes.////////// FIND OUT WHAT IS IT!
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA0X;  // Write A=E points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);
//#if 0;
			bytes4 = SCENERYARRAY[i+1+j].PointB0X;  // Write B points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC0X;  			// Write C points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD0X;  			// Write D points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#if 0;
			bytes4 = SCENERYARRAY[i].PointE0X;  			// Write E points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#endif; // E10
#endif; 
			}
#endif;
/// Subloop ends here!

	
#if 1;
	
int size9C = 5 * 0x9C;
testagain:
	//		filler = size2CCB - 0x50 - (TRACKDEPTH+1) * 0x258 - 0xC;
		filler = size2CCB - 0x50 - (TRACKDEPTH+1) * 0x258 - size9C - 0xC;

		if (filler < 0) {
			size2CCB += 0x100;
			goto testagain;
			}
//		filler = size2CCB - 0x50 - lengthTRKD;
//				cout << "i = 0x" << hex << i << endl;
//				cout << "filler = 0x" << hex << filler << endl;
	//			cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;

	// Write 5 0x9C blocks
	// Write block 1
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence//
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile); // =0

			bytes4 = SCENERYARRAY[i+1].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 2
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture1; // Texture1
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile); //=0

			bytes4 = SCENERYARRAY[i].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 3
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-1].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 4
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-2].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);


	// Write block 5
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence//
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-3].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

// Fill the rest before 80 bytes before CCB with zeroes.
			bytes4 = 0;
			for (int jw=0; jw < (filler/4); jw++) {
			fwrite(&bytes4, 4, 1, invtrackfile);
			}


///////////************************//////////////////
	// Write CCB here and calculate it's CCBsize for the header below
	// Load corresponding cel file from TRDKcels3DO folder
				// Generate filename
char tmppath[40] = {0};
char celN[40] = {0};
int tmpvalue = 0;
FILE *ccbFile;

CCBsize = 0;
				itoa(i, celN, 10); 
				strcpy(tmppath, "\TRDKcels3DO\\TRKD");
				strcat(tmppath, celN);

	// Write 80 bytes here
//				if ((ccbFile = fopen(tmppath, "rb")) == false) {
//			bytes4 = 0;	
//				} else {
//			bytes4 = 0x11111111;	// FIX!!!!!!!!!!!!!!!!!. ALSO if 0x2190 - filler is less than 0x50?
//				}
//			for (int jw1=0; jw1 < 20; jw1++) {
//			fwrite(&bytes4, 4, 1, invtrackfile);
//			}

				// Open file for reading
				if (ccbFile = fopen(tmppath, "rb")) {
					fseek(ccbFile, 0, SEEK_END);
					CCBsize = ftell(ccbFile);
//					cout << "CCBsize " << CCBsize <<"\n"; // Just for test
					fseek(ccbFile,0,SEEK_SET);

	// Write 80 bytes here. MAKE for backwards!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int	refPX = SCENERYARRAY[i+12].PointA0X; 
		int	refPZ = SCENERYARRAY[i+12].PointA0Z; 
		int	refPY = SCENERYARRAY[i+12].PointA0Y;

		int	next1X = SCENERYARRAY[i+20].PointA6X; 
		int	next1Z = SCENERYARRAY[i+20].PointA6Z; 
		int	next1Y = SCENERYARRAY[i+20].PointA6Y;

		int	next2X = SCENERYARRAY[i+22].PointA0X;
		int	next2Z = SCENERYARRAY[i+22].PointA0Z; 
		int	next2Y = SCENERYARRAY[i+22].PointA0Y;
		int ccb = Swap4Bytes(CCBsize);
		CCBsize = Swap4Bytes(CCBsize);

				if ((ccbFile = fopen(tmppath, "rb")) == false) {
			bytes4 = 0;	
			for (int jw1=0; jw1 < 20; jw1++) {
			fwrite(&bytes4, 4, 1, invtrackfile);
			}
				} else {
#if 1;
				{		
			fwrite(&refPX, 4, 1, invtrackfile);
			fwrite(&refPZ, 4, 1, invtrackfile); // Reference point A0 i+12
			fwrite(&refPY, 4, 1, invtrackfile);

			fwrite(&next1X, 4, 1, invtrackfile); 
			fwrite(&next1Z, 4, 1, invtrackfile);//A6 i+20
			fwrite(&next1Y, 4, 1, invtrackfile);

			fwrite(&next2X, 4, 1, invtrackfile);
			fwrite(&next2Z, 4, 1, invtrackfile);//A0 i+22
			fwrite(&next2Y, 4, 1, invtrackfile);

			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);// = 0
			fwrite(&bytes4, 4, 1, invtrackfile);
			
//				bytes4 = 0x11111111; //////////////////// MAKE!!!!!!!
//				bytes4 = 0xFF1000; //X?
//				bytes2 = 0x1000; //X?
				bytes4 = 0x104E00; 
//			fwrite(&bytes2, 2, 1, invtrackfile);
//				bytes2 = 0x004E; //X?
//			fwrite(&bytes2, 2, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

//				bytes4 = 0xF01D00; //Y?
//				bytes2 = 0xF000; //Y?
				bytes4 = 0xE02D00; 
//			fwrite(&bytes2, 2, 1, invtrackfile);
//				bytes2 = 0x0050; //Y?
//			fwrite(&bytes2, 2, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

				bytes4 = 0x11111111; 
//				bytes4 = 0xE02000; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0xC02D00; 
//				bytes4 = 0xC02D00; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0xC01A00; 
//				bytes4 = 0xC01A00; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0x302E00; 
//				bytes4 = 0x302E00; 
			fwrite(&bytes4, 4, 1, invtrackfile);

				bytes4 = 0x10001;
			fwrite(&bytes4, 4, 1, invtrackfile);

			fwrite(&ccb, 4, 1, invtrackfile);		// CCB size
				}
#endif;
//			bytes4 = 0x11111111;	CHECK if 0x2190 - filler is less than 0x50?
				}


					while (!feof(ccbFile)) {
						fread(&tmpvalue, 4, 1, ccbFile);
						// Write celfile after 80 bytes section to invtrackfile
						fwrite(&tmpvalue, 4, 1, invtrackfile);
					}
//				cout << "ccbFile " << i <<"\n"; // Just for test!!!
				fclose(ccbFile); // closed cel file
				};
///////////************************//////////////////

	// Write zeroes after CCB data
	// Check if points data with CCB pic is less than this value. Increase modulus if necessary.
//						cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;
checkagain:
			int size = modulus - size2CCB - CCBsize;
			if (CCBsize == 0) {
				size = 0x670+4; //Alpine segment 1 example // CHECK!!!!!!!!!!!!!!!!!!!!!!
				}
//					cout << "i = 0x" << hex << i << endl;
//					cout << "modulus = 0x" << hex << modulus << endl;
			if (size < 0) {
				modulus += 0x800;
				goto checkagain;
				}
if (straight) {
			modulusArray[i] = modulus;/// Turn this on to straight save
			modulus = 0x3000;
} else {
			modulusArray[numSceneryIndexes-i-1]	= modulus; /// Turn this on to backwards save
			modulus = 0x3000;
}
	// Align filler 2. 
			bytes4 = 0;
			for (int jw2=0; jw2 < ((size-1)/4); jw2++) {
			fwrite(&bytes4, 4, 1, invtrackfile);
			}
#endif; // Fillers

#if 1; // Header 0xC
		temp = 0x54524B44;
		temp = Swap4Bytes(temp);		
		fwrite(&temp, 4, 1, invtrackfile3);

lengthTRKD = 0;
//		filler = 0x30C; // Depend on TRACKDEPTH value. 
//		cout << "size9C = 0x" << hex << size9C << endl;
		lengthTRKD = 0xC + (TRACKDEPTH+1) * 0x258 + size9C + filler + 0x50 + CCBsize;  // Without filler 2
//		cout << "i = 0x" << hex << i << endl;
//		cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;
//		cout << "(TRACKDEPTH+1)*0x258 = 0x" << hex << (TRACKDEPTH+1)*0x258 << endl;
//		cout << "size9C = 0x" << hex << size9C << endl;
//		cout << "filler = 0x" << hex << filler << endl;
//		cout << "CCBsize = 0x" << hex << CCBsize << endl << endl;
//		lengthTRKD = CCBsize + 0xB60 + numSceneryIndexes - i;	// Current TRKD size. Must be precalculated before + CCB size and fix coords size! !!!!!!!!!!!!!

if (straight) {
		trkdArray[i] = lengthTRKD;/// Turn this on to straight save
} else {
		trkdArray[numSceneryIndexes-i-1] = lengthTRKD; // FIX!!!!!/// Turn this on to backwards save
}
	temp = Swap4Bytes(lengthTRKD); // Current TRKD size.
		fwrite(&temp, 4, 1, invtrackfile3);

if (straight) { 
		temp = i; /// Turn this on to straight save
} else {
		temp = numSceneryIndexes-i-1; // Record #  /// Turn this on to backwards save
}
//		temp = i-1; // Record #
		temp = Swap4Bytes(temp); // Record #
		fwrite(&temp, 4, 1, invtrackfile3);
#endif; // Header 0xC
//********************* Write TRKD header END ***************************************

fclose(invtrackfile3);
fclose(invtrackfile);
	} // End of main TRKD loop 







} else { // MAKE HERE TRKD LOOP AGAIN, BUT FOR REVERSED TRACK!
///////////*******************************************************************************************************************///
///////////*******************************************************************************************************************///
for (int i=numSceneryIndexes-1; i>=0; i--) { /// TURN THIS ON TO BACKWARDS SAVE (ALSO SET straight = false)

// Write unknown points.
float xx, yy, zz;

xx = (float)SCENERYARRAY[i].refpointXB4/SCALE;
zz = (float)SCENERYARRAY[i].refpointZB4/SCALE;
yy = (float)SCENERYARRAY[i].refpointYB4/SCALE;

fprintf(refFile, "v ");
fprintf(refFile, "%f %f %f\n", (float)xx, (float)zz, -(float)yy);

char trkd[40];
int temp1; 

	if (straight) { 
	temp1 = i;/// Turn this on to straight save
	} else {
	temp1 = numSceneryIndexes-i-1; /// Turn this on to backwards save FIX????????
	}

itoa(temp1, trkd, 10);
strcpy(path, "\Inverted_track\\TRKDs");
strcat(path, trkd);
invtrackfile = fopen(path, "wb");

strcpy(path3, "\Inverted_track\\TRKDheaders");
char trkdN[40];
itoa(temp1, trkdN, 10);
strcat(path3, trkdN);
invtrackfile3 = fopen(path3, "wb");


////////////////////// MAIN COORDS/////////////////
#if 1;
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].RawFence; // Fence. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);
#if 1; 
				if (i>100) {// CHECK and FIX if necessary for all tracks!!!
			bytes4 = SCENERYARRAY[i-1].PointE0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);			
				} else {
			bytes4 = SCENERYARRAY[i].PointA0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
				}
				// Write mystic 12 bytes.////////// FIND OUT WHAT IS IT!
			bytes4 = 0;
//			bytes4 = SCENERYARRAY[i].PointE0X;
			fwrite(&bytes4, 4, 1, invtrackfile);
//			bytes4 = SCENERYARRAY[i].PointE0Z;
			fwrite(&bytes4, 4, 1, invtrackfile);
//			bytes4 = SCENERYARRAY[i].PointE0Y;
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA0X;  // Write A points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointA9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0xB4 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB0X;  // Write B points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointB9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointB10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointB10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0x144 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC0X;  			// Write C points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointC9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointC10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointC10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at 0x1D4 offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD0X;  			// Write D points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointD9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointD10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointD10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#endif; 
#endif;
////////////////////// MAIN COORDS END //////////////




// Start subloop here. It's TRACKDEPTH entries. Size 0x258
#if 1;
			for (int j=0; j<TRACKDEPTH; j++) {

			byte1 = 0; // subloop start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].RawFence; // Fence. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1+j].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);
#if 1; 
			if (i>100) {// CHECK and FIX if necessary for all tracks!!!
			bytes4 = SCENERYARRAY[i+j].PointE0X;  // Write reference point (A0 point?) or E0 point from i-1  block???
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+j].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+j].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);			
				} else {
				
			bytes4 = SCENERYARRAY[i+1+j].PointA0X;  // Write reference point (A0=E0 point?)
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			}
				// Write mystic 12 bytes.////////// FIND OUT WHAT IS IT!
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA0X;  // Write A=E points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointA9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointA10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointA10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);
//#if 0;
			bytes4 = SCENERYARRAY[i+1+j].PointB0X;  // Write B points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointB9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointB10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointB10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

		// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC0X;  			// Write C points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointC9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointC10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointC10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			// Write unkown 12 bytes at ---- offset
			bytes4 = 0;
				fwrite(&bytes4, 4, 1, invtrackfile);
				fwrite(&bytes4, 4, 1, invtrackfile);				
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD0X;  			// Write D points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i+1+j].PointD9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1+j].PointD10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1+j].PointD10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#if 0;
			bytes4 = SCENERYARRAY[i].PointE0X;  			// Write E points
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE0Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE0Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE1X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE1Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE1Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE2X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE2Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE2Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE3X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE3Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE3Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE4X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE4Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE4Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE5X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE5Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE5Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE6X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE6Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE6Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE7X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE7Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE7Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE8X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE8Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE8Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			
			bytes4 = SCENERYARRAY[i].PointE9X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE9Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE9Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointE10X;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE10Z;  
				fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointE10Y;  
				fwrite(&bytes4, 4, 1, invtrackfile);
#endif; // E10
#endif; 
			}
#endif;
/// Subloop ends here!

	
#if 1;
	
int size9C = 5 * 0x9C;
testagain2:
	//		filler = size2CCB - 0x50 - (TRACKDEPTH+1) * 0x258 - 0xC;
		filler = size2CCB - 0x50 - (TRACKDEPTH+1) * 0x258 - size9C - 0xC;

		if (filler < 0) {
			size2CCB += 0x100;
			goto testagain2;
			}
//		filler = size2CCB - 0x50 - lengthTRKD;
//				cout << "i = 0x" << hex << i << endl;
//				cout << "filler = 0x" << hex << filler << endl;
	//			cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;

	// Write 5 0x9C blocks
	// Write block 1
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence//
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i+1].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile); // =0

			bytes4 = SCENERYARRAY[i+1].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i+1].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i+1].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 2
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture1; // Texture1
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile); //=0

			bytes4 = SCENERYARRAY[i].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 3
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-1].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-1].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-1].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-1].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

	// Write block 4
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence// 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-2].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-2].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-2].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-2].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);


	// Write block 5
			byte1 = 0; // start. 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = 0; // Don't write fence//
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture1; // Texture1 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture2; // Texture2 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture3; // Texture3 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture4; // Texture4 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture5; // Texture5 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture6; // Texture6 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture7; // Texture7 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture8; // Texture8 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture9; // Texture9 
				fwrite(&byte1, 1, 1, invtrackfile);
			byte1 = SCENERYARRAY[i-3].Texture10; // Texture10 
				fwrite(&byte1, 1, 1, invtrackfile);

			bytes4 = 0; //Weird 12 bytes//////////////  FIX!!!!!
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);//=0

			bytes4 = SCENERYARRAY[i-3].PointA0X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA0Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA0Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA1X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA1Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA1Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA2X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA2Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA2Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA3X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA3Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA3Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA4X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA4Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA4Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA5X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA5Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA5Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA6X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA6Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA6Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA7X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA7Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA7Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA8X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA8Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA8Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA9X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA9Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA9Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

			bytes4 = SCENERYARRAY[i-3].PointA10X;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA10Z;  
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = SCENERYARRAY[i-3].PointA10Y;  
			fwrite(&bytes4, 4, 1, invtrackfile);

// Fill the rest before 80 bytes before CCB with zeroes.
			bytes4 = 0;
			for (int jw=0; jw < (filler/4); jw++) {
			fwrite(&bytes4, 4, 1, invtrackfile);
			}


///////////************************//////////////////
	// Write CCB here and calculate it's CCBsize for the header below
	// Load corresponding cel file from TRDKcels3DO folder
				// Generate filename
char tmppath[40] = {0};
char celN[40] = {0};
int tmpvalue = 0;
FILE *ccbFile;

CCBsize = 0;
				itoa(i, celN, 10); 
				strcpy(tmppath, "\TRDKcels3DO\\TRKD");
				strcat(tmppath, celN);

	// Write 80 bytes here
//				if ((ccbFile = fopen(tmppath, "rb")) == false) {
//			bytes4 = 0;	
//				} else {
//			bytes4 = 0x11111111;	// FIX!!!!!!!!!!!!!!!!!. ALSO if 0x2190 - filler is less than 0x50?
//				}
//			for (int jw1=0; jw1 < 20; jw1++) {
//			fwrite(&bytes4, 4, 1, invtrackfile);
//			}

				// Open file for reading
				if (ccbFile = fopen(tmppath, "rb")) {
					fseek(ccbFile, 0, SEEK_END);
					CCBsize = ftell(ccbFile);
//					cout << "CCBsize " << CCBsize <<"\n"; // Just for test
					fseek(ccbFile,0,SEEK_SET);

	// Write 80 bytes here. MAKE for backwards!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		int	refPX = SCENERYARRAY[i+12].PointA0X; 
		int	refPZ = SCENERYARRAY[i+12].PointA0Z; 
		int	refPY = SCENERYARRAY[i+12].PointA0Y;

		int	next1X = SCENERYARRAY[i+20].PointA6X; 
		int	next1Z = SCENERYARRAY[i+20].PointA6Z; 
		int	next1Y = SCENERYARRAY[i+20].PointA6Y;

		int	next2X = SCENERYARRAY[i+22].PointA0X;
		int	next2Z = SCENERYARRAY[i+22].PointA0Z; 
		int	next2Y = SCENERYARRAY[i+22].PointA0Y;
		int ccb = Swap4Bytes(CCBsize);
		CCBsize = Swap4Bytes(CCBsize);

				if ((ccbFile = fopen(tmppath, "rb")) == false) {
			bytes4 = 0;	
			for (int jw1=0; jw1 < 20; jw1++) {
			fwrite(&bytes4, 4, 1, invtrackfile);
			}
				} else {
#if 1;
				{		
			fwrite(&refPX, 4, 1, invtrackfile);
			fwrite(&refPZ, 4, 1, invtrackfile); // Reference point A0 i+12
			fwrite(&refPY, 4, 1, invtrackfile);

			fwrite(&next1X, 4, 1, invtrackfile); 
			fwrite(&next1Z, 4, 1, invtrackfile);//A6 i+20
			fwrite(&next1Y, 4, 1, invtrackfile);

			fwrite(&next2X, 4, 1, invtrackfile);
			fwrite(&next2Z, 4, 1, invtrackfile);//A0 i+22
			fwrite(&next2Y, 4, 1, invtrackfile);

			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);// = 0
			fwrite(&bytes4, 4, 1, invtrackfile);
			
//				bytes4 = 0x11111111; //////////////////// MAKE!!!!!!!
//				bytes4 = 0xFF1000; //X?
//				bytes2 = 0x1000; //X?
				bytes4 = 0x104E00; 
//			fwrite(&bytes2, 2, 1, invtrackfile);
//				bytes2 = 0x004E; //X?
//			fwrite(&bytes2, 2, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

//				bytes4 = 0xF01D00; //Y?
//				bytes2 = 0xF000; //Y?
				bytes4 = 0xE02D00; 
//			fwrite(&bytes2, 2, 1, invtrackfile);
//				bytes2 = 0x0050; //Y?
//			fwrite(&bytes2, 2, 1, invtrackfile);
			fwrite(&bytes4, 4, 1, invtrackfile);

				bytes4 = 0x11111111; 
//				bytes4 = 0xE02000; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0xC02D00; 
//				bytes4 = 0xC02D00; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0xC01A00; 
//				bytes4 = 0xC01A00; 
			fwrite(&bytes4, 4, 1, invtrackfile);
//				bytes4 = 0x302E00; 
//				bytes4 = 0x302E00; 
			fwrite(&bytes4, 4, 1, invtrackfile);

				bytes4 = 0x10001;
			fwrite(&bytes4, 4, 1, invtrackfile);

			fwrite(&ccb, 4, 1, invtrackfile);		// CCB size
				}
#endif;
//			bytes4 = 0x11111111;	CHECK if 0x2190 - filler is less than 0x50?
				}


					while (!feof(ccbFile)) {
						fread(&tmpvalue, 4, 1, ccbFile);
						// Write celfile after 80 bytes section to invtrackfile
						fwrite(&tmpvalue, 4, 1, invtrackfile);
					}
					//fwrite(&tmpvalue, 4, 1, invtrackfile); // 2023 edit
//				cout << "ccbFile " << i <<"\n"; // Just for test!!!
				fclose(ccbFile); // closed cel file
				};
///////////************************//////////////////

	// Write zeroes after CCB data
	// Check if points data with CCB pic is less than this value. Increase modulus if necessary.
//						cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;
checkagain2:
			int size = modulus - size2CCB - CCBsize;
			if (CCBsize == 0) {
				size = 0x670+4; //Alpine segment 1 example // CHECK!!!!!!!!!!!!!!!!!!!!!!
				}
//					cout << "i = 0x" << hex << i << endl;
//					cout << "modulus = 0x" << hex << modulus << endl;
			if (size < 0) {
				modulus += 0x800;
				goto checkagain2;
				}
if (straight) {
			modulusArray[i] = modulus;/// Turn this on to straight save
			modulus = 0x3000;
} else {
			modulusArray[numSceneryIndexes-i-1]	= modulus; /// Turn this on to backwards save
			modulus = 0x3000;
}
	// Align filler 2. 
			bytes4 = 0;
			for (int jw2=0; jw2 < ((size-1)/4); jw2++) {//for (int jw2=0; jw2 < ((size-1)/4-1); jw2++) { 2023 edit
			fwrite(&bytes4, 4, 1, invtrackfile);
			}
#endif; // Fillers

#if 1; // Header 0xC
		temp = 0x54524B44;
		temp = Swap4Bytes(temp);		
		fwrite(&temp, 4, 1, invtrackfile3);

lengthTRKD = 0;
//		filler = 0x30C; // Depend on TRACKDEPTH value. 
//		cout << "size9C = 0x" << hex << size9C << endl;
		lengthTRKD = 0xC + (TRACKDEPTH+1) * 0x258 + size9C + filler + 0x50 + CCBsize;  // Without filler 2
//		cout << "i = 0x" << hex << i << endl;
//		cout << "lengthTRKD = 0x" << hex << lengthTRKD << endl;
//		cout << "(TRACKDEPTH+1)*0x258 = 0x" << hex << (TRACKDEPTH+1)*0x258 << endl;
//		cout << "size9C = 0x" << hex << size9C << endl;
//		cout << "filler = 0x" << hex << filler << endl;
//		cout << "CCBsize = 0x" << hex << CCBsize << endl << endl;
//		lengthTRKD = CCBsize + 0xB60 + numSceneryIndexes - i;	// Current TRKD size. Must be precalculated before + CCB size and fix coords size! !!!!!!!!!!!!!

if (straight) {
		trkdArray[i] = lengthTRKD;/// Turn this on to straight save
} else {
		trkdArray[numSceneryIndexes-i-1] = lengthTRKD; // FIX!!!!!/// Turn this on to backwards save
}
	temp = Swap4Bytes(lengthTRKD); // Current TRKD size.
		fwrite(&temp, 4, 1, invtrackfile3);

if (straight) { 
		temp = i; /// Turn this on to straight save
} else {
		temp = numSceneryIndexes-i-1; // Record #  /// Turn this on to backwards save
}
//		temp = i-1; // Record #
		temp = Swap4Bytes(temp); // Record #
		fwrite(&temp, 4, 1, invtrackfile3);
#endif; // Header 0xC
//********************* Write TRKD header END ***************************************

fclose(invtrackfile3);
fclose(invtrackfile);
}
///////////*******************************************************************************************************************///
///////////*******************************************************************************************************************///
	}

////////////////////////////////////////////////
//*************************
///////////////////////////////////////////////
fclose(refFile);






// Bind all TRKDs and their headers together.
#if 1;
// Bind all TRKDs and their headers together.
char trkd1[40];
//strcpy(path, "\Inverted_track\\TRKDs");
//strcpy(path3, "\Inverted_track\\TRKDheaders");
int temp1111;

for (int g=0; g<sizeof(path4); g++) {
	path4[g] = 0;
	}
strcpy(path4, "\Inverted_track\\TRKD");
//cout << "path = " << path4 << endl;
invtrackfile4 = fopen(path4, "wb"); // TRKDs result file 


#if 1;
for (int i=0; i<numSceneryIndexes; i++) {
//cout << "i = " << dec << i << endl;	


// Generate path3 and path for input files.
temp1111 = i;
unsigned int headersFileSize = 0;
unsigned int trkdFileSize = 0;


itoa(temp1111, trkd1, 10);

// Blank paths first
for (int g=0; g<sizeof(path); g++) {
	path[g] = 0;
	}

for (int g=0; g<sizeof(path3); g++) {
	path3[g] = 0;
	}

strcpy(path, "\Inverted_track\\TRKDs");
strcpy(path3, "\Inverted_track\\TRKDheaders");

strcat(path, trkd1);
strcat(path3, trkd1);
//cout << "path = " << path << endl;
//cout << "path3 = " << path3 << endl;

invtrackfile3 = fopen(path3, "rb"); // Headers input file
invtrackfile = fopen(path, "rb"); // TRKD body input file

fseek(invtrackfile3, 0, SEEK_END);  
headersFileSize = ftell(invtrackfile3);
fseek(invtrackfile3,0,SEEK_SET);


fseek(invtrackfile, 0, SEEK_END);  
trkdFileSize = ftell(invtrackfile);
fseek(invtrackfile,0,SEEK_SET);

//cout << "headersFileSize = " << headersFileSize << endl;
//cout << "trkdFileSize = " << trkdFileSize << endl;

int temp4 = 0;
for (int r=0; r<headersFileSize/4; r++) {
		fread(&temp, 4, 1, invtrackfile3);
		fwrite(&temp, 4, 1, invtrackfile4);
	}

for (int r1=0; r1<trkdFileSize/4; r1++) {
		fread(&temp, 4, 1, invtrackfile);
		fwrite(&temp, 4, 1, invtrackfile4);
	}

fclose(invtrackfile3);
fclose(invtrackfile);

// Delete current TRKD file 
remove(path);
// Delete current headers file
remove(path3);
	}
#endif;
fclose(invtrackfile4);
#endif;

//Debug ****** strcpy(path, "\Inverted_track\\TRKDs");fclose(invtrackfile);
#endif; // TRKDs



///////////////////*****************************///////////////////////////////
// Write all the headers sized 0x13B4
#if 1;
unsigned int finishLine = 0x208; // TEST and FIX. Make for all tracks!!!!!!!!
CCBsize = 0;
unsigned int summ = 0;
unsigned int lengthSceneryData; 

// Calculate whole length of the scenery data. summ of all trkdArray entries
// Length of the scenery data in bytes (All TRKD sections, from first TRKD till the end of file)
//***//
for (int i=0; i < numSceneryIndexes; i++) { // MAKE and CHECK!!!!!!!!!!!!!!!
	summ += modulusArray[i]; 
	}
lengthSceneryData = summ - 0x1B000 - 0x1B000;


strcpy(path, "\Inverted_track\\Headers");
invtrackfile = fopen(path, "wb");
	bytes4 = 0x0E000000;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write file version
	bytes4 = Swap4Bytes(finishLine);
		fwrite(&bytes4, 4, 1, invtrackfile); // Write finish line
	bytes4 = 0x00000600;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 0x60000
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write x coordinate of the first node
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write z coordinate of the first node	
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write y coordinate of the first node
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 0
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 0
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 0
	bytes4 = Swap4Bytes(lengthSceneryData); // 
		fwrite(&bytes4, 4, 1, invtrackfile); // Write length of the scenery data
	bytes4 = 1;
	bytes4 = Swap4Bytes(bytes4);
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 1
//////////////////////////
	// Write first index table 
	bytes4 = 0;
		fwrite(&bytes4, 4, 1, invtrackfile); // Write 0	

//int bytes41
int sum = 0;
int threshold = 0x1CEA0;
		// All offsets must be calculated as n, n+(n+1) ... 
		for (int s=0; s<numSceneryIndexes-1; s++) { // MAKE BACKWARDS???
//***//		for (int s=numSceneryIndexes; s>0; s--) {
			if (s==0) {
				bytes4 = trkdArray[s];
				} else {
			bytes4 = trkdArray[s-1] + trkdArray[s];
			}

			sum += trkdArray[s];
//			cout << "sum = " << sum << endl;
			trkdArray[s] = sum;
//			cout << "trkdArray[s+1] = " << trkdArray[s+1] << endl;
//			bytes4 = Swap4Bytes(sum);
			bytes4 = sum;


			if ((s>248)&&(s<280)) {  // Brute hack! FIX!!!!!!!!!!!!
			threshold = 0x1C000;
				}

			if ((s>281)&&(s<284)) {  // Brute hack! FIX!!!!!!!!!!!!
			threshold = 0x11000;
				}
			
			if ((s>284)&&(s<400)) {  // Brute hack! FIX!!!!!!!!!!!!
			threshold = 0x18000;
				}
#if 0;
			if (bytes4 > threshold) {
			if (s>248) {
				sum = 0;
				bytes4 = 0;
			} else {
		
				}

			} 

			if ((bytes4 > 0x1CEFF)&&(s<248)&&(s>348)) {
				sum = 0;
				bytes4 = 0;
			}
#endif;
#if 1;
			if (bytes4 > threshold) {/////////// TEST and FIX!!!!!!!!!!!!!!!!!!!245!!!!!!!!!!!!!!
//			if (bytes4 > 0x1CEFF) {
//				cout << "bytes4 = " << bytes4 << endl;
				sum = 0;
				bytes4 = 0;
//				fwrite(&bytes4, 4, 1, invtrackfile);
//				continue;
			}
#endif;

			bytes4 = Swap4Bytes(bytes4);
			fwrite(&bytes4, 4, 1, invtrackfile);
			bytes4 = Swap4Bytes(bytes4);
		} 

//////////////////////////
	// Write second index table 
	// All offsets must be calculated as n + 0x1B000, n + 0x1B000 + (n+1) ...// MAKE BACKWARDS????!!!!
int sumModulus = 0;

		for (int s=0; s<numSceneryIndexes; s++) { // MAKE BACKWARDS???
//***//		for (int s=numSceneryIndexes-1; s>0; s--) {
			if (s==0) {
				bytes4 = 0x1B000;
				goto make;
//				bytes4 = trkdArray[s];
//				} else {
//			bytes4 = trkdArray[s-1] + trkdArray[s];
			}

			sumModulus += modulusArray[s-1];
			modulusArray[s-1] = sumModulus;
			bytes4 = sumModulus + 0x1B000;
make:
			bytes4 = Swap4Bytes(bytes4);
			fwrite(&bytes4, 4, 1, invtrackfile);
		} 

			bytes4 = 0xFF; // Unknown bytes FIX and CHECK!!!!!!!!!!!!!!!!!!!!!!!
			bytes4 = Swap4Bytes(bytes4);
			fwrite(&bytes4, 4, 1, invtrackfile);
//////////////////////////
	// Write third index table 
		for (int s=0; s<32; s++) {
			bytes4 = 0xFFFFFFFF;
			fwrite(&bytes4, 4, 1, invtrackfile);
		} 
//////////////////////////
	// Write fourth index table 
		for (int s=0; s<16; s++) {
			bytes4 = 0;
			fwrite(&bytes4, 4, 1, invtrackfile);
		} 
//////////////////////////
		bytes4 = 0x3F;
			bytes4 = Swap4Bytes(bytes4);// Unknown bytes
			fwrite(&bytes4, 4, 1, invtrackfile);

fclose(invtrackfile);
#endif; // Headers 0x13B4



#if 1;
// Glue all files together (Headers, VirtualRoad, TrafficAndObjects, TRKD)
char path44[60], path55[30], path777[60];

// Blank paths first
for (int g=0; g<sizeof(path777); g++) {
	path777[g] = 0;
	}

strcpy(path777, "\Inverted_track\\"); 
//strcat("\Inverted_track\\", tmpstrREV);
//cout << "path777 = " << path777 << endl;
cout << "Filename for reversed track - " << tmpstrREV << endl;
//////////////////strcpy(path44, "\Inverted_track\\al1_reversed.trk"); // "\Inverted_track\\al1_reversed.trk"
strcat(path777, tmpstrREV);
//cout << "path777 = " << path777 << endl;
//strncpy(path777, path777, 31);
//cout << "result = " << path777 << endl;

strcpy(path55, "\Inverted_track\\Headers");
invtrackfile = fopen(path777, "wb");
invtrackfile2 = fopen(path55, "rb");
//cout << "path44 = " << path44 << endl;
//cout << "path55 = " << path55 << endl;
temp = 0;

// Bind Headers
fread(&temp, 4, 1, invtrackfile2);
	while (!feof(invtrackfile2)) {
		fwrite(&temp, 4, 1, invtrackfile);
		fread(&temp, 4, 1, invtrackfile2);
	}
fclose(invtrackfile2);

// Bind Virtual road
strcpy(path55, "\Inverted_track\\VirtualRoad");
invtrackfile2 = fopen(path55, "rb");

fread(&temp, 4, 1, invtrackfile2);
	while (!feof(invtrackfile2)) {
		fwrite(&temp, 4, 1, invtrackfile);
		fread(&temp, 4, 1, invtrackfile2);
	}
fclose(invtrackfile2);

// Bind TrafficAndObjects
strcpy(path55, "\Inverted_track\\TrafficAndObjects");
invtrackfile2 = fopen(path55, "rb");

fread(&temp, 4, 1, invtrackfile2);
	while (!feof(invtrackfile2)) {
		fwrite(&temp, 4, 1, invtrackfile);
		fread(&temp, 4, 1, invtrackfile2);
	}
fclose(invtrackfile2);


// Bind TRKDs
strcpy(path55, "\Inverted_track\\TRKD");
invtrackfile2 = fopen(path55, "rb");

fread(&temp, 4, 1, invtrackfile2);
	while (!feof(invtrackfile2)) {
		fwrite(&temp, 4, 1, invtrackfile);
		fread(&temp, 4, 1, invtrackfile2);
	}
fclose(invtrackfile2);

fclose(invtrackfile);
// Delete temp files 
//remove(path);
#endif; // Glue all files
}





int main(int argc, char *argv[])
{
string Name;
char tmpstr[40], tmpstr2[40];
int i, temp;
char *infilename2;

Buffer = (Byte *)malloc(BUFFER_SIZE);
	if (!Buffer) {
		printf("# Not enough memory for buffer!\n");
		return 1;
	}

VirtRd = (VirtRd_t *)malloc(BUFFER_SIZE);
	if (!VirtRd) {
		printf("# Not enough memory for buffer!\n");
		return 1;
	}


Object = (Object_t *)malloc(BUFFER_SIZE);
	if (!Object) {
		printf("# Not enough memory for buffer!\n");
		return 1;
	}

Scenery = (Scenery_t *)malloc(BUFFER_SIZE);
	if (!Scenery) {
		printf("# Not enough memory for buffer!\n");
		return 1;
	}

// Если параметров нет, выводим на экран предупреждение
	if (argc!=2) {
			cout << "Usage: NFS_TRI2OBJ inputfile\n";
			cout << "Press the enter key to continue ...";
			cin.get();
	return 1;
	}


// If folders ObjectTextures and SceneryTextures doesn't exist, just create them.
//	CreateDirectory(
#if 1;
    int check; 
    char* dirname = "ObjectTextures"; 
    check = _mkdir(dirname);

	dirname = "SceneryTextures"; 
	check = _mkdir(dirname); 

	dirname = "Textures3DO"; 
	check = _mkdir(dirname); 

	dirname = "TRDKcels3DO"; 
	check = _mkdir(dirname); 

	dirname = "ObjectTextures3D"; 
	check = _mkdir(dirname); 

	dirname = "ObjectTextures3D\\2"; 
	check = _mkdir(dirname); 

	dirname = "ThreeDOObjTextures"; 
	check = _mkdir(dirname); 

	dirname = "ThreeDOSceneryTextures"; 
	check = _mkdir(dirname); 

//	dirname = "ThreeDOObject3DTextures"; 
//	check = _mkdir(dirname); 

	dirname = "ThreeDO3DobjTextures"; 
	check = _mkdir(dirname); 

	dirname = "ThreeDO3DobjTextures\\2"; 
	check = _mkdir(dirname); 

	dirname = "Inverted_track"; 
	check = _mkdir(dirname);

    // check if directory is created or not 
    if (!check) 
        printf("Directories created\n"); 
    else { 
 //      printf("Unable to create directories or already created\n"); 
   } 
#endif;


// Открываем и читаем файл
infilename2 = argv[1];
strncpy(tmpstr, argv[1], 8); //скопировать 5 символов строки argv[1] в строку tmpstr2. Получилось "MAP01"
//strcpy(tmpstr, argv[1]);
//infilename = strncpy(tmpstr, argv[1], 7); //скопировать 7 символов строки argv[1] в строку tmpstr. Получилось "MAP01"
//cout << tmpstr <<endl;

infilename = tmpstr;

// Generate 3DO famfilename
for (int k=0; k<sizeof(tmpstr2); k++)	{
	tmpstr2[k] = 0;
	}

strncpy(tmpstr2, argv[1], 3); //скопировать 3 символа строки argv[1] в строку tmpstr2. Получилось "AL1"
strcat(tmpstr2, "_PKT_000");
///cout << tmpstr2 <<endl;
FamFileName3DO = tmpstr2;
//FamFileName3DO = "Al1_PKT_000";


//cout << FamFileName3DO <<endl;
//infilename = "AL1.TRI";
cout << "Trying to open " << infilename << " \n";


//cout << "File " << infilename << "opening...\n";

inputfile = fopen(infilename,"rb");

//testfile = fopen(testfilename,"wb");
//temp = 0xFFFFFFFF;
//fwrite(&temp,4,1,testfile);
//fclose(testfile);

//
	if (!inputfile) {
		printf("# Can't open input file: %s.\n", infilename);    /* Oh oh */
		free(Buffer);
		cin.get();
		return 1;
	}

cout << "File " << infilename << " is opened.\n";
//--------------------------------------------------------------------------
// Initialize data
InitVars(); 
// Virtual road
ParseVirtualRoad(inputfile);
// Objects
ParseObjects(inputfile);
// Scenery and Fences
ParseScenery(inputfile);
// Fences
//ParseFences(inputfile);
// Horizon texture
ExtractHorz(infilename);
MakeHorz(inputfile);

// Extract reversed track
if (version3DO) {
	ExtractReversedTrack(infilename);
}
// Extract car

//--------------------------------------------------------------------------

fclose(inputfile);
cout << "\nFile " << infilename << " is closed.\n";
free(Buffer);
free(VirtRd);
free(Object);
free(Scenery);

cin.get(); // Waiting for response.

	return 0;
}

