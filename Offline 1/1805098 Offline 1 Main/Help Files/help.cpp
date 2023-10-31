#include <GL/glut.h>
#include <cmath>
#include <ctime>

// Global variables
int windowWidth = 600;
int windowHeight = 600;
int clockRadius = 250;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void drawClockFace()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i += 6) {
        double angle = 2.0 * M_PI * i / 360.0;
        double x = clockRadius * cos(angle);
        double y = clockRadius * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();
}

void drawHourHand()
{
    glLineWidth(3.0);
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);

    time_t rawTime;
    struct tm* currentTime;
    time(&rawTime);
    currentTime = localtime(&rawTime);

    double hourAngle = (currentTime->tm_hour % 12 + currentTime->tm_min / 60.0) * 30.0;
    hourAngle = 2.0 * M_PI * hourAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.4 * clockRadius * cos(hourAngle), 0.4 * clockRadius * sin(hourAngle));

    glEnd();
}

void drawMinuteHand()
{
    glLineWidth(2.0);
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);

    time_t rawTime;
    struct tm* currentTime;
    time(&rawTime);
    currentTime = localtime(&rawTime);

    double minuteAngle = (currentTime->tm_min + currentTime->tm_sec / 60.0) * 6.0;
    minuteAngle = 2.0 * M_PI * minuteAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.6 * clockRadius * cos(minuteAngle), 0.6 * clockRadius * sin(minuteAngle));

    glEnd();
}

void drawSecondHand()
{
    glLineWidth(1.0);
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);

    time_t rawTime;
    struct tm* currentTime;
    time(&rawTime);
    currentTime = localtime(&rawTime);

    double secondAngle = (currentTime->tm_sec) * 6.0;
    secondAngle = 2.0 * M_PI * secondAngle / 360.0;

    glVertex2f(0.0, 0.0);
    glVertex2f(0.8 * clockRadius * cos(secondAngle), 0.8 * clockRadius * sin(secondAngle));

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(windowWidth / 2, windowHeight / 2, 0.0);

    drawClockFace();
    drawHourHand();
    drawMinuteHand();
    drawSecondHand();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Clock");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();

    return 0;
}