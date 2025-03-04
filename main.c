#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h> // Pour rand()

float camX1 = 0.0f, camY1 = 0.0f, camZ1 = -5.0f; // Position de la caméra du joueur 1
float camX2 = 0.0f, camY2 = 0.0f, camZ2 = -5.0f; // Position de la caméra du joueur 2
float camSpeed = 0.1f;                           // Vitesse de déplacement
float jumpHeight = 0.3f;                         // Hauteur du saut
int isJumping1 = 0, isJumping2 = 0;              // Flags pour savoir si chaque joueur saute

// Variables pour les "pixels" tirés
float pixelX1 = 0.0f, pixelY1 = 0.0f, pixelZ1 = -5.0f; // Position du pixel pour le joueur 1
float pixelX2 = 0.0f, pixelY2 = 0.0f, pixelZ2 = -5.0f; // Position du pixel pour le joueur 2

// Variables pour l'ennemi
float enemyX = 0.0f, enemyY = 0.0f, enemyZ = -5.0f; // Position de l'ennemi

void movePlayer1(const Uint8 *keystate)
{
    if (keystate[SDL_SCANCODE_W])
    {
        camZ1 -= camSpeed; // Avance (W)
    }
    if (keystate[SDL_SCANCODE_S])
    {
        camZ1 += camSpeed; // Recule (S)
    }
    if (keystate[SDL_SCANCODE_A])
    {
        camX1 -= camSpeed; // Aller à gauche (A)
    }
    if (keystate[SDL_SCANCODE_D])
    {
        camX1 += camSpeed; // Aller à droite (D)
    }
    if (keystate[SDL_SCANCODE_SPACE] && !isJumping1)
    {
        isJumping1 = 1; // Activer le saut pour joueur 1
    }

    // Saut pour le joueur 1
    if (isJumping1)
    {
        camY1 += jumpHeight;
        if (camY1 >= 1.0f)
        {
            isJumping1 = 0; // Fin du saut
        }
    }
    else if (camY1 > 0.0f)
    {
        camY1 -= jumpHeight; // Redescendre le joueur 1
    }
}

void movePlayer2(const Uint8 *keystate)
{
    if (keystate[SDL_SCANCODE_UP])
    {
        camZ2 -= camSpeed; // Avance (flèche haut)
    }
    if (keystate[SDL_SCANCODE_DOWN])
    {
        camZ2 += camSpeed; // Recule (flèche bas)
    }
    if (keystate[SDL_SCANCODE_LEFT])
    {
        camX2 -= camSpeed; // Aller à gauche (flèche gauche)
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        camX2 += camSpeed; // Aller à droite (flèche droite)
    }
    if (keystate[SDL_SCANCODE_RCTRL] && !isJumping2)
    {
        isJumping2 = 1; // Activer le saut pour joueur 2
    }

    // Saut pour le joueur 2
    if (isJumping2)
    {
        camY2 += jumpHeight;
        if (camY2 >= 1.0f)
        {
            isJumping2 = 0; // Fin du saut
        }
    }
    else if (camY2 > 0.0f)
    {
        camY2 -= jumpHeight; // Redescendre le joueur 2
    }
}

// Fonction pour tirer un pixel pour chaque joueur
void shootPixel1(const Uint8 *keystate)
{
    if (keystate[SDL_SCANCODE_R])
    { // Si la touche 'R' est pressée
        // Lancer un pixel vers l'avant du joueur 1
        pixelZ1 -= 0.1f; // Déplacer le pixel vers l'avant
    }
}

void shootPixel2(const Uint8 *keystate)
{
    if (keystate[SDL_SCANCODE_T])
    { // Si la touche 'T' est pressée
        // Lancer un pixel vers l'avant du joueur 2
        pixelZ2 -= 0.1f; // Déplacer le pixel vers l'avant
    }
}

void moveEnemy()
{
    // Déplacement aléatoire vers les joueurs
    float targetX = (rand() % 2 == 0) ? camX1 : camX2;
    float targetY = (rand() % 2 == 0) ? camY1 : camY2;
    float targetZ = (rand() % 2 == 0) ? camZ1 : camZ2;

    if (enemyX < targetX)
        enemyX += 0.01f;
    if (enemyX > targetX)
        enemyX -= 0.01f;
    if (enemyY < targetY)
        enemyY += 0.01f;
    if (enemyY > targetY)
        enemyY -= 0.01f;
    if (enemyZ < targetZ)
        enemyZ += 0.01f;
    if (enemyZ > targetZ)
        enemyZ -= 0.01f;
}

int main(int argc, char *argv[])
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return -1;
    }

    // Créer une fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("FPS Camera",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_OPENGL);
    if (!window)
    {
        printf("Erreur de création de la fenêtre SDL : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Créer un contexte OpenGL
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        printf("Erreur de création du contexte OpenGL : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialisation de GLEW
    if (glewInit() != GLEW_OK)
    {
        printf("Erreur GLEW\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Configuration de la perspective OpenGL
    glMatrixMode(GL_PROJECTION);                          // Choisir la matrice de projection
    glLoadIdentity();                                     // Réinitialiser la matrice
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); // Créer une projection en perspective
    glMatrixMode(GL_MODELVIEW);                           // Revenir à la matrice modèle

    glEnable(GL_DEPTH_TEST); // Activer la gestion de la profondeur

    // Boucle principale
    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        // Gestion des événements SDL
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        // Gestion des touches pour chaque joueur
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        movePlayer1(keystate);
        movePlayer2(keystate);
        shootPixel1(keystate); // Vérifier si le joueur 1 appuie sur 'R'
        shootPixel2(keystate); // Vérifier si le joueur 2 appuie sur 'T'

        // Déplacer les ennemis
        moveEnemy();

        // Limiter les déplacements des joueurs
        if (camX1 > 2.0f)
            camX1 = 2.0f;
        if (camX1 < -2.0f)
            camX1 = -2.0f;
        if (camY1 > 2.0f)
            camY1 = 2.0f;
        if (camY1 < -2.0f)
            camY1 = -2.0f;
        if (camZ1 > -3.0f)
            camZ1 = -3.0f;
        if (camZ1 < -10.0f)
            camZ1 = -10.0f;

        if (camX2 > 2.0f)
            camX2 = 2.0f;
        if (camX2 < -2.0f)
            camX2 = -2.0f;
        if (camY2 > 2.0f)
            camY2 = 2.0f;
        if (camY2 < -2.0f)
            camY2 = -2.0f;
        if (camZ2 > -3.0f)
            camZ2 = -3.0f;
        if (camZ2 < -10.0f)
            camZ2 = -10.0f;

        // Effacer l'écran et le tampon de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Vue pour le joueur 1
        glViewport(0, 0, 400, 600); // Diviser l'écran (gauche)
        glLoadIdentity();
        gluLookAt(camX1, camY1, camZ1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Dessiner le cube pour le joueur 1
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glEnd();

        // Tirer un pixel pour le joueur 1
        glPushMatrix();
        glTranslatef(pixelX1, pixelY1, pixelZ1);
        glPointSize(5.0f); // Définir la taille du point à 5 pixels
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();

        // Vue pour le joueur 2
        glViewport(400, 0, 400, 600); // Diviser l'écran (droite)
        glLoadIdentity();
        gluLookAt(camX2, camY2, camZ2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Dessiner le cube pour le joueur 2
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f); // Bleu
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glEnd();

        // Tirer un pixel pour le joueur 2
        glPushMatrix();
        glTranslatef(pixelX2, pixelY2, pixelZ2);
        glPointSize(5.0f); // Définir la taille du point à 5 pixels
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();

        // Dessiner l'ennemi
        glPushMatrix();
        glTranslatef(enemyX, enemyY, enemyZ);
        glPointSize(10.0f); // Définir la taille du point à 10 pixels
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();

        // Mettre à jour l'écran
        SDL_GL_SwapWindow(window);
    }

    // Quitter
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}