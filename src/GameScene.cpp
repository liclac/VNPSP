#include "GameScene.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include "common.h"
#include "App.h"
#include "PauseOverlayScene.h"

#define kTextBoxPadding 10
#define kTextBoxLineMargin 2
#define kTextBoxPMargin 3

#define kTextBoxPosX kTextBoxPadding
#define kTextBoxPosY kTextBoxPadding
#define kTextBoxWidth (SCREEN_WIDTH - (kTextBoxPadding*2))
#define kTextBoxHeight (SCREEN_HEIGHT - (kTextBoxPadding*2))



using namespace VNPSP;

GameScene::GameScene(App *app, Novel *novel):
	Scene(app),
	background(0),
	scaleX(SCREEN_WIDTH/(float)DS_SCREEN_WIDTH),
	scaleY(SCREEN_HEIGHT/(float)DS_SCREEN_HEIGHT),
	novel(novel), script(0)
{
	// Load the text font
	textFont = oslLoadFontFile(FONT_PATH_SMALL_SANS);
	//oslIntraFontSetStyle(textFont, 1, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	
	// Load the main script
	script = new Script(novel, "main.scr");
	script->scene = this;
	
	// Start!
	script->next();
}

GameScene::~GameScene()
{
	
}

void GameScene::tick()
{
	if(osl_pad.pressed.cross)
		script->next();
	if(osl_pad.pressed.start)
		this->app->push(new PauseOverlayScene(this->app));
}

void GameScene::draw()
{
	// -- Set the font to play nice with overlays
	oslSetFont(textFont);
	
	// -- Background
	if(background)
		oslDrawImage(background);
	else
		oslDrawFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(0,0,0));
	
	// -- Sprites
	for(std::deque<OSL_IMAGE *>::iterator it = sprites.begin(); it != sprites.end(); it++)
		oslDrawImage(*it);
	
	// -- Text
	const int lineHeight = textFont->charHeight;
	
	int xPos = kTextBoxPosX;
	int yPos = kTextBoxPosY + kTextBoxHeight;
	for(std::deque<BufferLine>::reverse_iterator it = textBuffer.rbegin(); it != textBuffer.rend(); it++)
	{
		BufferLine &line = *it;
		
		if(line.type == LineTypePBreak)
		{
			yPos -= kTextBoxPMargin;
		}
		else
		{
			yPos -= lineHeight + kTextBoxLineMargin;
			if(yPos <= kTextBoxPosY)
				break;
			
			oslDrawString(xPos, yPos, line.text.c_str());
		}
	}
}

void GameScene::pushLine(std::string text, LineType type)
{
	const char *separator = " ";
	
	std::stringstream textStream;
	std::string currentLine;
	
	// As strtok is destructive, make a duplicate of the string to work with
	char *textDup = strdup(text.c_str());
	{
		// Loop through tokens until we run out
		char *tok = strtok(textDup, separator);
		while(tok != NULL)
		{
			// Write the token and a space to the stream, and check the width
			textStream << tok << separator;
			int width = oslGetStringWidth(textStream.str().c_str());
			
			// If the width exceeds the allowed width, push what we had
			// *before* we added the last word.
			if(width >= kTextBoxWidth)
			{
				// Push the line
				this->_pushLineActual(currentLine, type);
				
				// Reset the text stream
				textStream.str(std::string());
				textStream.clear();	// Important!
				
				// Push the last word to the cleared stream
				textStream << tok << separator;
			}
			
			// Save what we currently have and advance to the next token
			currentLine = textStream.str();
			tok = strtok(NULL, separator);
		}
	}
	free(textDup);
	
	// Push the last line and a paragraph break
	this->_pushLineActual(currentLine, type);
	this->_pushLineActual("", LineTypePBreak);
}

void GameScene::clearBuffer()
{
	textBuffer.clear();
}

void GameScene::loadBackground(std::string filename)
{
	// Let the novel figure out where the image is.
	std::string path = novel->pathForResource("background", filename);
	
	// If there's an existing background, delete it first.
	// VRAM leaks are nasty.
	if(background)
		oslDeleteImage(background);
	
	// Load the new image (oslLoadImageFile and related take a char* instead
	// of a const char* for some reason, so we need a writable copy)
	char *pathDup = strdup(path.c_str());
	OSL_IMAGE *bgSrc = oslLoadImageFile(pathDup, OSL_IN_RAM|OSL_UNSWIZZLED, OSL_PF_5650);
	
	// Scale it up to fit the screen and figure out the scale factor
	background = oslScaleImageCreate(bgSrc, OSL_IN_RAM|OSL_SWIZZLED, SCREEN_WIDTH, SCREEN_HEIGHT, OSL_PF_5650);
	scaleX = bgSrc->sizeX/(float)background->sizeX;
	scaleY = bgSrc->sizeY/(float)background->sizeY;
	
	// Cleanup
	oslDeleteImage(bgSrc);
	free(pathDup);
	
	// Position it right at the center of the screen for now.
	// I should probably resize it to fit instead...
	/*background->x = (SCREEN_WIDTH - background->sizeX)/2;
	background->y = (SCREEN_HEIGHT - background->sizeY)/2;*/
}

OSL_IMAGE* GameScene::loadSprite(std::string filename, int origX, int origY)
{
	// This is basically loadBackground all over again, only with the addition
	// of setting img->x and img->y, and pushing it onto the sprite list.
	// Refer to loadBackground for more comments.
	
	std::string path = novel->pathForResource("foreground", filename);
	
	char *pathDup = strdup(path.c_str());
	OSL_IMAGE *imgSrc = oslLoadImageFile(pathDup, OSL_IN_VRAM|OSL_UNSWIZZLED, OSL_PF_5551);
	
	OSL_IMAGE *img = oslScaleImageCreate(imgSrc, OSL_IN_VRAM|OSL_SWIZZLED, ceil((float)imgSrc->sizeX * scaleX), ceil((float)imgSrc->sizeY * scaleY), OSL_PF_5551);
	
	oslDeleteImage(imgSrc);
	free(pathDup);
	
	// New-ish stuff
	img->x = origX * scaleX;	// Set x position, adjust for scale
	img->y = origY * scaleY;	// Set y position, adjust for scale
	sprites.push_back(img);		// Push it onto the sprite list
	
	return img;
}

void GameScene::clearSprites()
{
	for(std::deque<OSL_IMAGE*>::iterator it = sprites.begin(); it != sprites.end(); it++)
		oslDeleteImage(*it);
	sprites.clear();
}

void GameScene::_pushLineActual(std::string text, LineType type)
{
	BufferLine line = {text, type};
	textBuffer.push_back(line);
}
