use hidapi::{HidApi};
use std::io::{self, Write};

mod function;

const HYPERX_VID: u16 = 0x03f0;
const HYPERX_PID: u16 = 0x05b7;

enum VariablesTimeToAutoShutdown
{
    Never = 0,
    Min10 = 10,
    Min20 = 20, 
    Min30 = 30
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

fn main() -> Result<(), hidapi::HidError> 
{
    let api = match HidApi::new() 
    {
        Ok(api) => api,
        Err(e) => 
        {
            eprintln!("HID API error: {}", e);
            return Ok(());
        }
    };

    let devices: Vec<_> = api.device_list().collect();

    let device_info = devices.iter().find(|d| {
        d.vendor_id() == HYPERX_VID && d.product_id() == HYPERX_PID && d.interface_number() == 3
    }).expect("Headset interface not found! Please check the USB dongle.");

    let device = match device_info.open_device(&api) 
    {
        Ok(dev) => dev,
        Err(_) => 
        {
            eprintln!("Device access blocked. Please close NGENUITY software if it is running!");
            return Ok(());
        }
    };
    let mut buf = [0u8; 64];

    println!("=========================================");
    println!("HyperX Cloud 3 Wireless Status     ");
    println!("=========================================");
    
    function::battery_status(&device);

    println!("=========================================");

    function::status_of_auto_shutdown(&device);

    println!("=========================================");

    print!("Change Mic Monitoring / Auto Shutdown time? [M/T] ");
    io::stdout().flush().unwrap();

    let mut input_a: String = String::new();
    io::stdin()
        .read_line(&mut input_a)
        .expect("Failed to read line");
    let input_a = input_a.trim().to_lowercase();

    if input_a == "m"
    {
        print!("Enable/Disable? [E/D] ");
        io::stdout().flush().unwrap();

        let mut input_a1: String = String::new();
        io::stdin()
            .read_line(&mut input_a1)
            .expect("Failed to read line");
        let input_a1 = input_a1.trim().to_lowercase();

        match input_a1.as_str() 
        {
            "e" => function::change_mic_monitoring(&device, true),
            "d" => function::change_mic_monitoring(&device, false),
            _ => println!("Error! Undefined operation!")
        }
    }
    else if input_a == "t"
    {
        print!("Never / 10 / 20 / 30 minutes? [N/10/20/30] ");
        io::stdout().flush().unwrap();

        let mut input_a2: String = String::new();
        io::stdin()
            .read_line(&mut input_a2)
            .expect("Failed to read line");
        let input_a2 = input_a2.trim().to_lowercase();

        match input_a2.as_str() 
        {
            "n" => function::change_time_to_auto_shutdown(&device, VariablesTimeToAutoShutdown::Never as u8),
            "10" => function::change_time_to_auto_shutdown(&device, VariablesTimeToAutoShutdown::Min10 as u8),
            "20" => function::change_time_to_auto_shutdown(&device, VariablesTimeToAutoShutdown::Min20 as u8),
            "30" => function::change_time_to_auto_shutdown(&device, VariablesTimeToAutoShutdown::Min30 as u8),
            _ => println!("Error! Undefined operation!")
        }
    }
    
    println!("=========================================");
    println!("Listening HyperX HID...");

    loop 
    {
        let n = device.read_timeout(&mut buf, 1000)?;
        if n == 0 
        {
            continue;
        }

        if buf[0] == CommandLevel0::VolumeBaseCommandOfHeadset as u8 
        {
            match buf[1] {
                x if x == CommandLevel1::VolumeUp as u8 => 
                {
                    println!("Volume UP");
                }
                x if x == CommandLevel1::VolumeDown as u8 => 
                {
                    println!("Volume DOWN");
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
                        0x01 => println!("Microphone MUTED"),
                        0x00 => println!("Microphone UNMUTED"),
                        _ => {}
                    }
                },
                x if x == CommandLevel1::Power as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => println!("Headset turned OFF"),
                        0x01 => println!("Headset turned ON"),
                        _ => {}
                    }
                },
                x if x == CommandLevel1::MicConnection as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => println!("Microphone disconnected"),
                        0x01 => println!("Microphone connected"),
                        _ => {}
                    }
                },
                x if x == CommandLevel1::DeviceConnection as u8 => 
                {
                    match buf[2] 
                    {
                        0x00 => println!("Device connected to this PC"),
                        0x01 => println!("Device disconnected from this PC"),
                        _ => {}
                    }
                },
                _ => {}
            }
        }
    }
}
