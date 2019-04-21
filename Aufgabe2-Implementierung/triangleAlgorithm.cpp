#include<bits/stdc++.h>
#include"triangles.cpp"
#define EPSILON 0.5

using namespace std;

vector<int> bestPointIndex;
vector<int> bestAngle;
vector<double> bestAngleDouble;
vector<int> sol;

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

bool triangleSortFunc(Triangle &t1, Triangle &t2){
    return t1.shortestLength(bestPointIndex[t1.id-1]) < t2.shortestLength(bestPointIndex[t2.id-1]);
}

pair<vector<Triangle>,double> doAlgorithm(vector<Triangle> triangles, bool debug){
    Point centerPoint = Point(300,0);
    for(size_t i = 0; i<triangles.size(); i++){
        auto &t = triangles[i];
        int ind;
        double angle;
        tie(ind,angle) = locateSmallestAngle(t);
        bestPointIndex.push_back(ind);
        bestAngle.push_back((int) ceil(10000*angle));
        bestAngleDouble.push_back(angle);
        cout << t.id << " " << t.points[ind].x << " " << t.points[ind].y << " " << angle << "\n";
    }

    subsetSum(bestAngle,(int) floor(10000*M_PI));

    for(auto index : sol){
        auto &t = triangles[index];
        Vektor translation = Vektor(t.points[bestPointIndex[index]],centerPoint);
        for(int i=0;i<=2;i++){
            t.points[i] = addVektor(t.points[i],translation);
        }
        int rotatePoint = findAngleCalcPoint(t,bestPointIndex[index]);
        double rotateAngle = atan_angle(centerPoint,t.points[rotatePoint]);
        for(int i=0;i<=2;i++){
            rotate_tri(centerPoint, t.points[i], rotateAngle);
        }
        t.reGenVectors();
    }

    double triRotateAngle = bestAngleDouble[sol[0]];
    for(size_t i=1;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], triRotateAngle);
        }
        triRotateAngle += bestAngleDouble[sol[i]];
    }

    double minx_above = 100000, miny_above = -1, maxx_above = 0, maxy_above = -1;
    double minx_axis = 100000, miny_axis = -1, maxx_axis = 0, maxy_axis = -1;
    for(auto index: sol){
        auto &t = triangles[index];
        for(auto pnt: t.points){
            if(pnt.y <= EPSILON){
                if(pnt.x < minx_axis) {minx_axis = pnt.x; miny_axis = pnt.y;}
                if(pnt.x > maxx_axis) {maxx_axis = pnt.x; maxy_axis = pnt.y;}
            } else if(pnt.y > EPSILON) {
                if(pnt.x < minx_above) { minx_above = pnt.x; miny_above = pnt.y; }
                if(pnt.x > maxx_above) { maxx_above = pnt.x; maxy_above = pnt.y; }
            }
        }
    }

    cout << minx_axis << " " << miny_axis << " " << minx_above << " " << miny_above << endl;
    cout << maxx_axis << " " << maxy_axis << " " << maxx_above << " " << maxy_above << endl;

    //TODO order the triangles -> lengths
    //TODO what to do with triangles outside the subset
    //TODO Gesamtabstand berechnen

    return {triangles,0};
}