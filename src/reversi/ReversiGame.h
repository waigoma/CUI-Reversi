#ifndef OTHELLO_GAME_H
#define OTHELLO_GAME_H

#include "../PoolAllocator.h"
#include "ReversiConstEnum.h"
#include "ReversiRecord.h"
#include <conio.h>
#include <string>
#include <ctime>
#include <cstdlib>

class ReversiGame {
private:
    struct PieceState {
        PieceColor color; // WHITE:白, EMPTY:空, BLACK:黒
    };

    struct TurnState {
        OthelloTurn turn; // PLAYER:プレイヤー, CPU:CPU
        PieceColor color; // WHITE:プレイヤー, BLACK:CPU
    };

    struct OthelloCount {
        int white;
        int black;
    };

    PoolAllocator<PieceState, OTHELLO_BOARD_SIZE * OTHELLO_BOARD_SIZE> pieceAllocator;
    PieceState* board[OTHELLO_BOARD_SIZE][OTHELLO_BOARD_SIZE];

    TurnState turn = {OthelloTurn::PLAYER, PieceColor::WHITE}; // -1:白, 1:黒
    OthelloCount count = {2, 2};
    OthelloGameState state = OthelloGameState::TITLE;
    OthelloDifficulty difficulty = OthelloDifficulty::EASY;

    vector<ReversiRecord> records;
    vector<Point> legalPutPoints;
    int current = 0;

    void initialize();
    void title();
    void play();
    void end();
    void finalize();

    void draw();
    void redrawBoard();
    void drawBoard();
    void drawBoardLine(int);
    void drawTurn() const;
    void drawScore() const;
    void drawSelected();

    void playerTurn(int);
    void cpuTurn(int);
    void easy(int);
    void hard(int);

    void escSeq(char const *, char);
    void setFontColorRGB(unsigned char, unsigned char, unsigned char);
    void resetFontColor();

    bool isLegalPut(Point, int);
    bool isLegalPut(int x, int y);
    vector<Point> getLegalPutPoints(PieceColor);
    bool putPiece(int, int);
    void changeTurn();

    void pass();
    void undo();

public:
    ReversiGame() {
        for (auto& xBoard : board) {
            for (auto& x : xBoard) {
                x = pieceAllocator.Alloc();
            }
        }
    };

    ~ReversiGame() {
        for (auto& xBoard : board) {
            for (auto& x : xBoard) {
                pieceAllocator.Free(x);
            }
        }
    };

    void run();
};


#endif //OTHELLO_GAME_H
