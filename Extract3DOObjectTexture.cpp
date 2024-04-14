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



void Extract3DOObjTexture(int textN, char* FamFileName3DO, int chunkN) {
//	cout << "Extracting object cel texture #" << dec << textN << endl;
// Scenery textures - 1st chunk
// Object textures - 2nd chunk
// 3D objects - 3rd chunk
// Horizon textures - 4th chunk (bottom)
// Horizon textures - 5th chunk (top)
FILE* famFile;
FILE* cel;
int chunk2Off = 0;
int celOffset = 0;
int celN = 0;
int temp, temp2;
char bmpname[40], tmpbmp[40];
int off2CCB=0;
int j=0;
fpos_t pos;


// Find specific cel in 2nd chunk
// To find offset to the cel just *4 textN. It will be the number of the cel within the 2nd chunk.
//cout << textN << endl;
celN = textN*4;
//Read famfile3DO.
//cout << FamFileName3DO << endl;
// Seek 2nd chunk for the Objects, 1st chunk for scenery textures
//cout << "chunkN = " << dec << chunkN << endl;

famFile = fopen(FamFileName3DO, "rb");
fseek(famFile, 0, SEEK_SET);
	fread(&temp, 4, 1, famFile);
	fread(&temp, 4, 1, famFile);
	fread(&temp, 4, 1, famFile);
	fread(&temp, 4, 1, famFile);

	chunk2Off = Swap4Bytes(temp); // 2nd chunk offset


// Set to the 2nd chunk.
fseek(famFile, chunk2Off, SEEK_SET);

for	(int i=0; i<=(celN+2); i++) { 
	
	fread(&temp, 4, 1, famFile);
	if (temp == 0) {
//		i++;
		}
	celOffset = Swap4Bytes(temp);
}

//	cout << "chunkOff = 0x" << hex << chunk2Off << endl;
//	cout << "celOffset = 0x" << hex << celOffset << endl;

off2CCB = chunk2Off + celOffset;
//	cout << "off2CCB = 0x" << hex << off2CCB << endl;
// Set to the CCB:

fseek(famFile, off2CCB, SEEK_SET); // For 2nd chunk

	fread(&temp, 4, 1, famFile);
	temp2 = temp;
//	cout << "CCB = 0x" << hex << temp << endl;
// Store all data from CCB to !EOF or next CCB.
itoa(textN, bmpname, 10);
strcpy(tmpbmp, "\ThreeDOObjTextures\\Tex");
//strcat(bmpname, ".cel");
strcat(tmpbmp, bmpname);
//cout << tmpbmp << endl;

cel = fopen(tmpbmp, "wb");
fclose(cel);
cel = fopen(tmpbmp, "ab");

	fwrite(&temp2, 4, 1, cel); // Write Header

while (!feof(famFile)) {
	fread(&temp, 4, 1, famFile);
		temp2 = temp;
		if (temp == 0x20424343) {
		// next CCB is found. Stop!	
			break;
		}
	fwrite(&temp, 4, 1, cel); // Write 
	}

closefile:
fclose(cel);

// Convert cel to bmp and save to ThreeDOObjTextures folder.

fclose(famFile);
//cout << endl;
}