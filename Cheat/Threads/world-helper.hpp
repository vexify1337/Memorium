#include "../../../Source/UE/entity-class.hpp"
#include "../../../Source/UE/external-utils.hpp"
#include "Source/Cheat/Aimbot/aimbot.hpp"


uintptr_t RootComponent(uintptr_t actor)
{
	return readd <uintptr_t>(actor + Offsets::RootComponent);
}

fvector GetLocation(uintptr_t actor)
{
	return readd <fvector>(RootComponent(actor) + offset::relative_location);
}

#define FNAMEPOOL_OFFSET 0x12D85B40  


