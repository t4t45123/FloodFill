#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>

using namespace std;

#define xSize 8
#define ySize 8
#define cellSize 20


struct Colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
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
    int distance;
    uint8_t final; // set for defining final path;

};


Node map[ySize][xSize];
uint8_t goalx = xSize-1;
uint8_t goaly = ySize-1;



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

void removeWall(int x, int y, Wall w) {
    map[y][x].walls &= ~w;

    switch (w) {
        case WALL_N: map[y+1][x].walls &= ~WALL_S; break;
        case WALL_S: map[y-1][x].walls &= ~WALL_N; break;
        case WALL_E: map[y][x+1].walls &= ~WALL_W; break;
        case WALL_W: map[y][x-1].walls &= ~WALL_E; break;
    }
}

bool isValidWall(int x, int y, Wall w) {
    if (w == WALL_N && y == goaly) return false;
    if (w == WALL_S && y == 0) return false;
    if (w == WALL_E && x == goalx) return false;
    if (w == WALL_W && x == 0) return false;
    return true;
}

void generateMap() {
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            map[y][x].walls = 15;

        }
    }
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            map[y][x].visited = 0;
            map[y][x].distance = 100000;

            if (y == 0) map[y][x].walls |= WALL_S;
            if (y == goaly) map[y][x].walls |= WALL_N;
            if (x == 0) map[y][x].walls |= WALL_W;
            if (x == goalx) map[y][x].walls |= WALL_E;
            
        }
    }
}

bool hasUnvisitedNeighbours(int x, int y) {
    for (int i = 0; i < 4; i++) {
        switch(i) {
            case (0): if (x!=goalx) if (map[y][x+1].visited == 0) return true; break;
            case (1): if (x!=0)     if (map[y][x-1].visited == 0) return true; break;
            case (2): if (y!=goaly) if (map[y+1][x].visited == 0) return true; break;
            case (3): if (y!=0)     if (map[y-1][x].visited == 0) return true; break;
        }
    }
    return false;
}

Wall getUnvisitedNeighbour(int x, int y) {
    int pn[4] = {0,0,0,0}; //potential neighbours
    for (int i = 0; i < 4; i++) {
        switch(i) {
            case (0): if (x!=goalx) if (map[y][x+1].visited == 0) pn[0]=1; break;
            case (1): if (x!=0)     if (map[y][x-1].visited == 0) pn[1]=1; break;
            case (2): if (y!=goaly) if (map[y+1][x].visited == 0) pn[2]=1; break;
            case (3): if (y!=0)     if (map[y-1][x].visited == 0) pn[3]=1; break;
        }
    }
    int total = pn[0] + pn[1] + pn[2] + pn[3];
    int r = rand() % total;
    for (int i = 0; i < 4; i++) {
        if (pn[i] == 1) {
            if (r == 0) {
                switch(i) {
                    case (0): return WALL_E; break;
                    case (1): return WALL_W; break;
                    case (2): return WALL_N; break;
                    case (3): return WALL_S; break;
                }
            }
            r--;
        }
    }
    return WALL_N; //should never reach here
}

void generateMaze(int x,int y) {
    map[y][x].visited = 1;
    while (hasUnvisitedNeighbours(x,y)) {
        Wall w = getUnvisitedNeighbour(x,y);
        removeWall(x,y,w);
        int nx = x;
        int ny = y;
        switch (w) {
            case (WALL_E): nx = x+1; break;
            case (WALL_W): nx = x-1; break;
            case (WALL_N): ny = y+1; break;
            case (WALL_S): ny = y-1; break;
        }
        generateMaze(nx,ny);
    }
}

Wall getRandomWall() {
    int dir = rand() % 4;

    switch (dir) {
        case (0): return WALL_E; break;
        case (1): return WALL_N; break;
        case (2): return WALL_S; break;
        case (3): return WALL_W; break;
    }
    return WALL_E;

}

void removeRandomWalls() {
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            Wall w = getRandomWall();
            cout << w;
            int a = rand() % 50; // chance for wall removal
            if (a == 0) {
                if (isValidWall(x,y,w)) {
                    removeWall(x,y,w);
                }
            }
        }
    }
}

void printMapVisual() {
    cout << " ";
    for (int x = 0; x < xSize; x++) {
        cout << "_ ";
    }
    cout << endl;
    for (int y = ySize-1; y > -1; y--) {
        for (int x = 0; x < xSize; x++) {
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
            if (x == goaly) {
                cout << "|";
            }

        }
        cout <<endl;
    }
}

void printMap() {
    for (int y = ySize-1; y > -1; y--) {
        for (int x = 0; x < xSize; x++) {
            cout << (int)map[y][x].walls << "\t";
        }
        cout << endl;
    }
}

void printDistanceMap() {
    for (int y = ySize-1; y > -1; y--) {
        for (int x = 0; x < xSize; x++) {
            cout << (int)map[y][x].distance << "\t";
        }
        cout << endl;
    }
}


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
            Node n = map[yl][xl];
            // fputc(0x00,f);
            // fputc(0x00,f);
            // fputc(0x00,f);
            Colour c;
            c.r = 0; c.g = 0; c.b = 0;
            if (n.visited == 1) {
                c.b =0xFF;
            }
            if (n.walls & WALL_N) {
                if (y % cellSize > 18) {
                    c.r=0xFF;
                    c.b=0x00;

                }
            }
            if (n.walls & WALL_E) {
                if (x % cellSize > 18) {
                    c.r=0xFF;
                    c.b=0x00;
                }
            }
            if (n.walls & WALL_S) {
                if (y % cellSize < 3) {
                    c.r=0xFF;
                    c.b=0x00;
                }
            }
            if (n.walls & WALL_W) {
                if (x % cellSize < 3) {
                    c.r=0xFF;
                    c.b=0x00;
                } 
            }
            if (n.final) {
                c.r = 0xFF;
            }
            
            fputc(c.r,f);
            fputc(c.g,f);
            fputc(c.b,f);


            
            
        }
    }
    fclose(f);
    cout << "generated: " << path << endl;
}

void undiscoverMaze() {
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            map[y][x].visited = 0;
        }
    }
}

void loadMaze(const char* path) {
    /*
    example maze:
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    15,15,15,15,15,15,1,15
    */
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        cout << "failed to open maze file: " << path << endl;
        exit(1);
    }
    for (int y = ySize-1; y > -1; y--) {
        for (int x = 0; x < xSize; x++) {
            int wallValue = 0;
            fscanf(f, "%d,", &wallValue);
            map[y][x].walls = (Wall)wallValue;
        }
    }
    fclose(f);
}