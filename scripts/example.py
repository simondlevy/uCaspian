#!/usr/bin/env python3
import neuro
import caspian

NETWORK_FILE = "example.txt"

p = caspian.Processor({"Backend": "uCaspian_USB", "Verilator": {"Trace_File": "a.fst"}})

net = neuro.Network()

try:
    net.read_from_file(NETWORK_FILE)
except Exception:
    print('Unable to read from ' + NETWORK_FILE)
    exit(1)


prefix = 0
a = prefix
b = prefix + 1

p.load_network(net)


for i in range(prefix+2):
    p.track_output_events(i)

# Inputs
p.apply_spike(neuro.Spike(time=0, id=a, value=1))
p.apply_spike(neuro.Spike(time=5, id=a, value=1))
p.apply_spike(neuro.Spike(time=10, id=a, value=1))
p.apply_spike(neuro.Spike(time=20, id=a, value=1))

p.run(50)

for i in range(prefix, prefix+2):
    print(i, p.output_vector(i))
