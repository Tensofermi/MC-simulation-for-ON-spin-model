import header_footer as hf
import argparse

def combine_files(file_list, output_filename):
    data = []
    headers_added = False
    headers = []

    for file in file_list:
        with open(file, 'r') as f:
            lines = f.readlines()
            if not headers_added:
                headers = [line for line in lines if line.strip().startswith("#")]
                headers_added = True
            data += [line for line in lines if not line.strip().startswith("#") and line.strip() != '']

    return headers, data

def sort_by_fields(lines, field_1, field_2):
    return sorted(lines, key=lambda line: (
        float(line.split()[field_1]),  
        float(line.split()[field_2])
    ))

def save_sorted_data(headers, sorted_data, output_file):
    with open(output_file, 'w') as f:
        f.writelines(headers)  # Write the headers first
        f.writelines(sorted_data)  # Then write the sorted data

def segregate_rows_by_entry(input_data, entry_index_1):
    rows_dict = {}
    last_key = None

    for line in input_data:
        stripped_line = line.strip()

        if stripped_line:
            entries = stripped_line.split('\t')
            combined_key = (entries[entry_index_1])

            if combined_key not in rows_dict:
                rows_dict[combined_key] = []
            rows_dict[combined_key].append(line)
            last_key = combined_key

        elif last_key:
            rows_dict[last_key].append(line)

    return rows_dict

def write_rows_to_files(headers, rows_dict):
    for (entry_1), rows in rows_dict.items():
        formatted_name = f"../raw/L_{int(entry_1)}.dat"

        with open(formatted_name, 'w') as outfile:
            for header in headers:
                outfile.write(header)
            outfile.writelines(rows)


if __name__ == "__main__":
    ##############################################
    L_index = hf.L_index
    beta_index = hf.beta_index
    ##############################################
    
    parser = argparse.ArgumentParser(description="Combine multiple data files.")
    parser.add_argument('input_files', type=str, nargs='+', help="Input files to be combined")

    args = parser.parse_args()

    headers, data = combine_files(args.input_files, "../raw/combined.dat")

    # Sort data
    sorted_data = sort_by_fields(data, L_index - 1, beta_index - 1)
    save_sorted_data(headers, sorted_data, "../raw/sorted.dat")

    # fix
    rows_dict = segregate_rows_by_entry(sorted_data, L_index - 1)

    # Write to files
    write_rows_to_files(headers, rows_dict)
