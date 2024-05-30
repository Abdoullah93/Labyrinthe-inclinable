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

    // Rotation autour l'origine
    // Pour faire une rotation autour une axe il faut 
    // faire la translation d'abord et apres la rotation
    glRotated(anim.getTheta(), 1, 0, 0);
    glRotated(anim.getPhi(), 0, 0, 1);
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);

}


Sphere::Sphere(double r, Color cl)
{
    radius = r;
    col = cl;
    //anim.setPos(org);
}


void Sphere::update(double delta_t)
{
    anim.setSpeed(anim.getSpeed() + anim.getAccel());
    // Mise � jour de la position en fonction de la vitesse
    Point org = anim.getPos();
    org.translate(anim.getSpeed());
    anim.setPos(org);
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

void Cube_face::setHole(Hole* h)
{
	hole = h;
}

void Cube_face::update(double delta_t)
{

}


void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length * vdir1);
    p3 = p2;
    p3.translate(width * vdir2);
    p4.translate(width * vdir2);

    Form::render();

    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();

    // Render the hole if it exists

    if (hole != nullptr) {
        glPushMatrix();
        // Translate to the hole's position relative to the cube face
        glTranslatef(hole->getPosition().x, hole->getPosition().y, hole->getPosition().z);
        hole->render();
        glPopMatrix();
    }
    
}

Hole :: Hole(Point pos, double r) 
{
	position = pos;
	radius = r;
}

void Hole::render()
{
    GLUquadric* quad;
    quad = gluNewQuadric();
    Form::render();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);// Rotate the disk 90 degrees around the x-axis to make it horizontal
    glColor3f(0.0f, 0.0f, 0.0f); // the hole is black
    gluDisk(quad, 0, radius, 32, 1);
    gluDeleteQuadric(quad);
}
