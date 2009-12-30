#include "RomMemory.h"
#include "BusComponent.h"
#include "SlotSelector.h"
#include "Bus.h"
#include "debug.h"
#include <fstream>

using namespace nowind;
using namespace fastdelegate;
using namespace std;

RomMemory::RomMemory(Bus& bus, string filename) : BusComponent(bus)
{
	mFilename = filename;
}

void RomMemory::prepare()
{
	loadRom(mFilename);
}

void RomMemory::initialize()
{

}

void RomMemory::prepare_shutdown()
{

}

RomMemory::~RomMemory()
{

}


void RomMemory::loadRom(string filename) {
    
	DBERR("MemoryDevice::loadRom: %s\n", filename.c_str());

	ifstream romfile(filename.c_str(),ios::binary);
	if (romfile.fail()) {
		DBERR("Error opening file %s!\n", filename.c_str());
		NW_ASSERT(false);
    }
	romfile.seekg(0, ios::end);
	size_t fileSize = romfile.tellg();
	
	if (fileSize == 0) {
		DBERR("Filesize is 0!\n");
		NW_ASSERT(false);
		// TODO: maak mechanisme om fout aan gebruiker te door te geven
	}
	
	DBERR(" fileSize: %u\n", fileSize); 
	
	romfile.seekg(0);
	
	char *newmemChars = new char[fileSize];

	DBERR("  created rom at 0x%08X\n", newmemChars);
	DBERR("  reading %u bytes.\n", fileSize);

	romfile.read(newmemChars, fileSize);
	romfile.close();
	
    
    unsigned int sizeInBlocks = ((fileSize-1) / constSectionSize) + 1;
	
    byte * newMem = new byte[sizeInBlocks * constSectionSize];
	// transfer roms from unsigned char memory to native (faster?) datatype
	for (unsigned int i=0; i<fileSize; i++) {
        newMem[i] = newmemChars[i] & 0xff;
    }    
    mMemory = newMem;
    delete [] newmemChars;
}

void RomMemory::activate(Uint8 section)
{
    //DBERR("RomMemory::activate section: %d\n", section);
    mBus.deactivateMemWriteSection(section);

    // TODO: make use fileSize & start address of rom
    Uint32 offset = section * constSectionSize;
    if (section < 4) {
        mBus.activateMemReadSection(section, MakeDelegate(this, &RomMemory::readByte));
        mBus.setReadSectionMemory(section, &mMemory[offset]); 
    }
}

byte RomMemory::readByte(word address)
{
    // TODO: this function should me removed, as a rom has no memory mapped IO
    return mMemory[address];
}