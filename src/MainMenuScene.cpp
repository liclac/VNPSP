#include "MainMenuScene.h"

#include <sstream>
#include <pspiofilemgr.h>

#include "common.h"
#include "App.h"
#include "Novel.h"
#include "GameScene.h"

using namespace VNPSP;

// Definitions used for rendering the list
#define MAIN_MENU_ROW_HEIGHT 75
#define MAIN_MENU_IMAGE_WIDTH 100
#define MAIN_MENU_IMAGE_MARGIN 10
#define MAIN_MENU_ROW_MARGIN 10

MainMenuScene::MainMenuScene(App *app) :
	Scene(app),
	selectedIndex(0)
{
	this->loadNovels();
	this->loadUIResources();
}

MainMenuScene::~MainMenuScene()
{
	
}

void MainMenuScene::loadNovels()
{
	// Open the novel directory for reading...
	std::string novelsPath = NOVELS_PATH;
	SceUID dfd = sceIoDopen(novelsPath.c_str());
	
	// Create a zeroed reserved space for entries.
	// We have to zero it out before every sceIoDread call,
	// because otherwise it fails with an error.
	SceIoDirent entry;
	memset(&entry, 0, sizeof(entry));
	
	// Loop through the directory contents.
	// status > 0: there are more entries.
	// status = 0: there are NO more entries.
	// status < 0: error.
	int status = 0;
	while((status = sceIoDread(dfd, &entry)) > 0)
	{
		// Nope, we're not even gonna check . and ..
		if(strcmp(entry.d_name, ".") == 0 || strcmp(entry.d_name, "..") == 0)
		{
			memset(&entry, 0, sizeof(entry));
			continue;
		}
		
		// Get a path to it
		std::string path = novelsPath + std::string(entry.d_name);
		
		// If it has an info.txt, and is in here, it's probably a novel.
		// Stat'ing the info.txt file is a good way to figure out if it exists.
		// Just like sceIoDread, this fails if you don't zero out the out struct.
		SceIoStat stat;
		memset(&stat, 0, sizeof(stat));
		if(sceIoGetstat((path + std::string("/index.txt")).c_str(), &stat))
		{
			Novel *novel = new Novel(path);
			this->novels.push_back(novel);
		}
		
		// Break when we run out of file entries
		if(status == 0)
			break;
		
		// Zero out the entry struct before the next loop.
		// As noted above, not doing this makes sceIoDread fail.
		memset(&entry, 0, sizeof(entry));
	}
	
	// Cleanup
	sceIoDclose(dfd);
}

void MainMenuScene::loadUIResources()
{
	// Load the default font and set it to be used.
	// The styles are all defaults, but it's better to be overly specific than
	// to be vague and get trouble with implicit behavior later on.
	listFont = oslLoadFontFile(FONT_PATH_BIG_SANS);
	oslIntraFontSetStyle(listFont, 1, RGBA(255,255,255,255), RGBA(0,0,0,255), INTRAFONT_ALIGN_LEFT);
	oslSetFont(listFont);
}

void MainMenuScene::tick()
{
	if(novels.size() == 0)
		return;
	
	if(osl_pad.pressed.cross)
	{
		Novel *selectedNovel = novels.at(selectedIndex);
		GameScene *gameScene = new GameScene(m_app, selectedNovel);
		this->m_app->push(gameScene);
	}
	else if(osl_pad.pressed.up)
	{
		if(selectedIndex > 0) selectedIndex--;
		else selectedIndex = novels.size()-1;
	}
	else if(osl_pad.pressed.down)
	{
		if(selectedIndex < novels.size()-1) selectedIndex++;
		else selectedIndex = 0;
	}
}

void MainMenuScene::draw()
{
	
	if(novels.size() == 0)
	{
		static const char *text = "No Novels";
		int width = oslGetStringWidth(text);
		int height = listFont->charHeight;
		
		oslDrawString((SCREEN_WIDTH - width)/2, (SCREEN_HEIGHT - height)/2, text);
	}
	else
	{
		static const int leftMargin = 10;
		static const int yCenter = (SCREEN_HEIGHT - MAIN_MENU_ROW_HEIGHT)/2;
		static const int rowOffset = (MAIN_MENU_ROW_HEIGHT + MAIN_MENU_ROW_MARGIN);
		
		// Previous novels...
		if(selectedIndex >= 2)
			this->drawListItem(leftMargin, yCenter - rowOffset*2, novels[selectedIndex-2], false);
		if(selectedIndex >= 1)
			this->drawListItem(leftMargin, yCenter - rowOffset*1, novels[selectedIndex-1], false);
		
		// ...selected novel...
		this->drawListItem(leftMargin, yCenter, novels[selectedIndex], true);
		
		// ...and the next ones.
		if(selectedIndex < novels.size()-1)
			this->drawListItem(leftMargin, yCenter + rowOffset*1, novels[selectedIndex+1], false);
		if(selectedIndex < novels.size()-2)
			this->drawListItem(leftMargin, yCenter + rowOffset*2, novels[selectedIndex+2], false);
	}
}

void MainMenuScene::drawListItem(int x, int y, Novel *novel, bool selected)
{
	// Draw a black square instead of the thumbnail until I figure
	// out why libpng refuses to load them...
	//oslDrawFillRect(x, y, x + MAIN_MENU_IMAGE_WIDTH, y + MAIN_MENU_ROW_HEIGHT, RGB(0,0,0));
	
	// TODO: Scale incorrectly sized images
	oslDrawImageXY(novel->thumbnail(), x, y);
	
	// And draw the title alongside it; the XMB only draws it on the
	// selected title, but drawing it everywhere looks better IMO.
	// Note that text is vertically centered, so you need to add
	// half the character height to y to make it look right.
	oslDrawString(x + MAIN_MENU_IMAGE_WIDTH + MAIN_MENU_IMAGE_MARGIN, y + (MAIN_MENU_ROW_HEIGHT/2) - (listFont->charHeight/2),
		novel->info["title"].c_str());
}
