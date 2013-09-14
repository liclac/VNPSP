#ifndef _MAINMENUSCENE_H_
#define _MAINMENUSCENE_H_

#include "Scene.h"
#include <deque>
#include <oslib/oslib.h>
#include "Novel.h"

namespace VNPSP
{
	class MainMenuScene : public Scene
	{
	public:
		MainMenuScene(App *app);
		virtual ~MainMenuScene();
	
		virtual void tick();
		virtual void draw();
		
	protected:
		void loadNovels();
		void loadUIResources();
		
		void drawListItem(int x, int y, Novel *novel, bool selected);
		
		std::deque<Novel*> novels;
		unsigned int selectedIndex;
		
		OSL_FONT *listFont;
	};
}

#endif
