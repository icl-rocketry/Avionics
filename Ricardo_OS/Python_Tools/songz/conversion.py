from mido import MidiFile
from mido import tick2second
from mido import bpm2tempo

midi_file = MidiFile("OwlCity-Fireflies.mid",clip=True)

name = "Fireflies"
file_header = '''#pragma once \n#include "melodyClass.h"\n //GENERATED SONG FILE \n \n \n'''

notes = []
duration = []

#print(midi_file)

#    print(track)
#for track in midi_file.tracks:


track = midi_file.tracks[1]
print(track)

for msg in track:
    if msg.type == 'note_on':
        frequency = (2**((msg.note-69)/12))*440 #conversion to hz
        notes.append(round(frequency))
        duration.append(round( tick2second(msg.time,midi_file.ticks_per_beat,bpm2tempo(120))*(10**3) )) #conversion to ms
        print(msg)
        
print(midi_file.ticks_per_beat)
print(len(notes))

output_string = "melody_t<"+str(len(notes))+"> " + name + " = { { { \n"

for idx in range(len(notes)):
    if idx == len(notes) - 1 :
        output_string += "{"+str(notes[idx])+","+str(duration[idx])+"}\n} }, \ntrue \n };"
    else:
        output_string += "{"+str(notes[idx])+","+str(duration[idx])+"},\n"


output_file = open(name + ".h","w")

output_file.write(file_header)
output_file.write(output_string)

output_file.close()