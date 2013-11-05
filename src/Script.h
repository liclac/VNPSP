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
		/// Normal Constructor
		/// Initializes variables and calls load().
		/// @param novel The novel the script belongs to
		/// @param filename Script filename (relative to '<novel>/script/')
		Script(Novel *novel, std::string filename);
		
		///Destructor.
		virtual ~Script();
		
		/// Process the next command
		/// @param skipping Whether we're currently fast-forwarding for some reason
		void next(bool skipping = false);
		
		GameScene *scene;		///< The GameScene currently managing rendering
		Novel *novel;			///< The novel the script belongs to
		
	protected:
		/// Performs the actual loading of the script from a file
		/// @param filename Script filename (relative to '<novel>/script/')
		void load(std::string filename);
		
		// Script Data
		std::deque<Command *> commands;		///< All commands in the script
		unsigned int pos;					///< Current position
	};
}

#endif
