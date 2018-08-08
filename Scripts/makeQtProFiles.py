#!/usr/bin/env python3

from functions import *
from variables import *
from qtprofile import *

######################################
# Common setting for all apps and libs
######################################

def SetCommonForAll(pro):

    # Build type (as directory name)
    pro.addBuildType(DEBUG_DIR_NAME, PROFILE_DIR_NAME, RELEASE_DIR_NAME)

    # Destination
    pro.addDestDir(BUILD_TYPE_DIR)

    # Source paths to be included
    for lib in MY_LIBS_NAMES:
        pro.addIncludePath(MY_LIBS_DIR + [lib])
    for lib in OTHER_LIBS_NAMES:
        pro.addIncludePath(OTHER_LIBS_DIR + [lib])

    # Resources
    pro.addIncludePath(RESOURCES_DIR)

    # Compiled libraries paths to be included
    for lib in MY_LIBS_NAMES:
        pro.addLibs(BUILD_TYPE_DIR, MY_LIBS_PREFIX, lib)
    for lib in OTHER_LIBS_NAMES:
        pro.addLibs(BUILD_TYPE_DIR, '', lib)

    # C++11 support for qmake when generating a Makefile.
    pro.addCppVersion(CPP_VERSION)
    pro.addQmakeCxxFlags(QMAKE_CXXFLAGS)

################################################
# Common Window and Console application settings
################################################

def SetCommonForApps(pro):

    # Build output type
    pro.addTemplate(APPS_TEMPLATE)

    # Define macros to be used in the source code
    pro.addDefines(DEFINES_DICT)
    pro.addDefines(DEFINES_MISC)

    # List of libraries to be checked on changes when building the project
    for lib in MY_LIBS_NAMES:
        pro.addPostTargetDeps(BUILD_TYPE_DIR, MY_LIBS_PREFIX, lib)
    for lib in OTHER_LIBS_NAMES:
        pro.addPostTargetDeps(BUILD_TYPE_DIR, '', lib)

###########################
# Common libraries settings
###########################

def SetCommonForLibs(pro):

    # Build output type
    pro.addTemplate(LIBS_TEMPLATE)

    # Variable that qmake uses when generating a Makefile
    pro.addConfig(LIBS_CONFIG)
    
    #unix {
    #target.path = /usr/lib
    #INSTALLS += target }

#########
# Project
#########

project = QtProFile()

# Build output type
project.addTemplate(SUBDIRS_TEMPLATE)

# List of all the subdirectories to be processed
project.addSubDirs(PROJECT_SUBDIRS)

# What subproject depends on others: To build libs before apps
project.addDepends(APPS_DIR_NAME, LIBS_DIR_NAME)
project.addDepends(TESTS_DIR_NAME, LIBS_DIR_NAME)

# Other files which are part of a Qt project
#OTHER_FILES += README.md LICENSE Project.qdocconf .gitignore .travis.yml .appveyor.yml
for dirName in PROJECT_OTHER_FILES_DIRS:
    dirPath = PROJECT_DIR + [dirName]
    files = [os.path.join(dirName, fileName) for fileName in GetSelectedFileList(dirPath, ANY_EXT)]
    project.addOtherFiles(files)

project.save(PROJECT_DIR + [PROJECT_NAME])

##############
# Applications
##############

apps = QtProFile()

# Build output type
apps.addTemplate(SUBDIRS_TEMPLATE)

# List of all the subdirectories to be processed
apps.addSubDirs(APPS_SUBDIRS)

apps.save(APPS_DIR + [APPS_DIR_NAME])

####################
# Window application
####################

window = QtProFile()
SetCommonForAll(window)
SetCommonForApps(window)

# Set application icon
window.addIcon(ICON_DIR + [APP_NAME])

# Qt modules to used in the project
window.addQt(WINDOW_APP_QT_MODULES)

# Set name of the executable
window.addTarget(WINDOW_APP_NAME)

# Builds paths
window.addObjectsDir(BUILD_TYPE_DIR + [OBJECTS_DIR_NAME] + [APPS_DIR_NAME] + [APP_NAME])
window.addMocDir(BUILD_TYPE_DIR + [MOC_DIR_NAME] + [APPS_DIR_NAME] + [APP_NAME])
window.addRccDir(BUILD_TYPE_DIR + [RCC_DIR_NAME] + [APPS_DIR_NAME] + [APP_NAME])
window.addUiDir(BUILD_TYPE_DIR + [UI_DIR_NAME] + [APPS_DIR_NAME] + [APP_NAME])

# List of files to be used in the project
window.addHeaders(GetSelectedFileList(WINDOW_APP_DIR, HEADER_EXT))
window.addSources(GetSelectedFileList(WINDOW_APP_DIR, SOURCE_EXT))
window.addResources(RESOURCES_DIR, GetSelectedFileList(RESOURCES_DIR, RESOURCE_EXT))

window.save(WINDOW_APP_DIR + [WINDOW_APP_DIR_NAME])

#####################
# Console application
#####################

console = QtProFile()
SetCommonForAll(console)
SetCommonForApps(console)

# Qt modules to used in the project
console.addQt(CONSOLE_APP_QT_MODULES)

# Set name of the executable
console.addTarget(CONSOLE_APP_NAME)

# Variable that qmake uses when generating a Makefile
console.addConfig(CONSOLE_APP_CONFIG)
console.delConfig(CONSOLE_APP_CONFIG_DEL)

# Builds paths
console.addObjectsDir(BUILD_TYPE_DIR + [OBJECTS_DIR_NAME] + [APPS_DIR_NAME] + [CONSOLE_APP_NAME])
console.addMocDir(BUILD_TYPE_DIR + [MOC_DIR_NAME] + [APPS_DIR_NAME] + [CONSOLE_APP_NAME])
console.addRccDir(BUILD_TYPE_DIR + [RCC_DIR_NAME] + [APPS_DIR_NAME] + [CONSOLE_APP_NAME])
console.addUiDir(BUILD_TYPE_DIR + [UI_DIR_NAME] + [APPS_DIR_NAME] + [CONSOLE_APP_NAME])

# List of files to be used in the project
console.addHeaders(GetSelectedFileList(CONSOLE_APP_DIR, HEADER_EXT))
console.addSources(GetSelectedFileList(CONSOLE_APP_DIR, SOURCE_EXT))

console.save(CONSOLE_APP_DIR + [CONSOLE_APP_DIR_NAME])

###########
# Libraries
###########

libs = QtProFile()

# Build output type.
libs.addTemplate(SUBDIRS_TEMPLATE)

# List of all the subdirectories to be processed.
libs.addSubDirs(LIBS_SUBDIRS)

libs.save(LIBS_DIR + [LIBS_DIR_NAME])

####################
# 3rdParty libraries
####################

other_libs = QtProFile()

# Build output type.
other_libs.addTemplate(SUBDIRS_TEMPLATE)

# List of all the subdirectories to be processed.
other_libs.addSubDirs(OTHER_LIBS_NAMES)

other_libs.save(OTHER_LIBS_DIR + [OTHER_LIBS_DIR_NAME])

# Create pro file for every lib
for lib in OTHER_LIBS_NAMES:
    pro = QtProFile()
    SetCommonForAll(pro)
    SetCommonForLibs(pro)
    pro.addQt('widgets')
    pro.addObjectsDir(BUILD_TYPE_DIR + [OBJECTS_DIR_NAME] + [LIBS_DIR_NAME] + [OTHER_LIBS_DIR_NAME] + [lib])
    pro.addMocDir(BUILD_TYPE_DIR + [MOC_DIR_NAME] + [LIBS_DIR_NAME] + [OTHER_LIBS_DIR_NAME] + [lib])
    pro.addRccDir(BUILD_TYPE_DIR + [RCC_DIR_NAME] + [LIBS_DIR_NAME] + [OTHER_LIBS_DIR_NAME] + [lib])
    pro.addUiDir(BUILD_TYPE_DIR + [UI_DIR_NAME] + [LIBS_DIR_NAME] + [OTHER_LIBS_DIR_NAME] + [lib])
    dir = OTHER_LIBS_DIR + [lib]
    pro.addHeaders(GetSelectedFileList(dir, HEADER_EXT))
    pro.addSources(GetSelectedFileList(dir, SOURCE_EXT))
    pro.addOtherFiles(GetSelectedFileList(dir, DOC_EXT))
    pro.save(dir + [lib])

####################
# Original libraries
####################

my_libs = QtProFile()

# Build output type.
my_libs.addTemplate(SUBDIRS_TEMPLATE)

# List of all the subdirectories to be processed.
my_libs.addSubDirs(MY_LIBS_NAMES)

my_libs.save(MY_LIBS_DIR + [MY_LIBS_DIR_NAME])

# Create pro file for every lib
for lib in MY_LIBS_NAMES:
    pro = QtProFile()
    SetCommonForAll(pro)
    SetCommonForLibs(pro)
    if lib == 'Diffraction':
        pro.addQt('widgets concurrent') # move to variables.py!?
    if lib == 'Widgets':
        pro.addQt('widgets') # move to variables.py!?
    pro.addTarget(MY_LIBS_PREFIX + lib)
    pro.addObjectsDir(BUILD_TYPE_DIR + [OBJECTS_DIR_NAME] + [LIBS_DIR_NAME] + [MY_LIBS_DIR_NAME] + [lib])
    pro.addMocDir(BUILD_TYPE_DIR + [MOC_DIR_NAME] + [LIBS_DIR_NAME] + [MY_LIBS_DIR_NAME] + [lib])
    pro.addRccDir(BUILD_TYPE_DIR + [RCC_DIR_NAME] + [LIBS_DIR_NAME] + [MY_LIBS_DIR_NAME] + [lib])
    pro.addUiDir(BUILD_TYPE_DIR + [UI_DIR_NAME] + [LIBS_DIR_NAME] + [MY_LIBS_DIR_NAME] + [lib])
    dir = MY_LIBS_DIR + [lib]
    pro.addHeaders(GetSelectedFileList(dir, HEADER_EXT))
    pro.addSources(GetSelectedFileList(dir, SOURCE_EXT))
    pro.addOtherFiles(GetSelectedFileList(dir, DOC_EXT))
    pro.save(dir + [lib])

###################
# Tests application
###################

tests = QtProFile()
SetCommonForAll(tests)
SetCommonForApps(tests)

# Set name of the executable
tests.addTarget(TESTS_NAME)

# Variable that qmake uses when generating a Makefile
tests.addConfig(CONSOLE_APP_CONFIG)
tests.delConfig(CONSOLE_APP_CONFIG_DEL)

# Builds paths
tests.addObjectsDir(BUILD_TYPE_DIR + [OBJECTS_DIR_NAME] + [APPS_DIR_NAME] + [TESTS_NAME])
tests.addMocDir(BUILD_TYPE_DIR + [MOC_DIR_NAME] + [APPS_DIR_NAME] + [TESTS_NAME])
tests.addRccDir(BUILD_TYPE_DIR + [RCC_DIR_NAME] + [APPS_DIR_NAME] + [TESTS_NAME])
tests.addUiDir(BUILD_TYPE_DIR + [UI_DIR_NAME] + [APPS_DIR_NAME] + [TESTS_NAME])

# List of files to be used in the project
tests.addHeaders(GetSelectedFileList(TESTS_DIR, HEADER_EXT))
tests.addSources(GetSelectedFileList(TESTS_DIR, SOURCE_EXT))

# Save to files
tests.save(TESTS_DIR + [TESTS_DIR_NAME])
