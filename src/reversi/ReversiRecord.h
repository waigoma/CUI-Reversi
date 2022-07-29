#ifndef OTHELLO_RECORD_H
#define OTHELLO_RECORD_H

#include "ReversiConstEnum.h"
#include <vector>

using namespace std;

class ReversiRecord {
private:
    PieceColor color;
    Point placedPiece;
    vector<Point> recordPieces;

public:
    ReversiRecord(PieceColor pieceColor, Point point) {
        color = pieceColor;
        placedPiece = point;
        recordPieces.clear();
    };

    ~ReversiRecord() {
        recordPieces.clear();
        recordPieces.shrink_to_fit();
    };

    void record(int, int);

    PieceColor getColor() const {
        return color;
    };

    Point getPlacedPiece() const {
        return placedPiece;
    };

    int getRecordPiecesSize() const {
        return (int) recordPieces.size();
    };

    Point getRecordPiece(int i) {
        return recordPieces[i];
    };
};


#endif //OTHELLO_RECORD_H
