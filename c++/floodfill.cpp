#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>
#include "maze.cpp"
using namespace std;



struct Pos {
    int x;
    int y;
};



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





int main(int argc, char* argv[]) {
    int xPos = 0;
    int yPos = 0;
    srand(time(0));
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
    
    int i = 1;
    while (i < xSize*ySize && ((xPos != goalx) || (yPos != goaly))) {
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
    
    return 0;
} 