#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include "Common.h"
#include "Game.h"

class GameLoader
{
public:
    bool load_game(Game &g)
    {
        ifstream my_file("gemcascade.txt");

        if (!my_file.is_open())
        {
            system("cls");
            cout << RED << "There is no unfinished game!" << RESET << endl;
            Sleep(error_delay);

            return false;
        }

        string my_text;
        vector<char> vc[ROWS];
        int s, c, m;
        string l;

        int i = 0;
        while (getline(my_file, my_text))
        {
            if (i < 64)
            {
                int j = i / 8;
                vc[j].push_back(my_text[0]);
            }
            else
            {
                if (i == 64)
                    s = stoi(my_text);
                if (i == 65)
                    c = stoi(my_text);
                if (i == 66)
                    m = stoi(my_text);
                if (i == 67)
                    l = my_text;
            }

            i++;
        }

        g.set_level(l);
        g.set_coef(c);
        g.set_score(s);
        g.set_moves(m);

        vector<vector<char>> b(ROWS, vector<char>(COLS));
        for (int row = 0; row < ROWS; row++)
            for (int col = 0; col < COLS; col++)
                b[row][col] = vc[row][col];

        g.set_board(b);

        my_file.close();

        return true;
    }
};

#endif
