#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct RobotDist{
    double distance;
    double x,y;
};

int main(){
    vector<pair<double,double>> positions = {
        {1,2},{3,4},{0,0},{5,1},{2,3}
    };

    pair<double,double> target = {2,2};
    int k = 2;
    vector<RobotDist> dist_pos;
    
    for(int i = 0 ;i < positions.size(); i++){
        double dx = positions[i].first - target.first;
        double dy = positions[i].second - target.second;
        double d = sqrt(dx*dx + dy*dy);
        dist_pos.push_back({d,positions[i].first,positions[i].second});

    }

    sort(dist_pos.begin(),dist_pos.end(),[] (const RobotDist& a,const RobotDist& b){
        return a.distance < b.distance;
    });

    for(int i = 0;i<k; i++ ){
        cout<<"("<<dist_pos[i].x<<","<<dist_pos[i].y<<")"<<endl;
    }
}