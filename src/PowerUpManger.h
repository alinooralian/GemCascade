#ifndef POWER_UP_MANGER_H
#define POWER_UP_MANGER_H

#include "Common.h"
#include "Board.h"

class PowerUpManger
{
public:
    void bomb(int row, int col, Board &b)
    {
        int dr[] = {-1, 1, 0};
        int dc[] = {0, -1, 1};

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int r = row + dr[i];
                int c = col + dc[j];

                if (r >= ROWS || r < 0 || c < 0 || c >= COLS)
                    continue;

                b.set_cell(r, c, ' ');
            }
        }
    }

    void rocket(char type, int num, Board &b)
    {
        if (type == 'R' || type == 'r')
        {
            for (int j = 0; j < COLS; j++)
                b.set_cell(num, j, ' ');
        }
        else
        {
            for (int i = 0; i < ROWS; i++)
                b.set_cell(i, num, ' ');
        }
    }

    vector<pii> hint(Board &b)
    {
        int max_size = 0;
        int r1, r2;
        int c1, c2;

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (j != 0)
                {
                    b.swap_cells(i, j, i, j - 1);
                    auto tmp = b.find_match();
                    b.swap_cells(i, j, i, j - 1);

                    if (tmp.size() > max_size)
                    {
                        max_size = tmp.size();
                        r1 = i, r2 = i;
                        c1 = j, c2 = j - 1;
                    }
                }

                if (i != 0)
                {
                    b.swap_cells(i, j, i - 1, j);
                    auto tmp = b.find_match();
                    b.swap_cells(i, j, i - 1, j);

                    if (tmp.size() > max_size)
                    {
                        max_size = tmp.size();
                        r1 = i, r2 = i - 1;
                        c1 = j, c2 = j;
                    }
                }
            }
        }

        return {{r1, c1}, {r2, c2}};
    }
};

#endif
