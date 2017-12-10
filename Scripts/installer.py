#!/usr/bin/env python3

import yattag
from variables import *

def Indent(text):
    return yattag.indent(text, indentation = ' '*2, newline = '\r\n')

def Config(repo): # http://doc.qt.io/qtinstallerframework/ifw-globalconfig.html
    doc, tag, text = yattag.Doc().tagtext()
    with tag('Installer'):
        with tag('Name'):                           text(APP_NAME)
        with tag('Version'):                        text(INSTALLER_VERSION)
        with tag('Title'):                          text(INSTALLER_TITLE)
        with tag('Publisher'):                      text(APP_PUBLISHER)
        with tag('ProductUrl'):                     text(APP_URL)
        with tag('StartMenuDir'):                   text(APP_NAME)
        with tag('TargetDir'):                      text(TARGET_DIR)
        with tag('MaintenanceToolName'):            text(MAINTAINER_NAME)
        with tag('RemoteRepositories'):
            with tag('Repository'):
                with tag('Url'):                    text('{}/{}'.format(REPOS_DIR, repo))
        with tag('ControlScript'):                  text(CONTROLSCRIPT_FILE_NAME)
        #with tag('RepositorySettingsPageVisible'):  text('false')
        with tag('InstallActionColumnVisible'):     text('false')
        with tag('AllowSpaceInPath'):               text('true')
    return Indent(doc.getvalue())

def Package(): # http://doc.qt.io/qtinstallerframework/ifw-component-description.html
    doc, tag, text = yattag.Doc().tagtext()
    with tag('Package'):
        with tag('DisplayName'):        text(APP_NAME)
        with tag('Version'):            text(APP_VERSION)
        with tag('ReleaseDate'):        text(ConvertDate(APP_RELEASE_DATE, '%d %b %Y', '%Y-%m-%d'))
        #with tag('UpdateText'):         text(update_text)
        with tag('Description'):        text(APP_DESCRIPTION)
        with tag('Script'):             text(INSTALLSCRIPT_FILE_NAME)
        with tag('Licenses'):
            doc.stag('License', ('name', LICENSE_NAME), ('file', LICENSE_FILE_NAME))
        #with tag('UserInterfaces'):
        #    with tag('UserInterface'):  text(RUNAPP_FILE_NAME)
        #with tag('RequiresAdminRights'):text('true')
        with tag('Default'):            text('true')
        with tag('Essential'):          text('true')
        with tag('ForcedInstallation'): text('true')
    return Indent(doc.getvalue())


