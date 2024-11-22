#include "camera.h"
#include "timers/timers.h"

extern int cam_bridge[3];


void Cam_handle(uint8_t *data) {
	// Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej kamery
	
	// why don't we throw an error?
	if ((3 >= data[0]) && (data[0] >= 1)) {
		cam_bridge[0] = data[0]-1;
	}

	if ((3 >= data[1]) && (data[0] >= 1)) {
		cam_bridge[1] = data[1]-1;
	}

	if ((3 >= data[2]) && (data[0] >= 1)) {
		cam_bridge[2] = data[2]-1;
	}
}
