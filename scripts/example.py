#!/usr/bin/env python3
import neuro
import caspian

NETWORK_FILE = '../networks/example.txt'

net = neuro.Network()

try:
    net.read_from_file(NETWORK_FILE)
except Exception:
    print('Unable to read from ' + NETWORK_FILE)
    exit(1)

proc = caspian.Processor({'Backend': 'uCaspian_USB'})

proc.load_network(net)

print('======================================================================')

proc.clear_activity()

proc.track_output_events(0)
proc.track_output_events(1)

# Inputs
proc.apply_spike(neuro.Spike(time=0, id=0, value=1))
proc.apply_spike(neuro.Spike(time=5, id=0, value=1))
proc.apply_spike(neuro.Spike(time=10, id=0, value=1))
proc.apply_spike(neuro.Spike(time=20, id=0, value=1))

proc.run(50)

print(0, proc.output_vector(0))
print(1, proc.output_vector(1))
