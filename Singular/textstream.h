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

#ifndef TEXTSTREAM_H
#define TEXTSTREAM_H

#include <QObject>

#include "defines.h"

class TextStream : public QObject
{
    Q_OBJECT

public_construct:
    explicit TextStream(QObject *parent = 0);

private_methods:
    void output(const QString &message, const int verbose) const;

public slots:
    void get_text(const QString &message = "") const;

signals:
    void console(const QString &message) const;

};

#endif // TEXTSTREAM_H
