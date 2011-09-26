#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <math.h>
#include "GLWidget.h"
#include "debug.h"

// OpenGL limitation: a textures width and height need to be powers of 2
//

void setup2d_org(GLsizei width, GLsizei height)
{

/*
_glFormat = GL_RGB;  // Better since QImage RGBA is BGRA
_glType = GL_UNSIGNED_BYTE;

QGL::setPreferredPaintEngine(QPaintEngine::OpenGL2);

QGLFormat glFmt;
glFmt.setSwapInterval(1); // 1= vsync on 
glFmt.setAlpha(GL_RGBA==_glFormat);
glFmt.setRgba(GL_RGBA==_glFormat); 
glFmt.setDoubleBuffer(true); // default
glFmt.setOverlay(false);
glFmt.setSampleBuffers(false);
QGLFormat::setDefaultFormat(glFmt);

setAttribute(Qt::WA_OpaquePaintEvent,true);
setAttribute(Qt::WA_PaintOnScreen,true);   
*/

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);    // first edit the projection matrix
    glLoadIdentity();               // clear the projection matrix

    //Creating an orthoscopic view matrix going from -scale -> scale in each
    //dimension on the screen (x, y, z).
    int scale = 1.0f;
    glOrtho(-scale, scale, -scale, scale, -scale, scale);

    glMatrixMode(GL_MODELVIEW);   // now editing the model-view matrix.
    glLoadIdentity();         //Clearing the model-view matrix.

    //Disabling the depth test (z will not be used to tell what object
    //will be shown above another, only the order in which I draw them.)
    glDisable(GL_DEPTH_TEST);
}


/*  Create checkerboard texture  */
#define checkImageWidth 512
#define checkImageHeight 424
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;
QImage myTexture, myImage;

void makeCheckImage(void)
{
   bool result = myImage.load("./aleste21.png");
   if (result)
   {
        myTexture = QGLWidget::convertToGLFormat(myImage);
       nw_debug("image loaded!\n");
   }
   else
   {
       nw_debug("image not found!\n");
   }

   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}


void setup2d(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);    // first edit the projection matrix
    glLoadIdentity();               // clear the projection matrix

    // creating an orthoscopic view matrix equal in size to
    // the projection matrix
    glOrtho(0, width, 0, height, 0, -10);
    glMatrixMode(GL_MODELVIEW);   // now editing the model-view matrix.

    //Disabling the depth test, z will not be used to tell what object
    //will be shown above another, only the order in which I draw them. (painters algorithm)
    glDisable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // try GL_LINEAR for bi-linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /*
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                checkImage);
    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myTexture.width(),
        myTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
        myTexture.bits());
}

int DrawGLScene_2shapes(GLfloat width, GLfloat height)					// Here's Where We Do All The Drawing
{
    //glClearColor(0.0f,0.0f,5.0f,1.0f);                // background color
	glClear(GL_COLOR_BUFFER_BIT);	                    // clear Screen
	glLoadIdentity();									// Reset The Current Modelview Matrix

	glTranslatef(20.0f, 20.0f, 1.0f);				    // x,y origin is in the lower-left corner
    glScalef(100.0f, 100.0f, 1.0f);

    glBegin(GL_TRIANGLES);
	    glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
	    glVertex3f(1.0f, 2.0f, 0.0f);					// Top
	    glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
	    glVertex3f(0.0f, 0.0f, 0.0f);					// Bottom Left
	    glColor3f(0.0f,0.0f,1.0f);					    // Set The Color To Blue
	    glVertex3f(2.0f, 0.0f, 0.0f);					// Bottom Right
    glEnd();

	glTranslatef(3.0f, 1.0f, 0.0f);						// Move Right 3 Units
    glScalef(2.0f, 2.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    // bind texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);

	glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 1.0f, 0.0f);					// Top Left
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 0.0f);					// Top Right
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 0.0f, 0.0f);					// Bottom Right
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);					// Bottom Left
	glEnd();											// Done Drawing The Quad
	glFlush();
	glDisable(GL_TEXTURE_2D);
	return TRUE;										// Keep Going
}

float theta = 0.0;

int DrawGLScene(GLfloat width, GLfloat height)
{
	glClear(GL_COLOR_BUFFER_BIT);	                    // clear Screen
	glLoadIdentity();									// Reset The Current Modelview Matrix

    glScalef(width, height, 1.0f);
    glEnable(GL_TEXTURE_2D);

    glRotatef( theta, 0.0f, 0.0f, 1.0f );

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, texName);

	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 1.0f, 0.0f);					// Top Left
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 0.0f);					// Top Right
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 0.0f, 0.0f);					// Bottom Right
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);					// Bottom Left
	glEnd();											// Done Drawing The Quad

    glFlush();
	glDisable(GL_TEXTURE_2D);
	return TRUE;										// Keep Going
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    setup2d(this->width(), this->height());
    nw_debug("width: %u, height: %u\n", this->width(), this->height());
    DrawGLScene(this->width(), this->height());
    swapBuffers();  // actual paint the screen
}


/*
void GLWidget::resizeEvent(QResizeEvent* event)
{
    nw_debug("resizeEvent width: %u, height: %u\n", this->width(), this->height());
}
*/

void GLWidget::animate()
{
    theta += 1.0f;
    repaint();
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    makeCheckImage();
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(1);

}

GLWidget::~GLWidget()
{
}
