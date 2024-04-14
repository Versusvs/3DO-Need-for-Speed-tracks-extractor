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

FILE* famFile;


void Extract3DOTextures(char* FamFileName3DO) {
int temp;
int i=0;
char bmpname[40], tmpbmp[40];
FILE *ccbFile;

famFile = fopen(FamFileName3DO, "rb");
fseek(famFile, 0, SEEK_SET);

while (!feof(famFile)) {

	fread(&temp, 4, 1, famFile);
label:
		if (temp == 0x20424343) {
//			cout << "CCB = 0x" << hex << temp << endl;	
//			i++;
			// CCB found, so read all data below while next CCB is reached. Save all data in separate file.
//			cout << "i = " << dec << i << endl;
				// Generate filename
				itoa(i, bmpname, 10);
				strcpy(tmpbmp, "\Textures3DO\\Tex");
//				strcat(bmpname, ".bmp");
				strcat(tmpbmp, bmpname);
//				cout << tmpbmp << endl;
				// Open file for writing
				ccbFile = fopen(tmpbmp, "wb");
				temp = 0x20424343;
				fwrite(&temp, 4, 1, ccbFile);
					for (int j=0; j<118715; j++) {
						fread(&temp, 4, 1, famFile);
//							if ((temp != 0x20424343)||(!feof(famFile))) {
							if (temp == 0x20424343) {
								i++;
//								cout << "temp = " << hex << temp << endl;
//								cout << "i = " << dec << i << endl;
							fclose(ccbFile); // close separate file
								goto label;
//							break;
						// Save data
//						fwrite(&temp, 4, 1, ccbFile);
						}
						fwrite(&temp, 4, 1, ccbFile);
//					fclose(ccbFile); // close separate file
//					goto label;
				}
					fclose(ccbFile); // close separate file
					goto label;
		}
//	i++;
}


fclose(famFile);
}