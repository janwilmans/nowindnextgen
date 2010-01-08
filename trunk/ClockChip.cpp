#include "ClockChip.h"
#include "debug.h"
#include "Bus.h"

using namespace nowind;
using namespace fastdelegate;

ClockChip::ClockChip(Emulator& aEmulator) : BusComponent(aEmulator)
{
	DBERR("YYOOOOO clockChip in da house!!!!!!!\n");
	mTimeDiff = 0;

	mModeRegister = 0;	// todo: verify initial values
	mTestRegister = 0;
	//resetRegister = 0; // todo: bijhouden?

	for (int i=0;i<3;i++)
	{
		for (int j=0;j<13;j++)
		{
			mDataRegister[j][i] = 0;
		}
	}
}

ClockChip::~ClockChip()
{

}

void ClockChip::prepare()
{

}
void ClockChip::initialize()
{

}

void ClockChip::prepare_shutdown()
{

}

void ClockChip::attachIO()
{
    mBus.registerReadIO(0xb5, MakeDelegate(this, &ClockChip::readData));
    mBus.registerWriteIO(0xb4, MakeDelegate(this, &ClockChip::writeAddress));
    mBus.registerWriteIO(0xb5, MakeDelegate(this, &ClockChip::writeData));
}

void ClockChip::detachIO()
{

}

byte ClockChip::readData(word /*port*/)
{
	// mask unused bits (block0 and 1)
	//DBERR("RTC: read[%u] (emuTime %d)\n", address, Emulator::emuTime);
	//return 255;
	switch(mAddress)
	{
	case 0x0d:
		DBERR("RTC: read modeRegister[0x%02x]\n", mModeRegister | 0xf0);
		return mModeRegister | 0xf0;

	case 0x0e:
	case 0x0f:
		return 0xff;
	}
	
	int block = mModeRegister & 3;
	if (block == 0)
	{
		// time block
		time_t localTime;
		time_t msxTime;
		struct tm* timeinfo;
		byte value;

		localTime = time(NULL);
		msxTime = localTime - mTimeDiff;
		timeinfo = localtime(&msxTime);

		switch (mAddress)
		{
		case 0x00:	value = timeinfo->tm_sec%10; break;
		case 0x01:	value = timeinfo->tm_sec/10; break;
		case 0x02:	value = timeinfo->tm_min%10; break;
		case 0x03:	value = timeinfo->tm_min/10; break;
		case 0x04:	value = timeinfo->tm_hour%10; break;
		case 0x05:	value = timeinfo->tm_hour/10; break;
		case 0x06:	value = timeinfo->tm_wday; break;
		case 0x07:	value = timeinfo->tm_mday%10; break;
		case 0x08:	value = timeinfo->tm_mday/10; break;
		case 0x09:	value = (timeinfo->tm_mon+1)%10; break;
		case 0x0a:	value = (timeinfo->tm_mon+1)/10; break;
		case 0x0b:	value = (timeinfo->tm_year-80)%10; break;
		case 0x0c:	value = (timeinfo->tm_year-80)/10; break;
		default:
			NW_ASSERT(false);
			break;
		}
		return value | 0xf0;
	}	

	if ((block == 1) && (mAddress = 0x0b)) {
		// todo: update leap year (in alarm block)
	}
	DBERR("RTC: read register[%u][%u] value=0x%02x\n", mAddress, block, mDataRegister[mAddress][block-1] | 0xf0);
	return mDataRegister[mAddress][block-1] | 0xf0;
}

void ClockChip::writeAddress(word /*port*/, byte value)
{
	// set write and read address
	mAddress = value & 15;
}

void ClockChip::writeData(word /*port*/, byte value)
{
	value &= 15;
	
	switch (mAddress)
	{
	case 0x0d:
		// todo: acties?
		DBERR("RTC: write modeRegister[0x%02x]\n", value);
		mModeRegister = value;
		break;
	case 0x0e:
		DBERR("RTC: write testRegister[0x%02x]\n", value);
		mTestRegister = value;
		break;
	case 0x0f:
		DBERR("RTC: write resetRegister[0x%02x]\n", value);
		if (value & 1)
		{
			// reset ALARM
			for (int i=2;i<=8;i++) mDataRegister[i][0] = 0;
		}
		// todo: reset seconds?
		break;
	default:
		DBERR("RTC: write register[0x%02x][%u] with 0x%02x\n", mAddress, mModeRegister & 3, value);
		mDataRegister[mAddress][(mModeRegister & 3)-1] = value;
	}
}

