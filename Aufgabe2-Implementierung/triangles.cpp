#include<bits/stdc++.h>

using namespace std;

// Klasse für einen Punkt (x/y-Koordinate)
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

// KLasse für einen Vektor im R^2
class Vektor{
    public:

    double x;
    double y;

    Vektor(double _x, double _y){
        x = _x;
        y = _y;
    }

    // Vektor zwischen zwei Punkten
    Vektor(Point a, Point b){
        x = b.x - a.x;
        y = b.y - a.y;
    }

    Vektor(){
        x = 0;
        y = 0;
    }

    // Betrag/Laenge des Vektors
    double betrag(){
        return sqrt(x * x + y * y);
    }
};

// Klasse für ein Dreieck
class Triangle{
    public:

    // Punkte + Vektoren der Seiten + Längen dieser
    vector<Point> points;
    vector<Vektor> vektoren;
    vector<double> lengths;
    int id;

    Triangle(Point p1, Point p2, Point p3, int idd){
        points = {p1,p2,p3};
        id = idd;
        reGenVectors();
        for(int i=0;i<=2;i++){
            lengths.push_back(vektoren[i].betrag());
        }
    }

    // Vektoren nach Drehung etc. neu erstellen
    void reGenVectors(){
        Vektor p1p2 = Vektor(points[0],points[1]);
        Vektor p2p3 = Vektor(points[1],points[2]);
        Vektor p3p1 = Vektor(points[2],points[0]);
        vektoren = {p1p2,p2p3,p3p1};
    }

    // Berechnung kürzeste an einem Punkt anliegende Seite
    double shortestLength(int bestPoint){
        switch(bestPoint) {
        case 0: if(lengths[0] < lengths[2]){
                    return lengths[0];
                } else {
                    return lengths[2];
                }
                break;
        case 1: if(lengths[0] < lengths[1]){
                    return lengths[0];
                } else {
                    return lengths[1];
                }
                break;
        case 2: if(lengths[1] < lengths[2]){
                    return lengths[1];
                } else {
                    return lengths[2];
                }
                break;
        default: return 0;
                 break;
        }
    }
};

// Vektor zu Punkt addieren
Point addVektor(Point &p, Vektor &v){
    p.x += v.x;
    p.y += v.y;
    return p;
}

// Skalarprodukt zweier Vektoren
double dotProduct(Vektor &v1, Vektor &v2){
    return v1.x * v2.x + v1.y * v2.y;
}

// Winkel zwischen zwei Vektoren
// allgemein bekannte Cosinus-Formel
double angle(Vektor &v1, Vektor &v2){
    double cosvalue = dotProduct(v1,v2)/(v1.betrag()*v2.betrag());
    return acos(abs(cosvalue));
}

// kleinsten Winkel und anliegenden Punkt eines Dreiecks bestimmen
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

// Punkt mithilfe einer Drehmatrix rotieren um ein Zentrum rotieren
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

// Winkel zur positiven x-Achse mit atan2
double atan_angle(Point center, Point p){
    double dx = p.x - center.x;
    double dy = p.y - center.y;

    double angle = atan2(dy,dx);
    if(dy < 0){
        angle += 2 * M_PI;
    }

    return angle;
}

// 360 Grad minus diesen Winkel 
// (zum anfänglichen Drehen, so dass Dreieck auf x-Achse liegt)
double atan360(Point center, Point p){
    return 2 * M_PI - atan_angle(center,p);
}

// ähnlich (Berechnen des freien Winkels links)
double atan180(Point center, Point p){
    return M_PI - atan_angle(center,p);
}

// Lage Punkt c von ab aus
// CCW: < 0, CW: >0
double ccw(Point a, Point b, Point c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Punkt finden, von dem aus Drehwinkel bestimmt wird
// (beim Drehen auf die x-Achse)
int findAngleCalcPoint(Triangle &t, int bestPoint){
    //just return the point that is counterclockwise from line between other points
    switch(bestPoint) {
        case 0: if(ccw(t.points[0],t.points[1],t.points[2]) > 0){
                    //point 2 is clockwise
                    return 1;
                } else {
                    //point 2 is counterclockwise
                    return 2;
                }
                break;
        case 1: if(ccw(t.points[1],t.points[0],t.points[2]) > 0){
                    //point 2 is clockwise
                    return 0;
                } else {
                    //point 2 is counterclockwise
                    return 2;
                }
                break;
        case 2: if(ccw(t.points[2],t.points[1],t.points[0]) > 0){
                    //point 0 is clockwise
                    return 1;
                } else {
                    //point 0 is counterclockwise
                    return 0;
                }
                break;
        default: return 0;
                 break;
    }
}
