#pragma

#include <QGraphicsItem>

class GigBmpImage;

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
	GigBmpImage* m_image;
};
