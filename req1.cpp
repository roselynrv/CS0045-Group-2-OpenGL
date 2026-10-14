
#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

using namespace std;

// Object 1: Purple Gradient Square - LEFT
void drawSquare()
{
    glBegin(GL_QUADS);

        // Top-left: Light purple
        glColor3f(0.85f, 0.55f, 1.0f);
        glVertex2f(-0.90f, 0.35f);

        // Top-right: Lavender
        glColor3f(0.65f, 0.30f, 1.0f);
        glVertex2f(-0.50f, 0.35f);

        // Bottom-right: Deep violet
        glColor3f(0.35f, 0.0f, 0.75f);
        glVertex2f(-0.50f, -0.05f);

        // Bottom-left: Dark purple
        glColor3f(0.45f, 0.10f, 0.65f);
        glVertex2f(-0.90f, -0.05f);

    glEnd();
}

// Object 2: Green Triangle - CENTER
void drawStarterTriangle()
{
    glBegin(GL_TRIANGLES);

        glColor3f(0.0f, 0.8f, 0.2f);
        glVertex2f(0.0f, 0.45f);

        glColor3f(0.0f, 0.8f, 0.2f);
        glVertex2f(-0.30f, -0.15f);

        glColor3f(0.0f, 0.8f, 0.2f);
        glVertex2f(0.30f, -0.15f);

    glEnd();
}

// Object 3: Multicolor Gradient Diamond - RIGHT
void drawDiamond()
{
    glBegin(GL_POLYGON);

        // Top: Yellow
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.70f, 0.40f);

        // Right: Red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.90f, 0.15f);

        // Bottom: Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.70f, -0.10f);

        // Left: Purple
        glColor3f(0.7f, 0.0f, 1.0f);
        glVertex2f(0.50f, 0.15f);

    glEnd();
}

// Display Function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSquare();
    drawStarterTriangle();
    drawDiamond();

    glFlush();
}

// Main Function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Requirement 1");

    // Set background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register display callback
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
