#include<iostream>
#include "FILE2.h"
using namespace std;

int main(){
    Vector3* v;
    double p[] = {3,4,5};
    v = new Vector3(p);
    v->printVector();

}
