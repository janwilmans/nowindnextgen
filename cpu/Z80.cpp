// Z80.cpp

#include <iostream>
#include <fstream>
#include "cpu/Z80.h"
#include "debug.h"
#include <SDL.h>

#define CONSOLE_DEBUGGING_ON
#define FULL_SPEED_ON

using namespace std;
using namespace fastdelegate;
using namespace nowind;

Z80::Z80(AddressBus& addressBus, IOBus& ioBus) : CPU(addressBus, ioBus)
{
    DBERR("Z80 constructor...\n");

    mEmulator = Emulator::Instance();

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

    // load zexall.com at 0x100 (testing only)
    ifstream romfile("cpu/zexall/zexall.com", ios::binary);
    bool failed = romfile.fail();
    NW_ASSERT(!failed);

    romfile.seekg(0, ios::end);
    unsigned long fileSize = romfile.tellg();
    DBERR("loaded zexall.com (%u bytes)\n", fileSize);

    char* temp = new char[fileSize];
    romfile.seekg(0);
    romfile.read(temp, fileSize);
    romfile.close();

    // dont read files into memory directly
    // memory locations are not guaranteed to be '1 byte' locations
    for (Uint16 i = 0;i < fileSize;i++)
    {
        memblock[0x100+i] = temp[i];
    }
    delete temp;

    // patch bdos call 0x0005
    memblock[0x0005] = 0xED;
    memblock[0x0006] = 0x0E;
    memblock[0x0007] = 0xC9;

    DBERR("Z80 constructor finished.\n");
}

void Z80::initialize()
{
    DBERR("Cpu initialized...\n");
}

Z80::~Z80()
{
    DBERR("z80 destruction in progress...\n");

    DBERR("z80 destroyed.\n");
}

void Z80::reset()
{
    DBERR("start of Z80 RESET\n");

    /* rom inschakelen */
    writeIo(0xa8, 0);
    writeMem(0xffff, 0);

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
    if (readMem(reg_pc) == 0x76) reg_pc++;

    reg_sp -= 2;
    writeMem16(reg_sp, reg_pc); //TODO: volgens de documentatie wordt er maar een M1 uitgevoerd???
    reg_pc = 0x0066;
}

bool Z80::getIFF1()
{
    return IFF1;
}

void Z80::intCPU(byte interruptVectorOnDataBus)
{
    NW_ASSERT(reg_pc < 0x10000);
    NW_ASSERT(IFF1);   // if ints are disabled, when should never be called

    // When an interrupt is accepted, the Z80 clears IFF1 and IFF2
    IFF1 = false;
    IFF2 = false;

    //mEmulator-> call something to de-schedule new cpu-interupts

    // cpu weer wakker maken na een HALT
    if (unlikely(readMem(reg_pc) == 0x76)) reg_pc++;

    // Interrupt Acknowledge Cycle
    // The Z80 generates a special (NOT AN EXTRA) M1 cycle and adds two wait states to
    // this cycle. Futhermore, the MSX-engine adds another wait state to this M1 cycle.

    refreshCounter++;
    reg_sp -= 2;
    writeMem16(reg_sp, reg_pc);

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
        reg_pc = readMem16(interruptVectorOnDataBus | (reg_i << 8));
        break;
    default:
        // unknownd interrupt mode, this should never happen
        NW_ASSERT(false);
    }
}

inline byte Z80::opcodeFetch(word address)
{

    /*
    // Meer info over extra M1 wait-state:
    // http://www.funet.fi/pub/msx/mirrors/hanso/service_manuals/yamahacx5myis503ts.pdf

    // M1 wait state (msx engine)
    emuTime++;
    refreshCounter++;
    */

    byte oc = readMem(address);
    //DBERR("0x%04X  0x%02X\n", address, oc);
    return oc;
}


// execute instructions until the "nextInterrupt" time, nextInterrupt is a member variable so
// DI/EI can effect it! (via emulator::setCPUInterrupt())
//
// the amount of cycles actually executed can vary (with max. the cycles-1 of the biggest instruction)
// because an instruction is always completely executed.

// todo: optimize this method by copying the emutime into a local variable,
//   also add the 'register' compiler hint to 'endTime' and 'localEmuTime'
//   this will require passing the actual emutime at every readMem/writeMem readIO/writeIO
//   put the localemutime back into global emutime at exit of executeInstructions()
emuTimeType Z80::ExecuteInstructionsUntil(emuTimeType startTime, emuTimeType aEndTime)
{
    emuTimeType localEmuTime = startTime;
    mEndTime = aEndTime;

    // TODO: 1 instruction is always executed before the next interrupt occurs,
    // this is sometimes usefull, but not accurate.. why did we do that ?
    // see also Emulator::setCPUInterrupt
    // jan: if we are called the scheduler has determined that at least 1 instruction _needs_
    //      to be executed, so the behaviour is correct. we need to document these rationales somewhere.

    do
    {
        word reg1 = 0;
        word reg2 = 0;
        word opcode = 0;
        opcode = opcodeFetch(reg_pc);
        ++reg_pc;

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

//      NW_ASSERT (reg_sp != 0x0fffe);  // duidt op een stack overflow, vaak wordt dat veroorzaakt door een eerder probleem
//      NW_ASSERT (reg_sp != 1);        // komt voor in zexall!
//      NW_ASSERT (reg_sp != 2)         // jan: dat kan ook voorkomen als instructies worden getest die sp gebruiken,
        // als je maar zorgt dat je sp niet gebruikt op dat moment, gaat dat wel goed.

    }
    while ((mEndTime - localEmuTime) > 0); // end of while-not-next-interrupt

    return localEmuTime;
}

byte Z80::readMem(word address)
{

    return memblock[address] & 0xff;
}

word Z80::readMem16(word address)
{
    word addressHigh = (address + 1) & 0xffff;
    return readMem(address) & 0xff | (readMem(addressHigh) << 8);
}

void Z80::writeMem(word address, byte value)
{
    memblock[address] = value;
}

void Z80::writeMem16(word address, word value)
{
    word addressHigh = (address + 1) & 0xffff;
    writeMem(address, value & 0xff);
    writeMem(addressHigh, value >> 8);
}

void Z80::writeIo(word port, word value)
{

    NW_ASSERT(port < 0x100);
    NW_ASSERT(value < 0x100);
}

byte Z80::readIo(word port)
{

    NW_ASSERT(port < 0x100);
    return 0xff;
}

void Z80::dumpCpuInfo()
{

    DBERR(" AF:%04X BC:%04X DE:%04X HL:%04X", reg_af, reg_bc, reg_de, reg_hl);
    DBERR(" IX:%04X IY:%04X SP:%04X(%04X) F:", reg_ix, reg_iy, reg_sp, 0x1234);//readMem16(reg_sp));

    if (reg_f & SFLAG) DBERR("s");
    else DBERR(".");
    if (reg_f & ZFLAG) DBERR("z");
    else DBERR(".");
    if (reg_f & 0x20) DBERR("1");
    else DBERR("0");
    if (reg_f & HFLAG) DBERR("h");
    else DBERR(".");
    if (reg_f & 0x08) DBERR("1");
    else DBERR("0");
    if (reg_f & PFLAG) DBERR("p");
    else DBERR(".");
    if (reg_f & NFLAG) DBERR("n");
    else DBERR(".");
    if (reg_f & CFLAG) DBERR("c");
    else DBERR(".");

    /*
    int slot = ppi->readPortA();
    for (int i=0;i<4;i++) {
        int mainSlot = (slot>>(i*2))&3;
        Uint8 subSlot = slotSelector->getSelectedSubSlot(i);
        DBERR(" %u", mainSlot);
        if (slotSelector->isExpanded(mainSlot)) {
            DBERR("%u", subSlot);
        } else {
            DBERR("x");
        }
    }
    */
    DBERR("\n");
}

/* can return invalid information if the PPI is not yet initialized */
void Z80::dumpSlotSelection()
{

    /*
    int slot = ppi->readPortA();
    DBERR("Slot selection:\n");
    for (int i=0;i<4;i++) {
        int mainSlot = (slot>>(i*2))&3;
        Uint8 subSlot = slotSelector->getSelectedSubSlot(i);
        DBERR(" page%u (%u", i, mainSlot);
        if (slotSelector->isExpanded(mainSlot)) {
            DBERR("-%u", subSlot);
        }
        DBERR(") %s\n", slotSelector->getDeviceName(i).c_str());
    }
    */
}

void Z80::dumpPages()
{

    string filename("dumppages.bin");

    /* delete the existing? file */
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

void Z80::saveState()
{
    //mapper->saveState();
}

void Z80::loadState()
{
    //mapper->loadState();
}

word Z80::getSP()
{
    return reg_sp;
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
            byte c = readMem(de++);
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