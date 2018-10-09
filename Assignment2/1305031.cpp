#include <iostream>
#include<vector>
#include <fstream>
#include<string>
#include<math.h>
#include <iomanip>
#include<stack>

using namespace std;
ifstream sc;
vector<double>look;
vector<double>eye;
vector<double>up;
vector<double>gluPerspectiveParams;

vector<double>take_input(int limit){

    vector<double> v;
    for(int i = 0;i<limit;i++){
        double temp;
        sc>>temp;
        v.push_back(temp);
    }
    return v;
}
void print_vector(vector<double>v){
    int n = v.size();
    for(int i = 0;i<n;i++){
        cout<<v[i]<<" ";
    }
    cout<<endl;
}

class Vector{

    vector<double>v;
public:
    Vector(){
    }
    vector<double>subtraction(vector<double> a,vector<double> b){
        vector<double> ans;
        int s = a.size();
        for(int i = 0;i<s;i++){
            ans.push_back(a[i]-b[i]);
        }
        return ans;
    }

    vector<double>cross(vector<double> a,vector<double> b){
        vector<double> ans;
        ans.push_back(a[1]*b[2]-a[2]*b[1]);
        ans.push_back(a[2]*b[0]-a[0]*b[2]);
        ans.push_back(a[0]*b[1]-a[1]*b[0]);
        return ans;
    }

    double dotPro(vector<double> a,vector<double> b){
        double ans = 0;
        for(int i = 0;i<3;i++){
            ans += a[i]*b[i];
        }
        return ans;
    }

    vector<double>unit(vector<double> a){
        vector<double> ans;
        int s = a.size();
        double sqr = 0;
        for(int i = 0;i<s;i++){
            sqr += a[i]*a[i];
        }
        sqr = sqrt(sqr);

        for(int i = 0;i<s;i++){
            ans.push_back(a[i]/sqr);
        }
        return ans;
    }


};

class FourBy4Matrix{
private :
    double** matrix;
    vector<double>projection_param;
    vector<double>E;
    vector<double>L;
    vector<double>U;

    void mem_alloc(int s){
        matrix = new double*[s];

        int i = 0;
        while(i<4){
            matrix[i]=new double[s];
            i++;
        }
    }

    void init_array(int value){
        int i =0;
        while(i<4){
            int j =0;
            while(j<4){
                matrix[i][j]=value;
                j++;
            }
            i++;
        }
    }

    void mod_fovY(){
        double pi=3.14159265358979323846;
        double temp = pi * projection_param[0]/180.0;

        projection_param[0] = temp;
    }

    double det_t(){
        double tanTerm = tan(projection_param[0]*.5);
        double temp = projection_param[2] * tanTerm;
        return temp;
    }
    double det_r(double fovX){

        double tanTerm = tan(fovX*.5);
        double temp = projection_param[2] * tanTerm;
        return temp;

    }

public:
    double ** multMatrix(double ** a, double ** b){
        FourBy4Matrix result ;
        double** res_mat = result.getMatrix();

        int i =0;
        while(i<4){
            int j =0;
            while(j<4){
                int k=0;
                while(k<4){
                    res_mat[i][j] += a[i][k] * b[k][j];
                    k++;
                }
                j++;
            }
            i++;
        }
        return res_mat;

    }
    void createView(vector<double>eye,vector<double>look,vector<double>up){
        E = eye;
        L = look;
        U = up;

        Vector v;
        vector<double> l = v.subtraction(L,E);
        l = v.unit(l);
        vector<double> r = v.cross(l,U);
         r = v.unit(r);
         vector<double> u = v.cross(r,l);
         u = v.unit(u);

         FourBy4Matrix T;
         T.makeIdentity();
         double** T_mat = T.getMatrix();

         T_mat[0][3] = -E[0];
        T_mat[1][3] = -E[1];
        T_mat[2][3] = -E[2];

        FourBy4Matrix R;
         double** R_mat = R.getMatrix();
         for(int i =0;i<3;i++){

            if(i ==0){
                for(int j = 0;j<3;j++){
                    R_mat[i][j] = r[j];
                }
            }
            else if(i ==1){
                for(int j = 0;j<3;j++){
                    R_mat[i][j] = u[j];
                }
            }
            else if(i ==2){
                for(int j = 0;j<3;j++){
                    R_mat[i][j] = -l[j];
                }
            }
         }
         R_mat[3][3]=1;

         double** res = multMatrix( R_mat, T_mat);
         matrix = res;


    }
    void createProjection(vector<double>gluPers){
        projection_param = gluPers;
        // fovY = projection_param[0];
        // aspectRatio = projection_param[1];
        // near = projection_param[2];
        // far = projection_param[3];
        mod_fovY();
        double fovX = projection_param[0] * projection_param[1];
        double t = det_t();
        double r = det_r(fovX);

        for(int i = 0;i<4;i++){
            if( i == 0){
                matrix[i][0] =projection_param[2]/r;
            }
            else if(i == 1){
                matrix[i][1] = projection_param[2]/t;
            }
            else if(i == 2){
                matrix[i][2] = -(projection_param[3]+projection_param[2])/(projection_param[3]-projection_param[2]);
                matrix[i][3] = -(2*projection_param[3]*projection_param[2])/(projection_param[3]-projection_param[2]);
            }
            else if(i == 3){
                matrix[i][2] = -1;
            }

        }

    }

    FourBy4Matrix(){

        mem_alloc(4);
        init_array(0);
    }

    double** getMatrix(){
        return matrix;
    }
    void printThisMatrix(){

        int i =0;
        while(i<4){
            int j =0;
            while(j<4){
                cout<<matrix[i][j]<<" ";
                j++;
            }
            cout<<endl;
            i++;
        }
    }

    void printGivenMatrix(double ** temp){
        cout<<"lala"<<endl;
        int i =0;
        while(i<4){
            int j =0;
            while(j<4){
                cout<<temp[i][j]<<" ";
                j++;
            }
            cout<<endl;
            i++;
        }
    }


    void makeIdentity(){
        int i = 0;
        while(i<4){
            matrix[i][i]=1;
            i++;
        }
    }

    void setMatrix(double** t){
        int i =0;
        while(i<4){
            int j =0;
            while(j<4){
                matrix[i][j] = t[i][j] ;
                j++;
            }
            i++;
        }

    }

    vector<double>multByColVect(double** mat,vector<double> v ){
        vector<double> ans ;
        int i =0;
        while(i<4){
            int j =0;
            double tp  = 0;
            while(j<4){
                  tp+=   mat[i][j]*v[j];

                j++;
            }
            ans.push_back(tp);
            i++;
        }

        i = 0;
        while(i<4){
            ans[i] = ans[i]/ans[3];
            i++;
        }
        return ans;
    }

};

class stk{
    stack<FourBy4Matrix>s;
public:

    stk(){}
    void pushOne(FourBy4Matrix f){
        double** t = f.getMatrix();
        FourBy4Matrix tp;
        tp.setMatrix(t);
        s.push(tp);
    }

    FourBy4Matrix popOne(){
        FourBy4Matrix temp = s.top();
        double** t = temp.getMatrix();
        //temp.printThisMatrix();
        s.pop();
        return temp;
    }
};

class Rodrigues{
    vector<double>create_cos_sin(double angle){
        double pi=3.14159265358979323846;
        vector<double>trig;
        double angle_radian = (angle*pi)/180.0;
        trig.push_back(cos(angle_radian));
        trig.push_back(sin(angle_radian));
        return trig;
    }
public:
    vector<double>formula(vector<double> x, vector<double>a, double angle){

        vector<double>cos_sin = create_cos_sin(angle);
        vector<double>first;
        vector<double>second;
        vector<double>third;
        vector<double>answer;

        Vector v;
        double a_dot_x = v.dotPro(x,a);
        vector<double>a_cross_x =  v.cross(a,x);
        for(int i =0;i<3;i++){
            first.push_back(cos_sin[0]*x[i]);
            second.push_back((1-cos_sin[0])*a_dot_x*a[i]);
            third.push_back(cos_sin[1]*a_cross_x[i]);

        }
        for(int i =0;i<3;i++){
            answer.push_back(first[i] + second[i] + third[i]);
        }
        return answer;
    }

    FourBy4Matrix rotation(vector<double>rot_vect){
        FourBy4Matrix result;
        vector<double>i;
        vector<double>j;
        vector<double>k;
        vector<double>a;
        for(int i =1;i<=3;i++){
            a.push_back(rot_vect[i]);
        }
        Vector v;
        a=v.unit(a);

        i.push_back(1);i.push_back(0);i.push_back(0);
        j.push_back(0);j.push_back(1);j.push_back(0);
        k.push_back(0);k.push_back(0);k.push_back(1);

        vector<double>c1 = formula(i,a,rot_vect[0]);
        //print_vector(c1);
        vector<double>c2= formula(j,a,rot_vect[0]);
        //print_vector(c2);
        vector<double>c3= formula(k,a,rot_vect[0]);
        //print_vector(c3);

        double** result_matrix = result.getMatrix();

        result_matrix[3][3] = 1;

        for(int i =0;i<3;i++){
            if(i == 0){

                for(int j =0;j<3;j++){
                    result_matrix[j][i] = c1[j];
                }
            }
            else if(i == 1){

                for(int j =0;j<3;j++){
                    result_matrix[j][i] = c2[j];
                }
            }
            else if(i == 2){

                for(int j =0;j<3;j++){
                    result_matrix[j][i] = c3[j];
                }
            }
        }




            result.setMatrix(result_matrix);
            return result;
        }

};

int getChoice(string str){
    if(str == "triangle") return 1;
    if(str == "translate") return 2;
    if(str == "scale") return 3;
    if(str == "rotate") return 4;
    if(str == "push") return 5;
    if(str == "pop") return 6;
    if(str == "end") return 7;
    return 0;

}
ofstream s1;


void output_line1( vector<double>v){

    s1<<std::fixed<<std::setprecision(7);
    s1<<v[0]<<" ";
    s1<<v[1]<<" ";
    s1<<v[2]<<endl;

}
ofstream s2;
void output_line2( vector<double>v){

    s2<<std::fixed<<std::setprecision(7);
    s2<<v[0]<<" ";
    s2<<v[1]<<" ";
    s2<<v[2]<<endl;

}
ofstream s3;
void output_line3( vector<double>v){

    s3<<std::fixed<<std::setprecision(7);
    s3<<v[0]<<" ";
    s3<<v[1]<<" ";
    s3<<v[2]<<endl;

}
main(){


    sc.open("scene.txt");
    eye = take_input(3);
    //print_vector(eye);
    look = take_input(3);
    //print_vector(look);
    up = take_input(3);
    //print_vector(up);
    gluPerspectiveParams = take_input(4);
    //print_vector(gluPerspectiveParams);

    FourBy4Matrix currentTranformation;
    currentTranformation.makeIdentity();
    //currentTranformation.printThisMatrix();
    FourBy4Matrix projectionMat;
    projectionMat.createProjection(gluPerspectiveParams);
    //projectionMat.printThisMatrix();
    FourBy4Matrix viewMat;
    viewMat.createView(eye,look,up);
    //viewMat.printThisMatrix();

    stk s_;

    bool flag = true;
    vector<double>p1;
    vector<double>p2;
    vector<double>p3;
    vector<double>transformedVector1;
    vector<double>transformedVector2;
    vector<double>transformedVector3;

    vector<double>viewVector1;
    vector<double>viewVector2;
    vector<double>viewVector3;

    vector<double>projVector1;
    vector<double>projVector2;
    vector<double>projVector3;

    vector<double>viewedVector;
    vector<double>projectedVector;
    double** currTransMatrix;
    double** viewMat_;
    double** projMat;

    vector<double>translateVector;
    vector<double>scaleVector;
    vector<double>rotateVector;

    double** a;
    FourBy4Matrix tp;
    Rodrigues rod;
    FourBy4Matrix translateMatrix ;
    FourBy4Matrix scaleMatrix ;
    s1.open("Stage1.txt");
    s2.open("Stage2.txt");
    s3.open("Stage3.txt");


    while(flag){

        string input;
        sc>>input;
        int choice = getChoice(input);
        switch(choice){
            case 1:
                cout<<"triangle"<<endl;
                p1 = take_input(3);
                p1.push_back(1);
                p2 = take_input(3);
                p2.push_back(1);
                p3 = take_input(3);
                p3.push_back(1);
                currTransMatrix = currentTranformation.getMatrix();
                //print_vector(p1);
                //print_vector(p2);
                //print_vector(p3);
                //currentTranformation.printGivenMatrix(currTransMatrix);
                transformedVector1 =currentTranformation.multByColVect(currTransMatrix,p1);
                transformedVector2 =currentTranformation.multByColVect(currTransMatrix,p2);
                transformedVector3 =currentTranformation.multByColVect(currTransMatrix,p3);
                output_line1(transformedVector1);
                output_line1(transformedVector2);
                output_line1(transformedVector3);
                s1<<endl;
                viewMat_ = viewMat.getMatrix();
                viewVector1 =viewMat.multByColVect(viewMat_ ,transformedVector1) ;
                viewVector2=viewMat.multByColVect(viewMat_ ,transformedVector2);
                viewVector3=viewMat.multByColVect(viewMat_ ,transformedVector3);
                output_line2(viewVector1);
                output_line2(viewVector2);
                output_line2(viewVector3);
                s2<<endl;

                projMat = projectionMat.getMatrix();
                projVector1 = projectionMat.multByColVect(projMat,viewVector1);
                projVector2 = projectionMat.multByColVect(projMat,viewVector2);
                projVector3 = projectionMat.multByColVect(projMat,viewVector3);
                output_line3(projVector1);
                output_line3(projVector2);
                output_line3(projVector3);
                s3<<endl;


                break;
            case 2:cout<<"translate"<<endl;
                translateVector = take_input(3);
                translateMatrix.makeIdentity();
                a = translateMatrix.getMatrix();
                a[0][3] = translateVector[0];
                a[1][3] = translateVector[1];
                a[2][3] = translateVector[2];
                currTransMatrix = currentTranformation.getMatrix();
                a = translateMatrix.multMatrix(currTransMatrix,a);
                currentTranformation.setMatrix(a);
                //currentTranformation.printThisMatrix();
                //translateMatrix.printGivenMatrix(a);
                //print_vector(translateVector);
            break;
            case 3:cout<<"scale"<<endl;
                scaleVector = take_input(3);
                scaleMatrix.makeIdentity();
                a = scaleMatrix.getMatrix();
                for(int i = 0;i<3;i++){
                    a[i][i] = scaleVector[i];
                }
                currTransMatrix = currentTranformation.getMatrix();
                a = scaleMatrix.multMatrix(currTransMatrix,a);
                currentTranformation.setMatrix(a);
                //currentTranformation.printThisMatrix();
                //scaleMatrix.printGivenMatrix(a);
                //print_vector(scaleVector);
            break;
            case 4:cout<<"rotate"<<endl;
                rotateVector = take_input(4);
                tp = rod.rotation(rotateVector);
                //tp.printThisMatrix();
                a = tp.getMatrix();
                currTransMatrix = currentTranformation.getMatrix();
                a = tp.multMatrix(currTransMatrix,a);
                currentTranformation.setMatrix(a);
                //print_vector(rotateVector);
            break;
            case 5:cout<<"push"<<endl;
                s_.pushOne(currentTranformation);
            break;
            case 6:cout<<"pop"<<endl;
                tp = s_.popOne();
                //double ** tp_matrix = tp.getMatrix();
                //currentTranformation.setMatrix(tp_matrix);
                currentTranformation = tp;
                cout<<"in pop"<<endl;
                //currentTranformation.printThisMatrix();
            break;
            case 7: cout<<"end"<<endl;flag = false;break;

        }
    }



}
