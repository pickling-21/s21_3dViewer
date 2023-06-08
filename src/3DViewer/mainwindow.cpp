#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QImageWriter>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
//  QScreen *screen = ui->widget->screen();
  setFixedSize(1300, 1000);
  full_info();
  load_settings();

}

MainWindow::~MainWindow() {
  ui->widget->destroy_all();
  save_settings();
  delete ui;
}

void MainWindow::on_pushButton_save_image_jpeg_clicked()
{
    save_screen("jpeg");
}


void MainWindow::save_settings()
{
   settings->setValue("title", windowTitle());
   settings->beginGroup("Vertex");
   settings->setValue("type", ui->widget->p_type);
   settings->setValue("color", ui->widget->vertex_color);
   settings->setValue("size", ui->widget->vertex_size);
   settings->endGroup();

   settings->beginGroup("Edge");
   settings->setValue("dotted", ui->widget->dotted_edge);
   settings->setValue("color", ui->widget->edge_color);
   settings->setValue("size", ui->widget->edge_size);
   settings->endGroup();

   settings->beginGroup("Other");
   settings->setValue("back_color", ui->widget->background_color);
   settings->setValue("frustum", ui->widget->frustum);
   settings->setValue("ortho", ui->widget->ortho);
   settings->endGroup();


   settings->beginGroup("Rotation");
   settings->setValue("x", x_rot_sum);
   settings->setValue("y", y_rot_sum);
   settings->setValue("z", z_rot_sum);
   settings->endGroup();

   settings->beginGroup("Move");
   settings->setValue("x", x_move_sum);
   settings->setValue("y", y_move_sum);
   settings->setValue("z", z_move_sum);
   settings->endGroup();

}


void MainWindow::load_settings()
{
    settings = new QSettings("3D", "ConfViewer");
    //QSettings settings("/Users/petra/misc/myapp.plist",QSettings::NativeFormat);
    setWindowTitle(settings->value("title", "3DViewer").toString());


    settings->beginGroup("Vertex");

    if(settings->value("type") == NO)ui->radioButton_no_point->setChecked(true);
    if(settings->value("type") == SPHERE)ui->radioButton_sphere->setChecked(true);
    if(settings->value("type") == CUBE)ui->radioButton_cube->setChecked(true);
    ui->widget->p_type = (enum points_type)settings->value("type").toInt();
    ui->widget->vertex_color = settings->value("color").toString();
    ui->widget->vertex_size = settings->value("size").toFloat();
    ui->lineEdit_dot_size->setText(QString::number(ui->widget->vertex_size));

    settings->endGroup();




    settings->beginGroup("Edge");

    if(settings->value("dotted").toBool()){
        ui->widget->dotted_edge = true;
        ui->radioButton_dotted_edge->setChecked(true);
    }else{
        ui->widget->dotted_edge = false;
        ui->radioButton_solid_edge->setChecked(true);
    }
    ui->widget->edge_color = settings->value("color").toString();
    if(!ui->widget->edge_color.isValid())ui->widget->edge_color = Qt::white;
    ui->widget->edge_size = settings->value("size").toFloat();
    ui->lineEdit_thickness_edge->setText(QString::number(ui->widget->edge_size));

    settings->endGroup();

    settings->beginGroup("Other");
    ui->widget->background_color = settings->value("back_color").toString();
    ui->widget->frustum = settings->value("frustum").toBool();
    if(ui->widget->frustum)ui->radioButton_central->setCheckable(true);
    ui->widget->ortho = settings->value("ortho").toBool();
    if(ui->widget->ortho)ui->radioButton_ortho->setCheckable(true);

    settings->endGroup();


    settings->beginGroup("Rotation");
    ui->lineEdit_xRot->setText(settings->value("x").toString());
    ui->lineEdit_yRot->setText(settings->value("y").toString());
    ui->lineEdit_zRot->setText(settings->value("z").toString());
    settings->endGroup();


    settings->beginGroup("Move");
    ui->lineEdit_x_move->setText(settings->value("x").toString());
    ui->lineEdit_y_move->setText(settings->value("y").toString());
    ui->lineEdit_z_move->setText(settings->value("z").toString());
    settings->endGroup();

}

void MainWindow::save_screen(QString format)
{
    originalPixmap = QWidget::grab(ui->widget->frameGeometry());
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    QStringList mimeTypes;
    const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
    for (const QByteArray &bf : baMimeTypes)
        mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fileName = fileDialog.selectedFiles().first();
    if (!originalPixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                             .arg(QDir::toNativeSeparators(fileName)));
    }
}

void MainWindow::on_button_filename_clicked() {
    QString str = QFileDialog::getOpenFileName(this, "Open a file", "./models/", "(*.obj)");

  ui->widget->filename = str;

  if(!str.isEmpty()){
      ui->widget->paint_file_first();
      ui->widget->update();

      ui->label_edge_amount->setText(QString::number(ui->widget->get_edge()));
      ui->label_vertex_amount->setText(QString::number(ui->widget->get_top_pointers()-1));
      QStringList parts = str.split("/");
      QString lastBit = parts.at(parts.size()-1);
      ui->label_filename->setText(lastBit);
  }
}

void MainWindow::full_info() {
   x_rot_sum = 0;
   y_rot_sum = 0;
   z_rot_sum = 0;


   x_move_sum = 0;
   y_move_sum = 0;
   z_move_sum = 0;

  ui->radioButton_ortho->setChecked(true);
  ui->widget->ortho = true;

 ui->radioButton_solid_edge->setChecked(true);
 ui->widget->dotted_edge = false;

 ui->radioButton_no_point->setChecked(true);
 ui->widget->p_type = NO;

 ui->widget->vertex_size = 0.5;
 ui->widget->edge_size = 0.5;

 ui->lineEdit_thickness_edge->setText("0.5");
 ui->lineEdit_dot_size->setText("0.5");

 ui->widget->edge_color = Qt::white;
 ui->widget->background_color = Qt::black;

  if (ui->widget->error != NO_ERROR) {
    if (ui->widget->error == FILE_ERROR) ui->label_filename->setText("file");
    if (ui->widget->error == PARSER_ERROR)
      ui->label_filename->setText("parser");
  }
}



void MainWindow::on_radioButton_central_clicked(bool checked) {
  if (checked) {
    ui->widget->frustum = true;
    ui->widget->ortho = false;
    ui->widget->update();
  }
}

void MainWindow::on_radioButton_ortho_clicked(bool checked) {
  if (checked) {
    ui->widget->ortho = true;
    ui->widget->frustum = false;
    ui->widget->update();
  }
}

void MainWindow::on_pushButton_apply_rotate_clicked() {

  if(ui->widget->destroy){
    x_rot_sum += ui->lineEdit_xRot->text().toDouble();
    y_rot_sum += ui->lineEdit_yRot->text().toDouble();
    z_rot_sum += ui->lineEdit_zRot->text().toDouble();

    figure_rotation(ui->widget->get_top_pointers(), ui->widget->get_all_points(), ui->lineEdit_xRot->text().toDouble(), ui->lineEdit_yRot->text().toDouble(), ui->lineEdit_zRot->text().toDouble());
    ui->widget->update();
  }
}


void MainWindow::on_pushButton_apply_move_clicked()
{
   if(ui->widget->destroy){
       x_move_sum += ui->lineEdit_x_move->text().toDouble();
       y_move_sum += ui->lineEdit_y_move->text().toDouble();
       z_move_sum += ui->lineEdit_z_move->text().toDouble();
   figure_move_x(ui->widget->get_top_pointers(), ui->lineEdit_x_move->text().toInt(), ui->widget->get_all_points());
   figure_move_y(ui->widget->get_top_pointers(), ui->lineEdit_y_move->text().toInt(), ui->widget->get_all_points());
   figure_move_z(ui->widget->get_top_pointers(), ui->lineEdit_z_move->text().toInt(), ui->widget->get_all_points());
   ui->widget->update();
   }
}


void MainWindow::on_radioButton_no_point_clicked(bool checked)
{
    if(checked){
    ui->widget->p_type = NO;
    ui->widget->update();
    }
}


void MainWindow::on_radioButton_sphere_clicked(bool checked)
{
    if(checked){
    ui->widget->p_type = SPHERE;
    ui->widget->update();
    }

}


void MainWindow::on_radioButton_cube_clicked(bool checked)
{
    if(checked){
    ui->widget->p_type = CUBE;
    ui->widget->update();
    }

}




void MainWindow::on_lineEdit_thickness_edge_editingFinished()
{
    bool ok = true;
    ui->widget->edge_size = ui->lineEdit_thickness_edge->text().toFloat(&ok);
    if(!ok  || ui->widget->edge_size < 0){
        ui->widget->edge_size = 0.5;
        ui->lineEdit_thickness_edge->setText("0.5");
    }
    ui->widget->update();
}


void MainWindow::on_lineEdit_dot_size_editingFinished()
{
    bool ok = true;
    ui->widget->vertex_size = ui->lineEdit_dot_size->text().toFloat(&ok);
    if(!ok  || ui->widget->vertex_size < 0){
        ui->widget->vertex_size = 0.5;
        ui->lineEdit_dot_size->setText("0.5");
    }
    ui->widget->update();
}


void MainWindow::on_radioButton_solid_edge_clicked(bool checked)
{
    if(checked){
        ui->widget->dotted_edge = false;
        ui->widget->update();
    }

}


void MainWindow::on_radioButton_dotted_edge_clicked(bool checked)
{
    if(checked){
       ui->widget->dotted_edge = true;
       ui->widget->update();
    }
}


void MainWindow::on_lineEdit_thickness_edge_returnPressed()
{
    on_lineEdit_thickness_edge_editingFinished();
}


void MainWindow::on_lineEdit_dot_size_returnPressed()
{
    on_lineEdit_dot_size_editingFinished();
}


void MainWindow::on_pushButton_color_edge_clicked()
{
  ui->widget->edge_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребра");
  ui->widget->update();
}


void MainWindow::on_pushButton_color_vertex_clicked()
{
    ui->widget->vertex_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет вершин");
    ui->widget->update();
}


void MainWindow::on_pushButton_color_background_clicked()
{
    ui->widget->background_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет фона");
    ui->widget->update();
}


void MainWindow::on_pushButton_xRot_left_clicked()
{
    QString text = ui->lineEdit_xRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_xRot->setText(QString::number(number - (STEP_ROT)));
    }else{
        ui->lineEdit_xRot->setText(QString::number(-(STEP_ROT)));
    }
}


void MainWindow::on_pushButton_yRot_left_clicked()
{
    QString text = ui->lineEdit_yRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_yRot->setText(QString::number(number - (STEP_ROT)));
    }else{
        ui->lineEdit_yRot->setText(QString::number(-(STEP_ROT)));
    }
}


void MainWindow::on_pushButton_zRot_left_clicked()
{
    QString text = ui->lineEdit_zRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_zRot->setText(QString::number(number - (STEP_ROT)));
    }else{
        ui->lineEdit_zRot->setText(QString::number(-(STEP_ROT)));
    }
}


void MainWindow::on_pushButton_xRot_right_clicked()
{
    QString text = ui->lineEdit_xRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_xRot->setText(QString::number(number + (STEP_ROT)));
    }else{
        ui->lineEdit_xRot->setText(QString::number(STEP_ROT));
    }
}


void MainWindow::on_pushButton_yRot_right_clicked()
{
    QString text = ui->lineEdit_yRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_yRot->setText(QString::number(number + (STEP_ROT)));
    }else{
        ui->lineEdit_yRot->setText(QString::number(STEP_ROT));
    }
}


void MainWindow::on_pushButton_zRot_right_clicked()
{
    QString text = ui->lineEdit_zRot->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_zRot->setText(QString::number(number + (STEP_ROT)));
    }else{
        ui->lineEdit_zRot->setText(QString::number(STEP_ROT));
    }
}


void MainWindow::on_pushButton_x_move_left_clicked()
{
    QString text = ui->lineEdit_x_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_x_move->setText(QString::number(number - (STEP_MOVE)));
    }else{
        ui->lineEdit_x_move->setText(QString::number(-(STEP_MOVE)));
    }
}


void MainWindow::on_pushButton_y_move_left_clicked()
{
    QString text = ui->lineEdit_y_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_y_move->setText(QString::number(number - (STEP_MOVE)));
    }else{
        ui->lineEdit_y_move->setText(QString::number(-(STEP_MOVE)));
    }
}


void MainWindow::on_pushButton_z_move_left_clicked()
{
    QString text = ui->lineEdit_z_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_z_move->setText(QString::number(number - (STEP_MOVE)));
    }else{
        ui->lineEdit_z_move->setText(QString::number(-(STEP_MOVE)));
    }
}


void MainWindow::on_pushButton_x_move_right_clicked()
{
    QString text = ui->lineEdit_x_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_x_move->setText(QString::number(number + (STEP_MOVE)));
    }else{
        ui->lineEdit_x_move->setText(QString::number(STEP_MOVE));
    }
}


void MainWindow::on_pushButton_y_move_right_clicked()
{
    QString text = ui->lineEdit_y_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_y_move->setText(QString::number(number + (STEP_MOVE)));
    }else{
        ui->lineEdit_y_move->setText(QString::number(STEP_MOVE));
    }
}


void MainWindow::on_pushButton_z_move_right_clicked()
{
    QString text = ui->lineEdit_z_move->text();
    bool ok = true;
    double number = text.toDouble(&ok);
    if(!text.isEmpty() && ok){
        ui->lineEdit_z_move->setText(QString::number(number + (STEP_MOVE)));
    }else{
        ui->lineEdit_z_move->setText(QString::number(STEP_MOVE));
    }
}


void MainWindow::on_pushButton_minus_clicked()
{

    double zoom = ui->label_scale->text().toDouble();
    zoom -= STEP_SCALE;
    if(zoom > 20 && zoom < 200){
       ui->label_scale->setText(QString::number(zoom));
    }
}


void MainWindow::on_pushButton_plus_clicked()
{
    double zoom = ui->label_scale->text().toDouble();
    zoom += STEP_SCALE;
    if(zoom > 20 && zoom <= 200){
       ui->label_scale->setText(QString::number(zoom));
    }
}


void MainWindow::on_pushButton_clicked()
{
    double prev = ui->widget->scale;
    double now = ui->label_scale->text().toDouble()/100;

        ui->widget->scale = now/prev;
        ui->widget->set_scale();
//        ui->label_scale_name->setText(QString::number(ui->widget->scale));
        ui->widget->scale = now;

}




void MainWindow::on_pushButton_save_image_bmp_clicked()
{
    save_screen("bmp");
}

