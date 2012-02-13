#pragma

#include <QGraphicsItem>

class GigBmpImage;

class QGigImageGraphicsItem : public QGraphicsItem
{
public:
    QGigImageGraphicsItem();
    virtual ~QGigImageGraphicsItem();

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
					   QWidget* widget);

protected:
	GigBmpImage* m_image;
};
