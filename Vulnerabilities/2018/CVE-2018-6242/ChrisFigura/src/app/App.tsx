import './App.css';
import React, { ChangeEvent } from 'react';
import DropdownButton from 'react-bootstrap/DropdownButton';
import Dropdown from 'react-bootstrap/Dropdown';
import Button from 'react-bootstrap/Button';
import NintendoSwitch from '../devices/NintendoSwitch';
import Payload from '../payloads/Payload';

/**
 * Variable for storing the currently loaded Payload.
 */
let payload: Payload;

/**
 * Function for updating the UI. Defined within the App function so it can use 
 * React's state functions.
 */
let updateDisplay: Function;

/**
 * Function for creating the main site.
 * 
 * @returns A div with the main content of the site.
 */
export default function App() {
  // Create functions for manipulating and retrieving the data on the app.
  const payloadUploader = React.useRef<HTMLInputElement>(null);
  const [launchButtonDisabled, disableLaunchButton] = React.useState(true);
  const [dropdownTitle, setDropdownTitle] = React.useState("Select a payload");

  /**
   * Updates the display to show that the specified payload is selected.
   * 
   * @param name The name of the selected payload.
   */
  updateDisplay = (name: string) => {
    disableLaunchButton(false);
    setDropdownTitle(name);
  }

  // Return the div containing the app.
  return (
    <div className="App">
      <header>
        <input
          ref={payloadUploader}
          type="file"
          accept="*/*"
          onChange={uploadPayload}
          style={{
            display: "none"
          }}
        />

        <p>CVE-2018-6242 Payload Launcher</p>

        <DropdownButton id="payloadSelector" title={dropdownTitle} variant="secondary" className="PayloadOptions">
          <Dropdown.Item onClick={prepareHekate}>Hekate v5.2.1</Dropdown.Item>
          <Dropdown.Item onClick={prepareOldHekate}>Hekate v5.0.1</Dropdown.Item>
          <Dropdown.Divider></Dropdown.Divider>
          <Dropdown.Item onClick={() => payloadUploader.current!.click()}>Upload Payload</Dropdown.Item>
        </DropdownButton>

        <Button onClick={launchPayload} className="PayloadOptions" variant="primary" disabled={launchButtonDisabled}>Launch Payload</Button>
      </header>
    </div>
  );
}

/**
 * TODO: Document
 */
async function prepareHekate() {
  // Fetch the Hekate binary.
  const hekate = await fetch("/static/payloads/hekate_ctcaer_5.2.1.bin");
  
  // Prepare the payload.
  payload = await Payload.preparePayload(await hekate.blob());

  // Update the display with the selected Payload.
  updateDisplay("Hekate v5.2.1");
}

/**
 * TODO: Document
 */
async function prepareOldHekate() {
  // Fetch the old Hekate binary.
  const hekate = await fetch("/static/payloads/hekate_ctcaer_5.0.1.bin");

  // Prepare the payload.
  payload = await Payload.preparePayload(await hekate.blob());

  // Update the display with the selected Payload.
  updateDisplay("Hekate v5.0.1");
}

/**
 * TODO: Document
 * 
 * @param e The event that triggered this function. Contains the uploaded
 *          payload.
 */
async function uploadPayload(e: ChangeEvent<HTMLInputElement>) {
  // TODO: If someone clicks another option, and then clicks this again and 
  // uploads the same file, this will never be called and nothing will happen.
  // I can probably fix this by adding a function which clicks the file 
  // uploader, and then updates the title regardless of whether there was an 
  // update. I should be careful of if the first time someone tries to upload a
  // file, they don't actually select a file at all. Maybe I can make it so the 
  // other options clear out the uploaded payload. This might actually be the 
  // best option.

  // Get the uploaded payload.
  const uploadedFile = e.target.files![0];

  // Make sure it's not null.
  if (uploadedFile === null) {
    throw new Error("Uploaded file was null!");
  }

  // Prepare the payload.
  payload = await Payload.preparePayload(uploadedFile);

  // Update the display with the selected Payload.
  updateDisplay(uploadedFile.name);
}

/**
 * TODO: Document
 * 
 * @param payload The payload to be launched.
 */
async function launchPayload() {
  // Request the device from the user.
  let device = await NintendoSwitch.requestDevice();

  // Send the payload to the device.
  device.launch(payload);
}
