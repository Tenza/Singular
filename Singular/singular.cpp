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

#include "singular.h"
#include "ui_singular.h"
#include "settingsmanager.h"

#include <QDesktopWidget>

/**
 * @brief Singular::Singular
 *      Starts the sensors and loads settings.
 * @param parent
 *      Parent this class and to use signals and slots.
 */
Singular::Singular(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Singular)
{
    ui->setupUi(this);

    sensors = new Sensors(this);

    load_settings();
}

/**
 * @brief Singular::~Singular
 *      Saves the UI settings on exit.
 */
Singular::~Singular()
{
    save_settings();

    delete ui;
}

/**
 * @brief Singular::load_settings
 *      Loads all the UI settings, position, size and splitters state.
 */
void Singular::load_settings()
{
    ui->sw_cameras->removeWidget(ui->page);
    ui->sw_cameras->removeWidget(ui->page_2);
    ui->sw_cameras->removeWidget(ui->page_3);
    ui->sw_cameras->removeWidget(ui->page_4);

    //Window

    this->resize(SettingsManager::read("Window/WindowWidth", 800).toInt(),
                 SettingsManager::read("Window/WindowHeight", 600).toInt());

    this->move(SettingsManager::read("Window/WindowPosition",
               QApplication::desktop()->screen()->rect().center() - this->rect().center()).toPoint());

    if(SettingsManager::read("Window/WindowMaximized", false).toBool())
    {
        this->showMaximized();
    }

    ui->splitter->restoreState(SettingsManager::read("Window/Splitter").toByteArray());
    ui->splitter_sensors->restoreState(SettingsManager::read("Window/SplitterSensors").toByteArray());
    ui->splitter_output->restoreState(SettingsManager::read("Window/SplitterOutput").toByteArray());
    ui->splitter_options->restoreState(SettingsManager::read("Window/SplitterOptions").toByteArray());
}

/**
 * @brief Singular::save_settings
 *      Saves all the UI settings, position, size and splitters state.
 */
void Singular::save_settings() const
{
    //Window

    //Only save position and size if not maximized, to preserve the last state.
    if(!this->isMaximized())
    {
        SettingsManager::write("Window/WindowWidth", this->width());
        SettingsManager::write("Window/WindowHeight", this->height());
        SettingsManager::write("Window/WindowPosition", this->pos());
    }

    SettingsManager::write("Window/WindowMaximized", this->isMaximized());

    SettingsManager::write("Window/Splitter", ui->splitter->saveState());
    SettingsManager::write("Window/SplitterSensors", ui->splitter_sensors->saveState());
    SettingsManager::write("Window/SplitterOutput", ui->splitter_output->saveState());
    SettingsManager::write("Window/SplitterOptions", ui->splitter_options->saveState());
}

/**
 * @brief Singular::console
 *      Slot used to output messages from any part of the code.
 * @param message
 *      HTML message to be printed.
 */
void Singular::console(const QString &message) const
{
    ui->txt_console->appendHtml(message);
}

/**
 * @brief Singular::add_camera
 *      Adds a new entry or camera widget to the UI.
 * @param camera_name
 *      Name of the device.
 * @param camera
 *      The custom widget.
 * @param selected
 *      Auto-select this widget.
 */
void Singular::add_camera(const QString &camera_name, QWidget *camera, const bool selected) const
{
    int id = 0;
    ui->cb_cameras->addItem(camera_name);

    if(camera != 0)
    {
        id = ui->sw_cameras->addWidget(camera);
    }

    if(selected)
    {
        ui->sw_cameras->setCurrentIndex(id);
        ui->cb_cameras->setCurrentText(camera_name);
    }
}

/**
 * @brief Singular::add_microphone
 *      Adds a new entry or microphone widget to the UI.
 * @param microphone_name
 *      Name of the device.
 * @param microphone
 *      The custom widget.
 * @param selected
 *      Auto-select this widget.
 */
void Singular::add_microphone(const QString &microphone_name, QWidget *microphone, const bool selected) const
{
    int id = 0;
    ui->cb_microphones->addItem(microphone_name);

    if(microphone != 0)
    {
        id = ui->sw_microphones->addWidget(microphone);
    }

    if(selected)
    {
        ui->sw_microphones->setCurrentIndex(id);
        ui->cb_microphones->setCurrentText(microphone_name);
    }
}

/**
 * @brief Singular::on_cb_cameras_currentIndexChanged
 *      Changes the current camera widget.
 * @param index
 *      Index of the camera.
 */
void Singular::on_cb_cameras_currentIndexChanged(int index)
{
    ui->sw_cameras->setCurrentIndex(index);
}

/**
 * @brief Singular::on_cb_microphones_currentIndexChanged
 *      Invokes updates to the microphone class.
 *      It only has one widget, so there is no need to change its index.
 * @param index
 *      Index of the microphone.
 */
void Singular::on_cb_microphones_currentIndexChanged(int index)
{
    if(sensors != NULL)
    {
        sensors->update_microphones(index);
    }
}

/**
 * @brief Singular::on_txt_input_textChanged
 *      This is the only slot that send data directly to the sensors.
 */
void Singular::on_txt_input_textChanged()
{
    emit get_text(ui->txt_input->toPlainText());
}
