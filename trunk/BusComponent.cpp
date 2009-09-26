#include "BusComponent.h"
#include "SlotSelector.h"

using namespace nowind;

BusComponent::BusComponent(Bus& bus) : mBus(bus), mIsMemoryMapped(false)
{
    
}

void BusComponent::initialize()
{
    // todo: think through the startup and relation building
    //mBus.addBusComponent(this);
}
