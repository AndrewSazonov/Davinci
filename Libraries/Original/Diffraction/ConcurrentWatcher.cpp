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
#include "Macros.hpp"

#include "ProgressDialog.hpp"
#include "Scan.hpp"
#include "ScanArray.hpp"

#include "ConcurrentWatcher.hpp"

/*!
\class As::ConcurrentWatcher

\brief The ConcurrentWatcher class inherits from QFutureWatcher<void>. It allows monitoring
a QFuture computation started via QtConcurrent::map. During the processing, it shows the
progress dialog by As::ProgressDialog.

\inmodule Diffraction
*/

As::ConcurrentWatcher::ConcurrentWatcher()
{
    // Default number of threads to be used in parallel computation
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());
}

void As::ConcurrentWatcher::startComputation(const QString &type,
                                         ScanArray *scans)
{
    ADEBUG << "--------> start" << type;


    // Default sequence for QtConcurrent::map
    QVector<int> sequence(scans->size());
    for (int i = 0; i < scans->size(); ++i)
        sequence[i] = i;

    // Use lambda function, because QtConcurrent::map doesn't accept class member functions.
    // In QtConcurrent::run it was possible to do providing 2 parameters...
    std::function<void (int)> func;

    // Work type dependent parameters
    if (type == "extract") {
        QThreadPool::globalInstance()->setMaxThreadCount(1);
        const int size = scans->m_inputFilesContents.first.size();
        sequence.resize(size);
        for (int i = 0; i < size; ++i)
            sequence[i] = i;
        func = [&] (const int i) { scans->extractDataFromFile(i); }; }

    else if (type == "fill")
        func = [&] (const int i) { scans->fillMissingDataArray(i); };

    else if (type == "index") // join index with pretreat
        func = [&] (const int i) { scans->indexSinglePeak(i); scans->preTreatSingleScan(i); };

    //else if (type == "pretreat")
    //    func = [&] (const int i) { scans->preTreatSingleScan(i); };

    else if (type == "treat")
        func = [&] (const int i) { scans->treatSingleScan(i); };

    else
        return;

    // Start the computation
    setFuture(QtConcurrent::map(sequence, func));

    // Emit the computation started signal
    emit started();

    // Wait until the computation is finished
    waitForFinished();

    ADEBUG << "--------> finished" << type;
}





/*!
Constructs a watcher with the given \a parent. Calls \a function once for each item in \a sequence.
The \a function is passed a reference to the item, so that any modifications done to the item will
appear in \a sequence.
*/
As::ConcurrentWatcher::ConcurrentWatcher(const QString &type,
                                         ScanArray *scans,
                                         QWidget *parent)
    : QFutureWatcher<void>(parent)
{
    ADEBUG << "--------> start" << type;

    // Default number of threads to be used in parallel computation
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    // Default sequence for QtConcurrent::map
    QVector<int> sequence(scans->size());
    for (int i = 0; i < scans->size(); ++i)
        sequence[i] = i;

    // Use lambda function, because QtConcurrent::map doesn't accept class member functions.
    // In QtConcurrent::run it was possible to do providing 2 parameters...
    std::function<void (int)> func;

    // Work type dependent parameters
    if (type == "extract") {
        QThreadPool::globalInstance()->setMaxThreadCount(1);
        const int size = scans->m_inputFilesContents.first.size();
        sequence.resize(size);
        for (int i = 0; i < size; ++i)
            sequence[i] = i;
        func = [&] (const int i) { scans->extractDataFromFile(i); }; }

    else if (type == "fill")
        func = [&] (const int i) { scans->fillMissingDataArray(i); };

    else if (type == "index") // join index with pretreat
        func = [&] (const int i) { scans->indexSinglePeak(i); scans->preTreatSingleScan(i); };

    else if (type == "pretreat")
        func = [&] (const int i) { scans->preTreatSingleScan(i); };

    else if (type == "treat")
        func = [&] (const int i) { scans->treatSingleScan(i); };

    else
        return;

    // Find parent progress dialog and connect it with 'this' watcher
    ///auto dialog = parent->findChild<As::ProgressDialog*>();
    ///if (dialog) {
        //connect(this, &QFutureWatcher<void>::finished,             dialog, &As::ProgressDialog::reset);
        ///connect(this, &QFutureWatcher<void>::progressRangeChanged, dialog, &As::ProgressDialog::setRange);
        ///connect(this, &QFutureWatcher<void>::progressValueChanged, dialog, &As::ProgressDialog::setValue);
        //--connect(this, &QFutureWatcher<void>::started, dialog, &As::ProgressDialog::exec); }

    // Start the computation
    setFuture(QtConcurrent::map(sequence, func));

    // Display the progress dialog and start the event loop
    ///if (dialog)
    ///    dialog->exec();

    // Wait until the computation is finished
    waitForFinished();

    ADEBUG << "--------> finished" << type;
}

/*!
Destroys the watcher.
*/
As::ConcurrentWatcher::~ConcurrentWatcher() {}
