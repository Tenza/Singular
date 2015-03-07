/*
 * Singular
 * Copyright (C) 2015 Filipe Carvalho
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUDIOOUTPUTSURFACE_H
#define AUDIOOUTPUTSURFACE_H

#include <QIODevice>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>

#include "defines.h"

class AudioOutputSurface : public QIODevice
{
    Q_OBJECT

public_construct:
    explicit AudioOutputSurface(const int new_id, const QAudioDeviceInfo new_device_info, const QAudioFormat new_device_format, QObject *parent = 0);
    ~AudioOutputSurface();

public_methods:
    void start();
    void stop();

protected_methods:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);

private_methods:
    void output(const QString &message, const int verbose) const;

private_members:
    int id;

private_data_members:
    QAudioDeviceInfo device_info;
    QAudioFormat device_format;
    QAudioOutput *audio_output;

private slots:
    void notify();
    void stateChanged(QAudio::State state);

signals:
    void console(const QString &message) const;
    void speakers_data(const int level) const;

};

#endif // AUDIOOUTPUTSURFACE_H
