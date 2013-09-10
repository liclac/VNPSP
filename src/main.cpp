#include <pspkernel.h>
#include "App.h"

#define USE_KERNEL_MODE 0
#define VERSION_MAJOR 0
#define VERSION_MINOR 1

PSP_MODULE_INFO("VNPSP", USE_KERNEL_MODE, VERSION_MAJOR, VERSION_MINOR);

using namespace VNPSP;



/*
 * -- Common code for handling the home button --
 */
int __exitCallback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int __exitCallbackThread(SceSize argc, void *argv)
{
	int callbackID = sceKernelCreateCallback("Exit Callback", __exitCallback, NULL);
	sceKernelRegisterExitCallback(callbackID);
	
	sceKernelSleepThreadCB();
	return 0;
}

int setupExitCallback()
{
	int threadID = sceKernelCreateThread("Exit Callback Thread", __exitCallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(threadID >= 0)
		sceKernelStartThread(threadID, 0, 0);
	return threadID;
}



/*
 * Main function
 */
int main()
{
	setupExitCallback();
	
	App *app = new App();
	app->run();
	
	sceKernelExitGame();
	return 0;
}
