#ifndef GLVIEW_H
#define GLVIEW_H
#define GL_SILENCE_DEPRECATION
#include <QWidget>
#include <QOpenGLWidget>
#include <QMouseEvent>


extern "C"
{
#include "../parser.h"
}



namespace Ui {
class glView;
}

class glView : public QOpenGLWidget
{
    Q_OBJECT

public:
    glView(QWidget *parent = nullptr);

public:
    QColor background_color, vertex_color, edge_color;

    QString filename;

    double x_move, y_move, z_move;
    double prev_x_move, prev_y_move, prev_z_move;

    double x_rot, y_rot, z_rot;
    double prev_x_rot, prev_y_rot, prev_z_rot;

    double scale;

    uint64_t vertex_amount;
    uint64_t edge_amount;


private:
    float xRot, yRot, zRot;
    QPoint mPos;
    void drawObj(char *file);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    uint64_t count_edge(char * file);
    uint64_t count_vertex(char * file);


public:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    Ui::glView *ui;
};


#endif // GLVIEW_H
