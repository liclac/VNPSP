#include "Command.h"
#include <iostream>
#include "Script.h"
#include "GameScene.h"
#include "Novel.h"

using namespace VNPSP;

Command::Command(Script *script, char *remainingLine):
	script(script) { }
Command::~Command() { }

// 
// -- Unknown Command
// 
UnknownCommand::UnknownCommand(Script *script, char *line):
	Command(script, line),
	line(line)
{
	
}

UnknownCommand::~UnknownCommand() {}

bool UnknownCommand::exec(bool skipping)
{
	this->script->scene->pushLine(std::string("<< ") + line + std::string(" >>"), GameScene::LineTypeSystem);
	return true;
}

// 
// -- NOOP
// 
NOOPCommand::NOOPCommand(Script *script): Command(script, NULL) { }
NOOPCommand::~NOOPCommand() { }
bool NOOPCommand::exec(bool skipping) { return true; }

// 
// -- Text
// 
TextCommand::TextCommand(Script *script, char *remainingLine):
	Command(script, remainingLine),
	cont(false)
{
	// If the line begins with @, proceed to the next one right away.
	// If it's ~, print a blank line and proceed to the next line.
	// If it's !, print a blank line, but DON'T proceed to the next.
	if(strcmp(remainingLine, "~") == 0)
		cont = true;
	else if(strcmp(remainingLine, "!") == 0)
		cont = false;
	else if(strncmp(remainingLine, "@", 1) == 0)
	{
		text = remainingLine + (size_t)1;
		cont = true;
	}
	else
	{
		text = remainingLine;
		cont = false;
	}
}

TextCommand::~TextCommand() { }

bool TextCommand::exec(bool skipping)
{
	this->script->scene->pushLine(text);
	return cont;
}

// 
// -- Clear Text
// 
ClearTextCommand::ClearTextCommand(Script *script, char *remainingLine):
	Command(script, remainingLine),
	hard(strncmp(remainingLine, "!", 1) == 0)
{
	
}

ClearTextCommand::~ClearTextCommand() { }

bool ClearTextCommand::exec(bool skipping)
{
	//this->script->scene->clearBuffer();
	this->script->scene->pushLine("-- cleartext --");
	return true;
}

// 
// -- BgLoad
// 
BgLoadCommand::BgLoadCommand(Script *script, char *remainingLine):
	Command(script, remainingLine)
{
	const char *tmp = strtok(remainingLine, " ");
	filename = (tmp ? tmp : remainingLine);
}

BgLoadCommand::~BgLoadCommand() { }

bool BgLoadCommand::exec(bool skipping)
{
	this->script->scene->clearSprites();
	this->script->scene->loadBackground(this->filename);
	return true;
}

// 
// -- SetImg
// 
SetImgCommand::SetImgCommand(Script *script, char *remainingLine):
	Command(script, remainingLine)
{
	const char *tmp = strtok(remainingLine, " ");
	filename = (tmp ? tmp : remainingLine);
	
	tmp = strtok(NULL, " ");
	xPos = (tmp ? atoi(tmp) : 0);
	
	tmp = strtok(NULL, " ");
	yPos = (tmp ? atoi(tmp) : 0);
}

SetImgCommand::~SetImgCommand() { }

bool SetImgCommand::exec(bool skipping)
{
	this->script->scene->loadSprite(filename, xPos, yPos);
	return true;
}
