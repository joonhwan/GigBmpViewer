#include "mainwindow.h"
#include "qgigimageviewer.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
	, m_viewer(0)
{
	m_viewer = new QGigImageViewer;

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_viewer);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

MainWindow::~MainWindow()
{
}
