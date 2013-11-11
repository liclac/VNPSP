#ifndef _PAUSEOVERLAYSCENE_H_
#define _PAUSEOVERLAYSCENE_H_

#include "Scene.h"
#include <oslib/oslib.h>

namespace VNPSP
{
	class PauseOverlayScene : public Scene
	{
	public:
		PauseOverlayScene(App *app);
		~PauseOverlayScene();
		
		virtual void tick();
		virtual void draw();
		
		virtual bool passthroughDraw() { return true; }
		
	protected:
		OSL_FONT *font;
		
		enum Option {
			OptionContinue,
			OptionSave,
			OptionLoad,
			OptionQuit,
			_OptionCount
		};
		int selection;
		
		void drawOption(int startY, Option option, const char *text);
		void drawMenuItem(int yPos, bool selected, const char *text);
	};
}

#endif
