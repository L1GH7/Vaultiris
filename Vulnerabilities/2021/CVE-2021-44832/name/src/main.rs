use anyhow::Result;
use clap::{App, Arg};
use walkdir::WalkDir;
use zip::ZipArchive;

fn main() -> Result<()> {
    println!("Discovering Log4Shell vulnerability [CVE-2021-44832]");

    let matches = App::new("log4j-scan").version(env!("CARGO_PKG_VERSION")).setting(clap::AppSettings::ArgRequiredElseHelp).arg(Arg::with_name("PATH").required(true).help("Provide path to scan (e.g. C:\\)")).get_matches();
    println!("Starting discovery on {}\n", matches.value_of("PATH").unwrap());

    for entry in WalkDir::new(matches.value_of("PATH").unwrap()).follow_links(true).same_file_system(true).into_iter().filter_map(|e| e.ok()) {
        if entry.file_name() == "Logger.class" && entry.path().to_str().unwrap_or("").contains("log4j") {
            println!("Extracted vulnerable file found: {}", entry.path().display());
        } else if entry.file_name().to_str().unwrap_or("").ends_with(".jar") {
            if entry.file_name().to_str().unwrap_or("").contains("2.17.1") {
            } else {
                if let Ok(reader) = std::fs::File::open(entry.path()) {
                    if let Ok(mut zip) = ZipArchive::new(reader) {
                        for i in 0..zip.len() {
                            if let Ok(file) = zip.by_index(i) {
                                let name = file.name();
                                if name.ends_with("Logger.class") && name.contains("log4j") {
                                    println!("Vulnerable file found: {}", entry.path().display());
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    println!("\nDiscovery Complete.");
    Ok(())
}