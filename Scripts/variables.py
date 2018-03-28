#!/usr/bin/env python3

import os
from changelog import *
from functions import *

######
# Info
######

# https://askubuntu.com/questions/320996/how-to-make-python-program-command-execute-python-3
# mac: alias python=python3

# set PATH=c:\Users\localadmin\AppData\Local\Programs\Python\Python36-32;%PATH%

##################
# Application name
##################

# Apps
APP_NAME                    = 'Davinci'
APP_CONSOLE_SUFFIX          = 'Console'
TESTS_SUFFIX                = 'Tests'
APP_CONSOLE_NAME            = APP_NAME + APP_CONSOLE_SUFFIX
TESTS_NAME                  = APP_NAME + TESTS_SUFFIX

APP_FILE                    = OsSpecificGui(APP_NAME)
APP_CONSOLE_FILE            = OsSpecificCli(APP_CONSOLE_NAME)
TESTS_FILE                  = OsSpecificCli(TESTS_NAME)

APP_VERSION                 = Changelog().version()
APP_RELEASE_DATE            = Changelog().date()
APP_RELEASE_YEAR            = ConvertDate(APP_RELEASE_DATE, '%d %b %Y', '%Y')

APP_DOMAIN                  = 'sazonov.org'
APP_OWNER                   = 'Andrew Sazonov'
APP_PUBLISHER               = 'Sazonov'
APP_URL                     = 'http://davinci.sazonov.org'
APP_COPYRIGHT               = 'Copyright (C) {} {}. All rights reserved.'.format(APP_RELEASE_YEAR, APP_OWNER)
APP_DESCRIPTION             = 'A Scientific Software for the Visualization and Processing of Single-Crystal Diffraction Data Measured with a Point Detector'

ISSUETRACKER_URL            = 'https://github.com/AndrewSazonov/{}/issues'.format(APP_NAME)

###################
# Directories names
###################

# Global
APPS_TEMPLATE               = 'app'
LIBS_TEMPLATE               = 'lib'
SUBDIRS_TEMPLATE            = 'subdirs'
SCRIPT_DIR                  = os.path.dirname(os.path.abspath(__file__))
PROJECT_NAME                = 'Project'
PROJECT_DIR                 = [SCRIPT_DIR, '..']
PROJECT_FILE                = PROJECT_DIR + [PROJECT_NAME + '.pro']

# Apps
APPS_DIR_NAME               = 'Applications'
CONSOLE_APP_DIR_NAME        = 'Console'
WINDOW_APP_DIR_NAME         = 'Window'
APPS_DIR                    = PROJECT_DIR + [APPS_DIR_NAME]
CONSOLE_APP_DIR             = APPS_DIR + [CONSOLE_APP_DIR_NAME]
WINDOW_APP_DIR              = APPS_DIR + [WINDOW_APP_DIR_NAME]
APPS_SUBDIRS                = [CONSOLE_APP_DIR_NAME, WINDOW_APP_DIR_NAME]

# Modules and config
APP_QT_MODULES              = 'core gui xml svg network widgets printsupport'.split()
APP_CONSOLE_CONFIG          = 'console'.split()
APP_CONSOLE_CONFIG_DEL      = 'app_bundle'.split()
LIBS_CONFIG                 = 'staticlib'

# Libs
LIBS_DIR_NAME               = 'Libraries'
MY_LIBS_PREFIX              = 'As'
MY_LIBS_DIR_NAME            = 'Original'
MY_LIBS_NAMES               = 'Core Diffraction DataTypes Widgets'.split() # order is important!
OTHER_LIBS_DIR_NAME         = '3rdParty'
OTHER_LIBS_NAMES            = 'QCodeEditor QCustomPlot'.split()
LIBS_DIR                    = PROJECT_DIR + [LIBS_DIR_NAME]
MY_LIBS_DIR                 = LIBS_DIR + [MY_LIBS_DIR_NAME]
OTHER_LIBS_DIR              = LIBS_DIR + [OTHER_LIBS_DIR_NAME]
LIBS_SUBDIRS                = [OTHER_LIBS_DIR_NAME, MY_LIBS_DIR_NAME]

# Tests
TESTS_DIR_NAME              = 'Tests'
TESTS_DIR                   = PROJECT_DIR + [TESTS_DIR_NAME]

# Global
PROJECT_SUBDIRS             = [APPS_DIR_NAME, LIBS_DIR_NAME, TESTS_DIR_NAME]

# Resources
RESOURCES_DIR_NAME          = 'Resources'
ICON_DIR_NAME               = 'Icon'
RESOURCES_DIR               = PROJECT_DIR + [RESOURCES_DIR_NAME]
ICON_DIR                    = RESOURCES_DIR + [ICON_DIR_NAME]

# Builds
BUILD_DIR_NAME              = 'Build'
OBJECTS_DIR_NAME            = '.obj'
MOC_DIR_NAME                = '.moc'
RCC_DIR_NAME                = '.rcc'
UI_DIR_NAME                 = '.ui'
BUILD_DIR                   = PROJECT_DIR + ['..', BUILD_DIR_NAME]
BUILD_TYPE                  = '$${BUILD_TYPE}'
BUILD_TYPE_DIR              = BUILD_DIR + [BUILD_TYPE]

# Debug, Release
DEBUG_DIR_NAME              = 'Debug'
RELEASE_DIR_NAME            = 'Release'
RELEASE_DIR                 = BUILD_DIR + [RELEASE_DIR_NAME]
RELEASE_NAMES_TO_DEPLOY     = [APP_FILE, APP_CONSOLE_FILE, TESTS_FILE]
RELEASE_PATHS_TO_DEPLOY     = []
for item in RELEASE_NAMES_TO_DEPLOY:
    item = RELEASE_DIR + [item]
    RELEASE_PATHS_TO_DEPLOY.append([pjoin(item)])

# Deploy
DEPLOY_DIR_NAME             = 'Deploy'
DEPLOY_DIR                  = BUILD_DIR + [DEPLOY_DIR_NAME]
DEPLOY_NAMES                = [APP_FILE, APP_CONSOLE_FILE, TESTS_FILE]
DEPLOY_PATHS                = []
for item in RELEASE_NAMES_TO_DEPLOY:
    item = DEPLOY_DIR + [item]
    DEPLOY_PATHS.append([pjoin(item)])

# Installer
INSTALLER_SUFFIX            = 'Installer'
INSTALLER_TITLE             = '{} {}'.format(APP_NAME, INSTALLER_SUFFIX)
INSTALLER_VERSION           = '1.0.0'
INSTALLER_DIR_NAME          = INSTALLER_SUFFIX
INSTALLER_DIR               = BUILD_DIR + [INSTALLER_DIR_NAME]
INSTALLER_FILE              = INSTALLER_DIR + [APP_NAME + OsSpecificGui(INSTALLER_SUFFIX)]
INSTALLER_CONFIG_DIR        = INSTALLER_DIR + ['config']
INSTALLER_CONFIG_FILE       = INSTALLER_CONFIG_DIR + ['config.xml']
INSTALLER_PACKAGES_DIR      = INSTALLER_DIR + ['packages']
INSTALLER_PACKAGES_MAIN_DIR = INSTALLER_PACKAGES_DIR + ['org.sazonov.davinci'] # APP_NAME.lower()
INSTALLER_PACKAGES_DATA_DIR = INSTALLER_PACKAGES_MAIN_DIR + ['data']
INSTALLER_PACKAGES_META_DIR = INSTALLER_PACKAGES_MAIN_DIR + ['meta']
INSTALLER_PACKAGES_FILE     = INSTALLER_PACKAGES_META_DIR + ['package.xml']
#
#INSTALLER_SCRIPTS_DIR_NAME  = INSTALLER_SUFFIX
#INSTALLER_SCRIPTS_DIR       = PROJECT_DIR + [INSTALLER_SCRIPTS_DIR_NAME]
CONTROLSCRIPT_FILE_NAME     = 'controlscript.js'
CONTROLSCRIPT_FILE          = [SCRIPT_DIR] + [CONTROLSCRIPT_FILE_NAME]
INSTALLSCRIPT_FILE_NAME     = 'installscript.js'
INSTALLSCRIPT_FILE          = [SCRIPT_DIR] + [INSTALLSCRIPT_FILE_NAME]
#RUNAPP_FILE_NAME            = 'runapp.ui'
#RUNAPP_FILE                 = INSTALLER_SCRIPTS_DIR + [RUNAPP_FILE_NAME]
#
if Os() == 'mac':
    TARGET_DIR              = '@ApplicationsDir@/' + APP_NAME
elif Os() == 'win':
    TARGET_DIR              = '@ApplicationsDir@\\' + APP_NAME
else: #if Os() == 'lin':
    TARGET_DIR              = '@HomeDir@/' + APP_NAME

# Maintain
MAINTAINER_SUFFIX           = 'Uninstaller'
MAINTAINER_NAME             = APP_NAME + MAINTAINER_SUFFIX

# Repositories
REPOS_DIR_NAME              = 'repositories'
REPOS_DIR                   = '{}/{}'.format(APP_URL, REPOS_DIR_NAME)
REPO_DIR_NAME_MACOS         = 'macos_clang'
#if Os() == 'mac':
#    REPO_DIR_NAME           = 'macos_clang'
#elif Os() == 'win':
#    REPO_DIR_NAME           = 'windows_mingw'
#else: #if Os() == 'lin':
#    REPO_DIR_NAME           = 'lin_gcc'
#REPO_DIR                    = INSTALLER_DIR + [REPO_DIR_NAME]
#REPO_URL                    = '{}/{}/{}'.format(APP_URL, REPOS_DIR_NAME, REPO_DIR_NAME)

############
# Misc files
############

LICENSE_NAME                = 'GNU General Public License Version 3'
LICENSE_FILE_NAME           = 'LICENSE'
LICENSE_FILE                = PROJECT_DIR + [LICENSE_FILE_NAME]

CHANGELOG_FILE_NAME         = 'CHANGELOG.md'
CHANGELOG_FILE              = PROJECT_DIR + [CHANGELOG_FILE_NAME]
CHANGELOG_TITLE             = '{} Version History'.format(APP_NAME)

USERMANUAL_TITLE            = APP_NAME + ' User Manual'
USERMANUAL_DIR_NAME         = 'UserManual'
USERMANUAL_FILE_NAME        = 'umanual'
USERMANUAL_URL              = '{}/{}.html'.format(APP_URL, USERMANUAL_FILE_NAME)
USERMANUAL_FILE_EXT         = '.tex'
USERMANUAL_DIR              = PROJECT_DIR + ['..', USERMANUAL_DIR_NAME]
USERMANUAL_FILE             = USERMANUAL_DIR + [USERMANUAL_FILE_NAME + USERMANUAL_FILE_EXT]
USERMANUAL_CHAPTER_NAMES    = ['umanual_intro', 'umanual_start', 'umanual_use']
USERMANUAL_CHAPTER_FILES    = []
for item in USERMANUAL_CHAPTER_NAMES:
    item = USERMANUAL_DIR + [item + USERMANUAL_FILE_EXT]
    USERMANUAL_CHAPTER_FILES.append([pjoin(item)])

WEBPAGES_DIR_NAME           = 'WebPages'
WEBPAGES_FILE_EXT           = '.html'
WEBPAGES_DIR                = PROJECT_DIR + ['..', WEBPAGES_DIR_NAME]
WEBPAGES_INDEX_FILE         = WEBPAGES_DIR + ['index' + WEBPAGES_FILE_EXT]
WEBPAGES_CHANGELOG_FILE     = WEBPAGES_DIR + ['changelog' + WEBPAGES_FILE_EXT]
WEBPAGES_USERMANUAL_FILE    = WEBPAGES_DIR + [USERMANUAL_FILE_NAME + WEBPAGES_FILE_EXT]
WEBPAGES_USERMANUAL_FILES   = []
WEBPAGES_USERMANUAL_TOC     = [[] for i in range(4)]
for item in USERMANUAL_CHAPTER_NAMES:
    item = WEBPAGES_DIR + [item + WEBPAGES_FILE_EXT]
    WEBPAGES_USERMANUAL_FILES.append([pjoin(item)])

EXAMPLES_DIR_NAME           = 'Examples'
EXAMPLES_DIR                = PROJECT_DIR + [EXAMPLES_DIR_NAME]

DOCS_DIR_NAME               = 'Docs'
DOCS_DIR                    = PROJECT_DIR + [DOCS_DIR_NAME]
DOCS_QDOCCONF_FILE          = PROJECT_DIR + [APP_NAME]
DOCS_QHP_FILE               = DOCS_DIR + [APP_NAME]

HEADER_EXT                  = '.hpp .h'
SOURCE_EXT                  = '.cpp'
RESOURCE_EXT                = '.qrc'
DOC_EXT                     = '.qdoc'

##############
# DEFINE MACRO
##############

# Adding C preprocessor #DEFINE so we can use APP_NAME, etc. in C++ code
DEFINES_DICT                = {'APP_NAME':          APP_NAME,
                               'APP_VERSION':       APP_VERSION,
                               'APP_RELEASE_DATE':  APP_RELEASE_DATE,
                               'APP_URL':           APP_URL,
                               'APP_DOMAIN':        APP_DOMAIN,
                               'APP_OWNER':         APP_OWNER,
                               'APP_COPYRIGHT':     APP_COPYRIGHT,
                               'APP_DESCRIPTION':   APP_DESCRIPTION,
                               'MAINTAINER_NAME':   MAINTAINER_NAME,
                               'USERMANUAL_URL':    USERMANUAL_URL,
                               'ISSUETRACKER_URL':  ISSUETRACKER_URL}

# Add in release output of such information as %{function}, %{line}, %{message}, etc.
DEFINES_MISC                = 'QT_MESSAGELOGCONTEXT'

#####
# C++
#####

CPP_VERSION                 = 'c++11'
QMAKE_CXXFLAGS              = '-std=gnu++11 -std=c++11'.split()



#changelog = Changelog(title='{} Version History'.format(app_name))
#product_version = changelog.version()
#release_date_config = ConvertDate(changelog.date(), '%d.%m.%Y', '%Y-%m-%d')
#update_text = 'This changed compared to the last release'
#script_name = 'installscript.js'

