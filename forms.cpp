#include <cmath>
#include <gl\glew.h>
#include <SDL_opengl.h>
//#include <gl\glu.h> // Deja inclus dans glew.h
#include <SDL.h>

#include "forms.h"

using namespace std;


void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);
    glRotated(anim.getTheta(), 0, 1, 0);
    glRotated(anim.getPhi(), 0, 0, 1);
}


Sphere::Sphere(double r, Color cl)
{
    radius = r;
    col = cl;
    type = SPHERE;
}


void Sphere::update(double delta_t)
{

    //Recup tous les forces en Newton de ton objet 
    double masse = 1;
    double g = 9.81;
    Vector Fg(0, -masse*g*0.01, 0);
    Vector sumForce = Fg + anim.getFn(); //somme des forces = masse * acceleration

    Vector acc = sumForce / masse;
    Vector newSpeed = anim.getSpeed() + acc.integral(delta_t);
    anim.setSpeed(newSpeed);
    cout << "vitesse = " <<anim.getSpeed() << endl;

    // Mise à jour de la position en intégrant la vitesse sur le temps delta_t
    Point newPos = anim.getPos();
    newPos.translate(newSpeed.integral(delta_t));
    anim.setPos(newPos);

    anim.setTheta(anim.getTheta()+1);
    anim.setSpeed(anim.getSpeed() + anim.getAccel());
    Point org = anim.getPos();
    org.translate(anim.getSpeed());
    anim.setPos(org);
}


void Sphere::render()
{
    GLUquadric *quad;

    quad = gluNewQuadric();

    Form::render();
    gluSphere(quad, radius, 10, 10);

    gluDeleteQuadric(quad);
}


Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Cube_face::update(double delta_t)
{
    // Complete this part
}


void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    Form::render();

    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}
