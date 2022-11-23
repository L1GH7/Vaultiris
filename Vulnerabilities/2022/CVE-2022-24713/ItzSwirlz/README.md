# POC of CVE-2022-24713 on Ubuntu
Install the current rust-regex package on Ubuntu.

Then, clone this repo.

Then, run cargo build.

The regex dependency is set to the /usr/share/cargo/registry folder, so there
will be no dependency issues.

If the compilation takes an absurdly ridiculous long amount of time, it is
a denial of service and is CVE-2022-24713.

If it does not, it has been patched (https://github.com/rust-lang/regex/commit/ae70b41d4f46641dbc45c7a4f87954aea356283e)
