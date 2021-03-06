#include "stdafx.h"
#define MAX_NUM_PARTICLES 1000
#define INITIAL_NUM_PARTICLES 10
#define INITIAL_SPEED 1.0
#define TRUE 1
#define FALSE 0

#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
int sindex;
char op;
void myDisplay();
void myIdle();
void myReshape(int,int);
void main_menu(int);
void collision(int);
float forces(int,int);
void myinit();

	
	typedef struct particle
	{
		int color;
		float position[3];
		float velocity[3];
		float mass;
	}particle;

particle particles[MAX_NUM_PARTICLES];
static GLfloat spin = 0.0;
static int points=2;
int present_time;
int last_time;
int num_particles=INITIAL_NUM_PARTICLES;
float speed=INITIAL_SPEED;
bool gravity=1;
GLsizei wh=1500,ww=1500;
GLfloat colors[8][3]={{0.0,0.0,0.0},{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},{1.0,0.0,1.0},{1.0,1.0,0.0},{1.0,1.0,1.0}};

void myReshape(int w,int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0,8.0,-6.0,6.0,-8.0,8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.5,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
	if(w<h)
		glViewport(0,0,w,w);
	else
		glViewport(0,0,h,h);
	ww=w;
	wh=h;
}


int pointed()
{
	int i;
	for(i=0;i<num_particles;i++)
	{
	points=rand()%200;
	return points;
	}
}


void myinit()
{
	int i,j;
	for(i=0;i<num_particles;i++)
	{
		particles[i].mass=5.0;
		particles[i].color=i%8;
		for(j=0;j<3;j++)
		{   
			particles[i].position[j]=2.0*((float)rand()/RAND_MAX);
			particles[i].velocity[j]=speed*2.0*((float)rand()/RAND_MAX);
		}
	}
	glPointSize(points);
	glClearColor(0.5,0.5,0.5,1.0);
}


void myIdle()
{
	int i,j;
	float dt;
	present_time=glutGet(GLUT_ELAPSED_TIME);
	dt=0.001*(present_time-last_time);
	for(i=0;i<num_particles;i++)
	{
		for(j=0;j<3;j++)
		{
			particles[i].position[j]+=dt*particles[i].velocity[j];
			particles[i].velocity[j]+=dt*forces(i,j)/particles[i].mass;
		}
		collision(i);
	}
	last_time=present_time;
	glutPostRedisplay();
}


float forces(int i,int j)
{
	float force=0.0;
	if(gravity==1)force=-10.0;
	
	return(force);
}


void collision(int n)
{
	int i;
	for(i=0;i<3;i++)
	{
		if(particles[n].position[i]>=1.0)
		{
			particles[n].velocity[i]= -particles[n].velocity[i];
			particles[n].position[i]= 1.0-(particles[n].position[i]-1.0);
		}
		if(particles[n].position[i]<=-1.0)
		{
			particles[n].velocity[i]= -particles[n].velocity[i];
			particles[n].position[i]= -1.0-(particles[n].position[i]+1.0);
		}
	}
}
void sub_menu(int sindex){
	switch(sindex)
	{
	case 0:
		op=GL_POINTS;
		myinit();
		break;
	case 1:op=GL_LINES;
			myinit();
			break;			
	
	case 2:op=GL_TRIANGLES;
			myinit();
			break;			
	
	case 3:op=GL_QUADS;
			myinit();
			break;

	case 4:op=GL_POLYGON;
			myinit();
			break;
	case 5:op=GL_TRIANGLE_STRIP;
			myinit();
			break;
	case 6:op=GL_QUAD_STRIP;			
			myinit();
	
	}


}
	

void main_menu(int index)
{
	switch(index)
	{
	case 0:sub_menu(sindex);
		break;
	case 1:num_particles=2*num_particles;
			
			myinit();
			break;			
	
	case 2:num_particles=num_particles/2;
			myinit();
			break;			
	
	case 3:speed=2.0*speed;
			myinit();
			break;

	case 4:speed=speed/2.0;
			myinit();
			break;
	case 5:if(points<=32)
			   points=points*2;
		   else
		   {
			   printf("Overflow From Cube.\n");
			}
			myinit();
			break;
	case 6:if(points>0)
			   points=points/2;
		   else
		   {
			   printf("Particles wont be Visible.\n");
			}				
			myinit();
			break;
	case 7:gravity=!gravity;
			myinit();
			break;

	case 8:exit(0);
		break;
	}
}

	
void spinDisplay(void)
{
	spin = spin + 0.0000002;
	if (spin > 360.0)
	spin = spin - 360.0;
	
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:if (state == GLUT_DOWN)
							  glutIdleFunc(spinDisplay);
				break;

	case GLUT_MIDDLE_BUTTON:if (state == GLUT_DOWN)
								glutIdleFunc(NULL);
				break;

	default:
				break;
	}
}

void myDisplay()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(op);
	glPointSize(points);
	for(i=0;i<num_particles;i++)
	{
		glColor3fv(colors[particles[i].color]);
		glVertex3fv(particles[i].position);
	}
	glEnd();
	glColor3f(0.0,0.0,0.0);
	glutWireCube(6.5);
	
	glRotatef(spin, 0.0, 0.0, 1.0);
	
	glutSwapBuffers();
}


int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutCreateWindow("RANDOM POINTS");
	glutInitWindowPosition(0,0);
	glutDisplayFunc(myDisplay);
	myinit();
	
	glutCreateMenu(main_menu);
	glutAddMenuEntry("objet",0);
	glutCreateMenu(sub_menu);
	glutAddMenuEntry("points",0);
	glutAddMenuEntry("lines",1);
	glutAddMenuEntry("triangle",2);
	glutAddMenuEntry("quad",3);
	glutAddMenuEntry("poligon", 4);
	glutAddMenuEntry("triangle strip",5);
	glutAddMenuEntry("quad strip",6);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glutReshapeFunc(myReshape);
	glutAddMenuEntry("More Particles",1);
	glutAddMenuEntry("Fewer Particles",2);
	glutAddMenuEntry("Faster",3);
	glutAddMenuEntry("Slower",4);
	glutAddMenuEntry("Larger Particles", 5);
	glutAddMenuEntry("Smaller Particles",6);
	glutAddMenuEntry("Toggle Gravity",7);
	glutAddMenuEntry("Quit",8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(mouse);
	glutIdleFunc(myIdle);
	glutReshapeFunc(myReshape);
	glutMainLoop();
} 
