#include<bits/stdc++.h>
#include"triangles.cpp"
#define EPSILON 10

using namespace std;

vector<int> bestPointIndex; // Punkt mit dem kleinsten Winkel (0,1 oder 2)
vector<int> bestAngle; // kleinster Winkel als int (für Subset Sum)
vector<double> bestAngleDouble; // kleinster Winkel als double
vector<int> sol; // Ausgabe des Subset Sum Algorithmus
vector<Triangle> triangles; // noch nicht platzierte Dreiecke
vector<Triangle> placedTriangles; // platzierte Dreiecke

// Backtracen der Subset Sum Lösung
// gibt nur ein mögliches Subset zurück
bool getSubsetsRec(vector<int> arr, int i, int sum, vector<int>& p, vector<vector<bool>> &dp) { 
    // erstes Element erreicht, aber Summe nicht 0 
    // -> dp[0][sum] muss true sein, damit Subset möglich ist
    if (i == 0 && sum != 0 && dp[0][sum]) { 
        p.push_back(i); 
        sol = p;
        return true; 
    } 
  
    // erstes Element erreicht und Summe 0 -> möglich
    if (i == 0 && sum == 0) { 
        sol = p;
        return true; 
    } 
  
    // wenn Summe ohne aktuelles Element ereicht werden kann
    if (dp[i-1][sum]) { 
        vector<int> b = p;
        if(getSubsetsRec(arr, i-1, sum, b,dp)){
            return true;
        } 
    } 
  
    // wenn Summe mit aktuellem Element erreicht werden kann
    if (sum >= arr[i] && dp[i-1][sum-arr[i]]) { 
        p.push_back(i);
        if(getSubsetsRec(arr, i-1, sum-arr[i], p,dp)){
            return true;
        }
    }
    return false;
} 

void subsetSum(vector<int> set,int sum){
    int n = set.size();
    vector<vector<bool>> dp(n,vector<bool>(sum+1));

    // Summe 0 immer erreichbar
    for (int i=0; i<n; ++i) {
        dp[i][0] = true; 
    } 
  
    // Summe set[0] kann mit erstem Element erreicht werden
    if (set[0] <= sum) 
       dp[0][set[0]] = true; 
  
    // DP-Array nach Rekursionsgleichung füllen 
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < sum + 1; ++j) {
            dp[i][j] = (set[i] <= j) ? (dp[i-1][j] || dp[i-1][j-set[i]]) : dp[i - 1][j]; 
        }
    }

    // größte Summe, die erreicht werden kann
    int best = sum;
    for(;best>=0;best--){
        if(dp[n-1][best]) break;
    }

    // Backtracen
    vector<int> p; 
    getSubsetsRec(set, n-1, best, p,dp);
}

// Sortieren einer Teilmenge von Dreiecken
// nach der längsten Seite, die am auf der x-Achse liegenden
// Punkt des Dreiecks (mit dem kleinsten Winkel) anliegt.
// "Kleine" Dreiecke sind dadurch weiter rechts.
bool lengthSortFunc(const int t1, const int t2){
    return triangles[t1].longestLength(bestPointIndex[t1]) < triangles[t2].longestLength(bestPointIndex[t2]);
}

// Sortieren der Dreiecke anhand ihres Mittelpunkts (x-Koordinate)
// zur geordneten Ausgabe
bool triangleSortFunc(const Triangle &t1, const Triangle &t2){
    double mid1 = (t1.points[0].x + t1.points[1].x + t1.points[2].x) / 3;
    double mid2 = (t2.points[0].x + t2.points[1].x + t2.points[2].x) / 3;
    return mid1 < mid2;
}

// aktuelles Subset aus den noch nicht platzierten Dreiecken löschen
// und zu den platzierten hinzufügen
void deleteUsedTriangles(){
    // in absteigender Reihenfolge der Indexe -> sonst: Segfault
    sort(sol.begin(),sol.end(),greater<int>());
    for(auto index: sol){
        placedTriangles.push_back(triangles[index]);
        triangles.erase(triangles.begin()+index);
        bestAngle.erase(bestAngle.begin()+index);
        bestAngleDouble.erase(bestAngleDouble.begin()+index);
        bestPointIndex.erase(bestPointIndex.begin()+index);
    }
}

// Dreiecke eines Subsets so verschieben, dass sie
// mit dem "besten" Punkt auf der x-Achse liegen
// und so rotieren, dass sie direkt auf der Achse liegen
void translateAndRotateToAxis(Point centerPoint){
    for(auto index : sol){
        auto &t = triangles[index];
        Vektor translation = Vektor(t.points[bestPointIndex[index]],centerPoint);
        for(int i=0;i<=2;i++){
            t.points[i] = addVektor(t.points[i],translation);
        }
        int rotatePoint = findAngleCalcPoint(t,bestPointIndex[index]);
        double rotateAngle = atan360(centerPoint,t.points[rotatePoint]);
        for(int i=0;i<=2;i++){
            rotate_tri(centerPoint, t.points[i], rotateAngle);
        }
    }
}

// für Anfügen auf der rechten Seite
// Dreiecke so rotieren, dass sie sich nicht überlappen
// anschließend an die bisherige Anordnung "randrehen"
void rotateToPositionRight(Point centerPoint, double free_angle, bool rotateLeft){
    double triRotateAngle = 0;
    for(size_t i=0;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], triRotateAngle);
        }
        triRotateAngle += bestAngleDouble[sol[i]];
    }
    // "Randrehen"
    if(rotateLeft){
        for(size_t i=0;i<sol.size();i++){
            auto &t = triangles[sol[i]];
            for(int j=0;j<=2;j++){
                rotate_tri(centerPoint, t.points[j], free_angle-triRotateAngle);
            }
        }
    }
}

// für Anfügen auf der linken Seite
// wird bedingt durch Sortierung der Dreiecke normalerweise nie benutzt
void rotateToPositionLeft(Point centerPoint, double free_angle){
    double triRotateAngle = 0;
    for(size_t i=0;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], M_PI - triRotateAngle - bestAngleDouble[sol[i]]);
        }
        triRotateAngle += bestAngleDouble[sol[i]];
    }
    // "Randrehen"
    for(size_t i=0;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], -(free_angle-triRotateAngle));
        }
    }
}

// linkesten und rechtesten Punkt finden,
// die zur Berechnung des Gesamtabstands herangezogen werden
pair<double,double> calculateDistance(){
    double leftmost=300,rightmost=300;
    for(auto tri: placedTriangles){
        double left_triangle = 100000, right_triangle = 0;
        for(auto p: tri.points){
            if(p.y == 0){
                if(p.x < left_triangle) left_triangle = p.x;
                if(p.x > right_triangle) right_triangle = p.x;
            }
        }
        if(left_triangle < 300 && right_triangle < leftmost) leftmost = right_triangle;
        if(right_triangle > 300 && left_triangle > rightmost) rightmost = left_triangle;
    }
    return {leftmost, rightmost};
}

// Anordnung so verschieben, dass der "linkeste" Punkt
// auf der y-Achse liegt
void moveToRightOfY(){
    double minx = 100000;
    for(auto t: placedTriangles){
        for(auto pnt: t.points){
            if(pnt.x < minx) minx = pnt.x;
        }
    }
    for(auto &t: placedTriangles){
        for(auto &pnt: t.points){
            pnt.x -= minx;
        }
    }
}

// Hauptmethode des Algorithmus
pair<vector<Triangle>,double> doAlgorithm(vector<Triangle> i_triangles){
    triangles = i_triangles;
    Point centerPoint = Point(300,0); // erster Anlegepunkt (willkürlich gewählt)

    // kleinsten Winkel und entsprechenden Punkt der Dreiecke berechnen
    for(size_t i = 0; i<triangles.size(); i++){
        auto &t = triangles[i];
        int ind;
        double angle;
        tie(ind,angle) = locateSmallestAngle(t);
        bestPointIndex.push_back(ind);
        bestAngle.push_back((int) ceil(10000*angle));
        bestAngleDouble.push_back(angle);
    }

    // Subset-Sum ausführen und nach Dreiecke nach Seitenlänge ordnen
    subsetSum(bestAngle,(int) floor(10000*M_PI));
    sort(sol.begin(),sol.end(),lengthSortFunc);

    // Dreiecke passend verschieben und rotieren
    translateAndRotateToAxis(centerPoint);
    rotateToPositionRight(centerPoint,M_PI,false);
    deleteUsedTriangles();

    // solange nicht alle Dreiecke platziert
    double leftmost, rightmost;
    while(!triangles.empty()){
        // Punkte für die Berechnung der Gesamtdistanz
        tie(leftmost,rightmost) = calculateDistance();
        
        // min./max. Punkt finden, an dem angelegt werden kann
        // ist ein Punkt eines platzierten Dreiecks
        double minx_axis = 100000, maxx_axis = 0;
        for(auto t: placedTriangles){
            for(auto pnt: t.points){
                if(pnt.y <= EPSILON){
                    if(pnt.x < minx_axis) {minx_axis = (pnt.y > 0) ? pnt.x - pnt.y : pnt.x;}
                    if(pnt.x > maxx_axis) {maxx_axis = (pnt.y > 0) ? pnt.x + pnt.y : pnt.x;}
                }
            }
        }

        Point newCenterRight = Point(maxx_axis,0);
        Point newCenterLeft = Point(minx_axis,0);

        // freien Winkel (links/rechts) finden
        // wird durch den Punkt bestimmt, der den geringsten Winkel/atan vom Anlegepunkt hat
        Point minpnt_above, maxpnt_above;
        double angle_left = M_PI, angle_right = M_PI;
        for(auto t: placedTriangles){
            for(auto pnt: t.points){
                if(pnt.y == 0) continue;
                if(atan_angle(newCenterRight,pnt) < angle_right) { angle_right = atan_angle(newCenterRight,pnt); maxpnt_above = pnt;}
                if(atan180(newCenterLeft,pnt) < angle_left) {angle_left = atan_angle(newCenterLeft,pnt); minpnt_above = pnt;}
            }
        }

        // links oder rechts platzieren - was ist besser?
        // normalerweise rechts
        bool left = false;
        double free_angle = 0;
        if(leftmost - newCenterLeft.x < newCenterRight.x - rightmost){
            //place left
            free_angle = atan180(newCenterLeft,minpnt_above);
            left = true;
        } else {
            //place right
            free_angle = atan_angle(newCenterRight,maxpnt_above);
        }

        // Subset Sum + Sortieren (wie oben)
        subsetSum(bestAngle,(int) floor(10000*free_angle));        
        sort(sol.begin(),sol.end(),lengthSortFunc);

        // Links oder rechts platzieren
        // Links zuerst Reihenfolge umdrehen
        if(left){
            reverse(sol.begin(),sol.end());
            translateAndRotateToAxis(newCenterLeft);
            rotateToPositionLeft(newCenterLeft,free_angle);
        } else {
            translateAndRotateToAxis(newCenterRight);
            rotateToPositionRight(newCenterRight,free_angle,true);
        }
        deleteUsedTriangles();
    }

    // Distanz und zur Anzeige verschieben/sortieren
    auto dpair = calculateDistance();
    double dist = abs(dpair.second - dpair.first);
    moveToRightOfY();
    sort(placedTriangles.begin(),placedTriangles.end(),triangleSortFunc);

    return {placedTriangles,dist};
}