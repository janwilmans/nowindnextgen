#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <math.h>
#include "glwidget.h"

void setup2d(GLsizei width, GLsizei height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);    // first edit the projection matrix
    glLoadIdentity();               // clear the projection matrix

    //Creating an orthoscopic view matrix going from -scale -> scale in each
    //dimension on the screen (x, y, z). 
    int scale = 3.0f;
    glOrtho(-scale, scale, -scale, scale, -scale, scale);
    
    glMatrixMode(GL_MODELVIEW);   // now editing the model-view matrix.
    glLoadIdentity();         //Clearing the model-view matrix.

    //Disabling the depth test (z will not be used to tell what object 
    //will be shown above another, only the order in which I draw them.)
    glDisable(GL_DEPTH_TEST);
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT);	// Clear Screen 
	
	glLoadIdentity();									// Reset The Current Modelview Matrix					
	
	glTranslatef(-1.5f,0.0f,0.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
						
	glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units	
	
	glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	glEnd();											// Done Drawing The Quad
	return TRUE;										// Keep Going
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    setup2d(this->width(), this->height());
    DrawGLScene();
    swapBuffers();  // actual paint the screen
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{

}

GLWidget::~GLWidget()
{
}
