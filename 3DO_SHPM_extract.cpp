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
#include "3DO_SHPM_extract.h"
//#include "NFS_TRI2OBJ.h"
using namespace std;

//VirtRd_t VIRTUALROADARRAY[numVirtRdIndexes];
//extern int Extract3DOSHPM(int textN, char* infilename, int type, int lod, unsigned int offsetSHPMabs, unsigned int polygonTexture, unsigned int subchunk); // extract 3DO texture (cars and 3D objects)


int Extract3DOSHPM(int textN, char* infilename, int type, int lod, unsigned int offsetSHPMabs, unsigned int polygonTexture, unsigned int subchunk) {
//cout << "Extracting SHPM " << textN << " " << infilename << " " << type << " " << lod << " " << hex << offsetSHPMabs << " " << polygonTexture <<"\n";
int temp, offset2texture, offsetABS;

#if 0;
cout << "VergeLeft " << VIRTUALROADARRAY[textN].VergeLeft <<"\n"; // Just for test!!!
cout << "VergeRight " << VIRTUALROADARRAY[textN].VergeRight <<"\n";
cout << "BarrierLeft " << VIRTUALROADARRAY[textN].BarrierLeft <<"\n";
cout << "BarrierRight " << VIRTUALROADARRAY[textN].BarrierRight <<"\n";
cout << "Flag1 " << VIRTUALROADARRAY[textN].Flag1 <<"\n";
cout << "Flag2 " << VIRTUALROADARRAY[textN].Flag2 <<"\n";
cout << "Flag3 " << VIRTUALROADARRAY[textN].Flag3 <<"\n";
cout << "Flag4 " << VIRTUALROADARRAY[textN].Flag4 <<"\n";
cout << "X " << VIRTUALROADARRAY[textN].X <<"\n";
cout << "Y " << VIRTUALROADARRAY[textN].Y <<"\n";
cout << "Z " << VIRTUALROADARRAY[textN].Z <<"\n";
cout << "slope " << VIRTUALROADARRAY[textN].slope <<"\n";
cout << "slantA " << VIRTUALROADARRAY[textN].slantA <<"\n";
cout << "orientation " << VIRTUALROADARRAY[textN].orientation <<"\n";
cout << "zero " << VIRTUALROADARRAY[textN].zero <<"\n";
cout << "Yorientation " << VIRTUALROADARRAY[textN].Yorientation <<"\n";
cout << "slantB " << VIRTUALROADARRAY[textN].slantB <<"\n";
cout << "Xorientation " << VIRTUALROADARRAY[textN].Xorientation <<"\n";
cout << "zero1 " << VIRTUALROADARRAY[textN].zero1 <<"\n";
#endif;

//cout << "subchunk " << subchunk <<"\n";

// Open file
FILE *infile;
infile = fopen(infilename, "rb");
fseek(infile, offsetSHPMabs, SEEK_SET);
	fread(&temp, 4, 1, infile);
	fread(&temp, 4, 1, infile);
	fread(&temp, 4, 1, infile);
	fread(&temp, 4, 1, infile);
	

fread(&temp, 4, 1, infile);
fread(&temp, 4, 1, infile);
offset2texture = Swap4Bytes(temp);
//cout << "PolygonN " << polygonTexture <<"\n";
//cout << "Local offset within the SHPM to texture " << hex << offset2texture << "\n";

if (polygonTexture > 0) {
	for (int i = 0; i<polygonTexture; i++) {
		fread(&temp, 4, 1, infile);
		fread(&temp, 4, 1, infile);
		offset2texture = Swap4Bytes(temp);
//		cout << "PolygonN " << polygonTexture <<"\n";
//		cout << "Local offset within the SHPM to texture " << hex << offset2texture << "\n";
	}
}

offsetABS = offset2texture+offsetSHPMabs;
//cout << "offsetABS 0x" << hex << offsetABS << "\n";

//cout << "Done!\n";

// Seek corresponding texture
fseek(infile, offsetABS, SEEK_SET);

// Read pixel data header
unsigned short int ccbtype, off2palette, height, width, shorttemp, pdatSize, arrsize; 
fread(&shorttemp, 2, 1, infile);
ccbtype =  Swap2Bytes(shorttemp);
//cout << "ccbtype 0x" << hex << ccbtype << "\n";

fread(&shorttemp, 2, 1, infile);
off2palette = Swap2Bytes(shorttemp);
//cout << "off2palette 0x" << hex << off2palette << "\n";

fread(&shorttemp, 2, 1, infile);
width = Swap2Bytes(shorttemp);

fread(&shorttemp, 2, 1, infile);
height = Swap2Bytes(shorttemp);

fread(&temp, 4, 1, infile);
fread(&temp, 4, 1, infile);
//cout << "temp = " << hex <<  temp <<"\n";
//cout << "height = " << dec <<  height <<"\n";
//cout << "width = " << dec << width <<"\n\n";


// Read pixel data
fseek(infile, offsetABS+0x10, SEEK_SET);
pdatSize = off2palette - 0x10;
arrsize = pdatSize/4;
int *arrayPDAT = new int [arrsize];

for (int k=0; k < arrsize; k++) {
	fread(&temp, 4, 1, infile);
	arrayPDAT[k] = Swap4Bytes(temp);
//	cout << "arrayPDAT = 0x" << hex <<  arrayPDAT[k] <<" \n";
	}

// Read palette data
int *arrayPLUT = new int [16];

fread(&temp, 4, 1, infile); // skip
fread(&temp, 4, 1, infile); // skip
fread(&temp, 4, 1, infile); // skip
fread(&temp, 4, 1, infile); // skip

for (int k1=0; k1 < 16; k1++) {
	fread(&temp, 4, 1, infile);
	arrayPLUT[k1] = Swap4Bytes(temp);
//	cout << "arrayPLUT = 0x" << hex <<  arrayPLUT[k1] <<" \n";
	}

//cout << " \n";

// Make new texture file
char bmpname[40], tmpbmp[40];
int temp111;

if (subchunk==1) {
	temp111 = (polygonTexture + 111)*10;
} else {
	temp111 = polygonTexture;
	}

itoa(temp111, bmpname, 10);
FILE* cel;

if (subchunk==1) {
	strcpy(tmpbmp, "\ThreeDO3DobjTextures\\2\\Tex");
//	cout << tmpbmp << endl;
	} else 

{
strcpy(tmpbmp, "\ThreeDO3DobjTextures\\Tex");
	}
//strcat(bmpname, ".cel");
strcat(tmpbmp, bmpname);
//cout << tmpbmp << endl;

cel = fopen(tmpbmp, "wb");
fclose(cel);
cel = fopen(tmpbmp, "ab");

// Write all data to file
// Generate and write CCB header
temp = 0x43434220;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel); 
temp = 0x50000000;
fwrite(&temp, 4, 1, cel); 
temp = 0;
fwrite(&temp, 4, 1, cel);
temp = 0x4000;
fwrite(&temp, 4, 1, cel);
temp = 0;
for (int k2=0; k2 < 12; k2++) {
	fwrite(&temp, 4, 1, cel);
	}
temp = 0x34D34924;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);
temp = 0;
fwrite(&temp, 4, 1, cel);
temp = width;			// width
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);
temp = height;			// height
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);
temp = 0x50444154;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);
temp = pdatSize + 8; // 8 - occupied by PDAT and offset to PLUT
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);
// Write PDAT data
for (int i1=0; i1 < arrsize; i1++) {
	temp = Swap4Bytes(arrayPDAT[i1]);
	fwrite(&temp, 4, 1, cel);
	}


// Generate and write PLUT header
temp = 0x504C5554;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel); 
temp = 0x0000004C;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel); 
temp = 0x00000020;
temp = Swap4Bytes(temp);
fwrite(&temp, 4, 1, cel);

// Write PLUT data
for (int i2=0; i2 < 16; i2++) {
	temp = Swap4Bytes(arrayPLUT[i2]);
	fwrite(&temp, 4, 1, cel);
	}



// Close new file
fclose(cel);

fclose(infile);
delete[]arrayPDAT;
delete[]arrayPLUT;
return 0;
}