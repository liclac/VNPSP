#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "Scene.h"
#include <deque>
#include <oslib/oslib.h>
#include "Novel.h"
#include "Script.h"

namespace VNPSP
{
	class GameScene : public Scene
	{
	public:
		enum LineType {
			LineTypeText,
			LineTypePBreak,
			LineTypeSystem,
			LineTypeDebug
		};
		
		GameScene(App *app, Novel *novel);
		virtual ~GameScene();
		
		virtual void tick();
		virtual void draw();
		
		void pushLine(std::string text, LineType type = LineTypeText);
		void loadBackground(std::string filename);
		OSL_IMAGE* loadSprite(std::string filename);
		void clearSprites();
		
		OSL_IMAGE *background;
		std::deque<OSL_IMAGE *> sprites;
		
	protected:
		OSL_FONT *textFont;
		
		Novel *novel;
		Script *script;
		
		// Note:
		// A BufferLine isn't necessarily equivalent to a pushed line; pushing
		// a line that's too wide to fit on the screen will get it split up
		// into multiple, shorter BufferLine objects.
		struct BufferLine {
			std::string text;
			LineType type;
		};
		std::deque<BufferLine> textBuffer;
		
		void _pushLineActual(std::string text, LineType type);
	};
}

#endif
