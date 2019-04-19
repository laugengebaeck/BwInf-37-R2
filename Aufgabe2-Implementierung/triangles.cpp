#include<bits/stdc++.h>

using namespace std;

class Point{
    public:

    double x;
    double y;

    Point(double _x, double _y){
        x = _x;
        y = _y;
    }

    Point(){
        x = 0;
        y = 0;
    }
};

class Vektor{
    public:

    double x;
    double y;

    Vektor(double _x, double _y){
        x = _x;
        y = _y;
    }

    Vektor(Point a, Point b){
        x = b.x - a.x;
        y = b.y - a.y;
    }

    Vektor(){
        x = 0;
        y = 0;
    }

    double betrag(){
        return sqrt(x * x + y * y);
    }
};

class Triangle{
    public:

    vector<Point> points;
    int id;
    vector<Vektor> vektoren;

    Triangle(Point p1, Point p2, Point p3, int idd){
        points = {p1,p2,p3};
        id = idd;
        reGenVectors();
    }

    void reGenVectors(){
        Vektor p1p2 = Vektor(points[0],points[1]);
        Vektor p2p3 = Vektor(points[1],points[2]);
        Vektor p3p1 = Vektor(points[2],points[0]);
        vektoren = {p1p2,p2p3,p3p1};
    }
};

Point addVektor(Point &p, Vektor &v){
    p.x += v.x;
    p.y += v.y;
    return p;
}

double dotProduct(Vektor &v1, Vektor &v2){
    return v1.x * v2.x + v1.y * v2.y;
}

double angle(Vektor &v1, Vektor &v2){
    double cosvalue = dotProduct(v1,v2)/(v1.betrag()*v2.betrag());
    return acos(abs(cosvalue));
}

pair<int,double> locateSmallestAngle(Triangle t){
    double bestangle = M_PI;
    int pointindex;
    for(size_t i=0;i<=2;i++){
        double thisangle = angle(t.vektoren[i],t.vektoren[(i+1)%3]);
        if(thisangle < bestangle){
            bestangle = thisangle;
            pointindex = (i+1)%3;
        }
    }
    return {pointindex,bestangle};
}

//based on https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
void rotate_tri(Point center, Point &p, double angle){
    double sinus = sin(angle);
    double cosinus = cos(angle);

    p.x -= center.x;
    p.y -= center.y;
    
    double xnew = p.x * cosinus - p.y * sinus;
    double ynew = p.x * sinus + p.y * cosinus;

    p.x = xnew + center.x;
    p.y = ynew + center.y;
}

double atan_angle(Point center, Point p){
    double dx = p.x - center.x;
    double dy = p.y - center.y;

    double angle = atan2(dy,dx);
    if(dy < 0){
        angle += 2 * M_PI;
    }

    return 2* M_PI - angle;
}

double ccw(Point a, Point b, Point c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

