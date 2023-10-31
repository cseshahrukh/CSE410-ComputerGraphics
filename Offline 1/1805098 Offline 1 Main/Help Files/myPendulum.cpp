#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <chrono>
using namespace std; 
// Pendulum variables
double pendulumLength = 200.0; // Length of the pendulum
double pendulumAngle = 45.0;   // Initial angle of the pendulum
double maxAngle=30; 
double pendulumVelocity = 0.0; // Initial angular velocity of the pendulum
double pendulumAcceleration = 0.0; // Angular acceleration of the pendulum
double gravity = 9.8; // Acceleration due to gravity
double timePeriod = 2.0; // Desired time period in seconds

// Start time variable
std::chrono::steady_clock::time_point startTime;

// Function to start the timer
void startTimer() {
    startTime = std::chrono::steady_clock::now();
}

// Function to get the elapsed time in milliseconds
double getElapsedTime() {
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
    return elapsed.count();
}
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
    gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2); // Set the orthographic projection
}

// Function to update the pendulum's angle and velocity
void updatePendulum()
{
    // Get the elapsed time
    double elapsedTime = getElapsedTime();
    //std::cout << "Elapsed Time: " << elapsedTime << " milliseconds" << std::endl;
    elapsedTime=elapsedTime/1000; 
    //time_t now = time(0); // get current date and time  
        //tm* ltm = localtime(&now);  
        //int second=0+ltm->tm_sec;

    // Calculate the angular acceleration
    //pendulumAcceleration = -(gravity / pendulumLength) * sin(pendulumAngle * M_PI / 180.0);

    // Update the angular velocity and angle using the Verlet integration method
    //pendulumVelocity += pendulumAcceleration;
    //pendulumAngle += pendulumVelocity;

    pendulumAngle=maxAngle*cos((2*M_PI/timePeriod)*elapsedTime); 
    // Damping factor to gradually reduce the velocity
    //double damping = 1;
    //pendulumVelocity *= damping;

    // Normalize the angle between -180 and 180 degrees
    if (pendulumAngle > 180.0)
        pendulumAngle -= 360.0;
    else if (pendulumAngle < -180.0)
        pendulumAngle += 360.0;
}

// Function to draw the pendulum
void drawPendulum()
{
    glLineWidth(2.0);
    glColor3f(1.0, 1.0, 1.0); // Set the color to white

    // Calculate the position of the pendulum bob
    double x = pendulumLength * sin(pendulumAngle * M_PI / 180.0);
    double y = -pendulumLength * cos(pendulumAngle * M_PI / 180.0);

    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0); // Draw the top point of the pendulum
    glVertex2f(x, y);     // Draw the bob of the pendulum //Eita just Bob jeikhane thakbe. bob aki nai so far
    glEnd();

    // Draw a small circle at the bob of the pendulum
    const int numSegments = 100;
    const double radius = 10.0;
    
    glBegin(GL_POLYGON); 
    for (int i = 0; i <= numSegments; i++)
    {
        //x and y hocche Bob er center 
        double theta = 2.0 * M_PI * i / numSegments;
        double px = x + radius * cos(theta);
        double py = y + radius * sin(theta);
        glVertex2f(px, py);
    }
    glEnd();
}

// Function to display the scene
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawPendulum(); // Draw the pendulum

    glutSwapBuffers(); // Swap the front and back buffers
}

// Function to update the scene and trigger redisplay
void update(int value)
{
    glutTimerFunc(1000 / 60, update, 0); // Call the update function every 1/60th of a second
    updatePendulum(); // Update the pendulum

    glutPostRedisplay(); // Mark the current window as needing to be redisplayed
    
}

int main(int argc, char** argv)
{
    startTimer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pendulum");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();

    // Calculate the initial angular velocity based on the desired time period
    pendulumVelocity = (2.0 * M_PI) / (60.0 * timePeriod);

    glutTimerFunc(0, update, 0); // Call the update function immediately
    glutMainLoop();

    return 0;
}