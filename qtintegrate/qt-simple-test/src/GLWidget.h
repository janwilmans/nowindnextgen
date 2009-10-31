#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

class GLWidget : public QGLWidget
{
    // meta object linker errors?
    //Q_OBJECT 

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    
    virtual void animate();
    
    virtual void paintEvent(QPaintEvent *event);

};

#endif // GLWIDGET_H
