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

#ifndef SINGULAR_H
#define SINGULAR_H

#include <QMainWindow>

#include "defines.h"
#include "sensors.h"

namespace Ui
{
    class Singular;
}

class Singular : public QMainWindow
{
    Q_OBJECT

public_construct:
    explicit Singular(QWidget *parent = 0);
    ~Singular();

private_methods:
    void load_settings();
    void save_settings();

private_data_members:
    Ui::Singular *ui;
    Sensors *sensors;

signals:
    void get_text(const QString &message) const;

public slots:
    void console(const QString &message) const;
    void add_camera(const QString camera_name, QWidget *camera, const bool selected) const;
    void add_microphone(const QString microphone_name, QWidget *microphone = 0, const bool selected = false) const;

private slots:
    void on_cb_cameras_currentIndexChanged(int index);
    void on_cb_microphones_currentIndexChanged(int index);
    void on_txt_input_textChanged();

};

#endif // SINGULAR_H
