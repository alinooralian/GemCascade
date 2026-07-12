#ifndef BOARD_H
#define BOARD_H

#include "Common.h"
#include "RandomGemGenerator.h"

class Board
{
private:
    char symbol[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    RandomGemGenerator random_gem_generator;
    vector<vector<char>> board;
    bool mark[ROWS][COLS];

    bool create_initial_match(int row, int col)
    {
        if (row > 1)
        {
            if (board[row][col] == board[row - 1][col] && board[row][col] == board[row - 2][col])
                return true;
        }

        if (col > 1)
        {
            if (board[row][col] == board[row][col - 1] && board[row][col] == board[row][col - 2])
                return true;
        }

        if (row > 0 && col > 0)
        {
            if (board[row][col] == board[row][col - 1] && board[row][col] == board[row - 1][col])
                return true;

            if (board[row][col] == board[row][col - 1] && board[row][col] == board[row - 1][col - 1])
                return true;

            if (board[row][col] == board[row - 1][col] && board[row][col] == board[row - 1][col - 1])
                return true;
        }

        if (row > 0 && col < COLS - 1)
        {
            if (board[row][col] == board[row - 1][col] && board[row][col] == board[row - 1][col + 1])
                return true;
        }

        return false;
    }

    void dfs(int row, int col, vector<pii> &vc)
    {
        vc.push_back({row, col});
        mark[row][col] = true;

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++)
        {
            int r = row + dr[i];
            int c = col + dc[i];

            if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
                continue;

            if (mark[r][c])
                continue;

            if (board[r][c] != board[row][col])
                continue;

            dfs(r, c, vc);
        }
    }

public:
    Board()
    {
        board = vector<vector<char>>(ROWS, vector<char>(COLS, ' '));
    }

    string get_emoji(char c)
    {
        if (c == 'A')
            return "🍓";
        if (c == 'B')
            return "🥕";
        if (c == 'C')
            return "🍉";
        if (c == 'D')
            return "🍋";
        if (c == 'E')
            return "🍇";
        if (c == 'F')
            return "🌽";
        if (c == 'G')
            return "🥝";
        if (c == 'H')
            return "🍒";
        return "  ";
    }

    vector<vector<char>> get_board_copy()
    {
        return board;
    }

    void set_board(vector<vector<char>> b)
    {
        board = b;
    }

    char get_cell(int r, int c)
    {
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
            return board[r][c];
        return ' ';
    }

    void set_cell(int r, int c, char ch)
    {
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
            board[r][c] = ch;
    }

    void swap_cells(int r1, int c1, int r2, int c2)
    {
        swap(board[r1][c1], board[r2][c2]);
    }

    bool is_there_match()
    {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                mark[i][j] = 0;

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (mark[i][j])
                    continue;

                vector<pii> vc;
                dfs(i, j, vc);

                if (vc.size() >= 3)
                    return true;
            }
        }

        return false;
    }

    bool is_valid_board()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (j != 0)
                {
                    swap(board[i][j], board[i][j - 1]);
                    bool check = is_there_match();
                    swap(board[i][j], board[i][j - 1]);

                    if (check)
                        return true;
                }

                if (i != 0)
                {
                    swap(board[i][j], board[i - 1][j]);
                    bool check = is_there_match();
                    swap(board[i][j], board[i - 1][j]);

                    if (check)
                        return true;
                }
            }
        }
        return false;
    }

    vector<pii> find_match()
    {
        set<pii> match;

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                mark[i][j] = 0;

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (mark[i][j])
                    continue;

                vector<pii> vc;
                dfs(i, j, vc);

                int cnt = vc.size();

                if (cnt < 3)
                    continue;

                if (cnt == 4)
                {
                    bool check_row = true;
                    bool check_col = true;

                    for (int k = 0; k < cnt - 1; k++)
                    {
                        if (vc[k].ff != vc[k + 1].ff)
                            check_row = false;
                        if (vc[k].ss != vc[k + 1].ss)
                            check_col = false;
                    }

                    if (check_row)
                    {
                        int r = vc[0].ff;

                        for (int c = 0; c < COLS; c++)
                            vc.push_back({r, c});
                    }

                    if (check_col)
                    {
                        int c = vc[0].ss;

                        for (int r = 0; r < ROWS; r++)
                            vc.push_back({r, c});
                    }
                }

                if (cnt == 5)
                {
                    char target = board[vc[0].ff][vc[0].ss];

                    for (int r = 0; r < ROWS; r++)
                        for (int c = 0; c < COLS; c++)
                            if (board[r][c] == target)
                                vc.push_back({r, c});
                }

                for (auto cell : vc)
                    match.insert(cell);
            }
        }

        vector<pii> v(match.begin(), match.end());

        return v;
    }

    void apply_gravity()
    {
        for (int j = 0; j < COLS; j++)
        {
            while (true)
            {
                int first_empty_cell = ROWS + 1;
                int last_not_empty = ROWS + 1;

                for (int i = ROWS - 1; i >= 0; i--)
                {
                    if (board[i][j] == ' ')
                    {
                        first_empty_cell = i;
                        break;
                    }
                }

                if (first_empty_cell == ROWS + 1)
                    break;

                for (int i = first_empty_cell - 1; i >= 0; i--)
                {
                    if (board[i][j] != ' ')
                    {
                        last_not_empty = i;
                        break;
                    }
                }

                if (last_not_empty != ROWS + 1)
                {
                    while (last_not_empty >= 0)
                    {
                        if (board[last_not_empty][j] == ' ')
                            break;

                        if (board[first_empty_cell][j] != ' ')
                            break;

                        swap(board[first_empty_cell][j], board[last_not_empty][j]);
                        first_empty_cell--;
                        last_not_empty--;
                    }
                }
                else
                    break;
            }
        }
    }

    void refill()
    {
        for (int j = 0; j < COLS; j++)
        {
            for (int i = 0; i < ROWS; i++)
            {
                if (board[i][j] != ' ')
                    break;

                board[i][j] = random_gem_generator.generate();
            }
        }
    }

    void initialize()
    {
        bool check;

        do
        {
            map<char, bool> mp;

            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    do
                    {
                        board[i][j] = random_gem_generator.generate();
                    } while (create_initial_match(i, j));

                    mp[board[i][j]] = true;
                }
            }

            check = true;

            for (int i = 0; i < ROWS; i++)
            {
                if (!mp[symbol[i]])
                {
                    check = false;
                    break;
                }
            }

        } while (!is_valid_board() && check);
    }
};

#endif
