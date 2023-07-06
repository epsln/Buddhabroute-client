#ifndef mandel 
#define mandel 
#include <complex.h>

typedef struct xStuff{
	//Just a struct containing every var necessary to draw on the screen
	Display *dpy;
	Window root;
	XWindowAttributes wa;
	GC g;
}xStuff;
	

void drawTrajs(params_t params, complex* trajs);
void drawHistogram(params_t params, u_int32_t** histo);

#endif
