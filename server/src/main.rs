mod public_ip;

use public_ip::*;

pub static PORT: u16 = 1234;
pub static mut PASSWORD: [u8; 1024] = [0; 1024];
pub static mut SLICE_PASSWORD: &[u8] = unsafe { &PASSWORD }; // Don't trim_matches(char::from(0))
pub static mut DEFAULT_PASSWORD: &str = "test";

fn print_help() {
    println!(
        r#"Help:
{} [port] [password]
"#,
        std::env::args().nth(0).unwrap()
    );
}

fn set_password(str: &str) {
    unsafe {
        let mut i = 0;
        for c in str.bytes() {
            PASSWORD[i] = c;
            i += 1;
        }
        PASSWORD[i] = 0;
        SLICE_PASSWORD = &PASSWORD[..i];
    }
}

fn main() {
    set_password(unsafe { DEFAULT_PASSWORD });
    let port = match std::env::args().nth(1) {
        Some(str) => match str.as_str() {
            "-h" | "--help" => return print_help(),
            _ => match str.parse::<u16>() {
                Ok(port) => port,
                Err(_) => {
                    set_password(&str);
                    PORT
                }
            },
        },
        _ => PORT,
    };
    match std::env::args().nth(2) {
        Some(str) => {
            set_password(&str);
        }
        _ => {}
    }
    serv("0.0.0.0", port);
}

#[link(name = "Z:\\Github\\wxbot\\build\\Release\\wxbot", kind = "static")]
extern "C" {
    fn wx_send(text: *const u16);
    fn wx_audio();
    fn wx_video();
}

fn serv(host: &str, port: u16) {
    println!("server started on {}:{}", public_ip(), port);
    rouille::start_server(format!("{}:{}", host, port), move |request| {
        let err = || {
            println!("口令验证失败：{}", request.url());
            rouille::Response::empty_400()
        };
        if let Some(passwd) = request.get_param("passwd") {
            if passwd != String::from_utf8(unsafe { SLICE_PASSWORD.to_vec() }).unwrap() {
                return err();
            }
        } else {
            return err();
        }

        if let Some(_) = request.get_param("audio") {
            unsafe { wx_audio() };
            return rouille::Response::empty_204();
        }

        if let Some(_) = request.get_param("video") {
            unsafe { wx_video() };
            return rouille::Response::empty_204();
        }

        // URI解码
        let msg = request.get_param("msg").unwrap();
        let msg = form_urlencoded::parse(msg.as_bytes()).collect::<Vec<_>>();
        let (msg, _) = &msg[0];

        // Base64解码
        let msg = format!("{}", msg);
        let msg = msg.replace(" ", "+"); // 空格替换为加号

        let msg = base64::decode(msg).unwrap();
        let msg = String::from_utf8(msg).unwrap();
        println!("发送消息: {}", msg);
        unsafe {
            let msg = msg.encode_utf16();
            let mut msg = msg.collect::<Vec<u16>>();
            // 加上结束符L'\0'
            msg.push(0);
            msg.push(0);
            wx_send(msg.as_ptr());
        }

        rouille::Response::empty_204()
    });
}
