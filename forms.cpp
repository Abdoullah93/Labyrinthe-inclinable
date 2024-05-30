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


Sphere::Sphere(double r, Color cl, Point org)
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

Plateau::Plateau(double size, Color cl)
{
    Point origin(0, 0, 0);
    double half_size = size / 2.0;

    
    // Bottom face
    faces.push_back(Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(-half_size, -half_size, -half_size), size, size, cl));
    // Top face
    faces.push_back(Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(-half_size, half_size, -half_size), size, size, cl));
    // Front face
    faces.push_back(Cube_face(Vector(1, 0, 0), Vector(0, 0, 1), Point(-half_size, -half_size, -half_size), size, size, cl));
    // Back face
    faces.push_back(Cube_face(Vector(1, 0, 0), Vector(0, 0, 1), Point(-half_size, -half_size, half_size), size, size, cl));
    // Left face
    faces.push_back(Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(-half_size, -half_size, -half_size), size, size, cl));
    // Right face
    faces.push_back(Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(half_size, -half_size, -half_size), size, size, cl));
}

void Plateau::update(double delta_t)
{
    for (auto& face : faces)
    {
        face.update(delta_t);
    }
}

void Plateau::render()
{
    for (auto& face : faces)
    {
        face.render();
    }
}
