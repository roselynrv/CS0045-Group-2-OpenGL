#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <cmath>
#include <cstdlib>
#include <string>

int sceneIndex = 0;
int expressionIndex = 0;
int outfitIndex = 0;
int selectedArea = -1;
int hoveredArea = -1;
int habitatProgress[] = {0, 0, 0};
int waterFound = 0;
int foodFound = 0;
int factsLearned = 0;
int informationTicks = 0;
bool completed[3][3] = {};

float robotX = -0.65f;
float robotY = -0.61f;
float jumpOffset = 0.0f;
float jumpVelocity = 0.0f;
float cloudX = -1.25f;
float animationTime = 0.0f;
int idleTicks = 0;

bool jumping = false;
bool sleeping = false;

int nearbyArea();
void interactWithArea(int area);
void drawInteractionPrompt();

void rectangle(float left, float bottom, float right, float top)
{
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();
}

void triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void circle(float centerX, float centerY, float radius)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 28; ++i) {
        float angle = 2.0f * 3.1415926f * i / 28.0f;
        glVertex2f(centerX + radius * std::cos(angle),
                   centerY + radius * std::sin(angle));
    }
    glEnd();
}

void text(float x, float y, const std::string& value, void* font)
{
    glRasterPos2f(x, y);
    glutBitmapString(font,
        reinterpret_cast<const unsigned char*>(value.c_str()));
}

void drawGround(float red, float green, float blue)
{
    GLfloat vertices[] = {
        -1.0f, -0.62f,
         1.0f, -0.62f,
         1.0f, -1.0f,
        -1.0f, -1.0f
    };

    glColor3f(red, green, blue);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCloud(float x, float y)
{
    glColor3f(0.94f, 0.97f, 1.0f);
    rectangle(x, y, x + 0.28f, y + 0.07f);
    circle(x + 0.08f, y + 0.07f, 0.08f);
    circle(x + 0.17f, y + 0.10f, 0.10f);
    circle(x + 0.25f, y + 0.06f, 0.07f);
}

void drawTree(float x, float y, float size)
{
    glColor3f(0.36f, 0.20f, 0.08f);
    rectangle(x - 0.025f * size, y,
              x + 0.025f * size, y + 0.25f * size);
    glColor3f(0.05f, 0.45f, 0.20f);
    triangle(x - 0.15f * size, y + 0.18f * size,
             x + 0.15f * size, y + 0.18f * size,
             x, y + 0.46f * size);
    glColor3f(0.10f, 0.58f, 0.24f);
    triangle(x - 0.12f * size, y + 0.30f * size,
             x + 0.12f * size, y + 0.30f * size,
             x, y + 0.56f * size);
}

void drawBird(float x, float y)
{
    float flap = 0.025f * std::sin(animationTime * 4.0f);
    glColor3f(0.10f, 0.15f, 0.20f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.06f, y + flap);
    glVertex2f(x, y - 0.015f);
    glVertex2f(x + 0.06f, y - flap);
    glEnd();
}

void drawNightCloud(float x, float y, float size)
{
    glColor3f(0.18f, 0.23f, 0.34f);
    circle(x + 0.04f * size, y + 0.02f * size, 0.075f * size);
    circle(x + 0.13f * size, y + 0.07f * size, 0.105f * size);
    circle(x + 0.24f * size, y + 0.04f * size, 0.085f * size);
    circle(x + 0.32f * size, y + 0.01f * size, 0.065f * size);
    rectangle(x + 0.02f * size, y - 0.035f * size,
              x + 0.33f * size, y + 0.035f * size);
    glColor3f(0.28f, 0.34f, 0.47f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x + 0.04f * size, y + 0.025f * size);
    glVertex2f(x + 0.12f * size, y + 0.105f * size);
    glVertex2f(x + 0.23f * size, y + 0.075f * size);
    glVertex2f(x + 0.31f * size, y + 0.025f * size);
    glEnd();
}

void drawWetlandBird(float x, float y, float size, float phase)
{
    float flap = 0.045f * size * std::sin(animationTime * 5.0f + phase);
    glColor3f(0.04f, 0.06f, 0.11f);
    circle(x, y, 0.025f * size);
    triangle(x - 0.01f * size, y,
             x - 0.13f * size, y + 0.035f * size + flap,
             x - 0.055f * size, y - 0.015f * size);
    triangle(x + 0.01f * size, y,
             x + 0.13f * size, y + 0.035f * size - flap,
             x + 0.055f * size, y - 0.015f * size);
    triangle(x - 0.015f * size, y - 0.01f * size,
             x, y - 0.075f * size,
             x + 0.015f * size, y - 0.01f * size);
    glColor3f(0.78f, 0.58f, 0.20f);
    triangle(x + 0.022f * size, y,
             x + 0.055f * size, y + 0.008f * size,
             x + 0.025f * size, y - 0.008f * size);
}

void drawPerchedBird(float x, float y, float size)
{
    float breathe = 0.004f * std::sin(animationTime * 2.0f + x * 8.0f);
    glColor3f(0.04f, 0.11f, 0.14f);
    circle(x, y + breathe, 0.045f * size);
    circle(x + 0.032f * size, y + 0.045f * size + breathe,
           0.028f * size);
    glColor3f(0.10f, 0.28f, 0.30f);
    triangle(x - 0.035f * size, y + 0.01f * size + breathe,
             x + 0.02f * size, y + 0.02f * size + breathe,
             x - 0.005f * size, y - 0.055f * size + breathe);
    glColor3f(0.82f, 0.58f, 0.14f);
    triangle(x + 0.055f * size, y + 0.05f * size + breathe,
             x + 0.09f * size, y + 0.04f * size + breathe,
             x + 0.055f * size, y + 0.032f * size + breathe);
    glColor3f(0.82f, 0.85f, 0.72f);
    circle(x + 0.04f * size, y + 0.052f * size + breathe,
           0.006f * size);
    glColor3f(0.32f, 0.20f, 0.08f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.01f * size, y - 0.035f * size);
    glVertex2f(x - 0.01f * size, y - 0.07f * size);
    glVertex2f(x + 0.02f * size, y - 0.035f * size);
    glVertex2f(x + 0.02f * size, y - 0.07f * size);
    glEnd();
}

void drawHotspot(float left, float bottom, float right, float top,
                 int index, const std::string& label)
{
    if (nearbyArea() != index)
        return;

    bool highlighted = true;

    if (selectedArea == index)
        glColor3f(1.0f, 0.78f, 0.10f);
    else
        glColor3f(1.0f, 1.0f, 1.0f);

    glLineWidth(highlighted ? 3.0f : 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();

    float labelWidth = 0.022f * static_cast<float>(label.size()) + 0.05f;
    if (left + labelWidth > right)
        labelWidth = right - left;
    glColor3f(0.02f, 0.16f, 0.11f);
    rectangle(left, top - 0.07f, left + labelWidth, top);
    glColor3f(completed[sceneIndex][index] ? 0.35f : 0.98f,
              completed[sceneIndex][index] ? 1.00f : 0.82f,
              completed[sceneIndex][index] ? 0.55f : 0.18f);
    text(left + 0.015f, top - 0.048f,
         completed[sceneIndex][index] ? "DONE: " + label : label,
         GLUT_BITMAP_HELVETICA_12);
}

void drawForest()
{
    glColor3f(0.025f, 0.065f, 0.065f);
    rectangle(-1.0f, -1.0f, 1.0f, 1.0f);
    glColor3f(0.045f, 0.11f, 0.10f);
    rectangle(-1.0f, 0.08f, 1.0f, 0.72f);
    glColor3f(0.075f, 0.16f, 0.13f);
    rectangle(-1.0f, -0.34f, 1.0f, 0.08f);

    glColor3f(0.22f, 0.34f, 0.23f);
    circle(-0.70f, 0.59f, 0.10f);
    glColor3f(0.025f, 0.065f, 0.065f);
    circle(-0.66f, 0.62f, 0.095f);

    for (int i = 0; i < 10; ++i) {
        float x = -0.98f + i * 0.22f;
        float width = 0.035f + (i % 3) * 0.012f;
        glColor3f(0.07f, 0.095f, 0.075f);
        rectangle(x, -0.62f, x + width, 0.55f + (i % 2) * 0.12f);
        glColor3f(0.035f, 0.13f, 0.085f);
        circle(x - 0.05f, 0.50f + (i % 2) * 0.10f, 0.18f);
        circle(x + 0.09f, 0.54f + (i % 3) * 0.045f, 0.17f);
    }

    glColor3f(0.12f, 0.17f, 0.11f);
    rectangle(-1.0f, -1.0f, 1.0f, -0.62f);
    glColor3f(0.17f, 0.22f, 0.14f);
    rectangle(-1.0f, -0.66f, 1.0f, -0.62f);

    for (int i = 0; i < 6; ++i) {
        float x = -0.88f + i * 0.36f;
        glColor3f(0.15f, 0.09f, 0.045f);
        rectangle(x, -0.62f, x + 0.065f, 0.25f + (i % 2) * 0.12f);
        glColor3f(0.025f, 0.24f, 0.12f);
        circle(x - 0.06f, 0.24f + (i % 2) * 0.10f, 0.17f);
        circle(x + 0.08f, 0.29f + (i % 2) * 0.08f, 0.18f);
        circle(x + 0.01f, 0.40f + (i % 2) * 0.06f, 0.15f);
    }

    glColor3f(0.08f, 0.30f, 0.38f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.60f);
    glVertex2f(-0.56f, -0.60f);
    glVertex2f(-0.24f, -1.0f);
    glVertex2f(-0.88f, -1.0f);
    glEnd();
    glColor3f(0.15f, 0.34f, 0.30f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-1.0f, -0.60f);
    glVertex2f(-0.56f, -0.60f);
    glVertex2f(-0.24f, -1.0f);
    glEnd();

    glLineWidth(1.5f);
    for (int row = 0; row < 5; ++row) {
        glColor3f(0.22f + row * 0.025f, 0.50f + row * 0.018f,
                  0.56f + row * 0.02f);
        glBegin(GL_LINE_STRIP);
        for (int point = 0; point <= 11; ++point) {
            float streamX = -0.96f + point * 0.055f;
            float streamY = -0.67f - row * 0.065f - point * 0.017f;
            streamY += 0.008f * std::sin(animationTime * 2.4f + point);
            glVertex2f(streamX, streamY);
        }
        glEnd();
    }

    if (completed[0][0]) {
        glColor3f(0.42f, 0.44f, 0.41f);
        circle(-0.86f, -0.68f, 0.055f);
        circle(-0.70f, -0.76f, 0.045f);
        circle(-0.55f, -0.83f, 0.060f);
        circle(-0.40f, -0.91f, 0.048f);
        glColor3f(0.62f, 0.64f, 0.58f);
        circle(-0.88f, -0.66f, 0.020f);
        circle(-0.57f, -0.80f, 0.020f);
        circle(-0.42f, -0.89f, 0.016f);
    }

    glColor3f(0.20f, 0.12f, 0.055f);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(0.30f, 0.18f);
    glVertex2f(0.70f, 0.18f);
    glVertex2f(0.58f, 0.32f);
    glVertex2f(0.92f, 0.32f);
    glEnd();
    drawPerchedBird(0.47f, 0.24f, 0.78f);
    drawPerchedBird(0.78f, 0.38f, 0.66f);

    if (completed[0][1]) {
        for (int i = 0; i < 6; ++i) {
            float x = 0.30f + i * 0.13f;
            glColor3f(0.18f, 0.10f, 0.045f);
            rectangle(x, -0.62f, x + 0.035f, -0.02f + (i % 2) * 0.10f);
            glColor3f(0.02f, 0.34f, 0.15f);
            circle(x - 0.035f, 0.00f + (i % 2) * 0.09f, 0.105f);
            circle(x + 0.055f, 0.04f + (i % 2) * 0.08f, 0.11f);
        }
    }

    glColor3f(0.28f, 0.18f, 0.08f);
    circle(-0.02f, -0.55f, 0.050f);
    circle(0.13f, -0.54f, 0.040f);

    if (completed[0][2]) {
        glColor3f(0.25f, 0.13f, 0.055f);
        rectangle(-0.16f, -0.55f, 0.20f, -0.45f);
        glColor3f(0.48f, 0.28f, 0.10f);
        circle(-0.16f, -0.50f, 0.052f);
        circle(0.20f, -0.50f, 0.052f);
        glColor3f(0.76f, 0.68f, 0.46f);
        rectangle(-0.10f, -0.45f, -0.075f, -0.36f);
        rectangle(0.02f, -0.45f, 0.045f, -0.34f);
        rectangle(0.13f, -0.45f, 0.155f, -0.38f);
        glColor3f(0.72f, 0.18f, 0.13f);
        circle(-0.087f, -0.35f, 0.045f);
        glColor3f(0.86f, 0.70f, 0.24f);
        circle(0.032f, -0.33f, 0.042f);
        glColor3f(0.66f, 0.28f, 0.52f);
        circle(0.142f, -0.37f, 0.036f);
        glColor3f(0.08f, 0.42f, 0.17f);
        for (int i = 0; i < 4; ++i) {
            float fernX = -0.14f + i * 0.11f;
            triangle(fernX, -0.57f, fernX + 0.045f, -0.44f,
                     fernX + 0.075f, -0.57f);
        }
    }

    drawHotspot(-0.98f, -0.95f, -0.35f, -0.18f, 0, "RIVER");
    drawHotspot(0.32f, -0.58f, 0.98f, 0.70f, 1, "TREE CANOPY");
    drawHotspot(-0.18f, -0.61f, 0.23f, -0.32f, 2, "FOREST FLOOR");
}

void drawWetland()
{
    glColor3f(0.035f, 0.055f, 0.13f);
    rectangle(-1.0f, -1.0f, 1.0f, 1.0f);
    glColor3f(0.055f, 0.085f, 0.18f);
    rectangle(-1.0f, 0.25f, 1.0f, 0.72f);
    glColor3f(0.085f, 0.13f, 0.24f);
    rectangle(-1.0f, -0.20f, 1.0f, 0.25f);
    glColor3f(0.12f, 0.18f, 0.29f);
    rectangle(-1.0f, -0.62f, 1.0f, -0.20f);

    glColor3f(0.92f, 0.91f, 0.72f);
    circle(-0.72f, 0.62f, 0.105f);
    glColor3f(0.035f, 0.055f, 0.13f);
    circle(-0.675f, 0.655f, 0.095f);

    glColor3f(0.78f, 0.84f, 0.95f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glVertex2f(-0.94f, 0.55f);
    glVertex2f(-0.48f, 0.74f);
    glVertex2f(-0.23f, 0.48f);
    glVertex2f(0.12f, 0.69f);
    glVertex2f(0.48f, 0.52f);
    glVertex2f(0.83f, 0.76f);
    glEnd();

    drawNightCloud(cloudX + 0.32f, 0.58f, 0.95f);
    drawNightCloud(cloudX + 1.35f, 0.40f, 0.72f);

    glColor3f(0.12f, 0.14f, 0.18f);
    triangle(-1.0f, -0.62f, -0.42f, -0.62f, -0.74f, 0.22f);
    triangle(-0.62f, -0.62f, 0.12f, -0.62f, -0.25f, 0.10f);
    triangle(-0.10f, -0.62f, 0.66f, -0.62f, 0.34f, 0.18f);
    triangle(0.38f, -0.62f, 1.0f, -0.62f, 0.74f, 0.27f);
    glColor3f(0.25f, 0.27f, 0.31f);
    triangle(-0.86f, -0.20f, -0.74f, 0.22f, -0.62f, -0.20f);
    triangle(-0.38f, -0.25f, -0.25f, 0.10f, -0.12f, -0.25f);
    triangle(0.20f, -0.24f, 0.34f, 0.18f, 0.47f, -0.24f);
    triangle(0.62f, -0.18f, 0.74f, 0.27f, 0.86f, -0.18f);
    glColor3f(0.43f, 0.45f, 0.48f);
    triangle(-0.79f, 0.04f, -0.74f, 0.22f, -0.68f, 0.03f);
    triangle(0.68f, 0.06f, 0.74f, 0.27f, 0.80f, 0.05f);

    glColor3f(0.12f, 0.25f, 0.20f);
    rectangle(0.38f, -1.0f, 1.0f, -0.62f);
    glColor3f(0.16f, 0.33f, 0.25f);
    rectangle(0.38f, -0.66f, 1.0f, -0.62f);

    glColor3f(0.035f, 0.19f, 0.29f);
    rectangle(-1.0f, -1.0f, 0.38f, -0.62f);
    glColor3f(0.07f, 0.28f, 0.40f);
    rectangle(-1.0f, -0.68f, 0.38f, -0.62f);

    glLineWidth(1.5f);
    for (int row = 0; row < 7; ++row) {
        float waveY = -0.66f - row * 0.05f;
        float phase = animationTime * (1.5f + row * 0.08f);
        if (row % 2 == 0)
            glColor3f(0.18f, 0.46f, 0.58f);
        else
            glColor3f(0.09f, 0.34f, 0.48f);
        glBegin(GL_LINE_STRIP);
        for (int point = 0; point <= 28; ++point) {
            float waveX = -1.0f + point * 0.05f;
            float ripple = 0.009f * std::sin(point * 0.75f + phase + row);
            glVertex2f(waveX, waveY + ripple);
        }
        glEnd();
    }

    glColor3f(0.54f, 0.60f, 0.58f);
    glLineWidth(2.0f);
    for (int i = 0; i < 5; ++i) {
        float reflectionY = -0.69f - i * 0.055f;
        float halfWidth = 0.12f - i * 0.014f;
        glBegin(GL_LINES);
        glVertex2f(-0.72f - halfWidth, reflectionY);
        glVertex2f(-0.72f + halfWidth, reflectionY);
        glEnd();
    }

    if (completed[1][0]) {
        glColor3f(0.18f, 0.48f, 0.28f);
        circle(-0.80f, -0.75f, 0.055f);
        circle(-0.47f, -0.84f, 0.05f);
        circle(-0.14f, -0.72f, 0.048f);
        glColor3f(0.82f, 0.42f, 0.64f);
        circle(-0.47f, -0.81f, 0.022f);
    }

    for (int i = 0; i < 5; ++i) {
        float x = 0.45f + i * 0.12f;
        glColor3f(0.22f, 0.14f, 0.08f);
        rectangle(x, -0.62f, x + 0.025f, -0.25f);
        glColor3f(0.025f, 0.29f, 0.18f);
        circle(x - 0.025f, -0.20f, 0.095f);
        circle(x + 0.065f, -0.17f, 0.085f);
        glColor3f(0.28f, 0.18f, 0.10f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(x, -0.57f);
        glVertex2f(x - 0.07f, -0.69f);
        glVertex2f(x + 0.02f, -0.57f);
        glVertex2f(x + 0.09f, -0.69f);
        glEnd();
    }

    if (completed[1][1]) {
        glColor3f(0.08f, 0.42f, 0.22f);
        circle(0.43f, -0.31f, 0.07f);
        circle(0.92f, -0.29f, 0.075f);
    }

    drawWetlandBird(-0.10f + 0.32f * std::sin(animationTime * 0.55f),
                    0.43f, 0.78f, 0.0f);
    drawWetlandBird(0.20f + 0.28f * std::sin(animationTime * 0.48f + 1.2f),
                    0.56f, 0.62f, 1.4f);
    if (completed[1][2]) {
        drawWetlandBird(0.45f + 0.18f * std::sin(animationTime * 0.65f),
                        0.37f, 0.55f, 2.1f);
        drawWetlandBird(-0.42f + 0.20f * std::sin(animationTime * 0.58f),
                        0.62f, 0.50f, 3.0f);
    }

    drawHotspot(-0.98f, -0.96f, 0.34f, -0.38f, 0, "WETLAND WATER");
    drawHotspot(0.38f, -0.72f, 0.98f, 0.18f, 1, "MANGROVES");
    drawHotspot(-0.30f, 0.28f, 0.48f, 0.72f, 2, "MIGRATORY BIRDS");
}

void drawEcoCampus()
{
    glColor3f(0.20f, 0.12f, 0.32f);
    rectangle(-1.0f, -1.0f, 1.0f, 1.0f);
    glColor3f(0.42f, 0.20f, 0.38f);
    rectangle(-1.0f, 0.30f, 1.0f, 0.72f);
    glColor3f(0.76f, 0.31f, 0.32f);
    rectangle(-1.0f, -0.05f, 1.0f, 0.30f);
    glColor3f(0.96f, 0.52f, 0.27f);
    rectangle(-1.0f, -0.32f, 1.0f, -0.05f);

    glColor3f(1.0f, 0.70f, 0.25f);
    circle(0.68f, -0.02f, 0.15f);
    glColor3f(0.96f, 0.55f, 0.30f);
    rectangle(-1.0f, -0.20f, 1.0f, -0.14f);

    glColor3f(0.34f, 0.20f, 0.38f);
    circle(cloudX + 0.20f, 0.48f, 0.08f);
    circle(cloudX + 0.31f, 0.53f, 0.11f);
    circle(cloudX + 0.44f, 0.47f, 0.085f);
    rectangle(cloudX + 0.18f, 0.40f, cloudX + 0.46f, 0.48f);
    glColor3f(0.52f, 0.28f, 0.43f);
    circle(cloudX + 1.20f, 0.32f, 0.065f);
    circle(cloudX + 1.29f, 0.37f, 0.09f);
    circle(cloudX + 1.39f, 0.32f, 0.07f);
    rectangle(cloudX + 1.18f, 0.26f, cloudX + 1.42f, 0.33f);

    glColor3f(0.20f, 0.16f, 0.27f);
    rectangle(-1.0f, -0.58f, 1.0f, -0.32f);
    for (int i = 0; i < 9; ++i) {
        float x = -1.0f + i * 0.24f;
        float height = -0.21f + (i % 3) * 0.055f;
        glColor3f(0.25f, 0.19f, 0.31f);
        rectangle(x, -0.32f, x + 0.16f, height);
        glColor3f(0.13f, 0.11f, 0.18f);
        rectangle(x + 0.035f, height, x + 0.06f, height + 0.08f);
    }

    glColor3f(0.18f, 0.42f, 0.25f);
    rectangle(-1.0f, -1.0f, 1.0f, -0.58f);
    glColor3f(0.27f, 0.53f, 0.31f);
    rectangle(-1.0f, -0.62f, 1.0f, -0.58f);
    glColor3f(0.55f, 0.45f, 0.38f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.30f, -0.62f);
    glVertex2f(0.18f, -0.62f);
    glVertex2f(0.45f, -1.0f);
    glVertex2f(-0.55f, -1.0f);
    glEnd();
    glColor3f(0.74f, 0.63f, 0.52f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.27f, -0.66f);
    glVertex2f(0.14f, -0.66f);
    glVertex2f(-0.38f, -0.84f);
    glVertex2f(0.31f, -0.84f);
    glEnd();

    glColor3f(0.78f, 0.69f, 0.56f);
    rectangle(-0.96f, -0.58f, -0.30f, 0.29f);
    glColor3f(0.62f, 0.36f, 0.28f);
    rectangle(-0.98f, 0.29f, -0.28f, 0.36f);
    glColor3f(0.27f, 0.18f, 0.30f);
    rectangle(-0.92f, 0.39f, -0.34f, 0.53f);

    for (int i = 0; i < 4; ++i) {
        float panelX = -0.89f + i * 0.14f;
        glColor3f(0.10f, 0.22f, 0.40f);
        rectangle(panelX, 0.415f, panelX + 0.11f, 0.51f);
        glColor3f(0.34f, 0.55f, 0.72f);
        glBegin(GL_LINES);
        glVertex2f(panelX + 0.055f, 0.42f);
        glVertex2f(panelX + 0.055f, 0.505f);
        glVertex2f(panelX + 0.005f, 0.465f);
        glVertex2f(panelX + 0.105f, 0.465f);
        glEnd();
    }

    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            float windowX = -0.86f + col * 0.18f;
            float windowY = 0.10f - row * 0.24f;
            if (completed[2][0] || completed[2][2])
                glColor3f(1.0f, 0.72f, 0.28f);
            else
                glColor3f(0.20f, 0.29f, 0.39f);
            rectangle(windowX, windowY, windowX + 0.12f, windowY + 0.14f);
            glColor3f(0.84f, 0.74f, 0.58f);
            glLineWidth(1.0f);
            glBegin(GL_LINES);
            glVertex2f(windowX + 0.06f, windowY);
            glVertex2f(windowX + 0.06f, windowY + 0.14f);
            glEnd();
        }
    }
    glColor3f(0.24f, 0.15f, 0.14f);
    rectangle(-0.68f, -0.58f, -0.55f, -0.29f);
    glColor3f(0.94f, 0.72f, 0.30f);
    circle(-0.58f, -0.43f, 0.010f);

    if (completed[2][0]) {
        glColor3f(1.0f, 0.84f, 0.30f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(-0.90f, 0.58f);
        glVertex2f(-0.84f, 0.66f);
        glVertex2f(-0.62f, 0.57f);
        glVertex2f(-0.62f, 0.69f);
        glVertex2f(-0.35f, 0.58f);
        glVertex2f(-0.41f, 0.66f);
        glEnd();
    }

    glColor3f(0.22f, 0.16f, 0.18f);
    rectangle(-0.20f, -0.34f, 0.29f, -0.27f);
    glColor3f(0.40f, 0.29f, 0.24f);
    rectangle(-0.16f, -0.58f, -0.03f, -0.34f);
    rectangle(-0.01f, -0.58f, 0.12f, -0.34f);
    rectangle(0.14f, -0.58f, 0.27f, -0.34f);
    glColor3f(0.13f, 0.58f, 0.31f);
    rectangle(-0.145f, -0.56f, -0.045f, -0.39f);
    glColor3f(0.96f, 0.72f, 0.13f);
    rectangle(0.005f, -0.56f, 0.105f, -0.39f);
    glColor3f(0.18f, 0.48f, 0.78f);
    rectangle(0.155f, -0.56f, 0.255f, -0.39f);
    glColor3f(0.92f, 0.93f, 0.86f);
    text(-0.125f, -0.47f, "G", GLUT_BITMAP_HELVETICA_12);
    text(0.025f, -0.47f, "P", GLUT_BITMAP_HELVETICA_12);
    text(0.175f, -0.47f, "R", GLUT_BITMAP_HELVETICA_12);

    if (completed[2][2]) {
        glColor3f(0.94f, 0.94f, 0.86f);
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-0.04f, -0.24f);
        glVertex2f(0.04f, -0.14f);
        glVertex2f(0.12f, -0.24f);
        glEnd();
        glColor3f(0.42f, 0.75f, 0.44f);
        circle(-0.08f, -0.63f, 0.025f);
        circle(0.06f, -0.64f, 0.022f);
        circle(0.21f, -0.63f, 0.024f);
    }

    glColor3f(0.19f, 0.37f, 0.18f);
    rectangle(0.31f, -0.59f, 0.98f, -0.48f);
    for (int bed = 0; bed < 3; ++bed) {
        float left = 0.35f + bed * 0.21f;
        glColor3f(0.35f, 0.18f, 0.08f);
        rectangle(left, -0.51f, left + 0.16f, -0.31f);
        glColor3f(0.28f, 0.12f, 0.055f);
        rectangle(left + 0.01f, -0.48f, left + 0.15f, -0.33f);
        for (int plant = 0; plant < 3; ++plant) {
            float plantX = left + 0.035f + plant * 0.045f;
            float sway = 0.008f * std::sin(animationTime * 2.0f + plant + bed);
            glColor3f(0.05f, 0.50f, 0.20f);
            glBegin(GL_LINES);
            glVertex2f(plantX, -0.38f);
            glVertex2f(plantX + sway, -0.25f);
            glEnd();
            glColor3f(0.92f - bed * 0.15f, 0.35f + bed * 0.18f, 0.28f);
            circle(plantX + sway, -0.23f, 0.025f);
        }
    }

    glColor3f(0.40f, 0.24f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 6; ++i) {
        float fenceX = 0.31f + i * 0.13f;
        glVertex2f(fenceX, -0.61f);
        glVertex2f(fenceX, -0.12f);
    }
    glVertex2f(0.31f, -0.18f);
    glVertex2f(0.98f, -0.18f);
    glEnd();

    if (completed[2][1]) {
        glColor3f(1.0f, 0.82f, 0.18f);
        circle(0.42f, -0.13f, 0.018f);
        circle(0.72f, -0.10f, 0.018f);
        circle(0.91f, -0.15f, 0.018f);
        glColor3f(0.12f, 0.12f, 0.10f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex2f(0.40f, -0.13f); glVertex2f(0.36f, -0.10f);
        glVertex2f(0.44f, -0.13f); glVertex2f(0.48f, -0.10f);
        glVertex2f(0.70f, -0.10f); glVertex2f(0.66f, -0.07f);
        glVertex2f(0.74f, -0.10f); glVertex2f(0.78f, -0.07f);
        glEnd();
    }

    glColor3f(0.12f, 0.20f, 0.15f);
    rectangle(0.52f, -0.75f, 0.76f, -0.70f);
    rectangle(0.55f, -0.85f, 0.58f, -0.75f);
    rectangle(0.70f, -0.85f, 0.73f, -0.75f);

    float lampPositions[] = {-0.25f, 0.32f, 0.92f};
    for (int i = 0; i < 3; ++i) {
        float lampX = lampPositions[i];
        glColor3f(0.10f, 0.12f, 0.15f);
        rectangle(lampX - 0.012f, -0.62f,
                  lampX + 0.012f, -0.08f);
        rectangle(lampX - 0.055f, -0.10f,
                  lampX + 0.055f, -0.07f);

        if (completed[2][2]) {
            float pulse = 0.006f * std::sin(animationTime * 3.0f + i);
            glColor3f(0.95f, 0.51f, 0.16f);
            circle(lampX, -0.12f, 0.060f + pulse);
            glColor3f(1.0f, 0.88f, 0.42f);
            circle(lampX, -0.12f, 0.029f);
        } else {
            glColor3f(0.22f, 0.24f, 0.27f);
            circle(lampX, -0.12f, 0.030f);
        }
    }

    if (completed[2][2]) {
        glColor3f(0.12f, 0.08f, 0.16f);
        rectangle(-0.17f, 0.59f, 0.39f, 0.68f);
        glColor3f(1.0f, 0.84f, 0.30f);
        text(-0.14f, 0.62f, "RECYCLED ENERGY: LIGHTS ON",
             GLUT_BITMAP_HELVETICA_12);
    }

    drawHotspot(-0.98f, -0.58f, -0.28f, 0.56f, 0, "SOLAR BUILDING");
    drawHotspot(0.26f, -0.64f, 0.99f, -0.16f, 1, "COMMUNITY GARDEN");
    drawHotspot(-0.19f, -0.65f, 0.30f, -0.29f, 2, "RECYCLING HUB");
}

const float tamarawScale = 0.28f;

float tamarawX(float base, float offset)
{
    return base + offset * tamarawScale;
}

float tamarawY(float base, float offset)
{
    return base + offset * tamarawScale;
}

void drawRobotTamaraw()
{
    const float x = robotX - 0.031f;
    const float y = robotY + 0.10f * tamarawScale + jumpOffset;
    const bool flyingPose = robotY > -0.59f || jumping;
    const float legSwing = flyingPose || sleeping
        ? 0.0f
        : 0.025f * std::sin(animationTime * 7.0f);

    glColor3f(0.02f, 0.29f, 0.20f);
    if (flyingPose) {
        glBegin(GL_QUADS);
        glVertex2f(tamarawX(x, 0.02f), tamarawY(y, 0.10f));
        glVertex2f(tamarawX(x, 0.09f), tamarawY(y, 0.10f));
        glVertex2f(tamarawX(x, 0.12f), tamarawY(y, -0.08f));
        glVertex2f(tamarawX(x, 0.06f), tamarawY(y, -0.08f));
        glVertex2f(tamarawX(x, 0.13f), tamarawY(y, 0.10f));
        glVertex2f(tamarawX(x, 0.20f), tamarawY(y, 0.10f));
        glVertex2f(tamarawX(x, 0.18f), tamarawY(y, -0.08f));
        glVertex2f(tamarawX(x, 0.12f), tamarawY(y, -0.08f));
        glEnd();
        glColor3f(0.96f, 0.69f, 0.05f);
        triangle(tamarawX(x, 0.05f), tamarawY(y, -0.07f),
                 tamarawX(x, 0.13f), tamarawY(y, -0.07f),
                 tamarawX(x, 0.10f), tamarawY(y, -0.15f));
        triangle(tamarawX(x, 0.11f), tamarawY(y, -0.07f),
                 tamarawX(x, 0.19f), tamarawY(y, -0.07f),
                 tamarawX(x, 0.14f), tamarawY(y, -0.15f));
    } else {
        rectangle(tamarawX(x, 0.00f), tamarawY(y, -0.07f + legSwing),
                  tamarawX(x, 0.08f), tamarawY(y, 0.10f));
        rectangle(tamarawX(x, 0.14f), tamarawY(y, -0.07f - legSwing),
                  tamarawX(x, 0.22f), tamarawY(y, 0.10f));
        glColor3f(0.96f, 0.69f, 0.05f);
        rectangle(tamarawX(x, -0.02f), tamarawY(y, -0.10f + legSwing),
                  tamarawX(x, 0.09f), tamarawY(y, -0.04f + legSwing));
        rectangle(tamarawX(x, 0.13f), tamarawY(y, -0.10f - legSwing),
                  tamarawX(x, 0.24f), tamarawY(y, -0.04f - legSwing));
    }

    glColor3f(0.01f, 0.22f, 0.15f);
    rectangle(tamarawX(x, -0.03f), tamarawY(y, 0.06f),
              tamarawX(x, 0.25f), tamarawY(y, 0.16f));

    if (outfitIndex == 0)
        glColor3f(0.92f, 0.93f, 0.88f);
    else if (outfitIndex == 1)
        glColor3f(0.67f, 0.45f, 0.18f);
    else
        glColor3f(0.16f, 0.66f, 0.42f);
    rectangle(tamarawX(x, -0.06f), tamarawY(y, 0.15f),
              tamarawX(x, 0.28f), tamarawY(y, 0.34f));

    glColor3f(0.78f, 0.80f, 0.76f);
    rectangle(tamarawX(x, 0.09f), tamarawY(y, 0.15f),
              tamarawX(x, 0.12f), tamarawY(y, 0.30f));
    glBegin(GL_TRIANGLES);
    glVertex2f(tamarawX(x, -0.02f), tamarawY(y, 0.34f));
    glVertex2f(tamarawX(x, 0.10f), tamarawY(y, 0.23f));
    glVertex2f(tamarawX(x, 0.04f), tamarawY(y, 0.34f));
    glVertex2f(tamarawX(x, 0.24f), tamarawY(y, 0.34f));
    glVertex2f(tamarawX(x, 0.12f), tamarawY(y, 0.23f));
    glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.34f));
    glEnd();

    if (outfitIndex == 1) {
        glColor3f(0.96f, 0.76f, 0.18f);
        rectangle(tamarawX(x, -0.01f), tamarawY(y, 0.17f),
                  tamarawX(x, 0.03f), tamarawY(y, 0.32f));
        rectangle(tamarawX(x, 0.21f), tamarawY(y, 0.17f),
                  tamarawX(x, 0.25f), tamarawY(y, 0.32f));
    } else if (outfitIndex == 2) {
        glColor3f(0.30f, 0.90f, 0.72f);
        circle(tamarawX(x, 0.11f), tamarawY(y, 0.24f),
               0.04f * tamarawScale);
    }

    glColor3f(0.00f, 0.42f, 0.23f);
    if (flyingPose) {
        glBegin(GL_QUADS);
        glVertex2f(tamarawX(x, -0.05f), tamarawY(y, 0.24f));
        glVertex2f(tamarawX(x, 0.01f), tamarawY(y, 0.30f));
        glVertex2f(tamarawX(x, -0.16f), tamarawY(y, 0.52f));
        glVertex2f(tamarawX(x, -0.22f), tamarawY(y, 0.47f));
        glVertex2f(tamarawX(x, 0.23f), tamarawY(y, 0.30f));
        glVertex2f(tamarawX(x, 0.29f), tamarawY(y, 0.24f));
        glVertex2f(tamarawX(x, 0.46f), tamarawY(y, 0.47f));
        glVertex2f(tamarawX(x, 0.40f), tamarawY(y, 0.52f));
        glEnd();
        glColor3f(0.96f, 0.69f, 0.05f);
        circle(tamarawX(x, -0.20f), tamarawY(y, 0.51f),
               0.05f * tamarawScale);
        circle(tamarawX(x, 0.44f), tamarawY(y, 0.51f),
               0.05f * tamarawScale);
    } else {
        rectangle(tamarawX(x, -0.13f), tamarawY(y, 0.18f),
                  tamarawX(x, -0.05f), tamarawY(y, 0.31f));
        rectangle(tamarawX(x, 0.27f), tamarawY(y, 0.17f),
                  tamarawX(x, 0.34f), tamarawY(y, 0.30f));
        glColor3f(0.96f, 0.69f, 0.05f);
        rectangle(tamarawX(x, -0.15f), tamarawY(y, 0.14f),
                  tamarawX(x, -0.07f), tamarawY(y, 0.21f));
        rectangle(tamarawX(x, 0.29f), tamarawY(y, 0.13f),
                  tamarawX(x, 0.37f), tamarawY(y, 0.20f));
    }

    glColor3f(0.00f, 0.52f, 0.28f);
    rectangle(tamarawX(x, -0.09f), tamarawY(y, 0.31f),
              tamarawX(x, 0.31f), tamarawY(y, 0.55f));
    rectangle(tamarawX(x, -0.15f), tamarawY(y, 0.39f),
              tamarawX(x, -0.07f), tamarawY(y, 0.50f));
    rectangle(tamarawX(x, 0.29f), tamarawY(y, 0.38f),
              tamarawX(x, 0.37f), tamarawY(y, 0.50f));

    glColor3f(1.00f, 0.76f, 0.20f);
    rectangle(tamarawX(x, -0.08f), tamarawY(y, 0.52f),
              tamarawX(x, -0.01f), tamarawY(y, 0.64f));
    rectangle(tamarawX(x, -0.13f), tamarawY(y, 0.60f),
              tamarawX(x, -0.06f), tamarawY(y, 0.70f));
    rectangle(tamarawX(x, -0.16f), tamarawY(y, 0.67f),
              tamarawX(x, -0.10f), tamarawY(y, 0.74f));
    rectangle(tamarawX(x, 0.23f), tamarawY(y, 0.52f),
              tamarawX(x, 0.30f), tamarawY(y, 0.64f));
    rectangle(tamarawX(x, 0.28f), tamarawY(y, 0.60f),
              tamarawX(x, 0.35f), tamarawY(y, 0.70f));
    rectangle(tamarawX(x, 0.32f), tamarawY(y, 0.67f),
              tamarawX(x, 0.38f), tamarawY(y, 0.74f));

    glColor3f(0.96f, 0.82f, 0.48f);
    rectangle(tamarawX(x, 0.00f), tamarawY(y, 0.52f),
              tamarawX(x, 0.23f), tamarawY(y, 0.63f));
    rectangle(tamarawX(x, 0.04f), tamarawY(y, 0.61f),
              tamarawX(x, 0.19f), tamarawY(y, 0.67f));
    glBegin(GL_TRIANGLES);
    glVertex2f(tamarawX(x, 0.02f), tamarawY(y, 0.53f));
    glVertex2f(tamarawX(x, 0.08f), tamarawY(y, 0.45f));
    glVertex2f(tamarawX(x, 0.12f), tamarawY(y, 0.54f));
    glVertex2f(tamarawX(x, 0.12f), tamarawY(y, 0.54f));
    glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.46f));
    glVertex2f(tamarawX(x, 0.22f), tamarawY(y, 0.54f));
    glEnd();

    glColor3f(0.05f, 0.04f, 0.02f);
    if (sleeping) {
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(tamarawX(x, -0.01f), tamarawY(y, 0.44f));
        glVertex2f(tamarawX(x, 0.07f), tamarawY(y, 0.44f));
        glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.44f));
        glVertex2f(tamarawX(x, 0.26f), tamarawY(y, 0.44f));
        glEnd();
    } else if (expressionIndex == 2) {
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(tamarawX(x, -0.01f), tamarawY(y, 0.40f));
        glVertex2f(tamarawX(x, 0.07f), tamarawY(y, 0.49f));
        glVertex2f(tamarawX(x, -0.01f), tamarawY(y, 0.49f));
        glVertex2f(tamarawX(x, 0.07f), tamarawY(y, 0.40f));
        glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.40f));
        glVertex2f(tamarawX(x, 0.26f), tamarawY(y, 0.49f));
        glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.49f));
        glVertex2f(tamarawX(x, 0.26f), tamarawY(y, 0.40f));
        glEnd();
    } else {
        rectangle(tamarawX(x, -0.01f), tamarawY(y, 0.40f),
                  tamarawX(x, expressionIndex == 1 ? 0.08f : 0.07f),
                  tamarawY(y, expressionIndex == 1 ? 0.50f : 0.49f));
        rectangle(tamarawX(x, 0.18f), tamarawY(y, 0.40f),
                  tamarawX(x, 0.26f), tamarawY(y, 0.49f));
        glColor3f(1.0f, 1.0f, 1.0f);
        rectangle(tamarawX(x, 0.01f), tamarawY(y, 0.45f),
                  tamarawX(x, 0.04f), tamarawY(y, 0.48f));
        rectangle(tamarawX(x, 0.20f), tamarawY(y, 0.45f),
                  tamarawX(x, 0.23f), tamarawY(y, 0.48f));
    }

    glColor3f(0.97f, 0.68f, 0.08f);
    rectangle(tamarawX(x, 0.04f), tamarawY(y, 0.31f),
              tamarawX(x, 0.23f), tamarawY(y, 0.40f));
    glColor3f(0.30f, 0.20f, 0.08f);
    rectangle(tamarawX(x, 0.07f), tamarawY(y, 0.36f),
              tamarawX(x, 0.10f), tamarawY(y, 0.38f));
    rectangle(tamarawX(x, 0.17f), tamarawY(y, 0.36f),
              tamarawX(x, 0.20f), tamarawY(y, 0.38f));
    if (expressionIndex == 0) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(tamarawX(x, 0.09f), tamarawY(y, 0.33f));
        glVertex2f(tamarawX(x, 0.135f), tamarawY(y, 0.305f));
        glVertex2f(tamarawX(x, 0.18f), tamarawY(y, 0.33f));
        glEnd();
    } else if (expressionIndex == 1) {
        circle(tamarawX(x, 0.135f), tamarawY(y, 0.325f),
               0.018f * tamarawScale);
    } else {
        rectangle(tamarawX(x, 0.09f), tamarawY(y, 0.305f),
                  tamarawX(x, 0.18f), tamarawY(y, 0.335f));
    }

    glColor3f(0.96f, 0.69f, 0.05f);
    rectangle(tamarawX(x, 0.20f), tamarawY(y, 0.23f),
              tamarawX(x, 0.25f), tamarawY(y, 0.29f));
    glColor3f(0.00f, 0.45f, 0.24f);
    rectangle(tamarawX(x, 0.215f), tamarawY(y, 0.245f),
              tamarawX(x, 0.235f), tamarawY(y, 0.275f));

    if (sleeping) {
        glColor3f(0.92f, 0.95f, 1.0f);
        text(tamarawX(x, 0.36f), tamarawY(y, 0.62f),
             "Z", GLUT_BITMAP_HELVETICA_12);
        text(tamarawX(x, 0.48f), tamarawY(y, 0.73f),
             "z", GLUT_BITMAP_HELVETICA_12);
    }
}

std::string sceneName()
{
    if (sceneIndex == 0)
        return "LIVING FOREST";
    if (sceneIndex == 1)
        return "COASTAL WETLAND";
    return "ECO CAMPUS";
}

std::string expressionName()
{
    if (expressionIndex == 0)
        return "HAPPY";
    if (expressionIndex == 1)
        return "CURIOUS";
    return "EXCITED";
}

std::string outfitName()
{
    if (outfitIndex == 0)
        return "UNIFORM";
    if (outfitIndex == 1)
        return "EXPLORER";
    return "ECO SUIT";
}

void drawInformationPanel()
{
    if (selectedArea < 0 || informationTicks <= 0)
        return;

    glColor3f(0.02f, 0.10f, 0.08f);
    rectangle(-0.97f, -0.96f, 0.97f, -0.73f);
    glColor3f(0.98f, 0.80f, 0.18f);

    if (sceneIndex == 0 && selectedArea == 0) {
        text(-0.92f, -0.80f, "RIVER ECOSYSTEM", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Fresh water supports fish, insects, plants, and animals.", GLUT_BITMAP_HELVETICA_12);
    } else if (sceneIndex == 0 && selectedArea == 1) {
        text(-0.92f, -0.80f, "TREE CANOPY", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Leaves capture sunlight and provide shelter for wildlife.", GLUT_BITMAP_HELVETICA_12);
    } else if (sceneIndex == 0) {
        text(-0.92f, -0.80f, "FOREST FLOOR", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Fungi and decomposers return nutrients to the soil.", GLUT_BITMAP_HELVETICA_12);
    } else if (sceneIndex == 1 && selectedArea == 0) {
        text(-0.92f, -0.80f, "WETLAND WATER", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Wetlands store floodwater and support aquatic life.", GLUT_BITMAP_HELVETICA_12);
    } else if (sceneIndex == 1 && selectedArea == 1) {
        text(-0.92f, -0.80f, "MANGROVES", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Mangrove roots protect shores and shelter young fish.", GLUT_BITMAP_HELVETICA_12);
    } else if (sceneIndex == 1) {
        text(-0.92f, -0.80f, "MIGRATORY BIRDS", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Wetlands provide food and resting places during migration.", GLUT_BITMAP_HELVETICA_12);
    } else if (selectedArea == 0) {
        text(-0.92f, -0.80f, "SOLAR BUILDING", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Solar panels turn sunlight into cleaner electrical energy.", GLUT_BITMAP_HELVETICA_12);
    } else if (selectedArea == 1) {
        text(-0.92f, -0.80f, "COMMUNITY GARDEN", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Shared gardens grow food and create habitat for pollinators.", GLUT_BITMAP_HELVETICA_12);
    } else {
        text(-0.92f, -0.80f, "RECYCLING HUB", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1.0f, 1.0f, 1.0f);
        text(-0.92f, -0.88f, "Sorted materials can be reused instead of becoming waste.", GLUT_BITMAP_HELVETICA_12);
    }

    if (completed[sceneIndex][selectedArea]) {
        glColor3f(0.35f, 1.0f, 0.55f);
        text(-0.92f, -0.94f,
             "ACTIVITY COMPLETE - the habitat visibly improved.",
             GLUT_BITMAP_HELVETICA_12);
    }
}

void drawInterface()
{
    glColor3f(0.02f, 0.13f, 0.10f);
    rectangle(-1.0f, 0.72f, 1.0f, 1.0f);
    glColor3f(0.98f, 0.78f, 0.12f);
    text(-0.95f, 0.92f, "LIVING ENVIRONMENT EXPLORER", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.92f, 0.96f, 0.93f);
    text(-0.95f, 0.84f,
         "WASD Move | SPACE Jump | E Interact | C/V Scene | I/K Face | J/L Clothes",
         GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.98f, 0.78f, 0.12f);
    text(0.40f, 0.92f, sceneName(), GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.75f, 0.92f, 0.82f);
    text(0.40f, 0.84f,
         expressionName() + " | " + outfitName(),
         GLUT_BITMAP_HELVETICA_12);
    glColor3f(0.72f, 0.92f, 0.82f);
    text(-0.95f, 0.76f,
         "RESTORED " + std::to_string(habitatProgress[sceneIndex]) +
         "/3 | WATER " + std::to_string(waterFound) +
         " | FOOD " + std::to_string(foodFound) +
         " | DISCOVERIES " + std::to_string(factsLearned),
         GLUT_BITMAP_HELVETICA_12);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (sceneIndex == 0)
        drawForest();
    else if (sceneIndex == 1)
        drawWetland();
    else
        drawEcoCampus();

    drawRobotTamaraw();
    drawInteractionPrompt();
    drawInformationPanel();
    drawInterface();
    glFlush();
}

int areaAt(float x, float y)
{
    if (sceneIndex == 0) {
        if (x >= -0.98f && x <= -0.35f && y >= -0.95f && y <= -0.18f)
            return 0;
        if (x >= 0.32f && x <= 0.98f && y >= -0.58f && y <= 0.70f)
            return 1;
        if (x >= -0.18f && x <= 0.23f && y >= -0.61f && y <= -0.32f)
            return 2;
    } else if (sceneIndex == 1) {
        if (x >= -0.98f && x <= 0.34f && y >= -0.96f && y <= -0.38f)
            return 0;
        if (x >= 0.38f && x <= 0.98f && y >= -0.72f && y <= 0.18f)
            return 1;
        if (x >= -0.30f && x <= 0.48f && y >= 0.28f && y <= 0.72f)
            return 2;
    } else {
        if (x >= -0.98f && x <= -0.28f && y >= -0.58f && y <= 0.56f)
            return 0;
        if (x >= 0.26f && x <= 0.99f && y >= -0.64f && y <= -0.16f)
            return 1;
        if (x >= -0.19f && x <= 0.30f && y >= -0.65f && y <= -0.29f)
            return 2;
    }
    return -1;
}

void areaBounds(int scene, int area, float& left, float& bottom,
                float& right, float& top)
{
    if (scene == 0) {
        if (area == 0) {
            left = -0.98f; bottom = -0.95f; right = -0.35f; top = -0.18f;
        } else if (area == 1) {
            left = 0.32f; bottom = -0.58f; right = 0.98f; top = 0.70f;
        } else {
            left = -0.18f; bottom = -0.61f; right = 0.23f; top = -0.32f;
        }
    } else if (scene == 1) {
        if (area == 0) {
            left = -0.98f; bottom = -0.96f; right = 0.34f; top = -0.38f;
        } else if (area == 1) {
            left = 0.38f; bottom = -0.72f; right = 0.98f; top = 0.18f;
        } else {
            left = -0.30f; bottom = 0.28f; right = 0.48f; top = 0.72f;
        }
    } else {
        if (area == 0) {
            left = -0.98f; bottom = -0.58f; right = -0.28f; top = 0.56f;
        } else if (area == 1) {
            left = 0.26f; bottom = -0.64f; right = 0.99f; top = -0.16f;
        } else {
            left = -0.19f; bottom = -0.65f; right = 0.30f; top = -0.29f;
        }
    }
}

int nearbyArea()
{
    const float interactionRange = 0.25f;
    const float centerY = robotY + 0.12f;
    float targetX[3];
    float targetY[3];

    if (sceneIndex == 0) {
        targetX[0] = -0.72f; targetY[0] = -0.58f;
        targetX[1] = 0.68f; targetY[1] = -0.42f;
        targetX[2] = 0.05f; targetY[2] = -0.47f;
    } else if (sceneIndex == 1) {
        targetX[0] = -0.68f; targetY[0] = -0.62f;
        targetX[1] = 0.70f; targetY[1] = -0.48f;
        targetX[2] = 0.10f; targetY[2] = 0.45f;
    } else {
        targetX[0] = -0.64f; targetY[0] = -0.46f;
        targetX[1] = 0.66f; targetY[1] = -0.48f;
        targetX[2] = 0.05f; targetY[2] = -0.48f;
    }

    for (int area = 0; area < 3; ++area) {
        float distanceX = robotX - targetX[area];
        float distanceY = centerY - targetY[area];

        if (distanceX * distanceX + distanceY * distanceY <=
            interactionRange * interactionRange)
            return area;
    }
    return -1;
}

std::string areaName(int area)
{
    if (sceneIndex == 0) {
        if (area == 0) return "RIVER";
        if (area == 1) return "TREE CANOPY";
        return "FOREST FLOOR";
    }
    if (sceneIndex == 1) {
        if (area == 0) return "WETLAND WATER";
        if (area == 1) return "MANGROVES";
        return "MIGRATORY BIRDS";
    }
    if (area == 0) return "SOLAR BUILDING";
    if (area == 1) return "COMMUNITY GARDEN";
    return "RECYCLING HUB";
}

void drawInteractionPrompt()
{
    int area = nearbyArea();
    if (area < 0)
        return;

    float promptX = robotX - 0.16f;
    float promptY = robotY + jumpOffset + 0.30f;
    if (promptX < -0.96f)
        promptX = -0.96f;
    if (promptX > 0.50f)
        promptX = 0.50f;
    if (promptY > 0.68f)
        promptY = 0.68f;

    glColor3f(0.02f, 0.10f, 0.08f);
    rectangle(promptX - 0.02f, promptY - 0.045f,
              promptX + 0.45f, promptY + 0.035f);
    glColor3f(1.0f, 0.82f, 0.18f);
    text(promptX, promptY - 0.012f,
         "E - INTERACT: " + areaName(area), GLUT_BITMAP_HELVETICA_12);
}

void interactWithArea(int area)
{
    selectedArea = area;
    informationTicks = area >= 0 ? 313 : 0;
    if (area < 0 || completed[sceneIndex][area])
        return;

    completed[sceneIndex][area] = true;
    habitatProgress[sceneIndex] += 1;

    if (sceneIndex < 2 && area == 0)
        waterFound += 1;
    else if ((sceneIndex < 2 && area == 2) ||
             (sceneIndex == 2 && area == 1))
        foodFound += 1;
    else
        factsLearned += 1;
}

void mousePosition(int pixelX, int pixelY, float& x, float& y)
{
    x = 2.0f * pixelX / 1100.0f - 1.0f;
    y = 1.0f - 2.0f * pixelY / 700.0f;
}

void mouse(int button, int state, int pixelX, int pixelY)
{
    idleTicks = 0;
    sleeping = false;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float x;
        float y;
        mousePosition(pixelX, pixelY, x, y);
        interactWithArea(areaAt(x, y));
        glutPostRedisplay();
    }
}

void passiveMotion(int pixelX, int pixelY)
{
    idleTicks = 0;
    sleeping = false;
    float x;
    float y;
    mousePosition(pixelX, pixelY, x, y);
    hoveredArea = areaAt(x, y);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
    idleTicks = 0;
    sleeping = false;
    if (key == 27)
        std::exit(0);
    else if (key == 'a' || key == 'A')
        robotX -= 0.04f;
    else if (key == 'd' || key == 'D')
        robotX += 0.04f;
    else if (key == 'w' || key == 'W')
        robotY += 0.03f;
    else if (key == 's' || key == 'S')
        robotY -= 0.03f;
    else if (key == 'e' || key == 'E')
        interactWithArea(nearbyArea());
    else if (key == ' ' && !jumping) {
        jumping = true;
        jumpVelocity = 0.045f;
    } else if (key == 'c' || key == 'C') {
        sceneIndex = (sceneIndex + 2) % 3;
        selectedArea = -1;
        hoveredArea = -1;
    } else if (key == 'v' || key == 'V') {
        sceneIndex = (sceneIndex + 1) % 3;
        selectedArea = -1;
        hoveredArea = -1;
    } else if (key == 'i' || key == 'I')
        expressionIndex = (expressionIndex + 2) % 3;
    else if (key == 'k' || key == 'K')
        expressionIndex = (expressionIndex + 1) % 3;
    else if (key == 'j' || key == 'J')
        outfitIndex = (outfitIndex + 2) % 3;
    else if (key == 'l' || key == 'L')
        outfitIndex = (outfitIndex + 1) % 3;
    else if (key == 'r' || key == 'R') {
        robotX = -0.65f;
        robotY = -0.61f;
        jumpOffset = 0.0f;
        jumpVelocity = 0.0f;
        jumping = false;
    }

    if (robotX < -0.88f)
        robotX = -0.88f;
    if (robotX > 0.88f)
        robotX = 0.88f;
    if (robotY < -0.61f)
        robotY = -0.61f;
    if (robotY > 0.14f)
        robotY = 0.14f;

    glutPostRedisplay();
}

void timer(int)
{
    animationTime += 0.04f;
    if (informationTicks > 0)
        informationTicks -= 1;
    if (idleTicks < 100000)
        idleTicks += 1;
    sleeping = idleTicks >= 300 && !jumping;
    cloudX += 0.004f;
    if (cloudX > 1.15f)
        cloudX = -1.35f;

    if (jumping) {
        jumpOffset += jumpVelocity;
        jumpVelocity -= 0.004f;
        if (jumpOffset <= 0.0f) {
            jumpOffset = 0.0f;
            jumpVelocity = 0.0f;
            jumping = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1100, 700);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("Living Environment Explorer");
    glClearColor(0.45f, 0.78f, 0.94f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}
