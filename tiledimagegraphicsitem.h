#pragma once

#include <QBrush>
#include <QGraphicsItem>

class GigBmpImage;

class TiledImageGraphicsItem : public QGraphicsItem
{
public:
	enum { Type = UserType + 1 };
	TiledImageGraphicsItem(const QRectF& region = QRectF());
	virtual ~TiledImageGraphicsItem(void);
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
					   QWidget* widget);
    virtual int type() const
    {
        return Type;
    }
	void setIndex(int index)
	{
		m_index = index;
	}
	void setBrush(const QBrush& brush)
	{
		m_brush = brush;
	}
	const QRectF& region(void) const
	{
		return m_region;
	}
	void setThumbnail(QPixmap thumbnail);
	bool thumbnailed(void) const
	{
		return m_thumbnailed;
	}
	QColor colorAt(QPointF scenePos);
protected:
	bool m_thumbnailed;
	int m_index;
	QBrush m_brush;
	QRectF m_region;
	QPixmap m_scaledImage;
	QImage m_scaledImageI;
};
