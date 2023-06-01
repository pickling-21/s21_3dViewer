#include "glview.h"
#include <unistd.h>
glView::glView(QWidget *parent) :
    QOpenGLWidget(parent)
{


}


void glView:: initializeGL(){
//    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);


}
void glView:: resizeGL(int w, int h){
    glViewport(0, 0, w, h);

}



// вершинный буфер

void glView:: paintGL(){
    glClearColor(background_color.redF(), background_color.greenF(), background_color.blueF(), background_color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // проекция
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    projection();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);

    QByteArray ba = filename.toLocal8Bit();
    char * file = ba.data();


    if(p_type == SPHERE) glEnable(GL_POINT_SMOOTH);

    if (p_type != NO) drawObj(file, true);

    drawObj(file, false);

    if(p_type == SPHERE)glDisable(GL_POINT_SMOOTH);

//    update();
}


void glView::mousePressEvent(QMouseEvent* mo){
    //запоминаем координату мышки
    mPos = mo->pos();

}
void glView::mouseMoveEvent(QMouseEvent* mo){
    xRot = 1/M_PI*(mo->pos().y() - mPos.y());
    yRot = 1/M_PI*(mo->pos().x() - mPos.x());
    update();
}



void glView::projection()
{
    if(ortho){
        glOrtho(-1,1,-1,1, 1, 99999);
        glTranslatef(0, 0, -4);
    }else  {
        glFrustum(-1,1,-1,1, 1, 99999);
        glTranslatef(0, 0, -3);
    }
}


void glView::drawObj(char *file, bool points){
    ExtremeValues extreme_values;
    Point *all_points;
    int top_pointers = 0;
    int count_surfaces = 0;
    all_points = point_reading(file, &top_pointers, &extreme_values);
    figure_centering(top_pointers, &extreme_values, all_points);

    Surface *all_surfaces;
    all_surfaces = (Surface *)malloc(SIZE * sizeof(Surface));
    all_surfaces = surface_formation(file,&count_surfaces,
                                     all_surfaces, all_points);

    glPointSize(vertex_size);
    glLineWidth(edge_size);
    if(dotted_edge){
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x1010);
    }
    for(int i = 0; i < count_surfaces; ++i){

        for(int j = 0; j < all_surfaces[i].amount_of_points; ++j){
            if(points){
                glBegin(GL_POINTS);
            }else {
                glBegin(GL_LINE_LOOP);
            }
            if(!points)glColor3d(edge_color.redF(), edge_color.greenF(),edge_color.blueF());
            if(points)glColor3d(vertex_color.redF(), vertex_color.greenF(),vertex_color.blueF());
            glVertex3d(all_surfaces[i].one_point[j]->x,all_surfaces[i].one_point[j]->y,all_surfaces[i].one_point[j]->z);
        }
        glEnd();

    }

    if(dotted_edge)glDisable(GL_LINE_STIPPLE);

         for (int i = 0; i < count_surfaces; ++i) {
             free(all_surfaces[i].one_point);
         }
         free(all_surfaces);
    free(all_points);

}




//void glView::drawObj(char * s){
//    float a = 0.5;
//    float ver[] = {
//        -a, -a, a,  a,-a, a,  a, a, a,  -a, a, a,
//        a, -a, -a,  -a,-a, -a,  -a, a, -a,  a, a, -a,

//        -a, -a, -a,  -a,-a, a,  -a, a, a, -a, a, -a,


//        a, -a, a,  a,-a, -a,  a, a, -a, a, a, a,

//        -a, -a, a,  a,-a, a,  a, -a, -a, -a, -a, -a,
//        -a, a, a,  a,a, a,  a, a, -a, -a, a, -a,


//    };

//    float col[] = {
//          1, 0, 0,  1, 0, 0 ,  1, 0, 0,  1, 0, 0,
//          0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,
//          1, 1, 0,  1, 1, 0,  1, 1, 0,  1, 1, 0,
//          0, 1, 1,  0, 1, 1,  0, 1, 1,  0, 1, 1,
//          1, 0, 1,  1, 0, 1,  1, 0, 1,  1, 0, 1,
//          1, 0.5, 0.5,  1, 0.5, 0.5,  1, 0.5, 0.5,  1, 0.5, 0.5
//    };
//    glVertexPointer(3, GL_FLOAT, 0, ver);
//    glEnableClientState(GL_VERTEX_ARRAY);

//    glColorPointer(3, GL_FLOAT, 0, col);
//    glEnableClientState(GL_COLOR_ARRAY);

//    glDrawArrays(GL_QUADS, 0, 24);

//    glDisableClientState(GL_COLOR_ARRAY);
//    glDisableClientState(GL_VERTEX_ARRAY);
//}
