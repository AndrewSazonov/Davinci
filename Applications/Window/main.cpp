/*
    Davinci, a software for the single-crystal diffraction data reduction.
    Copyright (C) 2015-2017 Andrew Sazonov

    This file is part of Davinci.

    Davinci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Davinci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QTimer>

#include <QDebug>

#include "Window.hpp"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    app.setApplicationName(APP_NAME);
    //app.setApplicationDisplayName(APP_NAME); // by default: the application name
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName(APP_OWNER);
    app.setOrganizationDomain(APP_DOMAIN);

    //app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setAttribute(Qt::AA_DontShowIconsInMenus);

    As::Window mainWindow; //Q_UNUSED(mainWindow)
    QObject::connect(&mainWindow, &As::Window::quit, &app, &QApplication::quit, Qt::QueuedConnection);
    mainWindow.show();

    // Call the mainWindow::show from the application event loop
    //QTimer::singleShot(0, &mainWindow, &As::Window::show);

    return app.exec(); }

