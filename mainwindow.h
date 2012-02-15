#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QGigImageViewer;
class QGigImageGraphicsView;
class QGigImageGraphicsScene;
class QThreadedGigImageGraphicsScene;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent=0);
	~MainWindow();

protected slots:
	void open(void);
	void exit(void);

protected:
	void createActions();
    void createMenus();

	QGigImageGraphicsView* m_view;
	QThreadedGigImageGraphicsScene* m_scene;

	QAction *m_openAction;
	QAction *m_exitAction;

	QMenu* m_fileMenu;
};

#endif // MAINWINDOW_H
