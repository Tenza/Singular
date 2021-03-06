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

#include "sensors.h"
#include "helper.h"

#include <QCameraInfo>
#include <QAudioDeviceInfo>

/**
 * @brief Sensors::Sensors
 *      Connects the data from the sensors to the widgets.
 *      Starts all the available sensors.
 * @param parent
 *      Parent this class and to use signals and slots.
 */
Sensors::Sensors(QWidget *parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(add_camera(QString, QWidget*, bool)), parent, SLOT(add_camera(QString, QWidget*, bool)));
    connect(this, SIGNAL(add_microphone(QString, QWidget*, bool)), parent, SLOT(add_microphone(QString, QWidget*, bool)));
    connect(this, SIGNAL(console(QString)), parent, SLOT(console(QString)));
    connect(parent, SIGNAL(get_text(QString)), this, SIGNAL(get_text(QString)));

    start_cameras();
    start_textstream();
    start_microphones();
//    start_speakers();
}

/**
 * @brief Sensors::start_cameras
 *      Initializes the available cameras.
 *      Initializes the widgets for the cameras.
 *      Initializes the surfaces to transmit the data.
 *      Adds the camera widget to the UI.
 */
void Sensors::start_cameras()
{
    QString default_device = QCameraInfo::defaultCamera().deviceName();
    QList<QCameraInfo> cameras_info = QCameraInfo::availableCameras();

    for (int i = 0; i < cameras_info.size(); i++)
    {
        camera_widgets.append(new CameraWidget(this));
        camera_surfaces.append(new CameraSurface(i, cameras_info.at(i), this));
        camera_surfaces.at(i)->start();

        if(cameras_info.at(i).deviceName() == default_device)
        {
            emit add_camera(cameras_info.at(i).deviceName(), camera_widgets.at(i), true);
        }
        else
        {
            emit add_camera(cameras_info.at(i).deviceName(), camera_widgets.at(i));
        }
    }
}

/**
 * @brief Sensors::image_data
 *      Get image data from the image sensors.
 *      Updates the widget and sends the data to the network.
 * @param id
 *      ID of the surface, this is needed to update the correct camerawidget.
 * @param new_frame
 *      The new frame.
 */
void Sensors::image_data(const int id, const QImage new_frame) const
{
    camera_widgets.at(id)->update_frame(new_frame);
}

/**
 * @brief Sensors::start_microphones
 *      Initializes only the default microphone and one widget.
 *      Although this is a QList only a single device will be active, probably change this in the future.
 *      Adds the camera widget to the UI.
 */
void Sensors::start_microphones()
{ 
    QString default_device = QAudioDeviceInfo::defaultInputDevice().deviceName();
    QList<QAudioDeviceInfo> audio_input_info = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    for (int i = 0; i < audio_input_info.size(); i++)
    {
        if(audio_input_info.at(i).deviceName() == default_device)
        {
            audio_input_widgets.append(new AudioWidget(this));
            audio_input_surfaces.append(new AudioInputSurface(audio_input_surfaces.size(), audio_input_info.at(i), audio_input_info.at(i).preferredFormat(), this));
            audio_input_surfaces.last()->start();

            emit add_microphone(audio_input_info.at(i).deviceName(), audio_input_widgets.last(), true);
        }
        else
        {
            emit add_microphone(audio_input_info.at(i).deviceName());
        }
    }
}

/**
 * @brief Sensors::update_microphones
 *      Since we only have one device initialized at any given moment, this method will remove the current and create the new device.
 * @param id
 */
void Sensors::update_microphones(const int id)
{
    QList<QAudioDeviceInfo> audio_input_info = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    audio_input_surfaces.last()->stop();
    audio_input_surfaces.last()->deleteLater();
    audio_input_surfaces.clear();

    audio_input_surfaces.append(new AudioInputSurface(audio_input_surfaces.size(), audio_input_info.at(id), audio_input_info.at(id).preferredFormat(), this));
    audio_input_surfaces.last()->start();
}

/**
 * @brief Sensors::microphone_data
 *      Recives the data from the sensors.
 *      Updates the widget and sends the data to the network.
 * @param id
 *      ID of the surface, this is needed to update the correct audiowidget.
 * @param level
 *      Level to display the audio volume.
 */
void Sensors::microphone_data(const int id, const char *data, const int level) const
{
    Q_UNUSED(data);
    audio_input_widgets.at(id)->update_level(level);
}

void Sensors::start_speakers()
{
    QString default_device = QAudioDeviceInfo::defaultOutputDevice().deviceName();
    QList<QAudioDeviceInfo> audio_output_info = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    for (int i = 0; i < audio_output_info.size(); i++)
    {
        if(audio_output_info.at(i).deviceName() == default_device)
        {

        }
        else
        {

        }
    }
}

void Sensors::speakers_data(const int id, const int level) const
{
    Q_UNUSED(id);
    Q_UNUSED(level);
}

/**
 * @brief Sensors::start_textstream
 *      Start the textstream
 */
void Sensors::start_textstream()
{
    text = new TextStream(this);
}
