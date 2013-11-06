#include "Script.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "GameScene.h"
#include "Novel.h"
#include "Command.h"

using namespace VNPSP;

Script::Script(Novel *novel, std::string filename):
	novel(novel),
	pos(0)
{
	this->load(filename);
}

Script::~Script()
{
	// Loop through all the commands and delete them
	for(std::deque<Command*>::iterator it = commands.begin(); it != commands.end(); it++)
		delete *it;
}

void Script::next(bool skipping)
{
	// Boolean to determine if we should go right to the next command.
	bool cont = false;
	
	do
	{
		// Set this back to true to make sure we don't get an infinite loop!
		cont = false;
		
		// Make sure we don't go past the end of the script!
		if(pos >= commands.size())
		{
			std::cout << "-- Reached the end of the script!" << std::endl;
			break;
		}
		
		std::cout << "-- Executing Command " << pos << " of " <<
						commands.size() << "..." << std::endl;
		
		// Retrieve and execute the next command
		Command *cmd = commands.at(pos);
		cont = cmd->exec(skipping);
		++pos;
	}
	while(cont);
}

void Script::load(std::string filename)
{
	// Figure out where the source file is and open it
	std::string path = novel->pathForResource("script", filename);
	std::ifstream file(path.c_str());
	
	// Loop through the file line-by-line with getline()
	// (I would do this with pure C, but the PSP SDK lacks C getline())
	std::string line;
	while(std::getline(file, line))
	{
		char *cLine = strdup(line.c_str());
		
		// Skip empty lines and comments
		if(strlen(cLine) == 0 || cLine[0] == '#')
			continue;
		
		// Figure out how long the command is, and what it is
		char *pbrk = strpbrk(cLine, " \t\r");
		size_t cmdLength = (pbrk != NULL ? pbrk - cLine : strlen(cLine));
		char *remaining = cLine + (pbrk != NULL ? cmdLength + 1 : 0);
		
		// Create a command object for each command we pass
		if(strncmp("text", cLine, cmdLength) == 0)
			commands.push_back(new TextCommand(this, remaining));
		else if(strncmp("cleartext", cLine, cmdLength) == 0)
			commands.push_back(new ClearTextCommand(this, remaining));
		else if(strncmp("bgload", cLine, cmdLength) == 0)
			commands.push_back(new BgLoadCommand(this, remaining));
		// If we can't match it, make an UnknownCommand placeholder
		else
			commands.push_back(new UnknownCommand(this, cLine));
		
		free(cLine);
	}
}
