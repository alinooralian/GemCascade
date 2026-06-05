#include <bits/stdc++.h>
#include <random>

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

const int ROWS = 8, COL = 8;

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
    void initialize()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                board[i].push_back(random_gem());
                while (!create_initial_match(i, j))
                {
                    board[i].pop_back();
                    board[i].push_back(random_gem());
                }
            }
        }
    }

    void vertical_line()
    {
        cout << "  ";

        for (int i = 0; i < COL; i++)
            cout << "+-----";
        cout << "+" << endl;
    }

    void horizontal_line(int row)
    {
        cout << row << " ";

        for (int i = 0; i < COL; i++)
            cout << "|" << "  " << color_of_gems[board[row][i]] << board[row][i] << RESET << "  ";

        cout << "|" << endl;
    }

    void print_board()
    {
        cout << "Score: " << score << "\n\n";

        cout << "  ";

        for (int i = 0; i < COL; i++)
            cout << "   " << i << "  ";
        cout << endl;

        for (int i = 0; i < ROWS; i++)
        {
            vertical_line();
            horizontal_line(i);
        }
        vertical_line();
    }
};

int main()
{

    return 0;
}