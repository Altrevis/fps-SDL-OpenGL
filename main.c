#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdio.h>

float camX1 = 0.0f, camY1 = 0.0f, camZ1 = -5.0f;  // Position de la caméra du joueur 1
float camX2 = 0.0f, camY2 = 0.0f, camZ2 = -5.0f;  // Position de la caméra du joueur 2
float camSpeed = 0.1f;  // Vitesse de déplacement
float jumpHeight = 0.3f; // Hauteur du saut
int isJumping1 = 0, isJumping2 = 0;  // Flags pour savoir si chaque joueur saute

void movePlayer1(const Uint8 *keystate) {
    if (keystate[SDL_SCANCODE_W]) {
        camZ1 -= camSpeed;  // Avance (W)
    }
    if (keystate[SDL_SCANCODE_S]) {
        camZ1 += camSpeed;  // Recule (S)
    }
    if (keystate[SDL_SCANCODE_A]) {
        camX1 -= camSpeed;  // Aller à gauche (A)
    }
    if (keystate[SDL_SCANCODE_D]) {
        camX1 += camSpeed;  // Aller à droite (D)
    }
    if (keystate[SDL_SCANCODE_SPACE] && !isJumping1) {
        isJumping1 = 1;  // Activer le saut pour joueur 1
    }

    // Saut pour le joueur 1
    if (isJumping1) {
        camY1 += jumpHeight;
        if (camY1 >= 1.0f) {
            isJumping1 = 0;  // Fin du saut
        }
    } else if (camY1 > 0.0f) {
        camY1 -= jumpHeight;  // Redescendre le joueur 1
    }
}

void movePlayer2(const Uint8 *keystate) {
    if (keystate[SDL_SCANCODE_UP]) {
        camZ2 -= camSpeed;  // Avance (flèche haut)
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        camZ2 += camSpeed;  // Recule (flèche bas)
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        camX2 -= camSpeed;  // Aller à gauche (flèche gauche)
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        camX2 += camSpeed;  // Aller à droite (flèche droite)
    }
    if (keystate[SDL_SCANCODE_RCTRL] && !isJumping2) {
        isJumping2 = 1;  // Activer le saut pour joueur 2
    }

    // Saut pour le joueur 2
    if (isJumping2) {
        camY2 += jumpHeight;
        if (camY2 >= 1.0f) {
            isJumping2 = 0;  // Fin du saut
        }
    } else if (camY2 > 0.0f) {
        camY2 -= jumpHeight;  // Redescendre le joueur 2
    }
}

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

        // Gestion des touches pour chaque joueur
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        movePlayer1(keystate);
        movePlayer2(keystate);

        // Limiter les déplacements des joueurs
        if (camX1 > 2.0f) camX1 = 2.0f;
        if (camX1 < -2.0f) camX1 = -2.0f;
        if (camY1 > 2.0f) camY1 = 2.0f;
        if (camY1 < -2.0f) camY1 = -2.0f;
        if (camZ1 > -3.0f) camZ1 = -3.0f;
        if (camZ1 < -10.0f) camZ1 = -10.0f;

        if (camX2 > 2.0f) camX2 = 2.0f;
        if (camX2 < -2.0f) camX2 = -2.0f;
        if (camY2 > 2.0f) camY2 = 2.0f;
        if (camY2 < -2.0f) camY2 = -2.0f;
        if (camZ2 > -3.0f) camZ2 = -3.0f;
        if (camZ2 < -10.0f) camZ2 = -10.0f;

        // Effacer l'écran et le tampon de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Vue pour le joueur 1
        glViewport(0, 0, 400, 600);  // Diviser l'écran (gauche)
        glLoadIdentity();
        gluLookAt(camX1, camY1, camZ1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Dessiner le cube pour le joueur 1
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);  // Rouge
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glEnd();

        // Vue pour le joueur 2
        glViewport(400, 0, 400, 600);  // Diviser l'écran (droite)
        glLoadIdentity();
        gluLookAt(camX2, camY2, camZ2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Dessiner le cube pour le joueur 2
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);  // Bleu
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glEnd();

        // Échanger les buffers pour afficher
        SDL_GL_SwapWindow(window);
    }

    // Libérer les ressources et quitter
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
