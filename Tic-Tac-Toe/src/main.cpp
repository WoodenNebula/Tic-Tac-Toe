#include <iostream>
#include <GL/glut.h>

void RenderFunc() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);

    // creating a window
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    int width = 800, height = 600;
    glutInitWindowSize(width, height);
    int x_pos = 0, y_pos = 0;
    glutInitWindowPosition(x_pos, y_pos);
    const char* windowTitle = "Tic-Tac-Toe";
    glutCreateWindow(windowTitle);

    // displaying single color
    glClearColor(1, .69, .69, 0);
    glutDisplayFunc(RenderFunc);
    glutMainLoop();

    return 0;
}