#ifndef FILE2_H_INCLUDED
#define FILE2_H_INCLUDED

#include<vector>
#include "bitmap_image.hpp"
#include<iostream>
#define pi (2*acos(0.0))
using namespace std;
extern int recursion_level;
class Vector3 {

public:
    double x;
    double y;
    double z;

    Vector3() {

    }

    Vector3(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }


    void printVector(){
        cout<<x<<" "<<y<<" "<<z<<" "<<endl;
    }

    Vector3 operator*(double mult) {
        double x_ = x*mult;
        double y_ = y*mult;
        double z_ = z*mult;
        Vector3 result(x_, y_, z_);
        return result;
    }

    Vector3 operator+(Vector3 point_) {
        double x_ = x + point_.x;
        double y_ = y + point_.y;
        double z_ = z + point_.z;
        Vector3 result(x_, y_, z_);
        return result;
    }

    Vector3 operator-(Vector3 point_) {
        double x_ = x - point_.x;
        double y_ = y - point_.y;
        double z_ = z - point_.z;
        Vector3 result(x_, y_, z_);
        return result;
    }


    void drawPoint() {
        double thick = 2;

        glColor3f(1.0, 1.0, 1.0);

        glBegin(GL_QUADS);
        {
            glVertex3f(x+thick, y, z+thick);
            glVertex3f(x+thick, y, z-thick);
            glVertex3f(x-thick, y, z-thick);
            glVertex3f(x-thick, y, z+thick);
        }
        glEnd();
    }
    Vector3 normalize_vector()
    {
        double sq_sum = x*x + y*y + z*z;
        double sqrt_ = sqrt(sq_sum);
        Vector3 temp(x/sqrt_,y/sqrt_,z/sqrt_);
        return temp;
    }

};

class Ray {
public:
    Vector3 start;
    Vector3 dir;

    Ray(Vector3 start, Vector3 dir) {
        this->start = start;
        Vector3 temp = dir;
        temp = temp.normalize_vector();
        this->dir = temp;
    }
};


class Object {

public:

    Vector3 reference_point;

    double width;
    double height;
    double length;
    int shine;
    double color[3];
    double co_efficients[4];
    double modifiedColor[3];
    double  source_factor = 1.0;
    bitmap_image texture;
    vector<double>color_texture;
    double texture_height;
    double texture_width;
    Object() {
    }
    virtual void draw(){}
    void setColor(double r, double g, double b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShine(int s) {
        this->shine = s;
    }

    void setCoEfficients(double AMBIENT,double DIFFUSE,double SPECULAR,double REFLECTION) {
        //AMBIENT, DIFFUSE, SPECULAR, REFLECTION
        for(int i = 0;i<4;i++){
            if(i == 0)
                co_efficients[i] = AMBIENT;
            else if( i == 1)
                co_efficients[i] = DIFFUSE;
            else if(i == 2)
                co_efficients[i] = SPECULAR;
            else if(i == 3)
                co_efficients[i] = REFLECTION;
        }

    }

    virtual double intersect(Ray *r, double *current_color, int level){
        return -1;
    }
    virtual double getIntersectingT(Ray *r){return -1;}

    double dot_product(Vector3 v1, Vector3 v2){
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z ;
    }

    virtual Vector3 getNormal(Vector3 intersectionPoint) {
        return (intersectionPoint - reference_point).normalize_vector();
    }

    virtual void setColorAt(){}



};


vector<Object*> objects;
vector<Vector3> lights;


class Sphere: public Object {

public:
    double eta = 1.5;

    Sphere(Vector3 center, double radius) {
        reference_point = center;
        length = radius;
    }

    void draw() {
        int slices = 25;
        int  stacks = 25;
        glColor3f(color[0], color[1], color[2]);

        double h;
        double r;

        Vector3 points[100][100];

        //generate points
        for(int i=0; i<=stacks; i++) {
            h = length * sin(((double)i/(double)stacks)*(pi/2));
            r = length * cos(((double)i/(double)stacks)*(pi/2));
            for(int j=0; j<=slices; j++) {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }

        //draw quads using generated points
        for(int i=0; i<stacks; i++) {
            for(int j=0; j<slices; j++) {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x+reference_point.x,points[i][j].y+reference_point.y,points[i][j].z+reference_point.z);
                    glVertex3f(points[i][j+1].x+reference_point.x,points[i][j+1].y+reference_point.y,points[i][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j+1].x+reference_point.x,points[i+1][j+1].y+reference_point.y,points[i+1][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j].x+reference_point.x,points[i+1][j].y+reference_point.y,points[i+1][j].z+reference_point.z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x+reference_point.x,points[i][j].y+reference_point.y,-points[i][j].z+reference_point.z);
                    glVertex3f(points[i][j+1].x+reference_point.x,points[i][j+1].y+reference_point.y,-points[i][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j+1].x+reference_point.x,points[i+1][j+1].y+reference_point.y,-points[i+1][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j].x+reference_point.x,points[i+1][j].y+reference_point.y,-points[i+1][j].z+reference_point.z);
                }
                glEnd();
            }
        }
    }


    double getIntersectingT(Ray *r) {
        Vector3 st = r->start - reference_point; //without this only the first sphere comes up
        double A = dot_product(r->dir, r->dir);
        double B = 2* dot_product(st,r->dir);
        double C = dot_product(st,st) - length*length;

        double D = B*B - 4*A*C;
        if(D<0) return -1;
        D = sqrt(D);
        double t1 = -B+D;
        t1 /= (2.0*A);
        double t2 = -B-D;
        t2 /= (2.0*A);
        return min(t1,t2);

    }

    void setColorAt(){
        modifiedColor[0] = color[0]*co_efficients[0] ;//AMBIENCE
        modifiedColor[1] = color[1]*co_efficients[0] ;
        modifiedColor[2] = color[2]*co_efficients[0] ;
    }


    Vector3 getReflection(Ray* ray, Vector3 normal) {
        //out = incidentVec - 2.f * Dot(incidentVec, normal) * normal;
        //reflected vector should be normal
        Vector3 incidentVec = ray->dir;
        double dot_ =  dot_product(incidentVec, normal);
        Vector3 temp = normal*(2.0*dot_);
        Vector3 out = incidentVec - temp;
        return (incidentVec - temp).normalize_vector();
    }

    Vector3 getRefraction(Ray* ray, Vector3 normal) {



        Vector3 incidentVec = ray->dir;
        float N_dot_I = dot_product(normal, incidentVec);
          float k = 1.0 - eta * eta * (1.0 - N_dot_I * N_dot_I);
          Vector3 out;
          Vector3 zero_vector(0.0, 0.0, 0.0);
          if (k < 0.0)
            out = zero_vector;
          else{
            out = incidentVec*eta -  normal*(eta * N_dot_I + sqrt(k));
            return out.normalize_vector();
          }

    }

    Vector3 getNormal(Vector3 intersectionPoint) {
        return (intersectionPoint - reference_point).normalize_vector();
    }

    double intersect(Ray *r, double *current_color, int level){
        double t= getIntersectingT(r) ;//perform computation of intersection here

        if( t<=0) return -1;
        if(level==0)return t;

        Vector3 intersectionPoint = r->start+ r->dir*t;
        setColorAt();

        Vector3 normal=getNormal(intersectionPoint);
        Vector3 reflection=getReflection(r, normal);
        Vector3 refraction = getRefraction(r, normal);

        int limitIllum = lights.size();
        for(int i = 0;i<limitIllum;i++){
           //direction= (lightSource-intersectionPoint) //normalize it
            Vector3 lightSource = lights[i];
            Vector3 direction = (lightSource-intersectionPoint);
            double direction_length = dot_product(direction,direction);
            direction_length = sqrt(direction_length);
            direction = (lightSource-intersectionPoint).normalize_vector();
            Vector3 start = intersectionPoint + direction*1.0;
            //1 is for taking slightly above the point so
            //it doesnft again intersect with same object due to precision
            Ray L(start, direction);

            bool obscure_ray = false;
            int limitObj = objects.size();
            //For each object now check whether this L ray obscured by any object or not.

            for (int j=0; j < limitObj; j++) {
                //ray joto dur jabbeh tar moddhe jodi t thakkeh tailleh toh obstruction hocche
                double t_ = objects[j]->getIntersectingT(&L);

                if(t_ < 0 ) {
                    continue;
                }
                if( t_ > direction_length) continue;

                obscure_ray = true;
                break;
            }

            //If it is not obscured that means light falls
            //onto the intersection point so you have update current_color,
            if(obscure_ray == false){

                //Calculate lambert value,
                //Calculate phong value
                //check the illumination slide for formula
                //lambert value calculated by taking the dot product of the surface's normal vector, N and
                //a normalized light-direction vector,L DIFFUSE REFLECTION
                double lambert_value =dot_product(direction,normal);
                if(lambert_value<0) lambert_value = 0;
                //PHONG VALUE -> sPECULAR REFLECTION
                double phong_value =dot_product(r->dir,reflection);
                phong_value = pow(phong_value,shine);
                if(phong_value<0) phong_value = 0;

                //current_color+=source_factor*lambert*co_efficient[DIFFIUSE]*colorAt
                //current_color+=source_factor*pow(phong,shine)*co_efficient[SPECULAR]*colorAt
                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*lambert_value*co_efficients[1]*color[a];//DIFFUSE
                }

                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*phong_value*co_efficients[2]*color[a];//SPECULAR
                }

            }


            if(level<recursion_level){
                double t_min = 2147483647 ; double t;
                start=intersectionPoint+reflection*1.0 ; //slight up to avoid own intersection
                Ray reflectionRay(start, reflection);
                //Like capture method,  find the nearest intersecting object,
                // using intersect function
                int nearest=-1;
                for (int k=0; k < objects.size(); k++) {

                    t = objects[k]->getIntersectingT(&reflectionRay);


                    if(t <= 0) {
                        continue;
                    }

                    if (t < t_min) {
                        nearest = k; t_min = t;
                    }
                }

                double colorAt[3];

                if(nearest!=-1) {


                    t = objects[nearest]->intersect(&reflectionRay, colorAt, level+1);
                    double reflection_val = co_efficients[3];
                    for(int k = 0;k<3;k++){
                        if(k == 0) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 1) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 2) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                    }
                }

                //REFRACTION

                start = intersectionPoint + refraction * 1.0;

                Ray refractionRay(start, refraction);
                t_min = 2147483647 ; nearest=-1;

                for (int k=0; k < objects.size(); k++) {

                    t = objects[k]->getIntersectingT(&refractionRay);


                    if(t <= 0) {
                        continue;
                    }

                    if (t < t_min) {
                        nearest = k; t_min = t;
                    }
                }

                if(nearest!=-1) {


                    t = objects[nearest]->intersect(&refractionRay, colorAt, level+1);
                    double refraction_val = eta;
                    for(int k = 0;k<3;k++){
                        if(k == 0) modifiedColor[k] += objects[nearest]->modifiedColor[k]*refraction_val;
                        if(k == 1) modifiedColor[k] += objects[nearest]->modifiedColor[k]*refraction_val;
                        if(k == 2) modifiedColor[k] += objects[nearest]->modifiedColor[k]*refraction_val;
                    }
                }



            }
            //Check whether all current_color pixel value is within 1 or 0 if not set it

            for(int b = 0;b<3;b++){
                modifiedColor[b] = (modifiedColor[b]>1)?1:modifiedColor[b];
                modifiedColor[b] = (modifiedColor[b]<0)?0:modifiedColor[b];
            }


        }

        return t;
    }

};

class Floor: public Object {

public:

    void read_image(){
        char* filename = "texture.bmp";
        double scale_factor = 1000.0;

        texture = bitmap_image(filename);
        texture_height = texture.height()/scale_factor;
        texture_width= texture.width()/scale_factor;
        cout<<texture_height<<" "<<texture_width<<endl;
    }

    Floor(double floorWidth, double tileWidth) {
        read_image();
        reference_point = Vector3(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;



    }
    Vector3 getNormal(Vector3 intersection) {
        Vector3 normal(0,0,1);
        return normal;
    }

    void drawTile(Vector3 p1,Vector3 p2,Vector3 p3,Vector3 p4,double color_tile){
        glColor3f(color_tile, color_tile, color_tile);
        glBegin(GL_QUADS);
                {
                    glVertex3f(p1.x,p1.y,p1.z);
                    glVertex3f(p2.x,p2.y,p2.z);
                    glVertex3f(p3.x,p3.y,p3.z);
                    glVertex3f(p4.x,p4.y,p4.z);
                }
                glEnd();

    }

    void draw() {

        int totalLength = abs(reference_point.x*2);
        int numOfTiles = (int)totalLength/length;
        double r = 0, g = 0,b = 0;

        for (int i=0; i<numOfTiles; i++) {
            double color_tile;
            int odd_even = i;
            for (int j=0; j<numOfTiles; j++) {


                odd_even +=j;
                odd_even%=2;

                if (odd_even==1) {
                    color_tile= 1;

                }
                if (odd_even==0){
                    color_tile = 0;
                }

                Vector3 p1(reference_point.x+length*i, reference_point.y+length*j, reference_point.z);
                Vector3 p2(reference_point.x+length*(i+1), reference_point.y+length*j, reference_point.z);
                Vector3 p3(reference_point.x+length*(i+1), reference_point.y+length*(j+1), reference_point.z);
                Vector3 p4(reference_point.x+length*i, reference_point.y+length*(j+1), reference_point.z);

                drawTile(p1,p2,p3,p4,color_tile);

            }
        }
    }

    Vector3 getReflection(Ray* ray, Vector3 normal) {
        //out = incidentVec - 2.f * Dot(incidentVec, normal) * normal;
        //reflected vector should be normal
        Vector3 incidentVec = ray->dir;
        double dot_ =  dot_product(incidentVec, normal);
        Vector3 temp = normal*(2.0*dot_);
        Vector3 out = incidentVec - temp;
        return (incidentVec - temp).normalize_vector();
    }



    double getIntersectingT(Ray *r){
        Vector3 dummy(0,0,0);
        Vector3 normal(0,0,1);
        double numerator = (-1.0)*dot_product(r->start,normal) ;
        double denominator = dot_product(r->dir, normal);

        return numerator/denominator; //t
    }

    void setColorAt(){
        modifiedColor[0] = color[0]*co_efficients[0]*color_texture[0] ;//AMBIENCE
        modifiedColor[1] = color[1]*co_efficients[0]*color_texture[1]  ;
        modifiedColor[2] = color[2]*co_efficients[0]*color_texture[2]  ;

        /*modifiedColor[0] = color[0]*co_efficients[0] ;//AMBIENCE
        modifiedColor[1] = color[1]*co_efficients[0]  ;
        modifiedColor[2] = color[2]*co_efficients[0];*/
    }
    void texture_fixing(Vector3 intersectionPoint){
        Vector3 abs_pt((reference_point.x),(reference_point.y),0);
        int x_texture = (intersectionPoint.x - abs_pt.x) ;
        x_texture *= texture_width;
        int y_texture = (intersectionPoint.y - abs_pt.y) ;
        y_texture *= texture_height;

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        texture.get_pixel(x_texture, y_texture, red, green, blue);

        color_texture.push_back(red/255.0);
        color_texture.push_back(green/255.0);
        color_texture.push_back(blue/255.0);
    }

    int outOfBount(Vector3 intersectionPoint){
        double x_min = reference_point.x;
        if (intersectionPoint.x<x_min) return -1;
        double y_min = reference_point.y;
        if (intersectionPoint.y<y_min) return -1;
        double x_max = -reference_point.x;
        if(intersectionPoint.x>x_max) return -1;
        double y_max = -reference_point.y;
        if(intersectionPoint.y>y_max) return -1;
        return 1;
    }
    int determineTileColor(Vector3 intersectionPoint){
        int tile_x = intersectionPoint.x / length;
        int odd_even = tile_x;
        int tile_y = intersectionPoint.y / length;
        odd_even += tile_y;
        odd_even %=2;
        if(odd_even) return 1;
        return 0;
    }
    double intersect(Ray *r, double *current_color, int level){
        double t= getIntersectingT(r) ;//perform computation of intersection here

        if( t<=0) return -1;
        if(level==0)return t;

        Vector3 intersectionPoint = r->start+ r->dir*t;

        int y_n = outOfBount(intersectionPoint);
        if(y_n==-1)return -1;

        int color_tile = determineTileColor(intersectionPoint);

        for(int i = 0;i<3;i++) color[i] = color_tile;


        texture_fixing(intersectionPoint);
        setColorAt();

        Vector3 normal(0,0,1);
        Vector3 reflection=getReflection(r, normal);

        int limitIllum = lights.size();
        for(int i = 0;i<limitIllum;i++){
           //direction= (lightSource-intersectionPoint) //normalize it
            Vector3 lightSource = lights[i];
            Vector3 direction = (lightSource-intersectionPoint);
            double direction_length = dot_product(direction,direction);
            direction_length = sqrt(direction_length);
            direction = (lightSource-intersectionPoint).normalize_vector();
            Vector3 start = intersectionPoint + direction*1.0;
            //1 is for taking slightly above the point so
            //it doesnft again intersect with same object due to precision
            Ray L(start, direction);

            bool obscure_ray = false;
            int limitObj = objects.size();
            //For each object now check whether this L ray obscured by any object or not.

            for (int j=0; j < limitObj; j++) {
                //ray joto dur jabbeh tar moddhe jodi t thakkeh tailleh toh obstruction hocche
                double t_ = objects[j]->getIntersectingT(&L);

                if(t_ < 0 ) {
                    continue;
                }
                if( t_ > direction_length) continue;

                obscure_ray = true;
                break;
            }

            //If it is not obscured that means light falls
            //onto the intersection point so you have update current_color,
            if(obscure_ray == false){

                //Calculate lambert value,
                //Calculate phong value
                //check the illumination slide for formula
                //lambert value calculated by taking the dot product of the surface's normal vector, N and
                //a normalized light-direction vector,L DIFFUSE REFLECTION
                double lambert_value =dot_product(direction,normal);
                if(lambert_value<0) lambert_value = 0;
                //PHONG VALUE -> sPECULAR REFLECTION
                double phong_value =dot_product(r->dir,reflection);
                phong_value = pow(phong_value,shine);
                if(phong_value<0) phong_value = 0;

                //current_color+=source_factor*lambert*co_efficient[DIFFIUSE]*colorAt
                //current_color+=source_factor*pow(phong,shine)*co_efficient[SPECULAR]*colorAt
                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*lambert_value*co_efficients[1]*color[a];//DIFFUSE
                }

                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*phong_value*co_efficients[2]*color[a];//SPECULAR
                }

            }


            if(level<recursion_level){
                double t_min = 2147483647 ; double t;
                start=intersectionPoint+reflection*1.0 ; //slight up to avoid own intersection
                Ray reflectionRay(start, reflection);
                //Like capture method,  find the nearest intersecting object,
                // using intersect function
                int nearest=-1;
                for (int k=0; k < objects.size(); k++) {

                    t = objects[k]->getIntersectingT(&reflectionRay);


                    if(t <= 0) {
                        continue;
                    }

                    if (t < t_min) {
                        nearest = k; t_min = t;
                    }
                }

                double colorAt[3];

                if(nearest!=-1) {


                    t = objects[nearest]->intersect(&reflectionRay, colorAt,  level+1);
                    double reflection_val = co_efficients[3];
                    for(int k = 0;k<3;k++){
                        if(k == 0) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 1) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 2) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                    }

                }
            }
            //Check whether all current_color pixel value is within 1 or 0 if not set it

            for(int b = 0;b<3;b++){
                modifiedColor[b] = (modifiedColor[b]>1)?1:modifiedColor[b];
                modifiedColor[b] = (modifiedColor[b]<0)?0:modifiedColor[b];
            }


        }

        return t;
    }



};

class Triangle: public Object {

public:

    Vector3 a;
    Vector3 b;
    Vector3 c;

    Triangle(Vector3 a, Vector3 b, Vector3 c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    Vector3 cross_product(Vector3 a,Vector3 b){
        double new_a = a.y*b.z - a.z*b.y;
        double new_b = a.z*b.x - a.x*b.z;
        double new_c = a.x*b.y - a.y*b.x;
        Vector3 result(new_a,new_b,new_c);
        return result;
    }

    void draw() {
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    void setColorAt(){
        modifiedColor[0] = color[0]*co_efficients[0] ;//AMBIENCE
        modifiedColor[1] = color[1]*co_efficients[0] ;
        modifiedColor[2] = color[2]*co_efficients[0] ;
    }



    Vector3 getNormal(Vector3 intersection) {

        return cross_product(b - a, c - a).normalize_vector();
    }

    double getIntersectingT(Ray* r) {
        double EPSILON = 0.0000001;
        Vector3  edge1, edge2, h, s, q;
        double a_,f,u,v;
        edge1 = b - a;
        edge2 = c - a;

        h = cross_product(r->dir, edge2);
        a_ = dot_product(edge1, h);

        if(a_ > -EPSILON && a_ < EPSILON) {
            return -1;
        }

        f = 1.0 / a_;

        s = r->start - a;

        u =f* dot_product(s, h) ;

        if (u < 0.0 || u > 1.0) {
            return -1;
        }

        q = cross_product(s, edge1);

        v = f*dot_product(r->dir, q);

        if (v < 0.0 || u + v > 1.0){
            return -1;
        }

        double t = f*dot_product(edge2, q) ;

        if(t > EPSILON) { //ray intersection
            return t;
        }

        return -1;
    }

    Vector3 getReflection(Ray* ray, Vector3 normal) {
        //out = incidentVec - 2.f * Dot(incidentVec, normal) * normal;
        //reflected vector should be normal
        Vector3 incidentVec = ray->dir;
        double dot_ =  dot_product(incidentVec, normal);
        Vector3 temp = normal*(2.0*dot_);
        Vector3 out = incidentVec - temp;
        return (incidentVec - temp).normalize_vector();
    }


    double intersect(Ray *r, double *current_color, int level){
        double t= getIntersectingT(r) ;//perform computation of intersection here

        if( t<=0) return -1;
        if(level==0)return t;

        Vector3 intersectionPoint = r->start+ r->dir*t;
        setColorAt();

        Vector3 normal=getNormal(intersectionPoint);
        Vector3 reflection=getReflection(r, normal);

        int limitIllum = lights.size();
        for(int i = 0;i<limitIllum;i++){
           //direction= (lightSource-intersectionPoint) //normalize it
            Vector3 lightSource = lights[i];
            Vector3 direction = (lightSource-intersectionPoint);
            double direction_length = dot_product(direction,direction);
            direction_length = sqrt(direction_length);
            direction = (lightSource-intersectionPoint).normalize_vector();
            Vector3 start = intersectionPoint + direction*1.0;
            //1 is for taking slightly above the point so
            //it doesnft again intersect with same object due to precision
            Ray L(start, direction);

            bool obscure_ray = false;
            int limitObj = objects.size();
            //For each object now check whether this L ray obscured by any object or not.

            for (int j=0; j < limitObj; j++) {
                //ray joto dur jabbeh tar moddhe jodi t thakkeh tailleh toh obstruction hocche
                double t_ = objects[j]->getIntersectingT(&L);

                if(t_ < 0 ) {
                    continue;
                }
                if( t_ > direction_length) continue;

                obscure_ray = true;
                break;
            }

            //If it is not obscured that means light falls
            //onto the intersection point so you have update current_color,
            if(obscure_ray == false){

                //Calculate lambert value,
                //Calculate phong value
                //check the illumination slide for formula
                //lambert value calculated by taking the dot product of the surface's normal vector, N and
                //a normalized light-direction vector,L DIFFUSE REFLECTION
                double lambert_value =dot_product(direction,normal);
                if(lambert_value<0) lambert_value = 0;
                //PHONG VALUE -> sPECULAR REFLECTION
                double phong_value =dot_product(r->dir,reflection);
                phong_value = pow(phong_value,shine);
                if(phong_value<0) phong_value = 0;

                //current_color+=source_factor*lambert*co_efficient[DIFFIUSE]*colorAt
                //current_color+=source_factor*pow(phong,shine)*co_efficient[SPECULAR]*colorAt
                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*lambert_value*co_efficients[1]*color[a];//DIFFUSE
                }

                for( int a = 0;a<3;a++){
                    modifiedColor[a] += source_factor*phong_value*co_efficients[2]*color[a];//SPECULAR
                }

            }


            if(level<recursion_level){
                double t_min = 2147483647 ; double t;
                start=intersectionPoint+reflection*1.0 ; //slight up to avoid own intersection
                Ray reflectionRay(start, reflection);
                //Like capture method,  find the nearest intersecting object,
                // using intersect function
                int nearest=-1;
                for (int k=0; k < objects.size(); k++) {

                    t = objects[k]->getIntersectingT(&reflectionRay);


                    if(t <= 0) {
                        continue;
                    }

                    if (t < t_min) {
                        nearest = k; t_min = t;
                    }
                }

                double colorAt[3];

                if(nearest!=-1) {


                    t = objects[nearest]->intersect(&reflectionRay, colorAt,  level+1);
                    double reflection_val = co_efficients[3];
                    for(int k = 0;k<3;k++){
                        if(k == 0) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 1) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                        if(k == 2) modifiedColor[k] += objects[nearest]->modifiedColor[k]*reflection_val;
                    }
                }
            }
            //Check whether all current_color pixel value is within 1 or 0 if not set it

            for(int b = 0;b<3;b++){
                modifiedColor[b] = (modifiedColor[b]>1)?1:modifiedColor[b];
                modifiedColor[b] = (modifiedColor[b]<0)?0:modifiedColor[b];
            }


        }

        return t;
    }
};

class General_Quadratic: public Object {

public:

    double* co_effi_quad;


    General_Quadratic(double coefficients[], Vector3 ref_pt, double length, double width,double height) {
        this->co_effi_quad = coefficients;

        this->height = height;
        this->width = width;
        this->length = length;

        this->reference_point = ref_pt;
    }

    Vector3 getNormal(Vector3 intersectionPoint) {

        double A = co_effi_quad[0];
        double B = co_effi_quad[1];
        double C = co_effi_quad[2];
        double D = co_effi_quad[3];
        double E = co_effi_quad[4];
        double F = co_effi_quad[5];
        double G = co_effi_quad[6];
        double H = co_effi_quad[7];
        double I = co_effi_quad[8];
        double J = co_effi_quad[9];

        double i_x = intersectionPoint.x;
        double i_y = intersectionPoint.y;
        double i_z = intersectionPoint.z;

        double x_ = 2 * A * i_x + D * i_y + E * i_z  + G;
        double y_ = 2 * B * i_y+ D * i_x + F * i_z  + H;
        double z_ = 2 * C * i_z + F * i_y + E * i_x + I;

        Vector3 normal(x_, y_, z_);

        return normal.normalize_vector();
    }

    void draw() {}

};


#endif // FILE2_H_INCLUDED
