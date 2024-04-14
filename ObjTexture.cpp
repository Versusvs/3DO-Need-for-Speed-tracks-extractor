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

// Extract object texture from *.FAM file
void ExtractObjectTexture(int textN, char* infilename) {
	int subChunkN, temp2, count, offset2SHPI, chunkCount, strText, shpiCount, pixOffset, palOffset, palW, palH, wwwwOffset, remainder, remainder2;
	unsigned short int width, height, temp3, width2, height2;
	unsigned char pix[100000] = { 0 };
	unsigned char pixtemp, rgbtemp;
	unsigned char val;
	char filename[40], bmpname[40], tmpbmp[40], bmpname2[40], tmpbmp2[40];
	FILE* famFile, * bmpFile, * pixfile;
	static palette* pal;
	static RGB* rgb;
int widthcounter;
int heightcounter;

	palette* arrRGB = new palette[100000];
	palette* realArrRGB = new palette[100000];

	pal = (palette*)malloc(0x400000UL);
	rgb = (RGB*)malloc(0x400000UL);


//DEBUG******			cout << "\ntextN " << dec << textN << "\n";
strText = 0x30303030; // '0000' 

#if 1;
	char tmpstr2[10];
	char tmpstr3[11];
	for (int f=0; f<sizeof(tmpstr3); f++){
		tmpstr3[f] = 0;
	}
	for (int p=0; p<sizeof(tmpstr2); p++){
		tmpstr2[p] = 0;
		}


strncpy(tmpstr2, infilename, 3); //скопировать 3 символа строки infilename в строку tmpstr2. Получилось "al1"
//cout << "infilename - " << infilename << "\n";
//cout << "tmpstr2 - " << tmpstr2 << "\n";
strcat(tmpstr2, "_001.FAM");

	for (int p1=0; p1<sizeof(tmpstr3); p1++){
		tmpstr3[p1] = 0;
		}

strncpy(tmpstr3, tmpstr2, 11);
//cout << "tmpstr2.1 - " << tmpstr2 << "\n";
//cout << "tmpstr3 - " << tmpstr3 << "\n";
famFile = fopen(tmpstr2, "rb");
#endif;
//famFile = fopen("AL1_001.FAM", "rb");
fseek(famFile, 0, SEEK_SET);

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


// Finding pixOffset and palOffset
	// Read forward while find strText. There is a limit reading - shpiCount.
	for (int j = 0; j <= 8; j++) {
		fread(&temp2, 4, 1, famFile);
		if (temp2 == strText) {
			fread(&temp2, 4, 1, famFile);
			pixOffset = temp2;
			//DEBUG******			cout << "--------pixOffset 0x" << hex << pixOffset << "\n";
		}
	}


	// Set back to the "wwww", jump to the offset2SHPI from there. 
	fseek(famFile, wwwwOffset, SEEK_SET); // Set to the 

	// Jump to the offset2SHPI
	fseek(famFile, offset2SHPI, SEEK_CUR);

	// Set to the pixOffset.
	fseek(famFile, pixOffset, SEEK_CUR);

	// Check 0x7B as entry for the pixel data.
	// Skip 3 bytes
	fread(&temp2, 4, 1, famFile);
	//cout << "pix Start = 0x" << hex << temp2 << "\n";

// Reading texture width 
	fread(&temp3, 2, 1, famFile);
	width = temp3;
	width2 = temp3;
//		width = (int)width / 4;
//		width *= 4;
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
//		widthcounter = 
		}

	if (remainder == 2) {
		width2 = (int)width / round;
		width2 = width2 * round + 4;
		//DEBUG******		cout << "new width = " << dec << width2 << "\n";
//		width = width*4 - 1;
		}


	if (remainder == 3) {
		width2 = (int)width / round;
		width2 = width2 * round + 4;
		//DEBUG******		cout << "new width = " << dec << width2 << "\n";
//		width = width*4 - 1;
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
	// Reading palOffset, set back to the found SHPI header, jump to the palOffset.

	// Set back to the "wwww", jump to the offset2SHPI from there. 
	fseek(famFile, wwwwOffset, SEEK_SET); // Set to the 

	// Jump to the offset2SHPI
	fseek(famFile, offset2SHPI, SEEK_CUR);

		palOffset = 24;// If palette exists, but not listed in the header. Default value is 24 (0x18)

	for (int h = 0; h <= 8; h++) { // !PAL = 0x4C415021	!pal = 0x6C617021
		fread(&temp2, 4, 1, famFile);
		if ((temp2 == 0x6C617021) || (temp2 == 0x4C415021)) {
			fread(&temp2, 4, 1, famFile);
			palOffset = temp2;
//DEBUG******								cout << "--------palOffset = 0x" << hex << palOffset << "\n";
			palfound = true;
			break;
		}
	}

	// Set to the palette beginning
	// Set back to the "wwww", jump to the offset2SHPI from there. 
	fseek(famFile, wwwwOffset, SEEK_SET); // Set to the 

	// Jump to the offset2SHPI
	fseek(famFile, offset2SHPI, SEEK_CUR);

#if 1; // Palette not in header
if (palfound == false) {
	// Set defaults
// If palette exists, but not listed in the header.
palW = 0x100; 
palH = 0x3;
//fseek(famFile, 40, SEEK_CUR); // Alpine1 track
//fseek(famFile, 41, SEEK_CUR); // Coastal 1 track (part)

//fseek(famFile, pixOffset-0x300, SEEK_CUR);

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


// Jump to the palOffset
	fseek(famFile, palOffset, SEEK_CUR);


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
	fread(&val, 1, 1, famFile);
		arrRGB[f].blue = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].green = val << 2;
	fread(&val, 1, 1, famFile);
		arrRGB[f].red = val << 2;
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

#if 0;
	if ((a % width * 3 == 0) && (remainder != 0) && (a != 0)) {

		for (int g=0; g<(4-remainder); g++)
			realArrRGB[a+1].red = 0xFF;
			realArrRGB[a+1].green = 0xFF;
			realArrRGB[a+1].blue = 0xFF;
			a++;
	//				cout << dec << a << "\n";
		///			val = 0xFF;
		///			fwrite(&val, 3, 1, bmp); // Add zeros.
		//			continue;
		//			h++;
		///			goto cont;

	}
#endif;
}



// Saving BMP file.
//tmpbmp = "Tex";
itoa(textN, bmpname, 10);
strcpy(tmpbmp, "\ObjectTextures\\Tex");
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
strcpy(tmpbmp2, "\ObjectTextures\\Palette");
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
free(pal);
free(rgb);
delete[]arrRGB;
delete[]realArrRGB;
fclose(famFile);
}