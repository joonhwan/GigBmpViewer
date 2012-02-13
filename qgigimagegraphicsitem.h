#pragma

#include <QGraphicsItem>

class GigBmpImage;
class QRubberBand;

class QGigImageGraphicsItem : public QGraphicsItem
{
public:
    QGigImageGraphicsItem(GigBmpImage* image=0);
    virtual ~QGigImageGraphicsItem();

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
					   QWidget* widget);
	GigBmpImage* image(void)
	{
		return m_image;
	}
protected:
	// virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	// virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event );
	// virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

	GigBmpImage* m_image;
	// QRubberBand * m_rubberBand;
    // QPoint m_rubberBandOrigin;
};
