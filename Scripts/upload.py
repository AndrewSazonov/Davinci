#!/usr/bin/env python3

import ftputil
import sys
import os

# Upload directory to ftp
def UploadDir(server, username, password, local_path, remote_dir):
    with ftputil.FTPHost(server, username, password) as ftp:
        # Remove remote dir, including subdirs
        try:
            print(101)
            ftp.rmtree(remote_dir)
        except:
            print('Failed to found: {}'.format(remote_dir))
        # Create dir
        print(102)
        ftp.mkdir(remote_dir)
        # Go to this dir
        print(103)
        ftp.chdir(remote_dir)
        # Upload dir, including subdirs, recursively
        def UploadThis(path):
            print(105)
           # Go via all the file(s) or dir(s)
            for name in os.listdir(path):
                full_path = os.path.join(path, name)
                print(106,full_path)
                # Try to upload the file found
                if os.path.isfile(full_path):
                    try:
                        ftp.upload(full_path, name)
                        print('Uploaded file: {}'.format(name))
                    except:
                        print('Failed to upload: {}'.format(name))
                # Call function recursively for the dir found
                elif os.path.isdir(full_path):
                    print(107,full_path)
                    #print('Folder to upload: {}'.format(name))
                    ftp.mkdir(name)
                    ftp.chdir(name)
                    UploadThis(full_path)
            # Go one level up
            ftp.chdir('..')
        print(104)
        UploadThis(local_path)

# Upload file to ftp
def UploadFile(server, username, password, local_path, remote_dir):
    print(201)
    name = os.path.basename(local_path)
    print(202,name)
    remote_path = '{}/{}'.format(remote_dir, name)
    print(203,remote_path)
    with ftputil.FTPHost(server, username, password) as ftp:
        # Create remote dir if it doesn't exist
        try:
            print(204,remote_dir)
            ftp.mkdir(remote_dir)
        except:
            print(205)
            pass
        # Try to upload the file
        try:
            print(206)
            ftp.upload(local_path, remote_path)
            print('Uploaded file: {}'.format(name))
        except:
            print('Failed to upload: {}'.format(name))

# Get script arguments
server = sys.argv[1]
username = sys.argv[2]
password = sys.argv[3]
local_path = sys.argv[4]
remote_dir = sys.argv[5]

print(301,server)
print(302,username)
print(303,local_path)
print(304,remote_dir)

# Upload file or folder
if os.path.isfile(local_path):
    UploadFile(server, username, password, local_path, remote_dir)
elif os.path.isdir(local_path):
    UploadDir(server, username, password, local_path, remote_dir)


