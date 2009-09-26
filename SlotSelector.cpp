#include "SlotSelector.h"
#include "Bus.h"

using namespace nowind;
using namespace fastdelegate;

SlotSelector::SlotSelector(Bus& aBus) : IODevice(aBus), MemoryDevice(aBus)
{
    mSSSR[0] = 0;
    mSSSR[1] = 0;
    mSSSR[2] = 0;
    mSSSR[3] = 0;
    mA8Value = 0;
}

SlotSelector::~SlotSelector()
{

}

void SlotSelector::prepare()
{

}
void SlotSelector::initialize()
{
    // initialize all slots, expanded or not with the 'VoidDevice'
}

void SlotSelector::prepare_shutdown()
{

}

void SlotSelector::attachIO()
{
    IODevice::mBus.registerReadIO(0xa8, MakeDelegate(this, &SlotSelector::readIO));
    IODevice::mBus.registerWriteIO(0xa8, MakeDelegate(this, &SlotSelector::writeIO));
}

void SlotSelector::detachIO()
{

}

byte SlotSelector::readIO(word /* port */)
{
    return mA8Value;
}

void SlotSelector::writeIO(word /* port */, byte value)
{
    mA8Value = value;
    activateCurrent();
}

byte SlotSelector::readSSSR()
{
    return 0xff;
}

void SlotSelector::writeSSSR(byte)
{
   
}

void SlotSelector::addMemoryDeviceToSlot(MemoryDevice* aMemoryDevice, Uint8 slot, Uint8 subslot)
{
    // implementation choise: just one MemoryDevice in one slot/subslot
    for (Uint8 section=0; section<constSections; section++)
    {
        slotLayout[slot][subslot][section] = aMemoryDevice;
    }
}

void SlotSelector::addMemoryDevice(MemoryDevice* aMemoryDevice, Uint8 slot, Uint8 subslot)
{
    if (subslot > 0) mSlotExpanded[slot] = true;

    if (mSlotExpanded[slot] != 0)
    {
        addMemoryDeviceToSlot(aMemoryDevice, slot, subslot);
    }
    else
    {
        for (Uint8 i=0; i<4; i++)
        {
            addMemoryDeviceToSlot(aMemoryDevice, slot, i);
        }
    }
}

#define P0PART(x) (x & 0xff)
#define P1PART(x) ((x >> 8) & 0xff)
#define P2PART(x) ((x >> 16) & 0xff)
#define P3PART(x) ((x >> 24) & 0xff)

void SlotSelector::activateCurrent()
{
    byte mainSlot = P0PART(mA8Value);
    activatePage(0, mainSlot, P0PART(mSSSR[mainSlot]));
    
    mainSlot = P1PART(mA8Value);
    activatePage(0, mainSlot, P1PART(mSSSR[mainSlot]));
    
    mainSlot = P2PART(mA8Value);
    activatePage(0, mainSlot, P2PART(mSSSR[mainSlot]));

    mainSlot = P3PART(mA8Value);
    activatePage(0, mainSlot, P3PART(mSSSR[mainSlot]));
}

void SlotSelector::activatePage(Uint8 page, Uint8 slot, Uint8 subslot)
{
    Uint8 section0 = page*2;
    Uint8 section1 = section0+1;
    
    slotLayout[slot][subslot][section0]->activate(section0);
    slotLayout[slot][subslot][section1]->activate(section1);
}
