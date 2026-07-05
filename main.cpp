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
const int swap_delay = 2000;
const int cascade_delay = 3000;
const int error_delay = 4000;
const int process_delay = 3500;
const int loading_delay = 2500;

class RndomGemGenerator
{
private:
    char symbool[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    mt19937 gen{random_device{}()};

public:
    char generator()
    {
        uniform_int_distribution<> dist(0, 7);

        int rand_idx = dist(gen);
        return symbool[rand_idx];
    }
};

class Board
{
private:
    char symbool[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    RndomGemGenerator random_gem_generator;
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

                board[i][j] = random_gem_generator.generator();
            }
        }
    }

    void initialize()
    {
        map<char, bool> mp;

        while (true)
        {
            do
            {
                for (int i = 0; i < ROWS; i++)
                {
                    for (int j = 0; j < COLS; j++)
                    {
                        do
                        {
                            board[i][j] = random_gem_generator.generator();
                        } while (create_initial_match(i, j));

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
};

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

class Game
{
private:
    Board board;
    Renderer renderer;
    int score, coef, moves, goal_score, bomb_cost, rocket_cost, hint_cost;
    string level;

    bool validation(int r1, int c1, int r2, int c2)
    {
        if (r1 >= ROWS || r2 >= ROWS || r1 < 0 || r2 < 0)
            return false;
        if (c1 >= COLS || c2 >= COLS || c1 < 0 || c2 < 0)
            return false;
        return true;
    }

    bool cascade(int r1, int c1, int r2, int c2)
    {
        if (abs(r1 - r2) + abs(c1 - c2) != 1)
            return false;

        board.swap_cells(r1, c1, r2, c2);
        renderer.print_board(board);
        cout << endl;
        Sleep(swap_delay);

        bool check = board.is_there_match();

        if (!check)
        {
            board.swap_cells(r1, c1, r2, c2);
            return false;
        }

        while (check)
        {
            vector<pii> match = board.find_match();
            score += match.size() * coef;

            for (auto cell : match)
                board.set_cell(cell.ff, cell.ss, ' ');

            renderer.print_board(board);
            cout << endl;
            Sleep(loading_delay);

            board.apply_gravity();
            renderer.print_board(board);
            cout << endl;
            Sleep(loading_delay);

            board.refill();
            renderer.print_board(board);
            cout << endl;
            Sleep(cascade_delay);

            check = board.is_there_match();
        }

        (coef >= 40) ? (coef += 2) : (coef *= 2);

        if (!board.is_valid_board())
            board.initialize();

        return true;
    }

    bool win_status()
    {
        if (score >= goal_score)
        {
            cout << GREENII << "Congratulations!\nYou won." << RESET << endl;

            Sleep(error_delay);
            system("cls");

            return true;
        }

        if (moves == 0)
        {
            if (score >= goal_score)
                cout << GREENII << "Congratulations!\nYou won." << RESET << endl;
            else
                cout << RED << "Sorry!\nYou lost." << RESET << endl;

            Sleep(error_delay);
            system("cls");

            return true;
        }

        return false;
    }

public:
    Game(string l)
    {
        level = l;
        score = 0;
        coef = 10;
        bomb_cost = 100;
        rocket_cost = 120;
        hint_cost = 70;

        if (level == "Easy")
        {
            moves = 50;
            goal_score = 14700;
        }
        else if (level == "Medium")
        {
            moves = 30;
            goal_score = 5800;
        }
        else
        {
            moves = 10;
            goal_score = 3500;
        }
    }

    void set_level(string l)
    {
        level = l;
    }

    void set_coef(int c)
    {
        coef = c;
    }

    void set_score(int s)
    {
        score = s;
    }

    void set_moves(int m)
    {
        moves = m;
    }

    void set_board(vector<vector<char>> b)
    {
        board.set_board(b);
    }

    void initialize()
    {
        board.initialize();
    }

    void swap_handler()
    {
        cout << BLUEII << "[INPUT]: Enter First Row & Col(r1 c1):\n\n"
             << RESET;

        int r1, c1;
        cin >> r1 >> c1;
        cout << endl;

        cout << BLUEII << "[INPUT]: Enter Second Row & Col(r2 c2):\n\n"
             << RESET;

        int r2, c2;
        cin >> r2 >> c2;
        cout << endl;

        Sleep(loading_delay);
        system("cls");

        if (!validation(r1, c1, r2, c2))
        {
            cout << RED << "[ERROR]: Your inputs are invalid!\n\n"
                 << RESET;
            Sleep(error_delay);
        }

        if (!cascade(r1, c1, r2, c2))
        {
            cout << RED << "Invalid Move!\n\n"
                 << RESET;
            Sleep(error_delay);
        }
        else
            moves--;
    }

    void save_handler()
    {
        GameSaver gs;

        if (gs.save_game(level, coef, score, moves, board))
        {
            cout << GREENIII << "Game Saved!" << RESET << endl;
            Sleep(process_delay);
        }
    }

    void bomb_handler()
    {
        if (score >= bomb_cost)
        {
            PowerUpManger pu;

            cout << BLUEII << "[INPUT]:" << " Enter Row & Col(r c):\n\n"
                 << RESET;

            int r, c;
            cin >> r >> c;
            cout << endl;

            system("cls");

            if (!validation(r, c, 0, 0))
            {
                cout << RED << "[ERROR]: Your inputs are invalid!\n\n"
                     << RESET;

                Sleep(error_delay);

                return;
            }

            score -= bomb_cost;
            pu.bomb(r, c, board);
            moves--;

            if (r > 0)
                cascade(r, c, r - 1, c);
            else
                cascade(r, c, r + 1, c);
        }
        else
        {
            cout << RED << "[ERORR]:" << " Your score is less than 100.\n\n"
                 << RESET;
        }

        Sleep(process_delay);
    }

    void rocket_handler()
    {
        if (score >= rocket_cost)
        {
            PowerUpManger pu;

            cout << BLUEII << "[INPUT]:" << " Enter R or C:\n\n"
                 << RESET;

            char type;
            cin >> type;
            cout << endl;

            if (type != 'C' && type != 'c' && type != 'R' && type != 'r')
            {
                cout << RED << "[ERROR]: Your input is invalid!\n\n"
                     << RESET;

                Sleep(error_delay);

                return;
            }

            if (type == 'R' || type == 'r')
                cout << BLUEII << "[INPUT]:" << " Enter Row Number(r):\n\n"
                     << RESET;
            else
                cout << BLUEII << "[INPUT]:" << " Enter Col Number(c):\n\n"
                     << RESET;

            int num;
            cin >> num;
            cout << endl;

            system("cls");

            if (!validation(num, 0, 0, 0))
            {
                cout << RED << "[ERROR]: Your input is invalid!\n\n"
                     << RESET;

                Sleep(error_delay);

                return;
            }

            pu.rocket(type, num, board);
            score -= rocket_cost;
            moves--;

            if (type == 'R' || type == 'r')
                cascade(num, 0, num, 1);
            else
                cascade(0, num, 1, num);
        }
        else
        {
            cout << RED << "[ERORR]:" << " Your score is less than 120.\n\n"
                 << RESET;
        }

        Sleep(process_delay);
    }

    void hint_handler()
    {
        if (score >= hint_cost)
        {
            PowerUpManger pu;

            vector<pii> v = pu.hint(board);
            score -= hint_cost;

            cout << GREENIII << "[HINT]:" << "Swap " << v[0].ff << ' ' << v[0].ss << " with " << v[1].ff << ' ' << v[1].ss << "\n\n"
                 << RESET;
        }
        else
        {
            cout << RED << "[ERORR]:" << " Your score is less than 70.\n\n"
                 << RESET;
        }

        Sleep(error_delay);
    }

    bool game_control()
    {
        if (win_status())
            return false;

        renderer.print_header(score, moves, level, goal_score);
        renderer.print_board(board);
        renderer.print_controller(hint_cost, rocket_cost, bomb_cost);

        char choice;
        cin >> choice;
        cout << endl;

        if (choice == 'W' || choice == 'w')
            swap_handler();
        else if (choice == 'S' || choice == 's')
            save_handler();
        else if (choice == 'B' || choice == 'b')
            bomb_handler();
        else if (choice == 'R' || choice == 'r')
            rocket_handler();
        else if (choice == 'H' || choice == 'h')
            hint_handler();
        else if (choice == 'Q' || choice == 'q')
        {
            system("cls");
            return false;
        }
        else
        {
            cout << RED << "[ERROR]: Your input is invalid!\n\n"
                 << RESET;
            Sleep(error_delay);
        }

        return true;
    }
};

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

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    while (true)
    {
        system("cls");

        cout << ORANGE << "Enter number of your choice:" << RESET << endl;
        cout << ORANGEII << "1.New Game\n2.Load Game\n3.About this game\n4.Exit" << RESET << "\n\n";

        int choice;
        cin >> choice;
        cout << endl;

        if (choice < 1 || choice > 4)
        {
            cout << RED << "[ERROR]: Your Input is invalid!\n\n"
                 << RESET;
            Sleep(error_delay);
            continue;
        }

        system("cls");

        if (choice == 1)
        {
            cout << PINK << "Choose the game level:" << RESET << endl;
            cout << PINKII << "1.Easy\n2.Medium\n3.Hard\n4.Back" << RESET << "\n\n";

            cin >> choice;

            if (choice == 4)
                continue;

            string levels[3] = {"Easy", "Medium", "Hard"};
            Game g(levels[choice - 1]);
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
            GameLoader gl;

            if (!gl.load_game(g))
                continue;

            while (true)
            {
                system("cls");

                if (!g.game_control())
                    break;
            }
        }
        else if (choice == 3)
        {
            ifstream my_file("about_game.txt");
            string line;
            while (getline(my_file, line))
                cout << line << endl;
            my_file.close();

            cout << YELLOW << "\nEnter Q and Press ENTER for return." << RESET << endl;
            char c;
            cin >> c;
        }
        else if (choice == 4)
            return 0;
    }
}