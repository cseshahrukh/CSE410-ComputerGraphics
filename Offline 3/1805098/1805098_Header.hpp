#include <GL/glut.h>
#include <bits/stdc++.h>
#include "1805098_light.hpp"
#include "bitmap_image.hpp"

using namespace std;
class Object; 
extern vector <Object*> objects;
extern vector <Light*> lights;
extern vector <SpotLight*> spotLights;
extern int recursionLevel;
extern bitmap_image image; 
class Object{
    public: 
    string type; 
    Point position;
    double height, width, length;
    // vector of double of size 3
    vector <double> color; // r, g, b
    
    
    vector <double> coefficients; // ambient, diffuse, specular, reflection, shininess

    Object(){
        color = vector <double> (3, 0);
        coefficients = vector <double> (5, 0);
    }

    //getter methods
    Point getPosition(){
        return position;
    }

    double getHeight(){
        return height;
    }

    double getWidth(){
        return width;
    }

    double getLength(){
        return length;
    }

    vector <double> getColor(){
        return color;
    }

    //setter methods 
    void setPosition(Point position){
        this->position = position;
    }

    void setHeight(double height){
        this->height = height;
    }

    void setWidth(double width){
        this->width = width;
    }

    void setLength(double length){
        this->length = length;
    }

    void setColor(vector <double> colors){
        color = colors;
    }

    //get Color
    virtual vector <double> getColorAt(Point point ){
        return color; 
    }

    // get coefficients
    vector <double> getCoefficients(){
        return coefficients;
    }   

    // set coefficients
    void setCoefficients(vector <double> coefficients){
        this->coefficients = coefficients;
    }

    virtual void draw() = 0; // pure virtual function
    virtual Ray getNormal(Point point, Ray incidentRay) = 0; // pure virtual function   
    virtual double intersectionHelper(Ray ray, vector<double> &color, int level) = 0; // pure virtual function

    virtual double intersect(Ray ray, vector<double> &color, int level){
        double t = intersectionHelper(ray, color, level);
        if (t<0)
            return -1; // no intersection

        if (level==0)
            return t;

        Point intersectionPoint = ray.origin + ray.direction*t;
        vector<double> colorAtIntersection = getColorAt(intersectionPoint);

        // ambient (dimm korte chai)
        color[0] = colorAtIntersection[0]*coefficients[0];
        color[1] = colorAtIntersection[1]*coefficients[0];
        color[2] = colorAtIntersection[2]*coefficients[0];

        for (int i=0; i<lights.size(); i++)
        {
            Point lightPosition = lights[i]->position;
            Point lightDirection = intersectionPoint - lightPosition;
            lightDirection.normalize();

            // From light to intersection point, cast incident ray
            Ray incidentRay = Ray(lightPosition, lightDirection);

            // Calculate the normal at the intersection point\
            // normal is a vector
            Ray normal = getNormal(intersectionPoint, incidentRay);

            // Check if incident ray is blocked by any object
            bool isBlocked = false;
            double len= (intersectionPoint-lightPosition).length();

            if (len<1e-5)
                continue;
            
            // Traverse through all objects
            for (Object *obj : objects)
            {
                double t = obj->intersectionHelper(incidentRay, color, 0);
                if (t>0 && t+1e-5<len)
                {
                    isBlocked = true;
                    break;
                }
            }
            
            
            if (isBlocked)
            {
                // cout<<"isBlocked"<<endl; 
            }
            else 
            {
                // lambert's law
                double val = max(0.0, -incidentRay.direction*normal.direction);

                // find the reflection ray
                Ray refelction = Ray(intersectionPoint, incidentRay.direction - normal.direction*2*(incidentRay.direction*normal.direction));

                // now Phong model
                double phong = max(0.0, -ray.direction*refelction.direction);

                // update diffuse and specular
                color[0] += coefficients[1]*val *colorAtIntersection[0] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[0];
                color[1] += coefficients[1]*val *colorAtIntersection[1] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[1];
                color[2] += coefficients[1]*val *colorAtIntersection[2] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[2]; 
            }


        }

        // same calculation for sporlight
        for (int i=0; i<spotLights.size(); i++)
        {
            Point lightPosition = spotLights[i]->pointLight.position;
            Point lightDirection = intersectionPoint - lightPosition;
            lightDirection.normalize();
            
            double dot = lightDirection*spotLights[i]->direction;
            double angle = acos(dot/(lightDirection.length()*spotLights[i]->direction.length()));
            angle = angle*(180/acos(-1));

            if (fabs(angle)<spotLights[i]->cutOffAngle)
            {
                Ray incidentRay = Ray(lightPosition, lightDirection);
                Ray normal = getNormal(intersectionPoint, incidentRay);

                bool isBlocked = false;
                Ray reflection = Ray (intersectionPoint, incidentRay.direction - normal.direction*2*(incidentRay.direction*normal.direction));

                double len = (intersectionPoint-lightPosition).length();
                if (len<1e-5)
                {
                    continue;
                }

                for (Object *object : objects)
                {
                    double t = object->intersectionHelper(incidentRay, color, 0);
                    if (t>0 && t+1e-5<len)
                    {
                        isBlocked = true;
                        break;
                    }
                }

                if (isBlocked)
                {
                    //cout<<"isBlocked"<<endl;
                }
                else 
                {
                    
                    double phong = max(0.0, -ray.direction*reflection.direction);
                    double val = max(0.0, -incidentRay.direction*normal.direction);

                    color[0] += coefficients[1]*val *colorAtIntersection[0] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[0];
                    color[1] += coefficients[1]*val *colorAtIntersection[1] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[1];
                    color[2] += coefficients[1]*val *colorAtIntersection[2] + coefficients[2]*pow(phong, coefficients[4])*colorAtIntersection[2]; 
                }
            }

        }
        if (level>=recursionLevel)
        {
            return t; 
        }
        if (level<recursionLevel)
        {
            // Find normal at intersection point 
            Ray normal = getNormal(intersectionPoint, ray);

            // Find reflection ray
            Ray reflection = Ray(intersectionPoint, ray.direction - normal.direction*2*(ray.direction*normal.direction));

            reflection.origin= reflection.origin + reflection.direction*1e-5; // avoid self intersection

            // Find nearest intersection object and after that call intersect recursively
            double t = -1; 
            double tMin = 1e9; // infinity
            int nearObjectIndex=-1; 
            

            for (int k=0; k<(int)objects.size(); k++)
            {
                t = objects[k]->intersect(reflection, color, 0);
                if (t>0 && t<tMin)
                {
                    
                    nearObjectIndex = k;
                    tMin = t;
                }
            }

            if (nearObjectIndex==-1)
            {
                // cout<<"No object found"<<endl;
            }
            else 
            {
                vector<double> reflectedColor = vector<double> (3, 0);
                double t = objects[nearObjectIndex]->intersect(reflection, reflectedColor, level+1);

                color[0] += reflectedColor[0]*coefficients[3];
                color[1] += reflectedColor[1]*coefficients[3];
                color[2] += reflectedColor[2]*coefficients[3];
            }
        }

        return t; 
    }
    //destructors
    virtual ~Object(){
        coefficients.clear();
    }

    
}; 

class Triangle : public Object{
    public :
    Point a, b, c;
    
    Triangle(){
        type="triangle";
    }

    Triangle(Point A, Point B, Point C){
        type="triangle";
        a = A;
        b = B;
        c = C;
    }

    //getter methods
    Point geta(){
        return a;
    }

    Point getb(){
        return b;
    }

    Point getc(){
        return c;
    }

    //setter methods

    void seta(Point A){
        a = A;
    }

    void setb(Point B){
        b = B;
    }

    void setc(Point C){
        c = C;
    }


    virtual Ray getNormal(Point point, Ray incidentRay){
        Point normal = (b-a)^(c-a);
        normal.normalize();
        if (incidentRay.direction*normal<0)
        {
            normal = -normal;
        }
        return Ray(point, normal);
    }

    virtual void draw(){
        // to do later that getter method 
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
            glVertex3f(a.getX(), a.getY(), a.getZ());
            glVertex3f(b.getX(), b.getY(), b.getZ());
            glVertex3f(c.getX(), c.getY(), c.getZ());
        glEnd();
    }

    virtual double intersectionHelper(Ray ray, vector<double> &color, int level){
        double betaMatrix[3][3]= {
            {a.getX()-ray.origin.getX(), a.getX()-c.getX(), ray.direction.getX()},
            {a.getY()-ray.origin.getY(), a.getY()-c.getY(), ray.direction.getY()},
            {a.getZ()-ray.origin.getZ(), a.getZ()-c.getZ(), ray.direction.getZ()}
        };

        double gammaMatrix[3][3]= {
            {a.getX()-b.getX(), a.getX()-ray.origin.getX(), ray.direction.getX()},
            {a.getY()-b.getY(), a.getY()-ray.origin.getY(), ray.direction.getY()},
            {a.getZ()-b.getZ(), a.getZ()-ray.origin.getZ(), ray.direction.getZ()}
        };

        double tMatrix[3][3]= {
            {a.getX()-b.getX(), a.getX()-c.getX(), a.getX()-ray.origin.getX()},
            {a.getY()-b.getY(), a.getY()-c.getY(), a.getY()-ray.origin.getY()},
            {a.getZ()-b.getZ(), a.getZ()-c.getZ(), a.getZ()-ray.origin.getZ()}
        };

        double aMatrix[3][3]= {
            {a.getX()-b.getX(), a.getX()-c.getX(), ray.direction.getX()},
            {a.getY()-b.getY(), a.getY()-c.getY(), ray.direction.getY()},
            {a.getZ()-b.getZ(), a.getZ()-c.getZ(), ray.direction.getZ()}
        };

        double ADeterminant = determinant(aMatrix);
        
        double tDeterminant = determinant(tMatrix)/ADeterminant;

        double betaDeterminant = determinant(betaMatrix)/ADeterminant;

        double gammaDeterminant = determinant(gammaMatrix)/ADeterminant;

        if (betaDeterminant>0 && gammaDeterminant>0 && betaDeterminant+gammaDeterminant<1 && tDeterminant>0)
        {
            return tDeterminant;
        }
        else 
        {
            return -1;
        }
    }

    // input stream of triangle
    friend istream &operator>>(istream &input, Triangle &triangle){
        input >> triangle.a >> triangle.b >> triangle.c;
        input >> triangle.color[0] >> triangle.color[1] >> triangle.color[2];

        // now take the coefficients
        for(int i=0; i<5; i++){
            input >> triangle.coefficients[i];
        }
        return input;
    }
};

class Sphere : public Object{
    public : 
    Point center;
    double radius;

    Sphere(){
        type="sphere";
    }

    Sphere(Point center, double radius){
        this->center = center;
        this->radius = radius;
        position = center;
        length = radius;


    }

    //getter methods
    Point getCenter(){
        return center;
    }

    double getRadius(){
        return radius;
    }

    //setter methods
    void setCenter(Point center){
        this->center = center;
    }

    void setRadius(double radius){
        this->radius = radius;
    }



    virtual Ray getNormal(Point point, Ray incidentRay){
        Point normal = point - position;
        //normal.normalize();
        
        return Ray(point, normal);
    }
    // Will modify later
    virtual void draw(){
        int stacks = 30;
        int slices = 20;

        Point points[100][100];
        int i, j;
        double h, r;
        // generate points of the sphere
        for (i = 0; i <= stacks; i++)
        {
            h = length * sin(((double)i / (double)stacks) * (acos(-1) / 2));
            r = length * cos(((double)i / (double)stacks) * (acos(-1) / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * acos(-1));
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * acos(-1));
                points[i][j].z = h;
            }
        }
        //draw quadrelaterals using generated points
        for (i = 0; i < stacks; i++)
        {
            glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glColor3f(color[0], color[1], color[2]);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
            glPopMatrix();
        }
    }

    virtual double intersectionHelper(Ray ray, vector<double> &color, int level){

            ray.origin = ray.origin - position; // adjust ray origin
            
            
            double b = 2 * (ray.direction*ray.origin);
            double c = (ray.origin*ray.origin) - (length*length);
            double a = 1;

            double discriminant = pow(b, 2) - 4 * a * c;
            double t = -1;
            if (discriminant < 0){
                t = -1;
            }
            else{
                
                if(fabs(a) < 1e-5)
                {
                    t = -c/b;
                    return t;
                }

                double t1 = (-b - sqrt(discriminant)) / (2 * a);
                double t2 = (-b + sqrt(discriminant)) / (2 * a);

                if(t1>t2) swap(t1, t2);

                if (t1 > 0){
                    t = t1;
                }
                else if (t2 > 0){
                    t = t2;
                }
                else{
                    t = -1;
                }
            }

            return t;
            
        }

    // input stream of sphere
    friend istream &operator>>(istream &input, Sphere &sphere){
        input >> sphere.center >> sphere.radius;
        sphere.position=sphere.center;
        sphere.length=sphere.radius;
        input >> sphere.color[0] >> sphere.color[1] >> sphere.color[2];

        // now take the coefficients
        for(int i=0; i<5; i++){
            input >> sphere.coefficients[i];
        }
        return input;
    }
};

class Floor : public Object{

    public :
    int tiles;
    int tileWidth; 
    Point floorCenter;  
    Floor(){
        floorCenter = Point(0, 0, 0);
        type="floor";
        tiles=1; 
    }

    Floor (int floorWidth, int tileWidth){
        this->tileWidth = tileWidth;
        tiles = floorWidth/tileWidth;
        position = Point(-floorWidth/2, -floorWidth/2, 0);
        position = position + floorCenter;
        length = tileWidth;
        
    }

    void setFloor(int floorWidth, int tileWidth){
        tiles = floorWidth/tileWidth;
        position = Point(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;
    }


    virtual vector<double> getColorAt(Point point){

        int tileX = (point.x - position.x) / length;
		int tileY = (point.y - position.y) / length;

        if(tileX<0 || tileX>=tiles || tileY<0 || tileY>=tiles){
            return vector<double> {0, 0, 0};
        }

		if (((tileX + tileY) % 2) == 0)
		{
            vector<double> color = vector<double> {1, 1, 1};
			return color; 
		}
		else
		{
			return vector<double> {0, 0, 0};
		}
    }

    virtual Ray getNormal(Point point, Ray incidentRay){
        double value; 
        if (incidentRay.direction.z>0)
            value = 1;
        else 
            value = -1; 

        return Ray(point, Point(0,0, value)); 
        // if(incidentRay.direction.z > 0) 
        //     return Ray(point, Point(0, 0, 1));
        // else return Ray(point, Point(0, 0, -1));
    }

    virtual void draw(){
        //position = position - floorCenter;
        for (int i = 0; i < tiles; i++)
		{
			for (int j = 0; j < tiles; j++)
			{
				if (((i + j) % 2) == 0) glColor3f(1, 1, 1);
				else glColor3f(0, 0, 0);

				glBegin(GL_QUADS);
				{
					glVertex3f(position.x + i * length, position.y + j * length, 0);
					glVertex3f(position.x + (i + 1) * length, position.y + j * length, 0);
					glVertex3f(position.x + (i + 1) * length, position.y + (j + 1) * length, 0);
					glVertex3f(position.x + i * length, position.y + (j + 1) * length, 0);
				}
				glEnd();

                // glBegin(GL_QUADS);
                // {
                //     glVertex3f(cameraPosition.x + i * length, cameraPosition.y + j * length, 0);
                //     glVertex3f(cameraPosition.x + (i + 1) * length, cameraPosition.y + j * length, 0);
                //     glVertex3f(cameraPosition.x + (i + 1) * length, cameraPosition.y + (j + 1) * length, 0);
                //     glVertex3f(cameraPosition.x + i * length, cameraPosition.y + (j + 1) * length, 0);
                // }
                // glEnd();
			}
		}
        
    }

    virtual double intersectionHelper(Ray ray, vector<double> &color, int level){
        Point normal = Point(0, 0, 1);
        double dotP = normal * ray.direction;
        
        if (round(dotP * 100) == 0)
			return -1;

        double t = -(normal * ray.origin) / dotP;

        Point p = ray.origin + ray.direction * t;

        if(p.x <= position.x || p.x >= abs(position.x) && p.y <= position.y && p.y >= abs(position.y)){
            return -1;
        }
        
        return t;
    }

    // input stream of floor
    friend istream &operator>>(istream &input, Floor &floor){

        input >> floor.length;

        floor.setFloor(10000, floor.length);
        

        // now take the coefficients
        // ambient, diffuse, specular, reflection, shininess
        // here no specular and shininess
        for(int i=0; i<5; i++){
            if (i==2 || i==4){
                floor.coefficients[i] = 0;
                continue;
            }
            input >> floor.coefficients[i];
        }
        return input;
    }
}; 

// now pyramid 
class Pyramid : public Object{
    public : 
    Point A, B, C, D, top; 
    double width;
    double height;
    Triangle triangles[6] ; //total 5 face and 2 triangle for base and one of each side 

    Pyramid(){
        type="pyramid";
    }

    Pyramid(Point A, double width, double height){
        this->A = A;
        this->width = width;
        this->height = height;
    }

    // take input from file
    friend istream &operator>>(istream &input, Pyramid &pyramid){
        input >> pyramid.A >> pyramid.width >> pyramid.height;
        input >> pyramid.color[0] >> pyramid.color[1] >> pyramid.color[2];

        // now take the coefficients
        for(int i=0; i<5; i++){
            input >> pyramid.coefficients[i];
        }

        // now calculate the other points
        pyramid.B = Point(pyramid.A.getX()+pyramid.width, pyramid.A.getY(), pyramid.A.getZ());
        pyramid.C = Point(pyramid.A.getX()+pyramid.width, pyramid.A.getY()+pyramid.width, pyramid.A.getZ());
        pyramid.D = Point(pyramid.A.getX(), pyramid.A.getY()+pyramid.width, pyramid.A.getZ());

        // now calculate the top point
        pyramid.top = Point(pyramid.A.getX()+pyramid.width/2, pyramid.A.getY()+pyramid.width/2, pyramid.A.getZ()+pyramid.height);
        
        // now calculate the triangles
        // base triangle
        pyramid.triangles[0] = Triangle(pyramid.A, pyramid.B, pyramid.C);
        pyramid.triangles[1] = Triangle(pyramid.A, pyramid.C, pyramid.D);

        // now calculate the other triangles
        pyramid.triangles[2] = Triangle(pyramid.A, pyramid.B, pyramid.top);
        pyramid.triangles[3] = Triangle(pyramid.B, pyramid.C, pyramid.top);
        pyramid.triangles[4] = Triangle(pyramid.C, pyramid.D, pyramid.top);
        pyramid.triangles[5] = Triangle(pyramid.D, pyramid.A, pyramid.top);

        // all triangles will be same color of the pyramid
        for(int i=0; i<6; i++){
            pyramid.triangles[i].setColor(pyramid.color);
            pyramid.triangles[i].setCoefficients(pyramid.coefficients);
            objects.push_back(&pyramid.triangles[i]);
        }

        
        return input;
    }

    virtual void draw(){
        cout<<"Inside no man's land"<<endl; 
        for(int i=0; i<6; i++){
            triangles[i].draw();
        }
    }

    virtual double intersectionHelper(Ray ray, vector<double> &color, int level){
        cout<<"Inside no man's land"<<endl; 
        double t = -1;
        for(int i=0; i<6; i++){
            double temp = triangles[i].intersectionHelper(ray, color, level);
            if(temp>0){
                if(t<0) t = temp;
                else t = min(t, temp);
            }
        }
        return t;
    }

    virtual Ray getNormal(Point point, Ray incidentRay){
        cout<<"Inside no man's land"<<endl; 
        Point normal = (B-A)^(C-A);
        normal.normalize();
        if (incidentRay.direction*normal<0)
        {
            normal = -normal;
        }
        return Ray(point, normal);
    }


};

//Now Cube 
class Cube : public Object{
    public : 
    Point A, B, C, D, E, F, G, H; 
    double width;
    Triangle triangles[12] ; //total 6 face and 2 triangle for base and one of each side 

    Cube(){
        type="cube";
    }

    Cube(Point A, double width){
        this->A = A;
        this->width = width;
    }

    // take input from file
    friend istream &operator>>(istream &input, Cube &cube){
        input >> cube.A >> cube.width;
        input >> cube.color[0] >> cube.color[1] >> cube.color[2];

        // now take the coefficients
        for(int i=0; i<5; i++){
            input >> cube.coefficients[i];
        }

        
        // now calculate the other points
        cube.B = Point(cube.A.getX()+cube.width, cube.A.getY(), cube.A.getZ());
        cube.C = Point(cube.A.getX()+cube.width, cube.A.getY()+cube.width, cube.A.getZ());
        cube.D = Point(cube.A.getX(), cube.A.getY()+cube.width, cube.A.getZ());

        cube.E = Point(cube.A.getX(), cube.A.getY(), cube.A.getZ()+cube.width);
        cube.F = Point(cube.A.getX()+cube.width, cube.A.getY(), cube.A.getZ()+cube.width);
        cube.G = Point(cube.A.getX()+cube.width, cube.A.getY()+cube.width, cube.A.getZ()+cube.width);
        cube.H = Point(cube.A.getX(), cube.A.getY()+cube.width, cube.A.getZ()+cube.width);

        //print the points
        cout<<"A "<<cube.A<<endl;
        cout<<"B "<<cube.B<<endl;
        cout<<"C "<<cube.C<<endl;
        cout<<"D "<<cube.D<<endl;
        cout<<"E "<<cube.E<<endl;
        cout<<"F "<<cube.F<<endl;
        cout<<"G "<<cube.G<<endl;
        cout<<"H "<<cube.H<<endl;
        // now calculate the triangles
        // base triangle
        cube.triangles[0] = Triangle(cube.A, cube.B, cube.C);
        cube.triangles[1] = Triangle(cube.A, cube.C, cube.D);

        // now calculate the other triangles
        cube.triangles[2] = Triangle(cube.A, cube.B, cube.E);
        cube.triangles[3] = Triangle(cube.B, cube.F, cube.E);
        cube.triangles[4] = Triangle(cube.B, cube.C, cube.F);
        cube.triangles[5] = Triangle(cube.C, cube.G, cube.F);
        cube.triangles[6] = Triangle(cube.C, cube.D, cube.G);
        cube.triangles[7] = Triangle(cube.D, cube.H, cube.G);
        cube.triangles[8] = Triangle(cube.D, cube.A, cube.H);
        cube.triangles[9] = Triangle(cube.A, cube.E, cube.H);
        cube.triangles[10] = Triangle(cube.E, cube.F, cube.H);
        cube.triangles[11] = Triangle(cube.F, cube.G, cube.H);

        // all triangles will be same color of the cube
        for(int i=0; i<12; i++){
            cube.triangles[i].setColor(cube.color);
            cube.triangles[i].setCoefficients(cube.coefficients);
            objects.push_back(&cube.triangles[i]);
        }
        return input;
    }
    

    virtual void draw(){
        cout<<"Inside no man's land"<<endl; 
        for(int i=0; i<12; i++){
            triangles[i].draw();
        }
    }

    virtual double intersectionHelper(Ray ray, vector<double> &color, int level){
        cout<<"Inside no man's land"<<endl; 
        double t = -1;
        for(int i=0; i<12; i++){
            double temp = triangles[i].intersectionHelper(ray, color, level);
            if(temp>0){
                if(t<0) t = temp;
                else t = min(t, temp);
            }
        }
        return t;
    }

    virtual Ray getNormal(Point point, Ray incidentRay){
        cout<<"Inside no man's land"<<endl; 
        Point normal = (B-A)^(C-A);
        normal.normalize();
        if (incidentRay.direction*normal<0)
        {
            normal = -normal;
        }
        return Ray(point, normal);
    }

}; 

