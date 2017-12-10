/*
 * Davinci, a software for the single-crystal diffraction data reduction.
 * Copyright (C) 2015-2017 Andrew Sazonov
 *
 * This file is part of Davinci.
 *
 * Davinci is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Davinci is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Davinci.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>

#include "Window.hpp"

int main(int argc, char *argv[])
{
    // Create application, passing to it the arguments coming from the command line
    QApplication app(argc, argv);

    // Set application info
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName(APP_OWNER);
    app.setOrganizationDomain(APP_DOMAIN);
    //app.setApplicationDisplayName(APP_NAME); // by default: the application name

    // Enables support for high-dpi images
    //app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Icons are no longer shown in menus
    app.setAttribute(Qt::AA_DontShowIconsInMenus);

    // Create main window
    As::Window mainWindow;
    //mainWindow.show();
    Q_UNUSED(mainWindow)

    // Start event loop
    return app.exec();
}

