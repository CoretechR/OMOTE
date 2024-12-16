# Combine the macos ARM and x86 libraries into a fat library
# This makes it so we don't have to do more work on the build config to get the right platform
# It also helps work around an issue where the linker sees the dylib files in the flirc sdk folder
# and tries to link against and fails. This way we have a folder with only the static libraries
# The linker can handle that correctly.

import os
import subprocess

Import("env")

script_dir = env['PROJECT_DIR']

# Define paths
libs_folder = os.path.join(script_dir, 'hardware', 'windows_linux', 'lib', 'flirc_sdk', 'libs')
arm_path = os.path.join(script_dir, libs_folder, 'Darwin_arm64')
x86_path = os.path.join(script_dir, libs_folder, 'Darwin_x86_64')
universal_path = os.path.join(script_dir, libs_folder, 'Darwin_universal')

# Create directory if it doesn't exist
os.makedirs(universal_path, exist_ok=True)

# Define output files
libir_universal = os.path.join(universal_path, 'libir.a')
libflirc_universal = os.path.join(universal_path, 'libflirc.a')

# Check and create libir.a if it doesn't exist
if not os.path.exists(libir_universal):
    try:
        subprocess.run(['lipo', '-create', f'{arm_path}/libir.a', f'{x86_path}/libir.a', '-o', libir_universal], check=True)
        print("libir.a universal binary created successfully.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred while creating libir.a: {e}")
else:
    print("libir.a already exists. Skipping creation.")

# Check and create libflirc.a if it doesn't exist
if not os.path.exists(libflirc_universal):
    try:
        subprocess.run(['lipo', '-create', f'{arm_path}/libflirc.a', f'{x86_path}/libflirc.a', '-o', libflirc_universal], check=True)
        print("libflirc.a universal binary created successfully.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred while creating libflirc.a: {e}")
else:
    print("libflirc.a already exists. Skipping creation.")