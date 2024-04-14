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

///extern palette *averagePAL = new palette[100000];
//extern palette* avePAL;
//avePAL = (palette *)malloc(0x400000UL);

// Extract texture from *.FAM file
void ExtractScenTexture(int textN, char* infilename) {
	int subChunkN, remainder, temp2, count, offset2SHPI, chunkCount, strText, strText2, shpiCount, pixOffset, palOffset, palW, palH, wwwwOffset;
	unsigned short int width, height, temp3;
	unsigned char pix[100000] = { 0 };
	//unsigned char red[100000] = {0};
	//unsigned char green[100000] = {0};
	//unsigned char blue[100000] = {0};
	unsigned char pixtemp, rgbtemp;
	unsigned char val;
	char tmpstr2[10];
	char tmpstr3[11];
	char filename[40], bmpname[40], tmpbmp[40], bmpname2[40], tmpbmp2[40];
	//char *tmpbmp;
	FILE* famFile, * bmpFile, * pixfile;
	static palette* pal;
	static RGB* rgb;

	palette* arrRGB = new palette[100000];
	palette* realArrRGB = new palette[100000];

	pal = (palette*)malloc(0x400000UL);
	rgb = (RGB*)malloc(0x400000UL);
	//arrRGB = (arrRGB *)malloc(0x1000000UL);

	///if ((textN == 44) || (textN == 16)) return; 


//DEBUG******				cout << "\ntextN " << dec << textN << "\n";

	// Convert textN to subChunkN. 
	subChunkN = textN / 3;
	remainder = textN % 3;
//DEBUG******				cout << "subChunkN " << dec << subChunkN << "\n";
//DEBUG******				cout << "remainder " << dec << remainder << "\n";

#if 0;
	// If remainder = 0, A string needed.
	if (remainder == 0) { textStr[0] = 'A000'; } // 'A000' 
	// If remainder = 1, B string needed.
	if (remainder == 1) { textStr[0] = 'B000'; } // 'B000'
	// If remainder = 2, C string needed.
	if (remainder == 2) { textStr[0] = 'C000'; } // 'C000'
#endif;

#if 1;
// If remainder = 0, A string needed.
	if (remainder == 0) {
		strText = 0x30303041; // 'A000' 
		strText2 = 0x31303041;// 'A001' 
	}
	// If remainder = 1, B string needed.
	if (remainder == 1) {
		strText = 0x30303042;// 'B000'
		strText2 = 0x31303042;// 'B001'
	}
	// If remainder = 2, C string needed.
	if (remainder == 2) {
		strText = 0x30303043;// 'C000'
		strText2 = 0x31303043;// 'C001'
	}
#endif;

#if 0;
	switch (remainder) {
	case 0: strText = 0x30303041;
	case 1: strText = 0x30303042;
	case 2: strText = 0x30303043;
	}
#endif;
	// So, we'll seek corresponding string within SHPI subchunk later.

	// For the scenery textures we need 1st chunk of the FAM file.
	// Open FAM file for the reading //AL1_001.FAM
	for (int f=0; f<sizeof(tmpstr3); f++){
		tmpstr3[f] = 0;
	}

	for (int f1=0; f1<sizeof(tmpstr2); f1++){
		tmpstr2[f1] = 0;
	}

strncpy(tmpstr2, infilename, 3); //скопировать 5 символов строки argv[1] в строку tmpstr2. Получилось "MAP01"
//cout << "infilename - " << infilename << "\n";
strcat(tmpstr2, "_001.FAM");
strncpy(tmpstr3, tmpstr2, 11);
//cout << "tmpstr2 - " << tmpstr2 << "\n";
//cout << "tmpstr3 - " << tmpstr3 << "\n";
famFile = fopen(tmpstr2, "rb");
//famFile = fopen("AL1_001.FAM", "rb");
	fseek(famFile, 0, SEEK_SET);

	// Skip 8 bytes.
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);
	chunkCount = temp2;

	// Read 1st chunk offset1st.
	fread(&temp2, 4, 1, famFile);
	wwwwOffset = temp2;

	// Set to the offset1st and skip 4 bytes. 
	fseek(famFile, temp2, SEEK_SET); // Set
	fread(&temp2, 4, 1, famFile);

	// Read count SHPI indexes.
	fread(&temp2, 4, 1, famFile);
	count = temp2;
//	cout << "\ncount 0x" << hex << count << "\n";


	// Read relative offset to the needed SHPI index. subChunkN = number of subchunk.
	for (int i = 0; i <= subChunkN; i++) {
		fread(&temp2, 4, 1, famFile);
		offset2SHPI = temp2;
	}
//DEBUG******	cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";


	// Set back to the "wwww", jump to the offset2SHPI from there. 
	fseek(famFile, 8 + chunkCount * 4, SEEK_SET); // Set to the 
	///fread(&temp2,4,1,famFile);
	///cout << "\ntemp2 0x" << hex << temp2 << "\n";

	// Jump to the offset2SHPI
	fseek(famFile, offset2SHPI, SEEK_CUR);
	fread(&temp2, 4, 1, famFile); // SHPI
//DEBUG******	cout << "SHPI = 0x" << hex << temp2 << "\n";

	// Find entry after A000, B000 or C000 strings. A, B or C were defined earlier. "A000" = 0x30303041, "B000" = 0x30303042, "C000" = 0x30303043
	// Define count of text strings within SHPI section
	fread(&temp2, 4, 1, famFile); // Skip 4 bytes
	fread(&temp2, 4, 1, famFile);
	shpiCount = temp2;
	///cout << "\nshpiCount 0x" << hex << shpiCount << "\n";
	fread(&temp2, 4, 1, famFile); // Skip dir string
	// Read forward while find strText. There is a limit reading - shpiCount.
	for (int j = 0; j < shpiCount * 2; j++) {
		fread(&temp2, 4, 1, famFile);
		if (temp2 == strText) {
			fread(&temp2, 4, 1, famFile);
			pixOffset = temp2;
//			cout << "--------pixOffset 0x" << hex << pixOffset << "\n";

		}
	}


	// Set back to the offset2SHPI.
	fseek(famFile, (8 + chunkCount * 4 + offset2SHPI), SEEK_SET);
	//fread(&temp2, 4, 1, famFile); 
	//cout << "SHPI = 0x" << hex << temp2 << "\n";

	// Set to the pixOffset.
	fseek(famFile, (pixOffset), SEEK_CUR);

	// Check 0x7B as entry for the pixel data.
	// Skip 3 bytes
	fread(&temp2, 4, 1, famFile);
	//cout << "pix Start = 0x" << hex << temp2 << "\n";

	// Reading texture width 
	fread(&temp3, 2, 1, famFile);
	width = temp3;
//DEBUG******	cout << "width = 0x" << hex << width << "\n";
	if (width > 0x280) return;// Brute hack! Remove after solving!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Reading texture height
	fread(&temp3, 2, 1, famFile);
	height = temp3;
//DEBUG******	cout << "height = 0x" << hex << height << "\n";
	if (height > 0x280) return;// Brute hack! Remove after solving!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Skip 8 bytes
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);

	// Reading pixel data. Width*Height. 1 pixel = 1 byte. Add all pixels to the array.
#if 1; // temp files
	itoa(textN, filename, 10);
	strcat(filename, ".txt");
	int g = 0;
	pixfile = fopen(filename, "a");
	for (g; g < height * width; g++) {
		fread(&pixtemp, 1, 1, famFile);
		pix[g] = pixtemp;
		fprintf(pixfile, "pix %d ", g);
		fprintf(pixfile, "pixvalue %d", pix[g]);
		fwrite("\n", 1, 1, pixfile);
		//	cout << "pix = 0x" << hex << pix[g] << "\n";
	}
	fclose(pixfile);
	remove(filename); // delete test file 
#endif; // temp files


#if 1; // NEW
	FILE* palfile;
	unsigned char tmp11;
	bool palfound = false;

	// Seeking the palette.
	// Reading palOffset, set back to the found SHPI header, jump to the palOffset.
	fseek(famFile, (8 + chunkCount * 4 + offset2SHPI), SEEK_SET);

	for (int h = 0; h <= shpiCount * 2 + 4; h++) { // !PAL = 0x4C415021	!pal = 0x6C617021
		fread(&temp2, 4, 1, famFile);
		if ((temp2 == 0x6C617021) || (temp2 == 0x4C415021)) {
			fread(&temp2, 4, 1, famFile);
			palOffset = temp2;
//DEBUG******			cout << "palOffset = 0x" << hex << palOffset << "\n";
			palfound = true;
			break;
		}

#if 0; // READING PALETTE
if (palfound == false) { // Palette not found! Assign already existed palette.
			// reading data from palette file and jumping below
			palfile = fopen("palette", "rb");
			for (int z = 0; z < 768; z++) {
				fread(&tmp11, 1, 1, palfile);
				arrRGB[z].blue = tmp11;
				fread(&tmp11, 1, 1, palfile);
				arrRGB[z].green = tmp11;
				fread(&tmp11, 1, 1, palfile);
				arrRGB[z].red = tmp11;
			}
			fclose(palfile);
			goto withoutPAL;
		}
#endif; // READING PALETTE
	}

#if 1; // If palette not found (previous version)
	// If palette not found, read palette at the end of the texture pixel data. 
	// Or palette is not found at the end, seek it at the beginning of the pixel data.
	// Palette will exist at one of the files in SHPI dir. 
	if (palfound == false) {
		int tmp1, tmp2;
		// Seek for the end of the pixel data. 
		fseek(famFile, (8 + chunkCount * 4 + offset2SHPI + pixOffset + 16 + width * height + 8), SEEK_SET);
		fread(&temp2, 4, 1, famFile);
		tmp1 = temp2;
//		cout << "++++++++++palette entry = 0x" << hex << tmp1 << "\n";
		fread(&temp2, 4, 1, famFile);
		tmp2 = temp2;
//		cout << "++++++++++palette entry = 0x" << hex << tmp2 << "\n";
			if ((tmp1 != 0x100)&&(tmp2 != 0)) {
//				cout << "-----------Seek palette at the beginning of pixel data!\n";
				fseek(famFile, (8 + chunkCount * 4 + offset2SHPI + pixOffset - 0x300), SEEK_SET);
//					cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";
//					cout << "pixOffset = 0x" << hex << pixOffset << "\n";
				goto withoutPAL;
				}

		goto withoutPAL;
	}
#endif;// If palette not found (previous version)



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

		cout << "wwwwOffset = 0x" << hex << wwwwOffset << "\n";
		cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";
		cout << "pixOffset = 0x" << hex << pixOffset << "\n";

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


	// Set to the palette beginning
	fseek(famFile, (8 + chunkCount * 4 + offset2SHPI), SEEK_SET);
	fseek(famFile, (palOffset), SEEK_CUR);
	//fread(&temp2, 4, 1, famFile); 
	//cout << "PAL = 0x" << hex << temp2 << "\n";
	//fread(&temp2, 4, 1, famFile); 
	//cout << "PAL2 = 0x" << hex << temp2 << "\n";
	//fread(&temp2, 4, 1, famFile); 
	//cout << "PAL3 = 0x" << hex << temp2 << "\n";


	// Skip 4 bytes
	fread(&temp2, 4, 1, famFile);

	// Read palette width
	fread(&temp3, 2, 1, famFile);
	palW = temp3;
	//cout << "palW = 0x" << hex << palW << "\n";

	// Read palette height
	fread(&temp3, 2, 1, famFile);
	palH = temp3;
	//cout << "palH = 0x" << hex << palH << "\n";


	// Skip 8 bytes
	fread(&temp2, 4, 1, famFile);
	fread(&temp2, 4, 1, famFile);


	// Reading RGB palette values. 3 bytes per pixel. Forward to 256*3=768 bytes
	// Reading order: BLUE, GREEN, RED.
#if 1;
//if (palfound == true) {
/****///	palfile = fopen("palette", "wb");
/****///	fclose(palfile);
//}
withoutPAL:
/****///palfile = fopen("palette", "ab");
for (int f = 0; f < 256; f++) {
	fread(&val, 1, 1, famFile);
		arrRGB[f].blue = val << 2;
		/****///		tmp11 = arrRGB[f].blue;
/****///		fwrite(&tmp11, 1, 1, palfile);
///		averagePAL[f].blue = val << 2;
//	pal->red = val;
	fread(&val, 1, 1, famFile);
		arrRGB[f].green = val << 2;
		/****///		tmp11 = arrRGB[f].green;
/****///		fwrite(&tmp11, 1, 1, palfile);
///		averagePAL[f].green = val << 2;
//	pal->green = val;
	fread(&val, 1, 1, famFile);
		arrRGB[f].red = val << 2;
		/****///		tmp11 = arrRGB[f].red;
/****///		fwrite(&tmp11, 1, 1, palfile);
///		averagePAL[f].red = val << 2;
//	pal->blue = val;
	}
/****///fclose(palfile);
#endif;

#endif; // NEW

//withoutPAL:
// Calculate real RGB values based on palette.
int index=0;
for (int a=0; a < height*width; a++) {
	index = pix[a];
	realArrRGB[a].red = arrRGB[index].red;
	realArrRGB[a].green = arrRGB[index].green;
	realArrRGB[a].blue = arrRGB[index].blue;
}

// Saving BMP file.
//tmpbmp = "Tex";
itoa(textN, bmpname, 10);
strcpy(tmpbmp, "\SceneryTextures\\ScnTex");
strcat(bmpname, ".bmp");
strcat(tmpbmp, bmpname);
//cout << tmpbmp << "\n";


// Calculate BMP header.
// Filesize = 54 bytes + width*height*3
int filesize;

filesize = 54 + width*height*3;


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

	temp4 = width;
	fwrite(&temp4, 4, 1, bmp); // Write width
	temp4 = height;
	fwrite(&temp4, 4, 1, bmp); // Write height

	temp5 = 1;
	fwrite(&temp5, 2, 1, bmp); // Number of color planes being used
	temp5 = 0x18;
	fwrite(&temp5, 2, 1, bmp); // Number of bits per pixel

	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write empty data

	temp4 = width*height*3;
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
for (int h=0; h < height*width; h++) {
	fwrite(&realArrRGB[h].red, 1, 1, bmp);
	fwrite(&realArrRGB[h].green, 1, 1, bmp);
	fwrite(&realArrRGB[h].blue, 1, 1, bmp);
}


#if 1;  // PALETTE
//Save palette
itoa(textN, bmpname2, 10);
strcpy(tmpbmp2, "\SceneryTextures\\Palette");
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

// Close files file.
fclose(famFile);
fclose(bmp);
free(pal);
free(rgb);
delete[]arrRGB;
delete[]realArrRGB;
//delete[]averagePAL;
}





void ExtractHorz(char *infilename) { // Extract and save horizon texture
	int subChunkN, remainder, temp, count, offset2SHPI, pixOffset, palOffset, palW, palH, filesize, readlimit;
	unsigned short int width, height, temp3;
	unsigned char pix[100000] = { 0 };
	unsigned char pixtemp, rgbtemp;
	unsigned char val;
	char filename[40], bmpname[40], tmpbmp[40], bmpname2[40], tmpbmp2[40];
	FILE* famFile, * bmpFile, * pixfile;
	static palette* pal;
	static RGB* rgb;
	char tmpstr2[7];

	palette* arrRGB = new palette[100000];
	palette* realArrRGB = new palette[100000];

	pal = (palette*)malloc(0x400000UL);
	rgb = (RGB*)malloc(0x400000UL);


// For the horizon texture we need 3rd chunk of the FAM file.
	// Open FAM file for the reading //AL1_001.FAM
strncpy(tmpstr2, infilename, 3); //скопировать 5 символов строки argv[1] в строку tmpstr2. Получилось "MAP01"
famFile = fopen(strcat(tmpstr2, "_001.FAM"), "rb");
//cout << "infilename - " << infilename << "\n";
//cout << "tmpstr2 - " << tmpstr2 << "\n";
//famFile = fopen("AL1_001.FAM", "rb");
fseek(famFile, 0, SEEK_SET);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);

offset2SHPI = temp;
//cout << "offset2SHPI = 0x" << hex << offset2SHPI << "\n";
fseek(famFile, offset2SHPI, SEEK_SET);

fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);

filesize = temp;

fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);

fread(&temp, sizeof(int), 1, famFile);

	if ((temp == 0x6C617021) || (temp == 0x4C415021)) {
			fread(&temp, 4, 1, famFile);
			palOffset = temp;
//			cout << "palOffset = 0x" << hex << palOffset << "\n";
		}

fseek(famFile, (offset2SHPI + palOffset), SEEK_SET);
//fread(&temp, sizeof(int), 1, famFile); // For test
//cout << "temp = 0x" << hex << temp << "\n";// For test

fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);
fread(&temp, sizeof(int), 1, famFile);

for (int f = 0; f < 256; f++) {// Reading palette pixels
	fread(&val, 1, 1, famFile);
		arrRGB[f].blue = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].green = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].red = val << 2;
	}

// Reading horizon pixels
// filesize-768-48
readlimit = filesize-768-48;

for (int i=0; i<readlimit; i++) {
	fread(&val, 1, 1, famFile);
	pix[i] = val;
	}

height = 110;
width = 640;
// Calculate real RGB values based on palette.
int index=0;
for (int a=0; a < height*width; a++) {
	index = pix[a];
	realArrRGB[a].red = arrRGB[index].red;
	realArrRGB[a].green = arrRGB[index].green;
	realArrRGB[a].blue = arrRGB[index].blue;
}


#if 1; // SAVE BMPs
// Saving BMP file.
itoa(1, bmpname, 10);
strcpy(tmpbmp, "\SceneryTextures\\Horizon");
strcat(bmpname, ".bmp");
strcat(tmpbmp, bmpname);
//cout << tmpbmp << "\n";


// Calculate BMP header.
// Filesize = 54 bytes + width*height*3
int filesize2;

filesize2 = 54 + width*height*3;


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

	fwrite(&filesize2, 4, 1, bmp); // Write filesize

	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write empty data
	temp4 = 0x36;
	fwrite(&temp4, 4, 1, bmp); // Write 122
	temp4 = 0x28;
	fwrite(&temp4, 4, 1, bmp); // Write 108

	temp4 = width;
	fwrite(&temp4, 4, 1, bmp); // Write width
	temp4 = height;
	fwrite(&temp4, 4, 1, bmp); // Write height

	temp5 = 1;
	fwrite(&temp5, 2, 1, bmp); // Number of color planes being used
	temp5 = 0x18;
	fwrite(&temp5, 2, 1, bmp); // Number of bits per pixel

	temp4 = 0;
	fwrite(&temp4, 4, 1, bmp); // Write empty data

	temp4 = width*height*3;
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
for (int h=0; h < height*width; h++) {
	fwrite(&realArrRGB[h].red, 1, 1, bmp);
	fwrite(&realArrRGB[h].green, 1, 1, bmp);
	fwrite(&realArrRGB[h].blue, 1, 1, bmp);
}


#if 1;  // PALETTE
//Save palette
itoa(1, bmpname2, 10);
strcpy(tmpbmp2, "\SceneryTextures\\horzpalette");
strcat(bmpname2, ".bmp");
strcat(tmpbmp2, bmpname2);
//cout << tmpbmp << "\n";


// Calculate BMP header.
// Filesize = 54 bytes + width*height*3
int filesize3;

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

	fwrite(&filesize3, 4, 1, bmp2); // Write filesize

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

// Close files file.
fclose(famFile);
fclose(bmp);
free(pal);
free(rgb);
delete[]arrRGB;
delete[]realArrRGB;

#endif; // SAVE BMPs

fclose(famFile);
}




void MakeHorz(FILE *inputfile) {///////////// ADD 3DO PARSING!
FILE *outf, *mtlfile;
bool flag3DO = false;
int verBuffer, version;

//Open input file and check it's type (3DO, NFS, NFS SE)
#if 1;
fseek(inputfile,0,SEEK_SET);
fread(&verBuffer,4,1,inputfile);
version = verBuffer;
//cout << "\nversion =" << version <<"\n";
	if (version == 0x10) { // NFS original version
			cout << "++Original NFS horizon++\n";
		} 
	if (version == 0x11) {
			cout << "++SE NFS horizon++\n";
		}

		if (version == 0xE000000) {
			flag3DO = true;
			cout << "++3DO NFS horizon++\n";
		}
#endif; // version	


// Make 4 planes around track
// Planes size: x=500, y=600, z=200
outf = fopen("Horizon.obj","a");
fwrite("# Horizon data\n",15,1,outf);
fprintf(outf, "o Horizon\n");
	fwrite("v ",2,1,outf); //1
	fprintf(outf, "%d ", -400);
	fprintf(outf, "%d ", -50);
	fprintf(outf, "%d ", 0);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //2
	fprintf(outf, "%d ", 400);
	fprintf(outf, "%d ", -50);
	fprintf(outf, "%d ", 0);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //3
	fprintf(outf, "%d ", 400);
	fprintf(outf, "%d ", 100);
	fprintf(outf, "%d ", 0);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //4
	fprintf(outf, "%d ", -400);
	fprintf(outf, "%d ", 100);
	fprintf(outf, "%d ", 0);
	fwrite("\n",1,1,outf);// New line


	fwrite("v ",2,1,outf); //5
	fprintf(outf, "%d ", -400);
	fprintf(outf, "%d ", -50);
	fprintf(outf, "%d ", -700);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //6
	fprintf(outf, "%d ", -400);
	fprintf(outf, "%d ", 100);
	fprintf(outf, "%d ", -700);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //7
	fprintf(outf, "%d ", 400);
	fprintf(outf, "%d ", -50);
	fprintf(outf, "%d ", -700);
	fwrite("\n",1,1,outf);// New line

	fwrite("v ",2,1,outf); //8
	fprintf(outf, "%d ", 400);
	fprintf(outf, "%d ", 100);
	fprintf(outf, "%d ", -700);
	fwrite("\n",1,1,outf);// New line


#if 1;
// Write vt params. There are 10 vertices and 10 materials. Max uv coord is 1.0!
//	for (int c=1; c<=10; c++) {
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
fprintf(outf, "usemtl 1\n");
//	}
#endif; // vt

fwrite("f ",2,1,outf);
fprintf(outf, "1/2 ", 1);
fprintf(outf, "5/3 ", 1);
fprintf(outf, "6/4 ", 1);
fprintf(outf, "4/1 ", 1);
fwrite("\n",1,1,outf);// New line

fwrite("f ",2,1,outf);
fprintf(outf, "5/3 ", 1);
fprintf(outf, "7/5 ", 1);
fprintf(outf, "8/6 ", 1);
fprintf(outf, "6/4 ", 1);
fwrite("\n",1,1,outf);// New line

fwrite("f ",2,1,outf);
fprintf(outf, "7/5 ", 1);
fprintf(outf, "2/7 ", 1);
fprintf(outf, "3/8 ", 1);
fprintf(outf, "8/6 ", 1);
fwrite("\n",1,1,outf);// New line

fwrite("f ",2,1,outf);
fprintf(outf, "2/7 ", 1);
fprintf(outf, "1/9 ", 1);
fprintf(outf, "4/10 ", 1);
fprintf(outf, "3/8 ", 1);
fwrite("\n",1,1,outf);// New line

		mtlfile = fopen("Horizon.mtl","a");
		fprintf(mtlfile, "newmtl 1\n");
		if (flag3DO==true) {
			fprintf(mtlfile, "map_Kd ThreeDOSceneryTextures\\Horizon1.bmp\n");
		} else {
			fprintf(mtlfile, "map_Kd SceneryTextures\\Horizon1.bmp\n");
		}
		fclose(mtlfile);

fclose(outf);
//cout << "File Horizon.obj successfuly saved\n";
//cout << "File Horizon.mtl successfuly saved\n";
}