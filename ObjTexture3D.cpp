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


#if 1;
typedef struct {// RGB palette
	unsigned char  red;
	unsigned char  green;
	unsigned char  blue;
} palette;

typedef struct {// Real RGB for BMP
	unsigned char  red;
	unsigned char  green;
	unsigned char  blue;
} RGB;
#endif;

// Extract 3D object's texture from *.FAM file
// We need 4th chunk.
void ExtractObject3DTexture(int textN, char* infilename, int offset2SHPIdir, int textureValue, int subchunk) {
	int subChunkN, temp2, count, offset2SHPI, chunkCount, strText, shpiCount, pixOffset, palOffset, palW, palH, wwwwOffset, remainder, remainder2;
	unsigned short int width, height, temp3, width2, height2;
	unsigned char pix[1000000] = { 0 };
	unsigned char pixtemp, rgbtemp;
	unsigned char val;
	char filename[40], bmpname[40], tmpbmp[40], bmpname2[40], tmpbmp2[40];
	FILE* famFile, * bmpFile, * pixfile;
	static palette* pal;
	static RGB* rgb;
int widthcounter;
int heightcounter;

	palette* arrRGB = new palette[1000000];
	palette* realArrRGB = new palette[1000000];

	pal = (palette*)malloc(0x400000UL);
	rgb = (RGB*)malloc(0x400000UL);

//cout << "\nsubchunk " << dec << subchunk << "\n";
//DEBUG******			cout << "\ntextN " << dec << textN << "\n";
strText = 0x30303030; // '0000' 

#if 1;
	char tmpstr2[10];
	char tmpstr3[11];
	for (int f=0; f<sizeof(tmpstr3); f++){
		tmpstr3[f] = 0;
	}
strncpy(tmpstr2, infilename, 3); //скопировать 5 символов строки argv[1] в строку tmpstr2. Получилось "MAP01"
//cout << "infilename - " << infilename << "\n";
strcat(tmpstr2, "_001.FAM");
strncpy(tmpstr3, tmpstr2, 11);
//cout << "tmpstr2 - " << tmpstr2 << "\n";
//cout << "tmpstr3 - " << tmpstr3 << "\n";
famFile = fopen(tmpstr3, "rb");
#endif;
//famFile = fopen("AL1_001.FAM", "rb");
fseek(famFile, 0, SEEK_SET);
#if 0;
	// Skip 8 bytes.
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);
	chunkCount = temp2;

	// Read 2nd chunk offset2nd.
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);

	// Set to the offset2nd and skip "wwww". 
	fseek(famFile, temp2, SEEK_SET); // Set
	wwwwOffset = temp2;
	fread(&temp2, 4, 1, famFile);


	// Read count SHPI indexes.
	fread(&temp2, 4, 1, famFile);
	count = temp2;
//cout << "\ncount 0x" << hex << count << "\n";

// Read relative offset to the needed SHPI index. subChunkN = number of subchunk.
	for (int i = 0; i <= textN; i++) {
		fread(&temp2, 4, 1, famFile);
		offset2SHPI = temp2;
	}
	//DEBUG******cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";


	// Set back to the "wwww", jump to the offset2SHPI from there. 
	fseek(famFile, wwwwOffset, SEEK_SET); // Set to the 

	// Jump to the offset2SHPI
	fseek(famFile, offset2SHPI, SEEK_CUR);
//	fread(&temp2, 4, 1, famFile); // SHPI
//DEBUG******	cout << "SHPI = 0x" << hex << temp2 << "\n";
#endif;
fseek(famFile, offset2SHPIdir, SEEK_SET);
//cout << "offset2SHPIdir = 0x" << hex << offset2SHPIdir << "\n";
//cout << "textureValue = 0x" << hex << textureValue << "\n";

fread(&temp2, 4, 1, famFile);
fread(&temp2, 4, 1, famFile);
fread(&temp2, 4, 1, famFile);
shpiCount = temp2;

//cout << "shpiCount = 0x" << hex << shpiCount << "\n";

// Finding pixOffset and palOffset
// Read file intil textureValue or !pal (!PAL)
for (int j1 = 0; j1 < shpiCount*10; j1++) {// #10 is just an example, not a correct #
	fread(&temp2, 4, 1, famFile);
		if (temp2 == textureValue) {
			fread(&temp2, 4, 1, famFile);
			pixOffset = temp2;// Relative offset to pixel data
//			cout << "--------pixOffset 0x" << hex << pixOffset << "\n";
			}
}

// Jump to pixOffset and read all pixel data.
// Set to the pixOffset.
	fseek(famFile, offset2SHPIdir + pixOffset, SEEK_SET);

	// Check 0x7B as entry for the pixel data.
	// Skip 3 bytes
	fread(&temp2, 4, 1, famFile);
//	cout << "pix Start = 0x" << hex << temp2 << "\n";

// Reading texture width 
	fread(&temp3, 2, 1, famFile);
	width = temp3;
	width2 = temp3;
//DEBUG******	cout << "width = " << dec << width << "\n";
	if (width > 0x280) return;// Brute hack! Remove after solving!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Reading texture height
	fread(&temp3, 2, 1, famFile);
	height = temp3;
	height2 = temp3;
//DEBUG******	cout << "height = " << dec << height << "\n";
	if (height > 0x280) return;// Brute hack! Remove after solving!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

const int round = 4;

remainder = width % round;
remainder2 = height % round;
//DEBUG******cout << "=====================remainder = " << dec << remainder << "\n";
//DEBUG******cout << "=====================remainder2 = " << dec << remainder2 << "\n";

	if (remainder == 1) {
		width2 = (int)width / round;
		width2 = width2 * round + 4;
		//DEBUG******		cout << "new width = " << dec << width2 << "\n";
		}

	if (remainder == 2) {
		width2 = (int)width / round;
		width2 = width2 * round + 4;
		//DEBUG******		cout << "new width = " << dec << width2 << "\n";
		}


	if (remainder == 3) {
		width2 = (int)width / round;
		width2 = width2 * round + 4;
		//DEBUG******		cout << "new width = " << dec << width2 << "\n";
		}


	if (remainder2 == 1) {
		height2 = (int)height / round;
		height2 = height2 * round + 4;
		//DEBUG******		cout << "new height = " << dec << height2 << "\n";
		}


	if (remainder2 == 2) {
		height2 = (int)height / round;
		height2 = height2 * round + 4;
		//DEBUG******		cout << "new height = " << dec << height2 << "\n";
		}


	if (remainder2 == 3) {
		height2 = (int)height / round;
		height2 = height2 * round + 4;
		//DEBUG******		cout << "new height = " << dec << height2 << "\n";
		}

	// Skip 8 bytes
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);

	// Reading pixel data. Width*Height. 1 pixel = 1 byte. Add all pixels to the array.
#if 1; // array
	int g = 0;
	for (g; g < height * width; g++) {
		fread(&pixtemp, 1, 1, famFile);
		pix[g] = pixtemp;
	}
#endif; // array



/// PALETTE
	FILE* palfile;
	unsigned char tmp11;
	bool palfound = false;

	// Seeking the palette.
fseek(famFile, offset2SHPIdir, SEEK_SET);

palOffset = 24;// If palette exists, but not listed in the header. Default value is 24 (0x18)

	for (int h = 0; h <= 8; h++) { // !PAL = 0x4C415021	!pal = 0x6C617021
		fread(&temp2, 4, 1, famFile);
		if ((temp2 == 0x6C617021) || (temp2 == 0x4C415021)) {
			fread(&temp2, 4, 1, famFile);
			palOffset = temp2;
//DEBUG******							cout << "--------palOffset = 0x" << hex << palOffset << "\n";
			palfound = true;
			break;
		}
	}

	// Set to the palette beginning
fseek(famFile, offset2SHPIdir + palOffset, SEEK_SET);

#if 0; // Palette not in header
if (palfound == false) {
	// Set defaults
// If palette exists, but not listed in the header.
palW = 0x100; 
palH = 0x3;

//cout << "pixOffset = " << dec << pixOffset << "\n";
if (pixOffset>=0x60 && pixOffset<0x400) {
int off2palette = 0;
off2palette = wwwwOffset+offset2SHPI+pixOffset-0x300;

//DEBUG******		cout << "wwwwOffset = 0x" << hex << wwwwOffset << "\n";
//DEBUG******		cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";
//DEBUG******		cout << "pixOffset = 0x" << hex << pixOffset << "\n";

	fseek(famFile, off2palette, SEEK_SET);
//DEBUG******				cout << "off2palette = 0x" << hex << off2palette << "\n";
		} else 

 {
int off2palette = 0;
		off2palette = wwwwOffset+offset2SHPI+pixOffset+width*height+32;
	fseek(famFile, off2palette, SEEK_SET);
//DEBUG******			cout << "off2palette = 0x" << hex << off2palette << "\n";
}
	// Jump to reading palette data
	goto withoutPAL; 
} 
#endif;// Palette not in header

	// Skip 4 bytes
	fread(&temp2, 4, 1, famFile);

	// Read palette width
	fread(&temp3, 2, 1, famFile);
	palW = temp3;
//DEBUG******			cout << "palW = 0x" << hex << palW << "\n";

	// Read palette height
	fread(&temp3, 2, 1, famFile);
	palH = temp3;
//DEBUG******			cout << "palH = 0x" << hex << palH << "\n";

	// Skip 8 bytes
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);


	// Reading RGB palette values. 3 bytes per pixel. Forward to 256*3=768 bytes
	// Reading order: BLUE, GREEN, RED.
#if 1;
withoutPAL:
for (int f = 0; f < 256; f++) {

#if 1;
	fread(&val, 1, 1, famFile);
		arrRGB[f].blue = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].green = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].red = val << 2;
#endif;
	}
#endif;

// Calculate real RGB values based on palette.
int index=0;
for (int a=0; a < height*width; a++) {
//for (int a=0; a < width*2; a++) {
	index = pix[a];
	realArrRGB[a].red = arrRGB[index].red;
	realArrRGB[a].green = arrRGB[index].green;
	realArrRGB[a].blue = arrRGB[index].blue;
}
/////////////////////////////////--------------------------------------------------------------------------------------------------------------------------

#if 1;
// Saving BMP file.
//tmpbmp = "Tex";
itoa(textureValue, bmpname, 10);

if (subchunk==1) {
	strcpy(tmpbmp, "\ObjectTextures3D\\2\\Tex");
	} else {
		strcpy(tmpbmp, "\ObjectTextures3D\\Tex");
	}

//strcpy(tmpbmp, "\ObjectTextures3D\\Tex");
strcat(bmpname, ".bmp");
strcat(tmpbmp, bmpname);
//cout << tmpbmp << "\n";


// Calculate BMP header.
// Filesize = 54 bytes + width*height*3
int filesize;

filesize = 54 + width2*height*3;
//filesize = 54 + width*height*3 + remainder*3 + remainder2*3;

// Save header.
FILE * bmp;
unsigned char tmp;
int temp4;
short int temp5;

bmp = fopen(tmpbmp, "wb");
fclose(bmp);

bmp = fopen(tmpbmp, "ab");
	tmp = 0x42;
	fwrite(&tmp, 1, 1, bmp);
	tmp = 0x4D;
	fwrite(&tmp, 1, 1, bmp);

	fwrite(&filesize, 4, 1, bmp); // Write filesize

	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write empty data
	temp4 = 0x36;
	fwrite(&temp4, 4, 1, bmp); // Write 122
	temp4 = 0x28;
	fwrite(&temp4, 4, 1, bmp); // Write 108

	temp4 = width2;
	fwrite(&temp4, 4, 1, bmp); // Write width
	temp4 = height;
	fwrite(&temp4, 4, 1, bmp); // Write height

	temp5 = 1;
	fwrite(&temp5, 2, 1, bmp); // Number of color planes being used
	temp5 = 0x18;
	fwrite(&temp5, 2, 1, bmp); // Number of bits per pixel

	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write empty data

	temp4 = width2*height*3;
	fwrite(&temp4, 4, 1, bmp); // Write raw image data

	temp4 = 0xB12;
	fwrite(&temp4, 4, 1, bmp); // Write print res
	temp4 = 0xB12;
	fwrite(&temp4, 4, 1, bmp); // Write print res	
	
	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write Number of colors in the palette
	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // 0 means all colors are important


// Save real pixel data.
widthcounter = width;
heightcounter = height;
int rowN = 0;

for (int h=0; h < height*width2; h++) {
//cont:
// every width line skip remainder pixels.
	
	if ((h % width*3 == 0)&&(remainder == 1)&&(h != 0)) {
		// skip 4 bytes.
//			cout << dec << h << "\n";
			val = 0;
			fwrite(&val, 3*3, 1, bmp); // Add zeros.
//			return;
//			h++;
//			goto cont;

		} 

	if ((h % width * 3 == 0) && (remainder == 2) && (h != 0)) {
		// skip 4 bytes.
		val = 0;
		fwrite(&val, 2 * 3, 1, bmp); // Add zeros.
	}

	if ((h % width * 3 == 0) && (remainder == 3) && (h != 0)) {
		// skip 4 bytes.
		val = 0;
		fwrite(&val, 3, 1, bmp); // Add zeros.
	}

	fwrite(&realArrRGB[h].red, 1, 1, bmp);
	fwrite(&realArrRGB[h].green, 1, 1, bmp);
	fwrite(&realArrRGB[h].blue, 1, 1, bmp);
	
cont:
rowN++;
//cout << dec << rowN << "\n";
}


#if 1;  // PALETTE
//Save palette
itoa(textN, bmpname2, 10);

//if (subchunk==1) {
//	strcpy(tmpbmp, "\ObjectTextures3D\\2\\Palette");
//	} else {
	strcpy(tmpbmp2, "\ObjectTextures3D\\Palette");
//	 }
strcat(bmpname2, ".bmp");
strcat(tmpbmp2, bmpname2);
//cout << tmpbmp << "\n";


// Calculate BMP header.
// Filesize = 54 bytes + width*height*3
int filesize2;

filesize2 = 54 + palW;


// Save header.
FILE * bmp2;
unsigned char tmp2;
int temp41;
short int temp51;

bmp2 = fopen(tmpbmp2, "wb");
fclose(bmp2);

bmp2 = fopen(tmpbmp2, "ab");
	tmp2 = 0x42;
	fwrite(&tmp2, 1, 1, bmp2);
	tmp2 = 0x4D;
	fwrite(&tmp2, 1, 1, bmp2);

	fwrite(&filesize2, 4, 1, bmp2); // Write filesize

	temp41 = 0;
	fwrite(&temp41, 4, 1, bmp2); // Write empty data
	temp41 = 0x36;
	fwrite(&temp41, 4, 1, bmp2); // Write 122
	temp41 = 0x28;
	fwrite(&temp41, 4, 1, bmp2); // Write 108

	temp41 = 16;
	fwrite(&temp41, 4, 1, bmp2); // Write width
	temp41 = 16;
	fwrite(&temp41, 4, 1, bmp2); // Write height

	temp51 = 1;
	fwrite(&temp51, 2, 1, bmp2); // Number of color planes being used
	temp51 = 0x18;
	fwrite(&temp51, 2, 1, bmp2); // Number of bits per pixel

	temp41 = 0;
	fwrite(&temp41, 4, 1, bmp2); // Write empty data

	temp41 = 256;
	fwrite(&temp41, 4, 1, bmp2); // Write raw image data

	temp41 = 0xB12;
	fwrite(&temp41, 4, 1, bmp2); // Write print res
	temp41 = 0xB12;
	fwrite(&temp41, 4, 1, bmp2); // Write print res	
	
	temp41 = 0;
	fwrite(&temp41, 4, 1, bmp2); // Write Number of colors in the palette
	temp41 = 0;
	fwrite(&temp41, 4, 1, bmp2); // 0 means all colors are important


// Save real pixel data.
for (int h1=0; h1 < 256; h1++) {
//for (int h1=0; h1 < 300; h1++) {
	fwrite(&arrRGB[h1].red, 1, 1, bmp2);
	fwrite(&arrRGB[h1].green, 1, 1, bmp2);
	fwrite(&arrRGB[h1].blue, 1, 1, bmp2);
}
fclose(bmp2);
#endif; // PALETTE


fclose(bmp);
#endif;

free(pal);
free(rgb);
delete[]arrRGB;
delete[]realArrRGB;
fclose(famFile);
}