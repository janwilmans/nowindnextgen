#include "BusComponent.h"

using namespace nowind;

BusComponent::BusComponent(Bus& bus) : mBus(bus)
{
    mBus.addBusComponent(this);
}