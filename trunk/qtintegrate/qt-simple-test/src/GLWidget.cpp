#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include <math.h>
#include "glwidget.h"
#include "debug.h"

void setup2d_org(GLsizei width, GLsizei height)
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


/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

void makeCheckImage(void)
{
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
    
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
    //glClearColor(0.0f,0.0f,5.0f,1.0f);                // background color
	glClear(GL_COLOR_BUFFER_BIT);	                    // clear Screen 
	glLoadIdentity();									// Reset The Current Modelview Matrix					
	
	glTranslatef(200.0f, 200.0f, 1.0f);				    // x,y origin is in the lower-left corner, z is 
	glScalef(100.0f, 100.0f, 100.0f);

	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
						
	glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units	

    glEnable(GL_TEXTURE_2D);

    // bind texture	
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);

	glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	glEnd();											// Done Drawing The Quad
	glFlush();
	glDisable(GL_TEXTURE_2D);


	return TRUE;										// Keep Going
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    setup2d(this->width(), this->height());
    nw_debug("width: %u, height: %u\n", this->width(), this->height());
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
