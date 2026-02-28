#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
using namespace std;
struct Pos {
    int x;
    int y;
};

enum Wall : uint8_t {
    WALL_N = 1 << 0,
    WALL_E = 1 << 1,
    WALL_S = 1 << 2,
    WALL_W = 1 << 3
};

struct Node {
    uint8_t visited;
    uint8_t walls;
    uint8_t distance;

};

struct Colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

Node map[8][8];
uint8_t goalx = 7;
uint8_t goaly = 7;

uint8_t startx = 0;
uint8_t starty = 0;

void setWall(int x, int y, Wall w) {
    map[y][x].walls |= w;

    switch (w) {
        case WALL_N: map[y+1][x].walls |= WALL_S; break;
        case WALL_S: map[y-1][x].walls |= WALL_N; break;
        case WALL_E: map[y][x+1].walls |= WALL_W; break;
        case WALL_W: map[y][x-1].walls |= WALL_E; break;
    }
}

bool isValidWall(int x, int y, Wall w) {
    if (w == WALL_N && y == 7) return false;
    if (w == WALL_S && y == 0) return false;
    if (w == WALL_E && x == 7) return false;
    if (w == WALL_W && x == 0) return false;
    return true;
}

void generateMap() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            map[y][x].walls = 0;

        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            map[y][x].visited = 0;
            map[y][x].distance = 255;

            if (y == 0) map[y][x].walls |= WALL_S;
            if (y == 7) map[y][x].walls |= WALL_N;
            if (x == 0) map[y][x].walls |= WALL_W;
            if (x == 7) map[y][x].walls |= WALL_E;
            srand(time(0)*x+1*y+2);
            int r = rand() % 4;
            
            int d = rand() % 1;
            if (d == 0 ) {
                uint8_t wall = 1 << r;
                Wall w = static_cast<Wall>(1 << (rand() % 4));

                if (isValidWall(x, y, w)) {
                    setWall(x, y, w);
                }
            }
            
        }
    }
}




void printMapVisual() {
    cout << " ";
    for (int x = 0; x < 8; x++) {
        cout << "_ ";
    }
    cout << endl;
    for (int y = 7; y > -1; y--) {
        for (int x = 0; x < 8; x++) {
            //cout << "[" << x << "," << y << "]";
            if (map[y][x].walls & WALL_W) {
                cout << "|";
            }else {
                cout << " ";
            }
            if (map[y][x].walls & WALL_S ) {
                cout << "_";
            }else {
                cout << " ";
            }
            if (x == 7) {
                cout << "|";
            }

        }
        cout <<endl;
    }
}

void printMap() {
    for (int y = 7; y > -1; y--) {
        for (int x = 0; x < 8; x++) {
            cout << (int)map[y][x].walls << "\t";
        }
        cout << endl;
    }
}

void printDistanceMap() {
    for (int y = 7; y > -1; y--) {
        for (int x = 0; x < 8; x++) {
            cout << (int)map[y][x].distance << "\t";
        }
        cout << endl;
    }
}

int getDistanceNoWalls(int x, int y, int gx, int gy) {
    int distance = gx-x + gy-y;
    return distance;
}
void setBestDistance() {
    for (int y = 0; y < 8; y++) {
        for (int x=0; x < 8; x++) {
            map[y][x].distance = getDistanceNoWalls(x,y,goalx,goaly);
        }
    }
}
void outPixel(FILE *f, uint8_t r, uint8_t g, uint8_t b) {
    fputc(r,f);
    fputc(g,f);
    fputc(b,f);
} 

void outputToPPM() {
    const char * path = "output.ppm";
    FILE *f = fopen(path, "wb");
    int w = 160;
    int h = 160;
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n",w ,h);
    fprintf(f, "255\n");
    for (int y = h-1; y > -1; y--) {
        for (int x = 0; x < w; x++) {
            int xl = x/20;
            int yl = y/20;
            Node n = map[yl][xl];
            // fputc(0x00,f);
            // fputc(0x00,f);
            // fputc(0x00,f);
            Colour c;
            c.r = 0; c.g = 0; c.b = 0;
            if (n.walls & WALL_N) {
                if (y % 20 > 18) {
                    c.r=0xFF;

                }
            }
            if (n.walls & WALL_E) {
                if (x % 20 > 18) {
                    c.r=0xFF;
                }
            }
            if (n.walls & WALL_S) {
                if (y % 20 < 3) {
                    c.r=0xFF;
                }
            }
            if (n.walls & WALL_W) {
                if (x %20 < 3) {
                    c.r=0xFF;
                } 
            }
            // if (x == 0 ) {
            //     c.g=0xFF;
            // }
            // if (y == 1) {
            //     c.b=0xFF;
            // }

            fputc(c.r,f);
            fputc(c.g,f);
            fputc(c.b,f);


            
            
        }
    }
    fclose(f);
    cout << "generated: " << path;
}

int main() {
    generateMap();
    printMap();
    cout << endl;
    printMapVisual();
    cout << endl;
    setBestDistance();
    printDistanceMap();
    outputToPPM();
    return 0;
} 