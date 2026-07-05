#ifndef RENDERER_H
#define RENDERER_H

#include "Common.h"
#include "Board.h"

class Renderer
{
public:
    void vertical_line()
    {
        cout << "  ";

        cout << "╠══════";
        for (int i = 1; i < COLS; i++)
            cout << "╬══════";
        cout << "╣" << endl;
    }

    void horizontal_line(int row, Board b)
    {
        vector<vector<char>> board = b.get_board_copy();

        cout << row << " ";

        for (int i = 0; i < COLS; i++)
            cout << "║" << "  " << b.get_emoji(board[row][i]) << "  ";

        cout << "║" << endl;
    }

    void print_board(Board b)
    {
        cout << "  ";

        for (int i = 0; i < COLS; i++)
            cout << "    " << i << "  ";
        cout << endl;

        cout << "  ";
        cout << "╔══════";
        for (int i = 1; i < COLS; i++)
            cout << "╦══════";
        cout << "╗" << endl;

        for (int i = 0; i < ROWS; i++)
        {
            if (i != 0)
                vertical_line();
            horizontal_line(i, b);
        }

        cout << "  ";
        cout << "╚══════";
        for (int i = 1; i < COLS; i++)
            cout << "╩══════";
        cout << "╝" << endl;
    }

    void print_header(int score, int moves, string level, int goal_score)
    {
        cout << "╔";
        for (int i = 0; i < 111; i++)
            cout << "═";
        cout << "╗" << endl;

        cout << "║" << GREEN << "\t🏆Score: " << score << '\t' << RESET;
        cout << "║" << YELLOW << "\t⏳Remaining Moves: " << moves << '\t' << RESET;
        cout << "║" << PINK << "\t⚔️Level: " << level << '\t' << RESET;
        cout << "║" << ORANGE << "\t🎯Goal Score: " << goal_score << '\t' << RESET << "║" << endl;

        cout << "╚";
        for (int i = 0; i < 111; i++)
            cout << "═";
        cout << "╝" << "\n\n";
    }

    void print_controller(int hint_cost, int rocket_cost, int bomb_cost)
    {
        cout << "\n\n";
        for (int i = 0; i < 120; i++)
            cout << "═";
        cout << "\n\n";

        cout << BLUEII << "[CONTROLS]:" << RESET;
        cout << ORANGE << " W: 🔄Swap " << RESET << "║";
        cout << GREENII << " H: 💡Hint(-" << hint_cost << " Score) " << RESET << "║";
        cout << CYAN << " R: 🚀Rocket(-" << rocket_cost << " Score) " << RESET << "║";
        cout << PURPLEII << " B: 💣Bomb(-" << bomb_cost << " Score) " << RESET << "║";
        cout << YELLOW << " S: 💾Save " << RESET << "║";
        cout << RED << " Q: 🔚Quit" << RESET << "\n\n";
    }
};

#endif
