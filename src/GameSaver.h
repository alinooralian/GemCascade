#ifndef GAME_SAVER_H
#define GAME_SAVER_H

#include "Common.h"
#include "Board.h"

class GameSaver
{
public:
    bool save_game(string level, int coef, int score, int moves, Board b)
    {
        system("cls");

        ofstream my_file("gemcascade.txt");

        vector<vector<char>> board = b.get_board_copy();

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                my_file << board[i][j] << '\n';

        my_file << score << '\n'
                << coef << '\n'
                << moves << '\n'
                << level;

        my_file.close();

        return true;
    }
};

#endif
