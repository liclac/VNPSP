#include "ControllerSystem.h"

using namespace VNPSP;

ControllerSystem::ControllerSystem(App *app) :
	Subsystem(app)
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

ControllerSystem::~ControllerSystem() {}

void ControllerSystem::tick()
{
	sceCtrlPeekBufferPositive(&this->buttonInput, 1);
}
