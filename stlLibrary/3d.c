#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "3d.h"

Scene3D* Scene3D_create(){
    //Creates a new, empty 3D scene object.
    //allocate heap memory for a Scene3D object
    Scene3D* scene = malloc(sizeof(Scene3D));

    //initialize the elements to 0 and NULL
    scene->count = 0;
    scene->root = NULL;
    return scene;
}

void Scene3D_destroy(Scene3D* scene){
    //Free Triangle3DNode objects within the scene
    Triangle3DNode* node = scene->root;
    while (node != NULL){
        if(node->next == NULL){
            free(node);
            break;
        }
        Triangle3DNode* next = node->next;
        free(node);
        node = next;
    }

    //Free the Scene3D object itself.
    free(scene);
}

void Scene3D_add_triangle(Scene3D* scene, Triangle3D triangle){
    //Adds a triangle to the scene.
    //using linked list to store the triangles
    //each triangle represents a node in the linked list
    Triangle3DNode * node = malloc(sizeof(Triangle3DNode));
    node->triangle = triangle;
    node->next = NULL;

    //if the scene is empty, set the root to the new node
    //else, traverse the linked list to the end and add the new node
    if(scene->root == NULL) {
        scene->root = node;
    } else {
        Triangle3DNode * current = scene->root;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    //increment the count in the scene
    scene->count += 1;
}

void Scene3D_add_quadrilateral( Scene3D* scene,
Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d) {
    Triangle3D triangle_1 = (Triangle3D) {a, b, c};
    Triangle3D triangle_2 = (Triangle3D) {b, c, d};
    Triangle3D triangle_3 = (Triangle3D) {a, c, d};
    Triangle3D triangle_4 = (Triangle3D) {a, b, d};

    
    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
}

void Scene3D_add_pyramid(Scene3D* scene, Coordinate3D origin, double width, double height, char* orientation) {
    //Create the base of the pyramid which is a quadrilateral
    Coordinate3D a = { origin.x - width / 2, origin.y - width / 2, origin.z };
    Coordinate3D b = { origin.x + width / 2, origin.y - width / 2, origin.z };
    Coordinate3D c = { origin.x + width / 2, origin.y + width / 2, origin.z };
    Coordinate3D d = { origin.x - width / 2, origin.y + width / 2, origin.z };
    
    //Use add quadrilateral function to add the base to the scene
    Scene3D_add_quadrilateral(scene, a, b, c, d);
    
    //Create pyramid based on the orientation
    //orientation represents the direction that the top of the pyramid point towards
    Coordinate3D p;
    //right/left refers to +/- on X axis
    //so point p will have modified X value
    if(strcmp(orientation, "right") == 0) {
        p = (Coordinate3D) { origin.x + height, origin.y, origin.z };
    } else if(strcmp(orientation, "left") == 0) {
        p = (Coordinate3D) { origin.x - height, origin.y, origin.z };

    //forward/backward refers to +/- on Y axis
    //so point p will have modified Y value
    } else if(strcmp(orientation, "forward") == 0) {
        p = (Coordinate3D) { origin.x, origin.y - height, origin.z };
    } else if(strcmp(orientation, "backward") == 0) {
        p = (Coordinate3D) { origin.x, origin.y + height, origin.z };
    }

    //up/down refers to +/- on Z axis
    //so point p will have modified Z value 
    else if(strcmp(orientation, "up") == 0) {
        p = (Coordinate3D) { origin.x, origin.y, origin.z + height };
    } else if(strcmp(orientation, "down") == 0) {
        p = (Coordinate3D) { origin.x, origin.y, origin.z - height };
    } 
     
    //Add the sides of the pyramid to the scene
    Scene3D_add_triangle(scene, (Triangle3D) { a, b, p });
    Scene3D_add_triangle(scene, (Triangle3D) { b, c, p });
    Scene3D_add_triangle(scene, (Triangle3D) { c, d, p });
    Scene3D_add_triangle(scene, (Triangle3D) { d, a, p });
}

void Scene3D_write_stl_text(Scene3D* scene, char* file_name) {
    //Open file
    FILE* file = fopen(file_name, "w");
    // Write the beginning of the file with “solid NAME”
    fprintf(file, "solid scene\n");

    //Write each triangle through traversing the linked list
    Triangle3DNode* current = scene->root;
    while (current != NULL) {
        //Write the beginning of the facet
        //Within each facet use an “outer loop” and “endloop” combination
        fprintf(file, "  facet normal 0.0 0.0 0.0\n");
        fprintf(file, "    outer loop\n");

        //Write the vertices rounded to 5 decimal places
        fprintf(file, "      vertex %.5f %.5f %.5f\n", current->triangle.a.x, current->triangle.a.y, current->triangle.a.z);
        fprintf(file, "      vertex %.5f %.5f %.5f\n", current->triangle.b.x, current->triangle.b.y, current->triangle.b.z);
        fprintf(file, "      vertex %.5f %.5f %.5f\n", current->triangle.c.x, current->triangle.c.y, current->triangle.c.z);
        
        //End facet and loop
        fprintf(file, "    endloop\n");
        fprintf(file, "  endfacet\n");

        current = current->next;
    }

    //End with “endsolid NAME”
    fprintf(file, "endsolid scene\n");

    //Close file
    fclose(file);
}

void Scene3D_write_stl_binary(Scene3D* scene, char* file_name) {
    //Open file
    FILE* file = fopen(file_name, "wb");
    
    //Set the first 80 bytes to be zeroed-out.
    uint8_t header[80] = {0};
    fwrite(&header, sizeof(uint8_t), 80, file);

    //Next 4 bytes should be the facet (triangle) 
    //count as an unsigned 4 byte int (uint32_t)
    uint32_t numTriangles = (uint32_t) scene->count;
    fwrite(&numTriangles, sizeof(uint32_t), 1, file);

    //Each facet should consume exactly 50 bytes
    Triangle3DNode* current = scene->root;
    while (current != NULL) {
        //First 12 bytes should be three
        //4-bye floats representing the normal
        float normal[3] = {0.0f, 0.0f, 0.0f};
        fwrite(normal, sizeof(float), 3, file);

        //The next 36 bytes should be the nine
        //4-byte floats representing 
        //the coordinates of the corners of the triangle
        float vertices[9] = {
            (float) current->triangle.a.x, (float) current->triangle.a.y, (float) current->triangle.a.z,
            (float) current->triangle.b.x, (float) current->triangle.b.y, (float) current->triangle.b.z,
            (float) current->triangle.c.x, (float) current->triangle.c.y, (float) current->triangle.c.z
        };
        fwrite(vertices, sizeof(float), 9, file);

        //Two-byte unsigned int (uint16_t) at the end, just 0
        uint16_t endOfFacet = 0;
        fwrite(&endOfFacet, sizeof(uint16_t), 1, file);

        current = current->next;
    }

    //Close file
    fclose(file);
}


void Scene3D_add_cuboid(Scene3D* scene, Coordinate3D origin, double width, double height, double depth) {
    //Create the 8 vertices of the cuboid
    Coordinate3D a = {origin.x - width/2, origin.y - height/2, origin.z - depth/2};
    Coordinate3D b = {origin.x + width/2, origin.y - height/2, origin.z - depth/2};
    Coordinate3D c = {origin.x + width/2, origin.y + height/2, origin.z - depth/2};
    Coordinate3D d = {origin.x - width/2, origin.y + height/2, origin.z - depth/2};
    Coordinate3D e = {origin.x - width/2, origin.y - height/2, origin.z + depth/2};
    Coordinate3D f = {origin.x + width/2, origin.y - height/2, origin.z + depth/2};
    Coordinate3D g = {origin.x + width/2, origin.y + height/2, origin.z + depth/2};
    Coordinate3D h = {origin.x - width/2, origin.y + height/2, origin.z + depth/2};

    //Add each face of the cuboid as a quadrilateral
    Scene3D_add_quadrilateral(scene, a, b, c, d);
    Scene3D_add_quadrilateral(scene, a, b, f, e); 
    Scene3D_add_quadrilateral(scene, a, d, h, e);
    Scene3D_add_quadrilateral(scene, b, c, g, f);
    Scene3D_add_quadrilateral(scene, d, c, g, h);
    Scene3D_add_quadrilateral(scene, e, f, g, h);

}
                        






