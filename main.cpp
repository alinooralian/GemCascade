#include <bits/stdc++.h>
#include <random>
#include <windows.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\x1b[38;5;227m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define BROWN "\x1b[38;5;130m"
#define PINK "\x1b[38;5;206m"
#define ORANGE "\x1b[38;5;209m"
#define GREENII "\x1b[38;5;184m"
#define PURPLEII "\x1b[38;5;163m"
#define GREENIII "\x1b[38;5;155m"
#define BLUEII "\x1b[38;5;63m"
#define ORANGEII "\x1b[38;5;9m"
#define PINKII "\x1b[38;5;205m"
#define RESET "\e[0m"
#define ff first
#define ss second
typedef pair<int, int> pii;

const int ROWS = 8, COLS = 8;

class Game
{
private:
    char symbool[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    map<char, string> color_of_gems = {{'A', RED}, {'B', GREEN}, {'C', YELLOW}, {'D', BLUE}, {'E', PURPLE}, {'F', CYAN}, {'G', BROWN}, {'H', PINK}};
    vector<char> board[ROWS];
    int score = 0, coef = 10;
    int moves, point_limit;
    string level;
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

    char random_gem()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 7);

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

    void bomb(int row, int col)
    {
        score -= 100;

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

                board[r][c] = ' ';
            }
        }

        if (row > 0)
            cascade(row, col, row - 1, col);
        else
            cascade(row, col, row + 1, col);
    }

    void rocket(char type, int num)
    {
        score -= 120;

        if (type == 'R')
        {
            for (int j = 0; j < COLS; j++)
                board[num][j] = ' ';

            cascade(num, 0, num, 1);
        }
        else
        {
            for (int i = 0; i < ROWS; i++)
                board[i][num] = ' ';

            cascade(0, num, 1, num);
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
        score -= 70;

        return {{r1, c1}, {r2, c2}};
    }

    bool cascade(int r1, int c1, int r2, int c2)
    {
        if (abs(r1 - r2) + abs(c1 - c2) != 1)
            return false;

        swap(board[r1][c1], board[r2][c2]);
        print_board();
        cout << endl;
        Sleep(3000);

        bool check = is_there_match();

        if (!check)
        {
            swap(board[r1][c1], board[r2][c2]);
            return false;
        }

        while (check)
        {
            vector<pii> match = find_match();

            score += match.size() * coef;

            for (auto cell : match)
                board[cell.ff][cell.ss] = ' ';

            print_board();
            cout << endl;
            Sleep(2500);

            apply_gravity();
            print_board();
            cout << endl;
            Sleep(2500);

            refill();
            print_board();
            cout << endl;
            Sleep(3000);

            check = is_there_match();
        }

        (coef >= 40) ? (coef += 2) : (coef *= 2);

        if (!is_valid_board())
            initialize();

        return true;
    }

public:
    Game(string l)
    {
        level = l;

        if (level == "Easy")
        {
            moves = 50;
            point_limit = 14720;
        }
        else if (level == "Medium")
        {
            moves = 30;
            point_limit = 5830;
        }
        else
        {
            moves = 10;
            point_limit = 3530;
        }
    }

    bool load_game()
    {
        ifstream my_file("gemcascade.txt");

        if (!my_file.is_open())
        {
            system("cls");
            cout << RED << "There is no unfinished game!" << RESET << endl;
            Sleep(4000);

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
                char arr[my_text.length() + 1];
                strcpy(arr, my_text.c_str());

                int j = i / 8;
                vc[j].push_back(arr[0]);
            }
            else
            {
                if (i == 64)
                {
                    s = stoi(my_text);
                }
                if (i == 65)
                {
                    c = stoi(my_text);
                }
                if (i == 66)
                {
                    m = stoi(my_text);
                }
                if (i == 67)
                {
                    l = my_text;
                }
            }

            i++;
        }

        level = l;
        score = s;
        coef = c;
        moves = m;

        for (int i = 0; i < ROWS; i++)
            board[i] = vc[i];

        my_file.close();

        return true;
    }

    void initialize()
    {
        map<char, bool> mp;

        while (true)
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

                        mp[board[i][j]] = true;
                    }
                }

            } while (!is_valid_board());

            bool check = true;

            for (int i = 0; i < ROWS; i++)
            {
                if (!mp[symbool[i]])
                {
                    check = false;
                    break;
                }
            }

            if (check)
                break;
        }
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
        if (score >= point_limit)
        {
            cout << GREENII << "Congratulations!\nYou won." << RESET << endl;

            Sleep(5000);
            system("cls");

            return false;
        }

        if (moves == 0)
        {
            if (score >= point_limit)
                cout << GREENII << "Congratulations!\nYou won." << RESET << endl;
            else
                cout << RED << "Sorry!\nYou lost." << RED << endl;

            Sleep(5000);
            system("cls");

            return false;
        }

        for (int i = 0; i < 113; i++)
            cout << '*';
        cout << endl;

        cout << GREEN << "|\tScore: " << score << "\t|" << RESET;
        cout << YELLOW << "|\tRemaining Moves: " << moves << "\t|" << RESET;
        cout << PINK << "|\tLevel: " << level << "\t|" << RESET;
        cout << ORANGE << "|\tGoal Score: " << point_limit << "\t|" << RESET << endl;

        for (int i = 0; i < 113; i++)
            cout << '*';
        cout << "\n\n";

        print_board();

        cout << "\n\n";
        for (int i = 0; i < 120; i++)
            cout << "*";
        cout << "\n\n";

        cout << BLUEII << "[CONTROLS]:" << RESET;
        cout << ORANGE << " W: Swap " << RESET << '|';
        cout << GREENII << " H: Hint(-70 Score) " << RESET << '|';
        cout << CYAN << " R: Rocket(-120 Score) " << RESET << '|';
        cout << PURPLEII << " B: Bomb(-100 Score) " << RESET << '|';
        cout << YELLOW << " S: Save " << RESET << '|';
        cout << RED << " Q: Quit" << RESET;

        cout << "\n\n";

        char choice;
        cin >> choice;
        cout << endl;

        if (choice == 'W')
        {
            cout << BLUEII << "[INPUT]: " << "Enter First Row & Col:\n\n"
                 << RESET;

            int r1, c1;
            cin >> r1 >> c1;
            cout << endl;

            cout << BLUEII << "[INPUT]:" << " Enter Second Row & Col:\n\n"
                 << RESET;

            int r2, c2;
            cin >> r2 >> c2;
            cout << endl;

            Sleep(1000);

            system("cls");

            if (!cascade(r1, c1, r2, c2))
            {
                cout << RED << "Invalid Move!\n\n"
                     << RESET;

                Sleep(3000);
            }
            else
            {
                moves--;
            }
        }

        if (choice == 'S')
        {
            system("cls");

            ofstream my_file("gemcascade.txt");

            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    my_file << board[i][j] << '\n';
                }
            }

            my_file << score << '\n'
                    << coef << '\n'
                    << moves << '\n'
                    << level;

            cout << GREENIII << "Game Saved!" << RESET << endl;
            Sleep(3000);

            my_file.close();
        }

        if (choice == 'B')
        {
            if (score >= 100)
            {
                cout << BLUEII << "[INPUT]:" << " Enter Row & Col:\n\n"
                     << RESET;

                int r, c;
                cin >> r >> c;
                cout << endl;

                bomb(r, c);
            }
            else
            {
                cout << RED << "[ERORR]:" << " Your score is less than 100.\n\n"
                     << RESET;
            }

            Sleep(2000);
        }

        if (choice == 'R')
        {
            if (score >= 120)
            {
                cout << BLUEII << "[INPUT]:" << " Enter R or C:\n\n"
                     << RESET;

                char type;
                cin >> type;
                cout << endl;

                if (type == 'R')
                    cout << BLUEII << "[INPUT]:" << " Enter Row Number:\n\n"
                         << RESET;
                else
                    cout << BLUEII << "[INPUT]:" << " Enter Col Number:\n\n"
                         << RESET;

                int num;
                cin >> num;
                cout << endl;

                rocket(type, num);
            }
            else
            {
                cout << RED << "[ERORR]:" << " Your score is less than 120.\n\n"
                     << RESET;
            }

            Sleep(2000);
        }

        if (choice == 'H')
        {
            if (score >= 70)
            {
                vector<pii> v = hint();

                cout << GREENIII << "[HINT]:" << "Swap " << v[0].ff << ' ' << v[0].ss << " with " << v[1].ff << ' ' << v[1].ss << "\n\n"
                     << RESET;
            }
            else
            {
                cout << RED << "[ERORR]:" << " Your score is less than 70.\n\n"
                     << RESET;
            }

            Sleep(4000);
        }

        if (choice == 'Q')
        {
            system("cls");
            return false;
        }

        return true;
    }
};

int main()
{
    while (true)
    {
        system("cls");

        cout << ORANGE << "Enter number of your choice:" << RESET << endl;
        cout << ORANGEII << "1.New Game\n2.Load Game\n3.About this game\n4.Exit" << RESET << "\n\n";

        int choice;
        cin >> choice;
        cout << endl;

        system("cls");

        if (choice == 1)
        {
            cout << PINK << "Choose the game level:" << RESET << endl;
            cout << PINKII << "1.Easy\n2.Medium\n3.Hard\n4.Back" << RESET << "\n\n";

            cin >> choice;
            cout << endl;

            if (choice == 4)
                continue;

            string tmp[] = {"Easy", "Medium", "Hard"};

            Game g(tmp[choice - 1]);
            g.initialize();

            while (true)
            {
                system("cls");

                if (!g.game_control())
                    break;
            }
        }
        else if (choice == 2)
        {
            Game g("Easy");

            if (!g.load_game())
                continue;

            while (true)
            {
                system("cls");

                if (!g.game_control())
                    break;
            }
        }
        else if(choice == 3)
        {
            ifstream my_file("about_game.txt");

            string my_text;

            while(getline(my_file, my_text))
            {
                cout << my_text << endl;
            }

            my_file.close();

            cout << "\n\n";

            cout << YELLOW << "Enter Q and Press ENTER for return." << RESET << endl;

            char c;
            cin >> c;
        }
        else if (choice == 4)
        {
            return 0;
        }
    }
}