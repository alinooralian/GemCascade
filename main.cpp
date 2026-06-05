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
    char symbool[10] = {'A', 'B', 'C', 'D', 'E'};
    map<char, string> color_of_gems = {{'A', RED}, {'B', GREEN}, {'C', YELLOW}, {'D', BLUE}, {'E', PURPLE}};
    vector<char> board[10];
    int score = 0, coef = 10;

    bool create_initial_match(int row, int col)
    {
        if (row > 1)
        {
            if (board[row][col] == board[row - 1][col] && board[row][col] == board[row - 2][col])
                return false;
        }

        if (col > 1)
        {
            if (board[row][col] == board[row][col - 1] && board[row][col] == board[row][col - 2])
                return false;
        }

        return true;
    }

    char random_gem()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 4);

        int rand_idx = dist(gen);

        return symbool[rand_idx];
    }

public:
    bool is_there_match()
    {
        for (int i = 0; i < ROWS; i++)
        {
            int cnt = 1;
            for (int j = 0; j < COLS; j++)
            {
                if (j != COLS - 1 && board[i][j] == board[i][j + 1])
                {
                    cnt++;
                }
                else
                {
                    if (cnt >= 3)
                        return true;

                    cnt = 1;
                }
            }
        }

        for (int j = 0; j < COLS; j++)
        {
            int cnt = 1;
            for (int i = 0; i < ROWS; i++)
            {
                if (i != ROWS - 1 && board[i][j] == board[i + 1][j])
                {
                    cnt++;
                }
                else
                {
                    if (cnt >= 3)
                        return true;

                    cnt = 1;
                }
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
                    while (!create_initial_match(i, j))
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

    void apply_gravity()
    {
        for (int j = 0; j < COLS; j++)
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
                continue;

            for (int i = 0; i < ROWS; i++)
            {
                if (i != ROWS - 1 && board[i + 1][j] == ' ' && board[i][j] != ' ')
                {
                    last_not_empty_cell = i;
                }
            }

            if (last_not_empty_cell != ROWS + 1)
                while (last_not_empty_cell >= 0)
                {
                    swap(board[first_empty_cell][j], board[last_not_empty_cell][j]);
                    first_empty_cell--;
                    last_not_empty_cell--;
                }
        }
    }

    vector<pii> find_match()
    {
        vector<pii> match;

        for (int i = 0; i < ROWS; i++)
        {
            int cnt = 1;
            for (int j = 0; j < COLS; j++)
            {
                if (j != COLS - 1 && board[i][j] == board[i][j + 1])
                {
                    cnt++;
                }
                else
                {
                    if (cnt >= 3)
                    {
                        score += cnt * coef;

                        for (int k = 0; k < cnt; k++)
                            match.push_back({i, j - k});

                        if (cnt == 4)
                        {
                            for (int k = 0; k < j - 3; k++)
                                match.push_back({i, k});

                            for (int k = j + 1; k < COLS; k++)
                                match.push_back({i, k});
                        }

                        if (cnt == 5)
                        {
                            for (int r = 0; r < ROWS; r++)
                            {
                                for (int c = 0; c < COLS; c++)
                                {
                                    if (board[r][c] == board[i][j])
                                    {
                                        match.push_back({r, c});
                                    }
                                }
                            }
                        }
                    }
                    cnt = 1;
                }
            }
        }

        for (int j = 0; j < COLS; j++)
        {
            int cnt = 1;
            for (int i = 0; i < ROWS; i++)
            {
                if (i != ROWS - 1 && board[i][j] == board[i + 1][j])
                {
                    cnt++;
                }
                else
                {
                    if (cnt >= 3)
                    {
                        score += cnt * coef;

                        for (int k = 0; k < cnt; k++)
                            match.push_back({i - k, j});

                        if (cnt == 4)
                        {
                            for (int k = 0; k < i - 3; k++)
                                match.push_back({k, j});

                            for (int k = i + 1; k < ROWS; k++)
                                match.push_back({k, j});
                        }

                        if (cnt == 5)
                        {
                            for (int r = 0; r < ROWS; r++)
                            {
                                for (int c = 0; c < COLS; c++)
                                {
                                    if (board[r][c] == board[i][j])
                                    {
                                        match.push_back({r, c});
                                    }
                                }
                            }
                        }
                    }
                    cnt = 1;
                }
            }
        }

        return match;
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

                g.print_board();

                cout << "1 to (Swap -> r1 c1 r2 c2) or -1 to back\n";
                cin >> choice;

                if (choice == -1)
                    break;

                int r1, c1, r2, c2;
                cin >> r1 >> c1 >> r2 >> c2;

                if (!g.cascade(r1, c1, r2, c2))
                {
                    cout << "Invalid Move!\n\n";
                }
            }
        }
        else if (choice == 4)
        {
            return 0;
        }
    }
}