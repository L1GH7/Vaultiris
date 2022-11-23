import 'dart:io';
import 'dart:typed_data';

final appImageSignature = Uint8List.fromList([0x41, 0x49, 0x02]);

final elf64EiDataOffset = 0x05;
final elf64EiPadOffset = 0x08;
final elf64EntryOffset = 0x18;
final elf64PhOffOffset = 0x20;
final elf64ShOffOffset = 0x28;
final elf64HSizeOffset = 0x34;
final elf64PhEntSizeOffset = 0x36;
final elf64PhNumOffset = 0x38;
final elf64ShEntSizeOffset = 0x3A;
final elf64ShNumOffset = 0x3C;

final ph64POffset = 0x08;
final sh64SOffset = 0x18;

final maxElfAlign = 1 << 12;

Future<void> runProcess(List<String> cmd) async {
  var p = await Process.start(cmd[0], cmd.sublist(1));
  stdout.addStream(p.stdout);
  stderr.addStream(p.stderr);

  var rc = await p.exitCode;
  if (rc != 0) {
    print('${cmd[0]} failed');
    exit(1);
  }
}

Future<void> main() async {
  await runProcess(['fpc', 'payload.pas']);

  var payload = ByteData.view((await File('payload').readAsBytes()).buffer);
  var mp3 = await File('audio.mp3').readAsBytes();

  var movement =
      mp3.lengthInBytes + (maxElfAlign - mp3.lengthInBytes % maxElfAlign);

  Endian endian =
      payload.getUint8(elf64EiDataOffset) == 1 ? Endian.little : Endian.big;

  var ehSize = payload.getUint16(elf64HSizeOffset, endian);
  var newHeader =
      ByteData.view(payload.buffer.asUint8List().sublist(0, ehSize).buffer);
  var newElfContents =
      ByteData.view(payload.buffer.asUint8List().sublist(ehSize).buffer);

  for (var i = 0; i < appImageSignature.length; i++) {
    newHeader.setUint8(elf64EiPadOffset + i, appImageSignature[i]);
  }

  var offsetsToMove = [elf64ShOffOffset, elf64PhOffOffset];
  for (var offset in offsetsToMove) {
    newHeader.setUint64(
        offset, payload.getUint64(offset, endian) + movement, endian);
  }

  var entsToMove = [
    [elf64ShOffOffset, elf64ShEntSizeOffset, elf64ShNumOffset, sh64SOffset],
    [elf64PhOffOffset, elf64PhEntSizeOffset, elf64PhNumOffset, ph64POffset],
  ];
  for (var ents in entsToMove) {
    var entoff = payload.getUint64(ents[0], endian);
    var size = payload.getUint16(ents[1], endian);
    var n = payload.getUint16(ents[2], endian);
    var segoff = ents[3];

    for (var i = 0; i < n; i++) {
      var elfPos = entoff + i * size;
      var targetPos = elfPos - ehSize;
      if (payload.getUint32(elfPos, endian) == 0x6474e551) {
        // PT_GNU_STACK
        continue;
      }

      newElfContents.setUint64(targetPos + segoff,
          payload.getUint64(elfPos + segoff, endian) + movement, endian);
    }
  }

  var appdir = Directory('appdir');
  if (await appdir.exists()) {
    await appdir.delete(recursive: true);
  }
  await appdir.create(recursive: true);
  await Directory('${appdir.path}/usr/share/icons').create(recursive: true);

  var squashfs = File('appdir.sqsh');
  if (await squashfs.exists()) {
    await squashfs.delete();
  }

  await File('nautilus.desktop').copy('${appdir.path}/nautilus.desktop');
  await File('nautilus.svg')
      .copy('${appdir.path}/usr/share/icons/nautilus.svg');

  await runProcess(['mksquashfs', appdir.path, squashfs.path]);

  var output = File('target.mp3');
  var sink = await output.openWrite();

  sink.add(newHeader.buffer.asUint8List());
  sink.add(mp3);
  sink.add(List<int>.filled(movement - mp3.lengthInBytes, 0));
  sink.add(newElfContents.buffer.asUint8List());
  sink.add(await squashfs.readAsBytes());

  sink.close();
}
