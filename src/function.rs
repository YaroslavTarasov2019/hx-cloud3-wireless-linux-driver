use hidapi::{HidDevice};
use std::thread;
use std::time::Duration;

enum Command 
{
    MainCommandOfHeadset = 0x66,
    AutoShutdownStatus = 0x85, 
    BatteryStatus = 0x89,
    ChangeMicroMonitoring = 0x01,
    TimeToAutoShutdown = 0x02,
    Empty = 0x00
}

pub fn send_command(device: &HidDevice, report_id: u8, cmd: u8, data: &[u8]) 
{
    let mut tx_buf = [0u8; 62];
    tx_buf[0] = report_id;
    tx_buf[1] = cmd;
    for (i, &byte) in data.iter().enumerate() 
    {
        if i + 2 < 62 
        { 
            tx_buf[i + 2] = byte; 
        }
    }

    let mut tx_buf_rep = [0u8; 63];
    tx_buf_rep[1] = report_id;
    tx_buf_rep[2] = cmd;
    for (i, &byte) in data.iter().enumerate() 
    {
        if i + 3 < 63 
        { 
            tx_buf_rep[i + 3] = byte; 
        }
    }

    let _ = device.write(&tx_buf);
    let _ = device.write(&tx_buf_rep);
}

pub fn clear_buffer(device: &HidDevice) 
{
    let mut trash = [0u8; 64];
    while device.read_timeout(&mut trash, 5).unwrap_or(0) > 0 {}
}

pub fn change_mic_monitoring(device: &HidDevice, enable: bool) 
{
    clear_buffer(device);
    send_command(device, Command::MainCommandOfHeadset as u8, Command::ChangeMicroMonitoring as u8, &[if enable { Command::ChangeMicroMonitoring as u8 } else { Command::Empty as u8 }]);

    if enable 
    {
        println!("Microphone monitoring: ENABLED");
    } else {
        println!("Microphone monitoring: DISABLED");
    }
}

pub fn status_of_auto_shutdown(device: &HidDevice) 
{
    let mut timeout_config_res: Option<u8> = None;

    clear_buffer(device);
    send_command(device, Command::MainCommandOfHeadset as u8, Command::AutoShutdownStatus as u8, &[]);

    thread::sleep(Duration::from_millis(150));

    let mut rx_buf = [0u8; 64];
    if let Ok(bytes) = device.read_timeout(&mut rx_buf, 400) 
    {
        if bytes > 0 
        {
            let offset = if rx_buf[0] == Command::Empty as u8 { 1 } else { 0 };
            if rx_buf[offset] == Command::MainCommandOfHeadset as u8 && rx_buf[offset + 1] == Command::AutoShutdownStatus as u8 
            {
                timeout_config_res = Some(rx_buf[offset + 2]);
            }
        }
    }

    if let Some(timeout) = timeout_config_res 
    {
        if timeout == 0 
        {
            println!("Auto shutdown: Disabled (Never)");
        } else {
            println!("Auto shutdown: After {} minutes of inactivity", timeout);
        }
    } else {
        println!("Auto shutdown: Failed to retrieve setting");
    }
}

pub fn change_time_to_auto_shutdown(device: &HidDevice, target_minutes: u8) 
{
    clear_buffer(device);
    send_command(device, Command::MainCommandOfHeadset as u8, Command::TimeToAutoShutdown as u8, &[target_minutes]);

    println!("Sending command: Set sleep timer to {} minutes...", target_minutes);

    thread::sleep(Duration::from_millis(600));

    let mut rx_buf = [0u8; 64];
    match device.read_timeout(&mut rx_buf, 1000) {
        Ok(bytes_read) => {
            if bytes_read == 0 
            {
                println!("Headset did not send confirmation.");
            } else {
                let offset = if rx_buf[0] == Command::Empty as u8 { 1 } else { 0 };

                if rx_buf[offset] == Command::MainCommandOfHeadset as u8 && rx_buf[offset + 1] == Command::TimeToAutoShutdown as u8 
                {
                    let confirmed_minutes = rx_buf[offset + 2];
                    println!("Successfully written to headset memory!");
                    println!("New auto shutdown timer: {} min.", confirmed_minutes);
                } else {
                    println!("Received unexpected response: {:02X} {:02X}", rx_buf[offset], rx_buf[offset+1]);
                }
            }
        }
        Err(e) => eprintln!("Read error: {}", e),
    }
}

pub fn battery_status(device: &HidDevice) 
{
    clear_buffer(device);
    send_command(device, Command::MainCommandOfHeadset as u8, 0x89, &[]);

    thread::sleep(Duration::from_millis(100));

    let mut rx_buf = [0u8; 64];
    match device.read_timeout(&mut rx_buf, 500) 
    {
        Ok(bytes_read) => 
        {
            if bytes_read == 0 
            {
                println!("Headset did not respond. Try turning it on or playing audio.");
            } else {
                let offset = if rx_buf[0] == Command::Empty as u8 { 1 } else { 0 };

                if rx_buf[offset] == Command::MainCommandOfHeadset as u8 && rx_buf[offset + 1] == Command::BatteryStatus as u8 
                {
                    let charging_status = rx_buf[offset + 2];
                    let battery_percentage = rx_buf[offset + 4];

                    println!("Battery level: {}%", battery_percentage);

                    if charging_status == 0x0E 
                    {
                        println!("Status: Running on battery");
                    } else {
                        println!("Status: Charging / Changed (Code: 0x{:02X})", charging_status);
                    }
                } else {
                    println!("Received unexpected response: {:02X} {:02X}", rx_buf[offset], rx_buf[offset+1]);
                }
            }
        }
        Err(e) => eprintln!("Read error: {}", e),
    }
}
