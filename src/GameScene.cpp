#include "GameScene.h"
#include <sstream>
#include <cstring>
#include "common.h"

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
	novel(novel), script(0)
{
	// Load the text font
	textFont = oslLoadFontFile(FONT_PATH_SMALL_SANS);
	//oslIntraFontSetStyle(textFont, 1, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_LEFT);
	oslSetFont(textFont);
	
	// Load the main script
	script = new Script(novel, "main.scr");
	script->scene = this;
}

GameScene::~GameScene()
{
	
}

void GameScene::tick()
{
	if(osl_pad.pressed.cross)
		script->next();
}

void GameScene::draw()
{
	// -- Background
	if(background)
		oslDrawImage(background);
	else
		oslDrawFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(0,0,0));
	
	// -- Sprites
	
	
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
	background = oslLoadImageFile(pathDup, OSL_IN_VRAM, OSL_PF_5650);
	free(pathDup);
	
	// Position it right at the center of the screen for now.
	// I should probably resize it to fit instead...
	background->x = (SCREEN_WIDTH - background->sizeX)/2;
	background->y = (SCREEN_HEIGHT - background->sizeY)/2;
}

void GameScene::_pushLineActual(std::string text, LineType type)
{
	BufferLine line = {text, type};
	textBuffer.push_back(line);
}
