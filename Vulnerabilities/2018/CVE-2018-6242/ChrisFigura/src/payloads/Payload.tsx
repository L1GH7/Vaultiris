/**
 * The location in memory of the RCM payload.
 */
const RCM_PAYLOAD_ADDRESS = 0x40010000;

/**
 * The location in memory of the intermezzo.
 */
const INTERMEZZO_ADDRESS = 0x4001F000;

/**
 * Code to move the payload to the RCM load address.
 */
const intermezzo = new Uint8Array([
  0x44, 0x00, 0x9F, 0xE5, 0x01, 0x11, 0xA0, 0xE3, 0x40, 0x20, 0x9F, 0xE5, 0x00,
  0x20, 0x42, 0xE0, 0x08, 0x00, 0x00, 0xEB, 0x01, 0x01, 0xA0, 0xE3, 0x10, 0xFF,
  0x2F, 0xE1, 0x00, 0x00, 0xA0, 0xE1, 0x2C, 0x00, 0x9F, 0xE5, 0x2C, 0x10, 0x9F,
  0xE5, 0x02, 0x28, 0xA0, 0xE3, 0x01, 0x00, 0x00, 0xEB, 0x20, 0x00, 0x9F, 0xE5,
  0x10, 0xFF, 0x2F, 0xE1, 0x04, 0x30, 0x90, 0xE4, 0x04, 0x30, 0x81, 0xE4, 0x04,
  0x20, 0x52, 0xE2, 0xFB, 0xFF, 0xFF, 0x1A, 0x1E, 0xFF, 0x2F, 0xE1, 0x20, 0xF0,
  0x01, 0x40, 0x5C, 0xF0, 0x01, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01,
  0x40
]);

/**
 * Represents a payload for a Tegra based device.
 */
export default class Payload {
  /**
   * The raw data for the payload.
   */
  private readonly payload: Uint8Array;

  /**
   * Creates a new payload object using the provided array of bytes.
   * 
   * @param payload 
   */
  private constructor(payload: Uint8Array) {
    this.payload = payload;
  }

  /**
   * Gets the raw bytes for the payload.
   * 
   * @return A Uint8Array storing the bytes for this payload.
   */
  public getBytes(): Uint8Array {
    return this.payload;
  }

  /**
   * Reads a payload file into a Uint8Array.
   * 
   * @param file The payload file
   */
  private static readFile(file: Blob): Promise<ArrayBuffer> {
    // Return a promise, so we can block until the file is loaded.
    return new Promise((res, rej) => {
      const reader = new FileReader();
      reader.addEventListener('load', 
          e => res(e.target!.result as ArrayBuffer));
      reader.readAsArrayBuffer(file);
    })
  }

  /**
   * Prepare a payload to be sent to a Tegra X1 device.
   * 
   * @param payload The payload to be prepared.
   * @return The prepared Payload.
   */
  public static async preparePayload(payload: Uint8Array | Blob):
      Promise<Payload> {
    if (payload instanceof Blob) {
      // If we're passed a file, convert it into a Uint8Array.
      payload = new Uint8Array(await this.readFile(payload));
    } else {
      // Otherwise, cast it to only be a Uint8Array.
      payload = payload as Uint8Array;
    }

    // TODO: Document
    const rcmLength = 0x30298;

    // The number of times to write the intermezzo location to the stack.
    const intermezzoAddressRepeatCount =
      (INTERMEZZO_ADDRESS - RCM_PAYLOAD_ADDRESS) / 4;

    // The size of the entire payload.
    const finalPayloadSize = Math.ceil(
      (0x2A8 + (0x4 * intermezzoAddressRepeatCount) + 0x1000 +
        payload.byteLength) / 0x1000) * 0x1000;

    // Variables for storing the finalized payload.
    const finalPayload = new Uint8Array(new ArrayBuffer(finalPayloadSize));
    const finalPayloadView = new DataView(finalPayload.buffer);

    // TODO: Document
    finalPayloadView.setUint32(0x0, rcmLength, true);

    // Fill the execution stack with the address for the intermezzo, so when we
    // return to the stack, we immediately jump to the intermezzo.
    for (let i = 0; i < intermezzoAddressRepeatCount; i++) {
      finalPayloadView.setUint32(0x2A8 + i * 4, INTERMEZZO_ADDRESS, true);
    }

    // Put the intermezzo and the payload into the finalized payload.
    finalPayload.set(intermezzo, 0x2A8 + (0x4 * intermezzoAddressRepeatCount));
    finalPayload.set(payload, 0x2A8 + (0x4 * intermezzoAddressRepeatCount)
        + 0x1000);

    // Return the finalized payload.
    return new Payload(finalPayload);
  }
}
