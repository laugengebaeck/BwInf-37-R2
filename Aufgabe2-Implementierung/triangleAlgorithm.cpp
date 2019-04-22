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

bool triangleSortFunc(Triangle t1, Triangle t2){
    double right1 = 0, right2 = 0;
    for(auto p: t1.points){
        if(p.x > right1) right1 = p.x;
    }
    for(auto p: t2.points){
        if(p.x > right2) right2 = p.x;
    }
    return right1 < right2;
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

void rotateToPosition(Point centerPoint){
    double triRotateAngle = bestAngleDouble[sol[0]];
    for(size_t i=1;i<sol.size();i++){
        auto &t = triangles[sol[i]];
        for(int j=0;j<=2;j++){
            rotate_tri(centerPoint, t.points[j], triRotateAngle);
        }
        triRotateAngle += bestAngleDouble[sol[i]];
    }
}

double calculateDistance(){
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
    return rightmost - leftmost;
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
    rotateToPosition(centerPoint);
    deleteUsedTriangles();

    while(!triangles.empty()){
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

        Point newCenter = Point(maxx_axis,0);
        double free_angle = atan_angle(newCenter,Point(maxx_above,maxy_above));

        subsetSum(bestAngle,(int) floor(10000*free_angle));
        sort(sol.begin(),sol.end(),lengthSortFunc);

        translateAndRotateToAxis(newCenter);
        rotateToPosition(newCenter);
        deleteUsedTriangles();
    }

    sort(placedTriangles.begin(),placedTriangles.end(),triangleSortFunc);

    //TODO order the triangles -> lengths (von beiden seiten wenn sinnvoll)
    //TODO triangles auch links anfügen, wenn besser + spiegeln, wenn besser
    //TODO sometimes triangles are overlapping
    //TODO an die bisherige Konstruktion "randrehen"
    //TODO alles kommentieren

    return {placedTriangles,calculateDistance()};
}