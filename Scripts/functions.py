#!/usr/bin/env python3

import os
import sys
import shutil
from datetime import datetime

##############
# My functions
##############

#def pjoin(args, **kwargs):
    #return os.path.join(*args, **kwargs).replace(os.path.sep, '/')
def pjoin(*args):
    l = []
    for a in args:
        if type(a) is list:
            l.append(os.path.join(*a))
        else:
            l.append(a)
    p = os.path.normpath(os.path.join(*l))
    return p.replace(os.path.sep, '/') # windows and linux supports '/' separator

def ConvertDate(date, from_format, to_fromat):
    return datetime.strftime(datetime.strptime(date, from_format), to_fromat)

def GetFilePath(dir_name, file_name):
    script_path = os.path.dirname(os.path.abspath(__file__))
    return pjoin(script_path, dir_name, file_name)

def ReadFileToString(path):
    with open(path, 'r') as f:
        read_data = f.read()
    f.closed
    return read_data

def ReadFileToString(dir_name, file_name):
    path = GetFilePath(dir_name, file_name)
    with open(path, 'r') as f:
        read_data = f.read()
    f.closed
    return read_data



#def ScriptPath():
#    return os.path.dirname(os.path.abspath(__file__))



def WriteStringToFile(path, string, ext=''):
    if type(path) is not str:
        path = pjoin(path)
    if ext:
        path += ext
    with open(path, 'w') as f:
        f.write(string)
    f.closed
    #print('OK. Created file: {0}'.format(path))

def CreateDir(path):
    path = pjoin(path)
    if not os.path.exists(path):
        os.makedirs(path)

def RemoveDir(path):
    path = pjoin(path)
    if os.path.exists(path):
        if os.path.isdir(path) == True:
            shutil.rmtree(path)

def RemoveThis(path):
    path = pjoin(path)
    if os.path.exists(path):
        if os.path.isdir(path) == True:
            shutil.rmtree(path)
        if os.path.isfile(path) == True:
            os.remove(path)

def RecreateDir(path):
    RemoveDir(path)
    CreateDir(path)

def Copy(src, dst):
    #src = pjoin(ScriptPath(), *src)
    #dst = pjoin(ScriptPath(), *dst)
    src = pjoin(src)
    #dst = pjoin(dst)
    #print('*')
    #print(src, dst)
    #print(os.path.basename(src))
    #print(pjoin(dst, os.path.basename(src)))
    #print('*')
    dst = pjoin(dst, os.path.basename(src))
    if os.path.isdir(src) == True:
        if os.path.exists(dst):
            shutil.rmtree(dst)
        shutil.copytree(src, dst, symlinks=True)
        #shutil.copytree(src, dst, symlinks=False)
    else:
        #dst = pjoin(dst, os.path.basename(src))
        #print(src, dst)
        shutil.copy2(src, dst)

def GetFileList(dir):
    dir = pjoin(dir)
    return [file for file in os.listdir(dir) if os.path.isfile(pjoin(dir, file))]

def GetSubDirList(dir):
    dir = pjoin(dir)
    return [subdir for subdir in os.listdir(dir) if os.path.isdir(pjoin(dir, subdir))]

def GetFileAndSubDirList(dir):
    dir = pjoin(dir)
    return [fad for fad in os.listdir(dir)]

def GetSelectedFileList(dir, ext):
    list = []
    if type(ext) is str:
        exts = ext.split()
    # search files in the dir
    files = GetFileList(dir)
    for ext in exts:
        list += [file for file in files if os.path.splitext(file)[1] == ext]
    # search files in the 1st level subdirs
    subdirs = GetSubDirList(dir)
    for subdir in subdirs:
        files = GetFileList(dir + [subdir])
        for ext in exts:
            list += ['{}/{}'.format(subdir, file) for file in files if os.path.splitext(file)[1] == ext]
    return list

def GetAllSelectedFiles(path, ext):
    list = []
    if type(ext) is str:
        exts = ext.split()
    path = pjoin(path)
    def GetFileList(path):
        for name in os.listdir(path):
            full_path = os.path.join(path, name)
            if os.path.isfile(full_path):
                for ext in exts:
                    if os.path.splitext(full_path)[1] == ext:
                        list.append(full_path)
            elif os.path.isdir(full_path):
                GetFileList(full_path)
    GetFileList(path)
    return list

def GetDirTree(path):
    return [path for path, dirs, files in os.walk(pjoin(path))]

#def UpdateEnvironVars():
#    os.environ['PATH'] += os.pathsep + QT_BIN_PATH

def Os():
    if sys.platform.startswith('win'):
        return('win')
    if sys.platform.startswith('darwin'):
        return('mac')
    if sys.platform.startswith('linux'):
        return('lin')
    return('unknown')

def OsSpecificGui(name):
    if Os() == 'win':
        return('{}.exe'.format(name))
    if Os() == 'mac':
        return('{}.app'.format(name))
    if Os() == 'lin':
        return('{}'.format(name))

def OsSpecificCli(name):
    if Os() == 'win':
        return('{}.exe'.format(name))
    if Os() == 'mac':
        return('{}'.format(name))
    if Os() == 'lin':
        return('{}'.format(name))

