#ifndef _SUBSYSTEM_H_
#define _SUBSYSTEM_H_

namespace VNPSP
{
	class App;
	class Subsystem
	{
	public:
		Subsystem(App *app);
		virtual ~Subsystem();
		
		virtual void tick() = 0;
		
	protected:
		App *m_app;
	};
}

#endif
