#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <ctime>
#include <QMouseEvent>



//#include <gl/GL.h>
//#include <gl/GLU.h>

extern "C"
{
#include "../parser.h"
}


QT_BEGIN_NAMESPACE
namespace Ui { class glView; }
QT_END_NAMESPACE

class glView : public QOpenGLWidget
{
    Q_OBJECT
private:
    float xRot, yRot, zRot;
    QPoint mPos;
    QTimer tmr;
    void drawObj(char *file);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
public:
    glView(QWidget *parent = nullptr);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    ~glView();

private:
    Ui::glView *ui;

};
#endif // GLVIEW_H
