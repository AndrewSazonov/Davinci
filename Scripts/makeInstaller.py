#!/usr/bin/env python3

import sys
import subprocess
from functions import *
from variables import *
from installer import *

# Get script arguments
branch = sys.argv[1] if len(sys.argv) > 1 else REPO_DIR_NAME_MACOS

# Remove old folder
RemoveDir(INSTALLER_DIR)

# Create directories tree
CreateDir(INSTALLER_CONFIG_DIR)
CreateDir(INSTALLER_PACKAGES_DATA_DIR)
CreateDir(INSTALLER_PACKAGES_META_DIR)

# Create config files
WriteStringToFile(INSTALLER_CONFIG_FILE, Config(branch))
WriteStringToFile(INSTALLER_PACKAGES_FILE, Package())

# Copy files/dirs for installer
for src in GetFileAndSubDirList(DEPLOY_DIR):
    src = DEPLOY_DIR + [src]
    Copy(src, INSTALLER_PACKAGES_DATA_DIR)
Copy(EXAMPLES_DIR, INSTALLER_PACKAGES_DATA_DIR)
Copy(LICENSE_FILE, INSTALLER_PACKAGES_META_DIR)
Copy(CHANGELOG_FILE, INSTALLER_PACKAGES_DATA_DIR)
Copy(CONTROLSCRIPT_FILE, INSTALLER_CONFIG_DIR)
Copy(INSTALLSCRIPT_FILE, INSTALLER_PACKAGES_META_DIR)
#Copy(RUNAPP_FILE, INSTALLER_PACKAGES_META_DIR)

# Create installer
args = ['binarycreator',
        pjoin(INSTALLER_FILE),
        '-c', pjoin(INSTALLER_CONFIG_FILE),
        '-p', pjoin(INSTALLER_PACKAGES_DIR),
        #'--online-only',
        #'--offline-only',
        '--verbose' ]
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
#print(result)

# Create repository
args = ['repogen',
        '--verbose',
        '--update-new-components',
        '-p', pjoin(INSTALLER_PACKAGES_DIR),
        pjoin(INSTALLER_DIR, branch)]
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
#print(result)



