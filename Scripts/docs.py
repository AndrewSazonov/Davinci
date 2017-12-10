#!/usr/bin/env python3

import os
from functions import *

class Docs:
    
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
    
    def addText(self, text):
        self.s += '{}\n'.format(text)
    
    def out(self):
        return self.s
        
    def save(self, path, ext=''):
        WriteStringToFile(path, self.s, ext)
        
        
    


