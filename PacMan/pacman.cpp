/*
*	
* -	Coded By Turgay ÖZGÜR
* -	website: http://www.turgayozgur.com
*
* -	PACMAN.
*
*/

#include <GL\glut.h>
#include <time.h>
#include "windows.h"
#include "Gl/jitter.h"
#include "glm.c"

// 0=Geçilmemiþ yol, 1=duvar, 2=Geçilmiþ yol, 3=PACMAN, 4=hayalet1, 5=hayalet2, 6=hayalet3, 7=hayalet4, 8=hayalet5 ...
char map1[20][20] = {{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
					 {'1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','1'},
					 {'1','0','1','1','0','1','1','1','0','1','1','0','1','1','1','0','1','1','0','1'},
					 {'1','0','1','1','0','1','1','1','0','1','1','0','1','1','1','0','1','1','0','1'},
					 {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
					 {'1','0','1','1','0','1','0','1','1','1','1','1','0','1','1','0','1','1','0','1'},
					 {'1','0','0','0','0','1','0','0','0','1','0','0','0','1','0','0','0','0','0','1'},
					 {'1','1','1','1','0','1','1','1','0','1','0','1','0','1','0','1','1','1','1','1'},
					 {'1','1','1','1','0','1','0','0','0','0','0','1','0','1','0','1','1','1','1','1'},
					 {'1','0','0','0','0','0','0','1','1','1','1','1','0','0','0','0','0','0','0','1'},
					 {'1','1','1','1','0','1','0','0','0','0','0','0','0','1','0','1','1','1','1','1'},
					 {'1','1','1','1','0','1','0','1','1','1','1','1','0','1','0','0','0','0','0','1'},
					 {'1','0','0','0','0','0','0','0','0','1','0','0','0','0','0','1','1','1','0','1'},
					 {'1','0','1','1','0','1','1','1','0','1','0','1','1','1','0','1','1','0','0','1'},
					 {'1','0','0','1','0','0','0','0','0','3','0','0','0','0','0','1','1','0','1','1'},
					 {'1','1','0','1','0','1','0','1','1','1','1','1','0','1','0','1','1','0','1','1'},
					 {'1','0','0','0','0','1','0','0','0','1','0','0','0','1','0','0','0','0','0','1'},
					 {'1','0','1','1','1','1','1','1','0','1','0','1','1','1','1','1','1','1','0','1'},
					 {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
					 {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}};

char tmpMap1[20][20];
GLint DLid,id2;
GLMmodel* pmodel = NULL;
char pacmanWay = 'm'; //m=middle(static), u=up, d=down, l=left, r=right,
char ghostsWay[5] = {'m','m','m','m','m'}; //m=middle(static), u=up, d=down, l=left, r=right,
int locations_i_j[6][2] = {{14,9},{10,9},{10,8},{10,10},{10,11},{10,7}};//i denk y, j denk x;
float locations_x_y[6][2] = {{0.95,0.55},{0.95,0.95},{0.85,0.95},{1.05,0.95},{1.15,0.95},{0.75,0.95}}; // {j+0.05,(20-x)-0.05},...
float hiz = 0.05;
float sleep = 90;
bool end = false;
int tryAgain = 3;
static GLuint textures[3];
int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
GLfloat solidColor[] = {1,1,1,1.0};
int frame,time1,timebase=0;
char s[50];

void turnStart()
{
	map1[locations_i_j[0][0]][locations_i_j[0][1]] = '2';

	pacmanWay = 'm';

	for(int i=0;i<5;i++)
		ghostsWay[i] = 'm';

	//pacman
	locations_i_j[0][0] = 14; locations_i_j[0][1] = 9;
	//ghosts
	locations_i_j[1][0] = 10; locations_i_j[1][1] = 9;
	locations_i_j[2][0] = 10; locations_i_j[2][1] = 8;
	locations_i_j[3][0] = 10; locations_i_j[3][1] = 10;
	locations_i_j[4][0] = 10; locations_i_j[4][1] = 11;
	locations_i_j[5][0] = 10; locations_i_j[5][1] = 7;

	//pacman
	locations_x_y[0][0] = 0.95; locations_x_y[0][1] = 0.55;
	//ghosts
	locations_x_y[1][0] = 0.95; locations_x_y[1][1] = 0.95;
	locations_x_y[2][0] = 0.85; locations_x_y[2][1] = 0.95;
	locations_x_y[3][0] = 1.05; locations_x_y[3][1] = 0.95;
	locations_x_y[4][0] = 1.15; locations_x_y[4][1] = 0.95;
	locations_x_y[5][0] = 0.75; locations_x_y[5][1] = 0.95;

	map1[locations_i_j[0][0]][locations_i_j[0][1]] = '3';
}

bool control()
{
	for(int i=1;i<6;i++)
		for(int j=1;j<6;j++)
		{
			if(locations_i_j[0][0] == locations_i_j[i][0] && locations_i_j[0][1] == locations_i_j[i][1])
				return false;
		}
	return true;
}

bool goUp(int i, int j,float &cx,float &cy)
{
	if(map1[i-1][j] != '1')
	{
		float x = (cx*100) + 0.01;
		if(int(x) % 10 == 0) 
			cx += 0.05;
		return true;
	}
	else 
	{
		float y = (cy*100) + 0.01;
		if(int(y) % 10 != 5) 
			cy += 0.05;
		return false;
	}
}

bool goDown(int i, int j,float &cx,float &cy)
{
	if(map1[i+1][j] != '1')
	{
		float x = (cx*100) + 0.01;
		if(int(x) % 10 == 0) 
			cx += 0.05;
		return true;
		return true;
	}
	else 
	{
		float y = (cy*100) + 0.01;
		if(int(y) % 10 != 5) 
			cy -= 0.05;
		return false;
	}
}

bool goLeft(int i, int j,float &cx,float &cy)
{
	if(map1[i][j-1] != '1')
	{
		float y = (cy*100) + 0.01;
		if(int(y) % 10 == 0) 
			cy += 0.05;
		return true;
	}
	else 
	{
		float x = (cx*100) + 0.01;
		if(int(x) % 10 != 5) 
			cx -= 0.05;
		return false;
	}
}

bool goRight(int i, int j,float &cx,float &cy)
{
	if(map1[i][j+1] != '1')
	{
		float y = (cy*100) + 0.01;
		if(int(y) % 10 == 0) 
			cy += 0.05;
		return true;
	}
	else 
	{
		float x = (cx*100) + 0.01;
		if(int(x) % 10 != 5) 
			cx += 0.05;
		return false;
	}
}

void setLocation_i_j(float x,float y,int &i,int &j)
{
	x += 0.01;
	y += 0.01;

	int tmp_i = i;
	int tmp_j = j;

	i = 19 - (((int)(y*100) - ((int)(y*100) % 10))/10);
	j = ((int)(x*100) - ((int)(x*100) % 10))/10;

	if((tmp_i != i || tmp_j != j) && map1[tmp_i][tmp_j] == '3') 
	{
		map1[tmp_i][tmp_j] = '2';
		map1[i][j] = '3';
	}
}

void movePacman()
{
	if(pacmanWay == 'r')
	{
		if(goRight(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1]))
		{
			locations_x_y[0][0] += hiz;
			setLocation_i_j(locations_x_y[0][0],locations_x_y[0][1],locations_i_j[0][0],locations_i_j[0][1]);
		}
		else pacmanWay = 'm';
	}
	else if(pacmanWay == 'l')
	{
		if(goLeft(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1]))
		{
			locations_x_y[0][0] -= hiz;
			setLocation_i_j(locations_x_y[0][0],locations_x_y[0][1],locations_i_j[0][0],locations_i_j[0][1]);
		}
		else pacmanWay = 'm';
	}
	else if(pacmanWay == 'u')
	{
		if(goUp(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1]))
		{
			locations_x_y[0][1] += hiz;
			setLocation_i_j(locations_x_y[0][0],locations_x_y[0][1],locations_i_j[0][0],locations_i_j[0][1]);
		}
		else pacmanWay = 'm';
	}
	else if(pacmanWay == 'd')
	{
		if(goDown(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1]))
		{
			locations_x_y[0][1] -= hiz;
			setLocation_i_j(locations_x_y[0][0],locations_x_y[0][1],locations_i_j[0][0],locations_i_j[0][1]);
		}
		else pacmanWay = 'm';
	}
}

void moveGhosts()
{
	int rnd,loc_x_mod,loc_y_mod,loc_i,loc_j,j;
	char newWays[4];
	
	for(int i=1;i<6;i++)
	{
		loc_i = locations_i_j[i][0];
		loc_j = locations_i_j[i][1];

		if((ghostsWay[i-1] == 'r'))
		{
			if(map1[loc_i][loc_j + 1] != '1')
			{
				locations_x_y[i][0] += hiz;
				setLocation_i_j(locations_x_y[i][0],locations_x_y[i][1],locations_i_j[i][0],locations_i_j[i][1]);
			}
			else
			{
				ghostsWay[i-1] = 'm';
				locations_x_y[i][0] += hiz;
			}
		}
		else if((ghostsWay[i-1] == 'l'))
		{
			if(map1[loc_i][loc_j - 1] != '1')
			{
				locations_x_y[i][0] -= hiz;
				setLocation_i_j(locations_x_y[i][0],locations_x_y[i][1],locations_i_j[i][0],locations_i_j[i][1]);
			}
			else
			{
				ghostsWay[i-1] = 'm';
				//locations_x_y[i][0] -= hiz;
			}
		}
		else if((ghostsWay[i-1] == 'u'))
		{
			if(map1[loc_i - 1][loc_j] != '1')
			{
				locations_x_y[i][1] += hiz;
				setLocation_i_j(locations_x_y[i][0],locations_x_y[i][1],locations_i_j[i][0],locations_i_j[i][1]);
			}
			else
			{
				ghostsWay[i-1] = 'm';
				locations_x_y[i][1] += hiz;
			}
		}
		else if((ghostsWay[i-1] == 'd'))
		{
			if(map1[loc_i + 1][loc_j] != '1')
			{
				locations_x_y[i][1] -= hiz;
				setLocation_i_j(locations_x_y[i][0],locations_x_y[i][1],locations_i_j[i][0],locations_i_j[i][1]);
			}
			else
			{
				ghostsWay[i-1] = 'm';
				//locations_x_y[i][1] -= hiz;
			}
		}

		loc_x_mod = (int)(locations_x_y[i][0]*100 + 0.01) % 10;
		loc_y_mod = (int)(locations_x_y[i][1]*100 + 0.01) % 10;
		loc_i = locations_i_j[i][0];
		loc_j = locations_i_j[i][1];
		j = -1;

		if((ghostsWay[i-1] == 'm'))
		{
			if(map1[loc_i][loc_j + 1] != '1') {newWays[j+1] = 'r'; j++;}
			if(map1[loc_i][loc_j - 1] != '1') {newWays[j+1] = 'l'; j++;}
			if(map1[loc_i + 1][loc_j] != '1') {newWays[j+1] = 'd'; j++;}
			if(map1[loc_i - 1][loc_j] != '1') {newWays[j+1] = 'u'; j++;}

			if(j != -1)
			{
				rnd = rand() % (j+1);
				ghostsWay[i-1] = newWays[rnd];
			}
		}
		else if((ghostsWay[i-1] == 'l' || ghostsWay[i-1] == 'r') && (loc_x_mod == 5))
		{
			if(map1[loc_i + 1][loc_j] != '1') {newWays[j+1] = 'd'; j++;}
			if(map1[loc_i - 1][loc_j] != '1') {newWays[j+1] = 'u'; j++;}

			if(j != -1)
			{
				rnd = rand() % (j+1);
				ghostsWay[i-1] = newWays[rnd];
			}
		}
		else if((ghostsWay[i-1] == 'u' || ghostsWay[i-1] == 'd') && (loc_y_mod == 5))
		{
			if(map1[loc_i][loc_j + 1] != '1') {newWays[j+1] = 'r'; j++;}
			if(map1[loc_i][loc_j - 1] != '1') {newWays[j+1] = 'l'; j++;}

			if(j != -1)
			{
				rnd = rand() % (j+1);
				ghostsWay[i-1] = newWays[rnd];
			}
		}
	}
}

//pacman ve hayaletler çizer.
void drawCharacters()
{
	GLfloat pacmanColor[] = {1,1,0,1.0};
	GLfloat ghostColor[] = {1,1,1,1.0};

	glPushMatrix();
		glMaterialfv(GL_FRONT,GL_DIFFUSE,pacmanColor);
		glTranslatef(locations_x_y[0][0],locations_x_y[0][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	glMaterialfv(GL_FRONT,GL_DIFFUSE,ghostColor);

	glPushMatrix();
		glTranslatef(locations_x_y[1][0],locations_x_y[1][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(locations_x_y[2][0],locations_x_y[2][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(locations_x_y[3][0],locations_x_y[3][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(locations_x_y[4][0],locations_x_y[4][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(locations_x_y[5][0],locations_x_y[5][1],0.05);
		glutSolidSphere(0.05,8,8);
	glPopMatrix();

	/*if (!pmodel) {
        pmodel = glmReadOBJ("f-16.obj");
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }
    
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);*/
}

//yoldaki elmaslarý çizer.
void drawSolidSpeheres()
{
	GLfloat whiteSphereColor[] = {1,1,1,1};

	int i=0,j=0;
	int whiteSpheresCaunter = 0;

	for(float y=1.95;y>-0.05;y=y-0.1)
	{
		for(float x=0.05;x<2.05;x=x+0.1)
		{
			if(map1[i][j] == '0')//geçilmemiþ yol.
			{
				glPushMatrix();
					glMaterialfv(GL_FRONT,GL_DIFFUSE,whiteSphereColor);
					glTranslatef(x,y,0.05);
					glutSolidSphere(0.01,4,4);
				glPopMatrix();
				whiteSpheresCaunter++;
			}
			j++;
		}
		j=0;
		i++;
	}

	if(whiteSpheresCaunter == 0) end = true;
}

void drawWall()
{
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON);//taban
		glVertex3f(0.05,0.05,0.0001);
		glVertex3f(0.05,-0.05,0.0001);
		glVertex3f(-0.05,-0.05,0.0001);
		glVertex3f(-0.05,0.05,0.0001);
	glEnd();
	glBegin(GL_POLYGON);//tavan
		//glTexCoord2f(0.0, 1.0);
		glVertex3f(0.05,0.05,0.05); //glTexCoord2f(1.0, 1.0);
		glVertex3f(0.05,-0.05,0.05); //glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.05,-0.05,0.05); //glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.05,0.05,0.05);
	glEnd();
	glBegin(GL_POLYGON);//sað kenar.
		glTexCoord2f(0.0, 0.2);
		glVertex3f(0.05,0.05,0.0001); glTexCoord2f(0.2, 0.2);
		glVertex3f(0.05,-0.05,0.0001); glTexCoord2f(0.2, 0.0);
		glVertex3f(0.05,-0.05,0.05); glTexCoord2f(0.0, 0.0);
		glVertex3f(0.05,0.05,0.05);
	glEnd();
	glBegin(GL_POLYGON);//sol kenar
		glTexCoord2f(0.0, 0.5);
		glVertex3f(-0.05,0.05,0.0001); 
		glVertex3f(-0.05,-0.05,0.0001); glTexCoord2f(0.2, 0.2);
		glVertex3f(-0.05,-0.05,0.05); glTexCoord2f(0.2, 0.0);
		glVertex3f(-0.05,0.05,0.05); glTexCoord2f(0.0, 0.0);
	glEnd();
	glBegin(GL_POLYGON);//karþý kenar
		glTexCoord2f(0.0, 0.2);
		glVertex3f(0.05,0.05,0.0001); glTexCoord2f(0.2, 0.2);
		glVertex3f(-0.05,0.05,0.0001); glTexCoord2f(0.2, 0.0);
		glVertex3f(-0.05,0.05,0.05); glTexCoord2f(0.0, 0.0);
		glVertex3f(0.05,0.05,0.05);
	glEnd();
	glBegin(GL_POLYGON);//yakýn kenar
		glTexCoord2f(0.0, 0.2);
		glVertex3f(0.05,-0.05,0.0001); glTexCoord2f(0.2, 0.2);
		glVertex3f(-0.05,-0.05,0.0001); glTexCoord2f(0.2, 0.0);
		glVertex3f(-0.05,-0.05,0.05); glTexCoord2f(0.0, 0.0);
		glVertex3f(0.05,-0.05,0.05);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

// duvarlarý çizer.
void drawWalls()
{
	GLfloat wallColor[] = {0.136,0.472,0.652,1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,wallColor);

	int i=0,j=0;
	for(float y=1.95;y>-0.05;y=y-0.1)
	{
		for(float x=0.05;x<2.05;x=x+0.1)
		{
			if(map1[i][j] == '1')
			{
				glPushMatrix();
					glTranslatef(x,y,0);
					//glScalef(1,1,1);
					//glutSolidCube(0.1);
					drawWall();
				glPopMatrix();
			}
			j++;
		}
		j=0;
		i++;
	}
}

// zemini çizer.
void drawGround()
{
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 4.0); 
		glVertex3f(0,0,-0.0001); glTexCoord2f(8.0, 4.0);
		glVertex3f(2,0,-0.0001); glTexCoord2f(8.0, 0.0); 
		glVertex3f(2,2,-0.0001); glTexCoord2f(0.0, 0.0);
		glVertex3f(0,2,-0.0001);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void createDL()
{
	DLid = glGenLists(1);

	glNewList(DLid,GL_COMPILE);
		drawGround();
		drawWalls();
	glEndList();
}

#define ACSIZE	4

void initAntiAliasing()
{
	GLint viewport[4];
	int jitter;

	glGetIntegerv (GL_VIEWPORT, viewport);

	glClear(GL_ACCUM_BUFFER_BIT);
	for (jitter = 0; jitter < ACSIZE; jitter++) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix ();
		/*	Note that 4.5 is the distance in world space between
			*	left and right and bottom and top.
			*	This formula converts fractional pixel movement to 
			*	world coordinates.
			*/
		glTranslatef (j8[jitter].x*1.5/viewport[2],
					j8[jitter].y*1.5/viewport[3], 0.0);
		//Çizilecek nesler buraya:
		glTranslatef(-1,-1,0);
		glCallList(DLid);
		drawCharacters();
		drawSolidSpeheres();
		//////////////////////////
		glPopMatrix ();
		glAccum(GL_ACCUM, 1.0/ACSIZE);
	}
	glAccum (GL_RETURN, 1.0);
}

void setOrthographicProjection() {
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
       gluOrtho2D(0, 900, 0, 600);
    glScalef(1, -1, 1);
    glTranslatef(0, -600, 0);
    glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{  
  char *c;
  
  glRasterPos2f(x, y);
  
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void initScene()
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	//Iþýðý ayarla
	GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
	GLfloat lightColor[] = {1,1,1,0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor);
	GLfloat lightPosition[] = {2,2,10,1};
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel (GL_FLAT);

	// Kamerayý ayarla.
	glViewport(0, 0, 960, 680);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

	gluPerspective(52,1.00,1.0,10.0);
	gluLookAt(0,-1.1,2.25,
				0,0,0,
				0,1,0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//  TEXTURES
    unsigned char* image = NULL;
    int iheight, iwidth;
    glGenTextures(3, textures);

    image = glmReadPPM("images/ground.ppm", &iwidth, &iheight);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	/*
    image = glmReadPPM("images/ground.ppm", &iwidth, &iheight);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	*/

	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++)
			tmpMap1[i][j] = map1[i][j];
}

void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	initAntiAliasing();//antiAliasing yaparak nesneyi çizer.

	/*glTranslatef(-1,-1,0);
	glCallList(DLid);
	drawCharacters();
	drawSolidSpeheres();*/

	glMaterialfv(GL_FRONT,GL_DIFFUSE,solidColor);
	float j= -0.5;
	for(int i=0;i<tryAgain;i++)
	{
		glPushMatrix();
			glTranslatef(j,0,1.2);
			glutSolidSphere(0.01,16,16);
		glPopMatrix();
		j+= 0.05;
	}

	frame++;
	time1=glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time1-timebase));
		glutSetWindowTitle(s);
		timebase = time1;
		frame = 0;
	}

	glutSwapBuffers();

	if(control() && !end && map1[locations_i_j[0][0]][locations_i_j[0][1]] == '3')
	{
		movePacman();
		moveGhosts();
	}
	else
	{
		//daha hak varsa sahneyi tekrar ayarla. Yoksa oyunu bitir.
		if(tryAgain <= 1) {
			turnStart(); 
			tryAgain = 3;
			for(int i=0;i<20;i++)
				for(int j=0;j<20;j++)
					map1[i][j] = tmpMap1[i][j];
		}
		else
		{
			tryAgain--;
			turnStart();
		}
	}

	/*glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

	gluPerspective(52,1.00,1.0,10.0);
	gluLookAt(0,-2,0.8,
				0,2,0,
				0,1,0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
	
	Sleep(sleep); //90ms
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case '+': if(sleep > 11)sleep-=10;
			break;
		case '-': sleep+=10;
			break;
   }
}


void processSpecialKeys(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_RIGHT: if(goRight(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1])) pacmanWay = 'r';
			break;	
		case GLUT_KEY_LEFT: if(goLeft(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1])) pacmanWay = 'l';
			break;
		case GLUT_KEY_DOWN: if(goDown(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1])) pacmanWay = 'd';
			break;
		case GLUT_KEY_UP: if(goUp(locations_i_j[0][0],locations_i_j[0][1],locations_x_y[0][0],locations_x_y[0][1])) pacmanWay = 'u';
			break;
   }
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ACCUM);
	glutInitWindowSize (960, 680);
	glutCreateWindow (argv[0]);

	srand(time(NULL));

	initScene();
	createDL();
	glutDisplayFunc(mydisplay);
	glutIdleFunc(mydisplay);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}