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

#include "textstream.h"
#include "output.h"

#include <QTimer>

#define use_timer false

TextStream::TextStream(QObject *parent) :
    QObject(parent)
{
    connect(this, SIGNAL(console(const QString&)), parent, SIGNAL(console(const QString&)));
    connect(parent, SIGNAL(get_text(const QString&)), this, SLOT(get_text(const QString&)));

    if(use_timer)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(get_text()));

        timer->start(1000);
    }
}

void TextStream::get_text(const QString &message) const
{
    output("Text Stream: " + message, 1);
}

void TextStream::output(const QString &message, const int &verbose) const
{
    if(Output::get_verbose() >= verbose)
    {
        QVariantHash data;
        data.insert("message", message);
        data.insert("verbose", verbose);
        data.insert("load_thread_id", true);

        QString print = Output::builder(data);

        if(!print.isEmpty())
        {
            emit console(print);
        }
    }
}
