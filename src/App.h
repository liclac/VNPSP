#ifndef _APP_H_
#define _APP_H_

#include <deque>
#include <map>
#include "Scene.h"

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
		
		void run();
		void push(Scene *scene);
		void pop();
		void reset();
		
	protected:
		void initOSL();
		
		std::deque<Scene*> sceneStack;
		std::deque<Scene*> deadScenes;
	};
}

#endif
