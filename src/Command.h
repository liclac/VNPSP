#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

namespace VNPSP
{
	class Script;
	class Command
	{
	public:
		// Note: 'remaining' is never used anywhere else ever again.
		// Feel free to strtok it or whatever, just don't store it.
		Command(Script *script, char *remaining);
		virtual ~Command();
		virtual bool exec(bool skipping) = 0;
		
		Script *script;
	};
	
	/**
	 * An unknown command; note that the second param gets the FULL LINE.
	 */
	class UnknownCommand : public Command
	{
	public:
		UnknownCommand(Script *script, char *line);
		virtual ~UnknownCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string line;
	};
	
	/**
	 * Concrete representation of the 'text' command.
	 */
	class TextCommand : public Command
	{
	public:
		TextCommand(Script *script, char *remaining);
		virtual ~TextCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string text;
	};
	
	/**
	 * Concrete representation of the 'bgload' command.
	 */
	class BgLoadCommand : public Command
	{
	public:
		BgLoadCommand(Script *script, char *remaining);
		virtual ~BgLoadCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string filename;
	};
}

#endif
