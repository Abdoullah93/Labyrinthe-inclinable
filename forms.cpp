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

    //Vector v1_rotated = Rotation(v1, v2, anim.getPhi());
    //Vector v2_rotated = Rotation(v2, v1, anim.getTheta());
    //Vector a = (g * v1_rotated) * v1_rotated + (g * v2_rotated) * v2_rotated;
    //anim.setAccel(0.001*a);
    //anim.setSpeed(anim.getSpeed() * v1_rotated * v1_rotated + anim.getSpeed() * v2_rotated * v2_rotated + anim.getAccel());
    org.translate(anim.getSpeed());
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

// Constructeur de la classe Wall
// Construit les 4 murs du plateau en fonction du point d'origine et de la taille du plateau
Wall::Wall(Cube_face base, double wall_height, Color cl)
{
    this->wall_height = wall_height;
    this->base = base;
    this->length_base = base.getLength();
    this->width_base = base.getWidth();
    double P_width = base.getLength();
    double P_length = base.getWidth();
    Point org = base.getAnim().getPos();

    // Creer les murs du plateau
    // Les murs 1 et 2 sont paralleles a l'axe x
    // Les murs 3 et 4 sont paralleles a l'axe z
    Cube_face* Mur1 = NULL;
    Point p1 = org + Vector(0, wall_height/2, -P_length/2);
    Mur1 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), p1, P_width, wall_height, WHITE);
    cout << "p1 : " << p1 << endl;
    cout << "GetPos() Mur1 : " << Mur1->getAnim().getPos() << endl;


    Cube_face* Mur2 = NULL;
    Point p2 = org + Vector(0, wall_height/2, P_length);
    Mur2 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), p2, P_width, wall_height, YELLOW);
    cout << "p2 : " << p2 << endl;
    cout << "GetPos() Mur2 : " << Mur2->getAnim().getPos() << endl;


    Cube_face* Mur3 = NULL;
    Point p3 = org + Vector(-P_width/2,wall_height/2,-P_length/2);
    Mur3 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), p3, P_length, wall_height, GREEN);
    cout << "p3 : " << p3 << endl;
    cout << "GetPos() Mur3 : " << Mur3->getAnim().getPos() << endl;

    Cube_face* Mur4 = NULL;
    Point p4 = org + Vector(P_width, wall_height/2, -P_length/2);
    Mur4 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), p4, P_length, wall_height, ORANGE);
    cout << "p4 : " << p4 << endl;
    cout << "GetPos() Mur4 : " << Mur4->getAnim().getPos() << endl;


    this->wall_list[0] = Mur1;
    this->wall_list[1] = Mur2;
    this->wall_list[2] = Mur3;
    this->wall_list[3] = Mur4;
     cout << "Wall_list[0] : " << this->wall_list[0]->getAnim().getPos() << endl;
     cout << "Wall_list[1] : " << this->wall_list[1]->getAnim().getPos() << endl;
     cout << "Wall_list[2] : " << this->wall_list[2]->getAnim().getPos() << endl;
     cout << "Wall_list[3] : " << this->wall_list[3]->getAnim().getPos() << endl;

	/*vdir1 = 1.0 / v1.norm() * v1;
	vdir2 = 1.0 / v2.norm() * v2;
	anim.setPos(org);
	length = l;
	width = w;
	col = cl;*/
}

void Wall::update(double delta_t)
{
	for (int i = 0; i < 4; i++)
	{
		wall_list[i]->update(delta_t);
	}
}

void Wall::render()
{
	for (int i = 0; i < 4; i++)
	{
		wall_list[i]->render();
	}
}

// Change la position des murs et effectue la rotation nécessaire 
// en fonction des vecteurs v1 et v2 du plateau et rotation en fonction de l'axe donnée en arg et de l'angle
void Wall::setWallPos(Vector vRot, double angle)
{
    // Rotation des murs du plateau en fonction de l'axe donnée en arg
    for (int i = 0; i < 4; i++)
    {
        this->wall_list[i]->setv2(Rotation(wall_list[i]->getv2(), vRot, angle));
        this->wall_list[i]->setv1(Rotation(wall_list[i]->getv1(), vRot, angle));
    }

    // Translation des murs du plateau en fonction de la position du plateau
    // Les murs 1 et 2 sont paralleles a l'axe x
    // Les murs 3 et 4 sont paralleles a l'axe z
    /*for (int i = 0; i < 4; i++)
	{
        double coeff = ((i == 0)||(i==2) ? -0.5 : 0.5);
        Vector v1_v2 = (i<2 ? this->base.getv2() : this->base.getv1());
        Vector v = this->base.getLength() * coeff * v1_v2 ;
        Point newPos = this->base.getOrg() + v;
		this->wall_list[i]->getAnim().setPos(newPos);
	}*/
    //this->wall_list[0]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[2]->getLength() * -0.5 * base.getv2()));
    //this->wall_list[1]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[2]->getLength() * 0.5 * base.getv2()));
    //this->wall_list[2]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * -0.5) * base.getv1());
    //this->wall_list[3]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * 0.5) * base.getv1());

    this->wall_list[0]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * 0) * base.getv1() + (this->wall_list[0]->getLength() * -0.5 * base.getv2()));
    this->wall_list[1]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * 0) * base.getv1() + (this->wall_list[0]->getLength() * 0.5 * base.getv2()));
    this->wall_list[2]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * -0.5) * base.getv1() + (this->wall_list[0]->getLength() * 0) * base.getv2());
    this->wall_list[3]->getAnim().setPos(base.getAnim().getPos() + (this->wall_list[0]->getLength() * 0.5) * base.getv1() + (this->wall_list[0]->getLength() * 0) * base.getv2());

}



