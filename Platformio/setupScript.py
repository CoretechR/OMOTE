# Magic import line found on https://docs.platformio.org/en/latest/scripting/actions.html
Import("env")
import SCons
import SCons.Environment
import subprocess

buildEnv : SCons.Environment.Base = env

LINUX_APT_DEPENDENCES = {"libsdl2-dev","libcurl4-openssl-dev", "libboost-all-dev"}

def runGitCommand(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate()

    if process.returncode != 0:
        print(f"Error executing command: {command}")
        print(stderr)
        return None
    return stdout

def EnsureSubmoduleCheckout():
  runGitCommand("git submodule update --init")

def isAptInstalled(apt):
    try:
        result = subprocess.run(['dpkg', '-s', apt], capture_output=True, text=True)
        return result.returncode == 0
    except:
        return False

def installApt(apt):
    try:
        subprocess.run(['sudo', 'apt-get', 'install', '-y', apt], check=True)
        print(f"Successfully installed {apt}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to install {apt}: {e}")

def verifySimDependencies():
    # Check if apt-get exists on the system
    try:
        subprocess.run(['which', 'apt-get'], check=True, capture_output=True)
        verifyAptDependencies()
    except subprocess.CalledProcessError:
        print("apt-get not found - skipping apt dependencies check")

def verifyAptDependencies():   
    try:
        for dep in LINUX_APT_DEPENDENCES:
          if(not isAptInstalled(dep)):
            print(f"Detected Missing Dependency {dep} Installing...")
            installApt(dep)
    except: 
       print("Failed To Verify Dependencies are Installed!")

def verifyDependencies():   
  verifySimDependencies()

def PrintInfo():
    print('')
    print("PIO Build env:", buildEnv["PIOENV"])
    print("Detected Platform:", buildEnv["PLATFORM"])
    print('')

def before_upload(source, target, env):
    print("before_upload")

def after_upload(source, target, env):
    print("after_upload")

# Custom actions when launching simulator
buildEnv.AddPreAction("upload", before_upload)
buildEnv.AddPostAction("upload", after_upload)


PrintInfo()
EnsureSubmoduleCheckout()
verifyDependencies()