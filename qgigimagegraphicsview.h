#ifndef QGIGIMAGEGRAPHICSVIEW_H
#define QGIGIMAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class QLabel;
class QProgressBar;
class QThreadedGigImageGraphicsScene;

class QGigImageGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	QGigImageGraphicsView(QWidget *parent=0);
	~QGigImageGraphicsView();

	void init(QThreadedGigImageGraphicsScene* scene);
	void ZoomIn(void);
	void ZoomOut(void);
	void ResetZoom(void);
	void zoom(qreal factor, QPointF centerPoint);
	qreal zoomLevel(void) const;
protected slots:
	void updateProgress(int doneCount, int totalCount);
protected:
	virtual void wheelEvent ( QWheelEvent * e);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void scaleView(qreal scaleFactor);

	int m_panning;
	QLabel* pixelInfoLabel;
	QProgressBar* renderProgress;
	QLabel* renderStatus;
};

#endif // QGIGIMAGEGRAPHICSVIEW_H
