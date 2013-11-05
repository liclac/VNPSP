#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <string>
#include <deque>
#include "Command.h"

namespace VNPSP
{
	class GameScene;
	class Novel;
	class Script
	{
	public:
		Script(Novel *novel, std::string filename);
		virtual ~Script();
		
		void next(bool skipping = false);
		
		GameScene *scene;
		Novel *novel;
		
	protected:
		void load(std::string filename);
		
		std::deque<Command *> commands;
		unsigned int pos;
	};
}

#endif
