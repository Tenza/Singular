#ifndef SINGULAR_H
#define SINGULAR_H

#include <QMainWindow>

#include "defines.h"
#include "sensors.h"

namespace Ui
{
    class Singular;
}

class Singular : public QMainWindow
{
    Q_OBJECT

public_construct:
    explicit Singular(QWidget *parent = 0);
    ~Singular();

private_methods:
    void load_settings();
    void save_settings();
    void load_connections();

private_data_members:
    Ui::Singular *ui;
    Sensors *sensors;

signals:
    void get_text(const QString &message) const;

public slots:
    void console(const QString &message) const;
    void add_camera(QWidget *camera, QString camera_name) const;

    void add_microphone(QWidget *microphone, QString microphone_name) const;

private slots:  
    void on_cb_cameras_currentIndexChanged(int index);
    void on_txt_input_textChanged();

};

#endif // SINGULAR_H
