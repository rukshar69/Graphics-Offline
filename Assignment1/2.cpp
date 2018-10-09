#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define LEFT 0
#define RIGHT 1

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int boundaryEnable;
double angle;
double boundary_limit;
struct point
{
	double x,y,z;
};


struct point v;

void drawLine(point p1, point p2){
    glColor3f(1, 1, 1);	//grey
		glBegin(GL_LINES);{
			glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
		}glEnd();
}

void drawBoundary(){
	if(boundaryEnable==1)
	{
	    point p1,p2,p3,p4;
	    p1.x =boundary_limit;
	    p1.y =boundary_limit;
	    p1.z = 0;

	    p2.x =-boundary_limit;
	    p2.y =boundary_limit;
	    p2.z = 0;

	    p3.x =boundary_limit;
	    p3.y =-boundary_limit;
	    p3.z = 0;

	    p4.x =-boundary_limit;
	    p4.y =-boundary_limit;
	    p4.z = 0;
	    drawLine(p1,p2);
	    drawLine(p1,p3);
	    drawLine(p3,p4);
	    drawLine(p4,p2);

	}

}

struct point perpendicularVector(struct point a){
    struct point ret;
    ret.x = -a.y;
    ret.y = a.x;
    return ret;
}

// points for the pointer
struct point position_arrow;
struct point head_point1;
struct point head_point2;
struct point arrow_head;

void rotateV(double alpha)
{

    struct point vPerpendicular ;
    vPerpendicular.x  = -v.y;
    double sinTerm = vPerpendicular.x * sin(alpha);
    double cosTerm = v.x * cos(alpha);
    vPerpendicular.y = v.x;

    double sinTerm1 = vPerpendicular.y * sin(alpha);
    double cosTerm1 = v.y * cos(alpha);
    v.x = cosTerm +sinTerm ;
    v.y = cosTerm1 +sinTerm1 ;
}
struct point rotateOnePoint(point p,double alpha){
     point temp;
    temp.x = p.x;
    temp.y= p.y;

    temp.x = temp.x - position_arrow.x;
    temp.y = temp.y - position_arrow.y;
    point arrow_headNormal;
    arrow_headNormal.x = -temp.y;
    arrow_headNormal.y = temp.x;

    double sinTerm  = arrow_headNormal.x * sin(alpha);
    double cosTerm = temp.x*cos(alpha);

    temp.x = sinTerm+cosTerm;

    sinTerm = arrow_headNormal.y * sin(alpha);
    cosTerm = temp.y*cos(alpha);

    temp.y = sinTerm+cosTerm;

    temp.x = temp.x + position_arrow.x;
    temp.y = temp.y + position_arrow.y;

    return temp;
}
void rotation(double alpha){


    point p = rotateOnePoint(arrow_head, alpha);
    arrow_head.x = p.x;
    arrow_head.y = p.y;

    p = rotateOnePoint(head_point1, alpha);
    head_point1.x = p.x;
    head_point1.y = p.y;

    p = rotateOnePoint(head_point2, alpha);
    head_point2.x = p.x;
    head_point2.y = p.y;

    rotateV( alpha);
}

void rotateLeft(){

    double alpha =  (2.0 * pi)/120.0;
    rotation(alpha);
}

void rotateRight(){

    double alpha = -(2.0 * pi)/120.0;
    rotation(alpha);
}






void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
	    case GLUT_KEY_RIGHT:
			rotateRight();
			break;
		case GLUT_KEY_LEFT:
			rotateLeft();
			break;
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
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
				boundaryEnable=1-boundaryEnable;
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

void drawTriangle(point p1,point p2, point p3){

    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
    }glEnd();

}
void drawPointer(){


    point p1; point p2;
    p1.x = position_arrow.x;
    p1.y = position_arrow.y;
    p1.z = 0;

    p2.x = arrow_head.x;
    p2.y = arrow_head.y;
    p2.z = 0;

    drawLine(p1,p2);

    point p3;


    p1.x = head_point1.x;
    p1.y = head_point1.y;
    p1.z = 0;

    p3.x = head_point2.x;
    p3.y = head_point2.y;
    p3.z = 0;
    drawTriangle(p1,p2,p3);




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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


    drawBoundary();
    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);
    drawPointer();



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void changeX(){
    position_arrow.x += v.x;
    //printf("%f ",position_arrow.x);
    arrow_head.x += v.x;
    //printf("%f ",arrow_head.x);
    head_point1.x += v.x;
    //printf("%f ",head_point1.x);
    head_point2.x +=v.x;
    //printf("%f \n",head_point2.x);
}

void changeY(){
    position_arrow.y += v.y;
    arrow_head.y += v.y;
    head_point1.y += v.y;
    head_point2.y += v.y;
}
void usual_change(){
    changeX();
    changeY();
}

bool boundaryConditions(char ch){
    if(ch == 'x'){

        bool negativeSide = position_arrow.x<=-boundary_limit;
        bool positiveSide = position_arrow.x>=boundary_limit;
        bool flag = negativeSide || positiveSide;
        return flag;

    }
    else if(ch == 'y'){
            bool negativeSide = position_arrow.y<=-boundary_limit ;
        bool positiveSide = position_arrow.y>=boundary_limit;
        bool flag = negativeSide || positiveSide;
        return flag;
    }
}

void changeVelocity(char ch){
    if(ch == 'x'){

        v.x=-v.x;

    }
    else if(ch == 'y'){
       v.y=-v.y;
    }
}

struct point changePositionX(){

    double posX = position_arrow.x;
    double deltaArrow_head = 2*(arrow_head.x-posX);
    double deltaHead_point1 =  2*(head_point1.x-posX);
    double deltaHead_point2 =  2*(head_point2.x-posX);
    point p;
    p.x= arrow_head.x- deltaArrow_head;
        p.y =head_point1.x- deltaHead_point1;
        p.z =head_point2.x - deltaHead_point2;
    return p;
}
struct point changePositionY(){
    double posY = position_arrow.y;
    double deltaArrow_head = 2*(arrow_head.y-posY);
    double deltaHead_point1 =  2*(head_point1.y-posY);
    double deltaHead_point2 =  2*(head_point2.y-posY);
    point p;
    p.x= arrow_head.y- deltaArrow_head;
        p.y =head_point1.y- deltaHead_point1;
        p.z =head_point2.y - deltaHead_point2;
    return p;
}

void reflection(){
    if(boundaryConditions('x')){
        //printf("X: %f %f %f\n",arrow_head.x,head_point1.y,head_point2.y);
        changeVelocity('x');
        point temp = changePositionX();
        arrow_head.x = temp.x;
        head_point1.x = temp.y;
        head_point2.x = temp.z;
    }

    if(boundaryConditions('y')){
        //printf("Y: %f %f %f\n",arrow_head.y,head_point1.y,head_point2.y);
        changeVelocity('y');
        point temp =changePositionY();
        arrow_head.y = temp.x;
        head_point1.y = temp.y;
        head_point2.y = temp.z;

    }
}

void animate(){

	//codes for any changes in Models, Camera
	usual_change();
    reflection();


	glutPostRedisplay();
}
void init_boundary(){
boundaryEnable =1;
	boundary_limit = 120;

}

void init_stick(){
    double tempx, tempy;
    tempx = position_arrow.x = -26;
	tempy =position_arrow.y = 26;

	printf("bottom point : %lf %lf\n",tempx,tempy);
	tempx =arrow_head.x = position_arrow.x+20;
	tempy =arrow_head.y = position_arrow.y+20;
	printf("top point : %lf %lf\n",tempx,tempy);
}

void init_triangle_head(){
    double tempx, tempy;
	double r = 5;
	double cosV = r*cos(20);
	double sinV = r*sin(20);
	printf("%lf %lf\n",cosV,sinV);

	tempx = arrow_head.x -cosV;
	tempy = arrow_head.y -2*sinV;

	printf("%lf %lf\n",tempx,tempy);
	head_point1.x = tempx;
	head_point1.y = tempy;


	tempy = arrow_head.y -cosV;
	tempx = arrow_head.x -2*sinV;
    printf("%lf %lf\n",tempx,tempy);

	head_point2.x = tempx;
	head_point2.y = tempy;

}
void init_pointer(){

    init_stick();
    init_triangle_head();
	/**/
}

void init_displacement_vect(){

v.x = 0.07;
	v.y = 0.07;
}
void init(){
	//codes for initialization
	drawgrid=0;
	//drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;



	init_pointer();
	init_displacement_vect();

	//clear the screen
	glClearColor(0,0,0,0);

	init_boundary();
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
