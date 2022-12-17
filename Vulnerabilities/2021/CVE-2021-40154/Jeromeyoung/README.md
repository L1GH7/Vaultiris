# [CVE-2021-40154](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40154)


## LPC55S69 and K82 USB In-System Programming (ISP) Buffer Over Read Vulnerabilities

POC to test the BootROM vulnerability found in LPC55S69 and K82 Series Revision A2. The vulnerability is fixed in newer devices with newe Revision A3 for LPC55S69. 

**Vulnerable Devices:**

**Confirmed:** LPC55S69 and K82 series MCU’s

**Suspected:** other NXP series MCU’s with similar USB ISP stack - Kinetis K Series and RT Series

### Vulnerability Summary:

A Buffer over read vulnerability was observed in the USB In-system programming (ISP) mode of NXP LPC55s69 and Kinetis K82 Microcontroller series. When exploited, the vulnerability allows an attacker to read 16KB (LPC55S69) and 64KB (K82) of data from protected flash memory. 

### Technical Details:

NXP LPC55S69 and K82 MCUs provide In-System Programming (ISP) modes for field update of the MCU’s firmware using peripherals such as USB, SPI, UART, and I2C. The USP ISP mode provides an embedded host to update firmware through NXP’s documented update commands.

The ISP is part of the LPC55S69 and K82 BootROM, and gets triggered depending on the values of certain register bits, ISP pin, and image header definition. For LPC55S69, the USB HID class is used to download an image of the USB0/1 port and, similarly, for K82 the Kinetis Bootloader in the ROM supports loading data into flash via the USB peripheral as a USB HID class.

Both the LPC55S69 and K82 series IC’s USB ISP use three (3) endpoints, which are listed below:
- Control (0)
- Interrupt IN (1)
- Interrupt OUT (2)
    
The buffer over read vulnerability was observed for the USB Control endpoint 0, which is used for USB host enumeration.

### Proof of Concept

**Target 1: LPC55S69**

The buffer over read vulnerability is observed at the GET Descriptor Configuration request from the embedded USB host. An attacker can request up to 16KB of response data, when crafting an USB request to send GET Descriptor Configuration, by modifying the “wlength” value to 65535 bytes. LPC556S9 limits, and resets, when 16KB of data is copied to the USB TX buffer and successfully transmits to the host. 

__Device Details:__

Target Dev Board: LPC55S69 – EVK RevA2
USB Host: Ubuntu 20.04 or Raspberry-pi 4 Model B


__Steps to reproduce the vulnerability:__
1. In LPC55S69 – EVK RevA2 Development board the USB ISP is triggered by pressing the ISP button and reset button.
2. Connect the USB 1 High speed port to an Ubuntu Laptop or Raspberry-pi.
3. The LPC board gets detected as USB Composite device as documented by Figure 1.

Figure 1:  Ubuntu dmesg utility showing USB  device details
```
[43295.318228] usb 1-1.4: USB disconnect, device number 95
[43296.175536] usb 1-1.4: new full-speed USB device number 96 using xhci_hcd
[43296.397315] usb 1-1.4: New USB device found, idVendor=1fc9, idProduct=0021, bcdDevice= 3.00
[43296.397331] usb 1-1.4: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[43296.397337] usb 1-1.4: Product: USB COMPOSITE DEVICE
[43296.397342] usb 1-1.4: Manufacturer: NXP SEMICONDUCTOR INC.
[43296.400768] hid-generic 0003:1FC9:0021.0055: hiddev0,hidraw4: USB HID v1.00 Device [NXP SEMICONDUCTOR INC. USB COMPOSITE DEVICE] on usb-0000:00:14.0-1.4/input0
```

4. Connect an USB analyser in between the target board and PC, or use the Linux kernel module “usbmon” to capture USB packets:
- If using USBmon in Ubuntu, run this command “sudo modprobe usbmon”.
- While using “lsusb”, note the bus and device address.
- Run “sudo wireshark”, connect to the USBMON1/2 interface with reference to the bus number from “lsusb”.

5. In Wireshark, filter the USB device address with reference to the bus number from “lsusb”.
6. Run the Proof of Concept (PoC) “sudo python3 LPC_USB.py” in Ubuntu. In order to successfully execute Python script, below requirements are needed in the USB host PC:
        a. Install Python 3+ 
        b. Install the “pyusb” module
        
``` 
sudo python3 LPC_USB.py 
```

7. Verify the buffer overflow by checking the request and response from the USB traces, as documented by Figure 2; Figure 3 shows, instead, the USB analyser trace. The USB response would be 4KB.

![Figure 2](https://github.com/Xen1thLabs-AE/CVE-2021-40154/blob/main/wireshark.png)

![Figure 3](https://github.com/Xen1thLabs-AE/CVE-2021-40154/blob/main/usb_trace.png)

__Note 1:__ The buffer over read of 4KB can be requested in Ubuntu system due to the restriction of Libusb “MAX_CTRL_BUFFER_LENGTH”. This can be modified in a Raspberry-pi system and 16KB can be requested. Attached firmware “LPC.bin” retrieved from the LPC55s69-EVK board using a raspberry-pi 4 model b. 

If you would like to issue requests greater than 4KB, use this awesome script created by [Horac](https://github.com/h0rac/usb-tester/blob/main/prereq-pi.sh) on a Raspi and run the POC with wlength changed to 65535(0xffff).

**Target 2: Kinetis K82**

The buffer over read vulnerability is observed at the GET Status-Other request from an embedded USB host. An attacker can request up to 64KB of response data, when crafting an USB request to send GET status-Other, by modifying the “wlength” value to 65535 bytes. The K82 USB device successfully sends 64KB of data.  

__Device Details:__

Target Dev Board: FRDM-K82F
USB Host: Ubuntu Laptop or Raspberry-pi

``` 
sudo python3 K82_USB.py 
```
__Note 1:__ The buffer over read of 4KB can be requested in Ubuntu system due to the restriction of Libusb “MAX_CTRL_BUFFER_LENGTH”. This can be modified in a Raspberry-pi and 64KB can be requested.

__Note 2:__ the vulnerability in K82 is reproduced only when the USB requests for GET status-Device, Interface, Endpoint, and other has to happen subsequently in order to trigger the vulnerability. This can be achieved by running the developed PoC scripts after multiple ISP resets.


If you would like to issue requests greater than 4KB, use this awesome script created by [Horac](https://github.com/h0rac/usb-tester/blob/main/prereq-pi.sh) on a Raspi and run the POC with wlength changed to 65535(0xffff).
