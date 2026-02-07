#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>
using namespace std;

#define xSize 8
#define ySize 8
#define genAmount 128
#define cellSize 20

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
    int distance;
    uint8_t final; // set for defining final path;

};

struct Colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
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
            map[y][x].final = 0;

            if (y == 0) map[y][x].walls |= WALL_S;
            if (y == goaly) map[y][x].walls |= WALL_N;
            if (x == 0) map[y][x].walls |= WALL_W;
            if (x == goalx) map[y][x].walls |= WALL_E;
            // srand(time(0)*x+1*y+2);
            // int r = rand() % 4;
            
            // int d = rand() % 1;
            // if (d == 0 ) {
            //     uint8_t wall = 1 << r;
            //     Wall w = static_cast<Wall>(1 << (rand() % 4));

            //     if (isValidWall(x, y, w)) {
            //         setWall(x, y, w);
            //     }
            // }
            
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

int getDistanceNoWalls(int x, int y, int gx, int gy) {
    int distance = gx-x + gy-y;
    return distance;
}
void setBestDistance() {
    for (int y = 0; y < ySize; y++) {
        for (int x=0; x < xSize; x++) {
            map[y][x].distance = getDistanceNoWalls(x,y,goalx,goaly);
        }
    }
}
void outPixel(FILE *f, uint8_t r, uint8_t g, uint8_t b) {
    fputc(r,f);
    fputc(g,f);
    fputc(b,f);
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

Pos getNextToVisit(int cx, int cy) {
    // grab all visited cells with unvisited neighbours and no blocking wall, then pick the one with lowest distance
    vector<Pos> candidates;
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            if (map[y][x].visited == 1) {
                // check for unvisited neighbours without walls
                for (int i = 0; i < 4; i++) {
                    int nx = x;
                    int ny = y;
                    switch(i) {
                        case (0): nx = x+1; if (x==xSize-1) continue; break;
                        case (1): nx = x-1; if (x==0)       continue; break;
                        case (2): ny = y+1; if (y==ySize-1) continue; break;
                        case (3): ny = y-1; if (y==0)       continue; break;
                    }
                    if (map[ny][nx].visited == 0) {
                        // check for wall
                        bool wallBlocked = false;
                        switch(i) {
                            case (0): if (map[y][x].walls & WALL_E) wallBlocked = true; break;
                            case (1): if (map[y][x].walls & WALL_W) wallBlocked = true; break;
                            case (2): if (map[y][x].walls & WALL_N) wallBlocked = true; break;
                            case (3): if (map[y][x].walls & WALL_S) wallBlocked = true; break;
                        }
                        if (!wallBlocked) {
                            Pos p;
                            p.x = nx;
                            p.y = ny;
                            candidates.push_back(p);
                        }
                    }
                }
            }
        }
    }
    vector<Pos>::iterator it;
    vector<Pos> secondaryCandidates;
    int smallestDistance = xSize*ySize;
    for (it = candidates.begin(); it != candidates.end(); it++) {
        int dist = getDistanceNoWalls(goalx,goaly,cx,cy);
        if (dist < smallestDistance) {
            secondaryCandidates.clear();
            smallestDistance = dist;
            secondaryCandidates.push_back(*it);
        }
        if (dist == smallestDistance) {
            secondaryCandidates.push_back(*it);
        }
    }
    smallestDistance = xSize*ySize;
    Pos t{-1,-1};
    for (it = secondaryCandidates.begin(); it != secondaryCandidates.end(); it++) {
        int dist = getDistanceNoWalls(it->x,it->y,cx,cy);
        if (dist < smallestDistance) {
            t = *it;
            smallestDistance = dist;
        }
    }
    return t;
}

void updateFinalPath() {
    int cx = goalx;
    int cy = goaly;
    int currentSmallest = map[cy][cx].distance;
    int x = cx;
    int y = cy;
    while (map[cy][cx].distance != 0) {
        for (int i = 0; i < 4; i++) {
            int nx = cx;
            int ny = cy;
            switch (i) {
                case (0): nx = cx+1; if (cx==xSize-1) continue; if (map[cy][cx].walls & WALL_E) continue; break;
                case (1): nx = cx-1; if (cx==0)       continue; if (map[cy][cx].walls & WALL_W) continue; break;
                case (2): ny = cy+1; if (cy==ySize-1) continue; if (map[cy][cx].walls & WALL_N) continue; break;
                case (3): ny = cy-1; if (cy==0)       continue; if (map[cy][cx].walls & WALL_S) continue; break;
            }
            if (map[ny][nx].distance < currentSmallest) {
                x = nx;
                y = ny;
                currentSmallest = map[ny][nx].distance;
            }
        }
        if (x == cx && y == cy) {
            cout << "no path found" << endl;
            break;
        }
        map[cy][cx].final=1;
        cy=y;
        cx=x;
    }
    map[0][0].final =1;
}

void printFinalPath() {
    for (int y = ySize-1; y > -1; y--) {
        for (int x = 0; x < xSize; x++) {
            cout << (int)map[y][x].final << "\t";
        }
        cout << endl;
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

int main(int argc, char* argv[]) {
    int i = 1;
    for (int j = 0; j < genAmount; j++) {
    int xPos = 0;
    int yPos = 0;
    srand(time(0)*j);
    generateMap();
    
    if (argc == 2) {
        loadMaze(argv[1]);
    }else {
        generateMaze(4,4);
        removeRandomWalls();
    }
    undiscoverMaze();
    map[yPos][xPos].visited = 1; // set start point
    map[yPos][xPos].distance=0;
    cout << "\n general maze \n";
    printMapVisual();
    
    
    cout << "\n opening the maze \n";
    printMapVisual();
    cout << endl;
    printMap();
    cout << endl;
    //printDistanceMap();
    //#outputToPPM("output.ppm");
    
    while (i < xSize*ySize*genAmount && ((xPos != goalx) || (yPos != goaly))) {
        Pos n = getNextToVisit(xPos,yPos);
        if (n.x == -1 && n.y == -1) {
            cout << endl << "no valid moves remaining!";
            break;
        }
        map[n.y][n.x].visited=1;
        xPos = n.x;
        yPos = n.y;
        int smallestAdjacent = 25555;
        for (int i = 0; i < 4; i++) {
            int nx = xPos;
            int ny = yPos;
            switch (i) {
                case (0): nx = xPos+1; if (xPos==xSize-1) continue; if (map[yPos][xPos].walls & WALL_E) continue; break;
                case (1): nx = xPos-1; if (xPos==0)       continue; if (map[yPos][xPos].walls & WALL_W) continue; break;
                case (2): ny = yPos+1; if (yPos==ySize-1) continue; if (map[yPos][xPos].walls & WALL_N) continue; break;
                case (3): ny = yPos-1; if (yPos==0)       continue; if (map[yPos][xPos].walls & WALL_S) continue; break;
            }
            if (map[ny][nx].distance < smallestAdjacent) {
                smallestAdjacent = map[ny][nx].distance;
            }
        }
        map[yPos][xPos].distance = smallestAdjacent+1;
        string path = "image/output" + std::to_string(i) + ".ppm";
        i++;
        outputToPPM(path.c_str());
    }
    cout << endl;
    printDistanceMap();
    updateFinalPath();
    cout << endl;
    printFinalPath();
    cout << endl;
    

    for (int x = 0; x < 5; x++) {
        string path = "image/output" + std::to_string(i) + ".ppm";
        i++;
        outputToPPM(path.c_str());
    } 
}
    
    return 0;
} 