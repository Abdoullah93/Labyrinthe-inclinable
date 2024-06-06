#include <iostream>
#include <cmath>

//Using SDL, SDL OpenGL, GLEW
#include <SDL.h>
#include <gl\glew.h>  // glu.h included
#include <SDL_opengl.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"


using namespace std;


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 20;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos);

// Frees media and shuts down SDL
void close(SDL_Window** window);

Vector Rotation(Vector V, Vector A, double Alpha);

void collision(Sphere& Balle, Cube_face& mur);
// Definition des parametres des objets et de l'environnement
// Dans le code il faut faire reference a ces constants la, PAS DE VALEURS NUMERIQUES
// (pour le calcul de l'origine par exemple)
const double P_length = 3;
const double P_width = 2;
const double B_radius = 0.1;
const Color P_color = RED;
const Color B_color = BLUE;
const int P_Omega_normalise = 5;
const int P_incline_limit = 30;
const double Bord_width = 0.2;
const Vector g(0, -9.8, 0);



/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		// Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		// Create window
		*window = SDL_CreateWindow("TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (*window == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			// Create context
			*context = SDL_GL_CreateContext(*window);
			if (*context == NULL)
			{
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				// Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
				}

				// Initialize OpenGL
				if (!initGL())
				{
					std::cout << "Unable to initialize OpenGL!" << std::endl;
					success = false;
				}
			}
		}
	}

	return success;
}


bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	// Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport : use all the window to display the rendered scene
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Fix aspect ratio and depth clipping planes
	gluPerspective(40.0, (GLdouble)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 100.0);


	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Initialize clear color : black with no transparency
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Activate Z-Buffer
	glEnable(GL_DEPTH_TEST);


	// Lighting basic configuration and activation
	const GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

	const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[] = { 100.0f };

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


	// Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "Error initializing OpenGL!  " << gluErrorString(error) << std::endl;
		success = false;
	}

	return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
	// Update the list of forms
	unsigned short i = 0;
	while (formlist[i] != NULL)
	{
		formlist[i]->update(delta_t);
		i++;
	}
}

void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos)
{
	// Clear color buffer and Z-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the camera position and parameters
	gluLookAt(cam_pos.x, cam_pos.y, cam_pos.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Isometric view
	glRotated(-45, 0, 1, 0);
	glRotated(30, 1, 0, -1);

	// X, Y and Z axis
	glPushMatrix(); // Preserve the camera viewing point for further forms
	// Render the coordinates system
	glBegin(GL_LINES);
	{
		// X rouge
		// Y vert
		// Z bleue
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(1, 0, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 1, 0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 1);
	}
	glEnd();
	glPopMatrix(); // Restore the camera viewing point for next object

	// Render the list of forms
	unsigned short i = 0;
	while (formlist[i] != NULL)
	{
		glPushMatrix(); // Preserve the camera viewing point for further forms
		formlist[i]->render();
		glPopMatrix(); // Restore the camera viewing point for next object
		i++;
	}
}

void close(SDL_Window** window)
{
	//Destroy window
	SDL_DestroyWindow(*window);
	*window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
	// The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	// OpenGL context
	SDL_GLContext gContext;


	// Start up SDL and create window
	if (!init(&gWindow, &gContext))
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else
	{
		// Main loop flag
		bool quit = false;
		Uint32 current_time, previous_time, elapsed_time;

		// Event handler
		SDL_Event event;

		// Camera position
		Point camera_position(0, 0.0, 5.0);

		// The forms to render
		Form* forms_list[MAX_FORMS_NUMBER];
		unsigned short number_of_forms = 0, i;
		for (i = 0; i < MAX_FORMS_NUMBER; i++)
		{
			forms_list[i] = NULL;
		}
		// Create here specific forms and add them to the list...
		// Don't forget to update the actual number_of_forms ! 

		// Creer le plateau centré à l'origine (pour l'instant c'est une planche)
		Cube_face* Plateau = NULL;
		Vector v1(1, 0, 0);
		Vector v2(0, 0, 1);
		Plateau = new Cube_face(v1, v2, Point(0, 0, 0), P_width, P_length, P_color);
		forms_list[number_of_forms] = Plateau;
		number_of_forms++;

		// Creer les murs du plateau
		Cube_face* Mur1 = NULL;
		Mur1 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(0, Bord_width/2, -P_length / 2), P_width, Bord_width, GREEN);
		forms_list[number_of_forms] = Mur1;
		number_of_forms++;

		Cube_face* Mur2 = NULL;
		Mur2 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(0, Bord_width / 2, P_length / 2), P_width, Bord_width, GREEN);
		forms_list[number_of_forms] = Mur2;
		number_of_forms++;

		Cube_face* Mur3 = NULL;
		Mur3 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(-P_width / 2, Bord_width / 2, 0), P_length, Bord_width, GREEN);
		forms_list[number_of_forms] = Mur3;
		number_of_forms++;

		Cube_face* Mur4 = NULL;
		Mur4 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(P_width / 2, Bord_width / 2, 0), P_length, Bord_width, GREEN);
		forms_list[number_of_forms] = Mur4;
		number_of_forms++;

		Cube_face* murs_list[4] = { Mur1, Mur2, Mur3, Mur4 };
		// Creer la balle et la placer sur le plateau (une vitesse et une acceleration sont données pour tester)
		Sphere* Balle = NULL;
		Balle = new Sphere(B_radius, B_color);
		Balle->getAnim().setPos(Point (0.5, B_radius, 0.5));

		//Création de 6 murs pour le labyrinthe
		Cube_face* Mur5 = NULL;
		Mur5 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(-P_width/3, Bord_width / 2, -0.25 * P_length), P_length/2, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur5;
		number_of_forms++;

		Cube_face* Mur6 = NULL;
		Mur6 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(-P_width/12, Bord_width / 2, 0), P_width*0.5, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur6;
		number_of_forms++;

		Cube_face* Mur7 = NULL;
		Mur7 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(P_width/6, Bord_width / 2, -P_length/6), P_length*0.33, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur7;
		number_of_forms++;

		Cube_face* Mur8 = NULL;
		Mur8 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(2*P_width/6, Bord_width / 2, -P_length/6), 4*P_length/6, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur8;
		number_of_forms++;

		Cube_face* Mur9 = NULL;
		Mur9 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(0, Bord_width / 2, P_length/6), 4*P_width/6, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur9;
		number_of_forms++;

		Cube_face* Mur10 = NULL;
		Mur10 = new Cube_face(Vector(0, 0, 1), Vector(0, 1, 0), Point(-2*P_width/6, Bord_width / 2, 3 * P_length / 12), 2*P_length/12, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur10;
		number_of_forms++;

		Cube_face* Mur11 = NULL;
		Mur11 = new Cube_face(Vector(1, 0, 0), Vector(0, 1, 0), Point(0, Bord_width / 2, 2*P_length / 6), 4 * P_width / 6, Bord_width, WHITE);
		forms_list[number_of_forms] = Mur11;
		number_of_forms++;

		Cube_face* labyrinthe_list[7] = { Mur5, Mur6, Mur7, Mur8, Mur9, Mur10, Mur11 };
		
		Vector* B_Speed = new Vector(0, 0, 0);
		Balle->getAnim().setSpeed(*B_Speed);

		Vector* B_Accel = new Vector(0, 0, 0);
		Balle->getAnim().setAccel(*B_Accel);
		Balle->getAnim().setPos(Point(0, B_radius, P_length/4));
		
		forms_list[number_of_forms] = Balle;
		number_of_forms++;


		// Get first "current time"
		previous_time = SDL_GetTicks();
		// While application is running
		while (!quit)
		{
			// Handle events on queue
			while (SDL_PollEvent(&event) != 0)
			{
				int x = 0, y = 0;
				SDL_Keycode key_pressed = event.key.keysym.sym;


				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					Point pos = Balle->getAnim().getPos();
					Vector pos_vecteur(Point(0, 0, 0), pos);
					Point new_pos(0, 0, 0);
					//Plateau->getAnim().setTheta(Plateau->getAnim().getTheta() - P_Omega_normalise);
					Plateau->setv2(Rotation(Plateau->getv2(), Vector(1, 0, 0), -P_Omega_normalise));
					Plateau->setv1(Rotation(Plateau->getv1(), Vector(1, 0, 0), -P_Omega_normalise));
					//Balle->getAnim().setTheta(Balle->getAnim().getTheta() - P_Omega_normalise);

					for (int i = 0; i < 4; i++)
					{
						murs_list[i]->setv2(Rotation(murs_list[i]->getv2(), Vector(1, 0, 0), -P_Omega_normalise));
						murs_list[i]->setv1(Rotation(murs_list[i]->getv1(), Vector(1, 0, 0), -P_Omega_normalise));
					}
					for (int i = 0; i < 7; i++)
					{
						labyrinthe_list[i]->setv2(Rotation(labyrinthe_list[i]->getv2(), Vector(1, 0, 0), -P_Omega_normalise));
						labyrinthe_list[i]->setv1(Rotation(labyrinthe_list[i]->getv1(), Vector(1, 0, 0), -P_Omega_normalise));
					}

					pos_vecteur = Rotation(pos_vecteur, Vector(1, 0, 0), -P_Omega_normalise);
					new_pos.translate(pos_vecteur);
					Balle->getAnim().setPos(new_pos);

					// Revoir height pour qu'il soit correct
					Vector height = Bord_width / 2 * Plateau->getv2() ^ Plateau->getv1();
					Mur1->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * -0.5 * Plateau->getv2()) + height);
					Mur2->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 0.5 * Plateau->getv2()) + height);
					Mur3->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur4->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur5->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 3) * Plateau->getv1() + (Mur3->getLength() * -0.25) * Plateau->getv2() + height);
					Mur6->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 12) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur7->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 1 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur8->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 2 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur9->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 1 / 6) * Plateau->getv2() + height);
					Mur10->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -2 / 6) * Plateau->getv1() + (Mur3->getLength() * 3 / 12) * Plateau->getv2() + height);
					Mur11->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 2 / 6) * Plateau->getv2() + height);

				}
				if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					Point pos = Balle->getAnim().getPos();
					Vector pos_vecteur(Point(0, 0, 0), pos);
					Point new_pos(0, 0, 0);
					//Plateau->getAnim().setTheta(Plateau->getAnim().getTheta() + P_Omega_normalise);
					Plateau->setv2(Rotation(Plateau->getv2(), Vector(1, 0, 0), P_Omega_normalise));
					Plateau->setv1(Rotation(Plateau->getv1(), Vector(1, 0, 0), P_Omega_normalise));

					//Balle->getAnim().setTheta(Balle->getAnim().getTheta() + P_Omega_normalise);

					pos_vecteur = Rotation(pos_vecteur, Vector(1, 0, 0), P_Omega_normalise);

					new_pos.translate(pos_vecteur);
					Balle->getAnim().setPos(new_pos);

					for (int i = 0; i < 4; i++)
					{
						murs_list[i]->setv2(Rotation(murs_list[i]->getv2(), Vector(1, 0, 0), P_Omega_normalise));
						murs_list[i]->setv1(Rotation(murs_list[i]->getv1(), Vector(1, 0, 0), P_Omega_normalise));
					}

					for (int i = 0; i < 7; i++)
					{
						labyrinthe_list[i]->setv2(Rotation(labyrinthe_list[i]->getv2(), Vector(1, 0, 0), P_Omega_normalise));
						labyrinthe_list[i]->setv1(Rotation(labyrinthe_list[i]->getv1(), Vector(1, 0, 0), P_Omega_normalise));
					}
					Vector height = Bord_width / 2 * Plateau->getv2() ^ Plateau->getv1();
					Mur1->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * -0.5 * Plateau->getv2()) + height);
					Mur2->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 0.5 * Plateau->getv2()) + height);
					Mur3->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur4->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur5->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 3) * Plateau->getv1() + (Mur3->getLength() * -0.25) * Plateau->getv2() + height);
					Mur6->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 12) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur7->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 1 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur8->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 2 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur9->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 1 / 6) * Plateau->getv2() + height);
					Mur10->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -2 / 6) * Plateau->getv1() + (Mur3->getLength() * 3 / 12) * Plateau->getv2() + height);
					Mur11->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 2 / 6) * Plateau->getv2() + height);
				}
				if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					Point pos = Balle->getAnim().getPos();
					Vector pos_vecteur(Point(0, 0, 0), pos);
					Point new_pos(0, 0, 0);
					//Plateau->getAnim().setPhi(Plateau->getAnim().getPhi() + P_Omega_normalise);
					Plateau->setv1(Rotation(Plateau->getv1(), Vector(0, 0, 1), P_Omega_normalise));
					Plateau->setv2(Rotation(Plateau->getv2(), Vector(0, 0, 1), P_Omega_normalise));
					//Balle->getAnim().setPhi(Balle->getAnim().getPhi() + P_Omega_normalise);

					pos_vecteur = Rotation(pos_vecteur, Vector(0, 0, 1), P_Omega_normalise);

					new_pos.translate(pos_vecteur);
					Balle->getAnim().setPos(new_pos);


					for (int i = 0; i < 4; i++)
					{
						murs_list[i]->setv1(Rotation(murs_list[i]->getv1(), Vector(0, 0, 1), P_Omega_normalise));
						murs_list[i]->setv2(Rotation(murs_list[i]->getv2(), Vector(0, 0, 1), P_Omega_normalise));
					}

					for (int i = 0; i < 7; i++)
					{
						labyrinthe_list[i]->setv2(Rotation(labyrinthe_list[i]->getv2(), Vector(0, 0, 1), P_Omega_normalise));
						labyrinthe_list[i]->setv1(Rotation(labyrinthe_list[i]->getv1(), Vector(0, 0, 1), P_Omega_normalise));
					}

					Vector height = Bord_width / 2 * Plateau->getv2() ^ Plateau->getv1();
					Mur1->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * -0.5 * Plateau->getv2()) + height);
					Mur2->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 0.5 * Plateau->getv2()) + height);
					Mur3->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur4->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur5->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 3) * Plateau->getv1() + (Mur3->getLength() * -0.25) * Plateau->getv2() + height);
					Mur6->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 12) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur7->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 1 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur8->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 2 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur9->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 1 / 6) * Plateau->getv2() + height);
					Mur10->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -2 / 6) * Plateau->getv1() + (Mur3->getLength() * 3 / 12) * Plateau->getv2() + height);
					Mur11->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 2 / 6) * Plateau->getv2() + height);
				}
				if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					Point pos = Balle->getAnim().getPos();
					Vector pos_vecteur(Point(0, 0, 0), pos);
					Point new_pos(0, 0, 0);
					//Plateau->getAnim().setPhi(Plateau->getAnim().getPhi() - P_Omega_normalise);
					Plateau->setv1(Rotation(Plateau->getv1(), Vector(0, 0, 1), -P_Omega_normalise));
					Plateau->setv2(Rotation(Plateau->getv2(), Vector(0, 0, 1), -P_Omega_normalise));
					//Balle->getAnim().setPhi(Balle->getAnim().getPhi() - P_Omega_normalise);

					pos_vecteur = Rotation(pos_vecteur, Vector(0, 0, 1), -P_Omega_normalise);

					new_pos.translate(pos_vecteur);
					Balle->getAnim().setPos(new_pos);

					for (int i = 0; i < 4; i++)
					{
						murs_list[i]->setv1(Rotation(murs_list[i]->getv1(), Vector(0, 0, 1), -P_Omega_normalise));
						murs_list[i]->setv2(Rotation(murs_list[i]->getv2(), Vector(0, 0, 1), -P_Omega_normalise));
					}

					for (int i = 0; i < 7; i++)
					{
						labyrinthe_list[i]->setv2(Rotation(labyrinthe_list[i]->getv2(), Vector(0, 0, 1), -P_Omega_normalise));
						labyrinthe_list[i]->setv1(Rotation(labyrinthe_list[i]->getv1(), Vector(0, 0, 1), -P_Omega_normalise));
					}

					Vector height = Bord_width / 2 * Plateau->getv2() ^ Plateau->getv1();
					Mur1->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * -0.5 * Plateau->getv2()) + height);
					Mur2->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 0.5 * Plateau->getv2()) + height);
					Mur3->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur4->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0.5) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur5->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 3) * Plateau->getv1() + (Mur3->getLength() * -0.25) * Plateau->getv2() + height);
					Mur6->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -1 / 12) * Plateau->getv1() + (Mur3->getLength() * 0) * Plateau->getv2() + height);
					Mur7->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 1 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur8->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 2 / 6) * Plateau->getv1() + (Mur3->getLength() * -1 / 6) * Plateau->getv2() + height);
					Mur9->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 1 / 6) * Plateau->getv2() + height);
					Mur10->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * -2 / 6) * Plateau->getv1() + (Mur3->getLength() * 3 / 12) * Plateau->getv2() + height);
					Mur11->getAnim().setPos(Plateau->getAnim().getPos() + (Mur1->getLength() * 0) * Plateau->getv1() + (Mur3->getLength() * 2 / 6) * Plateau->getv2() + height);
				}
				if (currentKeyStates[SDL_SCANCODE_F])
				{
				}
				if (currentKeyStates[SDL_SCANCODE_S])
				{
				}
				if (currentKeyStates[SDL_SCANCODE_E])
				{
				}
				if (currentKeyStates[SDL_SCANCODE_D])
				{
				}
				if (currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					quit = true;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				collision(*Balle, *murs_list[i]);
			}
			for (int i = 0; i < 7; i++)
			{
				collision(*Balle, *labyrinthe_list[i]);
			}
			Vector v1_rotated = Plateau->getv1();
			Vector v2_rotated = Plateau->getv2();
			Vector a = (g * v1_rotated) * v1_rotated + (g * v2_rotated) * v2_rotated;
			Balle->getAnim().setAccel(a);
			Balle->getAnim().setSpeed(Balle->getAnim().getSpeed()* v1_rotated* v1_rotated + Balle->getAnim().getSpeed() * v2_rotated * v2_rotated + Balle->getAnim().getAccel());

			

			// Update the scene
			current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
			elapsed_time = current_time - previous_time;
			if (elapsed_time > ANIM_DELAY)
			{
				previous_time = current_time;
				update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
			}



			// Render the scene
			render(forms_list, camera_position);

			// Update window screen
			SDL_GL_SwapWindow(gWindow);
		}
	}

	// Free resources and close SDL
	close(&gWindow);

	return 0;
}

