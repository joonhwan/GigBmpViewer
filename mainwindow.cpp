#include "mainwindow.h"
#include "qgigimageviewer.h"
#include "gigbmpimage.h"
#include "qgigimagegraphicsitem.h"
#include "qgigimagegraphicsview.h"

#include <QAction>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_view(0)
	, m_imageItem(0)
{
	QGraphicsScene* scene = new QGraphicsScene;

	m_view = new QGigImageGraphicsView;
	m_view->setScene(scene);
	// view->scale(4.0, 4.0);

	setCentralWidget(m_view);

	createActions();
	createMenus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::open(void)
{
	QString filePath = QFileDialog::getOpenFileName(this,
													tr("Open a gray scale image"),
													"", // current dir
													tr("BMP File(*.bmp)"));
	if(!filePath.isEmpty()) {
		QGraphicsScene* scene = m_view->scene();
		if(m_imageItem) {
			scene->removeItem(m_imageItem);
			delete m_imageItem;
			m_imageItem = 0;
		}
		GigBmpImage* image = new GigBmpImage;
		if(image->Open(filePath)) {
			m_imageItem = new QGigImageGraphicsItem(image);
			scene->addItem(m_imageItem);
			m_view->resetTransform();
		}
	}
}

void MainWindow::exit(void)
{
}

void MainWindow::createActions()
{
	m_openAction = new QAction(tr("&Open"), this);
	m_openAction->setIcon(QIcon(":/QGigImageViewer/images/open.png"));
	m_openAction->setShortcut(tr("Ctrl+O"));
	m_openAction->setStatusTip(tr("Open a 8 bit grayscale image file in windows bmp format"));
	connect(m_openAction, SIGNAL(triggered()), this, SLOT(open()));

	m_exitAction = new QAction(tr("E&xit"), this);
	m_openAction->setIcon(QIcon(":/QGigImageViewer/images/exit.png"));
    m_exitAction->setShortcut(tr("Ctrl+Q"));
    m_exitAction->setStatusTip(tr("Exit the application"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus(void)
{
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_fileMenu->addAction(m_openAction);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_exitAction);
}

