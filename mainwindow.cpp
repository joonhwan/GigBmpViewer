#include "mainwindow.h"
#include "qgigimageviewer.h"
#include "gigbmpimage.h"
#include "qgigimagegraphicsview.h"
#include "qgigimagegraphicsscene.h"
#include "qthreadedgigimagegraphicsscene.h"

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
	, m_scene(0)
{
	m_scene = new QThreadedGigImageGraphicsScene;
	m_view = new QGigImageGraphicsView;
	m_view->setScene(m_scene);
	// view->scale(4.0, 4.0);

	setCentralWidget(m_view);

	createActions();
	createMenus();

	// if(m_scene->loadImage("c:/temp/RECORD_110715_174326/left/GoldenImage.bmp")) {
	// 	m_view->resetTransform();
	// }
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
		if(m_scene->loadImage(filePath)) {
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

