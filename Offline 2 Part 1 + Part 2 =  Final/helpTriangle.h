#include "helpPoint.h"
using namespace std;
static unsigned long int g_seed = 1;


inline int my_random()
{
 g_seed = (214013 * g_seed + 2531011);
 return (g_seed >> 16) & 0x7FFF;
}


class Triangle
{
    //for 3 I don't need to use array
    Point p1, p2, p3;
    int color1, color2, color3;

    public:
    Triangle()
    {
        color1 = my_random() % 256; //random number between 0 and 255
        color2 = my_random() % 256;
        color3 = my_random() % 256;

    }

    Triangle(Point p1, Point p2, Point p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        
        int randomColor1 = my_random() ; //random number between 0 and 255
        int randomColor2 = my_random() ;
        int randomColor3 = my_random() ;
        setColor(randomColor1, randomColor2, randomColor3);

    }

    void setColor(int color1, int color2, int color3)
    {
        this->color1 = color1;
        this->color2 = color2;
        this->color3 = color3;

    }

    

    std::vector<Point> getPoints()
    {
        std::vector<Point> points;
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        return points;
    }

    std::vector<double> getColors()
    {
        std::vector<double> colors;
        colors.push_back(color1);
        colors.push_back(color2);
        colors.push_back(color3);
        return colors;
    }


    
};