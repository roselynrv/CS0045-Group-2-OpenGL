void drawInstructions()
{
    glRasterPos2f(-0.90f, -0.85f);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)"A/D: Move square   C: Change triangle color   R: Reset   ESC: Exit");
}