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


bool collisionHole(Sphere* ball, Cube_face* plateau) {

    Hole* hole = plateau->getHole();

    Vector holeGlobaleV = hole->getPosition().x * plateau->getv1() + hole->getPosition().y* plateau->getv2();
    Point holeOGlobal(holeGlobaleV.x, holeGlobaleV.y, holeGlobaleV.z);
	
    // Get the Global position of the sphere
    Point ballOrigine = ball->getAnim().getPos(); // Globa
    
    double Distance = distance(ballOrigine, holeOGlobal);

     if (Distance < hole->getRadius() + ball->getRadius()) {
         //cout << "collision" << endl;
         // 
        // Update the position of the sphere
        cout << "holePos.x: " << holeOGlobal.x << " holePos.y: " << holeOGlobal.y << "" << holeOGlobal << endl;
        Point newPos = holeOGlobal;
        newPos.translate(-hole->getRadius()*(plateau->getv1()^plateau->getv2()));
        cout<< "newPos: " << newPos.x << " " << newPos.y << " " << newPos.z << endl;
       
        ball->getAnim().setPos(newPos);
        ball->getAnim().setSpeed(Vector(0, 0, 0));
        //ball->getAnim().setAccel(Vector(0, -9.81, 0));
        return true;
    }
	return false;
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


Sphere::Sphere(double r, Color cl)
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
    anim.setSpeed(anim.getSpeed() + anim.getAccel());
    // Mise � jour de la position en fonction de la vitesse
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
/*
Plateau :: Plateau(double l, double w, Point org, Point holePos, double holeWidth, Color cl)
{
	length = l;
	width = w;
	col = cl;
    //Creer la base du plateau
    Cube_face* Base = NULL;
    Base = new Cube_face(Vector(1, 0, 0), Vector(0, 0, 1), org, -w/2, -l/2, ORANGE);
    bords[0] = Base;
    //Creer les murs du plateau
    Cube_face* Mur1 = NULL;
    Mur1 = new Cube_face(Vector(0, 1, 0), Vector(1, 0, 0), Point(-l/2,0,-w/2), w, 0.1, RED);
    bords[1] = Mur1;
    Cube_face* Mur2 = NULL;
    Mur2 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(org.x, org.y, org.z + l), w, 0.1, cl);
    bords[2] = Mur2;
    Cube_face* Mur3 = NULL;
    Mur3 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), org, l, 0.1, cl);
    bords[3] = Mur3;
    Cube_face* Mur4 = NULL;
    Mur4 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(org.x+w, org.y, org.z ), l, 0.1, cl);
    bords[4] = Mur4;

    //Creer un trous dans plateau
    Hole* hole1 = NULL;
    hole1 = new Hole(holePos, holeWidth);
    Base->setHole(hole1);

  
}


void Plateau:: update(double delta_t)
{
	for (int i = 0; i < 5; i++)
	{
	    bords[i]->update(delta_t);
	}
}


void Plateau::render()
{
	for (int i = 0; i < 5; i++)
	{
		bords[i]->render();
	}
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

void Cube_face::setHole(Hole* h)
{
	hole = h;
}

void Cube_face::update(double delta_t)
{
    Vector v1 = getv1();
    Vector v2 = getv2();

}


void Cube_face::render() {
    // Save the current transformation state
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

    glColor3f(col.r, col.g, col.b);

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
        
                double x = hole->getPosition().x;
                double y = hole->getPosition().y;
                Vector holeOriginInNewBase = x*vdir1 + y*vdir2 + 0.001 * (vdir2^vdir1);
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
    // Restore the previous transformation state
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

