#include "header/action.h"

void change_time_to_auto_shutdown_local(int minutes)
{
    int result = change_time_to_auto_shutdown(minutes);

    if (result < 0)
    {
        clear();
        printw("Error!\n");
        printw("Press any key...");
        refresh();     
        getch();     
        endwin();   
    }
}

void change_mic_monitoring_local()
{
    if (get_micmon_status() == 1)
    {
        int result = change_mic_monitoring(0);
        if (result != 1 && result != 2)
        {
            clear();
            printw("Error!\n");
            printw("Press any key...");

            refresh();   
            getch();   

            endwin();
        }
    }
    else
    {
        int result = change_mic_monitoring(1);
        if (result != 1 && result != 2)
        {
            clear();
            printw("Error!\n");
            printw("Press any key...");

            refresh();   
            getch();   

            endwin();
        }
    }
}

void recording()
{
    clear();
    printw("Record start\n");
    refresh();
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

int action(int ch)
{
    int act, arg;

    switch(ch)
    {
        case 0:
        {
            if (get_full_mute_mode() == 1)
                set_full_mute_mode(0);
            else
            {
                set_full_mute_mode(1);
            }
        }
            break;
        case 1:
        {
            change_time_to_auto_shutdown_local(0);
        }
            break;
        case 2:
        {
            change_time_to_auto_shutdown_local(10);
        }
            break;
        case 3:
        {
            change_time_to_auto_shutdown_local(20);      
        }
            break;
        case 4:
        {
            change_time_to_auto_shutdown_local(30);    
        }
            break;
        case 5:
        {
            change_mic_monitoring_local();
        }
            break;
        case 6:
        {
            recording();       
        }
            break;
        case 7:
        {
            return -1;            
        }
            break;
        default:
            break;
    }
    return 0;
}
