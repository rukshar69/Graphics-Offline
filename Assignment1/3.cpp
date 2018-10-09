#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double limb[6] = {0,0,0,0,0,0};
double movement_limit[12] = {-60,60,-100,0,60,-60,-60,60,-100,0,-100,0};
double rotational_displacement;
struct point
{
	double x,y,z;
};



void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void upperArmMovement(int option){

    if(option == 1){
        if(limb[0]>movement_limit[0])
            limb[0]=limb[0] - rotational_displacement;
    }else if(option == 2){
        if(limb[0]<movement_limit[1])
            limb[0]=limb[0] + rotational_displacement;
    }
}

void lowerArmMovement(int option){

    if(option == 3){
        if(limb[2]>movement_limit[2])
            limb[2]=limb[2] - rotational_displacement;
    }else if(option == 4){
        if(limb[2]<movement_limit[3])
            limb[2]=limb[2] + rotational_displacement;
    }
}
void handMovement(int option){
    if(option == 5){
        if(limb[3]<movement_limit[4])
            limb[3]=limb[3]+rotational_displacement;
    }else if(option == 6){
        if(limb[3]>movement_limit[5])
            limb[3]=limb[3] - rotational_displacement;
    }
}

void fingerRotation(int option){
if(option == 7){
        if(limb[4]>movement_limit[6])
            limb[4]=limb[4]-rotational_displacement;
    }else if(option == 8){
        if(limb[4]<movement_limit[7])
            limb[4]=limb[4] + rotational_displacement;
    }
}

void fingerMovement(int option){
if(option == 9){
        if(limb[5]>movement_limit[8])
            limb[5]=limb[5]-rotational_displacement;
    }else if(option == 10){
        if(limb[5]<movement_limit[9])
            limb[5]=limb[5] + rotational_displacement;
    }
}

void sidewayMovement(int option){
if(option == 11){
        if(limb[1]>movement_limit[10])
            limb[1]=limb[1]-rotational_displacement;
    }else if(option == 12){
        if(limb[1]<movement_limit[11])
            limb[1]=limb[1] + rotational_displacement;
    }
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    upperArmMovement(1);
			break;

        case '2':
            upperArmMovement(2); //upper arm fb
			break;

        case '3':
            lowerArmMovement(3);
			break;

        case '4':
            lowerArmMovement(4); //lower arm
			break;

        case '5':

            handMovement(5);
			break;

        case '6':
            /// hand
            handMovement(6);
			break;

        case '7':

            fingerRotation(7);
			break;

        case '8':
            /// finger rotate
            fingerRotation(8);
			break;

        case '9':

			fingerMovement(9);
			break;

        case '0':
			fingerMovement(10); /// finger moves
			break;

        case 'q':
			sidewayMovement(11);
			break;

        case 'w':
			sidewayMovement(12); ///upper arm sideway
			break;


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void drawWiredSphere(double rad, double st, double sl, double sx,double sy,double sz){
    glScalef(sx,sy,sz);
    glutWireSphere(rad,st,sl);
}

void rotateUpperArm(double tz){
    glRotatef(limb[0],0,1,0); //upper arm fb
        glTranslatef(0,0,tz);
}

void rotateLowerArm(double tz){
glRotatef(limb[2],0,1,0); //lower arm
        glTranslatef(0,0,tz);
}

void drawUpperArm(){

    glPushMatrix();
    {

        rotateUpperArm(-35);
        drawWiredSphere(15,15,15,.5,.5,2.5);
    }
    glPopMatrix();

}

void lowerArm(){

     glPushMatrix();
    {
         rotateUpperArm(-70);

        rotateLowerArm(-17);

        drawWiredSphere(10,15,15,.7,.7,1.5);
    }
    glPopMatrix();


}

void drawHand(){
    glPushMatrix();
    {
         rotateUpperArm(-72);

        rotateLowerArm(-30);

        glRotatef(limb[3],0,0,1); ///hand

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,0);
            glVertex3f(0,15,-15);
            glVertex3f(0,-15,-15);
        }glEnd();
    }
    glPopMatrix();

}
void rotateHand(double ty,double tz){
    glRotatef(limb[3],0,0,1); ///hand
        glTranslatef(0,ty,tz);
}
void drawRotatingFinger(){
         glPushMatrix();
    {
         rotateUpperArm(-72);

        rotateLowerArm(-40);


        rotateHand(-15,-4);

        glRotatef(limb[4],0,0,1); ///finger rotate
        glTranslatef(0,0,-5);



        drawWiredSphere(2.5,6,6,.7,.7,2.5);
    }
    glPopMatrix();

}

void movingFinger()
{
    glPushMatrix();
    {
         rotateUpperArm(-72);

        rotateLowerArm(-40);;

        rotateHand(15,-4);

        glRotatef(limb[5],0,1,0); ///finger moves
        glTranslatef(0,0,-5);
        drawWiredSphere(2.5,6,6,.7,.7,2.5);
    }
    glPopMatrix();

}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);

	double rad = 130;
	double camraLookingZ = -25;
	gluLookAt(rad*cos(cameraAngle), rad*sin(cameraAngle), cameraHeight,	0,0,camraLookingZ,	0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
	//gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z,0,0,-50,0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
glRotatef(limb[1],1,0,0); ///upper arm sideways


    drawUpperArm();
    lowerArm();
    drawHand();
    movingFinger();
    drawRotatingFinger();





	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}




void init(){
	//codes for initialization
	cameraHeight=-10;
	drawgrid=0;
	drawaxes=1;

	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);
	rotational_displacement=2;

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}
void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
