# What

This is the code for the demonstration of the COVIDSafe exploit in
[![](http://img.youtube.com/vi/7UdVHB1ohNo/0.jpg)](http://www.youtube.com/watch?v=7UdVHB1ohNo "")

See https://medium.com/@wabz/covidsafe-ios-vulnerability-cve-2020-12717-30dc003f9708 for more information


## Server
This has to be run on linux. It uses @abandonware/bleno nodejs module (you need nodejs10), which trivially allows setting advertising bytes for the gatt server.

I run this on Linux Mint, in VirtualBox, adding a USB Bluetooth adapter to pass through. Install the requirements as per bleno's intructions, they are still current.

It runs an express server, with a very simple interface:

`http://0.0.0.0:3000/start` - starts advertising with exploit advertisement

`http://0.0.0.0:3000/stop` - stops advertising

## Client

A super simple Material Angular application. If you set up a Host-only Adapter for VirtualBox, you can connect to the server in the VM. Simply toggle to hit the APIs. See the site/README.md for details on how to run it. 
