import argparse
import re


ap = argparse.ArgumentParser()
ap.add_argument("-f",'--file', required=True, help="filename", type=str,default = None)
args = vars(ap.parse_args())






with open(args['file']) as f:
    contents = f.read()

name = args['file'].split('.')[0]
file_header = '''#include "melodyLibrary.h" \n#include "melodyClass.h"\n //GENERATED SONG FILE \n \n \n'''


no_comments = re.sub(r"//.*", "", contents).replace("\r", "")

#tempo = int(re.findall(r"int tempo=(.*);",no_comments)[0])

melody_raw = re.findall(r"(int melody\[\] = {)(.*)(};)",no_comments,re.DOTALL)[0]
melody = melody_raw[1].replace('\n', ' ').replace('\r', '').replace('\t','').replace(' ','')
melody = melody.split(',')
print(melody)
notes = [item for item in melody if 'NOTE' in item or 'REST' in item]
note_time = [int(item) for item in melody if item not in notes and item is not '']
print(notes)
print(note_time)

#tempo is bpm 
#duration of notes is in milliseconds
#bpm / 60  = bps
#bpms = bps / 1000 
bpm = 35
bps = bpm/60
bpms = bps/1000
duration = []

for inv_beats in note_time:
    if inv_beats > 0:
        duration.append(round(1/(bpms*inv_beats)))
    elif inv_beats < 0:
        #dotted note
        duration.append(round( (1/(abs(inv_beats)*bpms)) + (1/(abs(inv_beats*2)*bpms)) ))

output_string = "melody_t<"+str(len(notes))+"> SONG::" + name + " = { { { \n"

for idx in range(len(notes)):
    if idx == len(notes) - 1 :
        output_string += "{"+str(notes[idx])+","+str(duration[idx])+"}\n} }, \ntrue \n };"
    else:
        output_string += "{"+str(notes[idx])+","+str(duration[idx])+"},\n"


output_file = open(name + ".cpp","w")

output_file.write(file_header)
output_file.write(output_string)

output_file.close()
    
