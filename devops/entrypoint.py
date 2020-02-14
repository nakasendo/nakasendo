#!/usr/bin/python3
import subprocess
import os
import platform
import sys
import errno
import shlex
import pwd
import shutil
from distutils.dir_util import copy_tree

def do_windows_build():
    ######## Note: Windows build will not currently work in Docker, so this function is never used. #########
    print("doing windows build")
    try:
        os.mkdir("build")
    except OSError as exc:
        if exc.errno != errno.EEXIST:
            raise

    os.chdir('build')

    args = ["cmake", "%WORKSPACE%", "-G", "Visual Studio 16 2019", "-A", "x64"]
    subprocess.call(args)

    args = ["cmake", "--build", ".", "--target", "ALL_BUILD", "--config", "Debug"]
    try:
        subprocess.call(args)
    except:
        exit(-1)

    args = ["cmake", "--build", ".", "--target", "ALL_BUILD", "--config", "Release"]
    try:
        return subprocess.call(args)
    except:
        exit(-1)


def do_linux_build():
    print("in linux build")
    try:
        os.mkdir("releasebuild")
    except OSError as exc:
        if exc.errno != errno.EEXIST:
            raise

    rawargs = '. /emsdk/emsdk_env.sh'
    args = shlex.split(rawargs)
    subprocess.call(args, shell=True)

    os.environ['OPENSSL_ROOT_DIR'] = '/opt/openssl'
    os.environ['OPENSSL_WASM_ROOT_DIR'] = '/openssl'
    os.environ["PATH"] += os.pathsep + '/emsdk/fastcomp/emscripten/'
    os.environ['LD_LIBRARY_PATH'] = '/usr/lib/x86_64-linux-gnu/'
    os.environ['LIBRARY_PATH'] = '/usr/lib/x86_64-linux-gnu/'
    here  = os.getcwd()
    os.chdir('releasebuild')
    os.environ['LIBS'] = '-ldl'
    rawargs = '/usr/bin/cmake .. -G "Unix Makefiles" -DSDK_IS_BETA=ON -DBUILD_WEBASSEMBLY=ON -DCMAKE_BUILD_TYPE=Release -DCUSTOM_SYSTEM_OS_NAME=Ubuntu'
    args = shlex.split(rawargs)
    subprocess.call(args)

    args = ['/usr/bin/make']
    try:
        subprocess.call(args)
    except:
        exit(-1)

    rawargs = 'cpack -G TGZ'
    args = shlex.split(rawargs)
    subprocess.call(args)

    os.chdir(here)

    try:
        os.mkdir("debugbuild")
    except OSError as exc:
        if exc.errno != errno.EEXIST:
            raise
        pass

    os.chdir('debugbuild')

    rawargs = '/usr/bin/cmake .. -G "Unix Makefiles" -DBUILD_WEBASSEMBLY=ON -DSDK_IS_BETA=ON -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_SYSTEM_OS_NAME=Ubuntu'
    args = shlex.split(rawargs)
    subprocess.call(args)

    args = ['/usr/bin/make']
    try:
        return subprocess.call(args)
    except:
        exit(-1)

    rawargs = 'cpack -G TGZ'
    args = shlex.split(rawargs)
    subprocess.call(args)

def main():
    # do the release notes
    args = ["python3", "devops/releasenotes.py"]
    out = subprocess.check_output(args)
    # do the building
    if platform.system() == "Linux":
        return do_linux_build()
    else:
        return do_windows_build()

if __name__ == "__main__":
    main()

