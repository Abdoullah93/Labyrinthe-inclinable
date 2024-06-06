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


void collision(Sphere& Balle, Cube_face& mur) {
    //Par convention le vecteur v2 du mur est celui qui est orthogonal au plan
    
    Vector Mur_length_v = mur.getLength() * mur.getv1();
    //Point org = mur.getOrg();
    //printf("Point coordinates: x: %.2f, y: %.2f, z: %.2f\n", org.x, org.y, org.z);
    Vector Balle_pos_m(mur.getAnim().getPos(), Balle.getAnim().getPos());
    double Proj_length = abs(Balle_pos_m * mur.getv1());
    double dist_balle_mur = sqrt(pow(Balle_pos_m.norm(), 2) - pow(Proj_length, 2));
    Vector Normale_mur = mur.getv1() ^ mur.getv2();
    Vector V0 = Balle.getAnim().getSpeed();
    //printf("%f \n", V0 * Normale_mur);
    printf("Proj_length : %f        Balle_pos_m.norm() : %f      mur.getLength : %f     dist_balle_mur  : %f \n", Proj_length, Balle_pos_m.norm(), mur.getLength(), dist_balle_mur);
    if (Proj_length < mur.getLength() / 2 && dist_balle_mur < Balle.getRadius() && V0 * Vector(mur.getAnim().getPos(), Point(0, 0, 0))<0) {
        printf("collision detected \n");

        Vector newSpeed = 0.5 * ((V0 * mur.getv2()) * mur.getv2() + (V0 * mur.getv1()) * mur.getv1() - (V0 * Normale_mur) * Normale_mur);
        Point Balle_new_pos = Balle.getAnim().getPos();
        Balle_new_pos.translate(newSpeed);

        Balle.getAnim().setSpeed(newSpeed);
        Balle.getAnim().setAccel(Vector(0, 0, 0));
    }

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
    type = SPHERE;
    //anim.setPos(org);
}


void Sphere::update(double delta_t)
{   
    //const Vector g(0, -9.8, 0);
    //const Vector v1(1, 0, 0);
    //const Vector v2(0, 0, 1);
    Point org = anim.getPos();


    org.translate(0.01*anim.getSpeed().integral(delta_t));
    anim.setPos(org);
    //anim.setSpeed(anim.getSpeed() + anim.getAccel());

    /*
    Point pos = anim.getPos();
    Vector pos_vecteur(Point(0, 0, 0), pos);
    
    
    pos_vecteur = Rotation(Rotation(pos_vecteur, v2, anim.getPhi()), v1, anim.getTheta());
    anim.setTheta(0);
    anim.setPhi(0);
    
    Point new_pos(0, 0, 0);
    
    new_pos.translate(pos_vecteur);
    printf("x : %f      y : %f      z : %f \n", new_pos.x, new_pos.y, new_pos.z);
    new_pos.translate(anim.getSpeed());


    anim.setPos(new_pos);
    //printf("%f \n", anim.getSpeed() * (v1_rotated ^ v2_rotated));
    // Mise à jour de la position en fonction de la vitesse
    */
}


void Sphere::render()
{
    GLUquadric* quad;

    quad = gluNewQuadric();
    
    
    Form::render();
    //Point org = anim.getPos();
    //glTranslated(org.x, org.y, org.z);
    //glColor3f(col.r, col.g, col.b);

    gluSphere(quad, radius, 32, 32);  // On se prend pas la tete, c'est pour dessiner la sphere

    gluDeleteQuadric(quad);
}

/*
Vector Sphere::getPosVector()
{
    Point pos = getAnim().getPos();
    Vector pos_vecteur(Point(0, 0, 0), pos);
    pos_vecteur = Rotation(pos_vecteur, Vector(1, 0, 0), -P_Omega_normalise);
    Point new_pos(0, 0, 0);
    new_pos.translate(pos_vecteur);
    Balle->getAnim().setPos(new_pos);
}
*/



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
    Vector v1 = getv1();
    Vector v2 = getv2();
    //printf("%f \n", v1 * v2);
}

/*
void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    Form::render();
    //Point org = anim.getPos();

    // Rotation autour l'origine
    // Pour faire une rotation autour une axe il faut 
    // faire la translation d'abord et apres la rotation
    //glRotated(anim.getTheta(), 1, 0, 0);
    //glRotated(anim.getPhi(), 0, 0, 1);
    //glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);



    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}
*/


void Cube_face::render()
{
    // Initialize points at the center
    Point p1 = Point();
    Point p2 = Point();
    Point p3 = Point();
    Point p4 = Point();

    // Translate points from the center
    p1.translate(-length / 2 * vdir1);
    p1.translate(-width / 2 * vdir2);

    p2.translate(length / 2 * vdir1);
    p2.translate(-width / 2 * vdir2);

    p3.translate(length / 2 * vdir1);
    p3.translate(width / 2 * vdir2);

    p4.translate(-length / 2 * vdir1);
    p4.translate(width / 2 * vdir2);

    Form::render();
    // Rotation around the origin
    // For rotation around an axis, translation must be done first, then rotation
    // glRotated(anim.getTheta(), 1, 0, 0);
    // glRotated(anim.getPhi(), 0, 0, 1);
    // glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);

    glBegin(GL_QUADS);
    {
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}




