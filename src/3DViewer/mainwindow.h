#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QColorDialog>

#define FILNAME_PATH "../../../../s21_3dViewer/src/models"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     QSettings* settings;

    MainWindow(QWidget *parent = nullptr);
    glView * view;
    ~MainWindow();



private slots:
    void start_settings();
    void write_settings();
    void on_button_filename_clicked();

    void on_pushButton_color_edge_clicked();

    void on_pushButton_color_vertex_clicked();

    void on_pushButton_color_background_clicked();

    void full_info();



    void on_radioButton_central_clicked(bool checked);

    void on_radioButton_ortho_clicked(bool checked);

    void on_radioButton_no_point_clicked(bool checked);

    void on_radioButton_sphere_clicked(bool checked);

    void on_radioButton_cube_clicked(bool checked);

    void on_horizontalSlider_vertex_size_valueChanged(int value);

    void on_horizontalSlider_edge_size_valueChanged(int value);

    void on_radioButton_solid_edge_clicked(bool checked);

    void on_radioButton_dotted_edge_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
