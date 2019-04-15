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

    Point point1;
    Point point2;
    Point point3;
    int id;
    Vektor p1p2;
    Vektor p2p3;
    Vektor p3p1;

    Triangle(Point p1, Point p2, Point p3, int idd){
        point1 = p1;
        point2 = p2;
        point3 = p3;
        id = idd;
        p1p2 = Vektor(point1,point2);
        p2p3 = Vektor(point2,point3);
        p3p1 = Vektor(point3,point1);
    }
};

Point addVektor(Point &p, Vektor &v){
    p.x += v.x;
    p.y += v.y;
    return p;
}

double dotProduct(Vektor &v1, Vektor &v2){
    return v1.x * v2.x + v1.y + v2.y;
}

double angle(Vektor &v1, Vektor &v2){
    double cosvalue = dotProduct(v1,v2)/(v1.betrag()*v2.betrag());
    return acos(abs(cosvalue));
}

pair<Point,double> locateSmallestAngle(Triangle t){
        double p1angle = angle(t.p1p2,t.p3p1);
        double p2angle = angle(t.p1p2,t.p2p3);
        double p3angle = angle(t.p2p3,t.p3p1);
        Point smallest = t.point1;
        double smallestAngle = p1angle;
        if(p2angle < smallestAngle){
            smallest = t.point2;
            smallestAngle = p2angle;
        }
        if(p3angle < smallestAngle){
            smallest = t.point3;
            smallestAngle = p3angle;
        }
        return {smallest,smallestAngle};
    }

