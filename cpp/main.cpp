#include <ncurses.h>
#include "header/api.h"
#include "header/Info.h"
#include "header/action.h"


const int WIDTH = 60;
const char* items[] = {
    "Change full_mute mode to the opposite", 
    "Change auto shutdown to never auto shutdown", "Change auto shutdown to 10 minutes", 
    "Change auto shutdown to 20 minutes", "Change auto shutdown to 30 minutes", 
    "Change mic monitoring to the oposite", "Show actions", "Exit"
};

void print_border() {
    printw("+%.*s+\n", WIDTH, "------------------------------------------------------------");
}

int main()
{
    initscr();              
    noecho();            
    cbreak();               
    keypad(stdscr, TRUE);   

    int selected = 0;
    
    size_t length = sizeof(items) / sizeof(items[0]);

    Info info = getInfo();

    while (true)
    {
        clear();
        print_border();
        printw("| [u]pdate  |  [q]uit%*s|\n", WIDTH - 20, "");
        print_border();
        printw("| Battery: %-3d%% %*s |\n", info.bat_status, WIDTH - 14 - 2, "");
        printw("| Result: %-15s %*s |\n", info.Charging_status_string(), WIDTH - 24 - 2, "");
        printw("| Status_of_auto_shutdown: ");
        if (info.status_of_auto_shutdown_res == 1) {
            printw("Never %*s |\n", WIDTH - 33, "");
        }
        else if (info.status_of_auto_shutdown_res < 0) {
            printw("Error! %*s |\n", WIDTH - 34, "");
        }
        else {
            printw("%-2d %*s |\n", info.status_of_auto_shutdown_res, WIDTH - 30, "");
        }
        printw("| MicMonitoring Status: %-8s %*s |\n", info.MicMon_status_string(), WIDTH - 31 - 2, "");
        printw("| Full Mute Mode Status: %-8s %*s |\n", info.Full_mute_mode_status_string(), WIDTH - 32 - 2, "");
        print_border();

        for (int i = 0; i < length; i++)
        {
            if (i == selected)
            {
                attron(A_BOLD);
                printw("| -> %-45s %*s |\n", items[i], WIDTH - 51, "");
                attroff(A_BOLD);
            }
            else
                printw("|    %-45s %*s |\n", items[i], WIDTH - 51, "");
        }

        print_border();

        int ch = getch();
        switch (ch)
        {
            case KEY_UP:
                if (selected > 0) selected--;
                break;

            case KEY_DOWN:
                if (selected < length - 1) selected++;
                break;

            case 10:
            {
                int r = action(selected);
                if (r == -1)
                    return 0;
                info = getInfo();
            }
            break;

            case 'q':
                endwin();
                return 0;
            
            case 'u':
                info = getInfo();
        }
    }

    endwin();
    return 0;
}
