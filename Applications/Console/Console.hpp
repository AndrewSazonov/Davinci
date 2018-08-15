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

#ifndef AS_CONSOLE_HPP
#define AS_CONSOLE_HPP

#include <QCommandLineParser>

#include <QObject>

#include "ScanArray.hpp"

QT_BEGIN_NAMESPACE

class QAbstractItemModel;
class QCoreApplication;
class QString;
class QStringList;
class QTextStream;
template<typename> class QFutureWatcher;

QT_END_NAMESPACE

namespace As { //AS_BEGIN_NAMESPACE

class Console : public QObject {
    Q_OBJECT

  public:
    Console(QObject* parent = Q_NULLPTR);
    ~Console();

    void createCommandLineParser(QCoreApplication* app);

    bool checkRequiredOptionsAreProvided(const QStringList& optionList) const;
    bool setOutputFileExt();
    bool openFiles();
    bool loadData(const QStringList& filePathList);
    bool detectInputFilesType();

    void concurrentRun(const QString& type,
                       As::ScanArray* scans) const;
    void exportOutputTable();

    void printMessage(const QString& message,
                      const QString& arg = QString()) const;
    void printMessageList(const QStringList& messageList) const;

    void printAppDescription() const;
    void printProgramOutput() const;

    QString applicationDescription() const;
    QString outputFileFormat() const;
    QString outputFileName() const;
    QString outputFileExt() const;
    QString outputFileNameWithExt() const;

  public slots:
    void run();

  signals:
    void finished() const;

  private:
    QCommandLineParser m_parser;
    As::ScanArray m_scans;
    QString m_outputFileExt; };

} //AS_END_NAMESPACE

#endif // AS_CONSOLE_HPP
