#include <QApplication>

#include "MainWindow.h"

using namespace std;

int main( int argc, char **argv )
{
	QApplication app(argc, argv);

	MainWindow mainwindow;
	mainwindow.show();

	return app.exec();
}

#ifdef WIN32

#include <windows.h>

int WINAPI WinMain(      
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    return main(__argc, __argv);
}

#endif
