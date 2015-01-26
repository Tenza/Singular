#include "singular.h"
#include "ui_singular.h"

#include <QDesktopWidget>

#include "settingsmanager.h"

Singular::Singular(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Singular)
{
    ui->setupUi(this);

    sensors = new Sensors(this);

    load_connections();
    load_settings();
}

Singular::~Singular()
{
    save_settings();

    delete ui;
}

void Singular::load_connections()
{

}

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
    //ui->splitter_main->restoreState(SettingsManager::read("Window/SplitterMain").toByteArray());
    ui->splitter_sensors->restoreState(SettingsManager::read("Window/SplitterSensors").toByteArray());
    ui->splitter_output->restoreState(SettingsManager::read("Window/SplitterOutput").toByteArray());
    ui->splitter_options->restoreState(SettingsManager::read("Window/SplitterOptions").toByteArray());
}

void Singular::save_settings()
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
    //SettingsManager::write("Window/SplitterMain", ui->splitter_main->saveState());
    SettingsManager::write("Window/SplitterSensors", ui->splitter_sensors->saveState());
    SettingsManager::write("Window/SplitterOutput", ui->splitter_output->saveState());
    SettingsManager::write("Window/SplitterOptions", ui->splitter_options->saveState());
}

void Singular::add_camera(QWidget *camera, QString camera_name) const
{
    ui->sw_cameras->setCurrentIndex(ui->sw_cameras->addWidget(camera));

    ui->cb_cameras->addItem(camera_name);
    ui->cb_cameras->setCurrentText(camera_name);
}

void Singular::add_microphone(QWidget *microphone, QString microphone_name) const
{
    ui->sw_microphones->setCurrentIndex(ui->sw_microphones->addWidget(microphone));

    ui->cb_microphones->addItem(microphone_name);
    ui->cb_microphones->setCurrentText(microphone_name);
}

void Singular::on_cb_cameras_currentIndexChanged(int index)
{
    ui->sw_cameras->setCurrentIndex(index);
}

void Singular::console(const QString &message) const
{
    ui->txt_console->appendHtml(message);
}

void Singular::on_txt_input_textChanged()
{
    emit get_text(ui->txt_input->toPlainText());
}
