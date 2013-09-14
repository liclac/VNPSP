#include <pspkernel.h>
#include <oslib/oslib.h>
#include "common.h"
#include "App.h"

PSP_MODULE_INFO("VNPSP", USE_KERNEL_MODE, VERSION_MAJOR, VERSION_MINOR);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_MAX();

using namespace VNPSP;

int main()
{
	App *app = new App();
	app->run();
	
	sceKernelExitGame();
	return 0;
}
