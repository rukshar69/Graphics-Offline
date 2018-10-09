
#include <iomanip>
#include <math.h>
#include "bitmap_image.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include<queue>
#define division(a,b) a/b
#define RAND rand()%256
#define MAX 9999999


using namespace std;

typedef struct P{
    double x,y,z;
}Point;



double **z_buffer;
double dx ,dy ,Top_y,Left_x,z_max;
double Screen_Width,Screen_Height,x_limit_right,x_limit_left,y_limit_top,y_limit_bottom,z_front_limit,
z_rear_limit;

bitmap_image *image;

Point dir_vector(Point p1, Point p2){

    Point p;
    p.x = p1.x - p2.x;
    p.y = p1.y - p2.y;
    p.z = p1.z - p2.z;
    return p;
}

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

        vector<double> crossVector(){
            Point dir1 = dir_vector(points[0],points[1]);


            Point dir2=dir_vector(points[2],points[1]);
            double A,B,C;
            vector<double> ABC;
            double a1 = dir1.y * dir2.z;
            double a2 = dir1.z * dir2.y;
            A = a1 - a2;
            ABC.push_back(A);
            double b1 = dir1.z * dir2.x;
            double b2 = dir1.x * dir2.z;
            B =b1 -b2;
            ABC.push_back(B);
            double c1= dir1.x * dir2.y;
            double c2 = dir2.x * dir1.y;
            C = c1 - c2;
            ABC.push_back(C);

            return ABC;
        }
};
queue <Triangle> triangles;
int graph_to_screen(double y_coord){
    double temp1 = Top_y-y_coord;
    temp1 = round(temp1/dy);
    return (int)temp1;
}


int x_grid(double x){
    double temp = (x-Left_x);
    temp =round(temp /dx);
    return (int)temp;
}

class One_Edge{
public:
    int ID;
    double delX;
    double currentX;
    double yMin;
    double xAtYMin;
    double yMax;
    double xAtYMax;

    One_Edge(Point p1, Point p2,int id){
            this->ID = id;
            Point mod_p1, mod_p2;
             mod_p1.x = x_grid(p1.x);
            mod_p1.y = graph_to_screen(p1.y);
            mod_p2.x = x_grid(p2.x);
            mod_p2.y = graph_to_screen(p2.y);

            this->delX = (mod_p2.x-mod_p1.x)/(mod_p2.y -mod_p1.y );

            if(mod_p1.y<mod_p2.y){

                this->xAtYMax = mod_p2.x;
                this->xAtYMin = mod_p1.x;
                this->yMin = mod_p1.y;
                this->yMax = mod_p2.y;
            }
            else if(mod_p1.y>mod_p2.y){

                this->xAtYMax = mod_p1.x;
                this->xAtYMin = mod_p2.x;
                this->yMin = mod_p2.y;
                this->yMax = mod_p1.y;

            }

            this->currentX = this->xAtYMin;

    }



    double findX(double yGiven){

        double dir_x = xAtYMax - xAtYMin;
        double dir_y = yMax - yMin;


        double newX = xAtYMin + dir_x*((yGiven - yMin)/dir_y);
        return newX;
    }


    void clipping(){

        if(this->yMin<0){
                    double xNew = this->findX(0);
                    this->yMin = 0;
                    this->xAtYMin = xNew;
                    this->currentX = xNew;
                }

                if(this->yMax>Screen_Height){
                    double xNew = this->findX(Screen_Height);
                    this->yMax = Screen_Height;
                    this->xAtYMax = xNew;
                }

    }
};


class One_Polygon{
public:

    int color[3];
    double A,B,C,D;
    int inOut;
    int ID;
    One_Polygon(int id, int col[]){
        this->ID = id;
        int i  =0 ;
        inOut = 0;
        while(i<3){
            color[i] = col[i];
            i++;
        }
    }

    double determine_z(double x,double y){
        double temp1 = (A*x + B*y +D);
        return temp1/(-C);
    }

    void determine_factors(vector<double>crssV, Point p){

            this->A = crssV[0];
            this->B = crssV[1];
            this->C = crssV[2];
            this->D =-(p.x * this->A + p.y * this->B + p.z * this->C) ;

    }


};



vector <One_Edge*> *ET;
vector <One_Polygon*> PT;


class Init_Table{
private:

    bool isInValid(One_Edge* tempOne_Edge){
        bool validity = false;
        if(tempOne_Edge->yMax<0){
            return true;
        }
        if (tempOne_Edge->yMin>Screen_Height){
            return true;
        }
        double maxi = max(tempOne_Edge->xAtYMin,tempOne_Edge->xAtYMax);
        if(maxi<0){
            return true;
        }
        double mini = min(tempOne_Edge->xAtYMin,tempOne_Edge->xAtYMax);
        if (mini>Screen_Width){
            return true;
        }
        if(isinf(tempOne_Edge->delX )){
            return true;
        }
        return validity;
    }
    void determine_edge(Point p1, Point p2,int id){


            One_Edge *tempOne_Edge = new One_Edge(p1,p2,id);


            if(isInValid(tempOne_Edge)==false){


                tempOne_Edge->clipping();

                push_Edge_Bucket(tempOne_Edge);
            }
    }

    void push_Edge_Bucket(One_Edge* node){

        ET[(int)node->yMin].push_back(node);
        int n = ET[(int)node->yMin].size();
        int i, j;
        for (i = 0; i < n-1; i++)
           for (j = 0; j < n-i-1; j++)
               if (ET[(int)node->yMin][j]->xAtYMin> ET[(int)node->yMin][j+1]->xAtYMin){

                    One_Edge* act1 = ET[(int)node->yMin][j];
                    One_Edge* act2=ET[(int)node->yMin][j+1];
                    ET[(int)node->yMin][j]= act2;
                    ET[(int)node->yMin][j+1]= act1;
               }

    }

public:
    Init_Table(){}

    void initializeEdgePT(){

        ET = new vector <One_Edge*> [(int)Screen_Height];
        int t = 0;
        while(!triangles.empty()){
            Triangle tri = triangles.front();
            triangles.pop();

            One_Polygon *tempOne_Polygon = new One_Polygon(t, tri.color);


            vector<double> crssV = tri.crossVector();

            tempOne_Polygon->determine_factors(crssV,tri.points[0]);

            PT.push_back(tempOne_Polygon);

            determine_edge(tri.points[0],tri.points[1],t);
            determine_edge(tri.points[0],tri.points[2],t);
            determine_edge(tri.points[1],tri.points[2],t);


            t++;
        }
    }

};

vector <One_Edge*> activeET;
vector <One_Polygon*> activePT;

void swap_edges(int index1, int index2){
    One_Edge* act1 = activeET[index1];
    One_Edge* act2=activeET[index2];
    activeET[index1] = act2;
    activeET[index2] = act1;
}

void bubbleSort_AET(){

    int n = activeET.size();
    int i, j;
    for (i = 0; i < n-1; i++)
       for (j = 0; j < n-i-1; j++)
           if (activeET[j]->currentX> activeET[j+1]->currentX){
                swap_edges(j,j+1);
           }

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

               i++;

            }


            Triangle triangle(points);


            triangles.push(triangle);
        }

    }
    void readAndInit(){
        read_files();

        buffer();
    }
};

class CleanUp{

public:

    string img_fname = "2.bmp";
    string z_txt = "z_buffer.txt";
    ofstream buffer_save;

    void save(){


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


class ScanLineAlgo{
private:
    void update_AET(int scan_no){
        vector <One_Edge*> temp_AET;
            int i = 0; int sz_AET = activeET.size();
            while(i<sz_AET){
                if(scan_no < (int)(activeET[i]->yMax) ){
                    activeET[i]->currentX+=activeET[i]->delX;
                    temp_AET.push_back(activeET[i]);
                }
                i++;
            }
            activeET.clear();
            activeET = temp_AET;

    }

    void addNewEdge_AET(int scan_no){
        int i = 0;
        int bucketSz = ET[scan_no].size();
        while(i<bucketSz){
                One_Edge *node = ET[scan_no][i];
                activeET.push_back(node);
                i++;
            }
    }

    bool isPresent_APL(One_Polygon* node){
        int i=0;
        int n = activePT.size();
        while(i<n){
            if(activePT[i]->ID==node->ID){
                return true;

            }
            i++;
        }
        return false;
    }


    int determine_color(double pX,double pY){
        int result_index = -1;

        double X =  Left_x + dx*(int)pX;
        double Y =  Top_y - (int)pY*dy;


        double min_Z = MAX;
        int i = 0; int sz = activePT.size();
        while(i<sz){
            if(activePT[i]->inOut==1){
                double z = activePT[i]->determine_z(X,Y);
                if(z<min_Z){
                    min_Z = z;
                    result_index = i;
                }


            }

             i++;
        }

        return result_index;
    }
    void coloring(int scan_no){
        int i = 0;
        int limit = (int)activeET.size()-1;
        while(i<limit){
                int inout_t = PT[activeET[i]->ID]->inOut;
                if(inout_t){
                    inout_t = 0;
                    PT[activeET[i]->ID]->inOut =inout_t;
                }
                else{
                    inout_t = 1;
                    PT[activeET[i]->ID]->inOut =inout_t;
                }

                if(isPresent_APL(PT[activeET[i]->ID])){}
                else{
                    activePT.push_back(PT[activeET[i]->ID]);
                }


                if(activePT.size()!=0){
                    int whichPolygon = determine_color(activeET[i]->currentX,scan_no);
                    if(whichPolygon!=-1){
                        int left= (int)activeET[i]->currentX;
                        left = (left<0)? 0:left;
                        int right= (int)activeET[i+1]->currentX;
                        right = (right>Screen_Width)? Screen_Width:right;

                        int j = left;
                        while(j<right){

                            int r = activePT[whichPolygon]->color[0];
                            int g =activePT[whichPolygon]->color[1];
                            int b = activePT[whichPolygon]->color[2];

                            Draw* d_ = new Draw(scan_no,j,r,g,b);
                            d_->fill_cell();
                            j++;
                    }

                }


                }

                i++;
            }
    }
public:
    void scanline_algo(){
        int i;
        int scan_no = 0;
        while(scan_no<Screen_Height){

            addNewEdge_AET(scan_no);
            bubbleSort_AET();
            coloring(scan_no);


            update_AET(scan_no);

            i = 0;
            int sz = activePT.size();
            while(i<sz){
                activePT[i]->inOut = 0;
                i++;
            }
            activePT.clear();
            bubbleSort_AET();

            scan_no++;
    }


    }

};




int main(){

    Read_Init* rd = new Read_Init();
    rd->readAndInit();

    Init_Table *init = new Init_Table();
    init->initializeEdgePT();

    ScanLineAlgo* sc =new ScanLineAlgo();
    sc->scanline_algo();


    CleanUp* c = new CleanUp();
    c->save();
    c->free_memory();
}
