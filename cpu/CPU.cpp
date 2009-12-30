// CPU.cpp

#include "debug.h"
#include "cpu/CPU.h"

using namespace nowind;

CPU::CPU(Emulator& aEmulator) : Component(aEmulator), 
mBus(aEmulator.getBus())
{

}

std::string CPU::getMnemonics(word pc, word w1, word w2) {

	char *sstr;
	char buf[20], buf1[10];
	byte nb1 = 0;
	byte nb2 = 0;
	int size = 1;
	char mstr[30];

	switch (w1&0xFF) {
	case 0xCB: size++; strcpy(mstr, &mnemonicCB[w1>>8][0]); nb1=w2&0xFF; nb2=w2>>8; break;
	case 0xED: size++; strcpy(mstr, &mnemonicED[w1>>8][0]); nb1=w2&0xFF; nb2=w2>>8; break;
	case 0xDD:
	case 0xFD:
		size++;
		switch(w1>>8) {
		case 0xCB: nb1=w2&0xFF; strcpy(mstr, &mnemonicCB[w2>>8][0]); break;
		case 0xED: nb1=w2&0xFF; strcpy(mstr, &mnemonicED[w2>>8][0]); break;
		default: strcpy(mstr, &mnemonic[w1>>8][0]); nb1=w2&0xFF; nb2=w2>>8; break;
		}
		break;
	default: strcpy(mstr, &mnemonic[w1&0xFF][0]); nb1=w1>>8; nb2=w2&0xFF;
	}
	sstr = strstr(mstr, "NN");
	if (sstr!=NULL) {
		sprintf(buf, "%02X", nb1);
		memcpy(sstr, buf, 2);
		size++;
	}
	sstr = strstr(mstr, "MMMM");
	if (sstr!=NULL) {
		sprintf(buf, "%04X", (nb2<<8)+nb1);
		memcpy(sstr, buf, 4);
		size+=2;
	}
	sstr = strstr(mstr, "EEEE");
	if (sstr!=NULL) {
		sprintf(buf, "%04X", pc+(signed char)nb1+2);
		memcpy(sstr, buf, 4);
		size++;
	}
	sstr = strstr(mstr, "Y");
	if (sstr!=NULL) {
		strcpy(buf, sstr+1);
		switch (w1&0xFF) {
		case 0xDD: strcpy(sstr, "ix"); break;
		case 0xFD: strcpy(sstr, "iy"); break;
		default: strcpy(sstr, "hl");
		}
		strcat(mstr, buf);
	}
	sstr = strstr(mstr, "R");
	if (sstr!=NULL) {
		strcpy(buf, sstr+1);
		switch (w1&0xFF) {
		case 0xDD: strcpy(sstr, "ix"); break;
		case 0xFD: strcpy(sstr, "iy"); break;
		default: strcpy(sstr, "hl");break;
		}
		strcat(mstr, buf);
	}
	sstr = strstr(mstr, "X");
	if (sstr!=NULL) {
		strcpy(buf, sstr+1);
		switch (w1&0xFF) {
		case 0xDD:
			sprintf(buf1, "ix+%02X", nb1);
			strcpy(sstr, buf1);
			size++;
			break;
		case 0xFD:
			sprintf(buf1, "iy+%02X", nb1);
			strcpy(sstr, buf1);
			size++;
			break;
		default: strcpy(sstr, "hl");break;
		}
		strcat(mstr, buf);
	}
	return mstr;
}
