#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

using namespace std;

float squareOffsetX = 0.0f;
bool alternateColors = false;
float diamondAngle = 0.0f;
bool rotationPaused = false;

void display();
void keyboard(unsigned char key, int x, int y);
void updateRotation(int value);

// Dark navy-to-plum background that complements the bright objects
void drawGradientBackground()
{
    glBegin(GL_QUADS);
        glColor3f(0.03f, 0.07f, 0.18f);
        glVertex2f(-1.0f, -1.0f);

        glColor3f(0.05f, 0.13f, 0.25f);
        glVertex2f(1.0f, -1.0f);

        glColor3f(0.24f, 0.08f, 0.30f);
        glVertex2f(1.0f, 1.0f);

        glColor3f(0.10f, 0.06f, 0.25f);
        glVertex2f(-1.0f, 1.0f);
    glEnd();
}

// Object 1: Purple gradient square - LEFT
void drawSquare()
{
    glPushMatrix();
    glTranslatef(squareOffsetX, 0.0f, 0.0f);

    glBegin(GL_QUADS);
        glColor3f(0.85f, 0.55f, 1.0f);
        glVertex2f(-0.90f, 0.35f);

        glColor3f(0.65f, 0.30f, 1.0f);
        glVertex2f(-0.50f, 0.35f);

        glColor3f(0.35f, 0.00f, 0.75f);
        glVertex2f(-0.50f, -0.05f);

        glColor3f(0.45f, 0.10f, 0.65f);
        glVertex2f(-0.90f, -0.05f);
    glEnd();

    glPopMatrix();
}

// Object 2: Triangle rendered using a vertex array - CENTER
void drawStarterTriangle()
{
    GLfloat vertices[] = {
         0.00f,  0.45f,
        -0.30f, -0.15f,
         0.30f, -0.15f
    };

    if (alternateColors)
        glColor3f(0.00f, 0.75f, 1.00f);
    else
        glColor3f(0.00f, 0.80f, 0.20f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// Object 3: Multicolor gradient diamond - RIGHT
void drawDiamond()
{
    glPushMatrix();
    glTranslatef(0.70f, 0.15f, 0.0f);
    glRotatef(diamondAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.70f, -0.15f, 0.0f);

    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.70f, 0.40f);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.90f, 0.15f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.70f, -0.10f);

        glColor3f(0.7f, 0.0f, 1.0f);
        glVertex2f(0.50f, 0.15f);
    glEnd();

    glPopMatrix();
}

void drawInstructions()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.90f, -0.75f);

    const unsigned char* message =
        reinterpret_cast<const unsigned char*>(
            "A/D: Move square   C: Change color   SPACE: Pause rotation");
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, message);

    glRasterPos2f(-0.90f, -0.85f);
    const unsigned char* secondLine =
        reinterpret_cast<const unsigned char*>("R: Reset   ESC: Exit");
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, secondLine);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawGradientBackground();
    drawSquare();
    drawStarterTriangle();
    drawDiamond();
    drawInstructions();

    glutSwapBuffers();
}

void updateRotation(int value)
{
    if (!rotationPaused)
    {
        diamondAngle += 1.5f;
        if (diamondAngle >= 360.0f)
            diamondAngle -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateRotation, 0);
    (void)value;
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        if (squareOffsetX > -0.08f)
            squareOffsetX -= 0.05f;
    }
    else if (key == 'd' || key == 'D')
    {
        if (squareOffsetX < 0.90f)
            squareOffsetX += 0.05f;
    }
    else if (key == 'c' || key == 'C')
    {
        alternateColors = !alternateColors;
    }
    else if (key == 'r' || key == 'R')
    {
        squareOffsetX = 0.0f;
        alternateColors = false;
        diamondAngle = 0.0f;
        rotationPaused = false;
    }
    else if (key == ' ')
    {
        rotationPaused = !rotationPaused;
    }
    else if (key == 27)
    {
        glutLeaveMainLoop();
    }

    (void)x;
    (void)y;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CS0045 - Technical Assessment 1");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, updateRotation, 0);
    glutMainLoop();

    return 0;
}
