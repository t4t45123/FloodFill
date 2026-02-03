#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdint>
#include <iostream>

constexpr int GRID = 8;
constexpr int CELL_SIZE = 80;
constexpr int WINDOW_SIZE = GRID * CELL_SIZE;
constexpr float WALL_THICKNESS = 6.f;

enum Wall : uint8_t {
    WALL_N = 1 << 0,
    WALL_E = 1 << 1,
    WALL_S = 1 << 2,
    WALL_W = 1 << 3
};

uint8_t maze[GRID][GRID] = {0};

void initBorderWalls() {
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {
            if (y == 0)          maze[y][x] |= WALL_N;
            if (y == GRID - 1)   maze[y][x] |= WALL_S;
            if (x == 0)          maze[y][x] |= WALL_W;
            if (x == GRID - 1)   maze[y][x] |= WALL_E;
        }
    }
}

void syncWalls(int x, int y, Wall wall, bool enabled) {
    // Do not allow border removal
    if (!enabled) {
        if ((wall == WALL_N && y == 0) ||
            (wall == WALL_S && y == GRID - 1) ||
            (wall == WALL_W && x == 0) ||
            (wall == WALL_E && x == GRID - 1)) {
            return;
        }
    }

    if (enabled)
        maze[y][x] |= wall;
    else
        maze[y][x] &= ~wall;

    if (wall == WALL_N && y > 0)
        enabled ? maze[y - 1][x] |= WALL_S : maze[y - 1][x] &= ~WALL_S;

    if (wall == WALL_S && y < GRID - 1)
        enabled ? maze[y + 1][x] |= WALL_N : maze[y + 1][x] &= ~WALL_N;

    if (wall == WALL_W && x > 0)
        enabled ? maze[y][x - 1] |= WALL_E : maze[y][x - 1] &= ~WALL_E;

    if (wall == WALL_E && x < GRID - 1)
        enabled ? maze[y][x + 1] |= WALL_W : maze[y][x + 1] &= ~WALL_W;
}

void saveMaze() {
    std::ofstream file("maze.txt");
    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++) {
            file << static_cast<int>(maze[y][x]);
            if (x < GRID - 1) file << ",";
        }
        file << "\n";
    }
}

int main() {
    std::cout << "press s to save";

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(WINDOW_SIZE, WINDOW_SIZE)),
        "Maze Editor (Stable + Border Locked)"
    );

    initBorderWalls();

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::S)
                    saveMaze();
            }

            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {

                sf::Vector2f world =
                    window.mapPixelToCoords(mouse->position);

                float mx = world.x;
                float my = world.y;

                int cellX = static_cast<int>(mx) / CELL_SIZE;
                int cellY = static_cast<int>(my) / CELL_SIZE;

                if (cellX < 0 || cellX >= GRID ||
                    cellY < 0 || cellY >= GRID)
                    continue;

                float localX = mx - cellX * CELL_SIZE;
                float localY = my - cellY * CELL_SIZE;

                Wall clickedWall;
                bool hit = true;

                if (localY <= WALL_THICKNESS)          clickedWall = WALL_N;
                else if (localY >= CELL_SIZE - WALL_THICKNESS) clickedWall = WALL_S;
                else if (localX <= WALL_THICKNESS)     clickedWall = WALL_W;
                else if (localX >= CELL_SIZE - WALL_THICKNESS) clickedWall = WALL_E;
                else hit = false;

                if (!hit)
                    continue;

                uint8_t& cell = maze[cellY][cellX];
                syncWalls(cellX, cellY, clickedWall, !(cell & clickedWall));
            }
        }

        window.clear(sf::Color::White);

        for (int y = 0; y < GRID; y++) {
            for (int x = 0; x < GRID; x++) {

                int px = x * CELL_SIZE;
                int py = y * CELL_SIZE;

                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(sf::Vector2f(px, py));
                cell.setFillColor(sf::Color::Transparent);
                cell.setOutlineColor(sf::Color(200, 200, 200));
                cell.setOutlineThickness(1.f);
                window.draw(cell);

                uint8_t c = maze[y][x];

                auto drawWall = [&](float x, float y, float w, float h) {
                    sf::RectangleShape wall(sf::Vector2f(w, h));
                    wall.setPosition(sf::Vector2f(x, y));
                    wall.setFillColor(sf::Color::Red);
                    window.draw(wall);
                };

                if (c & WALL_N)
                    drawWall(px, py, CELL_SIZE, WALL_THICKNESS);
                if (c & WALL_S)
                    drawWall(px, py + CELL_SIZE - WALL_THICKNESS,
                             CELL_SIZE, WALL_THICKNESS);
                if (c & WALL_W)
                    drawWall(px, py, WALL_THICKNESS, CELL_SIZE);
                if (c & WALL_E)
                    drawWall(px + CELL_SIZE - WALL_THICKNESS,
                             py, WALL_THICKNESS, CELL_SIZE);
            }
        }

        window.display();
    }
}
