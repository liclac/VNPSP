#ifndef _SCENE_H_
#define _SCENE_H_

#include <oslib/oslib.h>

namespace VNPSP {
	class App;
	class Scene
	{
	public:
		Scene(App *app) : m_app(app) {};
		virtual ~Scene() {};
		
		virtual void tick() = 0;
		virtual void draw() = 0;
		
		virtual bool passthroughDraw() { return false; }
		virtual bool passthroughTick() { return false; }
		
	protected:
		App *m_app;
	};
}

#endif
