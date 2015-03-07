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

#ifndef SENSORS_H
#define SENSORS_H

#include <QObject>

#include "defines.h"
#include "textstream.h"
#include "audiowidget.h"
#include "camerawidget.h"
#include "audioinputsurface.h"
#include "audiooutputsurface.h"

class Sensors : public QWidget
{
    Q_OBJECT

public_construct:
    explicit Sensors(QWidget *parent = 0);

public_methods:
    void start_cameras();
    void start_textstream();
    void start_microphones();
    void start_speakers();

    void update_microphones(const int id);

private_data_members:
    QList<CameraWidget*> camera_widgets;
    QList<CameraSurface*> camera_surfaces;

    QList<AudioWidget*> audio_input_widgets;
    QList<AudioInputSurface*> audio_input_surfaces;

    QList<AudioWidget*> audio_output_widgets;
    QList<AudioOutputSurface*> audio_output_surfaces;

    TextStream* text;

public slots:
    void image_data(const int id, const QImage new_frame) const;
    void microphone_data(const int id, const char *data, const int level) const;
    void speakers_data(const int id, const int level) const;

signals:
    void console(const QString &message) const;
    void add_camera(const QString &camera_name, QWidget *camera = 0, const bool selected = false) const;
    void add_microphone(const QString &microphone_name, QWidget *microphone = 0, const bool selected = false) const;
    void get_text(const QString &message) const;

};

#endif // SENSORS_H
