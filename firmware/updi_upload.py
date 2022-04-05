"""
Custom script for uploading and setting the fuses using SerialUPDI method.
(c) 2022 by P.Z.

"""
import serial
import time
from os.path import join

Import("env")

platform = env.PioPlatform()
board = env.BoardConfig()
build_core = board.get("build.core", "")
# mcu type
mcu = env.GetProjectOption("board").lower()
# upload port 
up_port = env.GetProjectOption("upload_port")
# upload port speed
baud =  env.GetProjectOption("upload_speed")

PYEXE = env.subst("$PYTHONEXE")

FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-megaavr")
if build_core != "arduino":
    FRAMEWORK_DIR = platform.get_package_dir(
        "framework-arduino-megaavr-%s" % build_core.lower())
# path to the programmer
PROG_PATH = join(FRAMEWORK_DIR, "tools", "prog.py")
# prog.py command
CMD =  f'{PYEXE} {PROG_PATH} -t uart -u {up_port} -b {baud} -d {mcu} '

# callback for upload
def on_upload(source, target, env):
    firmware_path = str(source[0]) 
    # write the hex file
    env.Execute(CMD + f'-a write -f{firmware_path}')

env.Replace(UPLOADCMD=on_upload)

# callback for fuse setting, programming the lockbit is not implemented
def on_setFuses(source, target, env):
    fuses = ""
    fs = env.subst("$FUSESFLAGS").split(" ")
    for fuse in fs:
        if fuse.startswith('-Ufuse' ):
            # print(f'fuse no {int(fuse[6])} = {fuse[10:14]}')
            fuses += f' {fuse[6]}:{fuse[10:14]}'
    env.Execute(CMD + f' --fuses {fuses}')

env.Replace(SETFUSESCMD=on_setFuses)
