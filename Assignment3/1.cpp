#include <math.h>
#include <iomanip>
#include "bitmap_image.hpp"
#include <iostream>
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <vector>
#define division(a,b) a/b
#define MAX 9999999
#define MIN -9999999
#define RAND rand()%256
using namespace std;

typedef struct P{
    double x,y,z;
}Point;

double **z_buffer;

double Screen_Width,Screen_Height,x_limit_right,x_limit_left,y_limit_top,y_limit_bottom,z_front_limit,
z_rear_limit;

bitmap_image *image;
double dx ,dy ,Top_y,Left_x,z_max;




class Triangle{
public:
    Point points[3];
    int color[3];

    Triangle(Point p[]){

        int i = 0;
        while (i<3){
            points[i] = p[i];
            color[i] = RAND;
            i++;
        }
    }
};



queue <Triangle*> triangles;




int x_grid(double x){
    double temp = (x-Left_x);
    temp =round(temp /dx);
    return (int)temp;
}

double maximum(double a, double b,double c){
    if(a>=b && a>=c){
        return a;
    }
    else if(b>=a && b>=c) return b;
    else if (c>=a && c>=b) return c;
}

double minimum(double a, double b,double c){
    if(a<=b && a<=c){
        return a;
    }
    else if(b<=a && b<=c) return b;
    else if (c<=a && c<=b) return c;
}


int graph_to_screen(double y_coord){
    double temp1 = Top_y-y_coord;
    temp1 = round(temp1/dy);
    return (int)temp1;
}

double scanline_modification(int i,double top_scanline, double bottom_scanline,double scanline){

    if(i==graph_to_screen(top_scanline) ){
                if (scanline>top_scanline)
                    scanline = top_scanline;
            }

            if(i==graph_to_screen(bottom_scanline) ){
                if ( scanline<bottom_scanline)
                    scanline = bottom_scanline;
            }

    return scanline;
}

int isIntersecting(double x , double y,double scanline){

    int result = 0;

    if(scanline>=x){
            if (scanline<=y)
                result=1;
            }

    if(scanline<=x ){
            if(scanline>=y)
                result=1;
    }
    return result;
}


double slope_factor(double a, double b, double c){
    double temp = a- b;
    double factor = temp/c;
    return factor;
}

Point dir_vector(Point p1, Point p2){

    Point p;
    p.x = p1.x - p2.x;
    p.y = p1.y - p2.y;
    p.z = p1.z - p2.z;
    return p;
}

class Draw{

public:
    int y_dim, x_dim,r,g,b;
    Draw(){}
    Draw(int y, int x, int r,int g, int b){
        this->y_dim = y;
        this->x_dim = x;
        this->r = r;
        this->g= g;
        this->b = b;
    }

    void draw_image(){
        image = new bitmap_image(y_dim,x_dim);
        for(int i=0;i<y_dim;i++){
            for(int j=0;j<x_dim;j++){
                image->set_pixel(i,j,r,g,b);
            }
        }
    }

    void save_img(string filename){

        image->save_image(filename.c_str());
    }

    void fill_cell(){
        image->set_pixel(x_dim,y_dim,r,g,b);
    }

};




class CleanUp{

public:

    string img_fname = "1.bmp";
    string z_txt = "z_buffer.txt";
    ofstream buffer_save;

    void save(){
        buffer_save.open(z_txt.c_str());

        buffer_save<<std::fixed;
        buffer_save<<std::setprecision(6);
        int r =  (int) Screen_Height;
        int c = (int)Screen_Width;
        string space = "\t";
        int i = 0;
        while(i<r){
            int j = 0;
            while(j<c){
                if(z_buffer[i][j]<z_max){
                    buffer_save<<z_buffer[i][j]<<space;
                }
                j++;
            }
            i++;
            buffer_save<<endl;
        }

        Draw* s = new Draw();
        s->save_img(img_fname);
    }

    void free_memory(){
        free(z_buffer) ;
        free(image) ;


        while(!triangles.empty()){

            triangles.pop();
        }

    }


};


class Read_Init{
public:

    ifstream configuration;
    ifstream stage_three;
    string config_filename = "config.txt";
    string stage_filename = "stage3.txt";
    double x,y,z;

    void init_rect(){
        double x_dim, y_dim;
        configuration>>y_dim;
        configuration>>x_dim;
        Screen_Width = y_dim;
        Screen_Height = x_dim;
    }

    void init_lim(){
        double a,b,c,d;
        configuration>>a;
        configuration>>b;
        configuration>>c;
        configuration>>d;
        x_limit_left=a;
        y_limit_bottom=b;
        z_front_limit=c;
        z_rear_limit=d;
        x_limit_right = -1*x_limit_left;
        y_limit_top = -1*y_limit_bottom;
    }

    void init_grid(){

        double x_width, y_width;
        x_width = (x_limit_right-x_limit_left);
        y_width = (y_limit_top-y_limit_bottom);

        dx = division(x_width,Screen_Width);
        dy = division(y_width,Screen_Height);

        Top_y = dy/2.0;
        Top_y = y_limit_top-Top_y;

        Left_x = dx/2.0;
        Left_x = x_limit_left + Left_x;
    }

    void z_buffer_init(){
         z_max = z_rear_limit;

         int r = (int)Screen_Height;
         int c = (int)Screen_Width;
         z_buffer = (double**)malloc(r*sizeof(double*));
         int i =0;
         while(i<r){
            z_buffer[i] = (double *)malloc(c * sizeof(double));
            i++;
         }
         i = 0;
         while(i<r){
            int j = 0;
            while(j<c){
                z_buffer[i][j]=z_rear_limit;
                j++;
            }
            i++;
         }

    }

    void buffer(){

        init_grid();


        z_buffer_init();
        int r = (int)Screen_Height;
         int c = (int)Screen_Width;

        Draw* d = new Draw(c,r,0,0,0);
        d->draw_image();
    }





    void read_files(){
        configuration.open(config_filename.c_str());

        init_rect();
        init_lim();

        stage_three.open(stage_filename.c_str());

        double x,y,z;

        while(!stage_three.eof()){
            Point* points = new Point[3];
            int i =0;
            while(i<3){
               stage_three>>points[i].x>>points[i].y>>points[i].z;
               cout<<points[i].x<<endl;
               i++;

            }
            cout<<endl;

            Triangle* triangle = new Triangle(points);


            triangles.push(triangle);

        }
    }
    void readAndInit(){
        read_files();

        buffer();

    }
};



double top_scanline, bottom_scanline;

void determineScanlines(Triangle *tri){
    double a = tri->points[0].y;
        double b = tri->points[1].y;
        double c = tri->points[2].y;

       top_scanline =maximum(a,b,c);
        top_scanline = (top_scanline>y_limit_top)? y_limit_top:top_scanline;
        bottom_scanline = minimum(a,b,c);
        bottom_scanline =(bottom_scanline<y_limit_bottom)?y_limit_bottom:bottom_scanline;
}
int main(){
    Point dir_vector_buffer;

    Read_Init *rd = new Read_Init();
    rd->readAndInit();



    while(!triangles.empty()){
        Triangle* tri = triangles.front();
        triangles.pop();


        determineScanlines(tri);



        int i=graph_to_screen(top_scanline);
        while(i<=graph_to_screen(bottom_scanline)){

            double scanline = Top_y- i*dy;


            scanline = scanline_modification(i, top_scanline,bottom_scanline,scanline);

            int intersect1 = 0;int intersect2 = 0;int intersect3 = 0;



            double y1 = tri->points[0].y ;
            double y2= tri->points[1].y;
            double y3 = tri->points[2].y;

            intersect1 = isIntersecting(y1,y2,scanline);
            intersect2 = isIntersecting(y1,y3,scanline);
            intersect3 = isIntersecting(y2,y3,scanline);


            queue <double> x_intersection;
            queue <double> z_intersection;



            if(intersect1==1){
                Point vector_subtract = dir_vector(tri->points[1],tri->points[0]);
                double slope = slope_factor(scanline,tri->points[0].y,vector_subtract.y);
                double addendX = slope*vector_subtract.x;
                double addendZ = slope*vector_subtract.z;

                x_intersection.push(tri->points[0].x + addendX);
                z_intersection.push(tri->points[0].z + addendZ);
            }

            if(intersect2==1){
                Point vector_subtract = dir_vector(tri->points[2],tri->points[0]);

                double slope = slope_factor(scanline,tri->points[0].y,vector_subtract.y);
                double addendX = slope*vector_subtract.x;
                double addendZ = slope*vector_subtract.z;

                x_intersection.push(tri->points[0].x + addendX);
                z_intersection.push(tri->points[0].z + addendZ);
            }

            if(intersect3==1){
                Point vector_subtract = dir_vector(tri->points[2],tri->points[1]);

                double slope = slope_factor(scanline,tri->points[1].y,vector_subtract.y);
                double addendX = slope*vector_subtract.x;
                double addendZ = slope*vector_subtract.z;

                x_intersection.push(tri->points[1].x + addendX);
                z_intersection.push(tri->points[1].z + addendZ);
            }

            double right_intersecting_column = MIN;
            double maximum_Z = MIN;double left_intersecting_column = MAX;
            double minimum_Z = MAX;


            while(!x_intersection.empty()){
                double tempx = x_intersection.front();

                double tempz = z_intersection.front();

                if(tempx<left_intersecting_column){
                    left_intersecting_column = tempx;
                    minimum_Z = tempz;
                    if(left_intersecting_column<x_limit_left)
                        left_intersecting_column = x_limit_left;
                }
                z_intersection.pop();
                x_intersection.pop();
                if(tempx>right_intersecting_column){
                    right_intersecting_column = tempx;
                    maximum_Z = tempz;
                    if(right_intersecting_column>x_limit_right)
                        right_intersecting_column = x_limit_right;
                }


            }
            dir_vector_buffer.z = maximum_Z - minimum_Z;
            dir_vector_buffer.x = right_intersecting_column - left_intersecting_column;


            int j  =x_grid(left_intersecting_column);

            while(j<=x_grid(right_intersecting_column)){
                double c = j;
                double next_x = Left_x+c*dx;;


                double slope = slope_factor(next_x,left_intersecting_column,dir_vector_buffer.x);
                double addend = slope*dir_vector_buffer.z;
                double z_ = minimum_Z + addend;

                if(z_ >=z_buffer[i][j]){
                    //do nothing
                }
                else if(z_ <z_buffer[i][j]){

                    z_buffer[i][j] = z_ ;
                    Draw* d_ = new Draw(i,j,tri->color[0],tri->color[1],tri->color[2]);
                    d_->fill_cell();

                }

                j++;

            }
            i++;

        }

    }



    CleanUp* c = new CleanUp();
    c->save();
    c->free_memory();





}
