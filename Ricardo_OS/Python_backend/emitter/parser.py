# Module (with interactive interface)


# MODULES
import csv
import copy


# UTILS
# Custom Errors
class FileTypeError(Exception):
    pass

# Handlers
def filename_handler(filename):
    name_array = filename.split('.')
    if (len(name_array) == 1):
        return filename + '.csv'
    else:
        if (name_array[1] != 'csv'):
            raise FileTypeError('Given file is not a .csv file')
        else:
            return filename

def row_counter(reader):
    ans = 0
    for row in reader:
        ans += 1
    return ans

def interval_setter(data_array):
    for index in range(len(data_array) - 1):
        data_array[index]["interval"] = int(data_array[index+1]["data"]["system_time"]) - \
            int(data_array[index]["data"]["system_time"])


# DUTY FUNCTION
def parse_log(filename):
    # opening file
    try:
        filename = filename_handler(filename)
        filepath = "./data/" + filename
        target = open(filepath, "r")
    except FileTypeError:
        print("FileTypeError: Specified file is not a .csv file.")
        exit(1)
    except FileNotFoundError:
        print("FileNotFoundError: File with name " + filename + " not found.")
        exit(1)

    # reading csv
    reader = csv.reader(target)
    
    column_name = []
    parsed_data = []

    row_num = 0
    row_object = { "index": 0, "interval": 0, "data": {} }
    for row_content in reader:
        if (row_num == 0):
            column_name = row_content

        else:
            row_object["index"] = row_num - 1

            column_num = 0
            for column in column_name:
                row_object["data"][column] = row_content[column_num]
                column_num += 1
            
            parsed_data.append(copy.deepcopy(row_object))

        row_num += 1

    # calculate interval from log data
    interval_setter(parsed_data)
    
    return parsed_data


# MAIN
def main():
    print("Please enter csv file name: ", end = "")
    filename = input()

    print("Parsing " + filename + " data onto system memory...")
    parsed_data = parse_log(filename)
    print("Parsing done.")

    index = 0
    while True:
        print("Enter index number (max: " + str(len(parsed_data) - 1) + \
            ") or -1 to exit: ", end = "")
        try:
            index = int(input())
            if (index == -1): break
            print(parsed_data[index]["data"])
        except ValueError:
            print("Index value invalid.")
            continue
        except IndexError:
            print("Index value out of bounds.")
            continue
    
    print("Process completed.")


if __name__ == "__main__":
    main()