#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include "burger.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <cstring>
#include <string> 
#include <fstream> 
#include <list>
#include <algorithm>
#include <vector>
#include "Vars.h"
using namespace std;

FILE* famFile2;
FILE* famFile3;
int size13DObj=0;
int size83DObj=0;
int vt3DOcounter=1;

typedef struct {		// Polygon struct for 3DO 3D object
      unsigned int indexN; 
      unsigned int vertex1; 
      unsigned int vertex2;
      unsigned int vertex3;
      unsigned int vertex4;
	  char polytype;
	} poly3DO_t;


extern void ExtractObject3DTexture(int textN, char* infilename, int offset2SHPIdir, int textureValue, int subchunk);
extern void ExtractObject3D3DOTexture(int textN, char* FamFileName3DO, int offset2SHPIdir, int textureValue);
extern int Extract3DObject3DO(int textN, char* FamFileName3DO, float angle, float X, float Y, float Z, int counter, int vtcounter);
extern int Extract3DOSHPM(int textN, char* infilename, int type, int lod, unsigned int offsetSHPMabs, unsigned int polygonTexture, unsigned int subchunk); // extract 3DO texture (cars and 3D objects)



class Vector31{
public:
  float x,y,z;
  Vector31(){}
  Vector31(float _x, float _y, float _z){x=_x; y=_y; z=_z;}
  float len2(){return x*x + y*y + z*z;}
  float len(){return sqrt(len2());}
  void normalize(){float l=sqrt(x*x+y*y+z*z); x/=l; y/=l; z/=l;}
  float operator * (Vector31 arg){return x*arg.x + y*arg.y + z*arg.z;}
  Vector31 operator = (Vector31 src){x=src.x; y=src.y; z=src.z; return *this;}
  Vector31 operator + (Vector31 arg){Vector31 tmp(x+arg.x, y+arg.y, z+arg.z); return tmp;}
  Vector31 operator - (Vector31 arg){Vector31 tmp(x-arg.x, y-arg.y, z-arg.z); return tmp;}
  Vector31 operator * (float l){Vector31 tmp(x*l, y*l, z*l); return tmp;} //scalar multiply
};


#if 1;
int find_intersection2(Vector31 ray_st, Vector31 ray_dir, Vector31 sp_cen, float sp_rad, Vector31 &res1, Vector31 &res2){
  Vector31 middle; //middle point between intersections
  Vector31 dist; //vector from center of sphere to ray start
  Vector31 dpos; //vector from ray_st to sp_cen
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


int Extract3DObject3DO(int textN, char* FamFileName3DO, float angle, float X, float Y, float Z, int counter, int vtcounter) {
	char tmpstr77[15];
	char tmpstr88[11];
//	int BitmapN;
int temp, offset3Chunk, offset2obj, chunkN, offset2object, offset2LOD1, offset2LOD2, offset2LOD3, offset2ORI3abs, offsetABSLOD, offsetSHPM, offsetSHPMabs, selectedLOD;
Vector31 r1, r2;

for (int f=0; f<sizeof(tmpstr77); f++){
		tmpstr77[f] = 0;
	}
for (int f=0; f<sizeof(tmpstr88); f++){
		tmpstr88[f] = 0;
	}
//*********************
cout << "++3DO 3D object++" << endl;
//cout << "Extracting 3DO 3D object. Texture = #" << textN << endl;
// Generating filename Al1_PKT_000
	strncpy(tmpstr77, FamFileName3DO, 3); //скопировать 3 символа строки FamFileName3DO в строку tmpstr77. Получилось "al1"
	strcat(tmpstr77, "_PKT_000");
	strncpy(tmpstr88, tmpstr77, 11);
//BitmapN = textN;	
//*********************

//cout << "filename for 3D 3DO object = " << tmpstr77 << endl;
//cout << "textN = " << textN << endl;

famFile3 = fopen(tmpstr77, "rb");
// Seek 3rd chunk
fseek(famFile3, 0, SEEK_SET);
	fread(&temp, 4, 1, famFile3);
	fread(&temp, 4, 1, famFile3);
	fread(&temp, 4, 1, famFile3);
	fread(&temp, 4, 1, famFile3);
	fread(&temp, 4, 1, famFile3);
	offset3Chunk = Swap4Bytes(temp);
//	cout << "offset3Chunk = " << hex << offset3Chunk << endl;
	
	fseek(famFile3, offset3Chunk, SEEK_SET); // Jump to 3rd chunk.
	fread(&temp, 4, 1, famFile3);
	fread(&temp, 4, 1, famFile3);
	chunkN = Swap4Bytes(temp);
//	cout << "Number of subchunks in 3DO 3D object section is " << chunkN << "!\n";

#if 1;
int temp11 = 0;
unsigned float remainder = 0;

remainder = textN % 2;
//cout << "remainder = " << remainder << "!\n";

			if ((temp > 1)&&(remainder>0)) {
				for (int tm = 0; tm < temp; tm++)
				{
				fread(&temp11, 4, 1, famFile3);
				offset2object = Swap4Bytes(temp11);
//				cout << "offset2object = " << hex << offset2object << "\n";
				}
///				cout << "offset2object = " << hex << offset2object << "\n";
				goto ORI3found;
			}
#endif;
fread(&temp11, 4, 1, famFile3);
offset2object = Swap4Bytes(temp11);

ORI3found:
offset2obj = offset3Chunk+offset2object;
fseek(famFile3, offset2obj, SEEK_SET); // Jump 2 obj.
fread(&temp11, 4, 1, famFile3);

//cout << "offset2obj = " << hex << offset2obj << "\n";
// Now we at the header of the 3 LODs. Read one of them. For example, first. MAKE possibility to access second and third.
fread(&temp11, 4, 1, famFile3); // Number of LODs

int lods = 0;
lods = Swap4Bytes(temp11);
//cout << "LODs = " << hex << lods << "\n";
fread(&temp11, 4, 1, famFile3);
offset2LOD1 = Swap4Bytes(temp11); // local offset to the wwww header.

fread(&temp11, 4, 1, famFile3); // turn off for the second LOD.
offset2LOD2 = Swap4Bytes(temp11); // turn off for the second LOD.
fread(&temp11, 4, 1, famFile3); // turn off for the third LOD.
offset2LOD3 = Swap4Bytes(temp11); // turn off for the third LOD.

selectedLOD = offset2LOD3;///////////////////////////////////////////////////////////// SELECT LOD! //////////////////////////

offsetABSLOD = offset2obj + selectedLOD;

fseek(famFile3, offsetABSLOD, SEEK_SET); // Jump 2 LOD object.
//cout << "offsetABSLOD = " << hex << offsetABSLOD << "\n";
// Now we are at the wwww header for ORI3 and SHPM blocks.
// Offset to ORI3 has constant 0x10, so absolute offset equals offsetABSLOD + 0x10. Offset to SHPM must be read.
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
offsetSHPM = Swap4Bytes(temp11);
//cout << "offsetSHPM = " << hex << offsetSHPM << "\n";

offsetSHPMabs = offsetSHPM + offsetABSLOD; // absolute offset. 
offset2ORI3abs = offsetABSLOD + 0x10;				// absolute offset.
//cout << "offset2ORI3abs = " << hex << offset2ORI3abs << "\n";
//cout << "offsetSHPMabs = " << hex << offsetSHPMabs << "\n";
///////////////////// NOW I CAN READ ORI3 SECTION //////////////////////////
unsigned int size8, offset8, size10, offset10, size5, offset5, sizeORI3, size11, offset11, offset6;

fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);

sizeORI3 = Swap4Bytes(temp11);

fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
size8 = Swap4Bytes(temp11); 
fread(&temp11, 4, 1, famFile3);
offset8 = Swap4Bytes(temp11);
//cout << "size8 = " << hex << size8 << "\n";
//cout << "offset8 = " << hex << offset8 << "\n";
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
fread(&temp11, 4, 1, famFile3);
size10 = Swap4Bytes(temp11); 
fread(&temp11, 4, 1, famFile3);
offset10 = Swap4Bytes(temp11);
//cout << "size10 = " << hex << size10 << "\n";
//cout << "offset10 = " << hex << offset10 << "\n";
for (int i=0; i < 11; i++) {
	fread(&temp11, 4, 1, famFile3);
	}
fread(&temp11, 4, 1, famFile3);
size5 = Swap4Bytes(temp11);
fread(&temp11, 4, 1, famFile3);
offset5 = Swap4Bytes(temp11);
//cout << "size5 = " << hex << size5 << "\n";
//cout << "offset5 = " << hex << offset5 << "\n";
fread(&temp11, 4, 1, famFile3);
size11 = size10;
fread(&temp11, 4, 1, famFile3);
offset11 = Swap4Bytes(temp11);
//cout << "size11 = " << hex << size11 << "\n";
//cout << "offset11 = " << hex << offset11 << "\n";
fread(&temp11, 4, 1, famFile3); // size6
fread(&temp11, 4, 1, famFile3);
offset6 = Swap4Bytes(temp11);
//cout << "offset6 = " << hex << offset6 << "\n";
fread(&temp11, 4, 1, famFile3);// size7
fread(&temp11, 4, 1, famFile3);// offset7
fread(&temp11, 4, 1, famFile3);// 0x118 or 0x120

// Read all blocks and make 3D geometry for export.///////////
// Read block 8 (vertexes X, Z, Y)
fseek(famFile3, offset2ORI3abs+offset8, SEEK_SET); // Set to the start of 8th block
//cout << "offset2ORI3abs+offset8 = 0x" << hex << offset2ORI3abs+offset8 << "\n";

float *arrayX = new float [size8];
float *arrayZ = new float [size8];
float *arrayY = new float [size8];
int tmp1 = 0;

#if 1; // rotation

for (int x=0; x<size8; x++) {
		fread(&temp11, 4, 1, famFile3); // X coord.
			arrayX[x] = Swap4Bytes(temp11);
//			cout << "+arrayX ["<< x << "]= " << dec << arrayX[x] << "\n";
		fread(&temp11, 4, 1, famFile3); // Z coord.
			arrayZ[x] = Swap4Bytes(temp11);
//			cout << "+arrayZ ["<< x << "]= " << dec << arrayZ[x] << "\n";
		fread(&temp11, 4, 1, famFile3); // Y coord.
			tmp1 = Swap4Bytes(temp11);
			arrayY[x] = -tmp1;
//			cout << "+arrayY ["<< x << "]= " << dec << arrayY[x] << "\n";
}

for (int a=0; a<size8; a++) {
	if ((arrayX[a]==0)&&(arrayY[a]==0)) continue;

/// Find new vector dir for current point.
// Tan alfa = Y/X. Alfa = arctg(Y/X).
float alfa = 0;
alfa = radiansToDegrees(atan(arrayY[a]/arrayX[a]));

find_intersection2(Vector31(0, 0, (float)arrayZ[a]),													// Vector start
				   Vector31(cos(degreesToRadians(alfa+angle)), sin(degreesToRadians(alfa+angle)), 0),	// Vector direction
				   Vector31(0, 0, (float)arrayZ[a]),													// Sphere center
				   sqrt((float)arrayX[a]*(float)arrayX[a]+(float)arrayY[a]*(float)arrayY[a]),			// Sphere radius
				   r1, r2);																				// Result points
//cout << "+++++Vector start: Z= "<< (float)arrayZ[a] << "\n";
//cout << "+++++Vector cos(degreesToRadians(angle)= "<< cos(degreesToRadians(angle)) << "\n";
//cout << "+++++Vector sin(degreesToRadians(angle)= "<< sin(degreesToRadians(angle)) << "\n";
//cout << "+++++Sphere radius= "<< sqrt((float)arrayX[a]*(float)arrayX[a]+(float)arrayY[a]*(float)arrayY[a]) << "\n";
(float)arrayX[a] = r2.x;
(float)arrayY[a] = r2.y;
}
#endif;// rotation


// Read block 10 (polygons binded by vertices)
//poly3DO_t *polys;
//polys = new poly3DO_t[size10] ; // initialize 
poly3DO_t* polys = new poly3DO_t[size10];
int *arrayTextureN = new int [size10];
	int	type = 1; // SHPM for 3D object
	int lod = 1; // LOD
	char ptype;

for (int g=0; g<size10; g++) {
	fread(&ptype, 1, 1, famFile3);  // polygon property. 0 - quadrangle, 1 - triangle
		polys[g].polytype = ptype;
	fread(&temp11, 3, 1, famFile3); // skip
//	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // polygon index #
		polys[g].indexN = Swap4Bytes(temp11);
	fread(&temp11, 4, 1, famFile3); // vertex 1 index
		polys[g].vertex1 = Swap4Bytes(temp11);
	fread(&temp11, 4, 1, famFile3); // vertex 2 index
		polys[g].vertex2 = Swap4Bytes(temp11);
	fread(&temp11, 4, 1, famFile3); // vertex 3 index
		polys[g].vertex3 = Swap4Bytes(temp11);
	fread(&temp11, 4, 1, famFile3); // vertex 4 index
		polys[g].vertex4 = Swap4Bytes(temp11);
//	arrayTextureN[g] = polys[g].indexN;/////////////////////////////////// NOT CORRECT!!!!!!!!!!!!
//	cout << "arrayTextureN[g] = " << arrayTextureN[g] << "\n"; 
}


#if 0;
for (int g1=0; g1<size10; g1++) {
	cout << "indexN = "<< polys[g1].indexN << "\n";
	cout << "vertex 1 = "<< polys[g1].vertex1 << "\n";
	cout << "vertex 2 = "<< polys[g1].vertex2 << "\n";
	cout << "vertex 3 = "<< polys[g1].vertex3 << "\n";
	cout << "vertex 4 = "<< polys[g1].vertex4 << "\n";
	}
#endif;


// Read block 5 ('NON-SORT'....)
for (int h=0; h<size5; h++) {
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	fread(&temp11, 4, 1, famFile3); // skip
	}

// Read block 11 (in case of common 3D objects, reading is unnecessary)

// Must read !ori section.
fseek(famFile3, offsetSHPMabs, SEEK_SET); // Jump 2 SHPM to read !ori.

// Read file until !ori is reached. Then read it's offset.
int ori = 0;
unsigned int off2ori = 0;

for (int i3 = 0; i3 < 128; i3++) {
	fread(&temp11, 4, 1, famFile3);
	ori = temp11;
	if (ori == 0x69726F21) break;
	}

fread(&temp11, 4, 1, famFile3);
off2ori = Swap4Bytes(temp11);
//cout << "off2ori 0x" << hex << off2ori <<"\n";


// Jump to !ori and skip it's header (+ 0x40)
fseek(famFile3, offsetSHPMabs + off2ori + 0x40, SEEK_SET); // Jump 2 !ori and skip 0x40.

// Read polygon number then read texture number located in SHPM.
int subchunk = 0;
subchunk = remainder;
for (int g1=0; g1<size10; g1++) {
	fread(&temp11, 4, 1, famFile3);
	fread(&temp11, 4, 1, famFile3);
	arrayTextureN[g1] = Swap4Bytes(temp11);
	// Read SHPM section and extract textures.
	Extract3DOSHPM(textN, tmpstr77, type, lod, offsetSHPMabs, arrayTextureN[g1], subchunk);
}



////////******************/////////////////////////**********************///////////////////
// 1) Write vertexes from block 8 to the obj file.
//int temp, offset, offset2, offset2ORIP, tmp;
char bmpname[40], tmpbmp[40];


FILE *outf;

itoa(textN, bmpname, 10);
strcpy(tmpbmp, "Object3D.obj");
//cout << tmpbmp << "\n";


//outf = fopen(tmpbmp, "wb");// Blank it.
//fclose(outf);

outf = fopen(tmpbmp,"a");
fwrite("# 3D object\n",12,1,outf);
fprintf(outf, "mtllib Obj3D.mtl\n\n");
//fprintf(outf, "o 3DObject");
fprintf(outf, "o 3DObject%f\n", X);
//fprintf(outf, "%d\n", BitmapN);

// Write vertex coords.
for (int s=0; s<size8; s++) {
	fwrite("v ",2,1,outf); //1
	fprintf(outf, "%f ", (float)arrayX[s]/SCALE3D + (float)X);
	fprintf(outf, "%f ", (float)arrayZ[s]/SCALE3D + (float)Z);
	fprintf(outf, "%f ", (float)arrayY[s]/SCALE3D + (float)Y);
///	fprintf(outf, "%f ", (float)arrayX[s]);
///	fprintf(outf, "%f ", (float)arrayZ[s]);
///	fprintf(outf, "%f ", (float)arrayY[s]);
	fwrite("\n",1,1,outf);// New line
}
fwrite("\n",1,1,outf);// New line

//fclose(outf);


// Write vt params. There are 4 vertices. Max uv coord is 1.0!
for (int i4 = 0; i4<size10; i4++) {
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000\n");
fprintf(outf, "vt 1.000000 1.000000\n");
fprintf(outf, "vt 1.000000 0.000000\n");
fprintf(outf, "vt 0.000000 0.000000\n");
}

//fprintf(outf, "\n");
//fprintf(outf, "vt 1.000000 1.000000\n");
//fprintf(outf, "vt 0.000000 1.000000\n");
//fprintf(outf, "vt 0.000000 0.000000\n");
//fprintf(outf, "vt 1.000000 0.000000\n");

// Write polygons
// Draw size 10 polygons.
{
	for (int b=0; b<size10; b++) {

		if (subchunk==1) {
			fprintf(outf, "usemtl %d\n", ((arrayTextureN[b]+111)*10));
		} else {
			fprintf(outf, "usemtl %d\n", arrayTextureN[b]);
		}
	fwrite("f ",2,1,outf);
#if 0;		
		fprintf(outf, "%d %d %d %d \n", 
		polys[b].vertex1+1 + counter, 
		polys[b].vertex2+1 + counter,
		polys[b].vertex3+1 + counter,
		polys[b].vertex4+1 + counter);
#endif;
#if 1;
		if (polys[b].polytype == 1) {
		fprintf(outf, "%d/%d %d/%d %d/%d\n", 
		polys[b].vertex1+1 + counter, vtcounter,
		polys[b].vertex2+1 + counter, vtcounter+1,
		polys[b].vertex3+1 + counter, vtcounter+2);
		vtcounter += 3;
		} else
		{
		fprintf(outf, "%d/%d %d/%d %d/%d %d/%d\n", 
		polys[b].vertex1+1 + counter, vtcounter,
		polys[b].vertex2+1 + counter, vtcounter+1,
		polys[b].vertex3+1 + counter, vtcounter+2,
		polys[b].vertex4+1 + counter, vtcounter+3);
#endif;

	vtcounter += 4;
		}

	}
}
//counter += 4;
vt3DOcounter = vtcounter;
//size13DObj = size13DObj+size1;
size83DObj = size83DObj+size8; 
fclose(outf);



// Make material file
FILE *mtlfile;

mtlfile = fopen("Obj3D.mtl","a");

for (int t=0; t<size10; t++) {
//fprintf(mtlfile, "newmtl %d\n", arrayTextureN[t]);
	if (subchunk==1) {
	fprintf(mtlfile, "newmtl %d\n", ((arrayTextureN[t]+111)*10));
	fprintf(mtlfile, "map_Kd ThreeDO3DobjTextures\\2\\Tex%d.bmp\n", ((arrayTextureN[t]+111)*10));
	} else {
	fprintf(mtlfile, "newmtl %d\n", arrayTextureN[t]);
	fprintf(mtlfile, "map_Kd ThreeDO3DobjTextures\\Tex%d.bmp\n", arrayTextureN[t]);
	}
}
fclose(mtlfile);

///////////////////////////////////////////
#if 0;
FILE *mtlfile;
int c, count;

// Open mtlfile for reading, check presence for all arrayTexture[] values.

// Read all file and seek all arrayTexture[]. If found, this arrayTexture[] shouldn't be written to the mtlfile.
   ifstream file("Obj3D.mtl"); 
    string s;
	char find[256] = {0};
    char c1;
	bool found = false;

	 while (!file.eof()){ // Fill s string
        file.get(c1);
        s.push_back(c1);
    }
    file.close();


//for (int c=0; c < count; c++) {
	for (int c=0; c < size3; c++) {


		strcpy(find,itoa(arrayTexture[c],find,10)); 
int pos;
		pos = s.find(find);
//		pos = s.find(itoa(arrayTexture[c],find,10));

	if (pos == -1)
		cout << "Material entry not found. Adding material to mtlfile." << endl;
	else {
		found = true;
//		cout << "Found! Position = " << pos << endl;
	}

	if (found == true) continue;
		mtlfile = fopen("Obj3D.mtl","a");
		if (flag3DO == true) {	
			fprintf(mtlfile, "newmtl %d\n", c);
			fprintf(mtlfile, "map_Kd ThreeDO3DobjTextures\\Tex%d.bmp\n", arrayTexture[c]);

		} else {
	if (found == true) continue;

		fprintf(mtlfile, "newmtl %d\n", c);
		fprintf(mtlfile, "map_Kd ThreeDO3DobjTextures\\Tex%d.bmp\n", arrayTexture[c]);
		}
	fclose(mtlfile);
	found = false;
	}
#endif;
///////////////////////////////////////////

////////******************/////////////////////////**********************///////////////////




fclose(famFile3);
delete []polys;  
return size83DObj;
}











int Extract3D(float angle, float X, float Y, float Z, int BitmapN, char* infilename, int counter, int vtcounter, int flag, char* FamFileName3DO) {
//	char tmpstr5[11]; was before 10.08.2020
	char tmpstr5[15];
	char tmpstr6[11];
int temp, offset, offset2, offset2ORIP, tmp;
unsigned char tmp1;
unsigned int size1, size2, size3, size4, size5, size6, size7, size8, size9;
unsigned int off1, off2, off3, off4, off5, off6, off7, off8, off9;
const char triangle = 0x83;
const char quadrangle = 0x84;
const char quadOrNull = 0x8C;
unsigned int chunkLength = 0;
unsigned int offtoORIPabs = 0;
bool flag3DO = false;
char bmpname[40], tmpbmp[40];
Vector31 r1, r2;
int offset2SHPIdir = 0;
//static Obj3Dparams_t *Obj3Dparams;

Obj3Dparams = (Obj3Dparams_t *)malloc(0x400000UL);
	if (!Obj3Dparams) {
		printf("# Not enough memory for buffer!\n");
		return 1;
	}


flag3DO = flag;
//cout << "flag3DO = " << flag3DO << endl;


//cout << "Extracting 3D object. Texture = #" << BitmapN << endl;
// Search 4th chunk in famfilename
for (int f=0; f<sizeof(tmpstr6); f++){
		tmpstr6[f] = 0;
	}
for (int f=0; f<sizeof(tmpstr5); f++){
		tmpstr5[f] = 0;
	}

#if 1;
strncpy(tmpstr5, infilename, 3); //скопировать 3 символа строки infilename в строку tmpstr5. Получилось "al1"
//cout << "infilename - " << infilename << "\n";


if (flag3DO == true) {						// If it's a 3DO 3D object, use another func to extract it.
//	cout << "tmpstr5 - " << tmpstr5 << "\n";
	Extract3DObject3DO(BitmapN, tmpstr5, angle, X, Y, Z, counter, vtcounter); // Extract 3DO 3D object with it's textures (?)
return 0;
	}

//cout << "Extracting PC 3D object. Texture = #" << BitmapN << endl;
strcat(tmpstr5, "_001.FAM");
strncpy(tmpstr6, tmpstr5, 11);
//cout << "tmpstr5 - " << tmpstr5 << "\n";
//cout << "tmpstr6 - " << tmpstr6 << "\n";

famFile2 = fopen(tmpstr5, "rb");
///famFile2 = fopen(tmpstr6, "rb");
// Seek 4th chunk
fseek(famFile2, 0, SEEK_SET);
	fread(&temp, 4, 1, famFile2);
	fread(&temp, 4, 1, famFile2);
	fread(&temp, 4, 1, famFile2);
	fread(&temp, 4, 1, famFile2);
	fread(&temp, 4, 1, famFile2);
	fread(&temp, 4, 1, famFile2);

offset = temp;
//cout << "offset = 0x" << hex << offset << "\n";
fseek(famFile2, offset, SEEK_SET); // Set offset
		fread(&temp, 4, 1, famFile2);
		fread(&temp, 4, 1, famFile2);
//			cout << "Number of subchunks in 3D object section is " << temp << "!\n";
//*********************
#if 1;
int temp11 = 0;
unsigned float remainder = 0;

remainder = BitmapN % 2;
//cout << "remainder = " << remainder << "!\n";

			if ((temp > 1)&&(remainder>0)) {
				for (int tm = 0; tm < temp; tm++)
				{
				fread(&temp11, 4, 1, famFile2);
//				cout << "temp = " << temp11 << "!\n";
				}
				offset2 = temp11;
				goto offfound;
			} ///else {
#endif;
//*********************
		fread(&temp, 4, 1, famFile2);
///			}
	offset2 = temp;

offfound:
//cout << "offset2 = 0x" << hex << offset2 << "\n";
fseek(famFile2, offset+offset2, SEEK_SET); // Set offset to ORIP's wwww
//cout << "offset+offset2 = 0x" << hex << offset+offset2 << "\n";

fread(&temp, 4, 1, famFile2);	
tmp = temp;
//cout << "wwww? 0x" << hex << tmp << "\n";
// Seek ORIP
fread(&temp, 4, 1, famFile2);
fread(&temp, 4, 1, famFile2);	
offset2ORIP = temp;
//cout << "offset2ORIP = 0x" << hex << offset2ORIP << "\n";

// Set to the ORIP's wwww and jump to ORIP
fseek(famFile2, offset+offset2+offset2ORIP, SEEK_SET);
offtoORIPabs = offset+offset2+offset2ORIP;
//cout << "offset+offset2+offset2ORIP = 0x" << hex << offtoORIPabs << "\n";
fread(&temp, 4, 1, famFile2);	
offset = temp;
//cout << "ORIP = 0x" << hex << offset << "\n";

// ORIP is found. Read 3D geometry.
// 1st block. (12-byte records) and describes the polygons that are used to draw the car
// 2nd block. (8-byte records), which describe the (x,y) coordinates inside the textures referenced by pol2
// 3rd block. (20-byte records)
// 4th block. (20-byte records)
// 5th block. (28-byte records)
// 6th block. (12-byte records)
// 7th block. (12-byte records)
// 8th block. (12-byte records)
// 9th block. long integers, each representing a vertex number

fread(&temp, 4, 1, famFile2);
chunkLength = temp; // Length from ORIP to SHPI

fread(&temp, 4, 1, famFile2); // Skip 4 bytes
fread(&temp, 4, 1, famFile2); // Skip 4 bytes

fread(&temp, 4, 1, famFile2); // Block 8 size
size8 = temp; 

fread(&temp, 4, 1, famFile2); // Skip 2 bytes

fread(&temp, 4, 1, famFile2); // Block 8 offset
off8 = temp;

fread(&temp, 4, 1, famFile2); // Block 2 size
size2 = temp; 
fread(&temp, 4, 1, famFile2); // Block 2 offset
off2 = temp;

fread(&temp, 4, 1, famFile2); // Block 1 size
size1 = temp; 
fread(&temp, 4, 1, famFile2); // Block 1 offset
off1 = temp;

//cout << "off1 = 0x" << hex << off1 << "\n";
fread(&temp, 4, 1, famFile2); // Skip 4 bytes
fread(&temp, 4, 1, famFile2); // Skip 4 bytes	// TRESTLE (identifier)
fread(&temp, 4, 1, famFile2); // Skip 4 bytes

fread(&temp, 4, 1, famFile2); // Block 3 size
size3 = temp; 
fread(&temp, 4, 1, famFile2); // Block 3 offset
off3 = temp;

fread(&temp, 4, 1, famFile2); // Block 4 size
size4 = temp; 
fread(&temp, 4, 1, famFile2); // Block 4 offset
off4 = temp;

fread(&temp, 4, 1, famFile2); // Block 5 size
size5 = temp; 
fread(&temp, 4, 1, famFile2); // Block 5 offset
off5 = temp;

fread(&temp, 4, 1, famFile2); // Block 9 offset
off9 = temp;

fread(&temp, 4, 1, famFile2); // Block 6 size
size6 = temp; 
fread(&temp, 4, 1, famFile2); // Block 6 offset
off6 = temp;

fread(&temp, 4, 1, famFile2); // Block 7 size
size7 = temp; 
fread(&temp, 4, 1, famFile2); // Block 7 offset
off7 = temp;

fread(&temp, 4, 1, famFile2); // Skip 4 bytes
fread(&temp, 4, 1, famFile2); // Skip 4 bytes	
fread(&temp, 4, 1, famFile2); // Skip 4 bytes
// Now we are at the start of block 1

//fread(&temp, 4, 1, famFile2); // Skip 2 bytes
//cout << "t = 0x" << hex << temp << "\n";

////////////////////////////////////////// Read block 1. There are size1 entries.
fseek(famFile2, offtoORIPabs+off1, SEEK_SET); // Set to the start of 1st block
//cout << "off1 = 0x" << hex << offtoORIPabs+off1 << "\n";
//cout << "size1 = " << dec << size1 << "\n";

// Polygons count = size1:
//cout << "size1 = " << dec << size1 << "\n";
// Prepare params for each polygon.
// Make dynamic array for the polys.
unsigned int *arrayPolyType = new unsigned int [size1];
int *arrayTextureN = new int [size1];
int *arrayPolyOff1 = new int [size1];
int *arrayPolyOff2 = new int [size1];

// Texture N
// Polygon1 offset
// Polygon2 offset
#if 1;
for (int i=0; i<size1; i++) {
	fread(&tmp1, 1, 1, famFile2); // Poly type
		arrayPolyType[i] = tmp1;
//		cout << "+arrayPolyType ["<< i << "]= 0x" << hex << arrayPolyType[i] << "\n";
	fread(&tmp1, 1, 1, famFile2); // Skip 1 byte
	fread(&tmp1, 1, 1, famFile2); // TextureN
		arrayTextureN[i] = tmp1;
//		cout << "+arrayTextureN ["<< i << "]= " << dec << arrayTextureN[i] << "\n";
	fread(&tmp1, 1, 1, famFile2); // Skip 1 byte
	fread(&temp, 4, 1, famFile2); // Poly1 offset
		arrayPolyOff1[i] = temp;
//		cout << "+arrayPolyOff1 ["<< i << "]= " << dec << arrayPolyOff1[i] << "\n";
	fread(&temp, 4, 1, famFile2); // Poly1 offset
		arrayPolyOff2[i] = temp;
//		cout << "+arrayPolyOff2 ["<< i << "]= " << dec << arrayPolyOff2[i] << "\n";
}
#endif; 

#if 0; // For the test
for (int j=0; j<size1; j++) {
	cout << "arrayPolyType ["<< j << "]= " << dec << arrayPolyType[j] << "\n";
	}

for (int h=0; h<size1; h++) {
	cout << "arrayTextureN ["<< h << "]= " << dec << arrayTextureN[h] << "\n";
	}

for (int r=0; r<size1; r++) {
	cout << "arrayPolyOff1 ["<< r << "]= " << dec << arrayPolyOff1[r] << "\n";
	}

for (int d=0; d<size1; d++) {
	cout << "arrayPolyOff2 ["<< d << "]= " << dec << arrayPolyOff2[d] << "\n";
	}

#endif;// For the test


/////////////////////////////////////// Read block 2. There are size2 entries.
fseek(famFile2, offtoORIPabs+off2, SEEK_SET); // Set to the start of 2nd block
//cout << "off2 = 0x" << hex << offtoORIPabs+off2 << "\n";
//cout << "off2 = 0x" << hex << off2 << "\n";
//cout << "size2 = " << dec << size2 << "\n";

if (size2==0) {
	goto block3;
		} else {	

	}

unsigned int *arrayXcoord = new unsigned int [size2];
unsigned int *arrayYcoord = new unsigned int [size2];

#if 1;
for (int y=0; y<size2; y++) {
	fread(&temp, 4, 1, famFile2); // X coord
		arrayXcoord[y] = temp;
//		cout << "+arrayXcoord ["<< y << "]= " << dec << arrayXcoord[y] << "\n";
	fread(&temp, 4, 1, famFile2); // Y coord
		arrayYcoord[y] = temp;
//		cout << "+arrayYcoord ["<< y << "]= " << dec << arrayYcoord[y] << "\n";
}
#endif;

block3:

///////////////////////////////////// Read block 3. There are size3 entries.
fseek(famFile2, offtoORIPabs+off3, SEEK_SET); // Set to the start of 3rd block
//cout << "off3 = 0x" << hex << offtoORIPabs+off3 << "\n";
//cout << "size3 = " << dec << size3 << "\n";

int *arrayTexture = new int [size3];

#if 1;
for (int u=0; u<size3; u++) {
	fread(&temp, 4, 1, famFile2); // Skip 4 bytes.
	fread(&temp, 4, 1, famFile2); // Skip 4 bytes.
	fread(&temp, 4, 1, famFile2); // Identifier 2.
		// Store value of texture. We'll use it later in SHPI chunk.
		arrayTexture[u] = temp;
//		cout << "+arrayTexture ["<< u << "]= 0x" << hex << arrayTexture[u] << "\n";

	fread(&temp, 4, 1, famFile2); // Skip 4 bytes.
	fread(&temp, 4, 1, famFile2); // Skip 4 bytes.
}
#endif;

//////////////////////////////////////// Read block 4. There are size4 entries.
fseek(famFile2, offtoORIPabs+off4, SEEK_SET); // Set to the start of 4th block
//cout << "size4 = " << dec << size4 << "\n";

if (size4==0) {
	goto block5;
		} else {	
	cout << "-------------------BLOCK 4 is nonZero!"<< endl;
	}


block5:
///////////////////////////////////// Read block 5. There are size5 entries.
fseek(famFile2, offtoORIPabs+off5, SEEK_SET); // Set to the start of 5th block
//cout << "off5 = 0x" << hex << offtoORIPabs+off5 << "\n";
//cout << "size5 = " << dec << size5 << "\n";

int *arrayPolyOffsetINBlock9 = new int [size5];
int *arrayNumOfVertices = new int [size5];
#if 1;
for (int p=0; p<size5; p++) {
		fread(&temp, 4, 1, famFile2); // Skip 4 bytes.	
		fread(&temp, 4, 1, famFile2); // Skip 4 bytes.	
		fread(&temp, 4, 1, famFile2); // Skip 4 bytes.	
		fread(&temp, 4, 1, famFile2); // Skip 4 bytes.	
		fread(&temp, 4, 1, famFile2); // Read polygon offset.
			arrayPolyOffsetINBlock9[p] = temp;
//					cout << "+arrayPolyOffsetINBlock9 ["<< p << "]= " << dec << arrayPolyOffsetINBlock9[p] << "\n";
		fread(&temp, 4, 1, famFile2); // Read number of vertices.
			arrayNumOfVertices[p] = temp;
//			cout << "+arrayNumOfVertices ["<< p << "]= " << dec << arrayNumOfVertices[p] << "\n";
		fread(&temp, 4, 1, famFile2); // Skip 4 bytes.
}
#endif;



//////////////////////////////////////// Read block 6. There are size6 entries.
fseek(famFile2, offtoORIPabs+off6, SEEK_SET); // Set to the start of 6th block
//cout << "size6 = " << dec << size6 << "\n";
if (size6==0) {
	goto block7;
		} else {	
	cout << "-------------------BLOCK 6 is nonZero!"<< endl;
	}

block7:
/////////////////////////////////////// Read block 7. There are size7 entries.
fseek(famFile2, offtoORIPabs+off7, SEEK_SET); // Set to the start of 7th block
//cout << "size7 = " << dec << size7 << "\n";
if (size7==0) {
	goto block8;
		} else {	
//	cout << "-------------------BLOCK 7 is nonZero!"<< endl;
	}

block8:
//////////////////////////////////// Read block 8. There are size8 entries.
fseek(famFile2, offtoORIPabs+off8, SEEK_SET); // Set to the start of 8th block
//cout << "off8 = 0x" << hex << offtoORIPabs+off8 << "\n";
//cout << "off8 = 0x" << hex << off8 << "\n";
//cout << "size8 = " << dec << size8 << "\n";

float *arrayX = new float [size8];
float *arrayZ = new float [size8];
float *arrayY = new float [size8];
#if 1;

for (int x=0; x<size8; x++) {
		fread(&temp, 4, 1, famFile2); // X coord.
			arrayX[x] = temp;
//			cout << "+arrayX ["<< x << "]= " << dec << arrayX[x] << "\n";
		fread(&temp, 4, 1, famFile2); // Z coord.
			arrayZ[x] = temp;
//			cout << "+arrayZ ["<< x << "]= " << dec << arrayZ[x] << "\n";
		fread(&temp, 4, 1, famFile2); // Y coord.
			arrayY[x] = -temp;
//			cout << "+arrayY ["<< x << "]= " << dec << arrayY[x] << "\n";
//			cout << "+arrayZ ["<< x << "]= " << dec << arrayZ[x] << "\n\n";
}

for (int a=0; a<size8; a++) {
	if ((arrayX[a]==0)&&(arrayY[a]==0)) continue;

/// Find new vector dir for current point.
// Tan alfa = Y/X. Alfa = arctg(Y/X).
float alfa = 0;
alfa = radiansToDegrees(atan(arrayY[a]/arrayX[a]));

find_intersection2(Vector31(0, 0, (float)arrayZ[a]),													// Vector start
				   Vector31(cos(degreesToRadians(alfa+angle)), sin(degreesToRadians(alfa+angle)), 0),	// Vector direction
				   Vector31(0, 0, (float)arrayZ[a]),													// Sphere center
				   sqrt((float)arrayX[a]*(float)arrayX[a]+(float)arrayY[a]*(float)arrayY[a]),			// Sphere radius
				   r1, r2);																				// Result points
//cout << "+++++Vector start: Z= "<< (float)arrayZ[a] << "\n";
//cout << "+++++Vector cos(degreesToRadians(angle)= "<< cos(degreesToRadians(angle)) << "\n";
//cout << "+++++Vector sin(degreesToRadians(angle)= "<< sin(degreesToRadians(angle)) << "\n";
//cout << "+++++Sphere radius= "<< sqrt((float)arrayX[a]*(float)arrayX[a]+(float)arrayY[a]*(float)arrayY[a]) << "\n";
(float)arrayX[a] = r2.x;
(float)arrayY[a] = r2.y;
//		cout << "+arrayX_corr ["<< a << "]= " << dec << (float)arrayX[a] << "\n";
//		cout << "+arrayY_corr ["<< a << "]= " << dec << (float)arrayY[a] << "\n";
//		cout << "+arrayZ_corr ["<< a << "]= " << dec << (float)arrayZ[a] << "\n\n";
}

#endif;


///////////////////////////////////// Read block 9. There are size9 entries.
fseek(famFile2, offtoORIPabs+off9, SEEK_SET); // Set to the start of 9th block
//cout << "off9 = 0x" << hex << offtoORIPabs+off9 << "\n";

// Read vertex numbers to SHPI. chunkLength - offtoORIPabs+off9
//cout << "Length of block 9 = 0x" << hex << chunkLength - off9 << "\n";
int length = 0;
length = chunkLength - off9;
size9 = length/4;
//cout << "size9 = " << dec << size9 << "\n";

int *arrayVertexN = new int [size9];
#if 1;
for (int e=0; e<size9; e++) {
		fread(&temp, 4, 1, famFile2); // Read vertexN.
			arrayVertexN[e] = temp;
//			cout << "+arrayVertexN ["<< e << "]= " << dec << arrayVertexN[e] << "\n";
}
#endif;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1) Write vertexes from block 8 to the obj file.
FILE *outf;

itoa(BitmapN, bmpname, 10);
strcpy(tmpbmp, "Object3D.obj");
//cout << tmpbmp << "\n";
//outf = fopen(tmpbmp, "wb");// Blank it.
//fclose(outf);

outf = fopen(tmpbmp,"a");
fwrite("# 3D object\n",12,1,outf);
fprintf(outf, "mtllib Obj3D.mtl\n\n");
//fprintf(outf, "o 3DObject");
fprintf(outf, "o 3DObject%f\n", X);
//fprintf(outf, "%d\n", BitmapN);

// Write vertex coords.
for (int s=0; s<size8; s++) {
	fwrite("v ",2,1,outf); //1
	fprintf(outf, "%f ", (float)arrayX[s]/SCALE3D + (float)X);
	fprintf(outf, "%f ", (float)arrayZ[s]/SCALE3D + (float)Z);
	fprintf(outf, "%f ", (float)arrayY[s]/SCALE3D + (float)Y);
///	fprintf(outf, "%f ", (float)arrayX[s]);
///	fprintf(outf, "%f ", (float)arrayZ[s]);
///	fprintf(outf, "%f ", (float)arrayY[s]);
	fwrite("\n",1,1,outf);// New line
}
fwrite("\n",1,1,outf);// New line

// 2) pol1 polygons in block 1 points to block 9, and then translated in 3D in block 8.
// There are size1 polygons.

#if 0; // Turn off vts. // FIX FOR THE SECOND SUBCHUNK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Write vt params
fprintf(outf, "\n");

if (size2==0) {
	cout << "size2= " << dec << size2 << " Set vt params to defaults!\n";
		} else	
	{
	for (int a=0; a < size1; a++) {
		fwrite("vt ",3,1,outf);
		fprintf(outf, "%f %f\n", (float)arrayXcoord[arrayVertexN[arrayPolyOff2[a]]]/SCALE3DObjUVMap, 
			((float)arrayYcoord[arrayVertexN[arrayPolyOff2[a]]])/SCALE3DObjUVMap);

		fwrite("vt ",3,1,outf);
		fprintf(outf, "%f %f\n", (float)arrayXcoord[arrayVertexN[arrayPolyOff2[a]+1]]/SCALE3DObjUVMap, 
			((float)arrayYcoord[arrayVertexN[arrayPolyOff2[a]+1]])/SCALE3DObjUVMap);

		fwrite("vt ",3,1,outf);
		fprintf(outf, "%f %f\n", (float)arrayXcoord[arrayVertexN[arrayPolyOff2[a]+2]]/SCALE3DObjUVMap, 
			((float)arrayYcoord[arrayVertexN[arrayPolyOff2[a]+2]])/SCALE3DObjUVMap);

		fwrite("vt ",3,1,outf);
		fprintf(outf, "%f %f\n", (float)arrayXcoord[arrayVertexN[arrayPolyOff2[a]+3]]/SCALE3DObjUVMap, 
			(float)arrayYcoord[arrayVertexN[arrayPolyOff2[a]+3]]/SCALE3DObjUVMap);
		}
	}
//int vtcounter = 1;
#endif; // Turn off vts.

#if 1;
// Write vt params. Max uv coord is 1.0!
for (int i4 = 0; i4<size1; i4++) {
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 1.000000\n");
fprintf(outf, "vt 0.000000 0.000000\n");
fprintf(outf, "vt 1.000000 0.000000\n");
fprintf(outf, "vt 1.000000 1.000000\n");
}
#endif;

#if 0;
// Write vt params. Max uv coord is 1.0!
for (int i4 = 0; i4<size1; i4++) {
fprintf(outf, "\n");
fprintf(outf, "vt 0.000000 0.000000\n");
fprintf(outf, "vt 0.000000 1.000000\n");
fprintf(outf, "vt 1.000000 1.000000\n");
fprintf(outf, "vt 1.000000 0.000000\n");
}
#endif;

// Write polygons
// N of polys based on previous max vertex counter.
// Vt params must be + size1*4, Size1 must be from previous object!
//vtExtend = size1*4; //=4 If it's quadrangle
#if 0;
if (size2==0)
{
	for (int b1=0; b1<size1; b1++) {// If block 2 == 0, set default vt params.

	fprintf(outf, "usemtl %d\n", arrayTextureN[b1]);
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d %d %d %d\n", 
		arrayVertexN[arrayPolyOff1[b1]]+1 + counter,
		arrayVertexN[arrayPolyOff1[b1]+1]+1 + counter,
		arrayVertexN[arrayPolyOff1[b1]+2]+1 + counter,
		arrayVertexN[arrayPolyOff1[b1]+3]+1 + counter 
			);
	}
}
else
#endif;

{
	for (int b=0; b<size1; b++) {

		if (remainder==1) {
			fprintf(outf, "usemtl %d\n", ((arrayTextureN[b]+111)*10));
		} else {
			fprintf(outf, "usemtl %d\n", arrayTextureN[b]);
		}
	fwrite("f ",2,1,outf);
	fprintf(outf, "%d/%d %d/%d %d/%d %d/%d\n", 
		arrayVertexN[arrayPolyOff1[b]]+1 + counter,
			vtcounter,
		arrayVertexN[arrayPolyOff1[b]+1]+1 + counter,
			vtcounter+1,
		arrayVertexN[arrayPolyOff1[b]+2]+1 + counter,
			vtcounter+2,
		arrayVertexN[arrayPolyOff1[b]+3]+1 + counter, 
			vtcounter+3);

	vtcounter += 4;
	}
}
fclose(outf);

// Make material file
///////////////////////////////////////////
FILE *mtlfile;
#if 1;
int c, count;

///////*********////////
#if 0;
// New array size
size_t size = sizeof(arrayTexture) / sizeof(arrayTexture[0]); 
sort(arrayTexture, arrayTexture + size);
count = distance(arrayTexture, unique(arrayTexture, arrayTexture + size)); // unique values quantity.

  for (size_t f = 0; f < size; ++f) { // Show sorted list
    cout << arrayTexture[f] << ' ';
  }
cout << endl << "Unique materials = " << count << endl;
c = count;
//if (c==1) {c+=1;}
#endif;
//////**********////////
// Check for the unique materials.
//	for (int e=0; e<size3; e++) {
//		char *string=new char[255],
//	}

// Open mtlfile for reading, check presence for all arrayTexture[] values.
//	mtlfile = fopen("Obj3D.mtl","r");
#if 0; // ifstream
    ifstream file("Obj3D.mtl"); 
    string s, find;
    char c1;
	bool found = false;

	 while (!file.eof()){ // Fill s string
        file.get(c1);
        s.push_back(c1);
    }

    file.close();

	find = "846491238";
//	itoa(arrayTexture[0],find,10);
int pos;

for (int r1=0; r1<size3; r1++) {
		pos = s.find(find);
//		pos = s.find(itoa(arrayTexture[0],find,10));

	if (pos == -1)
		cout << "not finded" << endl;
	else
		found = true;
		cout << "Found! "<<" Position = " << pos << endl;
}
#endif; // ifstream

// Read all file and seek all arrayTexture[]. If found, this arrayTexture[] shouldn't be written to the mtlfile.
   ifstream file("Obj3D.mtl"); 
    string s;
	char find[256] = {0};
    char c1;
	bool found = false;

	 while (!file.eof()){ // Fill s string
        file.get(c1);
        s.push_back(c1);
    }
    file.close();


//for (int c=0; c < count; c++) {
	for (int c=0; c < size3; c++) {


//		find = "892350512";
		strcpy(find,itoa(arrayTexture[c],find,10)); 
int pos;
		pos = s.find(find);
//		pos = s.find(itoa(arrayTexture[c],find,10));

	if (pos == -1)
		cout << "Material entry not found. Adding material to mtlfile." << endl;
	else {
		found = true;
//		cout << "Found! Position = " << pos << endl;
	}

	if (found == true) continue;///////////////////////// CHECK!!!!!!!!!!!!!!
		mtlfile = fopen("Obj3D.mtl","a");
		if (flag3DO == true) {	
			fprintf(mtlfile, "newmtl %d\n", c);
//			fprintf(mtlfile, "map_Kd ThreeDOObject3DTextures\\Tex%d.bmp\n", arrayTexture[c]);
//			cout << FamFileName3DO << endl;
			//ExtractObject3D3DOTexture(BitmapN, FamFileName3DO, offset2SHPIdir, arrayTexture[c]);
			} else {
		
	if (found == true) continue;
		if (remainder==1) {
				fprintf(mtlfile, "newmtl %d\n", ((c+111)*10)); 
				fprintf(mtlfile, "map_Kd ObjectTextures3D\\2\\Tex%d.bmp\n", arrayTexture[c]);
			} else {
				fprintf(mtlfile, "newmtl %d\n", c);
				fprintf(mtlfile, "map_Kd ObjectTextures3D\\Tex%d.bmp\n", arrayTexture[c]);
				}
		// Extract object textures 
//		ExtractObjectTexture(arrayTexture[c], infilename);
		offset2SHPIdir = offtoORIPabs+chunkLength;
//		cout << "offset2SHPIdir= 0x" << hex << offset2SHPIdir << endl;
		ExtractObject3DTexture(BitmapN, infilename, offset2SHPIdir, arrayTexture[c], remainder);
		}
	fclose(mtlfile);
	found = false;
	}
#endif;
///////////////////////////////////////////
// Seek SHPI section under ORIP and extract textures.
//fseek(famFile2, offtoORIPabs+chunkLength, SEEK_SET); // Set to the start of 9th block
//	fread(&temp, 4, 1, famFile2); // test.
//	cout << "temp= 0x" << hex << temp << endl;
// SHPI found. Seek one by one all arrayTexture[].
//while (temp != arrayTexture[0])

//	fread(&temp, 4, 1, famFile2);
fclose(famFile2);
// Block 1
delete [] arrayPolyType;
delete [] arrayTextureN;
delete [] arrayPolyOff1;
delete [] arrayPolyOff2;
// Block 2
if ((!arrayXcoord)||(!arrayYcoord)) {
delete [] arrayXcoord;
delete [] arrayYcoord;
}
// Block 3
delete [] arrayTexture;
// Block 5
delete [] arrayPolyOffsetINBlock9;
delete [] arrayNumOfVertices;
// Block 8
delete [] arrayX;
delete [] arrayZ;
delete [] arrayY;
// Block 9
delete [] arrayVertexN;
//cout << "Bitmap = " << dec << BitmapN << "\n";
#endif;
//Obj3Dparams->size1 = size1;
//Obj3Dparams->size8 = size8;
size13DObj = size13DObj+size1;
size83DObj = size83DObj+size8;
//	cout << "\nsize13DObj3D= " << size13DObj << endl;
//	cout << "\nsize83DObj3D= " << size83DObj << endl;
return size8;
}