use hidapi::{HidDevice};
use std::thread;
use std::time::Duration;

mod connecting;

enum Command 
{
    MainCommandOfHeadset = 0x66,
    AutoShutdownStatus = 0x85, 
    BatteryStatus = 0x89,
    ChangeMicroMonitoring = 0x01,
    TimeToAutoShutdown = 0x02,
    Empty = 0x00
}

enum CommandLevel0
{
    VolumeBaseCommandOfHeadset = 0x0c,
    MainCommandOfHeadset = 0x66
}

enum CommandLevel1
{
    VolumeUp = 0x01,
    VolumeDown = 0x02,
    MicMute = 0x0a,
    Power = 0x0f,
    MicConnection = 0x07,
    DeviceConnection = 0x0c,
    
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


#[unsafe(no_mangle)]
pub extern "C" fn battery_status() -> i32
{
    let device = connecting::connect().expect("Failed to connect");

    clear_buffer(&device);
    send_command(&device, Command::MainCommandOfHeadset as u8, 0x89, &[]);

    thread::sleep(Duration::from_millis(100));

    let mut rx_buf = [0u8; 64];
    match device.read_timeout(&mut rx_buf, 500) 
    {
        Ok(bytes_read) => 
        {
            if bytes_read == 0 
            {
                println!("Headset did not respond. Try turning it on or playing audio.");
                return -1;
            } else {
                let offset = if rx_buf[0] == Command::Empty as u8 { 1 } else { 0 };

                if rx_buf[offset] == Command::MainCommandOfHeadset as u8 && rx_buf[offset + 1] == Command::BatteryStatus as u8 
                {
                    let battery_percentage = rx_buf[offset + 4];
                    return battery_percentage as i32;
                } else {
                    println!("Received unexpected response: {:02X} {:02X}", rx_buf[offset], rx_buf[offset+1]);
                    return -2;
                }
            }
        }
        Err(e) => 
        {
            eprintln!("Read error: {}", e);
            return -3;
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn status_of_auto_shutdown() -> i32
{
    let device = connecting::connect().expect("Failed to connect");

    let mut timeout_config_res: Option<u8> = None;

    clear_buffer(&device);
    send_command(&device, Command::MainCommandOfHeadset as u8, Command::AutoShutdownStatus as u8, &[]);

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
            return 1;
        } else {
            println!("Auto shutdown: After {} minutes of inactivity", timeout);
            return timeout as i32;
        }
    } else {
        println!("Auto shutdown: Failed to retrieve setting");
        return -1;
    }
}


#[unsafe(no_mangle)]
pub extern "C" fn change_time_to_auto_shutdown(target_minutes: i32) -> i32
{
    let device = connecting::connect().expect("Failed to connect");

    clear_buffer(&device);
    send_command(&device, Command::MainCommandOfHeadset as u8, Command::TimeToAutoShutdown as u8, &[target_minutes as u8]);

    println!("Sending command: Set sleep timer to {} minutes...", target_minutes);

    thread::sleep(Duration::from_millis(600));

    let mut rx_buf = [0u8; 64];
    match device.read_timeout(&mut rx_buf, 1000) {
        Ok(bytes_read) => {
            if bytes_read == 0 
            {
                println!("Headset did not send confirmation.");
                return -1;
            } else {
                let offset = if rx_buf[0] == Command::Empty as u8 { 1 } else { 0 };

                if rx_buf[offset] == Command::MainCommandOfHeadset as u8 && rx_buf[offset + 1] == Command::TimeToAutoShutdown as u8 
                {
                    let confirmed_minutes = rx_buf[offset + 2];
                    println!("Successfully written to headset memory!");
                    println!("New auto shutdown timer: {} min.", confirmed_minutes);
                    return confirmed_minutes as i32;
                } else {
                    println!("Received unexpected response: {:02X} {:02X}", rx_buf[offset], rx_buf[offset+1]);
                    return -2;
                }
            }
        }
        Err(e) => 
        {
            eprintln!("Read error: {}", e);
            return -3;
        },
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn change_mic_monitoring(enable: bool) -> i32
{
    let device = connecting::connect().expect("Failed to connect");

    clear_buffer(&device);
    send_command(&device, Command::MainCommandOfHeadset as u8, Command::ChangeMicroMonitoring as u8, &[if enable { Command::ChangeMicroMonitoring as u8 } else { Command::Empty as u8 }]);

    if enable 
    {
        println!("Microphone monitoring: ENABLED");
        return 1;
    } else {
        println!("Microphone monitoring: DISABLED");
        return 2;
    }
}



#[unsafe(no_mangle)]
pub extern "C" fn show_actions() -> i32
{
    let device = match connecting::connect() {
        Ok(d) => d,
        Err(_) => return -1,
    };
    let mut buf = [0u8; 64];
    
    loop 
    {
        let n = match device.read_timeout(&mut buf, 1000) {
            Ok(bytes) => bytes,
            Err(_) => return -2,
        };
        if n == 0 
        {
            continue;
        }

        if buf[0] == CommandLevel0::VolumeBaseCommandOfHeadset as u8 
        {
            match buf[1] {
                x if x == CommandLevel1::VolumeUp as u8 => 
                {
                    return 1;
                }
                x if x == CommandLevel1::VolumeDown as u8 => 
                {
                    return 2;
                }
                _ => {}
            }
        }

        if buf[0] == CommandLevel0::MainCommandOfHeadset as u8
        {
            match buf[1] 
            {
                x if x == CommandLevel1::MicMute as u8 => 
                {
                    match buf[2] 
                    {
                        0x01 => return 3,
                        0x00 => return 4,
                        _ => {}
                    }
                },
                x if x == CommandLevel1::Power as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => return 5,
                        0x01 => return 6,
                        _ => {}
                    }
                },
                x if x == CommandLevel1::MicConnection as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => return 7,
                        0x01 => return 8,
                        _ => {}
                    }
                },
                x if x == CommandLevel1::DeviceConnection as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => return 9,
                        0x01 => return 10,
                        _ => {}
                    }
                },
                _ => {}
            }
        }
    }
}
