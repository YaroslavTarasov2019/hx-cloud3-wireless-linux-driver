#ifndef API_H
#define API_H

#include <cstdint>

extern "C" int get_battery_status();
extern "C" int get_micmon_status();
extern "C" int get_charging_status();
extern "C" int get_status_of_auto_shutdown();
extern "C" int change_time_to_auto_shutdown(int target_minutes);
extern "C" int change_mic_monitoring(uint8_t enable);
extern "C" int show_actions();
extern "C" int get_full_mute_mode();
extern "C" int set_full_mute_mode(uint8_t enable);

#endif
