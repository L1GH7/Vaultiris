# CVE-2020-14292: A bluetooth transport issue in COVIDSafe App
## Author: Alwen Tiu, The Australian National University
## Last updated: 2020-09-08

## Summary

In the [COVIDSafe](https://play.google.com/store/apps/details?id=au.gov.health.covidsafe&hl=en_AU) application through 1.0.21 for Android, unsafe use of the Bluetooth transport option in the GATT connection
allows attackers to trick the application into establishing a connection over Bluetooth BR/EDR transport, which reveals the public Bluetooth address of
the victim's phone without authorisation, bypassing the Bluetooth address randomisation protection in the user's phone.
This issue is tracked using [CVE-2020-14292.](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-14292) 

## Technical description
The cause of the issue is in the function [startWork](https://github.com/AU-COVIDSafe/mobile-android/commit/c16533add66e043f5f6e841df8921afff8593d04#diff-6786c947344b9413a1f626ee8edd8cd6L39), 
where a GATT connection is made using [connectGatt](https://developer.android.com/reference/android/bluetooth/BluetoothDevice#connectGatt(android.content.Context,%20boolean,%20android.bluetooth.BluetoothGattCallback)), but no transport option was specified. 

```java 
fun startWork(
    context: Context,
    gattCallback: StreetPassWorker.StreetPassGattCallback
) {
    gatt = device.connectGatt(context, false, gattCallback)
    if (gatt == null) {
       CentralLog.e(TAG, "Unable to connect to ${device.address}")
    }
}
```
This will cause Android to use the TRANSPORT_AUTO option for the transport in initiating the Bluetooth connection. It seems that, in the implementation in Android, this will default to BR/EDR if the peripheral it is connecting to is a dual mode device (supporting both BR/EDR and LE); this may be due to the following implementation of GATT client in Android, see for example, 
[this code](https://cs.android.com/android/platform/superproject/+/master:system/bt/btif/src/btif_gatt_client.cc;l=281;drc=47d6483316780d9fbbf9a554afb215983eaee7fe) (relevant code snippet included below):
```C
 // Determine transport
  if (transport_p != BT_TRANSPORT_AUTO) {
    transport = transport_p;
  } else {
    switch (device_type) {
      case BT_DEVICE_TYPE_BREDR:
        transport = BT_TRANSPORT_BR_EDR;
        break;

      case BT_DEVICE_TYPE_BLE:
        transport = BT_TRANSPORT_LE;
        break;

      case BT_DEVICE_TYPE_DUMO:
        if (transport_p == BT_TRANSPORT_LE)
          transport = BT_TRANSPORT_LE;
        else
          transport = BT_TRANSPORT_BR_EDR;
        break;
    }
```


Since the MAC address randomisation is not supported in BR/EDR, the central will reveal its identity address to the peripheral.  

 
To launch an attack, the attacker advertises a GATT server on a dual mode device (BR/EDR + LE), using the public address (this part is important, as otherwise the downgrade will not happen and the Android central will use LE transport). This will cause the COVIDSafe app to use the BR/EDR transport, instead of the LE transport, to connect to the peripheral, which then causes the public address of the phone to be revealed. This works for all the phones and android versions tested.  

 
For Android 6 and later, the fix is simple: simply specify the transport option explicitly.

```java 
  gatt = device.connectGatt(context, false, gattCallback, BluetoothDevice.TRANSPORT_LE)
````

However, this does not work for Android 5.1 (API Level 22), because that method is not exposed in the API. It is actually there but it is [hidden.](https://cs.android.com/android/platform/superproject/+/android-5.1.1_r38:frameworks/base/core/java/android/bluetooth/BluetoothDevice.java;l=1424;drc=2b8696e3a91194db0bfd876b8cc68843a7ccd080) 
So for Android 5.1, a possible fix involves using a reflection to call that hidden method. 
These fixes have been implemented in [COVIDSafe (Android) v1.0.48](https://github.com/AU-COVIDSafe/mobile-android/blob/afb55cce5d6d283194cff209be0219d04148f3db/app/src/main/java/au/gov/health/covidsafe/streetpass/Work.kt#L41).  

However, the fix for Android 5.1 may not work 100% of the time; my tests indicated there was still leakage of identity addresses. This could be due to the failure of the reflection technique at runtime; further investigations will be needed. 

## Security & privacy implication
The possession of the identity address of a phone would allow an attacker to track the phone when its bluetooth is turned on and it is within the bluetooth range of the attacker. Some bluetooth related vulnerabilties (e.g., CVE-2020-0022) rely on the knowledge of the identity address to mount further attacks, so there's a potential to chain this attack with others. See [this document](https://github.com/alwentiu/COVIDSafe-CVE-2020-12856) for some of the consequences of the attacker possessing the identity address.

## Recommendation
Users of the Android version of the COVIDSafe app are strongly recommended to update their app. For those running Android 5, it is recommended to turn off bluetooth when
in a situation where the bluetooth functionality is not required.

## Disclosure timeline
This issue was first reported to the Australian Government Digital Transformation Agency on June 2nd, 2020. The first fix was released on June 22nd, 2020, for Android 6 and above. This has been confirmed to work for Android 6 and above. But the issue still affected Android 5.1. The second fix, released on July 30th, 2020, addressed the remaining problem with Android 5.1. 

## Acknowledgment
Thanks to Jim Mussared for confirming this issue independently and for various discussions related to this issue.  