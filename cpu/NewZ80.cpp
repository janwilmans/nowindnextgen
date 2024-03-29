// NewZ80.cpp

#include "cpu/NewZ80.h"
#include <iostream>
#include <fstream>
#include "debug.h"
#include <SDL.h>
#include "Bus.h"

#define CONSOLE_DEBUGGING_ON
#define FULL_SPEED_ON

using namespace std;
using namespace fastdelegate;
using namespace nowind;

/* all of these macros are using in opcodes*.inc so they are easely be adapted
   to used on different platforms and in different configurations */

#define TS(states) localEmuTime += states

// gebruikt BUITEN executeInstructions
#define m_reg_af ((m_reg_a << 8) | m_reg_f)
#define m_reg_bc ((m_reg_b << 8) | m_reg_c)
#define m_reg_d (m_reg_de >> 8)
#define m_reg_e (m_reg_de & 255)
#define m_reg_h (m_reg_hl >> 8)
#define m_reg_l (m_reg_hl & 255)

// lokaal binnen executeInstructions
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

NewZ80::NewZ80(Emulator& aEmulator) : CPU(aEmulator)
{
    DBERR("NewZ80 constructor...\n");
     
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

    DBERR("NewZ80 constructor finished.\n");
}

void NewZ80::prepare()
{
    
}

void NewZ80::initialize()
{

}

void NewZ80::setupBdosEnv(const char* filename)
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
    m_reg_sp = 0xc800;
}

void NewZ80::prepare_shutdown()
{

}

NewZ80::~NewZ80()
{
    DBERR("z80 destruction in progress...\n");

    DBERR("z80 destroyed.\n");
}

void NewZ80::reset()
{
    DBERR("start of NewZ80 RESET\n");

    // todo: remove this, initialize this where it is suppose to be done!
    writeIO(0xa8, 0);
    WRITEMEM(0xffff, 0);

    IFF1 = IFF2 = false;  // disable interrupts
    interruptMode = 0;

    m_reg_pc = 0x0000;
    m_reg_a = 0xFF;
    m_reg_f = 0xFF;
    m_reg_b = 0xFF;
    m_reg_c = 0xFF;
    m_reg_de = 0xFFFF;
    m_reg_hl = 0xFFFF;
    m_reg_ix = 0xFFFF;
    m_reg_iy = 0xFFFF;
    m_reg_sp = 0xFFFF;
    m_reg_wz = 0;//0xFFFF;
	shadow_af = shadow_de = shadow_hl = 0xFFFF;
    shadow_b = shadow_c = 0xFF;

    m_reg_i = 0;
    m_reg_r = 0;
    refreshCounter = 0;
}

void NewZ80::setPC(word regpc)
{
    m_reg_pc = regpc;
}

void NewZ80::nmiCPU()
{
    // aantal klokpulsen???
    // IFF2 = IFF1;    /* gebeurt niet volgens Sean Young, moet de NMI routine zelf doen */
    IFF1 = false;

    // cpu weer wakker maken na een HALT
    if (READMEM(m_reg_pc) == 0x76) m_reg_pc++;

    m_reg_sp -= 2;
    WRITEMEM16(m_reg_sp, m_reg_pc); //TODO: volgens de documentatie wordt er maar een M1 uitgevoerd???
    m_reg_pc = 0x0066;
}

bool NewZ80::getIFF1()
{
    return IFF1;
}

void NewZ80::intCPU(byte interruptVectorOnDataBus)
{
    // ignore interrupt if they are disabled
    if (!IFF1) return;

    NW_ASSERT(m_reg_pc < 0x10000);

    // When an interrupt is accepted, the NewZ80 clears IFF1 and IFF2
    IFF1 = false;
    IFF2 = false;

    // cpu weer wakker maken na een HALT
    if (unlikely(READMEM(m_reg_pc) == 0x76)) m_reg_pc++;

    // Interrupt Acknowledge Cycle
    // The NewZ80 generates a special (NOT AN EXTRA) M1 cycle and adds two wait states to
    // this cycle. Futhermore, the MSX-engine adds another wait state to this M1 cycle.

    refreshCounter++;
    m_reg_sp -= 2;
    WRITEMEM16(m_reg_sp, m_reg_pc);

    switch (interruptMode)
    {
    case 0:
        //mEmuTime += 14;

        // the instruction can be any 1 byte instruction, usually this will be rst.
        // TODO: hoeveel states...
        switch (interruptVectorOnDataBus)
        {
        case 0xc7: m_reg_pc = 0x0000; break;
        case 0xcf: m_reg_pc = 0x0008; break;
        case 0xd7: m_reg_pc = 0x0010; break;
        case 0xdf: m_reg_pc = 0x0018; break;
        case 0xe7: m_reg_pc = 0x0020; break;
        case 0xef: m_reg_pc = 0x0028; break;
        case 0xf7: m_reg_pc = 0x0030; break;
        case 0xff: m_reg_pc = 0x0038; break;
        default:
            m_reg_pc = 0x0038; // just in case...
            DBERR("Alleen RST-instr. van een interrupt-device worden ondersteund!\n");
        };
        break;
    case 1:
        // RST $38 will be executed, so the total number of states is:
        // 11 (rst $38) + 2 (wait states in special M1) + 1 (MSX-engine) = 14
        // TODO: dit is onze (stellig) aanname, maar klopt dit ook werkelijk? De redenatie
        // komt overeen met die van Sean met als toevoeging 1 extra state door de engine.
        //mEmuTime += 14;
        m_reg_pc = 0x38;
        break;
    case 2:
        //mEmuTime += 20;
        m_reg_pc = READMEM16(interruptVectorOnDataBus | (m_reg_i << 8));
        break;
    default:
        // unknownd interrupt mode, this should never happen
        NW_ASSERT(false);
    }
}

//todo: fix emutime++ !!!
byte NewZ80::opcodeFetch(word address)
{

    /*
    // Meer info over extra M1 wait-state:
    // http://www.funet.fi/pub/msx/mirrors/hanso/service_manuals/yamahacx5myis503ts.pdf
	*/
    // M1 wait state (msx engine)
    //emuTime++;
    refreshCounter++;

    byte oc = READMEM(address);
    DBERR("0x%04X  0x%02X\n", address, oc);
    return oc;
}

void NewZ80::saveState()
{
    //MemoryMapper->saveState();
}

void NewZ80::loadState()
{
    //MemoryMapper->loadState();
}

void NewZ80::hijackBdos()
{
    long seconds = SDL_GetTicks() / 1000;

    switch (m_reg_c)
    {
    case 2:
        // BDOS function 2 (C_WRITE) - Console output
        // C=2, E=ASCII character
        DBERR("%c", m_reg_e);
        break;
    case 9:
    {
        // BDOS function 9 (C_WRITESTR) - Output string
        // C=9, DE=address of string
        word de = m_reg_de;
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

// the amount of cycles actually executed can vary (with max. the cycles-1 of the biggest instruction)
// because an instruction is always completely executed.
// if we are called the scheduler has determined that at least 1 instruction needs to be executed.
emuTimeType NewZ80::ExecuteInstructions(emuTimeType startTime, emuTimeType aEndTime)
{
    emuTimeType localEmuTime = startTime;
    
    //word reg_a = m_reg_a;
     word reg_f = m_reg_f;

//	word reg_b = m_reg_b;
//	word reg_c = m_reg_c;

 //   word reg_i = m_reg_i;
//    word reg_r = m_reg_r;

	//word reg_de = m_reg_de;
 //   word reg_hl = m_reg_hl;
      word reg_pc = m_reg_pc;
 //   word reg_sp = m_reg_sp;
 //   word reg_ix = m_reg_ix;
 //   word reg_iy = m_reg_iy;
 //   word reg_wz = m_reg_wz;

#define reg_a m_reg_a
//#define reg_f m_reg_f
#define reg_b m_reg_b
#define reg_c m_reg_c

#define reg_i m_reg_i
#define reg_r m_reg_r

#define reg_de m_reg_de
#define reg_hl m_reg_hl
//#define reg_pc m_reg_pc
#define reg_sp m_reg_sp
#define reg_ix m_reg_ix
#define reg_iy m_reg_iy
#define reg_wz m_reg_wz
	
	do
    {
        // define reg1/reg2 more locally! defining them here could prevent use of registers!
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
    }
    while ( ((Sint32)(aEndTime - localEmuTime)) > 0);

    m_reg_a = reg_a;
    m_reg_f = reg_f;
    m_reg_b = reg_b;
    m_reg_c = reg_c;
    
    m_reg_i = reg_i;
    m_reg_r = reg_r;
    m_reg_de = reg_de;
    m_reg_hl = reg_hl;
    m_reg_pc = reg_pc;
    m_reg_sp = reg_sp;
    m_reg_ix = reg_ix;
    m_reg_iy = reg_iy;
    m_reg_wz = reg_wz;
        
    return localEmuTime;
}
