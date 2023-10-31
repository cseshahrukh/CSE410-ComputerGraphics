#include <GL/glut.h>

// Function to initialize OpenGL
void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set the clear color to black
}

// Function to handle window resizing
void reshape(int width, int height)
{
    glViewport(0, 0, width, height); // Set the viewport to cover the entire window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0); // Set the perspective projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Function to draw the octahedron
void drawOctahedron()
{
    glBegin(GL_TRIANGLES);

    // Front face
    glColor3f(1.0, 0.0, 0.0); // Set the color to red
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, 1.0);

    // Right face
    glColor3f(0.0, 1.0, 0.0); // Set the color to green
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 0.0, -1.0);

    // Back face
    glColor3f(0.0, 0.0, 1.0); // Set the color to blue
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(1.0, 0.0, -1.0);
    glVertex3f(-1.0, 0.0, -1.0);

    // Left face
    glColor3f(1.0, 1.0, 0.0); // Set the color to yellow
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, 0.0, -1.0);
    glVertex3f(-1.0, 0.0, 1.0);

    // Bottom face
    glColor3f(0.0, 1.0, 1.0); // Set the color to cyan
    glVertex3f(-1.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, -1.0);
    glVertex3f(1.0, 0.0, 1.0);

    glColor3f(1.0, 0.0, 1.0); // Set the color to magenta
    glVertex3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, 0.0, 1.0);
    glVertex3f(-1.0, 0.0, -1.0);

    glEnd();
}

// Function to display the scene
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Set the camera position

    drawOctahedron(); // Draw the octahedron

    glutSwapBuffers(); // Swap the front and back buffers
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Octahedron");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    glutMainLoop();

    return 0;
}