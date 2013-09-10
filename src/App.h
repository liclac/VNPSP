#ifndef _APP_H_
#define _APP_H_

#include <map>
#include "ControllerSystem.h"

namespace VNPSP
{
	/**
	 * Main class for the engine.
	 */
	class App
	{
	public:
		App();
		~App();
		
		//
		// -- Subsystems
		//
		ControllerSystem *ctrl;		///< Controller Subsystem
		
		
		
		/**
		 * 
		 */
		void run();
	};
}

#endif
