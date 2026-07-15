#ifndef INFO_H
#define INFO_H

struct Info {
    int bat_status;
    int charging_status;
    int status_of_auto_shutdown_res;
    int micmon_status;
    int full_mute_mode_status;

    Info(int _bat_status, int _charging_status, int _status_of_auto_shutdown_res, int _micmon_status, int _full_mute_mode_status);
    
    const char* Charging_status_string();
    const char* MicMon_status_string();
    const char* Full_mute_mode_status_string();
};

Info getInfo();

#endif
