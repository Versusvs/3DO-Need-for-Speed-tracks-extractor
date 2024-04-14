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


// Extract all TRKD background cels 
void ExtractTRKDs3DO(char* infilename) {
FILE* famFile;
int temp, tempOFST;
int tempArr[20];
int i=0;
int counter = 0;
int counterCCB = 0;
char bmpname[40], tmpbmp[40];
FILE *ccbFile;
unsigned int pdatSize = 0;
unsigned int ofstSize = 0x18;

//		cout << infilename << endl;

famFile = fopen(infilename, "rb");
fseek(famFile, 0, SEEK_SET);

while (!feof(famFile)) {

label:
	fread(&temp, 4, 1, famFile);
//label:
		counter = 0;
		if (temp == 0x20424343) {
//				counterCCB++;
//				cout << counterCCB << endl;
				counter++;
// CCB found, so read all data below while zeroes after CCB data is reached. Save all data in separate file.
				// Generate filename
				itoa(i, bmpname, 10);
				strcpy(tmpbmp, "\TRDKcels3DO\\TRKD");
				strcat(tmpbmp, bmpname);

				// Open file for writing
				ccbFile = fopen(tmpbmp, "wb");
		// Step back and look for 'OFST' section before CCB. If found, write it too. MAKE!!!!!!!!!
		// This section is 0x18 sized and has constant values. /// CHECK constant for all tracks.
		//Save before CCB! Must read file to another int (not temp!)
				fseek(famFile, -0x1C, SEEK_CUR);
				fread(&tempOFST, 4, 1, famFile);

				if (tempOFST==0x5453464F) {
				fwrite(&tempOFST, 4, 1, ccbFile);
				fread(&tempOFST, 4, 1, famFile);
				fwrite(&tempOFST, 4, 1, ccbFile);
				fread(&tempOFST, 4, 1, famFile);
				fwrite(&tempOFST, 4, 1, ccbFile);
				fread(&tempOFST, 4, 1, famFile);
				fwrite(&tempOFST, 4, 1, ccbFile);
				fread(&tempOFST, 4, 1, famFile);
				fwrite(&tempOFST, 4, 1, ccbFile);
				fread(&tempOFST, 4, 1, famFile);
				fwrite(&tempOFST, 4, 1, ccbFile);
					fseek(famFile, 0x4, SEEK_CUR);
				} else {
					fseek(famFile, 0x18, SEEK_CUR);
					}

				fwrite(&temp, 4, 1, ccbFile);

//				i++;
				// Read to PDAT section for end data offset. Also write data to separate file
				for (int j=0; j<70; j++) { // 60 - FIX!!!!! to overlap possible PDAT location
					fread(&temp, 4, 1, famFile);
						counter++;
					fwrite(&temp, 4, 1, ccbFile);
					if (temp==0x54414450) {
//						counterCCB++;
//						cout << counterCCB << endl;
//						i++;
						fread(&temp, 4, 1, famFile);
							counter++;
						pdatSize = Swap4Bytes(temp);
//						cout << "i = 0x" << hex << i << endl;
//						cout << "pdatSize = 0x" << hex << pdatSize << endl;
						temp = Swap4Bytes(temp);
						fwrite(&temp, 4, 1, ccbFile);
							break;
						}
					}

unsigned int toRead = pdatSize-8;
				for (int f=0; f<(toRead/4); f++) {					
//				for (int f=0; f<((pdatSize-8)/4 - counter/4); f++) {
				fread(&temp, 4, 1, famFile);
				fwrite(&temp, 4, 1, ccbFile); 
				}
#if 1;
				// Trying to find a palette. If PLUT not found - stop.
					fread(&temp, 4, 1, famFile);
					if (temp==0x54554C50) { // if PLUT
						fwrite(&temp, 4, 1, ccbFile); 
						for (int k=0; k<18; k++) { // Read PLUT and write to external file
							fread(&temp, 4, 1, famFile);
//							if ((k>17)&&(temp == 0)) break; // FIX brute hack! 
							fwrite(&temp, 4, 1, ccbFile); 
							}

					}
#endif;
//				cout << "i = " << dec << i << endl;
//				cout << "temp = 0x" << hex << temp << endl;
			fclose(ccbFile); // close separate file
			i++;
			counter = 0;
			goto label;
		}
}
fclose(famFile);
}
