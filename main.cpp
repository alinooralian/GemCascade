#include <bits/stdc++.h>
#include <random>
#include <windows.h>
#include <stdlib.h>

using namespace std;

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define RESET "\e[0m"
#define ff first
#define ss second
typedef pair<int, int> pii;

const int ROWS = 8, COLS = 8;

class Game
{
private:
    char symbool[10] = {'A', 'B', 'C', 'D', 'E', 'F'};
    map<char, string> color_of_gems = {{'A', RED}, {'B', GREEN}, {'C', YELLOW}, {'D', BLUE}, {'E', PURPLE}, {'F', CYAN}};
    vector<char> board[10];
    int score = 0, coef = 10;
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

            if (board[row][col] == board[row][col - 1] && board[row - 1][col - 1])
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

    char random_gem()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 5);

        int rand_idx = dist(gen);

        return symbool[rand_idx];
    }

    vector<pii> dfs(int row, int col)
    {
        vector<pii> v;
        v.push_back({row, col});

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

            vector<pii> vc = dfs(r, c);

            for (auto cell : vc)
                v.push_back(cell);
        }

        return v;
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

                vector<pii> vc = dfs(i, j);

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
        vector<pii> match;

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                mark[i][j] = 0;

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (mark[i][j])
                    continue;

                vector<pii> vc = dfs(i, j);
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

                        cnt = 8;
                    }

                    if (check_col)
                    {
                        int c = vc[0].ss;

                        for (int r = 0; r < ROWS; r++)
                            vc.push_back({r, c});

                        cnt = 8;
                    }
                }

                if (cnt == 5)
                {
                    char target = board[vc[0].ff][vc[0].ss];

                    for (int r = 0; r < ROWS; r++)
                        for (int c = 0; c < COLS; c++)
                            if (board[r][c] == target)
                                vc.push_back({r, c});

                    cnt = vc.size() - 5;
                }

                score += cnt * coef;

                for (auto cell : vc)
                    match.push_back(cell);
            }
        }

        return match;
    }

    void apply_gravity()
    {
        for (int j = 0; j < COLS; j++)
        {
            while (true)
            {
                int first_empty_cell = ROWS + 1, last_not_empty_cell = ROWS + 1;
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
                        last_not_empty_cell = i;
                        break;
                    }
                }

                if (last_not_empty_cell != ROWS + 1)
                {
                    while (last_not_empty_cell >= 0)
                    {
                        if (board[last_not_empty_cell][j] == ' ')
                            break;

                        if (board[first_empty_cell][j] != ' ')
                            break;

                        swap(board[first_empty_cell][j], board[last_not_empty_cell][j]);
                        first_empty_cell--;
                        last_not_empty_cell--;
                    }
                }
                else
                {
                    break;
                }
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

                board[i][j] = random_gem();
            }
        }
    }

public:
    void initialize()
    {
        do
        {
            for (int i = 0; i < ROWS; i++)
                board[i].clear();

            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    board[i].push_back(random_gem());
                    while (create_initial_match(i, j))
                    {
                        board[i].pop_back();
                        board[i].push_back(random_gem());
                    }
                }
            }

        } while (!is_valid_board());
    }

    void vertical_line()
    {
        cout << "  ";

        for (int i = 0; i < COLS; i++)
            cout << "+-----";
        cout << "+" << endl;
    }

    void horizontal_line(int row)
    {
        cout << row << " ";

        for (int i = 0; i < COLS; i++)
            cout << "|" << "  " << color_of_gems[board[row][i]] << board[row][i] << RESET << "  ";

        cout << "|" << endl;
    }

    void print_board()
    {
        cout << "  ";

        for (int i = 0; i < COLS; i++)
            cout << "   " << i << "  ";
        cout << endl;

        for (int i = 0; i < ROWS; i++)
        {
            vertical_line();
            horizontal_line(i);
        }
        vertical_line();
    }

    bool game_control()
    {
        print_board();

        cout << "[CONTROLS]: " << "W : Swap " << '|' << "S : Save " << '|' << "Q : Quit" << endl;

        char choice;
        cin >> choice;

        if (choice == 'W')
        {
            cout << "[INPUT]: " << "Enter First Row & Col:" << endl;

            int r1, c1;
            cin >> r1 >> c1;

            cout << "[INPUT]: " << "Enter Second Row & Col:" << endl;

            int r2, c2;
            cin >> r2 >> c2;

            if (!cascade(r1, c1, r2, c2))
                cout << "Invalid Move!" << endl;

            return true;
        }

        if (choice == 'Q')
            return false;
    }

    void bomb(int row, int col)
    {
        int tmp[3] = {0, 1, -1};

        for (int i = 0; i < 3; i++)
        {
            int r = row + tmp[i];
            int c = col;

            if (r == COLS || r == -1)
                continue;

            board[r][c] = ' ';

            if (c > 0)
                board[r][c - 1] = ' ';

            if (c < COLS - 1)
                board[r][c + 1] = ' ';
        }
    }

    void rocket(char type, int num)
    {
        if (type == 'r')
        {
            for (int j = 0; j < COLS; j++)
                board[num][j] = ' ';
        }
        else
        {
            for (int i = 0; i < ROWS; i++)
                board[i][num] = ' ';
        }
    }

    vector<pii> hint()
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
                    swap(board[i][j], board[i][j - 1]);
                    auto tmp = find_match();
                    swap(board[i][j], board[i][j - 1]);

                    if (tmp.size() > max_size)
                    {
                        max_size = tmp.size();
                        r1 = i, r2 = i;
                        c1 = j, c2 = j - 1;
                    }
                }

                if (i != 0)
                {
                    swap(board[i][j], board[i - 1][j]);
                    auto tmp = find_match();
                    swap(board[i][j], board[i - 1][j]);

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

    bool cascade(int r1, int c1, int r2, int c2)
    {
        if (abs(r1 - r2) + abs(c1 - c2) != 1)
            return false;

        swap(board[r1][c1], board[r2][c2]);
        print_board();
        Sleep(3500);

        bool check = is_there_match();

        if (!check)
        {
            swap(board[r1][c1], board[r2][c2]);
            return false;
        }

        while (check)
        {
            vector<pii> match = find_match();

            for (auto cell : match)
                board[cell.ff][cell.ss] = ' ';

            print_board();
            Sleep(3500);

            (coef >= 40) ? (coef += 2) : (coef *= 2);

            apply_gravity();
            print_board();
            Sleep(3500);
            refill();
            print_board();
            Sleep(5000);

            check = is_there_match();
        }

        if (!is_valid_board())
            initialize();

        return true;
    }
};

int main()
{
    while (true)
    {
        cout << "Enter number of your choice:" << endl;
        cout << "1.New Game\n2.Load Game\n3.Shop\n4.Exit" << endl;

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            Game g;
            g.initialize();

            while (true)
            {
                system("cls");

                if (!g.game_control())
                    break;
            }
        }
        else if (choice == 4)
        {
            return 0;
        }
    }
}