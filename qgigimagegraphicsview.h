#ifndef QGIGIMAGEGRAPHICSVIEW_H
#define QGIGIMAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class QGigImageGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	QGigImageGraphicsView(QWidget *parent=0);
	~QGigImageGraphicsView();

	void ZoomIn(void);
	void ZoomOut(void);
	void ResetZoom(void);
	void zoom(qreal factor, QPointF centerPoint);
	qreal zoomLevel(void) const;
protected:
	virtual void wheelEvent ( QWheelEvent * e);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent * event);
	void scaleView(qreal scaleFactor);

	int m_panning;
};

#endif // QGIGIMAGEGRAPHICSVIEW_H
