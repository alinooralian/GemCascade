#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include "Board.h"
#include "Renderer.h"
#include "PowerUpManger.h"
#include "GameSaver.h"

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

#endif
