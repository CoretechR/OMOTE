# Magic import line found on https://docs.platformio.org/en/latest/scripting/actions.html
Import("env")
import SCons
import SCons.Environment

buildEnv : SCons.Environment.Base = env

def PrintInfo():
    print('')
    print("PIO Build env:", buildEnv["PIOENV"])
    print("Detected Platform:", buildEnv["PLATFORM"])
    print('')
    # print('DUMPING:')
    # print(buildEnv.Dump())
    # print('')

def before_upload(source, target, env):
    print("before_upload")

def after_upload(source, target, env):
    print("after_upload")

def pre_build(source,target,env):
    print("pre MAIN build")

def post_build(source, targe, env):
    print("post MAIN BUILD")

relativeBuildDir = ".pio/build/" + buildEnv["PIOENV"]
simMain = f"{relativeBuildDir}/src/simMain.o"
print (simMain)
# Custom actions when building simulator
buildEnv.AddPreAction("program", pre_build )
buildEnv.AddPostAction("program", post_build)
# Custom actions when launching simulator
buildEnv.AddPreAction("upload", before_upload)
buildEnv.AddPostAction("upload", after_upload)


PrintInfo()