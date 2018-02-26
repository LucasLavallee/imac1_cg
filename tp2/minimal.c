#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


typedef struct Point{
	float x,y;
	unsigned char r, g, b;
	struct Point* next;
} Point, *PointList;

typedef struct Primitive{
	GLenum primitiveType;
	PointList points;
	struct Primitive* next;
} Primitive, *PrimitiveList;

#define NB_SEG 500
/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* Variable type dessin */
char type = 'n';
int palette = 0; 
int color[3] = {255,255,255};
float scaleX = 8.0;
float scaleY = 4.0;
float posX = 0.0;
float posY = 0.0;
float rotX = 0.0;


int randInt(int a, int b){ /* Renvoi un entier au hasard en a et n */
	static int first = 0;
	if(first == 0){
		srand (time (NULL) );
		first = 1;
	}
	return rand()%((b+1)-a) + a;
}
void resizeModif(){
	SDL_SetVideoMode(WINDOW_WIDTH,WINDOW_HEIGHT,BIT_PER_PIXEL,SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

float getX(float x){
	return ((-1 + 2. * x / WINDOW_WIDTH)*scaleX);
}
float getY(float y){
	return ((-(-1 + 2. * y /WINDOW_HEIGHT))*scaleY);
}

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b){
	Point *p = malloc(sizeof(Point));
	if(p == NULL)
	{
		printf("Erreur dans le malloc\n");
		exit(0);
	}
	p->x = x;
	p->y = y;
	p->r = r;
	p->g = g;
	p->b = b;
	p->next = NULL;
	return p;
}

Primitive* allocPrimitive(GLenum primitiveType){
	Primitive *p;
	p = malloc(sizeof(Primitive));
	if(p == NULL){
		printf("Erreur dans le malloc Primitive\n");
		exit(0); 
	}
	p->primitiveType = primitiveType;
	p->points = NULL;
	p->next = NULL;
	return p;
}

void addPointToList(Point* point, PointList* list){
	PointList tmp = *list;
	if(tmp == NULL){
		*list = point;
	}
	else{
		while(tmp->next!=NULL){
			tmp = tmp->next;
		}
		tmp->next = point;
	}
	
}

void addPrimitive(Primitive* primitive, PrimitiveList* l){
	PrimitiveList tmp = *l;
	
	if(tmp == NULL){
		
		*l = primitive;
	}
	else{
		while(tmp->next!=NULL){
			tmp = tmp->next;
		}
		tmp->next = primitive;
	}
}

void drawPoints(PointList list, GLenum type){
	
	PointList tmp = list;
	if(tmp != NULL){
		glBegin(type);
			while(tmp != NULL){
				glColor3ub(tmp->r,tmp->g,tmp->b);
				glVertex2f(getX(tmp->x),getY(tmp->y));
				tmp = tmp->next;
			}
		glEnd();
	}
	
}

void drawSquare(int full,int r, int g, int b){
	glColor3ub(r,g,b);
	if(full == 1){
		glBegin(GL_QUADS);
	}
	else{
		glBegin(GL_LINE_LOOP);
	}
			glVertex2f(-0.5,-0.5);
			glVertex2f(0.5,-0.5);
			glVertex2f(0.5,0.5);
			glVertex2f(-0.5,0.5);
		glEnd();
}

void drawLandmark(){
	glBegin(GL_LINES);	
		glColor3ub(255,0,0);
		glVertex2f(-0.5,0);
		glVertex2f(0.5,0);
		glColor3ub(0,255,0);
		glVertex2f(0,-0.5);
		glVertex2f(0,0.5);
	glEnd();
}

void drawCircle(int full,int r,int g,int b){
	float i = 0;
	if(full == 1){
		glBegin(GL_POLYGON);
	}
	else{
		glBegin(GL_LINE_LOOP);
	}
			
			for(i=0.0;i<2*M_PI;i=i+((2*M_PI)/NB_SEG)){
				glColor3ub(r,g,b);
				glVertex2f(cos(i)/2,sin(i)/2);
			}
		glEnd();
}

void drawPrimitive(PrimitiveList list){
	PrimitiveList tmp = list;
	if(tmp!=NULL){
		while(tmp != NULL){
			drawPoints(tmp->points,tmp->primitiveType);
			tmp = tmp->next;
		}
	}
}



void pal(){
	palette = 1;
	int i;
	int size = 8;
	int tab[8][3] = {{0,0,0},
			  {255,255,255},
			  {255,0,0},
		          {0,255,0},
			  {0,0,255},					
			  {255,255,0},	
			  {0,255,255},	
			  {255,0,255}};
	for(i=0;i<size;i++){
		glColor3ub(tab[i][0],tab[i][1],tab[i][2]);
		glRectd(getX(i*(WINDOW_WIDTH/8)),getY(0),getX(i*((i+1)*(WINDOW_WIDTH/8))),getY(WINDOW_HEIGHT));
	}
}

void colorChoice(float x){
	int i = 0;
	i = ((float)x/WINDOW_WIDTH)*8;
	int tab[8][3] = {{255,255,255},
			  {0,0,0},
			  {255,0,0},
		          {0,255,0},
			  {0,0,255},					
			  {255,255,0},	
			  {0,255,255},	
			  {255,0,255}};
	color[0] = tab[i][0];
	color[1] = tab[i][1];
	color[2] = tab[i][2];
}

void deletePoints(PointList list){	
	if(list!=NULL){
		deletePoints(list->next);
		free(list);
	}
}
void deletePrimitive(PrimitiveList list){
	if(list!=NULL){
		deletePrimitive(list->next);
		deletePoints(list->points);
		free(list);
	}
}


int main(int argc, char** argv) {
	GLenum currentState = GL_POINTS;
	PointList pts = NULL;
	Point *p1;
	float randX = 0.0;
	float randY = 0.0;
	
	PrimitiveList pl = NULL;
	Primitive *pr1;
	pr1 = allocPrimitive(GL_POINTS);
	addPrimitive(pr1, &pl);

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    
    /* Titre de la fenêtre */
    SDL_WM_SetCaption("OpenGL", NULL);
    
    /* Boucle d'affichage */
    int loop = 1;
  
    	
	/*glTranslatef(1.0,2.0,0.0);*/
    while(loop) {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */

	if(palette==1){
		pal();
	} 
	else{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScalef(1/scaleX,1/scaleY,1.0);

		drawPrimitive(pl);
		drawLandmark();

		/* Déplacement cercle orange centré en 1;2 */
		glTranslatef(1.0,2.0,0.0);
		drawCircle(0,255,128,0);
		glTranslatef(-1.0,-2.0,0.0);		

		/* Rotation + Déplacement carré rouge*/
		glRotatef(45.0,0.0,0.0,1.0);
		glTranslatef(2.0,0.0,0.0);		
		drawSquare(0,255,0,0);
		glTranslatef(-2.0,0.0,0.0);
		glRotatef(-45.0,0.0,0.0,1.0);	

		/* Déplacement + rotation (45 deg) carré violet*/
		glTranslatef(2.0,0.0,0.0);
		glRotatef(45.0,0.0,0.0,1.0);		
		drawSquare(0,128,0,180);
		glRotatef(-45.0,0.0,0.0,1.0);
		glTranslatef(-2.0,0.0,0.0);
					

		/* Carré jaune 0-0 */
		glTranslatef(posX,posY,0.0);
		glRotatef(rotX,0.0,0.0,1.0);
		drawSquare(0,255,255,0);
		glRotatef(-(rotX),0.0,0.0,1.0);
		glTranslatef(-(posX),-(posY),0.0);

		/* Cercle bleu qui se déplace */
		randX = randInt(-(scaleX),scaleX);
		randY = randInt(-(scaleY),scaleY);
		glTranslatef(randX,randY,0.0);
		drawCircle(0,0,0,255);
		glTranslatef(-(randX),-(randY),0.0);
		
	}
        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            
            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                /* Clic souris */

		case SDL_MOUSEBUTTONDOWN:
			if(e.button.button == SDL_BUTTON_RIGHT){
				
			}			
			break;
			
                case SDL_MOUSEBUTTONUP:
                    	if(e.button.button == SDL_BUTTON_LEFT){
				if(palette == 1){
					/*colorChoice(e.button.x);
					palette = 0;*/
				}
			    	else{
					p1 = allocPoint(e.button.x, e.button.y, color[0],color[1],color[2]);
					addPointToList(p1, &pr1->points);
					posX = getX(e.button.x);
					posY = getY(e.button.y);
				}
			}
			else if(e.button.button == SDL_BUTTON_RIGHT){
				pr1 = allocPrimitive(currentState);
				addPrimitive(pr1,&pl);
				rotX = (e.button.x-posX)/36;
			}
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    	printf("touche pressée (code = %d)\n", e.key.keysym.sym);
			switch(e.key.keysym.sym){
				case 113: /* Q */
					SDL_Quit();
					return EXIT_SUCCESS;
					break;
				case 112: /* P */
					pr1 = allocPrimitive(GL_POINTS);
					addPrimitive(pr1, &pl);
					currentState = GL_POINTS;
					break;
				case 108: /* L */
					pr1 = allocPrimitive(GL_LINES);
					addPrimitive(pr1, &pl);
					currentState = GL_POINTS;
					break;
				case 116: /* T */
					pr1 = allocPrimitive(GL_TRIANGLES);
					addPrimitive(pr1, &pl);
					currentState = GL_TRIANGLES;
					break;
				case 115: /* S */
					pr1 = allocPrimitive(GL_LINE_STRIP);					
					addPrimitive(pr1,&pl);
					currentState = GL_LINE_LOOP;
					break;
				case 32: /* ESPACE */
					pal();					
					break;
				default:
					break;
			}
					
                    break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_VIDEORESIZE:
			WINDOW_WIDTH = e.resize.w;
			WINDOW_HEIGHT = e.resize.h;
			resizeModif();			
			break;
                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */ 
    deletePoints(pts);
    deletePrimitive(pl);
    SDL_Quit();

    return EXIT_SUCCESS;
}
