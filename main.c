#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdio.h>

float camX = 0.0f;  // Position de la caméra (déplacement sur l'axe X)
float camY = 0.0f;  // Position de la caméra (déplacement sur l'axe Y)
float camZ = -5.0f;  // Position de la caméra sur l'axe Z (en avant)
float camSpeed = 0.1f;  // Vitesse de déplacement

int main(int argc, char *argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return -1;
    }

    // Créer une fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("FPS Camera",
                                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Erreur de création de la fenêtre SDL : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Créer un contexte OpenGL
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        printf("Erreur de création du contexte OpenGL : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialisation de GLEW
    if (glewInit() != GLEW_OK) {
        printf("Erreur GLEW\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Configuration de la perspective OpenGL
    glMatrixMode(GL_PROJECTION);   // Choisir la matrice de projection
    glLoadIdentity();              // Réinitialiser la matrice
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);  // Créer une projection en perspective
    glMatrixMode(GL_MODELVIEW);    // Revenir à la matrice modèle

    glEnable(GL_DEPTH_TEST);  // Activer la gestion de la profondeur

    // Boucle principale
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        // Gestion des événements SDL
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Gestion des touches ZQSD et Espace
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_S]) {
            camY += camSpeed;  // Déplacer la caméra vers le bas (S)
        }
        if (keystate[SDL_SCANCODE_W]) {
            camY -= camSpeed;  // Déplacer la caméra vers le haut (W)
        }
        if (keystate[SDL_SCANCODE_D]) {
            camX -= camSpeed;  // Déplacer la caméra vers la droite (D)
        }
        if (keystate[SDL_SCANCODE_A]) {
            camX += camSpeed;  // Déplacer la caméra vers la gauche (A)
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
            camZ += camSpeed;  // Déplacer la caméra vers l'avant (espace)
        }
        if (keystate[SDL_SCANCODE_LSHIFT]) {
            camZ -= camSpeed;  // Déplacer la caméra vers l'arrière (shift)
        }

        // Limiter les déplacements de la caméra
        if (camX > 2.0f) {
            camX = 2.0f;  // Limite maximale pour X
        } else if (camX < -2.0f) {
            camX = -2.0f;  // Limite minimale pour X
        }

        if (camY > 2.0f) {
            camY = 2.0f;  // Limite maximale pour Y
        } else if (camY < -2.0f) {
            camY = -2.0f;  // Limite minimale pour Y
        }

        if (camZ > -3.0f) {
            camZ = -3.0f;  // Limite maximale pour Z (ne pas trop rapprocher de la scène)
        } else if (camZ < -10.0f) {
            camZ = -10.0f;  // Limite minimale pour Z
        }

        // Effacer l'écran et le tampon de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Appliquer une transformation pour déplacer la caméra
        glLoadIdentity();  // Réinitialiser la matrice de transformation
        glTranslatef(camX, camY, camZ);  // Déplacer la caméra selon camX, camY et camZ

        // Dessiner un cube
        glBegin(GL_QUADS);  // Commencer à dessiner des quads (faces du cube)

        // Face avant
        glColor3f(1.0f, 0.0f, 0.0f);  // Rouge
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);

        // Ajouter d'autres faces ici...

        glEnd();  // Fin du dessin

        // Échanger les buffers pour afficher
        SDL_GL_SwapWindow(window);
    }

    // Libérer les ressources et quitter
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
