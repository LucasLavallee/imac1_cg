#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>


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

typedef struct Color{
	unsigned char r, g, b;
} Color;

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* Variable type dessin */
char type = 'n';
int palette = 0; 
int color[3] = {255,255,255};


void resizeModif(){
	printf("%d\n",WINDOW_WIDTH);
	printf("%d\n",WINDOW_HEIGHT);
	glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

float getX(float x){
	return (-1 + 2. * x / WINDOW_WIDTH);
}
float getY(float y){
	return (-(-1 + 2. * y /WINDOW_HEIGHT));
}

/*void point(int r, int v, int b, int x, int y){
	glColor3ub(r,v,b);
	glBegin(GL_POINTS);
		glVertex2f(-1 + 2. * x / WINDOW_WIDTH, -(-1 + 2. * y /WINDOW_HEIGHT));
	glEnd();
}*/


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
void drawLine(PointList list){
	while(list->next->next!=NULL){
		glColor3ub(list->r,list->g,list->b);
		glBegin(GL_LINES);
			glVertex2f(getX(list->x), getY(list->y));
			printf("list->x: %f - list->y: %f\n",list->x,list->y);
			list = list->next;
			glVertex2f(getX(list->x), getY(list->y));
			printf("list->x: %f - list->y: %f\n",list->x,list->y);
		glEnd();
		list = list->next;
	}
}

void drawTri(PointList list){
	while(list->next->next->next!=NULL){
		glColor3ub(list->r,list->g,list->b);
		glBegin(GL_TRIANGLES);
			glVertex2f(getX(list->x), getY(list->y));
			list = list->next;
			glVertex2f(getX(list->x), getY(list->y));
			list = list->next;
			glVertex2f(getX(list->x), getY(list->y));
		glEnd();	
		list = list->next;
	}
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
    	/*Point * p1, *p2;
	PointList l;
	p1 = allocPoint(5.0, 102.0, 255, 255, 255);
	p2 = allocPoint(200.0, 202.0, 255, 255, 255);
	addPointToList(p1, &l);
	addPointToList(p2, &l);*/
	
	PointList pts = NULL;
	Point *p1;
	
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
  
    
    while(loop) {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
	/*glClearColor(R,V,0,150);
    	glClear(GL_COLOR_BUFFER_BIT);*/
	/*drawPoints(l);*/

	if(palette==1){
		pal();
	} 
	else{
		
		drawPrimitive(pl);
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
                case SDL_MOUSEBUTTONUP:
                    	
			if(palette == 1){
				colorChoice(e.button.x);
				palette = 0;
			}
		    	else{
				p1 = allocPoint(e.button.x, e.button.y, color[0],color[1],color[2]);
				addPointToList(p1, &pr1->points);
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
					break;
				case 108: /* L */
					type = 'l';
					pr1 = allocPrimitive(GL_LINES);
					addPrimitive(pr1, &pl);
					break;
				case 116: /* T */
					type = 't';
					pr1 = allocPrimitive(GL_TRIANGLES);
					addPrimitive(pr1, &pl);
					break;
				case 32: /* ESPACE */
					pal();					
					break;
				default:
					break;
			}
					
                    break;
		case SDL_MOUSEMOTION:
			/*R = (float)((e.button.x) % WINDOW_WIDTH)/WINDOW_WIDTH;
		    	V = (float)((e.button.y) % WINDOW_HEIGHT)/WINDOW_HEIGHT;*/
			
			/*R = (float)(((e.button.x)+200) % WINDOW_WIDTH)/(WINDOW_WIDTH/2);
		    	V = (float)(((e.button.y)+200) % WINDOW_HEIGHT)/(WINDOW_HEIGHT/2);*/
			/*printf("R = %f - V = %f\n",R,V);*/
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
