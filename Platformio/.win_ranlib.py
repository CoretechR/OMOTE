# Make sure that the windows flirc archives have an index
# if they don't mingw can't link against them
# This script will add an index to the archives and create a .stamp file to indicate it was processed

import os
import subprocess

Import("env")

script_dir = env['PROJECT_DIR']

def update_archive_and_stamp(archive_path):
    # Determine the directory and stamp file's path
    directory = os.path.dirname(archive_path)
    stamp_file = os.path.join(directory, ".stamp")

    # Check if the stamp file already exists
    if os.path.exists(stamp_file):
        print(f"Skipping {archive_path} as it is already processed.")
        return
    
    # Run the 'ar s' command to add an index to the archive
    try:
        subprocess.run(['ar', 's', archive_path], check=True)
        print(f"Indexed {archive_path} successfully.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred while indexing {archive_path}: {e}")
        return

    # Create a .stamp file to indicate it was processed
    open(stamp_file, 'w').close()
    print(f"Created stamp file: {stamp_file}")

archives = [
    os.path.join(script_dir, "hardware/windows_linux/lib/flirc_sdk/libs/Win/libflirc.a"),
    os.path.join(script_dir, "hardware/windows_linux/lib/flirc_sdk/libs/Win64/libflirc.a")
]

for archive in archives:
    update_archive_and_stamp(archive)