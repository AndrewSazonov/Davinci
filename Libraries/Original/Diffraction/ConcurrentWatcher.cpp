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

#include <QThread>
#include <QThreadPool>
#include <QtConcurrent>

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

/*!
    Constructs a default watcher.
*/
As::ConcurrentWatcher::ConcurrentWatcher() {
    // Use default number of threads in parallel computation
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount()); }

/*!
    Starts parallel computation of type \a type on the scan array \a scans.
*/
void As::ConcurrentWatcher::startComputation(const QString& type,
                                             ScanArray* scans) {
    ADEBUG << "--------> parallel computation are started for:" << type;

    // Default sequence for QtConcurrent::map
    QVector<int> sequence(scans->size());
    for (int i = 0; i < scans->size(); ++i) {
        sequence[i] = i; }

    // Use lambda function, because QtConcurrent::map doesn't accept class member functions.
    // In QtConcurrent::run it was possible by providing 2 parameters...
    std::function<void (int)> func;

    // Computation type dependent parameters
    if (type == "extract") {
        QThreadPool::globalInstance()->setMaxThreadCount(1);
        const int size = scans->m_inputFilesContents.first.size();
        sequence.resize(size);
        for (int i = 0; i < size; ++i) {
            sequence[i] = i; }
        func = [&] (const int i) {
            scans->extractDataFromFile(i); }; }

    else if (type == "fill")
        func = [&] (const int i) {
        scans->fillMissingDataArray(i); };

    else if (type == "index") // join "index" with "pretreat"
        func = [&] (const int i) {
        scans->indexSinglePeak(i);
        scans->preTreatSinglePeak(i); };

    else if (type == "treat")
        func = [&] (const int i) {
        scans->treatSinglePeak(i); };

    else {
        return; }

    // Start the computation, force to emit the started signal and wait
    setFuture(QtConcurrent::map(sequence, func));
    emit started();
    waitForFinished();

    ADEBUG << "--------> parallel computation are finished." << type; }

/*!
    Destroys the watcher.
*/
As::ConcurrentWatcher::~ConcurrentWatcher() {}
