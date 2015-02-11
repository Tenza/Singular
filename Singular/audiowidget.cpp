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

#include "audiowidget.h"

#include "output.h"

/**
 * @brief AudioWidget::AudioWidget
 *      Starts the audio widget, sets the size and color.
 * @param parent
 *      To parent this class and to use signals and slots.
 */
AudioWidget::AudioWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(console(const QString&)), parent, SIGNAL(console(const QString&)));
    output("Audio device started.", 1);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    level = 0;
    setMinimumHeight(30);
    setMinimumWidth(200);
}

/**
 * @brief AudioWidget::set_level
 *      Saves the new volume level and updates the widget.
 * @param level
 *      The new volume level from the surface.
 */
void AudioWidget::set_level(double level)
{
    this->level = level;
    update();
}

/**
 * @brief AudioWidget::paintEvent
 *      Paints the widget with the volume set in the set_level function.
 * @param event
 */
void AudioWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setPen(Qt::black);

    painter.drawRect(0, 0, painter.viewport().width() - 1, painter.viewport().height() - 1);

    if (level > 0.0)
    {
        int volume = (level * painter.viewport().width() - 1) / 100;

        painter.fillRect(1, 1, volume, painter.viewport().height() - 2, Qt::red);
    }
}

void AudioWidget::output(const QString &message, const int &verbose) const
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
