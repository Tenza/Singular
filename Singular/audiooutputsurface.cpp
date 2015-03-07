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

#include "audiooutputsurface.h"
#include "output.h"

AudioOutputSurface::AudioOutputSurface(const int new_id, const QAudioDeviceInfo new_device_info, const QAudioFormat new_device_format, QObject *parent)
    : QIODevice(parent),
      id(new_id),
      device_info(new_device_info),
      device_format(new_device_format)
{
    connect(this, SIGNAL(console(QString)), parent, SIGNAL(console(QString)));
    connect(this, SIGNAL(speakers_data(int)), parent, SLOT(speakers_data(int)));

    if (!device_info.isFormatSupported(device_format))
    {
        output("Audio format not supported, trying to use nearest.", 1);
        device_format = device_info.nearestFormat(device_format);
    }

    audio_output = new QAudioOutput(device_info, device_format, this);
    connect(audio_output, SIGNAL(notify()), SLOT(notify()));
    connect(audio_output, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));

}

AudioOutputSurface::~AudioOutputSurface()
{

}

void AudioOutputSurface::start()
{
    open(QIODevice::ReadOnly | QIODevice::Truncate);
    audio_output->start(this);
}

void AudioOutputSurface::stop()
{
    audio_output->stop();
    close();
}

qint64 AudioOutputSurface::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return 0;
}

qint64 AudioOutputSurface::writeData(const char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return 0;
}

void AudioOutputSurface::notify()
{
    output("Bytes free: " + QString::number(audio_output->bytesFree()), 3);
    output("Elapsed microseconds: " + QString::number(audio_output->elapsedUSecs()), 3);
    output("Processed microseconds: " + QString::number(audio_output->processedUSecs()), 3);
}

void AudioOutputSurface::stateChanged(QAudio::State state)
{
    switch (state)
    {
        case QAudio::ActiveState:
        {
            output("Audio-out device state: ActiveState", 3);
            break;
        }
        case QAudio::SuspendedState:
        {
            output("Audio-out device state: SuspendedState", 3);
            break;
        }
        case QAudio::StoppedState:
        {
            output("Audio-out device state: StoppedState", 3);

            if (audio_output->error() != QAudio::NoError)
            {
                output("Audio-out device error, stopping audio.", 1);
                stop();
            }
            break;
        }
        case QAudio::IdleState:
        {
            output("Audio-out device state: IdleState", 3);
            break;
        }
    }
}

void AudioOutputSurface::output(const QString &message, const int verbose) const
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
