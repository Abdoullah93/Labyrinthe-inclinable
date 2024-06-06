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


void collision(Sphere& Balle, Cube_face& mur, double delta_t) {
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
    //printf("Proj_length : %f        Balle_pos_m.norm() : %f      mur.getLength : %f     dist_balle_mur  : %f \n", Proj_length, Balle_pos_m.norm(), mur.getLength(), dist_balle_mur);
    if (Proj_length < mur.getLength() / 2 && dist_balle_mur < Balle.getRadius() && V0 * Vector(mur.getAnim().getPos(), Point(0, 0, 0)) < 0) {
        printf("collision detected \n");

        Vector newSpeed = 0.5 * ((V0 * mur.getv2()) * mur.getv2() + (V0 * mur.getv1()) * mur.getv1() - (V0 * Normale_mur) * Normale_mur);
        Point new_Balle_pos = Balle.getAnim().getPos();
        new_Balle_pos.translate(0.01 * newSpeed.integral(delta_t));
        Vector new_balle_vect(mur.getAnim().getPos(), new_Balle_pos);

        double Proj_length = abs(new_balle_vect * mur.getv1());
        double dist_balle_mur = sqrt(pow(new_balle_vect.norm(), 2) - pow(Proj_length, 2));
        if (Proj_length < mur.getLength() / 2 && dist_balle_mur < Balle.getRadius()) {
            new_balle_vect = (Balle.getRadius() / dist_balle_mur) * new_balle_vect;
            Point newPos = Balle.getAnim().getPos();
            newPos.translate(new_balle_vect);
            Balle.getAnim().setPos(newPos);
        }

        Balle.getAnim().setSpeed(newSpeed);
        Balle.getAnim().setAccel(Balle.getAnim().getAccel() - (Balle.getAnim().getAccel() * Normale_mur) * Normale_mur);
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
}


void Sphere::update(double delta_t)
{   
    const Vector g(0, -9.8, 0);
    const Vector v1(1, 0, 0);
    const Vector v2(0, 0, 1);
    Point org = anim.getPos();

    //Vector v1_rotated = Rotation(v1, v2, anim.getPhi());
    //Vector v2_rotated = Rotation(v2, v1, anim.getTheta());
    //Vector a = (g * v1_rotated) * v1_rotated + (g * v2_rotated) * v2_rotated;
    //anim.setAccel(0.001*a);
    //anim.setSpeed(anim.getSpeed() * v1_rotated * v1_rotated + anim.getSpeed() * v2_rotated * v2_rotated + anim.getAccel());
    org.translate(0.01 * anim.getSpeed().integral(delta_t));
    anim.setPos(org);
    //anim.setSpeed(anim.getSpeed() + anim.getAccel());

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




Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
    type = CUBE_FACE;
    init_org = org;
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
    glPushMatrix();
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

    if (hole != nullptr) {

        double x = hole->getPosition().x;
        double y = hole->getPosition().y;
        Vector holeOriginInNewBase = x * vdir1 + y * vdir2 + 0.001 * (vdir2 ^ vdir1);
        Point holeOrigin(holeOriginInNewBase.x, holeOriginInNewBase.y, holeOriginInNewBase.z);

        // Translate to the hole's position relative to the cube face
        glTranslatef(holeOrigin.x, holeOrigin.y, holeOrigin.z);

        // Rotate the disk to be parallel with the cube face
        // Adjust the axis and angle based on your cube face orientation
       // glRotatef(90, 0.0f, 0.0f, 1.0f);

        // Set the color for the hole (black)
        glColor3f(0.0f, 0.0f, 0.0f);

        // Draw the filled disk
        glBegin(GL_TRIANGLE_FAN);
        {
            //glVertex3f(0.0f, 0.0f, 0.0f); // Center of the disk
            for (int i = 0; i <= 360; i++) {
                float angle = i * M_PI / 180.0f;
                float x = hole->getRadius() * cos(angle);
                float y = hole->getRadius() * sin(angle);
                Point bord = holeOrigin;
                bord.translate(x * vdir1);
                bord.translate(y * vdir2);
                glVertex3f(bord.x, bord.y, bord.z);
            }
        }
        glEnd();

    }
    glPopMatrix();

}

Hole::Hole(Point pos, double r)
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




