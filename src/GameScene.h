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
		/// The type of a line; controls display style
		enum LineType {
			LineTypeText,		///< Normal Text
			LineTypePBreak,		///< Paragraph Break (pushed after 'text' cmds)
			LineTypeSystem,		///< System Messages
			LineTypeDebug		///< Debug Messages (normally hidden)
		};
		
		/// Normal Constructor
		/// @param app The app the scene belongs to; from Scene
		/// @param novel The novel we're reading
		GameScene(App *app, Novel *novel);
		
		/// Destructor
		virtual ~GameScene();
		
		
		
		virtual void tick();
		virtual void draw();
		
		
		
		/// Load a script, unloading the last one plus any resources
		void loadScript(std::string filename);
		
		
		
		/// Pushes a new line onto the buffer.
		/// This function also manages splitting of the too long lines, and
		/// makes sure to push a Paragraph Break at the end for readability.
		/// @param text The text to write out
		/// @param type The type of the line
		void pushLine(std::string text, LineType type = LineTypeText);
		
		/// Clears the text buffer
		void clearBuffer();
		
		
		
		/// Loads a background image to replace the current one
		/// @param filename Image filename (relative to '<novel>/background/')
		void loadBackground(std::string filename);
		
		/// Loads a foreground sprite
		/// @param filename Image filename (relative to '<novel>/foreground/')
		/// @param origX X Position; will be adjusted by `scaleX`
		/// @param origY Y Position; will be adjusted by `origY`
		OSL_IMAGE* loadSprite(std::string filename, int origX, int origY);
		
		/// Clears all foreground sprites
		void clearSprites();
		
		
		
		OSL_IMAGE *background;				///< Pre-placed background image
		std::deque<OSL_IMAGE *> sprites;	///< Onscreen foreground sprites
		float scaleX, scaleY;				///< Scale factor (for the current bg)
		
	protected:
		OSL_FONT *textFont;		///< The font used for onscreen text
		
		Novel *novel;			///< The novel we're currently reading
		Script *script;			///< The script we're currently reading
		
		// Note:
		// A BufferLine isn't necessarily equivalent to a pushed line; pushing
		// a line that's too wide to fit on the screen will get it split up
		// into multiple, shorter BufferLine objects.
		struct BufferLine {
			std::string text;
			LineType type;
		};
		std::deque<BufferLine> textBuffer;	///< Buffered lines
		
		/// Does the actual pushing of a line, doesn't check width.
		void _pushLineActual(std::string text, LineType type);
	};
}

#endif
