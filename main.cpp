#include "mainwindow.h"
#include <QtGui/QApplication>
#include <limits.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// Q_INIT_RESOURCE(qgigimageviewer);
	MainWindow w;
	w.show();
	return a.exec();
}
