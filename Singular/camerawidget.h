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

#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QPainter>

#include "defines.h"
#include "camerasurface.h"

class CameraWidget : public QWidget
{
    Q_OBJECT

public_construct:
    explicit CameraWidget(QWidget *parent = 0);

public_methods:
    void update_frame(const QImage new_frame);

protected_methods:
    void paintEvent(QPaintEvent *event);

private_methods:
    void output(const QString &message, const int verbose) const;

private_data_members:
    QImage frame;

signals:
    void console(const QString &message) const;

};

#endif // CAMERAWIDGET_H
