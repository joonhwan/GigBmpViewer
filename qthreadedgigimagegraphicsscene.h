#ifndef QTHREADEDGIGIMAGEGRAPHICSSCENE_H
#define QTHREADEDGIGIMAGEGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QImage>
#include <QObject>

class GigRenderThread;
class GigBmpImage;
class TiledImageGraphicsItem;

struct ThumbnailWorkResult
{
	ThumbnailWorkResult()
		: valid(false)
		, item(0)
	{}
	bool valid;
	QImage image;
	TiledImageGraphicsItem* item;
};

class QThreadedGigImageGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QThreadedGigImageGraphicsScene(QObject *parent=0);
	virtual ~QThreadedGigImageGraphicsScene();
	GigRenderThread* thread()
	{
		return m_thread;
	}
Q_SIGNALS:
	void colorDetected(int x, int y, QColor color);
public Q_SLOTS:
	void updateCachedImage(TiledImageGraphicsItem* item, QImage pixmap);
	void updateCachedImage(TiledImageGraphicsItem* item);
	void finished(void);
	bool loadImage(const QString& filePath);
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	GigRenderThread* m_thread;
	QGraphicsRectItem* m_rubberBand;
	QGraphicsRectItem* m_roi;
	QPointF m_rubberBandOrigin;
};

#endif // QTHREADEDGIGIMAGEGRAPHICSSCENE_H
