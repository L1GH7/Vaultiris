# CVE-2022-26269
Suzuki connect app is used to get the car information like Fuel, Ignition status, Current location, Seat buckle status etc. In Ignis, Zeta variant car if the Fuel CAN messages and Seat buckle status is spoofed via OBD 2 port with the crafted value (e.g. zero percent fuel and Car seat is buckled ), then the same value is reflected on Suzuki connect app, which can mislead the user.

Affected Product:
Suzuki Connect app version 1.0.15
Car Ignis Zeta variant, Manufacturing year 2019

Addition details on Suzuki connect URL:
https://www.marutisuzuki.com/corporate/technology/suzuki-connect

Detailed report

Required Setup:
1. Ignis Zeta variant ( Make 2019 ), Ensure Car is internet connection zone
2. Suzuki connect device with app version 1.0.15
3. USB to CAN Hardware ( OBD 2 port )
4. Software to receive and transmit CAN message

Following steps shall be followed to achieve the POC:
1. Turn On the car engine
2. Connect OBD 2 to USB connector
![image](https://user-images.githubusercontent.com/7817473/160451963-810f7931-18ea-4172-a5ed-fa1fb793c907.png)
3. Car fuel should not be empty
![Fuel Status](https://user-images.githubusercontent.com/7817473/160454630-6764ee6a-fee7-4adb-8300-13bc646ec183.jpeg)
4. Driver seat belts are not buckled.
5. Check Suzuki connect app shows Fuel and Buckle status
6. Now transmit following message with every 10 msec
- Fuel status 0% : Identifier - 3B9, Length - 6, Data - 00 00 00 00 00 00
- Seat Belt status : Buckled: Identifier - 3D1, Length - 8, Data - 00 00 00 00 00 81 94 9B

![Data Snap](https://user-images.githubusercontent.com/7817473/160454764-1931efbd-deb4-4107-b498-ea6515aa60d4.JPG)
7. Wait for a few mins and check the suzuki connect app that Seat status shows buckled and fuel status should be zero and Low fuel alert will be displayed in suzuki connect app.

![Seat buckle and fuel](https://user-images.githubusercontent.com/7817473/160454818-bfc8a649-103d-4964-8955-8eddf19f7cdc.jpeg)
![fuel warning](https://user-images.githubusercontent.com/7817473/160454855-e40dc9a3-cd80-4ca1-9ab8-9f715476d02d.jpeg)


Additional Note: Further analysis is not conducted, but multiple message can be spoofed on OBD 2 port
**Credits : Nikhil Bogam**
