#include <ncurses.h>

extern "C" int get_battery_status();
extern "C" int get_micmon_status();
extern "C" int get_charging_status();
extern "C" int get_status_of_auto_shutdown();
extern "C" int change_time_to_auto_shutdown(int target_minutes);
extern "C" int change_mic_monitoring(bool enable);
extern "C" int show_actions();
extern "C" int get_full_mute_mode();
extern "C" int set_full_mute_mode(bool enable);

int action(int ch)
{
    int act, arg;

    switch(ch)
    {
        case 0:
        {
            if (get_full_mute_mode() == 1)
                set_full_mute_mode(false);
            else
            {
                set_full_mute_mode(true);
            }
        }
            break;
        case 1:
        {
            clear();

            int result = change_time_to_auto_shutdown(0);

            if (result < 0)
            {
                printw("Error!");
            }
            else
            {
                printw("Status: %d\n", result);
            }
            printw("Press any key...");

            refresh();     
            getch();     

            endwin();   
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

            refresh();     
            getch();     

            endwin();      
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

            refresh();    
            getch();      

            endwin();      
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

            refresh();    
            getch();      

            endwin();    
        }
            break;
        case 5:
        {
            clear();

            if (get_micmon_status() == 1)
            {
                int result = change_mic_monitoring(false);

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
            }
            else
            {
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
            }

            printw("Press any key...");

            refresh();   
            getch();   

            endwin();  
        }
            break;
        case 6:
        {
            
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

            refresh();   
            getch();       

            endwin();       
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

const int WIDTH = 60;
const char* items[] = {"Change full_mute mode to the opposite", "Change auto shutdown to never auto shutdown", "Change auto shutdown to 10 minutes", "Change auto shutdown to 20 minutes", 
        "Change auto shutdown to 30 minutes", "Change mic monitoring to the oposite", "Empty", "Show actions", "Exit"};

void print_border() {
    printw("+%.*s+\n", WIDTH, "------------------------------------------------------------");
}

struct Info {
    int bat_status;
    int charging_status;
    int status_of_auto_shutdown_res;
    int micmon_status;
    int full_mute_mode_status;

    Info(int _bat_status, int _charging_status, int _status_of_auto_shutdown_res, int _micmon_status, int _full_mute_mode_status) : 
        bat_status(_bat_status), 
        charging_status(_charging_status), 
        status_of_auto_shutdown_res(_status_of_auto_shutdown_res), 
        micmon_status(_micmon_status),
        full_mute_mode_status(_full_mute_mode_status) 
        {}
    
    const char* Charging_status_string() {
        return (charging_status == 1) ? "Charging" : "Is not charging";
    }

    const char* MicMon_status_string() {
        return (micmon_status == 1) ? "Enabled" : "Disabled";
    }

    const char* Full_mute_mode_status_string() {
        return (full_mute_mode_status == 1) ? "Enabled" : "Disabled";
    }    
};

Info getInfo() {
    return Info ( get_battery_status(), get_charging_status(), get_status_of_auto_shutdown(), get_micmon_status(), get_full_mute_mode() );
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
        printw("| Result: %-15s %*s |\n", info.Charging_status_string(), WIDTH - 24 - 2, "");   // 0 -- is not charging, 1 -- charging
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
        printw("| full_muteMode Status: %-8s %*s |\n", info.Full_mute_mode_status_string(), WIDTH - 28 - 2, "");

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
