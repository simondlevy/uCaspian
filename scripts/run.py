#!/usr/bin/env python3
import neuro
import caspian

proc = caspian.Processor({"Backend": "uCaspian_USB"})

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
