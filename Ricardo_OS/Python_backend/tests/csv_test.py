import csv

with open('telemetry_log.txt',newline='') as csvfile:
    reader = csv.reader(csvfile)
    print(list(reader)[0])
    # for line in reader:
    #     print(len(line))

