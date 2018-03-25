#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#define LENGTH 11

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

const char* filename = "logo_imac_400x400.jpg";
char *tab[LENGTH] = {
	"./numbers/0.png",
	"./numbers/1.png",
	"./numbers/2.png",
	"./numbers/3.png",
	"./numbers/4.png",
	"./numbers/5.png",
	"./numbers/6.png",
	"./numbers/7.png",
	"./numbers/8.png",
	"./numbers/9.png",
	"./numbers/colon.png"
};
SDL_Surface imgs[LENGTH];
GLuint textsId[LENGTH];

SDL_Surface  loadImg(const char* filename){
	SDL_Surface *tmp;
	tmp = malloc(sizeof(SDL_Surface));
	tmp = IMG_Load(filename);
	return *tmp;
}

int main(int argc, char** argv) {
    int i = 0;
    SDL_Surface *img = NULL;
    GLuint textureID;
    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("td04", NULL);
    resizeViewport();

    // TODO: Chargement et traitement de la texture

	/*img = IMG_Load("./logo_imac_400x400.jpg");
	if(img==NULL)
		printf("Erreur\n");

	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		img->w,
		img->h,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		img->pixels
	);
	glBindTexture(GL_TEXTURE_2D,0);*/
	for(i=0;i<LENGTH;i++){
		imgs[i] = loadImg(tab[i]);
	}
	
    // ...

    // TODO: Libération des données CPU

	SDL_FreeSurface(img);
    // ...

    /* Boucle de dessin (à décommenter pour l'exercice 3)*/
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        // TODO: Code de dessin

        glClear(GL_COLOR_BUFFER_BIT);

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPushMatrix();	
		glScalef(0.5,0.8,1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);
			glVertex2f(-0.5,-0.5);
			glTexCoord2f(1,1);
			glVertex2f(0.5,-0.5);
			glTexCoord2f(1,0);
			glVertex2f(0.5,0.5);
			glTexCoord2f(0,0);
			glVertex2f(-0.5,0.5);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);*/

        // ...

        // Fin du code de dessin

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // TODO: Libération des données GPU

	glDeleteTextures(1,&textureID);
    // ...

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
