#include "RomMemory.h"
#include "BusComponent.h"
#include "SlotSelector.h"
#include "Bus.h"
#include "debug.h"

using namespace nowind;
using namespace fastdelegate;
using namespace std;

RomMemory::RomMemory(Bus& bus, string filename) : BusComponent(bus)
{
	mFilename = filename;
}

void RomMemory::prepare()
{
	//loadRom(mFilename);
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

/*
void RomMemory::loadRom(string theFilename) {
    
    string filename = Debug::Instance()->getPath()+theFilename;
	DBERR("MemoryDevice::loadRom: %s\n", filename.c_str());
    mID = string("RomMemory (") + theFilename + ")";
	ifstream romfile(filename.c_str(),ios::binary);
	if (romfile.fail()) {
		DBERR("Error opening file!\n");
		mFileSize = 0;
		mSections = 0;
		assert(false);
    }
	romfile.seekg(0, ios::end);
	fileSize = romfile.tellg();
	
	DBERR(" fileSize: %u\n", fileSize); 
	
	romfile.seekg(0);
	
	char *newmemChars = new char[fileSize];

	DBERR("  created rom at 0x%08X\n", newmemChars);
	DBERR("  reading %u bytes.\n", fileSize);

	romfile.read(newmemChars, fileSize);
	romfile.close();
	
	unsigned int sizeInBlocks = (fileSize / 0x2000);
	if (fileSize & 0x1fff) {
		sizeInBlocks += 1;
	}
	
    nw_byte * newMem = new nw_byte[sizeInBlocks*BLOCKSIZE];
	// transfer roms from unsigned char memory to native (faster?) datatype
	for (unsigned int i=0;i<fileSize;i++) {
        newMem[i] = newmemChars[i] & 0xff;
    }    
    memory = newMem;
    releaseMemory = true;
    delete [] newmemChars;
}
*/

void RomMemory::activate(Uint8 section)
{
    DBERR("RomMemory::activate section: %d\n", section);
    mBus.deactivateMemWriteSection(section);
    
	//todo: use mSections to insert device in the right sections!?
/*
    Uint8 page = section >> 1; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = (currentBank*16*1024) + ((section & 1) * 8*1024);
    //DBERR("page %u, bank: %u, offset: $%04X\n", page, currentBank, offset);
    mBus.setReadSectionMemory(section, &mMemory[offset]); 
	*/
}
