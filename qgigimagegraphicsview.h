#ifndef QGIGIMAGEGRAPHICSVIEW_H
#define QGIGIMAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class DecoratedSimpleLabel;
class QLabel;
class QThreadedGigImageGraphicsScene;
class TransparentProgressBar;

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
	void updatePixelInfo(int x, int y, QColor color);
protected:
	virtual void wheelEvent ( QWheelEvent * e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void scaleView(qreal scaleFactor);

	int m_panning;
	DecoratedSimpleLabel* pixelInfoLabel;
	TransparentProgressBar* renderProgress;
	DecoratedSimpleLabel* renderStatus;
};

#endif // QGIGIMAGEGRAPHICSVIEW_H
