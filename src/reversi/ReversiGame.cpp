#include "ReversiGame.h"

// TODO 全体的にコード見にくい
// Board クラスとか作って分けようとしたけど board がバグった。

/// 盤面の初期化を行う
void ReversiGame::initialize() {
    count = {2, 2};
    turn = {OthelloTurn::PLAYER, PieceColor::WHITE};
    records.clear();
    legalPutPoints.clear();
    int i = 0;
    for (auto& xBoard : board) {
        int j = 0;
        for (auto& x : xBoard) {
            if ((i == 3 && j == 3) || (i == 4 && j == 4))
                x->color = PieceColor::WHITE;
            else if ((i == 3 && j == 4) || (i == 4 && j == 3))
                x->color = PieceColor::BLACK;
            else
                x->color = PieceColor::EMPTY;
            ++j;
        }
        ++i;
    }
}

/// 外から呼ばれるところ。プログラムの始動。
void ReversiGame::run() {
    while (true) {
        printf("\033[H\033[J");
        switch (state) {
            case OthelloGameState::TITLE:
                title();
                break;
            case OthelloGameState::PLAYING:
                play();
                break;
            case OthelloGameState::END:
                end();
                break;
            case OthelloGameState::QUIT:
                finalize();
                return;
        }
    }
}

/// タイトル部分
void ReversiGame::title() {
    printf("\x1b[%d;%dH%s", 2, 1, OTHELLO_TITLE);
    printf("\x1b[%d;%dH%s", 10, 18, ">>> Press Space to start <<<");
    printf("\x1b[%d;%dH%s", 12, 13, ">> Press A or D to select difficulty <<");
    printf("\x1b[%d;%dH%s", 15, 20, ">>> Press Q to quit <<<");

    string str = difficulty == OthelloDifficulty::EASY ? "Easy" : "Hard";
    printf("\x1b[%d;%dH > %s", 13, 25, str.c_str());

    int i = _getch();

    switch (i) {
        case ' ':
            state = OthelloGameState::PLAYING;
            initialize();
            break;
        case 'a':
            difficulty = OthelloDifficulty::EASY;
            break;
        case 'd':
            difficulty = OthelloDifficulty::HARD;
            break;
        case 'q':
            state = OthelloGameState::QUIT;
            break;
        default:
            break;
    }
}

/// ゲームプレイ部分
void ReversiGame::play() {
    // 両方ともパス判定なら試合終了
    bool isPassed = false;

    while (true) {
        legalPutPoints = getLegalPutPoints(turn.color);
        int size = (int) legalPutPoints.size() - 1;
        current = 0;

        // 置けるところがなくなったらパス
        if (size == -1) {
            if (isPassed) break;
            isPassed = true;

            changeTurn();
            continue;
        }

        // 描画
        draw();
        isPassed = false;

        // それぞれのターン処理
        switch (turn.turn) {
            case OthelloTurn::PLAYER:
                playerTurn(size);
                break;
            case OthelloTurn::CPU:
                cpuTurn(size);
                break;
        }

        changeTurn(); // ターンを変更
    }

    state = OthelloGameState::END;
}

/// Winner を judgement する
void ReversiGame::end() {
    string str;

    drawBoard();
    drawScore();

    if (count.white > count.black)
        str = ">>> Player! <<<";
    else if (count.white < count.black)
        str = "  >>> CPU! <<<";
    else
        str = " >>> Draw! <<<";

    printf("\x1b[%d;%dH%s", 2, 30, "##### Winner #####");
    printf("\x1b[%d;%dH %s", 3, 30, str.c_str());

    printf("\x1b[%d;%dH%s", 9, 30, "##### Guide #####");
    printf("\x1b[%d;%dH> Press R to title", 11, 30);
    printf("\x1b[%d;%dH> Press Q to quit", 13, 30);

    int i = _getch();
    if (i == 'r')
        state = OthelloGameState::TITLE;
    else if (i == 'q')
        state = OthelloGameState::QUIT;
}

void ReversiGame::finalize() {

}

/// プレイヤーのターン処理
void ReversiGame::playerTurn(int size) {
    while (true) {
        int x = legalPutPoints[current].x;
        int y = legalPutPoints[current].y;

        redrawBoard();

        int key = _getch(); // キー入力を待つ

        switch (key) {
            case 'a':
                if (--current < 0) current = size;
                break;
            case 'd':
                if (++current > size) current = 0;
                break;
            case ' ':
                putPiece(x, y);
                return;
            default:
                continue;
        }
    }
}

/// CPU のターン処理
void ReversiGame::cpuTurn(int size) {
    if (difficulty == OthelloDifficulty::EASY) {
        easy(size);
    } else {
        for (auto p: legalPutPoints) {
            // 角見つけたらとる
            if ((p.x == 0 && p.y == 0) || (p.x == 7 && p.y == 0) || (p.x == 0 && p.y == 7) || (p.x == 7 && p.y == 7)) {
                putPiece(p.x, p.y);
                return;
            } else {
                continue;
            }
        }
        easy(size);
    }
}

/// 難易度 EASY
void ReversiGame::easy(int size) {
    int x = 0;
    srand((unsigned int) time(NULL));
    if (size != 0) x = rand() % size;
    putPiece(legalPutPoints[x].x, legalPutPoints[x].y);
}

/// 全体の描画
void ReversiGame::draw() {
    drawBoard();
    drawTurn();
    drawScore();
    drawSelected();

    printf("\x1b[%d;%dH%s", 12, 30, "##### Guide #####");
    printf("\x1b[%d;%dH > a: prev", 13, 30);
    printf("\x1b[%d;%dH > d: next", 14, 30);
    printf("\x1b[%d;%dH > space: put stone", 15, 30);
}

/// 盤面の再描写
void ReversiGame::redrawBoard() {
    drawBoard();
    drawSelected();
}

/// 盤面の描画
void ReversiGame::drawBoard() {
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        drawBoardLine(i);
    }
}

/// 1行の描画
void ReversiGame::drawBoardLine(int i) {
    int yOffset = OTHELLO_BOARD_Y_OFFSET;
    int xOffset = OTHELLO_BOARD_X_OFFSET;

    printf("\x1b[%d;%dH%d", yOffset, ((i + 1) + xOffset) * 2, i);
    printf("\x1b[%d;%dH%d", (i + 1) + yOffset, 2 + xOffset, i);
    printf("\x1b[%d;%dH%c", (i + 1) + yOffset, 3 + xOffset, '|');

    for (int j = 0; j < OTHELLO_BOARD_SIZE; j++) {
        // 盤面を見て str 選択
        string str;
        switch (board[i][j]->color) {
            case PieceColor::WHITE:
                str = OTHELLO_WHITE_CHAR;
                break;
            case PieceColor::BLACK:
                str = OTHELLO_BLACK_CHAR;
                break;
            default:
                str = OTHELLO_EMPTY_CHAR;
        }

        // 現在置ける場所の色と char を変える
        for (auto p : legalPutPoints) {
            if (p.x == i && p.y == j) {
                setFontColorRGB(255, 255, 0);
                str = OTHELLO_PLACEABLE_CHAR;
            }

            // 現在の場所だけ色を変える
            if (legalPutPoints[current].x == i && legalPutPoints[current].y == j) {
                setFontColorRGB(255, 0, 0);
            }
        }

        printf("\x1b[%d;%dH%s", (i + 1) + yOffset, ((j + 1) + xOffset) * 2, str.c_str());
        resetFontColor();
    }
}

/// 現在のターンを描写
void ReversiGame::drawTurn() const {
    string turnStr = turn.turn == OthelloTurn::PLAYER ? " > Player (White)" : " > CPU (Black)  ";
    printf("\x1b[%d;%dH%s", 2, 30, "##### Turn #####");
    printf("\x1b[%d;%dH%s", 3, 30, turnStr.c_str());
}

/// 現在のスコアを描写
void ReversiGame::drawScore() const {
    printf("\x1b[%d;%dH%s", 5, 30, "##### Score #####");
    printf("\x1b[%d;%dH > Player: %d  ", 6, 30, count.white);
    printf("\x1b[%d;%dH > CPU:    %d  ", 7, 30, count.black);
}

/// プレイヤー視点でどこのマスを選択しているかを描写
void ReversiGame::drawSelected() {
    int x = legalPutPoints[current].x;
    int y = legalPutPoints[current].y;
    printf("\x1b[%d;%dH%s", 9, 30, "##### Selected #####");
    printf("\x1b[%d;%dH > (%i, %i)", 10, 30, y, x);
}

/// プレイヤーと CPU のターン変更
void ReversiGame::changeTurn() {
    turn.turn = turn.turn == OthelloTurn::PLAYER ? OthelloTurn::CPU : OthelloTurn::PLAYER;
    turn.color = turn.color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
}

/// isLegalPut(int, int) で呼ばれる
bool ReversiGame::isLegalPut(Point point, int direction) {
    Point p = OTHELLO_DIRECTIONS[direction];
    for (int i = 1; i < OTHELLO_BOARD_SIZE; i++) {
        int x = point.x + p.x * i;
        int y = point.y + p.y * i;
        if (x < 0 || x >= OTHELLO_BOARD_SIZE || y < 0 || y >= OTHELLO_BOARD_SIZE) {
            return false;
        }
        PieceColor color = board[x][y]->color;
        if (color == PieceColor::EMPTY) {
            return false;
        } else if (color == turn.color) {
            return i > 1;
        }
    }
    return false;
}

/// x, y に置けるかどうかを返す
bool ReversiGame::isLegalPut(int x, int y) {
    // 範囲外か既に置かれていたら置けない
    if (x < 0 || x >= OTHELLO_BOARD_SIZE ||
        y < 0 || y >= OTHELLO_BOARD_SIZE ||
        board[x][y]->color != PieceColor::EMPTY
       ) {
        return false;
    }

    // 置けるかどうかを判定
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        Point newPoint = {x, y};
        if (isLegalPut(newPoint, i)) {
            return true;
        }
    }
    return false;
}

/// 石の色で置ける場所をすべて取得する
vector<Point> ReversiGame::getLegalPutPoints(PieceColor) {
    vector<Point> points;
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        for (int j = 0; j < OTHELLO_BOARD_SIZE; j++) {
            if (isLegalPut(i, j)) {
                points.push_back({i, j});
            }
        }
    }
    return points;
}

/// 石を実際に置く
/// 置けなければ false 置ければ true
bool ReversiGame::putPiece(int x, int y) {
    // 置けるかどうか
    if (!isLegalPut(x, y)) return false;

    // 置ける場所を周囲8方向で探索
    bool legalFlags[OTHELLO_BOARD_SIZE];
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        legalFlags[i] = isLegalPut({x, y}, i);
    }

    // x, y に置いたので、石のカウントを増やす
    board[x][y]->color = turn.color;
    if (turn.turn == OthelloTurn::PLAYER) count.white++;
    else count.black++;
    ReversiRecord rec(turn.color, {x, y});
    records.push_back(rec);

    // 置ける方向の石をひっくり返していく
    for (int i = 0; i < OTHELLO_BOARD_SIZE; i++) {
        // 置ける方向じゃなかったら continue
        if (!legalFlags[i]) continue;

        // 置ける方向に石を置く
        Point p = OTHELLO_DIRECTIONS[i];
        for (int j = 1; j < OTHELLO_BOARD_SIZE; j++) {
            int newX = x + p.x * j;
            int newY = y + p.y * j;

            // 置こうとした場所が自分の色なら終了
            if (board[newX][newY]->color == turn.color) break;

            // ひっくり返す
            board[newX][newY]->color = turn.color;

            // ひっくり返したので石のカウントを増減させる
            if (turn.turn == OthelloTurn::PLAYER) {
                count.white++;
                count.black--;
            } else {
                count.black++;
                count.white--;
            }
            rec.record(newX, newY);
        }
    }
    return true;
}

void ReversiGame::escSeq(const char *controlSequence, const char function) {
    printf("\x1b[");
    printf(controlSequence);
    putchar(function);
}

void ReversiGame::setFontColorRGB(const unsigned char r, const unsigned char g, const unsigned char b) {
    char buf[64];
    sprintf_s(buf, "38;2;%d;%d;%d", r, g, b);
    escSeq(buf, 'm');
}

void ReversiGame::resetFontColor() {
    escSeq("0", 'm');
}

void ReversiGame::pass() {
    records.push_back(ReversiRecord(PieceColor::NULLPTR, {-1, -1}));
    changeTurn();
}

void ReversiGame::undo() {
    ReversiRecord rec = records.back();
    records.pop_back();

    if (rec.getColor() == PieceColor::NULLPTR) {
        changeTurn();
        return;
    }

    board[rec.getPlacedPiece().x][rec.getPlacedPiece().y]->color = PieceColor::EMPTY;
    int recCount = rec.getRecordPiecesSize();
    PieceColor oppColor = rec.getColor() == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
    for (int i = 0; i < recCount; i++) {
        Point p = rec.getRecordPiece(i);
        board[p.x][p.y]->color = oppColor;
    }
    if (rec.getColor() == PieceColor::WHITE) {
        count.white -= recCount + recCount;
        count.black += recCount;
    } else {
        count.black -= recCount + recCount;
        count.white += recCount;
    }
}
