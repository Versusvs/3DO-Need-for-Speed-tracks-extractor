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


void Extract3DOHorz(char *infilename) { // Extract and save 3DO horizon texture to ThreeDOSceneryTextures folder

}


void Extract3DOSCNTexture(int textN, char* FamFileName3DO, int chunkN) {
////	cout << "Extracting scenery cel texture #" << dec << textN << endl;
// Scenery textures - 1st chunk
// Object textures - 2nd chunk
// 3D objects - 3rd chunk
// Horizon textures - 4th chunk (bottom)
// Horizon textures - 5th chunk (top)
FILE* famFile;
FILE* cel;
int chunk1Off = 0;// Offset to chunk1
int wwwwOffset = 0;// Offset to wwww
int localCCBOffset = 0; // Offset to CCB within wwww
int off2CCB=0; // Absolute offset to CCB 
int celN = 0;
int firstwwww = 0;
int temp, temp2;
char bmpname[40], tmpbmp[40];

int j=0;
fpos_t pos;

//if (textN>30) return;///////////// BRUTE HACK!!!!!!!!!!!!!!!!!!!

// Find specific cel in 1st chunk
// To find offset to the cel just *4 textN. It will be the number of the cel within the 2nd chunk.
//celN = textN/3;//////////// MAKE THIS!!!!!!!!!!!!!!!!!!!!
//Read famfile3DO.
//cout << FamFileName3DO << endl;
// Seek 2nd chunk for the Objects, 1st chunk for scenery textures
//cout << "chunkN = " << dec << chunkN << endl;

famFile = fopen(FamFileName3DO, "rb");
fseek(famFile, 0, SEEK_SET);
	fread(&temp, 4, 1, famFile);
	fread(&temp, 4, 1, famFile);
	fread(&temp, 4, 1, famFile);

	chunk1Off = Swap4Bytes(temp); // 1st chunk offset
//cout << "chunk1Off = 0x" << hex << chunk1Off << endl;

// Set to the 1st chunk.
fseek(famFile, chunk1Off, SEEK_SET);

	fread(&temp, 4, 1, famFile); // skip wwww header
	fread(&temp, 4, 1, famFile); // skip N of entries
	fread(&temp, 4, 1, famFile); // first wwww subchunk
	firstwwww = Swap4Bytes(temp);
//celN = -1;
// Seek for subchunk offset
//fseek(famFile, firstwwww-4, SEEK_SET);
	for (int i=0; i<textN; i++) {
		fread(&temp, 4, 1, famFile); 
		wwwwOffset = Swap4Bytes(temp);
		if (wwwwOffset==0) {
			wwwwOffset = firstwwww;
			}
		}
//cout << "wwwwOffset = 0x" << hex << wwwwOffset << endl;

// Seek first texture in the wwww subchunk
fseek(famFile, chunk1Off + wwwwOffset, SEEK_SET);
fread(&temp, 4, 1, famFile); // skip wwww header
fread(&temp, 4, 1, famFile); // skip N of entries
fread(&temp, 4, 1, famFile); // first CCB offset within wwww subchunk /////////////// DEFINE SUBCHUNK FOR SCENERY???

localCCBOffset = Swap4Bytes(temp);
//cout << "localCCBOffset = 0x" << hex << localCCBOffset << endl;

off2CCB = chunk1Off + wwwwOffset + localCCBOffset;
//cout << "off2CCB = 0x" << hex << off2CCB << endl << endl;


// Set to the CCB:
fseek(famFile, off2CCB, SEEK_SET); // For 1st chunk

	fread(&temp, 4, 1, famFile);
	temp2 = temp;
//	cout << "CCB = 0x" << hex << temp << endl;
// Store all data from CCB to !EOF or next CCB.
itoa(textN, bmpname, 10);
strcpy(tmpbmp, "\ThreeDOSceneryTextures\\Tex");
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
Extract3DOHorz(infilename);
//cout << endl;
}
