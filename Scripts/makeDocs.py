#!/usr/bin/env python3

# http://doc.qt.io/qt-5.8/qthelp-framework.html
# http://doc.qt.io/qt-5/qdoc-guide-conf.html
# http://doc.qt.io/qt-5/22-qdoc-configuration-generalvariables.html

import subprocess
import os

from functions import *
from variables import *
from docs import *

#########################
# Set Qt global variables
#########################

result = subprocess.run(['qmake', '-query'], stdout=subprocess.PIPE).stdout.decode('utf-8')
for pair in result.split('\n'):
    if pair:
        var, value = pair.split(':')
        if value:
            os.environ[var] = value
            #print(var, value)
os.environ['QT_VERSION_TAG'] = os.environ['QT_VERSION']
os.environ['QT_VER'] = os.environ['QT_VERSION']
os.environ['BUILDDIR'] = pjoin(BUILD_DIR)

###############
# Create config
###############

doc = Docs()

# Include
doc.addText('include({}/global/qt-module-defaults.qdocconf)'.format(os.environ['QT_INSTALL_DOCS']))

# App details
doc.addData('project',     '=', APP_NAME)
doc.addData('description', '=', APP_DESCRIPTION)
doc.addData('version',     '=', APP_VERSION)

# Redefine: Set the main Qt index.html from config.qdocconf
#navigation.homepage = "Qt $QT_VER"
doc.addData('navigation.homepage', '=', '{} {}'.format(APP_NAME, APP_VERSION))

# Redefine: Show Qt version as part of the navigation bar
#buildversion = "Qt $QT_VERSION Reference Documentation"
doc.addData('buildversion', '=', '{} {} Reference Documentation'.format(APP_NAME, APP_VERSION))

# qhp details
doc.addData('qhp.projects',              '=', APP_NAME)
doc.addData('qhp.Davinci.file',          '=', '{}.qhp'.format(APP_NAME.lower()))
doc.addData('qhp.Davinci.namespace',     '=', 'org.sazonov.davinci.003')
doc.addData('qhp.Davinci.virtualFolder', '=', APP_NAME)
doc.addData('qhp.Davinci.indexTitle',    '=', APP_NAME)
doc.addData('qhp.Davinci.indexRoot',     '=', '')

# Ignore tokens
doc.addData('Cpp.ignoretokens', '+=', ['AS_BEGIN_NAMESPACE', 'AS_END_NAMESPACE'])

# Excludes specified QDoc warnings from the output, e.g., Missing parameter name
doc.addData('spurious', '=', ['"Cannot find .*"', '"Missing .*"'])

# Output
doc.addData('outputdir',     '=', pjoin(DOCS_DIR))
doc.addData('outputformats', '=', 'HTML')

# Header dirs
doc.addData('headerdirs', '+=', GetDirTree(MY_LIBS_DIR))

# Header files
#doc.addData('headers', '+=', GetAllSelectedFiles(MY_LIBS_DIR, HEADER_EXT))

# Source dirs
doc.addData('sourcedirs', '+=', GetDirTree(MY_LIBS_DIR))

# Source files
#doc.addData('sources', '+=', GetAllSelectedFiles(MY_LIBS_DIR, SOURCE_EXT))

# Save qdocconf
doc.save(DOCS_QDOCCONF_FILE, '.qdocconf')

#############
# Create docs
#############

# Re-create doc folder
RecreateDir(DOCS_DIR)

# Go to release folder
#os.chdir(pjoin(RELEASE_DIR))

# QDoc
args = ['qdoc',
        '{}.qdocconf'.format(pjoin(DOCS_QDOCCONF_FILE)),
        #'--debug'
        ]
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
print(result)

# QHelpGenerator
args = ['qhelpgenerator',
        '{}.qhp'.format(pjoin(DOCS_QHP_FILE)),
        '-o',
        '{}.qch'.format(pjoin(DOCS_QHP_FILE)),
        '-s' # Suppresses status messages.
        ]
result = subprocess.run(args, stdout=subprocess.PIPE).stdout.decode('utf-8')
print(result)

##########
# Clean up
##########

RemoveThis('{}.qdocconf'.format(pjoin(DOCS_QDOCCONF_FILE)))
