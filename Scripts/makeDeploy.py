#!/usr/bin/env python3

import subprocess
from functions import *
from variables import *

# Re-create old folder
RecreateDir(DEPLOY_DIR)

# Copy files/dirs for deployment
dst = DEPLOY_DIR
for src in RELEASE_PATHS_TO_DEPLOY:
    Copy(src, dst)

# Deploy
if Os() == 'mac':
    args = ['macdeployqt',
            '{}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_FILE)][0]),
            '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_CONSOLE_FILE)][0]),
            '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(TESTS_FILE)][0]),
            '-verbose=1']
    subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
elif Os() == 'win':
    for path in DEPLOY_PATHS:
        args = ['windeployqt',
                '{}'.format(pjoin(path)),
                '--release',
                '--verbose', '1']
        subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
elif Os() == 'lin':
    args = ['linuxdeployqt',
            '{}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_FILE)][0]),
            '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_CONSOLE_FILE)][0]),
            '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(TESTS_FILE)][0]),
            '-bundle-non-qt-libs']
    result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
    print(result)
    #args = ['linuxdeployqt',
    #        '{}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_FILE)][0]),
    #        '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(APP_CONSOLE_FILE)][0]),
    #        '-executable={}'.format(DEPLOY_PATHS[DEPLOY_NAMES.index(TESTS_FILE)][0]),
    #        '-appimage']
    #result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
    #print(result)
else:
    print('ERROR: Unknown OS')
    exit()

