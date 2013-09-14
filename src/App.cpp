#include "App.h"
#include <string>
#include <deque>
#include <sstream>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspiofilemgr.h>
#include <oslib/oslib.h>
#include "Novel.h"
#include "MainMenuScene.h"

using namespace VNPSP;

App::App():
	sceneStack()
{
	// Init stuff...
	this->initOSL();
	
	// Load the Main Menu screen
	MainMenuScene *mainMenu = new MainMenuScene(this);
	this->push(mainMenu);
}

App::~App()
{
	
}

void App::initOSL()
{
	//oslInit(OSL_IF_USEOWNCALLBACKS);
	oslInit(0);
	// Don't crash if we can't load a resource
	//oslSetQuitOnLoadFailure(1);
	
	// -- Input
	// We want the D-Pad to repeat if you hold it down, makes it easier to scroll through stuff
	// Start repeating after 40 frames, then repeat every 10th
	oslSetKeyAutorepeatMask(OSL_KEYMASK_UP|OSL_KEYMASK_RIGHT|OSL_KEYMASK_DOWN|OSL_KEYMASK_LEFT);
	oslSetKeyAutorepeatInit(40);
	oslSetKeyAutorepeatInterval(10);
	// Map the analog stick to the D-Pad with 80/127 sensitivty
	// This is oslib's recommended default, considering the analog stick's dead zone
	oslSetKeyAnalogToDPad(80);
	
	// -- Graphics
	// Init graphics with 16bit (for now) and double buffering
	// Might bump it up to 32bit later if it looks too bad
	oslInitGfx(OSL_PF_5650, true);
	
	// -- Fonts
	oslIntraFontInit(INTRAFONT_CACHE_MED);
	
	// -- Synchronization
	oslSetFrameskip(1);
	oslSetMaxFrameskip(4);
}

void testDraw(int x, int y, OSL_FONT *font, std::string text)
{
	oslSetFont(font);
	oslDrawString(x, y, text.c_str());
}

void App::run()
{
	// True if the last frame was in late, in which case we should drop the next frame
	bool lastFrameWasLate = false;
	
	while(!osl_quit)
	{
		// 
		// -- Update Phase
		// 
		
		// Check the controller for new inputs; this updates osl_pad
		oslReadKeys();
		
		// Loop through the scene stack backwards to update them
		// This will stop once it finds something that /isn't/ an overlay (eg. isOverlay() returns false)
		// because there's not much of a point in updating an invisible scene
		// Store all the found Scenes in a deque so that we know what to draw later
		std::deque<Scene*> scenesToDraw;
		for(unsigned int i = sceneStack.size() - 1; i >= 0; i--)
		{
			Scene *scene = sceneStack.at(i);
			scene->tick();
			
			// Add to the front of the visible stack, since we're iterating backwards here,
			// but forward during the draw phase (otherwise overlays would be drawn under things)
			scenesToDraw.push_front(scene);
			
			// Break when we find something that isn't an overlay
			if(!scene->isOverlay()) break;
		}
		
		
		
		// 
		// -- Draw Phase
		// 
		
		// Drop the frame if the last one was in late, to avoid lag
		
		if(!lastFrameWasLate) {
			oslStartDrawing();
			oslClearScreen(RGB(128, 0, 0));
			
			// This time, loop through the array of visible scenes we determined during the
			// update phase, and forward this time.
			for(unsigned int i = 0; i < scenesToDraw.size(); i++)
			{
				Scene *scene = scenesToDraw.at(i);
				scene->draw();
			}
			
			oslEndDrawing();
		}
		
		// This takes care of timing and determining when to drop a frame
		oslEndFrame();
		lastFrameWasLate = oslSyncFrame();
	}
}

void App::push(Scene *scene)
{
	this->sceneStack.push_back(scene);
}

void App::pop()
{
	this->sceneStack.pop_back();
}
