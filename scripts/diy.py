#!/usr/bin/env python3
import serial
from time import sleep

ser = serial.Serial('/dev/ttyUSB0', 3_000_000)

ser.write(b'\x04')

sleep(.125)

print(ser.in_waiting)

print(ser.read(1))


'''
Preparing to send clear config... < Async write of 1 bytes -- offset: 0 -- total: 1
  x05 = 5
Enter parse_cmds -- buf size: 1
 > Clear Ack 1
[TIME: 0] Processed 1 bytes: [ x04 ]  - 0 leftover
 Clear ack'd
Send config for 3 elements with 19 bytes
 < Async write of 19 bytes -- offset: 0 -- total: 19
  x08 = 8
  x00 = 0
  x0A = 10
  xF8 = 248
  x00 = 0
  x00 = 0
  x01 = 1
  x10 = 16
  x00 = 0
  x00 = 0
  x0B = 11
  x01 = 1
  x08 = 8
  x01 = 1
  x0A = 10
  x08 = 8
  x00 = 0
  x01 = 1
  x00 = 0
Enter parse_cmds -- buf size: 3
 > Config Ack 1
 > Config Ack 2
 > Config Ack 3
[TIME: 0] Processed 3 bytes: [ x18 x18 x18 ]  - 0 leftover
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> clear_activity: dev=0x5ae0d6296df0
+++++++++++++++++++++++ clear_activity()
 < Async write of 1 bytes -- offset: 0 -- total: 1
  x04 = 4
Enter parse_cmds -- buf size: 1
 > Clear Ack 1
[TIME: 0] Processed 1 bytes: [ x04 ]  - 0 leftover
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> track_output_events
+++++++++++++++++++++++ track_timing()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> track_output_events
+++++++++++++++++++++++ track_timing()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> apply_spike
+++++++++++++++++++++++ apply_input()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> apply_spike
+++++++++++++++++++++++ apply_input()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> apply_spike
+++++++++++++++++++++++ apply_input()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> apply_spike
+++++++++++++++++++++++ apply_input()
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> run
+++++++++++++++++++++++ simulate()
[t=  0] FIRE   0:255
 > STEP 5
[t=  5] FIRE   0:255
 > STEP 5
[t= 10] FIRE   0:255
 > STEP 10
[t= 20] FIRE   0:255
 > STEP 30
 < Async write of 16 bytes -- offset: 0 -- total: 16
  x80 = 128
  xFF = 255
  x01 = 1
  x05 = 5
  x80 = 128
  xFF = 255
  x01 = 1
  x05 = 5
  x80 = 128
  xFF = 255
  x01 = 1
  x0A = 10
  x80 = 128
  xFF = 255
  x01 = 1
  x1E = 30
Enter parse_cmds -- buf size: 76
 > Time Update: 1
 > Fire 0 [t=1]
 > Time Update: 5
 > Time Update: 6
 > Fire 0 [t=6]
 > Time Update: 10
 > Time Update: 11
 > Fire 0 [t=11]
 > Time Update: 17
 > Fire 1 [t=17]
 > Time Update: 20
 > Time Update: 21
 > Fire 0 [t=21]
 > Time Update: 22
 > Fire 1 [t=22]
 > Time Update: 27
 > Fire 1 [t=27]
 > Time Update: 37
 > Fire 1 [t=37]
 > Time Update: 50
[TIME: 50] Processed 76 bytes: [ x01 x00 x00 x00 x01 x80 x00 x01 x00 x00 x00 x05 x01 x00 x00 x00 x06 x80 x00 x01 x00 x00 x00 x0A x01 x00 x00 x00 x0B x80 x00 x01 x00 x00 x00 x11 x80 x01 x01 x00 x00 x00 x14 x01 x00 x00 x00 x15 x80 x00 x01 x00 x00 x00 x16 x80 x01 x01 x00 x00 x00 x1B x80 x01 x01 x00 x00 x00 x25 x80 x01 x01 x00 x00 x00 x32 ]  - 0 leftover
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> output_vector
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> output_vector
'''
