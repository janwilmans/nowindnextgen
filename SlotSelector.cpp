#include "SlotSelector.h"
#include "Bus.h"
#include "debug.h"

using namespace nowind;
using namespace fastdelegate;

SlotSelector::SlotSelector(Bus& aBus) : BusComponent(aBus)
{
    // todo: verify initial values! (random? 0xff ?)
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
    mBus.activateSSSRRead(MakeDelegate(this, &SlotSelector::readSSSR));
    mBus.activateSSSRWrite(MakeDelegate(this, &SlotSelector::writeSSSR));
}

void SlotSelector::prepare_shutdown()
{

}

void SlotSelector::attachIO()
{
    mBus.registerReadIO(0xa8, MakeDelegate(this, &SlotSelector::readIO));
    mBus.registerWriteIO(0xa8, MakeDelegate(this, &SlotSelector::writeIO));
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
    DBERR("read SSSR = $ff\n");
    return 0xFF;
}

void SlotSelector::writeSSSR(byte value)
{
    DBERR("write SSSR = $%02X\n", value);
}

void SlotSelector::addBusComponentToSlot(BusComponent* aBusComponent, Uint8 slot, Uint8 subslot)
{
    // implementation choise: just one BusComponent in one slot/subslot
    for (Uint8 section=0; section<constSections; section++)
    {
        slotLayout[slot][subslot][section] = aBusComponent;
    }
}

void SlotSelector::addBusComponent(BusComponent* aBusComponent, Uint8 slot, Uint8 subslot)
{
    if (subslot > 0) mSlotExpanded[slot] = true;
    aBusComponent->setSlot(this, slot, subslot);
    
    if (mSlotExpanded[slot] != 0)
    {
        addBusComponentToSlot(aBusComponent, slot, subslot);
    }
    else
    {
        for (Uint8 i=0; i<4; i++)
        {
            addBusComponentToSlot(aBusComponent, slot, i);
        }
    }
}

void SlotSelector::activateCurrent()
{
    activatePage(0);
    activatePage(1);
    activatePage(2);
    activatePage(3);
}

void SlotSelector::activatePage(Uint8 page)
{
    byte pageShift = page*8;
    byte mainSlot = (mA8Value >> pageShift) & 0xff;
    byte subSlot = (mSSSR[mainSlot] >> pageShift) & 0xff;
    activatePage(page, mainSlot, subSlot);
}

void SlotSelector::activatePage(Uint8 page, Uint8 slot, Uint8 subslot)
{
    Uint8 section0 = page*2;
    Uint8 section1 = section0+1;
    
    slotLayout[slot][subslot][section0]->activate(section0);
    slotLayout[slot][subslot][section1]->activate(section1);
}
