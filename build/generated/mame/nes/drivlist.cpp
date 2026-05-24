#include "emu.h"

#include "drivenum.h"

GAME_EXTERN(___empty);
GAME_EXTERN(dendy);
GAME_EXTERN(dendy2);
GAME_EXTERN(drpcjr);
GAME_EXTERN(famicom);
GAME_EXTERN(famicomo);
GAME_EXTERN(famitvc1);
GAME_EXTERN(famitwin);
GAME_EXTERN(fctitler);
GAME_EXTERN(fds);
GAME_EXTERN(gchinatv);
GAME_EXTERN(iq501);
GAME_EXTERN(iq502);
GAME_EXTERN(nes);
GAME_EXTERN(nespal);
GAME_EXTERN(sb486);

game_driver const *const driver_list::s_drivers_sorted[16] =
{
	&GAME_NAME(___empty),
	&GAME_NAME(dendy),
	&GAME_NAME(dendy2),
	&GAME_NAME(drpcjr),
	&GAME_NAME(famicom),
	&GAME_NAME(famicomo),
	&GAME_NAME(famitvc1),
	&GAME_NAME(famitwin),
	&GAME_NAME(fctitler),
	&GAME_NAME(fds),
	&GAME_NAME(gchinatv),
	&GAME_NAME(iq501),
	&GAME_NAME(iq502),
	&GAME_NAME(nes),
	&GAME_NAME(nespal),
	&GAME_NAME(sb486),
};

std::size_t const driver_list::s_driver_count = 16;
