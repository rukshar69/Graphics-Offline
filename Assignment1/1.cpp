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

double radius;
double side;
struct point
{
	double x,y,z;
};


void drawAxes()
{
     //X is white
     //Y is RED
     // Z is Green
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
        }glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);{
			glVertex3f( 0,100,0);
			glVertex3f(0,-100,0);
        }glEnd();

        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);{
			glVertex3f( 0,0,100);
			glVertex3f(0,0,-100);
        }glEnd();



	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}



void drawQuad(point p1, point p2, point p3, point p4){

        glColor3f(0,1,0);
        glBegin(GL_QUADS);{
                glVertex3d(p1.x,p1.y,p1.z);
                glVertex3d(p2.x,p2.y,p2.z);
                glVertex3d(p3.x,p3.y,p3.z);
                glVertex3d(p4.x,p4.y,p4.z);

        }glEnd();
}
double cosVal(double r,int i, int segments ){
    double constant = pi/2.0;
    double angle = ((double)i/(double)segments)*constant;
    double temp = r*cos(angle);
    return temp;
}

double sinVal(double r,int i, int segments ){
    double constant = pi/2.0;
    double angle = ((double)i/(double)segments)*constant;
    double temp = r*sin(angle);
    return temp;
}
void One4thCylinder(double radius,double height,int segments){
    struct point points[200]; //first hundred for top circle the rest are for bottom circle


    double zCoOrd =  height/2.0;
    for(int i =0;i<=segments;i++){

        points[i].x = cosVal(radius,i,segments);
        points[i].y = sinVal(radius,i,segments);;
        points[i].z = zCoOrd;

        points[i+100].x = cosVal(radius,i,segments);
        points[i+100].y = sinVal(radius,i,segments);;
        points[i+100].z = -zCoOrd;
    }

    //draw segments using generated points
    for(int i=0;i<segments;i++)
    {
    glColor3f(0,1,0);
    drawQuad(points[i],points[i+1],points[i+101],points[i+100]);
    }

}


void drawSphere(double radius,int slices,int stacks){
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<=stacks;i++)
	{
        glColor3f(1,0,0);
		for(j=0;j<=slices/4;j++)
		{

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere

			}glEnd();
		}
	}
}

struct point l;
struct point u;
struct point r;

struct point pos;

double rotate_term(double x,double y , double ax , double ay){
    double term = x*ax + y*ay;
    return term;
}

struct point rotate_vect(struct point a,struct point b,double A){

    struct point temp;

    temp.x = a.y*b.z - b.y*a.z;
    temp.y = b.x*a.z -a.x*b.z;
    temp.z = a.x*b.y - b.x*a.y;

    double cosA = cos(A); double sinA = sin(A);
    a.x = rotate_term(a.x,temp.x,cosA,sinA);
    a.y = rotate_term(a.y,temp.y,cosA,sinA);
    a.z = rotate_term(a.z,temp.z,cosA,sinA);
    return a;

}


double setAngle(){

    double angle =  pi/(double)120.0;
    return angle;
}
void keyboardListener(unsigned char key, int x,int y){
    double A = setAngle();
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			{
			    A *= -1;

               struct point temp = rotate_vect( l, u, A);
               l.x = temp.x; l.y = temp.y; l.z = temp.z;

               temp = rotate_vect( r, u, A);
               r.x = temp.x;
               r.y = temp.y;
               r.z = temp.z;
               break;

			}
        case '2':
            {


               struct point temp = rotate_vect( l, u, A);
               l.x = temp.x;
                l.y = temp.y;
                l.z = temp.z;

              temp = rotate_vect( r, u, A);
               r.x = temp.x;
               r.y = temp.y;
               r.z = temp.z;


               break;
            }
        case '3':
            {
                A *= -1;
              struct point temp = rotate_vect( l,r, A);
               l.x = temp.x;
                l.y = temp.y;
                l.z = temp.z;

               temp = rotate_vect( u,r, A);
               u.x = temp.x;
               u.y = temp.y;
               u.z = temp.z;
               break;

            }
        case '4':
            {

                struct point temp = rotate_vect( l,r, A);
               l.x = temp.x;
                l.y = temp.y;
                l.z = temp.z;

               temp = rotate_vect( u,r, A);
               u.x = temp.x;
               u.y = temp.y;
               u.z = temp.z;

               break;

            }

        case '5':
            {
                struct point temp = rotate_vect( r,l, A);
               r.x = temp.x;
                r.y = temp.y;
                r.z = temp.z;

              temp = rotate_vect( u,l, A);
               u.x = temp.x;
               u.y = temp.y;
               u.z = temp.z;
               break;

            }

        case '6':
            {
                 A *= -1;

               struct point temp = rotate_vect( r,l, A);
               r.x = temp.x;
                r.y = temp.y;
                r.z = temp.z;

               temp = rotate_vect( u,l, A);
               u.x = temp.x;
               u.y = temp.y;
               u.z = temp.z;

               break;

            }

		default:
			break;
	}
}

void change_pos_vect_inc(struct point temp){

    pos.x += temp.x;
    pos.y += temp.y;
    pos.z += temp.z;
}

void change_pos_vect_dec(struct point temp){

    pos.x -= temp.x;
    pos.y -= temp.y;
    pos.z -= temp.z;
}

void homeKeyPress(){
        if(side>0 ){
            if(radius<30){
                radius++;
                side--;
            }

        }
}

void endKeyPress(){
        if(side<30 ){
                if( radius>0){
                    radius--;
                    side++;
                }

		    }

}
void specialKeyListener(int key, int x,int y){
	switch(key){
	    case GLUT_KEY_PAGE_UP:
		    change_pos_vect_inc(u);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    change_pos_vect_dec(u);
			break;

		case GLUT_KEY_UP:		// up arrow key
            change_pos_vect_inc(l);
			//cameraHeight += 3.0;
			break;

        case GLUT_KEY_DOWN:		//down arrow key
		    change_pos_vect_dec(l);
			//cameraHeight -= 3.0;
			break;

		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			change_pos_vect_dec(r);
			break;
        case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			change_pos_vect_inc(r);
			break;



		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    homeKeyPress();
			break;
		case GLUT_KEY_END:
		    endKeyPress();
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



void drawSquareZ(double tx,double ty,double tz){

    double sqSide = tz;
    if(sqSide<0){
            sqSide *= -1;
        }

    glPushMatrix();
    glTranslatef(tx,ty,tz);
    drawSquare(sqSide);
    glPopMatrix();



}

void drawOneSquareSurface(double tx,double ty,double tz,double A,double rx,double ry,double rz,bool f=true){

    if(f){
       double sqSide;
        if(tx != 0) {
                sqSide = tx;

                if(tx>0) tx += radius;
                else tx -= radius;

        }
        else if(ty != 0){
            sqSide = ty;
            if(ty>0) ty += radius;
                else ty -= radius;
        }
        else {
                sqSide = tz;
                if(tz>0) tz += radius;
                else tz -= radius;
        }
        if(sqSide<0){
            sqSide *= -1;
        }

        glPushMatrix();
        glTranslatef(tx,ty,tz);
        glRotatef(A,rx,ry,rz);
        drawSquare(sqSide);
        glPopMatrix();


    }
    else{
        drawSquareZ(tx, ty, tz);
    }



}

void drawUpperOne8thSpere(double tx,double ty,double tz,double A,double r){

    glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(A,0,0,1);
    drawSphere(r,50,50);
    glPopMatrix();
}

void drawLowerOne8thSpere(double tx,double ty,double tz,double A,double r){
    glPushMatrix();
    glTranslatef(tx,ty,-tz);
    glRotatef(A,0,0,1);
    glRotatef(90,0,1,0);
    drawSphere(r,50,50);
    glPopMatrix();

}
void drawSurfaceSquares(){


drawOneSquareSurface(0,0,side,90,0,0,0,false);
drawOneSquareSurface(0,0,-side,90,0,0,0,false);


drawOneSquareSurface(side,0,0,90,0,1,0);
drawOneSquareSurface(-side,0,0,90,0,1,0);

drawOneSquareSurface(0,side,0,90,1,0,0);
drawOneSquareSurface(0,-side,0,90,1,0,0);

}

void drawOneFourthSphere(double tx,double ty,double tz,double A,double r){

    drawUpperOne8thSpere(tx,ty,tz,A,r);
    drawLowerOne8thSpere(tx,ty,tz,A,r);
}

void exceptionUpperSphere(double tx,double ty,double tz,double r){

    glPushMatrix();
    glTranslatef(tx,ty,tz);
    drawSphere(r,50,50);
    glPopMatrix();
}

void exceptionLowerSphere(double tx,double ty,double tz,double r){
    glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(90,0,1,0);
    drawSphere(r,50,50);
    glPopMatrix();

}

void drawExceptionSphere(){
    exceptionLowerSphere(side,side,-side,radius);
    exceptionUpperSphere(side,side,side,radius);

}

void combineSphereParts(){
    drawExceptionSphere();


    drawOneFourthSphere(-side,side,side,90,radius);
    drawOneFourthSphere(side,-side,side,270,radius);

    drawOneFourthSphere(-side,-side,side,180,radius);
}

void oneVerticalQuad(double tx,double ty,double tz,double heightForCyl,double angle, bool f = true){
    if(f){
        glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(angle,0,0,1);
    One4thCylinder(radius,heightForCyl,50);
    glPopMatrix();
    }
    else {
        glPushMatrix();
    glTranslatef(tx,ty,tz);
    One4thCylinder(radius,heightForCyl,50);
    glPopMatrix();
    }

}
void drawVerticlaQuarterCylinder(){

     double   heightForCyl = side+side;
    oneVerticalQuad(side,side,0,heightForCyl,0,false);
    oneVerticalQuad(-side,side,0,heightForCyl,90);
    oneVerticalQuad(-side,-side,0,heightForCyl,180);
    oneVerticalQuad(side,-side,0,heightForCyl,270);


}

void oneTopQuad(double tx,double ty,double tz,double heightForCyl,double angle,double angle2, bool f = true){
    if(f){
        glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(angle,0,0,1); //angle
    glRotatef(angle2,0,1,0); //angle2
    One4thCylinder(radius,heightForCyl,50);
    glPopMatrix();
    }
    else {
        glPushMatrix();
    glTranslatef(tx,ty,tz);
    glRotatef(angle2,0,1,0);
    One4thCylinder(radius,heightForCyl,50);
    glPopMatrix();
    }

}

void drawTopCylinders(){

    oneTopQuad(0,side,side,side+side,0,270,false);
    oneTopQuad(-side,0,side,side+side,90,270);
    oneTopQuad(0,-side,side,side+side,180,270);
    oneTopQuad(side,0,side,side+side,270,270);

}

void drawBottomCylinders(){

    oneTopQuad(0,side,-side,side+side,0,90,false);
    oneTopQuad(-side,0,-side,side+side,90,90);
    oneTopQuad(0,-side,-side,side+side,180,90);
    oneTopQuad(side,0,-side,side+side,270,90);
}

void draw12Cylind(){
drawVerticlaQuarterCylinder();

    drawTopCylinders();
    drawBottomCylinders();

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
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();





    //drawing  spheres

    combineSphereParts();

    drawSurfaceSquares();
    //finished squares

    // cylinders

    draw12Cylind();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

///functions for initialization ////
void initialize_pos_vect(){
    pos.x = 100;
	pos.y = 100;
	pos.z = 0;

}

void initialize_l_vect(){
    l.x = -(double)1/(double)sqrt(2);
	l.y = -(double)1/(double)sqrt(2);
	l.z = 0;
}
void initialize_u_vect(){
    u.x = 0;
	u.y = 0;
	u.z = 1;

}
void initialize_r_vect(){
    r.x = -(double)1/(double)sqrt(2);
	r.y = (double)1/(double)sqrt(2);
	r.z = 0;

}

void intialize_vectors(){

    initialize_l_vect();
	initialize_u_vect();
    initialize_r_vect();
	initialize_pos_vect();

}
///END of functions for initialization ////
void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	radius = 0;
	side = 30;

	//////////~~~~~~~~~~~~ pos,  l, u , r are initialized here ~~~~~~~~///////////////
	intialize_vectors();
	//////////~~~~~~~~~~~~ end of initialization ~~~~~~~~///////////////
	//clear the screen
	glClearColor(0,0,0,0);

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
