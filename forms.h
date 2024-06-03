#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <vector>

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
    Sphere(double r = 1.0, Color cl = Color(), Point org = Point());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
    //Vector getPosVector();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
    Point org;
public:
    Cube_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());
    void update(double delta_t);
    void render();
    Vector getv1() const { return vdir1; }
    Vector getv2() const { return vdir2; }
    void setv1(Vector vect) { vdir1 = vect; }
    void setv2(Vector vect) { vdir2 = vect; }
    double getLength() const { return length; }
    double getWidth() const { return width; }
    Point getOrg() const { return org; }
    void setOrg(Point pt) { org = pt; }

};

// A wall of cubes
class Wall : public Form
{
    private:
	Cube_face base; // Plateau
    Cube_face* wall_list[4]; // Liste des faces du mur (4)
	double length_base, width_base, wall_height;
public:
    Wall(Cube_face base, double wall_height, Color cl = Color());// constructeur wall_list
	void update(double delta_t);
	void render();
	void setBase(Cube_face cf) { base = cf; }
	Cube_face getBase() const { return base; }
	//void setWallList(Cube_face* cf, int i) { wall_list[i] = cf; }
	Cube_face* getWallList() const { return *wall_list; }
	double getLengthBase() const { return length_base; }
	double getWidthBase() const { return width_base; }
	void setWallHeight(double h) { wall_height = h; }
	double getWallHeight() const { return wall_height; }
	Point getBaseOrg() const { return base.getOrg(); }
    // Change la position des murs en fonctions et effectue la rotation nécessaire
    void setWallPos(Vector vRot, double angle);
};



class Plateau : public Form
{
private:
    std::vector<Cube_face> faces;

public:
    Plateau(double size = 1.0, Color cl = Color());
    void update(double delta_t);
    void render();
};

#endif // FORMS_H_INCLUDED
