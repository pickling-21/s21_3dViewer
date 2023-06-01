#include "mainwindow.h"
#include "ui_mainwindow.h"
#
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    full_info();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::write_settings()
{
//    QSettings settings("3DViewer_v1", "Conf");
}

void MainWindow::start_settings(){
//    QSettings settings("3DViewer_v1", "Conf");

}


void MainWindow::on_button_filename_clicked()
{
        QString str;
        str = QFileDialog::getOpenFileName(this, tr("Откройте obj file"), FILNAME_PATH ,tr("Object Files (*.obj)"));
        QStringList parts = str.split("/");
        QString lastBit = parts.at(parts.size()-1);
        ui->label_filename->setText(lastBit);
        ui->widget->filename = str;
}


void MainWindow::on_pushButton_color_edge_clicked()
{
    QColor color;
    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
//    ui->label_color_edge->setText(color.name());
    ui->widget->edge_color = color;
}


void MainWindow::on_pushButton_color_vertex_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
    ui->widget->vertex_color = color;
//    ui->label_color_vertex->setText(color.name());
}


void MainWindow::on_pushButton_color_background_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
    ui->label_color_background->setText(color.name());
    ui->widget->background_color = color;
}

void MainWindow::full_info()
{
    QString str = QString:: number(ui->widget->edge_amount);
    ui->label_edge_amount->setText(str);
    ui->radioButton_central->setChecked(true);
    ui->widget->frustum = true;
    ui->widget->edge_color = Qt::white;
    ui->radioButton_no_point->setChecked(true);
    ui->widget->p_type = NO;

    ui->widget->edge_size = 0.5;
    ui->horizontalSlider_edge_size->setValue(1);

    ui->radioButton_no_point->setChecked(true);

    ui->widget->dotted_edge=false;
    ui->radioButton_solid_edge->setChecked(true);
}


void MainWindow::on_radioButton_central_clicked(bool checked)
{
    if(checked) {
        ui->widget->frustum = true;
        ui->widget->ortho = false;
    }

}


void MainWindow::on_radioButton_ortho_clicked(bool checked)
{
    if(checked) {
        ui->widget->ortho = true;
        ui->widget->frustum = false;
    }

}


void MainWindow::on_radioButton_no_point_clicked(bool checked)
{
    if(checked)ui->widget->p_type = NO;

}


void MainWindow::on_radioButton_sphere_clicked(bool checked)
{
    if(checked)ui->widget->p_type = SPHERE;
}


void MainWindow::on_radioButton_cube_clicked(bool checked)
{
    if(checked)ui->widget->p_type = CUBE;
}


void MainWindow::on_horizontalSlider_vertex_size_valueChanged(int value)
{
    ui->widget->vertex_size = value/2.0;
    ui->label_vertex_size->setText(QString::number(value/2.0));
}


void MainWindow::on_horizontalSlider_edge_size_valueChanged(int value)
{
    ui->widget->edge_size = value/2.0;
    ui->label_edge_size->setText(QString::number(value/2.0));
}


void MainWindow::on_radioButton_solid_edge_clicked(bool checked)
{
  if(checked)ui->widget->dotted_edge = false;
}


void MainWindow::on_radioButton_dotted_edge_clicked(bool checked)
{
    if(checked)ui->widget->dotted_edge = true;
}

