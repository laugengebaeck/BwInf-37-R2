#include<bits/stdc++.h>
#define pdd pair<double,double>

using namespace std;

class Triangle{
    public:

    pdd point1;
    pdd point2;
    pdd point3;
    int id;

    Triangle(pdd p1, pdd p2, pdd p3, int idd){
        point1 = p1;
        point2 = p2;
        point3 = p3;
        id = idd;
    }
};