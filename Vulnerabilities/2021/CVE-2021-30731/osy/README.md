# Webcam Viewer

Previews the connected webcam as a PoC of [CVE-2021-30731](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-30731).

Must be running macOS 11.3.1 or lower or macOS 10.15 before Catalina Security Update 2021-004.

## Building

1. Install build prerequisites: `brew install autoconf automake cmake libtool pkg-config`
2. Build dependencies: `./bootstrap.sh`
3. Open the Xcode project and build.

## Usage

Select a UVD compatible USB webcam from the list. If it does not appear, unplug and replug the device. Note that built in FaceTime HD cameras on MacBooks cannot be captured because of `UsbUserClientEntitlementRequired`.

Tested working with the LG UltraFine 5K built-in webcam.

## Issues

Since this is just a PoC, there's a few bugs that's not been resolved.

* Frame tearing is observed. Unsure if this is a libuvc issue or not.
* Stopping and re-starting a preview fails and the app freezes.
* Force quitting the app results in the USB device being un-able to be used by the system again. A un-plug and re-plug is required.
