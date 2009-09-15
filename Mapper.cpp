#include "Mapper.h"

Mapper::Mapper(Bus& bus, Uint8 banks) : BusComponent(bus)
{
    mBanks = banks;
}

Mapper::~Mapper()
{

}

void Mapper::prepare()
{

}
void Mapper::initialize()
{

}

void Mapper::prepare_shutdown()
{

}
