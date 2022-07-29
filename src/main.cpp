#include "PoolAllocator.h"
#include "reversi/ReversiGame.h"
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <io.h>
#include <conio.h>
#include <stdio.h>

/* この実習で学んだ様々なものを使って簡単なCUIのゲームを作ってみよう。
 * 作成するものは何でもよいが下記の要件を満たす事。
 * タイトル ゲームシーケンス 結果表示が存在しそれらがステートマシンによってコントロールされていること（任意)
 * クラスの多態性を用いたオブジェクト管理が入っていること(任意)
 * 自分が作成した Poolアロケータが使われていること(必須)
 * 何かしらの処理にスレッドを用いた並行処理が入っていること(任意)
 * ゲームエンジンの使用は不可
 * 使用 する言語は C ++
 */

void initialize() {
    // エスケープシーケンスを有効に
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdOut, &mode);
    SetConsoleMode(hStdOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // カーソルを消す
    printf("\x1b[?25l");
}

void finalize() {
    // clear
    printf("\033[H\033[J");
    // カーソルを表示
    printf("\x1b[?25h");
}

int main() {
	initialize();

    ReversiGame game;
    game.run();

	finalize();
	return EXIT_SUCCESS;
}
