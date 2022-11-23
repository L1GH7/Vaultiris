fn main() {
    cc::Build::new()
        .file("./src/exploit/exploit.c")
        .include("./src/exploit/include")
        .flag("-w")
        .flag("-static")
        .compile("exploit");
}
