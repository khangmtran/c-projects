#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "3d.h"

int main() {
	//create a scene with 10 pyramids in different locations and orientations
    Scene3D* scene = Scene3D_create();
    char* directions[] = {"up", "down", "left", "right", "forward", "backward"};
    for (int i = 0; i < 10; i++) {
        Coordinate3D origin = (Coordinate3D) {i * 20.0, i * 30.0, i * 40.0};
        char* direction = directions[i % 6]; 
        Scene3D_add_pyramid(scene, origin, 20, 30, direction);
    }

    Scene3D_write_stl_text(scene, "output.stl");
	//free the memory
    Scene3D_destroy(scene);

    return 0;
}