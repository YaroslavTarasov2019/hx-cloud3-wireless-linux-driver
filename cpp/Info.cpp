#include "header/Info.h"
#include "header/api.h"

Info::Info(
    int _bat_status,
    int _charging_status,
    int _status_of_auto_shutdown_res,
    int _micmon_status,
    int _full_mute_mode_status
) :
    bat_status(_bat_status),
    charging_status(_charging_status),
    status_of_auto_shutdown_res(_status_of_auto_shutdown_res),
    micmon_status(_micmon_status),
    full_mute_mode_status(_full_mute_mode_status)
{
}

const char* Info::Charging_status_string() {
    return (charging_status == 1) ? "Charging" : "Is not charging";      // 0 -- is not charging, 1 -- charging
}

const char* Info::MicMon_status_string() {
    return (micmon_status == 1) ? "Enabled" : "Disabled";
}

const char* Info::Full_mute_mode_status_string() {
    return (full_mute_mode_status == 1) ? "Enabled" : "Disabled";
}    

Info getInfo() {
    return Info ( get_battery_status(), get_charging_status(), get_status_of_auto_shutdown(), get_micmon_status(), get_full_mute_mode() );
}
