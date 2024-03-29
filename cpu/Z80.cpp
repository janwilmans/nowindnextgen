// Z80.cpp

#include "cpu/Z80.h"
#include <iostream>
#include <fstream>
#include "debug.h"
#include <SDL.h>
#include "Bus.h"
#include "SlotSelector.h"       //todo: remove
#include "Emulator.h"

#define CONSOLE_DEBUGGING_ON
#define FULL_SPEED_ON

using namespace std;
using namespace fastdelegate;
using namespace nowind;

/* all of these macros are using in opcodes*.inc so they are easely be adapted
   to used on different platforms and in different configurations */

#define TS(states) localEmuTime += states

#define reg_af ((reg_a << 8) | reg_f)
#define reg_bc ((reg_b << 8) | reg_c)

#define reg_d (reg_de >> 8)
#define reg_e (reg_de & 255)

#define reg_h (reg_hl >> 8)
#define reg_l (reg_hl & 255)

#define SFLAG 0x80
#define ZFLAG 0x40
#define YFLAG 0x20
#define HFLAG 0x10
#define XFLAG 0x08
#define PFLAG 0x04
#define NFLAG 0x02
#define CFLAG 0x01

/* memory read/write macros */
#define READMEM mBus.readByte
#define READMEM16 readWord
#define WRITEMEM mBus.writeByte
#define WRITEMEM16 writeWord

//  create read/write mem fp's

// int* a = new int[3] { 1, 2, 0 }; //C++11 only // NOT OK
// vector<string> vs={ "first", "second", "third"};  // NOT OK

 std::tr1::shared_ptr<int>  f;  // OK

 /*
class C
{
 int a=7; //C++11 only  // not OK
public:
 C();
};

*/

Z80::Z80(Emulator& aEmulator) : CPU(aEmulator)
{
    DBERR("Z80 constructor...\n");

    // test C++11 
    auto x=0;       //OK

     
    /* set the CPU frequency to MSX's normal 3,57 Mhz */
    cpuFrequency = 3579545;

    /* initialize flag tables
     * instructions use these so the flags do not have to be calculated
     * the undocumented (we call them: X and Y) flags are calculated here too.
     */
    for (int i = 0;i < 256;++i)
    {
        flagSZ[i] = (i & (SFLAG | XFLAG | YFLAG)) | ((i == 0) ? ZFLAG : 0);
        flagSZsub[i] = flagSZ[i] | NFLAG;
        byte p = 1;
        for (int j = 0;j < 8;++j) p ^= ((i >> j) & 1);
        flagSZP[i] = flagSZ[i] | (p << 2);
        flagInc[i] = flagSZ[i] | ((i == 0x80) ? PFLAG : 0) | ((i ^ (i - 1)) & HFLAG);
        flagDec[i] = flagSZsub[i] | ((i == 0x7F) ? PFLAG : 0) | ((i ^ (i + 1)) & HFLAG);
    }

    DBERR("Z80 constructor finished.\n");
}

void Z80::prepare()
{
    DBERR("z80 prepare\n");
}

void Z80::initialize()
{

}

void Z80::setupBdosEnv(const char* filename)
{
    writeIO(0xFC, 3);
    writeIO(0xFD, 2);
    writeIO(0xFE, 1);
    writeIO(0xFF, 0);
    
    writeIO(0xa8, 0);
    WRITEMEM(0xffff, 0);

    // load file at 0x100
    ifstream romfile(filename, ios::binary);
    bool failed = romfile.fail();
    NW_ASSERT(!failed);

    romfile.seekg(0, ios::end);
    Uint32 fileSize = romfile.tellg();
    DBERR("loaded %s (%u bytes)\n", filename, fileSize);

    char* temp = new char[fileSize];
    romfile.seekg(0);
    romfile.read(temp, fileSize);
    romfile.close();

    // dont read files into memory directly
    // memory locations are not guaranteed to be '1 byte' locations
    for (Uint16 i = 0;i < fileSize;i++)
    {
        byte value = temp[i] & 0xff;    // todo: find out why this fails without the & 0xff!!
        WRITEMEM(0x100+i, value);
    }
    delete temp;

    // patch bdos call 0x0005
    WRITEMEM(0x0005, 0xED);
    WRITEMEM(0x0006, 0x0E);
    WRITEMEM(0x0007, 0xC9);
    
	word testnr = 0;
	word testAdres = 0x13A+(testnr*2);

	WRITEMEM(0x120, testAdres & 255);
	WRITEMEM(0x121, testAdres >> 8);
	
	// set initial SP 
    reg_sp = 0xc800;
}

void Z80::prepare_shutdown()
{
    DBERR("z80 prepare_shutdown\n");
}

Z80::~Z80()
{
    DBERR("z80 destruction in progress...\n");

    DBERR("z80 destroyed.\n");
}

void Z80::reset()
{
    DBERR("start of Z80 RESET\n");

    // todo: remove this, initialize this where it is suppose to be done!
    writeIO(0xa8, 0);
    WRITEMEM(0xffff, 0);

    IFF1 = IFF2 = false;  // disable interrupts
    interruptMode = 0;

    reg_pc = 0x0000;
    reg_a = 0xFF;
    reg_f = 0xFF;
    reg_b = 0xFF;
    reg_c = 0xFF;
    reg_de = 0xFFFF;
    reg_hl = 0xFFFF;
    reg_ix = 0xFFFF;
    reg_iy = 0xFFFF;
    reg_sp = 0xFFFF;
    shadow_af = shadow_de = shadow_hl = 0xFFFF;
    shadow_b = shadow_c = 0xFF;

    reg_i = 0;
    reg_r = 0;
    refreshCounter = 0;
}

void Z80::setPC(word regpc)
{
    reg_pc = regpc;
}

void Z80::nmiCPU()
{
    // aantal klokpulsen???
    // IFF2 = IFF1;    /* gebeurt niet volgens Sean Young, moet de NMI routine zelf doen */
    IFF1 = false;

    // cpu weer wakker maken na een HALT
    if (READMEM(reg_pc) == 0x76) reg_pc++;

    reg_sp -= 2;
    WRITEMEM16(reg_sp, reg_pc); //TODO: volgens de documentatie wordt er maar een M1 uitgevoerd???
    reg_pc = 0x0066;
}

bool Z80::getIFF1()
{
    return IFF1;
}

void Z80::intCPU(byte interruptVectorOnDataBus)
{
    // ignore interrupt if they are disabled
    if (!IFF1) return;

    NW_ASSERT(reg_pc < 0x10000);

    // When an interrupt is accepted, the Z80 clears IFF1 and IFF2
    IFF1 = false;
    IFF2 = false;

    // cpu weer wakker maken na een HALT
    if (unlikely(READMEM(reg_pc) == 0x76)) reg_pc++;

    // Interrupt Acknowledge Cycle
    // The Z80 generates a special (NOT AN EXTRA) M1 cycle and adds two wait states to
    // this cycle. Futhermore, the MSX-engine adds another wait state to this M1 cycle.

    refreshCounter++;
    reg_sp -= 2;
    WRITEMEM16(reg_sp, reg_pc);

    switch (interruptMode)
    {
    case 0:
        //mEmuTime += 14;

        // the instruction can be any 1 byte instruction, usually this will be rst.
        // TODO: hoeveel states...
        switch (interruptVectorOnDataBus)
        {
        case 0xc7: reg_pc = 0x0000; break;
        case 0xcf: reg_pc = 0x0008; break;
        case 0xd7: reg_pc = 0x0010; break;
        case 0xdf: reg_pc = 0x0018; break;
        case 0xe7: reg_pc = 0x0020; break;
        case 0xef: reg_pc = 0x0028; break;
        case 0xf7: reg_pc = 0x0030; break;
        case 0xff: reg_pc = 0x0038; break;
        default:
            reg_pc = 0x0038; // just in case...
            DBERR("Alleen RST-instr. van een interrupt-device worden ondersteund!\n");
        };
        break;
    case 1:
        // RST $38 will be executed, so the total number of states is:
        // 11 (rst $38) + 2 (wait states in special M1) + 1 (MSX-engine) = 14
        // TODO: dit is onze (stellig) aanname, maar klopt dit ook werkelijk? De redenatie
        // komt overeen met die van Sean met als toevoeging 1 extra state door de engine.
        //mEmuTime += 14;
        reg_pc = 0x38;
        break;
    case 2:
        //mEmuTime += 20;
        reg_pc = READMEM16(interruptVectorOnDataBus | (reg_i << 8));
        break;
    default:
        // unknownd interrupt mode, this should never happen
        NW_ASSERT(false);
    }
}

byte Z80::opcodeFetch(word address)
{

    /*
    // Meer info over extra M1 wait-state:
    // http://www.funet.fi/pub/msx/mirrors/hanso/service_manuals/yamahacx5myis503ts.pdf

    // M1 wait state (msx engine)
    emuTime++;
    refreshCounter++;
    */

    byte oc = READMEM(address);
    //DBERR("0x%04X  0x%02X\n", address, oc);
    return oc;
}

// the amount of cycles actually executed can vary (with max. the cycles-1 of the biggest instruction)
// because an instruction is always completely executed.
// if we are called the scheduler has determined that at least 1 instruction needs to be executed.
emuTimeType Z80::ExecuteInstructions(emuTimeType startTime, emuTimeType aEndTime)
{
    Sint32 emuTimeDiff = 0;
    emuTimeType localEmuTime = startTime;
    do
    {
        // define reg1/reg2 more locally! defining them here could prevent use of registers!
        word reg1 = 0;      
        word reg2 = 0;
        word opcode = 0;
        opcode = opcodeFetch(reg_pc);
        ++reg_pc;

        if (reg_pc > 0xffff)
        {
            DBERR("reg_pc to high: 0x%04X !\n", reg_pc); //todo: find out why/when this happens ?!
            reg_pc = 0;
        }

        // execute opcode
        switch (opcode)
        {
            #include "opcodes.inc"
        case 0xcb:
            switch (opcodeFetch(reg_pc++))
            {
                #include "opcodesCB.inc"
            }
            break;
        case 0xed:
            opcode = opcodeFetch(reg_pc++);
            switch (opcode)
            {
                #include "opcodesED.inc"
            }
            break;
        case 0xdd:
        case 0xfd:
            reg2 = 0;
            do
            {
                reg2++;
                reg1 = opcode;
                opcode = opcodeFetch(reg_pc++);
                NW_ASSERT(opcode != 0xed); // TODO: deze situatie kan voorkomen, er moet dan naar opcodeED gesprongen worden (dd en fd worden dan dus genegeerd)
                NW_ASSERT(reg2 == 1); // TODO: eruit halen als het een keer is voorgekomen! reg2 dan ook dus
            }
            while ((opcode == 0xdd) || (opcode == 0xfd));

            switch (reg1)
            {
            case 0xdd:
                switch (opcode)
                {
                    #define REGIX reg_ix
                    #include "opcodesDD.inc"
                }
                break;
            case 0xfd:
                switch (opcode)
                {
                    #undef REGIX
                    #define REGIX reg_iy
                    #include "opcodesDD.inc"
                }
                break;
            default:
                NW_ASSERT(false);
            }
            break;
        default:
            NW_ASSERT(false); // opcode > 255 ?!
            break;
        }
        
        //dumpStateInfo(mEmulator.getSlotSelector());

        NW_ASSERT (reg_a < 256);
        NW_ASSERT (reg_f < 256);
        NW_ASSERT (reg_b < 256);
        NW_ASSERT (reg_c < 256);
        NW_ASSERT (reg_de < 0x10000);
        NW_ASSERT (reg_hl < 0x10000);
        NW_ASSERT (reg_ix < 0x10000);
        NW_ASSERT (reg_iy < 0x10000);
        NW_ASSERT (reg_pc < 0x10000); 
        NW_ASSERT (reg_sp < 0x10000);
    }
    while ( ((Sint32)(aEndTime - localEmuTime)) > 0);
    return localEmuTime;
}

/*
void Z80::dumpPages()
{
    string filename("dumppages.bin");

    // delete the existing? file
    ofstream ofs_delete(filename.c_str(), ios::trunc);
    ofs_delete.close();

    ofstream ofs(filename.c_str(), ios::binary);
    if (ofs.fail())
    {
        DBERR("Error opening file %s!\n", filename.c_str());
    }

    for (unsigned int b = 0;b < 8;b++)
    {
        DBERR("readBlock[%u]: %u\n", b, readBlock[b]);
        ofs.write((char *)readBlock[b], 8*1024);
    }
    ofs.close();
}
*/

void Z80::saveState()
{
    //MemoryMapper->saveState();
}

void Z80::loadState()
{
    //MemoryMapper->loadState();
}

void Z80::hijackBdos()
{
    long seconds = SDL_GetTicks() / 1000;

    switch (reg_c)
    {
    case 2:
        // BDOS function 2 (C_WRITE) - Console output
        // C=2, E=ASCII character
        DBERR("%c", reg_e);
        break;
    case 9:
    {
        // BDOS function 9 (C_WRITESTR) - Output string
        // C=9, DE=address of string
        word de = reg_de;
        while (1)
        {
            byte c = READMEM(de++);
            if (c == 0x0A)
            {
                DBERR(" (time: %us)\n", seconds);
                continue;
            }
            if (c == 0x24) break;
            DBERR("%c", c);
        }
        break;
    }
    default:
        // no nothing
        break;
    }
}


void Z80::dumpStateInfo(SlotSelector& slotSelector) 
{
	DBERR(" AF:%04X BC:%04X DE:%04X HL:%04X", reg_af, reg_bc, reg_de, reg_hl);
    DBERR(" IX:%04X IY:%04X SP:%04X F:", reg_ix, reg_iy, reg_sp);

	if (reg_f & SFLAG) DBERR("s"); else DBERR(".");
	if (reg_f & ZFLAG) DBERR("z"); else DBERR(".");
	if (reg_f & 0x20) DBERR("1"); else DBERR("0");
	if (reg_f & HFLAG) DBERR("h"); else DBERR(".");
	if (reg_f & 0x08) DBERR("1"); else DBERR("0");
	if (reg_f & PFLAG) DBERR("p"); else DBERR(".");
	if (reg_f & NFLAG) DBERR("n"); else DBERR(".");
	if (reg_f & CFLAG) DBERR("c"); else DBERR(".");

    int slots = readIO(0xa8);        //TODO: create alternative way to read without triggering IO-actions
    for (int page=0;page<4;page++) 
    {
        int mainSlot = (slots>>(page*2))&3;
        Uint8 subSlot = (slotSelector.getActivateSubslots(mainSlot) >>(page*2))&3;
        DBERR(" %u", mainSlot);
        if (slotSelector.getSlotExpanded(mainSlot)) {
            DBERR("-%u", subSlot);
        } else {
            DBERR("-.");
        }
    }
    DBERR("\n");
}

