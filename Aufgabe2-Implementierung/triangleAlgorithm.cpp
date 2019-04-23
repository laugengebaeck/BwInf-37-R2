#include<bits/stdc++.h>
#include"triangles.cpp"
#define EPSILON 0.5

using namespace std;

vector<int> bestPointIndex;
vector<int> bestAngle;
vector<double> bestAngleDouble;
vector<int> sol;
vector<Triangle> triangles;
vector<Triangle> placedTriangles;

bool getSubsetsRec(vector<int> arr, int i, int sum, vector<int>& p, vector<vector<bool>> &dp) 
{ 
    // If we reached end and sum is non-zero. We print 
    // p[] only if arr[0] is equal to sun OR dp[0][sum] 
    // is true.
    if (i == 0 && sum != 0 && dp[0][sum]) 
    { 
        p.push_back(i); 
        sol = p;
        return true; 
    } 
  
    // If sum becomes 0 
    if (i == 0 && sum == 0) 
    { 
        sol = p;
        return true; 
    } 
  
    // If given sum can be achieved after ignoring 
    // current element.
    if (dp[i-1][sum]) 
    { 
        // Create a new vector to store path 
        vector<int> b = p; 
        if(getSubsetsRec(arr, i-1, sum, b,dp)){
            return true;
        } 
    } 
  
    // If given sum can be achieved after considering 
    // current element.
    if (sum >= arr[i] && dp[i-1][sum-arr[i]]) 
    { 
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
    for (int i=0; i<n; ++i) {
        dp[i][0] = true; 
    } 
  
    // Sum arr[0] can be achieved with single element 
    if (set[0] <= sum) 
       dp[0][set[0]] = true; 
  
    // Fill rest of the entries in dp[][] 
    for (int i = 1; i < n; ++i) 
        for (int j = 0; j < sum + 1; ++j) 
            dp[i][j] = (set[i] <= j) ? dp[i-1][j] || 
                                       dp[i-1][j-set[i]] 
                                     : dp[i - 1][j]; 

    int best = sum;
    for(;best>=0;best--){
        if(dp[n-1][best]) break;
    }
    cout << best << endl;
    vector<int> p; 
    getSubsetsRec(set, n-1, best, p,dp);
    for(auto x: sol) cout << x << " ";
    cout << "\n";
}

bool lengthSortFunc(const int t1, const int t2){
    return triangles[t1].shortestLength(bestPointIndex[t1]) < triangles[t2].shortestLength(bestPointIndex[t2]);
}

// Sortieren der Dreiecke anhand ihres Mittelpunkts (x-Koordinate)
bool triangleSortFunc(const Triangle &t1, const Triangle &t2){
    double mid1 = (t1.points[0].x + t1.points[1].x + t1.points[2].x) / 3;
    double mid2 = (t2.points[0].x + t2.points[1].x + t2.points[2].x) / 3;
    return mid1 < mid2;
}

void deleteUsedTriangles(){
    sort(sol.begin(),sol.end(),greater<int>());
    for(auto index: sol){
        placedTriangles.push_back(triangles[index]);
        triangles.erase(triangles.begin()+index);
        bestAngle.erase(bestAngle.begin()+index);
        bestAngleDouble.erase(bestAngleDouble.begin()+index);
        bestPointIndex.erase(bestPointIndex.begin()+index);
    }
}

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

void rotateToPositionLeft(Point centerPoint, double free_angle){
    double triRotateAngle = 0;
    for(size_t i=0;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], M_PI - triRotateAngle);
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

void moveToRightOfY(){
    double minx = 100000;
    for(auto t: placedTriangles){
        for(auto pnt: t.points){
            if(pnt.x < minx) minx = pnt.x;
        }
    }
    if(minx < 0){
        minx = abs(minx);
        for(auto t: placedTriangles){
            for(auto pnt: t.points){
                pnt.x += minx;
            }
        }
    }
}

pair<vector<Triangle>,double> doAlgorithm(vector<Triangle> i_triangles){
    triangles = i_triangles;
    Point centerPoint = Point(300,0);
    for(size_t i = 0; i<triangles.size(); i++){
        auto &t = triangles[i];
        int ind;
        double angle;
        tie(ind,angle) = locateSmallestAngle(t);
        bestPointIndex.push_back(ind);
        bestAngle.push_back((int) ceil(10000*angle));
        bestAngleDouble.push_back(angle);
    }

    subsetSum(bestAngle,(int) floor(10000*M_PI));
    sort(sol.begin(),sol.end(),lengthSortFunc);

    translateAndRotateToAxis(centerPoint);
    rotateToPositionRight(centerPoint,M_PI,false);
    deleteUsedTriangles();

    bool move = false;
    double leftmost, rightmost;
    while(!triangles.empty()){
        tie(leftmost,rightmost) = calculateDistance();

        double minx_above = 100000, miny_above = -1, maxx_above = 0, maxy_above = -1;
        double minx_axis = 100000, maxx_axis = 0;
        for(auto t: placedTriangles){
            for(auto pnt: t.points){
                if(pnt.y <= EPSILON){
                    if(pnt.x < minx_axis) {minx_axis = (pnt.y > 0) ? floor(pnt.x) : pnt.x;}
                    if(pnt.x > maxx_axis) {maxx_axis = (pnt.y > 0) ? ceil(pnt.x) : pnt.x;}
                } else if(pnt.y > EPSILON) {
                    if(pnt.x < minx_above) { minx_above = pnt.x; miny_above = pnt.y; }
                    if(pnt.x > maxx_above) { maxx_above = pnt.x; maxy_above = pnt.y; }
                }
            }
        }

        Point newCenterRight = (move) ? Point(maxx_above,0) : Point(maxx_axis,0);
        Point newCenterLeft = (move) ? Point(minx_above,0) : Point(minx_axis,0);

        bool left = false;
        double free_angle = 0;
        if(leftmost - newCenterLeft.x < newCenterRight.x - rightmost){
            //place left
            free_angle = atan180(newCenterLeft,Point(minx_above,miny_above));
            left = true;
        } else {
            //place right
            free_angle = atan_angle(newCenterRight,Point(maxx_above,maxy_above));
        }
        if(move) move = false;

        subsetSum(bestAngle,(int) floor(10000*free_angle));
        if(sol.size() == 0){
            move = true;
            continue;
        }
        
        sort(sol.begin(),sol.end(),lengthSortFunc);

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

    moveToRightOfY();
    sort(placedTriangles.begin(),placedTriangles.end(),triangleSortFunc);

    //TODO sortieren von beiden seiten wenn sinnvoll -> nach oben hin
    //TODO ggf. spiegeln an seitenhalbierender
    //TODO überlapp-cases (sweepline?) -> Beispiel 4!! (überall!!)
    //TODO randrehen ist bei Beispiel 5 schlecht

    auto dpair = calculateDistance();
    double dist = dpair.second - dpair.first;
    return {placedTriangles,dist};
}