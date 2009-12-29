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

    for (int slot=0; slot<4; slot++)
    {
        for (int subslot=0; subslot<4; subslot++)
        {   
            for (int section=0; section<constSections; section++)
            {   
                slotLayout[slot][subslot][section] = 0;
            }
        }
    }    
}

SlotSelector::~SlotSelector()
{

}

void SlotSelector::prepare()
{
    for (int slot=0; slot<4; slot++)
    {
        for (int subslot=0; subslot<4; subslot++)
        {   
            NullComponent* aNullComponent = new NullComponent(mBus);
            addBusComponent(aNullComponent, slot, subslot);
        }
    }
}

void SlotSelector::initialize()
{
    mBus.activateSSSRRead(MakeDelegate(this, &SlotSelector::readSSSR));
    mBus.activateSSSRWrite(MakeDelegate(this, &SlotSelector::writeSSSR));
}

void SlotSelector::prepare_shutdown()
{
    delete mNullComponent;
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
    byte mainSlot = mA8Value >> 6;
    byte value = mSSSR[mainSlot] ^ 0xff;
    DBERR("read SSSR = $%02X\n", value);
    return value;
}

void SlotSelector::writeSSSR(byte value)
{
    byte mainSlot = mA8Value >> 6;
    mSSSR[mainSlot] = value;
    DBERR("write SSSR = $%02X\n", value);
}

void SlotSelector::addBusComponentToSlot(BusComponent* aBusComponent, Uint8 slot, Uint8 subslot)
{
    // implementation choise: just one BusComponent in one slot/subslot
    
    // todo: figure out lifetime of NullComponents and overwritten components
    for (Uint8 section=0; section<constSections; section++)
    {
        slotLayout[slot][subslot][section] = aBusComponent;
    }
}

void SlotSelector::addBusComponent(BusComponent* aBusComponent, Uint8 slot, Uint8 subslot)
{
    aBusComponent->setSlot(this, slot, subslot);
    
    if (mSlotExpanded[slot])
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
    // if an exception occurs here, slotSelector->addBusComponent was not executed for the calling component
    byte pageShift = page*2;
    byte mainSlot = (mA8Value >> pageShift) & 0x03;
    byte subSlot = (mSSSR[mainSlot] >> pageShift) & 0x03;
    activatePage(page, mainSlot, subSlot);
}

void SlotSelector::activatePage(Uint8 page, Uint8 slot, Uint8 subslot)
{
    Uint8 section0 = page*2;
    Uint8 section1 = section0+1;
    
    slotLayout[slot][subslot][section0]->activate(section0);
    slotLayout[slot][subslot][section1]->activate(section1);
    
    /*
    const Uint8 sectionsPerPage = constSections/4;
    
    Uint8 section = page * sectionsPerPage;
    for (int i=0; i<sectionsPerPage;i++)
    {
        slotLayout[slot][subslot][section]->activate(section);
        section++;
    }
    */
}

Uint8 SlotSelector::getActivateSubslot(Uint8 slot)
{
    return mSSSR[slot];
}