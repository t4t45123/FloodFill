#include <iostream>
#include <string>
#include <cstdint>

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

struct Colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#define ySize 16
#define xSize 16
#define cellSize 20

int maze[ySize][xSize] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1},
    {1,1,1,1,1,0,1,0,1,0,1,0,0,0,0,1},
    {1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,1},
    {1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1},
    {1,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


#define startYPos 1
#define startXPos 1

int xPos = startXPos;
int yPos = startYPos;

vec2 pos(xPos,yPos);

void outputToPPM(const char* path) {
    FILE *f = fopen(path, "wb");
    int w = xSize*cellSize;
    int h = ySize*cellSize;
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n",w ,h);
    fprintf(f, "255\n");
    for (int y = h-1; y > -1; y--) {
        for (int x = 0; x < w; x++) {
            int xl = x/cellSize;
            int yl = y/cellSize;
            Colour c;
            c.r = 0; c.g = 0; c.b = 0;
            if ((yl+xl)%2 == 0) {
                c.r+=0x0f;
                c.g+=0x0f;
                c.b+=0x0f;
            }
            if (maze[yl][xl] == 1) {
                c.r = 0xff;
            }
            if (pos.x == xl && pos.y == yl) {
                c.g = 0xff;
            }
            
            fputc(c.r,f);
            fputc(c.g,f);
            fputc(c.b,f);


            
            
        }
    }
    fclose(f);
    cout << "generated: " << path << endl;
}

int getRayDistance(vec2 start, vec2 dir, bool draw) {
    
    if (dir.magnitude() == 0) return -1;
    int dist = 0;
    //cout << pos.x << " " << pos.y << endl;
    while (maze[pos.y][pos.x] != 1 || dist > 64) {
        if (draw) {
            string path = "image/output" + std::to_string(dist) + ".ppm";
            outputToPPM(path.c_str());
        }
        cout << pos.x << "," << pos.y << endl;
        dist ++;
        pos.x = pos.x + dir.x;
        pos.y = pos.y + dir.y;
        
    }
    if (dist > 48) dist = -1;
    return dist;
    return 0;
}



int main() {
    
    cout << "startPosVal: " << maze[yPos][xPos] << endl;
    vec2 dir(1,-1);
    cout << getRayDistance(pos,dir, true) << endl;
    
    return 0;
}