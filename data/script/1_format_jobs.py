import header_footer as hf
import os
import subprocess

# Define the constants
MODE = hf.mode
TARGET_FILE = hf.format_file
SCRIPT = 'format_data.py'
RUN = 'python3'

# Define the path to search
search_path = os.path.join('..', '..', MODE, hf.run_folder)

# Walk through the directory tree
for root, dirs, files in os.walk(search_path):
    for file in files:
        if file == TARGET_FILE:
            file_path = os.path.join(root, file)
            # Check if the file is indeed a file and not a symbolic link, etc.
            if os.path.isfile(file_path):
                # Run the script on the found file
                subprocess.run([RUN, SCRIPT, file_path])
