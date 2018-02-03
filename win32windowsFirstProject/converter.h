//Convertor.h
#pragma once
#pragma warning(disable:4996)
#include"Decoder.h"
#define MAXTEXT 70 //maximum charachter number : for button (its half for utf)
#define WCHARLENGTH 20
#define MAXCONTROLS 100 //maximum size of my array for saving json data : maximum component number
#define DROPSIZE 10
WCHAR words[MAXCONTROLS][MAXTEXT];
LPCWSTR utfOrAnsiText;
char* ansiText;

inline const wchar_t *char2Wchar(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize); // warrning this is unsafe replace with mbstowcs_s
	return wc;
}

inline const wchar_t *string2Wchar(const std::string& s) {
	char tempC[WCHARLENGTH];
	strcpy(tempC, s.c_str());
	return char2Wchar(tempC);
}

inline LPCWSTR charArray2utf(char* c, int i) {
	ansiText = c;
	int spaceCount = 0;
	for (int q = 0; q < MAXTEXT; q++) {
		if (ansiText[q] != '0') {
			//_RPT1(0, "%c", test[q]);
			//OutputDebugString(L" -> char\n");

			const auto myByte1 = byte(ansiText[q]);
			if (myByte1 == '\0')break;
			const auto myByte2 = byte(ansiText[q + 1]);
			persianDecoder(myByte1, myByte2);

			//space character's -> ' ' byte value is 32
			if (myByte1 > 127 || myByte1 == 32) {

				if (spaceCount % 2 == 0 && spaceCount != 0)
				{
					words[i][q / 2 + spaceCount / 2] = word;
				}
				else words[i][q / 2 + spaceCount] = word;

				if (words[i][q / 2 + spaceCount] == ' ') {
					spaceCount++;
				}
				if (myByte1 > 127)q++;
			}

			//this code used for debuging convert of byte to char 
			//_RPT1(0, "\n int : %d int : %d", myByte1,myByte2);

		}
		// '\0' is character for end of array
		else {
			words[i][q / 2 + spaceCount] = '\0';
			ansiText[q] = '\0';

			break;
		}
	}
	//use utf chars : only supports persian characters
	if (byte(ansiText[0]) > 127) utfOrAnsiText = words[i];
	//use ansi chars
	else utfOrAnsiText = char2Wchar(ansiText); //char2wchar is a const char* to const wchar_t* convertor
	return utfOrAnsiText;
}

