#include "PauseOverlayScene.h"
#include <iostream>
#include "common.h"
#include "App.h"

#define kOptionSpacing 10

using namespace VNPSP;

PauseOverlayScene::PauseOverlayScene(App *app):
	Scene(app),
	selection(0)
{
	font = oslLoadFontFile(FONT_PATH_BIG_SANS);
}

PauseOverlayScene::~PauseOverlayScene() { }

void PauseOverlayScene::tick()
{
	if(osl_pad.pressed.up && selection > 0)
		--selection;
	if(osl_pad.pressed.down && selection < _OptionCount-1)
		++selection;
	
	if(osl_pad.pressed.start)
		this->app->pop();
	else if(osl_pad.pressed.cross)
	{
		switch((Option)selection)
		{
			case OptionContinue:
				this->app->pop();
				break;
			case OptionSave:
			case OptionLoad:
			case OptionQuit:
				this->app->reset();
				break;
			default:
				std::cerr << "Unknown Option Selected! " << selection << std::endl;
				break;
		}
	}
	
	std::cout << "end PauseOverlayScene::tick()" << std::endl;
}

void PauseOverlayScene::draw()
{
	oslSetFont(font);
	
	// Darken the game scene to make the menu more prominent, but without
	// actually hiding the game. Just because I can.
	oslDrawFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA(0,0,0,128));
	
	// Calculate menu metrics
	const int menuHeight = (osl_curFont->charHeight * _OptionCount) + (kOptionSpacing * _OptionCount-1);
	const int startY = (SCREEN_HEIGHT - menuHeight) / 2;
	
	// Draw the menu items
	this->drawOption(startY, OptionContinue, "Continue");
	this->drawOption(startY, OptionSave, "Save");
	this->drawOption(startY, OptionLoad, "Load");
	this->drawOption(startY, OptionQuit, "Quit");
}

void PauseOverlayScene::drawOption(int startY, Option option, const char *text)
{
	this->drawMenuItem(
		startY + (osl_curFont->charHeight + kOptionSpacing)*(int)option,
		selection == (int)option,
		text
	);
}

void PauseOverlayScene::drawMenuItem(int yPos, bool selected, const char *text)
{
	const char *marker = "> ";
	const int lineWidth = oslGetStringWidth(text);
	const int xPos = (SCREEN_WIDTH - lineWidth)/2;
	
	// If the item is selected, draw the marker /before/ it;
	// Otherwise the marker's shadow may overlap the text and look weird
	if(selected)
		oslDrawString(xPos - oslGetStringWidth(marker), yPos, marker);
	
	// Then just draw the text, centered
	oslDrawString(xPos, yPos, text);
}
