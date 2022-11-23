# CVE-2020-25265 / CVE-2020-25266

- [CVE-2020-25265](https://nvd.nist.gov/vuln/detail/CVE-2020-25265)
- [CVE-2020-25266](https://nvd.nist.gov/vuln/detail/CVE-2020-25266)

This is a PoC exploit of libappimage and appimaged.

Using a combination of the two CVEs, one cancreate a file that does not appear to be an appimage
that is both implicitly picked up by appimaged and installed, overriding a system installed version
of the same application.

In this case, an MP3 file was used as the payload, and Nautilus was the application overridden.

## Affected versions

This affects all versions of appimaged and all libappimage versions older than 1.0.3.

## What happens

- The MP3 codec does not have a file header, thus media software must scan through the file looking for a frame header.
  This means that we can place the ELF and AppImage type 2 headers at the front of the file, and the MP3 will still be playable. This makes MP3 a prime target to use for this PoC.

- An MP3 file is modified to contain a payload binary after the playing media, with a Dart script being used to place the
  MP3 data in between the payload executable and the ELF header. (Note that I did not adjust the locations of symbols in
  the symbol table, as that would have taken more effort and was not needed for this PoC.) The payload binary is
  less than 1mb, thus it does not cause a noticeable increase in file size. At the very end of the file, a squashfs
  filesystem is appended, containing the fake Nautilus desktop file and icon.

  Note that, although adding the MP3 to the end of the squashfs would have technically worked, it has a higher chance of
  confusing media players due to appearing so late in the file, whereas right after the ELF header is only a few
  dozen bytes into the file.

- appimaged scans all files in all tracked directories, regardless of the file extension. When it picks up the MP3, it
  detects the ELF and AppImage headers, and proceeds to extract the desktop file within.

- The rogue desktop file is now written and points to the malicious MP3. When the user next runs Files, it will start the
  MP3's embedded ELF executable instead of the system installation.

- Normally, the desktop file would be named as "appimagekit_...desktop", thus it would not be able to entirely override
  any system applications. However, Integrator.cpp in libappimage would grab the Name field without validation or
  escaping, thus we can add this to the rogue desktop file:

  ```ini
  Name=/../org.gnome.Nautilus
  Name[en]=Files
  ```

  and the file will be written to ~/.local/share/applications/org.gnome.Nautilus.desktop, thus masking the system-installed
  Nautilus.

## Mitigation

Make sure your installed libappimage version is at least 1.0.3, which fixes the `Name` field validation.
Notably, this does *not* include Debian's libappimage at this time, which is quite out of date.

All versions of appimaged are vulnerable, and [the project has been deprecated](
https://github.com/AppImage/appimaged/commit/eeb739ef5ca05aa5faf6eec544d85c2df92e73e3).
[go-appimage](https://github.com/probonopd/go-appimage) avoids the filename issue by only picking up appimages named
`*.AppImage`. [AppImageLauncher](https://github.com/TheAssassin/AppImageLauncher) avoids the filename issue by
asking the user for permission to install before installation takes place.

## Testing

Place a random audio file in `audio.mp3` and run `dart bin/write_mp3.dart` to build the payload and write the
modified MP3 file. You need the Dart SDK and FreePascal installed for this to work.

## Implementation notes

Pascal was used for te binary because it produced the smallest statically linked ones of the other
languages tested (C statically linked using musl, Go) when using subprocess functionality.

In testing, `world.execute(me)` was used as the audio file, because it seemed fitting.
