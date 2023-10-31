//Reshape means window reshape here 

#define _USE_MATH_DEFINES

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
int hour, minute, second; 
float innerCircleRadius=0.32; 
float outerCircleRadius=0.40; 

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    //Display function er suru teo nicher jinis ta call korte hobe 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void drawPoint(){
    // Draw two points
    // at 'top-left'
    glPointSize(2); 
    glBegin(GL_POINTS); // Each vertex is drawn as a single pixel

        glColor3f(1.0f,1.0f,1.0f); // Magenta
        glVertex2d(0.0f, 0.0f);
    glEnd();
}
void drawLine(){
    //frow 0,0 to 1,1
    glBegin(GL_LINES);  
        glColor3f(1.0f,1.0f,1.0f);  // White
        glVertex2d(0.0f, 0.0f);
        glVertex2d( 0.1f, 0.0f);
    glEnd();
}


void drawClockLine(){
    glPushMatrix();
        glTranslated(innerCircleRadius, 0, 0); 

        glRotated(90, 0,0,1); 
                    drawLine();
            //for (int i = 0; i < 1; i++) {   
                    //std::cout<<"Inside draw line"<<std::endl;     
                    
            //}
    glPopMatrix(); 
}




/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    //glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    
    glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    //Draw Clock Line
    //glPushMatrix();
        //glTranslatef(0,0.32,0); 
        drawPoint(); 
        drawClockLine(); 
    //glPopMatrix();


    glFlush();  // Render now
}

//Parameter automatically chole ashbe. oita niye chinta kora lagbe na. 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */

   //It takes two parameters: width and height, 
   //which represent the new dimensions of the window after resizing.
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0

    //The aspect ratio is used to maintain the correct proportions of objects displayed in the window.
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    //Weight 0 holeo same problem hote pare 

    // Set the viewport to cover the new window
    /*
    This line sets the viewport, which defines the portion of the window where OpenGL 
    rendering will be displayed. Here, the viewport is set to cover the entire window 
    with the given width and height.
    */
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    //This line resets the current projection matrix to the identity matrix, clearing any previous transformations.
    glLoadIdentity();             // Reset the projection matrix

    //set the projection matrix based on the aspect ratio.
    //je choto oke thik e -1 to 1 rakhtechi 
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        //left, right, bottom, top
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 480);           // Set the window's initial width & height - non-square
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("Clock"); // Create window with the given title
    glutDisplayFunc(display);               // Register callback handler for window re-paint event

    //maximize minimize shape window size change korle ki kora lagbe oita nicher parameter function e bole dite hobe
    glutReshapeFunc(reshape);               // Register callback handler for window re-size event
    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the infinite event-processing loop
    return 0;
}
