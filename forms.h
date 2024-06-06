#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"

class Hole; // Forward declaration of Hole class

//bool collisionHole(Hole* hole, Sphere* ball, Cube_face& plateau);

class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);
const Color BLACK(0.0f, 0.0f, 0.0f);

// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};


// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
public:
    Sphere(double r = 1.0, Color cl = Color());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}

    void update(double delta_t);
    void render();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
    Point org;
    Hole* hole;
public:
    Cube_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());
    Vector getv1() const { return vdir1; }
    Vector getv2() const { return vdir2; }
    void setv1(Vector vect) { vdir1 = vect; }
    void setv2(Vector vect) { vdir2 = vect; }
    double getLength() const { return length; }
    double getWidth() const { return width; }
    Point getOrg() const { return org; }
    Point setOrg(Point pt) { org = pt; }
    void setHole(Hole* h);
    Hole* getHole() { return hole; }
    void update(double delta_t);
    void render();
};

class Plateau : public Form
{
    private:
	double length, width;
    Point origin;
    public:
        Cube_face* bords[5];
        Plateau(double l = 1.0, double w = 1.0,Point origine = Point(), Point holePos = Point(), double holeWidth = 0.1, Color cl = RED);
        void setLength(double l) {length = l;}
        void setWidth(double w) {width = w;}
        void update(double delta_t);
        void render();
};

// Hole class definition
class Hole : public Form
{
private:
    Point position; 
    double radius; 
public:
     
    Hole(Point pos = Point(), double r = 1);
    Point getPosition() const {return position;}
    void setPosition(Point pos) {position = pos;}
    double getRadius() const {return radius;}
    void update(double delta_t) override {}
    void render();
};

#endif // FORMS_H_INCLUDED
