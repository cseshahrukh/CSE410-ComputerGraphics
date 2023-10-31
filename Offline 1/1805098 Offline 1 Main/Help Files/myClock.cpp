//Reshape means window reshape here 

#define _USE_MATH_DEFINES

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
using namespace std; 
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
// Draw a 1x1 Square
void drawSquare() {
    glBegin(GL_QUADS);
        glVertex2f(-0.5,-0.5);
        glVertex2f(-0.5, 0.5);
        glVertex2f( 0.5, 0.5);
        glVertex2f( 0.5,-0.5);
    glEnd();
}

void drawSquareBorder(){
        // Draw border of a square
    // at 'top-right'
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(1.0f,1.0f,0.0f); // Yellow
        glVertex2f(-0.5,-0.5);
        glVertex2f(-0.5, 0.5);
        glVertex2f( 0.5, 0.5);
        glVertex2f( 0.5,-0.5);
    glEnd();
}

void drawCircle(){
        float cx, cy, r;

    // Draw the border of a circle
    // at 'bottom-middle'
    glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        glColor3f(1.0f,1.0f,1.0f);  // Light-blue
        cx = 0.0;
        cy = 0.0;
        r = 0.1;
        for (float theta = 0; theta < 360; theta += 5) {
            float x = cx + r * cos(theta/180*M_PI);
            float y = cy + r * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();
}

void drawClockLine(){
    
        
        for (float theta = 0; theta < 360; theta += 30) {

            float x = innerCircleRadius * cos(theta/180*M_PI);
            float y = innerCircleRadius * sin(theta/180*M_PI);
            //glVertex2f(x,y);
            glPushMatrix();
                glTranslated(x, y, 0); 
                glRotated(theta, 0,0,1); 
                drawLine();
            glPopMatrix(); 
        }

        //glTranslated(innerCircleRadius, 0, 0); 
            //for (int i = 0; i < 12; i++) {   
                    //std::cout<<"Inside draw line"<<std::endl;     
                    //glRotated(30, 0,0,1); 
                    //drawLine();
            //}
    
}


void drawFilledCircle(){
    // Draw a filled circle
    // at 'bottom-right'
    glBegin(GL_POLYGON);    // All vertices form a single polygon
        glColor3f(1.0f,0.5f,0.0f);  // Orange
        float cx = 0.7;
        float cy = -0.7;
        float r = 0.2;
        for (float theta = 0; theta < 360; theta += 10) {
            float x = cx + r * cos(theta/180*M_PI);
            float y = cy + r * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();
}

void drawHourHand()
{

    time_t now = time(0); // get current date and time  
        tm* ltm = localtime(&now);  
      
        // print various components of tm structure.  
        //cout << "Year:" <<  1900 + ltm->tm_year << endl; // print the year  
        //cout << "Month: " << 1 + ltm->tm_mon << endl; // print month number  
        //cout << "Day: " << ltm->tm_mday << endl; // print the day  
        // Print time in hour:minute:second  
        int hour=0+ltm->tm_hour; 
        int minute=0 + ltm->tm_min; 
        int second=0+ltm->tm_sec;
        //cout<<"Hour is "<<hour<<endl; 
    glLineWidth(3.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);

    time_t rawTime;
    struct tm* currentTime;
    time(&rawTime);
    currentTime = localtime(&rawTime);

    double hourAngle = 90.0-(hour % 12 + minute / 60.0) * 30.0;
    hourAngle = 2.0 * M_PI * hourAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.4 * innerCircleRadius * cos(hourAngle), 0.4 * innerCircleRadius * sin(hourAngle));

    glEnd();
}

void drawMinuteHand()
{
    glLineWidth(2.0);
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);

    time_t now = time(0); // get current date and time  
        tm* ltm = localtime(&now);  
      
        // print various components of tm structure.  
        //cout << "Year:" <<  1900 + ltm->tm_year << endl; // print the year  
        //cout << "Month: " << 1 + ltm->tm_mon << endl; // print month number  
        //cout << "Day: " << ltm->tm_mday << endl; // print the day  
        // Print time in hour:minute:second  
        int hour=0+ltm->tm_hour; 
        int minute=0 + ltm->tm_min; 
        int second=0+ltm->tm_sec;
    

    double minuteAngle = 90-(minute + second / 60.0) * 6.0;
    minuteAngle = 2.0 * M_PI * minuteAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.6 * innerCircleRadius * cos(minuteAngle), 0.6 * innerCircleRadius * sin(minuteAngle));

    glEnd();
}

void drawSecondHand()
{
    glLineWidth(1.0);
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);

    time_t now = time(0); // get current date and time  
        tm* ltm = localtime(&now);  
      
        // print various components of tm structure.  
        //cout << "Year:" <<  1900 + ltm->tm_year << endl; // print the year  
        //cout << "Month: " << 1 + ltm->tm_mon << endl; // print month number  
        //cout << "Day: " << ltm->tm_mday << endl; // print the day  
        // Print time in hour:minute:second  
        int hour=0+ltm->tm_hour; 
        int minute=0 + ltm->tm_min; 
        int second=0+ltm->tm_sec;

    double secondAngle = 90-(second) * 6.0;
    secondAngle = 2.0 * M_PI * secondAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.8 * innerCircleRadius * cos(secondAngle), 0.8 * innerCircleRadius * sin(secondAngle));

    glEnd();
}


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    //glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    
    glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    
    glPushMatrix();
        glTranslatef(0,0.4,0); 
        //glRotatef(rot_a_1, 0,0,1);  // Align the square for Rotation about it's own axis
        glScalef(innerCircleRadius*10,innerCircleRadius*10,1);    // Scale down the square
        glColor3f(1,1,1);   // Yellow
        drawCircle();
    glPopMatrix();

        
    glPushMatrix();
        glTranslatef(0,0.4,0); 
        //glRotatef(rot_a_1, 0,0,1);  // Align the square for Rotation about it's own axis
        glScalef(outerCircleRadius*10,outerCircleRadius*10,1);    // 
        glColor3f(1,1,1);   // Yellow
        drawCircle();
    glPopMatrix();
    
    glBegin(GL_LINE_LOOP);  // Each set of 4 vertices form a quad
        glColor3f(1.0f,1.0f,0.0f);  // 
        glVertex2d( 0.5f,0.9f);
        glVertex2d( -0.5f, 0.9f);
        glVertex2d( -0.5f,-0.1f);
        glVertex2d(0, -0.5); 
        glVertex2d( 0.5f, -0.1f);
    glEnd();

    //Draw Clock Line
    glPushMatrix();
        glTranslatef(0,0.4,0); 
        drawClockLine(); 
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0,0.4,0.0);  
        glScaled(1.2,1.2,1.2); 
        drawPoint();
    glPopMatrix(); 

    glPushMatrix();
        glTranslated(0.0,0.4,0.0);  
        drawHourHand();
        drawMinuteHand();
        drawSecondHand();
    glPopMatrix(); 

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

void update(int value)
{
    glutPostRedisplay(); // Mark the current window as needing to be redisplayed
    glutTimerFunc(1000 / 60, update, 0); // Call the update function every 1/60th of a second
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
    glutTimerFunc(0, update, 0); // Call the update function immediately
    glutMainLoop();                         // Enter the infinite event-processing loop
    return 0;
}
