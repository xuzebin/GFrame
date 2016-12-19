//
//  BasicGeometry.cpp
//  TemplateProject
//
//  Created by xuzebin on 12/17/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include <glut.h>
#include "glsupport.h"
#include "geometrymaker.h"
#include "matrix4.h"
#include "quat.h"


void init(void) {
    glClearColor(1.0, 0.0, 0.0, 1.0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void idle(void) {
    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("CS-6533");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);


    init();
    glutMainLoop();
    return 0;
}