#include "Component.h"
#include "Emulator.h"

using namespace nowind;

Component::Component(Emulator& aEmulator) : 
mEmulator(aEmulator), 
mScheduler(aEmulator.getScheduler())
{
        
}
