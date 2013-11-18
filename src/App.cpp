#include "App.h"
#include <iostream>
#include <string>
#include <deque>
#include <sstream>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspiofilemgr.h>
#include <oslib/oslib.h>
#include "Novel.h"
#include "MainMenuScene.h"

bool restartUpdatePhase = false;

using namespace VNPSP;

App::App():
	sceneStack(),
	deadScenes()
{
	// Init stuff...
	this->initOSL();
	
	// Load the Main Menu screen
	this->push(new MainMenuScene(this));
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
	oslIntraFontInit(INTRAFONT_CACHE_ASCII);
	
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
		
		std::deque<Scene*> scenesToDraw;
		do
		{
			// Reset stuff
			restartUpdatePhase = false;
			scenesToDraw.clear();
			
			// Loop through the scene stack backwards to update them and add them
			// to a list of things to draw during the draw phase. Scenes can allow
			// passthrough of draw and/or tick to ones below it in the stack, to
			// allow for semi/transparent overlays with or without their own logic.
			// Note: for performance, tick() calls are done top-down, while draw()
			// calls are done bottom-up, otherwise overlays would be drawn over.
			bool tickStopHit = false;
			bool drawStopHit = false;
			for(std::deque<Scene*>::reverse_iterator it = sceneStack.rbegin(); it != sceneStack.rend(); it++)
			{
				Scene *scene = *it;

				// tick() scenes until we find one that won't allow passthrough
				if(!tickStopHit)
				{
					scene->tick();
					tickStopHit = !scene->passthroughTick();
				}
				
				// If the last update phase caused a reset of some kind, restart
				// the update phase to account for it.
				if(restartUpdatePhase)
					break;
				
				// Add to the front of the visible stack, since we're iterating backwards here,
				// but forward during the draw phase (otherwise overlays would be drawn under things)
				if(!drawStopHit)
				{
					scenesToDraw.push_front(scene);
					drawStopHit = !scene->passthroughDraw();
				}
				
				// No point in continuing if we've already hit both stops
				if(tickStopHit && drawStopHit)
					break;
			}
		} while(restartUpdatePhase);
		
		
		
		// 
		// -- Draw Phase
		// 
		
		// Drop the frame if the last one was in late, to avoid lag
		if(!lastFrameWasLate)
		{
			oslStartDrawing();
			oslClearScreen(RGB(128, 0, 0));
			
			// This time, loop through the array of visible scenes we determined during the
			// update phase, and forward this time.
			for(std::deque<Scene*>::iterator it = scenesToDraw.begin(); it != scenesToDraw.end(); it++)
				(*it)->draw();
			
			oslEndDrawing();
		}
		
		// This takes care of timing and determining when to drop a frame
		oslEndFrame();
		lastFrameWasLate = oslSyncFrame();
		
		
		
		// Delete dead scenes after they are no longer needed
		for(std::deque<Scene*>::iterator it = deadScenes.begin(); it != deadScenes.end(); it++)
			delete *it;
		deadScenes.clear();
	}
}

void App::push(Scene *scene)
{
	this->sceneStack.push_back(scene);
}

void App::pop()
{
	this->deadScenes.push_back(this->sceneStack.back());
	this->sceneStack.pop_back();
}

void App::reset()
{
	std::cout << "Resetting..." << std::endl;
	this->deadScenes.insert(this->deadScenes.end(), this->sceneStack.begin(), this->sceneStack.end());
	this->sceneStack.clear();
	
	std::cout << "Creating new Main Menu..." << std::endl;
	this->push(new MainMenuScene(this));
	
	std::cout << "Marking update phase for repetition.." << std::endl;
	restartUpdatePhase = true;
	
	std::cout << "Done!" << std::endl;
}
