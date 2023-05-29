#include "mainwindow.h"
#include "ui_mainwindow.h"
#
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::write_settings()
{
    QSettings settings("3DViewer_v1", "Conf");
}

void MainWindow::start_settings(){
    QSettings settings("3DViewer_v1", "Conf");

//    ui->centralwidget.


}


void MainWindow::on_button_filename_clicked()
{
            QString str;
            str = QFileDialog::getOpenFileName(this, tr("Откройте obj file"), FILNAME_PATH ,tr("Object Files (*.obj)"));
            ui->label_filename->setText(str);
}


void MainWindow::on_pushButton_color_edge_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
    ui->label_color_edge->setText(color.name());
}


void MainWindow::on_pushButton_color_vertex_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
    ui->label_color_vertex->setText(color.name());
}


void MainWindow::on_pushButton_color_background_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
    ui->label_color_background->setText(color.name());
}

