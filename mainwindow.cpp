#include "mainwindow.h"
#include "qgigimageviewer.h"
#include "qgigimagegraphicsitem.h"
#include "qgigimagegraphicsview.h"

#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
	, m_viewer(0)
{
	// m_viewer = new QGigImageViewer;

	QGraphicsScene* scene = new QGraphicsScene;
	QGigImageGraphicsItem* item = new QGigImageGraphicsItem;
	scene->addItem(item);

	QGraphicsView* view = new QGigImageGraphicsView;
	view->setScene(scene);
	// view->scale(4.0, 4.0);

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(view);
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

MainWindow::~MainWindow()
{
}
