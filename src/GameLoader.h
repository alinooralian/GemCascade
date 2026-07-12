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
            return false;

        string my_text;
        vector<vector<char>> vc(ROWS, vector<char>(COLS, ' '));
        int s, c, m;
        string l;

        int i = 0;
        while (getline(my_file, my_text))
        {
            if (i < 64)
            {
                int j = i / 8;
                int k = i % 8;
                vc[j][k] = my_text[0];
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
        g.set_board(vc);

        my_file.close();

        return true;
    }
};

#endif
