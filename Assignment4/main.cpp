#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <fstream>
#include<vector>

#include <windows.h>
#include <glut.h>
#include "FILE2.h"


#define pi (2*acos(0.0))
#define Window_width 500
#define Window_height 500
#define VIEW_ANGLE  80
int recursion_level = 3;

int image_width;
Vector3** bitmap_image1;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


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
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
        }glEnd();

        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);{
			glVertex3f( 0,100,0);
			glVertex3f(0,-100,0);
        }glEnd();

        glColor3f(0.0 ,0.0 ,1.0);
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
    //glColor3f(1.0,0.0,0.0);
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

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
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
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
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

void save_image(){

    bitmap_image img(image_width, image_width);

    for (int i=0; i<image_width; i++) {

        for (int j=0; j<image_width; j++) {
            double r = 255*bitmap_image1[i][j].x;
            double g = 255*bitmap_image1[i][j].y;
            double b = 255*bitmap_image1[i][j].z;
            img.set_pixel(j, i,r , g,b);
        }
    }
    cout<<"image saved"<<endl;

    img.save_image("1.bmp");
}

void init_frame_buf(){
    cout<<"init frame buf\n";


    bitmap_image1 = new Vector3* [image_width];

    Vector3 color(0, 0, 0);

    for (int i=0; i<image_width; i++) {
        bitmap_image1[i] = new Vector3[image_width];
        for (int j=0; j<image_width; j++) {
            bitmap_image1[i][j] = color;
        }
    }
}

void Capture() {

    init_frame_buf();

    //plane_distance= (window_height/2)/tan(VIEW_ANGLE/2)
    double angle_temp = VIEW_ANGLE/2;
    double planeDistance = (Window_height/2)/tan(angle_temp*pi/180);
    //topleft= eye + l*plane_distance-r*WINDOW_WIDTH/2+u*WINDOW_HEIGHT/2);
    Vector3 eye(pos.x,pos.y,pos.z);
    Vector3 l_(l.x,l.y,l.z);
    Vector3 r_(r.x,r.y,r.z);
    Vector3 u_(u.x,u.y,u.z);
    Vector3 topLeft = eye + l_ * planeDistance - r_ * (Window_width/2) + u_ * (Window_height/2);
    double du = (double)Window_width / (double)image_width;
    double dv =(double)Window_height / (double)image_width;

    for (int i=0; i<image_width; i++) {

        for (int j=0; j<image_width; j++) {
            double t_min = 2147483647 ; double t;
            double constant1 = j*du;

            double constant2 = i*dv;
            Vector3 t1 = r_*constant1 ;
            Vector3 t2 =  u_*constant2;
            Vector3 corner = topLeft + t1-t2;

            Ray ray(eye, corner - eye);

            int nearest=-1;

            double dummyColAt[3];

            for (int k=0; k < objects.size(); k++) {

                t = objects[k]->intersect(&ray, dummyColAt, 0);


                if(t <= 0) {
                    continue;
                }

                if (t < t_min) {
                    nearest = k; t_min = t;
                }
            }
            double colorAt[3];

            if(nearest!=-1) {


                t = objects[nearest]->intersect(&ray, colorAt, 1);
                for(int k = 0;k<3;k++){
                    if(k == 0) bitmap_image1[i][j].x = objects[nearest]->modifiedColor[k];
                    if(k == 1) bitmap_image1[i][j].y = objects[nearest]->modifiedColor[k];
                    if(k == 2) bitmap_image1[i][j].z = objects[nearest]->modifiedColor[k];
                }
                cout<<"caught"<<endl;
            }
        }
    }

    save_image();


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
        case '0':
            Capture();
            break;
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
	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	int limit = lights.size();

	for (int i=0; i < limit; i++) {
        lights[i].drawPoint();
    }

    limit = objects.size();
    for (int i=0; i < limit; i++) {
        objects[i]->draw();
    }





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
    //(0, -200, 10
    pos.x = 100;
	pos.y = 100;
	pos.z = 10;

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

	//////////~~~~~~~~~~~~ pos,  l, u , r are initialized here ~~~~~~~~///////////////
	intialize_vectors();

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
	gluPerspective(VIEW_ANGLE ,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void loadTestData(){
    image_width =768;
    Object *temp;

    Vector3 center(20,20,10);
    double radius = 10;
    image_width =768;

    temp = new Sphere(center, radius);
    temp->setColor(1,0,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Vector3 center3(20,40,10);
    radius = 10;
    image_width =768;

    temp = new Sphere(center3, radius);
    temp->setColor(1,0.75,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Vector3 center1(-20,-20,60);
    radius = 10;

    temp = new Sphere(center1, radius);
    temp->setColor(1,0,1);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Vector3 center2(20,20,40);
    radius = 5;

    temp = new Sphere(center2, radius);
    temp->setColor(1,1,1);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Vector3 a(-20.0, -20.0, 20.0);
    Vector3 b(40.0, -30.0, 10.0);
    Vector3 c(-50.0, 40.0, 0.0);

    temp = new Triangle(a, b, c);
    temp->setColor(0, 1, 0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);


    /*double coeff[] = {0.0625, 0.04,0.04 ,0 ,0, 0 ,0 ,0 ,0 ,-36};
    Vector3 reff(0, 0, 0);

    temp = new General_Quadratic(coeff, reff, 15, 5,5);
    temp->setColor(1, 0, 1);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(15);
    objects.push_back(temp);*/

    temp=new Floor(1000, 20);
	temp->setCoEfficients(0.4,0.2,0.2,0.2);
	temp->setShine(1);
	objects.push_back(temp);


    Vector3 light1(-50,50,50);
    lights.push_back(light1);

}

void loadTestData2(){
    image_width =768;
    Object *temp;

    Vector3 a(50 ,30, 0);
    Vector3 b(70, 60 ,0);
    Vector3 c(50, 45, 50);

    temp = new Triangle(a, b, c);
    temp->setColor(1.0, 0.0, 0.0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    Vector3 a1(30 ,60, 0);
    Vector3 b1(70, 60 ,0);
    Vector3 c1(50, 45, 50);

    temp = new Triangle(a1, b1, c1);
    temp->setColor(1.0, 1.0, 0.0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);


    Vector3 a2(30 ,60, 0);
    Vector3 b2(50, 30 ,0);
    Vector3 c2(50, 45, 50);

    temp = new Triangle(a2, b2, c2);
    temp->setColor(0.0, 1.0, 0.0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    temp=new Floor(1000, 20);
	temp->setCoEfficients(0.4,0.2,0.2,0.2);
	temp->setShine(1);
	objects.push_back(temp);


    Vector3 light1(-50,50,50);
    lights.push_back(light1);

}

ifstream sc;
int getChoice(string str){
    if(str == "triangle") return 2;
    if(str == "sphere") return 1;
    if(str == "general") return 3;
    return 0;

}
vector<double>take_input(int limit){

    vector<double> v;
    for(int i = 0;i<limit;i++){
        double temp;
        sc>>temp;
        v.push_back(temp);
    }
    return v;
}

void push_floor(){
    Object *obj;
    obj = new Floor(1000, 20);
    obj->setCoEfficients(0.4,0.2,0.2,0.2);
    obj->setShine(1);
    objects.push_back(obj);
}

void input_sphere(){
    cout<<"sphere\n";
    double radius, a;
    Object*obj;
    vector<double>v1 = take_input(3);
            Vector3 center(v1[0],v1[1],v1[2]);
            sc>>radius;

            vector<double>v2 = take_input(3);
            vector<double>v3 = take_input(4);

            obj = new Sphere(center, radius);
            obj->setColor(v2[0],v2[1],v2[2]);
            obj->setCoEfficients(v3[0],v3[1],v3[2],v3[3]);

            sc>>a;
            obj->setShine(a);
            objects.push_back(obj);

}

void input_triangle(){
    cout<<"triangle\n";
    Object*obj;
    double a;
    vector<double>v1 = take_input(3);
            Vector3 A(v1[0],v1[1],v1[2]);

            vector<double>v2 = take_input(3);
            Vector3 B(v2[0],v2[1],v2[2]);

            vector<double>v3 = take_input(3);
            Vector3 C(v3[0],v3[1],v3[2]);

            obj = new Triangle(A, B, C);

            v3 = take_input(3);

            obj->setColor(v3[0],v3[1],v3[2]);
            v3 = take_input(4);
            obj->setCoEfficients(v3[0],v3[1],v3[2],v3[3]);

            sc>>a;
            obj->setShine(a);

            objects.push_back(obj);

}

void input_quad(){
    cout<<"quad\n";
    Object*obj;
    double a;
    vector<double>v1 = take_input(10);
            double coefficients[10];

            for (int i=0; i<10; i++) {
                coefficients[i] = v1[i];
            }

            vector<double>v2 = take_input(3);


            vector<double>v3 = take_input(3);
            vector<double>v4 = take_input(3);
            vector<double>v5 = take_input(4);
            Vector3 ref_pt(v2[0],v2[1],v2[2]);
            obj = new General_Quadratic(coefficients, ref_pt,v3[0],v3[1],v3[2]);
            obj->setColor(v4[0],v4[1],v4[2]);
            obj->setCoEfficients(v5[0],v5[1],v5[2],v5[3]);

            sc>>a;
            obj->setShine(a);

            //objects.push_back(obj);


}
void clear_memory() {
    // myvector.clear();
    lights.clear();
    objects.clear();
}
void loadActualData2()
{
    int objects_number;
    int lights_no;
    sc.open("scene.txt");

    sc>>recursion_level;
    sc>>image_width;
    Object *obj;

    sc>>objects_number;
    cout<<recursion_level<<" "<<image_width<<" "<<objects_number<<endl;
    for (int i=0; i<objects_number; i++) {
        string input;
        sc>>input;
        int choice = getChoice(input);
        double a, b, c, radius;


        if(choice == 1) {//sphere
            input_sphere();
        }
        if(choice == 2) {
                input_triangle();
        }
        if(choice == 3) {
              input_quad();
        }

        }

        sc>>lights_no;
    for (int i=0; i<lights_no; i++) {
            vector<double>v1 = take_input(3);

        Vector3 light(v1[0],v1[1],v1[2]);
        lights.push_back(light);
    }


    push_floor();
    }







int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(Window_width, Window_height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();
	//loadTestData2();
	loadActualData2();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();
		//The main loop of OpenGL

    clear_memory();

	return 0;
}
