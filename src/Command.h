#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

namespace VNPSP
{
	class Script;
	
	/**
	 * Abstract base class for all commands.
	 */
	class Command
	{
	public:
		/// Standard Constructor
		/// 
		/// Note: 'remaining' is a copy of the command line, offset past the
		/// command name. This is freed shortly after the command is initalized
		/// and never used by anything else, so feel free to strtok() it or
		/// whatever. Just don't store it - it'll be deallocated shortly.
		/// 
		/// @param script The script the command belongs to
		/// @param remaining The remainder of the string
		Command(Script *script, char *remaining);
		
		/// Destructor
		virtual ~Command();
		
		/// Executes the command (implemented by subclasses)
		virtual bool exec(bool skipping) = 0;
		
		/// The script the command belongs to
		Script *script;
	};
	
	/// Concrete representation of an unrecognized command.
	/// Note: the second constructor parameter gets the full line!
	class UnknownCommand : public Command
	{
	public:
		UnknownCommand(Script *script, char *line);
		virtual ~UnknownCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string line;		///< The unrecognized line
	};
	
	/// Concrete representation of the 'text' command.
	class TextCommand : public Command
	{
	public:
		TextCommand(Script *script, char *remaining);
		virtual ~TextCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string text;		///< The text
		bool cont;				///< Continue to the next line right away?
	};
	
	/// Concrete representation of the 'cleartext' command.
	class ClearTextCommand : public Command
	{
	public:
		ClearTextCommand(Script *script, char *remaining);
		virtual ~ClearTextCommand();
		virtual bool exec(bool skipping);
		
	protected:
		bool hard;				///< Do a "hard" break (delete backlog)
	};
	
	/// Concrete representation of the 'bgload' command.
	class BgLoadCommand : public Command
	{
	public:
		BgLoadCommand(Script *script, char *remaining);
		virtual ~BgLoadCommand();
		virtual bool exec(bool skipping);
		
	protected:
		std::string filename;	///< The filename of the image to load
	};
}

#endif
