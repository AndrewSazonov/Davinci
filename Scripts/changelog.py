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
                 list=[ 'First release'])

        self.add(version="1.0.1",
                 date="2018-01-11",
                 list=[ 'Adds support of the new input format: HEiDi at MLZ instrument log data',
                        'Fixes an issue with automatic determination of the peak and background parameters',
                        'Fixes a bug with cursor position change in the input data viewer when go through the scans',
                        'Fixes a crash when open a new file',
                        'Fixes a sidebar issue with the tabs width'])

        self.add(version="1.0.2",
                 date="2018-01-27",
                 list=[ 'Fixes a bug with the update function of the installer parogram'])

        self.add(version="1.0.3",
                 date="2018-03-13",
                 list=[ 'Fixes issue #2, Not reading new files with Oxford magnet on POLI'])

        self.add(version="1.0.4",
                 date="2018-03-28",
                 list=[ 'Adds support of the new input format for POLI: polarized NICOS data with adet',
                        'Adds user manual and report issue links to the Help menu',
                        'Adds a program icon and desktop file for the Linux version'])

        self.add(version="1.0.5",
                 date="2018-05-30",
                  list=[ 'Adds calculation of the direction cosines (Issue #3)', # update manual
                         'Adds reading of Psi angle from the HEiDi instrument data collection files', # update manual
                         'Adds natural sort order and sort by date and time to the table widgets', # update manual
                         'Adds ShelX output with real (non-integer) Miller indices hkl',
                         'Fixes calculation of the Phi angle in the 4-circle geometry',
                         'Fixes an issue with Tbar/D9 output (theta + temperature, psi, fwhm)',
                         'Fixes some minor issues'])

        self.add(version="1.0.6",
                 date="2018-07-07",
                  list=[ 'Adds reading of Psi angle from the NICOS instrument data collection files (Issue #5)', # update manual
                         'Adds UMWEG output for the calculation of multiple-diffraction patterns']) # update manual
