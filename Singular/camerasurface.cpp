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

#include "camerasurface.h"
#include "output.h"

/**
 * @brief CameraSurface::CameraSurface
 *      Connect this surface with the console and camera widget.
 * @param new_id
 *      The ID of this camera.
 * @param parent
 *      To parent this class and to use signals and slots.
 */
CameraSurface::CameraSurface(const int new_id, QCameraInfo new_camera_info, QObject *parent)
    : QAbstractVideoSurface(parent),
      id(new_id),
      camera_info(new_camera_info)
{
    connect(this, SIGNAL(image_data(int, QImage)), parent, SLOT(image_data(int, QImage)));
    connect(this, SIGNAL(console(QString)), parent, SIGNAL(console(QString)));

    //Initializes the camera.
    camera = new QCamera(camera_info, this);
    connect(camera, SIGNAL(stateChanged(QCamera::State)), SLOT(stateChanged(QCamera::State)));

    camera->setViewfinder(this);
}

/**
 * @brief CameraSurface::start
 *      Starts the camera interface.
 */
void CameraSurface::start() const
{
    if(camera->error() == QCamera::NoError)
    {
        camera->start();
    }
}

/**
 * @brief CameraSurface::start
 *      Starts the surface if the requested format is supported.
 *      This function is called internally by the QCamera class.
 * @param format
 *      The format to start the frame.
 * @return
 *      Success = true; Failed = false
 */
bool CameraSurface::start(const QVideoSurfaceFormat &format)
{
    bool result = false;

    if(isFormatSupported(format))
    {
        output("Starting video surface.", 1);
        QAbstractVideoSurface::start(format);

        result = true;
    }
    else
    {
        output("Camera format not supported, surface not started.", 1);
    }

    return result;
}

/**
 * @brief CameraSurface::stop
 *      Stops the surface.
 *      This function is called internally by the QCamera class.
 */
void CameraSurface::stop()
{
    output("Stopping video surface.", 1);
    QAbstractVideoSurface::stop();
    camera->stop();
}

/**
 * @brief CameraSurface::present
 *      Present the current frame.
 *      This function is called internally by the QCamera class.
 * @param frame
 *      The frame to be presented.
 * @return
 *      Success = true; Failed = false
 */
bool CameraSurface::present(const QVideoFrame &frame)
 {
    bool result = false;

    if (frame.isValid() && surfaceFormat().pixelFormat() == frame.pixelFormat() && surfaceFormat().frameSize() == frame.size())
    {
        QVideoFrame clone_frame(frame);

        if(clone_frame.map(QAbstractVideoBuffer::ReadOnly))
        {

            QImage image(clone_frame.bits(),
                         clone_frame.width(),
                         clone_frame.height(),
                         QVideoFrame::imageFormatFromPixelFormat(clone_frame.pixelFormat()));

            //Process image, if needed, before signal.

            emit image_data(id, image);
            clone_frame.unmap();

            result = true;
        }
    }
    else
    {
        output("Frame error, stopping camera.", 1);
        setError(IncorrectFormatError);
        stop();
    }

    return result;
 }

/**
 * @brief CameraSurface::isFormatSupported
 *      The camera is only supported if there is a correspondent QImage format.
 *      This function is called internally by the QCamera class.
 * @param format
 *      The format of the camera.
 * @return
 *      Supported = true; Not supported = false
 */
bool CameraSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    bool result = false;

    const QImage::Format image_format = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if(image_format != QImage::Format_Invalid && !size.isEmpty() && format.handleType() == QAbstractVideoBuffer::NoHandle)
    {
        result = true;
    }

    output("Returned isFormatSupported.", 1);
    return result;
}

/**
 * @brief CameraSurface::supportedPixelFormats
 *      List of pixel formats the surface can paint.
 *      This function is called internally by the QCamera class.
 * @param handleType
 *      We don't support rendering using any special frame handles.
 * @return
 *      List of supported pixel formats.
 */
QList<QVideoFrame::PixelFormat> CameraSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> formats;

    if(handleType == QAbstractVideoBuffer::NoHandle)
    {
        //Formats that have a QImage format equivalent.
        formats.append(QVideoFrame::Format_ARGB32);
        formats.append(QVideoFrame::Format_ARGB32_Premultiplied);
        formats.append(QVideoFrame::Format_RGB32);
        formats.append(QVideoFrame::Format_RGB24);
        formats.append(QVideoFrame::Format_RGB565);
        formats.append(QVideoFrame::Format_RGB555);

//        //Formats that DONT have a QImage format equivalent.
//        formats.append(QVideoFrame::Format_ARGB8565_Premultiplied);
//        formats.append(QVideoFrame::Format_BGRA32);
//        formats.append(QVideoFrame::Format_BGRA32_Premultiplied);
//        formats.append(QVideoFrame::Format_BGR32);
//        formats.append(QVideoFrame::Format_BGR24);
//        formats.append(QVideoFrame::Format_BGR565);
//        formats.append(QVideoFrame::Format_BGR555);
//        formats.append(QVideoFrame::Format_BGRA5658_Premultiplied);
//        formats.append(QVideoFrame::Format_AYUV444);
//        formats.append(QVideoFrame::Format_AYUV444_Premultiplied);
//        formats.append(QVideoFrame::Format_YUV444);
//        formats.append(QVideoFrame::Format_YUV420P);
//        formats.append(QVideoFrame::Format_YV12);
//        formats.append(QVideoFrame::Format_UYVY);
//        formats.append(QVideoFrame::Format_YUYV);
//        formats.append(QVideoFrame::Format_NV12);
//        formats.append(QVideoFrame::Format_NV21);
//        formats.append(QVideoFrame::Format_IMC1);
//        formats.append(QVideoFrame::Format_IMC2);
//        formats.append(QVideoFrame::Format_IMC3);
//        formats.append(QVideoFrame::Format_IMC4);
//        formats.append(QVideoFrame::Format_Y8);
//        formats.append(QVideoFrame::Format_Y16);
//        formats.append(QVideoFrame::Format_Jpeg);
//        formats.append(QVideoFrame::Format_CameraRaw);
//        formats.append(QVideoFrame::Format_AdobeDng);
//        formats.append(QVideoFrame::Format_User);
    }

    output("Returned supportedPixelFormats.", 1);

    return formats;
}

/**
 * @brief AudioInputSurface::stateChanged
 *      Signal that displays the current state of the device.
 * @param state
 *      Current state of the device.
 */
void CameraSurface::stateChanged(QCamera::State state)
{
    switch (state)
    {
        case QCamera::UnloadedState:
        {
            output("Camera device state: UnloadedState", 3);
            break;
        }
        case QCamera::LoadedState:
        {
            output("Camera device state: LoadedState", 3);
            break;
        }
        case QCamera::ActiveState:
        {
            output("Camera device state: ActiveState", 3);
            break;
        }
    }
}

/**
 * @brief TextStream::output
 *      Generic function responsible for all the outputs.
 */
void CameraSurface::output(const QString &message, const int verbose) const
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
