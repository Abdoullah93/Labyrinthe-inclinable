#include <cmath>
#include <gl\glew.h>
#include <SDL_opengl.h>
//#include <gl\glu.h> // Deja inclus dans glew.h
#include <SDL.h>

#include "forms.h"

using namespace std;


//BB Rotation de vecteur V autour l'axe A (qui est un vecteur) d'angle Alpha
//le vecteur A doit etre normalisé
Vector Rotation(Vector V, Vector A, double Alpha) {
    Alpha = Alpha * M_PI / 180;
    Vector V_rot = cos(Alpha) * V + sin(Alpha) * (A ^ V) + (1 - cos(Alpha)) * (A * V) * A;
    return V_rot;
}



void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();

    // Rotation autour l'origine
    // Pour faire une rotation autour une axe il faut 
    // faire la translation d'abord et apres la rotation
    glRotated(anim.getTheta(), 1, 0, 0);
    glRotated(anim.getPhi(), 0, 0, 1);
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);
}


Sphere::Sphere(double r, Color cl, Point org)
{
    radius = r;
    col = cl;
    //anim.setPos(org);
}


void Sphere::update(double delta_t)
{   
    const Vector g(0, -9.8, 0);
    const Vector v1(1, 0, 0);
    const Vector v2(0, 0, 1);
    Point org = anim.getPos();
    org.translate(anim.getSpeed());
    anim.setPos(org);
    Vector a = g * Rotation(v1, v2, anim.getPhi()) * Rotation(v1, v2, anim.getPhi()) + g * Rotation(v2, v1, anim.getTheta()) * Rotation(v2, v1, anim.getTheta());
    anim.setAccel(0.0001 * a);
    anim.setSpeed(anim.getSpeed() * Rotation(v1, v2, anim.getPhi()) * Rotation(v1, v2, anim.getPhi()) + anim.getSpeed() * Rotation(v2, v1, anim.getTheta()) * Rotation(v2, v1, anim.getTheta()) + anim.getAccel());
    // Mise à jour de la position en fonction de la vitesse

}


void Sphere::render()
{
    GLUquadric* quad;

    quad = gluNewQuadric();
    Form::render();
    gluSphere(quad, radius, 32, 32);  // On se prend pas la tete, c'est pour dessiner la sphere

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



