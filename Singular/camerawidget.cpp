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

#include "camerawidget.h"
#include "output.h"

#include <QResizeEvent>

/**
 * @brief CameraWidget::CameraWidget
 *      Connect the widget with the console.
 * @param parent
 *      To parent this class and to use signals and slots.
 */
CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(console(const QString&)), parent, SIGNAL(console(const QString&)));
    output("Camera widget started.", 1);
}

/**
 * @brief CameraWidget::update_frame
 *      Saves the new frame and updates the widget.
 * @param new_frame
 *      The new frame from the surface.
 */
void CameraWidget::update_frame(const QImage new_frame)
{
    frame = new_frame;
    update();
}

/**
 * @brief CameraWidget::paintEvent
 *      Displays the frame to the user.
 * @param event
 *      Used to get the size of the widget.
 */
void CameraWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //Paints the frame and handles resize on the widget, not the frame.
    painter.drawImage(event->rect(), frame);

    //Shows a border with the true size of the frame.
    painter.setBrush(Qt::CrossPattern);
    painter.setPen(Qt::red);
    painter.drawRect(0, 0, frame.width(), frame.height());
}

void CameraWidget::output(const QString &message, const int &verbose) const
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
