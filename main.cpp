#include "src/Common.h"
#include "src/RandomGemGenerator.h"
#include "src/Board.h"
#include "src/Renderer.h"
#include "src/PowerUpManger.h"
#include "src/GameSaver.h"
#include "src/Game.h"
#include "src/GameLoader.h"

void game_loop(Game &g)
{
    while (true)
    {
        system("cls");

        if (!g.game_control())
            return;
    }
}

bool validation(int a, int l, int u)
{
    return (a >= l && a <= u);
}

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

        if (!validation(choice, 1, 4))
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

            if (!validation(choice, 1, 4))
            {
                cout << RED << "[ERROR]: Your Input is invalid!\n\n"
                     << RESET;
                Sleep(error_delay);
                continue;
            }

            if (choice == 4)
                continue;

            string levels[3] = {"Easy", "Medium", "Hard"};
            Game g(levels[choice - 1]);
            g.initialize();

            game_loop(g);
        }
        else if (choice == 2)
        {
            Game g("Easy");
            GameLoader gl;

            if (!gl.load_game(g))
            {
                system("cls");
                cout << RED << "There is no unfinished game!" << RESET << endl;
                Sleep(error_delay);

                continue;
            }

            game_loop(g);
        }
        else if (choice == 3)
        {
            ifstream my_file("about_game.txt");
            string line;
            while (getline(my_file, line))
                cout << line << endl;
            my_file.close();

            cout << YELLOW << "\n\nEnter Q and Press ENTER for return." << RESET << endl;
            char c;
            cin >> c;
        }
        else if (choice == 4)
            return 0;
    }
}