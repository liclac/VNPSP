#ifndef _CONTROLLERSYSTEM_H_
#define _CONTROLLERSYSTEM_H_

#include <pspctrl.h>
#include "Subsystem.h"

namespace VNPSP
{
	class ControllerSystem : Subsystem
	{
	public:
		ControllerSystem(App *app);
		virtual ~ControllerSystem();
		virtual void tick();
		
	private:
		SceCtrlData buttonInput;
	};
}

#endif
