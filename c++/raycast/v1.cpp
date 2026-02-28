#include <iostream>
#include <string>
#include <cstdint>


#include "../maze.cpp"
using namespace std;

class vec2 {
    public:
        int x;
        int y;
    vec2(int a, int b) {
        x = a;
        y = b;
    };
    int magnitude() {
        return abs(x)+abs(y);
    }
};






#define startYPos 1
#define startXPos 1

int xPos = startXPos;
int yPos = startYPos;

vec2 pos(xPos,yPos);



int getRayDistance(vec2 start, vec2 dir, bool draw) {
    
    if (dir.magnitude() == 0) return -1;
    int dist = 0;
    //cout << pos.x << " " << pos.y << endl;
    while (pos.x > -1 && pos.x < xSize && pos.y > -1 && pos.y < ySize) {
        if (draw) {
            string path = "image/output" + std::to_string(dist) + ".ppm";
            outputToPPM(path.c_str());
        }
        cout << pos.x << "," << pos.y << endl;
        dist ++;
        if (dir.x > 0) {
            if (map[pos.y][pos.x].walls & WALL_E) return dist;
        }else if (dir.x < 0) {
            if (map[pos.y][pos.x].walls & WALL_W) return dist;
        }
        if (dir.y > 0) {
            if (map[pos.y][pos.x].walls & WALL_N) return dist;
        }else if (dir.y < 0) {
            if (map[pos.y][pos.x].walls & WALL_S) return dist;
        }
        pos.x = pos.x + dir.x;
        pos.y = pos.y + dir.y;
        
    }
    if (dist > 48) dist = -1;
    return dist;
    return 0;
}



int main() {
    
    //cout << "startPosVal: " << map[yPos][xPos] << endl;
    vec2 dir(0,1);
    cout << getRayDistance(pos,dir, true) << endl;
    
    return 0;
}