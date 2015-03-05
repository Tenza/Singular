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

#include "audioinputsurface.h"
#include "output.h"

#include <QtEndian>

/**
 * @brief AudioInputSurface::AudioInputSurface
 *      I will use the hardware prefered format instead of setting the device settings manually.
 *      We should give a option to inicialize with manual settings, if not use the default ones.
 *      Will this class be destoyed with changing devices?
 * @param parent
 */
AudioInputSurface::AudioInputSurface(const int new_id, const QAudioDeviceInfo new_device_info, const QAudioFormat new_device_format, QObject *parent)
    : QIODevice(parent),
      id(new_id),
      device_info(new_device_info),
      device_format(new_device_format)
{
    connect(this, SIGNAL(console(const QString&)), parent, SIGNAL(console(const QString&)));
    connect(this, SIGNAL(microphone_data(const int, const int)), parent, SLOT(microphone_data(const int, const int)));

//    Automatic device settings
//    device_info = QAudioDeviceInfo::defaultInputDevice();
//    device_format = device_info.preferredFormat();

//    Manual device settings
//    device_format.setSampleRate(8000);
//    device_format.setSampleSize(8);
//    device_format.setSampleType(QAudioFormat::UnSignedInt);
//    device_format.setChannelCount(1);
//    device_format.setByteOrder(QAudioFormat::LittleEndian);
//    device_format.setCodec("audio/pcm");

    if (!device_info.isFormatSupported(device_format))
    {
        output("Audio format not supported, trying to use nearest.", 1);
        device_format = device_info.nearestFormat(device_format);
    }

    audio_input = new QAudioInput(device_info, device_format, this);
    connect(audio_input, SIGNAL(notify()), SLOT(notified()));
    connect(audio_input, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));

    set_amplitude();
    device_print();

    output("Audio-in device started: " + device_info.deviceName(), 1);
}

AudioInputSurface::~AudioInputSurface()
{

}

/**
 * @brief AudioInputSurface::set_amplitude
 *      Determine peak amplitude given the audio format.
 *      Each sample cannot be bigger than its amplitude, the amplitude is the maximum possible value for the given sample size.
 *      For example, an 8 bit audio sample has a maximum value of 255 because 11111111 (8 bits unsigned) is 255: 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1.
 */
void AudioInputSurface::set_amplitude()
{
    switch (device_format.sampleSize())
    {
        case 8:
        {
            switch (device_format.sampleType())
            {
                case QAudioFormat::UnSignedInt:
                {
                    audio_peak_amplitude = 255;
                    break;
                }
                case QAudioFormat::SignedInt:
                {
                    audio_peak_amplitude = 127;
                    break;
                }
                default:
                {
                    audio_peak_amplitude = 0;
                    break;
                }
            }
            break;
        }
        case 16:
        {
            switch (device_format.sampleType())
            {
                case QAudioFormat::UnSignedInt:
                {
                    audio_peak_amplitude = 65535;
                    break;
                }
                case QAudioFormat::SignedInt:
                {
                    audio_peak_amplitude = 32767;
                    break;
                }
                default:
                {
                    audio_peak_amplitude = 0;
                    break;
                }
            }
            break;
        }
    }
}

/**
 * @brief AudioInputSurface::start
 *      Opens the device.
 */
void AudioInputSurface::start()
{
    open(QIODevice::WriteOnly | QIODevice::Truncate);
    audio_input->start(this);
}

/**
 * @brief AudioInputSurface::stop
 *      Stops the device.
 */
void AudioInputSurface::stop()
{
    audio_input->stop();
    close();
}

/**
 * @brief AudioInputSurface::readData
 *      This is a microphone device, so there is no need to implement a read function.
 * @param data
 * @param maxSize
 * @return
 */
qint64 AudioInputSurface::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return 0;
}

/**
 * @brief AudioInputSurface::writeData
 *      Receives the data from the device, consumes the data by looping all the samples independently and
 *
 * converts it to a unsigned type, independently for the device preferences.
 *
 *      Sample = Single point in a analog signal.
 *      Sample size = Commonly called bit depth, is the resolution of each sample. 16-bit digital audio means 16bit or 2bytes worth of data.
 *      Channel numbers = Self explanatory. But keep in mind that this number multiplies with the sample size to get the total size.
 *      Amplitude = Maximum possible value.
 * @param data
 * @param maxSize
 * @return
 */
qint64 AudioInputSurface::writeData(const char *data, qint64 maxSize)
{
    if (audio_peak_amplitude)
    {
        quint16 max_value = 0;

        const int sample_bytes = device_format.sampleSize() / 8;
        const int channel_bytes = device_format.channelCount() * sample_bytes;        
        const int total_samples = maxSize / channel_bytes;

        const unsigned char *data_ptr = reinterpret_cast<const unsigned char *>(data);

        //Loops all the data, split by total size and number of channels.
        for (int i = 0; i < total_samples; ++i)
        {
            for (int j = 0; j < device_format.channelCount(); ++j)
            {
                quint16 value = 0;

                if (device_format.sampleSize() == 8 && device_format.sampleType() == QAudioFormat::UnSignedInt)
                {
                    value = *reinterpret_cast<const quint8*>(data_ptr);
                }
                else if (device_format.sampleSize() == 8 && device_format.sampleType() == QAudioFormat::SignedInt)
                {
                    value = qAbs(*reinterpret_cast<const qint8*>(data_ptr));
                }
                else if (device_format.sampleSize() == 16 && device_format.sampleType() == QAudioFormat::UnSignedInt)
                {
                    if (device_format.byteOrder() == QAudioFormat::LittleEndian)
                    {
                        value = qFromLittleEndian<quint16>(data_ptr);
                    }
                    else
                    {
                        value = qFromBigEndian<quint16>(data_ptr);
                    }
                }
                else if (device_format.sampleSize() == 16 && device_format.sampleType() == QAudioFormat::SignedInt)
                {
                    if (device_format.byteOrder() == QAudioFormat::LittleEndian)
                    {
                        value = qAbs(qFromLittleEndian<qint16>(data_ptr));
                    }
                    else
                    {
                        value = qAbs(qFromBigEndian<qint16>(data_ptr));
                    }
                }

                max_value = qMax(value, max_value);
                data_ptr += sample_bytes;
            }
        }

        max_value = qMin(max_value, audio_peak_amplitude);
        max_value = (max_value * 100) / audio_peak_amplitude; // 100 - peak | X - value

        emit microphone_data(id, max_value);
    }

    return maxSize;
}

/**
 * @brief AudioInputSurface::device_print
 *      Prints all the device current settings.
 */
void AudioInputSurface::device_print()
{
    output("Device: " + device_info.deviceName(), 3);

    output("Codec: " + device_format.codec(), 3);
    output("Channels: " + QString::number(device_format.channelCount()), 3);

    output("Sample rate: " + QString::number(device_format.sampleRate()), 3);
    output("Sample size: " + QString::number(device_format.sampleSize()), 3);

    const int sample_bytes = device_format.sampleSize() / 8;
    const int channel_bytes = device_format.channelCount() * sample_bytes;

    output("Sample bytes: " + QString::number(sample_bytes), 3);
    output("Channel bytes: " + QString::number(channel_bytes), 3);

    if(device_format.sampleType() == QAudioFormat::Unknown)
    {
        output("Sample type: Unknown", 3);
    }
    else if(device_format.sampleType() == QAudioFormat::SignedInt)
    {
        output("Sample type: SignedInt", 3);
    }
    else if(device_format.sampleType() == QAudioFormat::UnSignedInt)
    {
        output("Sample type: UnSignedInt", 3);
    }
    else if(device_format.sampleType() == QAudioFormat::Float)
    {
        output("Sample type: Float", 3);
    }

    if(device_format.byteOrder() == QAudioFormat::BigEndian)
    {
        output("Sample byte order: BigEndian", 3);
    }
    else if(device_format.byteOrder() == QAudioFormat::LittleEndian)
    {
        output("Sample byte order: LittleEndian", 3);
    }
}

void AudioInputSurface::notified()
{
    output("Bytes ready: " + QString::number(audio_input->bytesReady()), 3);
    output("Elapsed microseconds: " + QString::number(audio_input->elapsedUSecs()), 3);
    output("Processed microseconds: " + QString::number(audio_input->processedUSecs()), 3);
}

void AudioInputSurface::stateChanged(QAudio::State state)
{
    switch (state)
    {
        case QAudio::ActiveState:
        {
            output("Audio-in device state: ActiveState", 3);
            break;
        }
        case QAudio::SuspendedState:
        {
            output("Audio-in device state: SuspendedState", 3);
            break;
        }
        case QAudio::StoppedState:
        {
            output("Audio-in device state: StoppedState", 3);

            if (audio_input->error() != QAudio::NoError)
            {
                // Error handling
            }
            else
            {
                // Finished recording
            }
            break;
        }
        case QAudio::IdleState:
        {
            output("Audio-in device state: IdleState", 3);
            break;
        }
    }
}

void AudioInputSurface::output(const QString &message, const int &verbose) const
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
