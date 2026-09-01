#!/usr/bin/env python3
import neuro
import caspian

NETWORK_FILE = "../../networks/example.txt"

net = neuro.Network()

try:
    net.read_from_file(NETWORK_FILE)
except Exception:
    print('Unable to read from ' + NETWORK_FILE)
    exit(1)

proc = caspian.Processor({"Backend": "uCaspian_USB"})

proc.load_network(net)
