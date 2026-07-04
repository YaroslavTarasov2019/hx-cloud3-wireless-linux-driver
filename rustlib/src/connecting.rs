use hidapi::{HidApi, HidDevice};

const HYPERX_VID: u16 = 0x03f0;
const HYPERX_PID: u16 = 0x05b7;

pub fn connect() -> Result<HidDevice, String>
{
    let api = match HidApi::new() 
    {
        Ok(api) => api,
        Err(e) => 
        {
            eprintln!("HID API error: {}", e);
            return Err("Failed to init HID API".to_string());
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
            return Err("Device access blocked".to_string());
        }
    };

    Ok(device)
}
