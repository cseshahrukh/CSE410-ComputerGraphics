#define _USE_MATH_DEFINES

#include <windows.h>  // For MS Windows //MS means Microsoft 
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <cmath>
#include <iostream>

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */

   //Mane first e ekbar call hobe then window jodi drag kori then abar call hobe
   //Minimize kore open korar por abar call hobe 
void display() {
    //std::cout<<"Inside display\n"; 

//alpha: Specifies the alpha component (opacity) of the clear color.
//To set the clear color to, for example, black, 
//you would call glClearColor(0.0f, 0.0f, 0.0f, 1.0f);. 
//This sets the clear color to black with full opacity.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    

    //If you don't use glClear(GL_COLOR_BUFFER_BIT) to clear the color buffer, 
    //the contents of the color buffer from the previous frame will persist into the current frame. 
    //This means that whatever was rendered in the previous frame will remain on the screen.
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    // Draw two points
    // at 'top-left'
    glPointSize(5); // Make next points look bigger
    glBegin(GL_POINTS); // Each vertex is drawn as a single pixel

        //RGB bole nam 3f 
        //floating point bujhate 1.0 er por f use kora hoyeche 
        //f na dile single precision na bujhe double precision bujhbe. oita opengl expect korche na 
        glColor3f(1.0f,0.0f,1.0f); // Magenta
        glVertex2d(-0.6f, 0.8f);

        glColor3f(0.0f,1.0f,1.0f);  // Cyan
        glVertex2d(-0.8f, 0.6f);
    glEnd();

    // Draw two lines
    // at 'top-middle'
    glLineWidth(5); // Make next lines wider
    glBegin(GL_LINES);  // Each set of 2 vertices form a line of single pixel width
        glColor3f(1.0f,1.0f,1.0f);  // White
        glVertex2d(-0.2f, 0.9f);
        glVertex2d( 0.2f, 0.5f);

        glColor3f(0.0f,0.0f,1.0f);  // Blue
        glVertex2d(-0.2f, 0.5f);
        glVertex2d( 0.2f, 0.9f);
    glEnd();

    // Draw border of a square
    // at 'top-right'
    //start of a sequence of vertices that define a closed loop of connected line segments.
    // Sequence ta kintu maintain korte hobe 
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(1.0f,1.0f,0.0f); // Yellow
        glVertex2d( 0.9f, 0.9f);
        glVertex2d( 0.9f, 0.5f);
        glVertex2d( 0.5f, 0.5f);
        glVertex2d( 0.5f, 0.9f);
    glEnd();


    //Vitore vorat korte chiale GL_TRIANGLES na chaile GL_LINE_LOOP
    /* Draw two filled triangles with second triangle having
       different colors in different vertices */
    // at 'left-middle'
    glBegin(GL_TRIANGLES);  // Each set of 3 vertices form a triangle
        glColor3f(0.0f,1.0f,1.0f);  // Cyan
        glVertex2f(-0.8f, 0.3f);
        glVertex2f(-1.0f,-0.1f);
        glVertex2f(-0.6f,-0.1f);

        // 3 ta point peye gele triangle sesh 
        // Rainbow colored triangle
        glColor3f(1.0f,0.0f,0.0f);  // Red
        glVertex2f(-0.5f, 0.1f);
        glColor3f(0.0f,1.0f,0.0f);  // Green
        glVertex2f(-0.7f,-0.3f);
        glColor3f(0.0f,0.0f,1.0f);  // Blue
        glVertex2f(-0.3f,-0.3f);
    glEnd();

    // Draw two filled quadrilaterals: the second one on top of first one
    // at the 'middle'
    glBegin(GL_QUADS);  // Each set of 4 vertices form a quad
        glColor3f(1.0f,0.0f,0.0f);  // Red
        glVertex2d( 0.0f, 0.3f);
        glVertex2d(-0.1f,-0.1f);
        glVertex2d( 0.4f,-0.2f);
        glVertex2d( 0.3f, 0.3f);

        //Porer ta ager tar upor overlap hobe. drawing er moto na kintu. just new jinis diye replace 
        glColor3f(0.0f,1.0f,0.0f);  // Green
        glVertex2d(-0.3f, 0.1f);
        glVertex2d(-0.2f,-0.3f);
        glVertex2d( 0.1f,-0.3f);
        glVertex2d( 0.2f, 0.2f);
    glEnd();

    /* Erroneous drawing of a quadrilateral.
       If v0, v1, v2, v3 are the vertices of the quadrilateral,
       then it actually draws two triangles: (v0,v1,v2) & (v0,v2,v3) */
    // at 'right-middle'
    glBegin(GL_QUADS);  // Each set of 4 vertices form a quad
        glColor3f(1.0f,0.0f,0.0f);  // Red
        glVertex2d( 0.5f,-0.2f);
        glVertex2d( 0.5f, 0.2f);
        glVertex2d( 0.9f,-0.2f);
        glVertex2d( 0.9f, 0.2f);
    glEnd();


    //ekta polygon function e multiple aka kintu jabe na 
    // Draw a filled hexagon (which is a polygon)
    // at 'bottom-left'
    glBegin(GL_POLYGON);    // All vertices form a single polygon
        glColor3f(0.5f,0.5f,0.5f);  // Gray
        glVertex2d(-0.82f,-0.5f);
        glVertex2d(-0.92f,-0.7f);
        glVertex2d(-0.82f,-0.9f);
        glVertex2d(-0.58f,-0.9f);
        glVertex2d(-0.48f,-0.7f);
        glVertex2d(-0.58f,-0.5f);
    glEnd();


    //Ekhon amra ekta circle akbo //center (cx, cy) and radius r 
    float cx, cy, r;

    // Draw the border of a circle
    // at 'bottom-middle'
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(0.5f,0.5f,1.0f);  // Light-blue
        cx = 0;
        cy = -0.7;
        r = 0.2;
        //circle aro beshi smooth korte chaile theta +=1 dite paro 
        //kintu same e thaktese
        // Help me 
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + r * cos(theta/180*M_PI); //x=rcos theta
            float y = cy + r * sin(theta/180*M_PI); //y=rsing theta
            glVertex2f(x,y);
        }
    glEnd();

    // Draw a filled circle
    // at 'bottom-right'
    glBegin(GL_POLYGON);    // All vertices form a single polygon
        glColor3f(1.0f,0.5f,0.0f);  // Orange
        cx = 0.7;
        cy = -0.7;
        r = 0.2;
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + r * cos(theta/180*M_PI);
            float y = cy + r * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();

    glFlush();  // Render now
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    //This initializes the GLUT library and parses any command-line arguments passed to the program.
    glutInit(&argc, argv);                  // Initialize GLUT
    
    //This sets the initial size of the window to 640x640 pixels.
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    
    //This sets the initial position of the window's top-left corner on the screen to (50, 50) pixels.
    //eita na dile OS thik kore dibe 
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("OpenGL 2D Drawing");  // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutMainLoop();                         // Enter the infinitely event-processing loop
    return 0;
}
