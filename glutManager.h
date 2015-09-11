//This class manages the glut loop
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

//Glew is needed for C++, but not C, to avoid errors with implicit declaration
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glut.h>

#include"Vector.h"
#include"Camera.h"
#include"plane.h"
#include"cglLight.h"

class glutManager{
public:
    glutManager(int argc, char** argv);
    ~glutManager();
    void run();
private:
    Vector3d eye;
    Vector3d view;
    Vector3d up; 

    void setLights();
    void setMaterial();
    void initViewport();
    unsigned int setShaders();

    //since glut is old and bad  it won't accept member functions unless they are static
    //better to make them static and part of the class than in global scope... maybe?
    static void ballSimulation();
    static void rotate(int angle, int x, int y, int z);
    static void keyBindingsFunc(unsigned char key, int x, int y);

    static void draw();
    static void drawCube();
};

