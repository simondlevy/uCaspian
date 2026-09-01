#!/usr/bin/env python3
import neuro
import caspian

NETWORK_FILE = "../networks/example.txt"

net = neuro.Network()

try:
    net.read_from_file(NETWORK_FILE)
except Exception:
    print('Unable to read from ' + NETWORK_FILE)
    exit(1)

p = caspian.Processor({"Backend": "uCaspian_USB"})

p.load_network(net)

p.track_output_events(0)
p.track_output_events(1)

# Inputs
p.apply_spike(neuro.Spike(time=0, id=0, value=1))
p.apply_spike(neuro.Spike(time=5, id=0, value=1))
p.apply_spike(neuro.Spike(time=10, id=0, value=1))
p.apply_spike(neuro.Spike(time=20, id=0, value=1))

p.run(50)

print(0, p.output_vector(0))
print(1, p.output_vector(1))
