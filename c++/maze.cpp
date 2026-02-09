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
    uint8_t final;

};

