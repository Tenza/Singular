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

#ifndef CAMERASURFACE_H
#define CAMERASURFACE_H

#include <QCamera>
#include <QCameraInfo>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoSurface>

#include "defines.h"

class CameraSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public_construct:
    explicit CameraSurface(const int new_id, QCameraInfo new_camera_info, QObject *parent = 0);

public_methods:
    void start() const;

    bool start(const QVideoSurfaceFormat &format);
    void stop();
    bool present(const QVideoFrame &frame);

    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

private_methods:
    void output(const QString &message, const int verbose) const;

private_members:
    int id;

private_data_members:
    QCamera* camera;
    QCameraInfo camera_info;

private slots:
    void stateChanged(QCamera::State state);

signals:
    void console(const QString &message) const;
    void image_data(const int id, const QImage new_frame) const;

};

#endif // CAMERASURFACE_H
