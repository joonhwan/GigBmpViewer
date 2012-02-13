#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QGigImageViewer;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent=0);
	~MainWindow();

private:
	QGigImageViewer* m_viewer;
};

#endif // MAINWINDOW_H
