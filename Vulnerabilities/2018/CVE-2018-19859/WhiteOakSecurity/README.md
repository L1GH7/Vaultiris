# CVE-2018-19859 - RCE Proof of Concept

This repository contains a proof of concept for Remote Code Execution (RCE) against OpenRefine < 3.1-beta. By exploiting a directory traversal vulnerability inside of the Create Project functionality, [CVE-2018-19859](https://github.com/OpenRefine/OpenRefine/issues/1840), a malicious user can upload a custom Java extension to gain code execution.

This proof of concept contains a simple Java Reverse Shell which is activated when a user navigates to `{webroot}/extension/whiteoak/`.

## Installation

1. Grab a local vulnerable version of OpenRefine such as [version 2.8](https://github.com/OpenRefine/OpenRefine/releases/tag/2.8).
2. Clone this extension repository into the `openrefine/extensions/` directory.
3. Modify the `build.xml` file in the extensions directory to add a reference to the new extension.
4. Compile the extension with `./refine clean && ./refine build`
5. Generate the malicious zip archive using `evilarc_whiteoak.py` to create a zip slip archive of an entire directory. Ensure the webroot path is provided:
```bash
python3 evilarc_whiteoak.py -d 14 -p "{webroot directory}/openrefine/webapp/extensions/" whiteoak/
```
6. Upload the extension using the vulnerability described in CVE-2018-19859 via the Create Project functionality & restart the OpenRefine webserver.
7. Navigate to `{webroot}/extension/whiteoak/` and catch your new shell.


## Credits
@itsacoderepo for the [CVE details](https://github.com/OpenRefine/OpenRefine/issues/1840) on GitHub.

@ptoomy3 for the original [Zip Slip archive generation tool](https://github.com/ptoomey3/evilarc), which White Oak Security updated to python3 and added support for archiving directories.
