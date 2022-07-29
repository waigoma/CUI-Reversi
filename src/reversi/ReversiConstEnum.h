#ifndef OTHELLO_CONSTANT_H
#define OTHELLO_CONSTANT_H

const char OTHELLO_TITLE[] =   "   ----------------------------------------------------------- \n"
                               "  |   OOOOO  TTTTTTT HH   HH EEEEEEE LL      LL       OOOOO   |\n"
                               "  |  OO   OO   TTT   HH   HH EE      LL      LL      OO   OO  |\n"
                               "  |  OO   OO   TTT   HHHHHHH EEEEE   LL      LL      OO   OO  |\n"
                               "  |  OO   OO   TTT   HH   HH EE      LL      LL      OO   OO  |\n"
                               "  |   OOOO0    TTT   HH   HH EEEEEEE LLLLLLL LLLLLLL  OOOO0   |\n"
                               "   ----------------------------------------------------------- \n";

const char OTHELLO_EMPTY_CHAR = '-';
const char OTHELLO_PLACEABLE_CHAR = '*';
const char OTHELLO_WHITE_CHAR[] = "●";
const char OTHELLO_BLACK_CHAR[] = "〇";

const int OTHELLO_BOARD_SIZE = 8;
const int OTHELLO_BOARD_X_OFFSET = 2;
const int OTHELLO_BOARD_Y_OFFSET = 2;

struct Point {
    int x;
    int y;
};

const Point OTHELLO_DIRECTIONS[OTHELLO_BOARD_SIZE] = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
};

enum class PieceColor {
    WHITE,
    BLACK,
    EMPTY,
    NULLPTR
};

enum class OthelloTurn {
    PLAYER,
    CPU
};

enum class OthelloGameState {
    TITLE,
    PLAYING,
    END,
    QUIT
};

enum class OthelloDifficulty {
    EASY,
    HARD
};

#endif //OTHELLO_CONSTANT_H
