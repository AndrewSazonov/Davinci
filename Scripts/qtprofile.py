#!/usr/bin/env python3

import os
from functions import *

class QtProFile:
    
    def __init__(self):
        self.s = ''
    
    def addData(self, name, sign, vars, newline=True):
        nl = '\n' if newline else ''
        if type(vars) is str:
            vars = [vars]
        string = '{} {}'.format(name, sign)
        for item in vars:
            string += ' {}'.format(item)
        self.s += '{}{}'.format(string, nl)
        
    def addTarget(self, target):
        self.addData('TARGET', '=', target)
    def addTemplate(self, path):
        self.addData('TEMPLATE', '=', path)
    def addObjectsDir(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('OBJECTS_DIR', '=', path)
    def addMocDir(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('MOC_DIR', '=', path)
    def addRccDir(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('RCC_DIR', '=', path)
    def addUiDir(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('UI_DIR', '=', path)
        
    def addDestDir(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('DESTDIR', '=', path)

    def addHeaders(self, vars):
        if vars: self.addData('HEADERS', '+=', vars)
    def addSources(self, vars):
        if vars: self.addData('SOURCES', '+=', vars)
    def addResources(self, dir, vars):
        if type(dir) is str:
            dir = [dir]
        string = 'RESOURCES += '
        for item in vars:
            string += ' {}'.format(pjoin(dir, item))
        self.s += '{}\n'.format(string)

    def addSubDirs(self, vars):
        if vars: self.addData('SUBDIRS', '+=', vars)
    def addOtherFiles(self, vars):
        if vars: self.addData('OTHER_FILES', '+=', vars)
    def addQt(self, vars):
        if vars: self.addData('QT', '+=', vars)

    def addDefines(self, vars):
        if type(vars) is str:
            self.addData('DEFINES', '+=', vars)
        if type(vars) is dict:
            for key, value in vars.items():
                string = r'"{}=\"\\\"{}\\\"\""'.format(key, value)
                self.addData('DEFINES', '+=', string)
        
    def addConfig(self, vars):
        self.addData('CONFIG', '+=', vars)
    def delConfig(self, vars):
        self.addData('CONFIG', '-=', vars)
        
    def addIncludePath(self, path):
        if type(path) is not str:
            path = pjoin(path)
        self.addData('INCLUDEPATH', '+=', path)

    def addLibs(self, path, prefix, lib):
        if type(path) is not str:
            path = pjoin(path)
        string = '-L{} -l{}{}'.format(path, prefix, lib)
        self.addData('LIBS', '+=', string)

    def addPostTargetDeps(self, path, prefix, lib): # PRE_TARGETDEPS?
        if type(path) is not str:
            path = pjoin(path)
        if (Os() == 'mac'): # or even unix... what about win? .a will not fit
            string = '{}/lib{}{}.a'.format(path, prefix, lib)
            self.addData('POST_TARGETDEPS', '+=', string)

    def addQmakeCxxFlags(self, vars): # QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic
        self.s += '!msvc { '
        self.addData('QMAKE_CXXFLAGS', '+=', vars, False)
        self.s += ' }\n'

    def addCppVersion(self, ver): 
        self.addConfig(ver)

    def addBuildType(self, debug_dir_name, profile_dir_name, release_dir_name):
        #string  = 'CONFIG(debug, debug|release) {{ BUILD_TYPE = {} }}\n'
        #string += 'CONFIG(force_debug_info) {{ BUILD_TYPE = {} }}\n'
        #string += 'CONFIG(release, debug|release) {{ BUILD_TYPE = {} }}\n'
        string  = 'CONFIG(debug, debug|release)  {{ BUILD_TYPE = {} }}\n'
        string += 'else:CONFIG(force_debug_info) {{ BUILD_TYPE = {} }}\n'
        string += 'else                          {{ BUILD_TYPE = {} }}\n'
        self.s += string.format(debug_dir_name, profile_dir_name, release_dir_name)

    def addDepends(self, app, lib):
        self.s += '{}.depends = {}\n'.format(app, lib)

    def addIcon(self, path):
        if type(path) is not str:
            path = pjoin(path)
        if (Os() == 'mac'):
            self.addData('ICON', '=', '{}.icns'.format(path))
        if (Os() == 'win'):
            self.addData('RC_ICONS', '=', '{}.ico'.format(path))
            self.addData('RC_FILE', '=', '{}.rc'.format(path))
        if (Os() == 'lin'):
            self.s += 'isEmpty(PREFIX) { PREFIX = /usr/local }\n'
            self.s += 'shortcutfiles.files = Resources/Desktop/davinci.desktop\n'
            self.s += 'shortcutfiles.path = $$PREFIX/share/applications/\n'
            self.s += 'INSTALLS += shortcutfiles\n'

    def out(self):
        return self.s
        
    def save(self, path):
        WriteStringToFile(path, self.s, '.pro')
        
        
    


