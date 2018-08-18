#!/usr/bin/env python3

import subprocess
import sys
import os
from functions import *
from variables import *

# Get script arguments
branch = sys.argv[1] if len(sys.argv) > 1 else ''

# Re-create release folder
RecreateDir(RELEASE_DIR)

# Go to release folder
os.chdir(pjoin(RELEASE_DIR))

# Common vars
pro_file = '{}'.format(pjoin(PROJECT_FILE))

# Os spceific vars
if Os() == 'mac':
    spec = '-spec macx-clang "CONFIG+=x86_64"'
    compiler = 'make'# -j' # -j$(nproc)
elif Os() == 'win':
    if branch == 'windows_mingw':
        spec = '-spec win32-g++ "CONFIG+=x86"'
        compiler = 'mingw32-make'# -j' # -j%NUMBER_OF_PROCESSORS%
    elif branch == 'windows_msvc':
        spec = '-spec win32-msvc "CONFIG+=x64"'
        compiler = 'nmake /NOLOGO'
elif Os() == 'lin':
    spec = 'PREFIX=/usr'
    compiler = 'make'# -j'
else:
    print('ERROR: Unknown OS')
    exit()

# Build
#print(subprocess.run(['qmake', '-v'], stdout=subprocess.PIPE).stdout.decode('utf-8'))
#print(subprocess.run([compiler, '-v'], stdout=subprocess.PIPE).stdout.decode('utf-8'))

# Qmake
args = ['qmake', pro_file, *spec.split()]
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
print(result)

# Make
args = compiler.split()
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
print(result)
