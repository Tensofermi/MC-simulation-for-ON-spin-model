import header_footer as hf
import os
import glob

# Define the paths and files
MODE = hf.mode
TARGET_FILE = hf.formated_file
OUTPUT_FILE = '../raw/raw.dat'

# Combine and sort
# Ensure output file is empty or create it if it doesn't exist
with open(OUTPUT_FILE, 'w') as f:
    pass

# Use glob to locate all target files in subdirectories
search_path = f'../../{MODE}/jobs/**/{TARGET_FILE}'
files = glob.glob(search_path, recursive=True)

# Process each file found
first_file = True
for file in files:
    with open(file, 'r') as f:
        contents = f.readlines()
    
    if first_file:
        # For the first file, take both header lines and data
        with open(OUTPUT_FILE, 'a') as f:
            f.writelines(contents)
        first_file = False
    else:
        # For subsequent files, skip the header lines
        with open(OUTPUT_FILE, 'a') as f:
            f.writelines(contents[2:])