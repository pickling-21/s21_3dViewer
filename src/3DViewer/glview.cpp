#include "glview.h"
#include "ui_glview.h"

glView::glView(QWidget *parent) :
    QOpenGLWidget(parent)
{

    background_color.setRgb(0, 0, 0);
    edge_color.setRgb(255, 255, 255);
    vertex_color.setRgb(255, 255, 255);
}


void glView:: initializeGL(){
//    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);


}
void glView:: resizeGL(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1, 1, 99999);
}



// вершинный буфер

void glView:: paintGL(){
    glClearColor(background_color.redF(), background_color.greenF(), background_color.blueF(), background_color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glTranslatef(0,0, -2);
    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);
    char a[]  = "/Users/pickling/0projects/3D/s21_3dViewer/src/models/skull.obj";
    drawObj(a);

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

uint64_t glView::count_edge(char *file)
{
     Point *all_points;
     int top_pointers = 0;
     int count_surfaces = 0;
     edge_amount =0;
     ExtremeValues extreme_values;
    all_points = point_reading(file, &top_pointers, &extreme_values);
    Surface *all_surfaces;
    all_surfaces = (Surface *)malloc(SIZE * sizeof(Surface));
    all_surfaces = surface_formation(file, top_pointers, &count_surfaces,
                                     all_surfaces, all_points);
    for(int i = 0; i < count_surfaces; i++){
        edge_amount+=(uint64_t)all_surfaces[i].amount_of_points;
    }
    edge_amount/=2;
    return edge_amount;
}


void glView::drawObj(char *file){
    //    char *file = {"cube.obj"};
    main88();
    printf("%s", file);
    Point *all_points;
    int top_pointers = 0;
    int count_surfaces = 0;
    double alfa = 45;
    alfa = (alfa * M_PI)/180;
    ExtremeValues extreme_values;


    all_points = point_reading(file, &top_pointers, &extreme_values);
    figure_centering(top_pointers, &extreme_values, all_points);
    figure_rotation_z(top_pointers,alfa, all_points);
//    figure_move_x(top_pointers,2, all_points);
    Surface *all_surfaces;
    all_surfaces = (Surface *)malloc(SIZE * sizeof(Surface));
    all_surfaces = surface_formation(file, top_pointers, &count_surfaces,
                                     all_surfaces, all_points);

    for(int i = 0; i < count_surfaces; i++){
        for(int j = 0; j < all_surfaces[i].amount_of_points; j++){
            glBegin(GL_LINE_LOOP);
            glVertex3d(all_surfaces[i].one_point[j]->x,all_surfaces[i].one_point[j]->y,all_surfaces[i].one_point[j]->z);
        }glEnd();
    }

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
