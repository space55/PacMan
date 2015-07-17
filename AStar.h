#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

class AStar{
    public:
    int calcH(int x, int y, int endX, int endY);
    void nextMove(int startX, int startY, int endX, int endY, char board[][23], int boardX, int boardY, int ret[]);
};

#endif // ASTAR_H_INCLUDED
