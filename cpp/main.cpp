#include <ncurses.h>

extern "C" int battery_status();
extern "C" int charging_status();
extern "C" int status_of_auto_shutdown();
extern "C" int change_time_to_auto_shutdown(int target_minutes);
extern "C" int change_mic_monitoring(bool enable);
extern "C" int show_actions();

int action(int ch)
{
    int act, arg;

    switch(ch)
    {
        case 0:
        {
            clear();

            int result = battery_status();
            printw("Result: %d\n", result);
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 1:
        {
            clear();

            int result = status_of_auto_shutdown();

            if (result == 1)
            {
                printw("Status: Never");
            }
            else if (result < 0)
            {
                printw("Error!");
            }
            else
            {
                printw("Status: %d\n", result);
            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 2:
        {
            clear();

            int result = change_time_to_auto_shutdown(10);

            if (result < 0)
            {
                printw("Error!");
            }
            else
            {
                printw("Status: %d\n", result);
            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 3:
        {
            clear();

            int result = change_time_to_auto_shutdown(20);

            if (result < 0)
            {
                printw("Error!");
            }
            else
            {
                printw("Status: %d\n", result);
            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 4:
        {
            clear();

            int result = change_time_to_auto_shutdown(30);

            if (result < 0)
            {
                printw("Error!");
            }
            else
            {
                printw("Status: %d\n", result);
            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 5:
        {
            clear();

            int result = change_mic_monitoring(true);

            if (result == 1)
            {
                printw("Status: Enabled\n");
            }
            else if (result == 2)
            {
                printw("Status: Disabled\n");
            }
            else
            {
                printw("Error!\n");
            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 6:
        {
            clear();

            int result = change_mic_monitoring(false);

            if (result == 1)
            {
                printw("Status: %d\n", result);
            }
            else if (result == 2)
            {
                printw("Status: %d\n", result);
            }
            else
            {

            }
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 7:
        {
            clear();
            printw("Record start\n");
            refresh();      // показать
            while (true)
            {
                int result = show_actions();

                if (result == 1)
                {
                    printw("Volume Up\n");
                }
                else if (result == 2)
                {
                    printw("Volume Down\n");
                }
                else if (result == 3)
                {
                    printw("Microphone MUTED\n");
                }
                else if (result == 4)
                {
                    printw("Microphone UNMUTED\n");
                }
                else if (result == 5)
                {
                    printw("Headset turned OFF\n");
                }
                else if (result == 6)
                {
                    printw("Headset turned ON\n");
                }
                else if (result == 7)
                {
                    printw("Microphone disconnected\n");
                }
                else if (result == 8)
                {
                    printw("Microphone connected\n");
                }
                else if (result == 9)
                {
                    printw("Device connected to this PC\n");
                }
                else if (result == 10)
                {
                    printw("Device disconnected from this PC\n");
                }
                else
                {
                    
                }
                refresh();
            }
            
            printw("Press any key...");

            refresh();      // показать
            getch();        // подождать

            endwin();       // корректно закрыть ncurses
        }
            break;
        case 8:
        {
            return -1;            
        }
            break;
        default:

            break;
    }
    return 0;
}


int main()
{
    initscr();              // старт ncurses
    noecho();               // не печатать ввод
    cbreak();               // мгновенный ввод
    keypad(stdscr, TRUE);   // включить стрелки

    int amount_variants = 9;

    int selected = 0;
    const char* items[] = {"Battery", "Status of auto shutdown", "Change auto shutdown to 10 minutes", "Change auto shutdown to 20 minutes", 
        "Change auto shutdown to 30 minutes", "Change mic monitoring to enabled", "Change mic monitoring to disabled", "Show actions", "Exit"};

    while (true)
    {
        clear();

        for (int i = 0; i < amount_variants; i++)
        {
            if (i == selected)
                printw("-> %s\n", items[i]);
            else
                printw("   %s\n", items[i]);
        }

        int ch = getch();

        switch (ch)
        {
            case KEY_UP:
                if (selected > 0) selected--;
                break;

            case KEY_DOWN:
                if (selected < amount_variants - 1) selected++;
                break;

            case 10:
            {
                int r = action(selected);

                if (r == -1)
                    return 0;
            }
            break;

            case 'q':
                endwin();
                return 0;
        }
    }

    endwin();
    return 0;
}