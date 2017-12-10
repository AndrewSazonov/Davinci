#!/usr/bin/env python3

from functions import *

# ChangelogArticle
class ChangelogArticle:
    
    __slots__ = ['version', 'date', 'list']
    
    def __init__(self, version='', date='', list=[]):
        self.version = version
        self.date = date
        self.list = list

# Changelog
class Changelog:
    def __init__(self, title=''):
        #self.app = 'Davinci'
        self.title = title
        self.last_article = ChangelogArticle()
        self.all_articles = []
        self.idx = 0
        self.add_articles()

    def __iter__(self):
        return self
    
    def __next__(self):
        self.idx += 1
        try:
            return self.all_articles[self.idx-1]
        except IndexError:
            self.idx = 0
            raise StopIteration

    def __getitem__(self, idx):
        return self.all_articles[idx]

    def version(self):
        return self.last_article.version

    def date(self):
        return self.last_article.date
        #return ConvertDate(self.last_article.date, '%Y-%m-%d', '%d %b %Y')

    def add(self, version, date, list):
        date = ConvertDate(date, '%Y-%m-%d', '%d %b %Y')
        self.last_article = ChangelogArticle(version, date, list)
        self.all_articles.insert(0, self.last_article)
        #self.all_articles.append(self.last_article)

    def add_articles(self):
        self.add(version="1.0.0",
                 date="2017-12-09",
                 list=['First release'])

        #self.add(version="1.0.1",
        #         date="2017-??-??",
        #         list=['Fixes a crash when closing a tab while connecting',
        #               'Improves macOS 10.13 High Sierra compatibility...',
        #               'Adds Polish localization',
        #               'Numerous other fixes and improvements'])

        #self.add(version="1.0.2",
        #         date="2017-??-??",
        #         list=['Fixes a bug to build with CI',
        #               'Fixes an issue with editing files on mounted remote volumes resulted in adding NUL to the end of the document',
        #               'Fixes a crash when closing a tab while connecting'])

        #self.add(version="1.0.3",
        #         date="2017-??-??",
        #         list=['Fixes a bug to build with CI',
        #               'Fixes an issue with editing files on mounted remote volumes resulted in adding NUL to the end of the document',
        #               'Fixes a crash when closing a tab while connecting'])



