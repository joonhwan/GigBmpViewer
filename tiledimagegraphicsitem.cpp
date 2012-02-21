#include "tiledimagegraphicsitem.h"
#include "qthreadedgigimagegraphicsscene.h"
#include "gigrenderthread.h"

#include <QDebug>
#include <QPainter>
#include <QImage>
#include <QStyleOptionGraphicsItem>

TiledImageGraphicsItem::TiledImageGraphicsItem(const QRectF& region)
{
	setPos(region.topLeft());
	m_region = region;
	m_thumbnailed = false;
}

TiledImageGraphicsItem::~TiledImageGraphicsItem()
{
}

//virtual
QRectF TiledImageGraphicsItem::boundingRect() const
{
	// QRectF box(-m_region.width()/2, -m_region.height()/2,
	// 		   m_region.width(), m_region.height());
	// return box;
	//return QRectF(QPointF(-0.5,-0.5), m_region.size()+QSizeF(0.5,0.5));
	return QRectF(QPointF(0,0), m_region.size());
}

//virtual
void TiledImageGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
								   QWidget* widget)
{
	QThreadedGigImageGraphicsScene* gs = (QThreadedGigImageGraphicsScene*)scene();
	if(m_scaledImage.isNull()) {
		// QRectF sceneRect = boundingRect();
		// qDebug() << m_region << " : drawing dummy image";
		// painter->fillRect(sceneRect, m_brush);
		int updatePriority = 1;
		gs->thread()->render(this, updatePriority);
	} else {
		bool drawn = false;
		qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
		bool useRawImage =  lod > 0.2;
		if(useRawImage)
		{
			QImage rawImage = gs->thread()->cachedRawImageOf(this);
			if(!rawImage.isNull()) {
				// qDebug() << m_region << " : drawing raw image";
				painter->drawImage(boundingRect(),
								   rawImage,
							   QRectF(QPointF(0,0), rawImage.size()));
				drawn = true;
			}
		}
		if(!drawn) {
			// qDebug() << m_region << " : drawing scaled image only.";
			if(useRawImage)
			{
				gs->thread()->cacheRawImage(this);
			}
			painter->drawPixmap(boundingRect(),
								m_scaledImage,
								QRectF(QPointF(0,0),
									   m_scaledImage.size()));
		}
		if(lod > 4.0) {
			QPen oldPen = painter->pen();

			painter->setPen(QColor(0x00,0xFF,0x00,60));
			QRectF clipBox = painter->clipBoundingRect();
			//qDebug() << "clipBox : " << clipBox << "option->rect(): " << option->rect;
			QRectF box = boundingRect();
			for(qreal x=box.left(); x<box.right(); x+=1) {
				painter->drawLine(x, box.top(), x, box.bottom());
			}
			for(qreal y=box.top(); y<box.bottom(); y+=1) {
				painter->drawLine(box.left(), y, box.right(), y);
			}

			painter->setPen(oldPen);
		}
	}

	// queue 'non-scaled image' drawing job.
}

void TiledImageGraphicsItem::setThumbnail(QPixmap thumbnail)
{
	if(!m_thumbnailed) {
		m_thumbnailed = true;
		m_scaledImage = thumbnail;
		m_scaledImageI = m_scaledImage.toImage();
		update();
	}
}

QColor TiledImageGraphicsItem::colorAt(QPointF scenePos)
{
	QColor color;
	QPoint pixelPos = mapFromScene(scenePos).toPoint();

	QThreadedGigImageGraphicsScene* gs = (QThreadedGigImageGraphicsScene*)scene();
	if(m_scaledImage.isNull()) {
		// color = ?
	} else {
		QImage rawImage = gs->thread()->cachedRawImageOf(this);
		if(!rawImage.isNull()) {
			color = rawImage.pixel(pixelPos);
		} else {
			color = m_scaledImageI.pixel(pixelPos);
		}
	}
	return color;
}
