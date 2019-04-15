#include<bits/stdc++.h>
#include"triangles.cpp"

using namespace std;

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

pair<vector<Triangle>,double> doAlgorithm(vector<Triangle> triangles, bool debug){
    Point centerPoint = Point(300,0);
    vector<Point> bestPoint;
    vector<int> bestAngle;
    for(size_t i = 0; i<triangles.size(); i++){
        auto &t = triangles[i];
        Point p;
        double angle;
        tie(p,angle) = locateSmallestAngle(t);
        bestPoint.push_back(p);
        bestAngle.push_back((int) ceil(10000*angle));
        /*Vektor translation = Vektor(p,centerPoint);
        t.point1 = addVektor(t.point1,translation);
        t.point2 = addVektor(t.point2,translation);
        t.point3 = addVektor(t.point3,translation);*/
        cout << p.x << " " << p.y << " " << angle << "\n";
    }
    for(auto x: bestAngle){
            cout << x << " ";
        }
        cout << "\n";
    subsetSum(bestAngle,(int) floor(10000*M_PI));
    //TODO rotate -> no overlap
    //TODO what to do with triangles outside the subset
    return {triangles,0};
}