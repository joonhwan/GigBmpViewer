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
				qDebug() << m_region << " : drawing raw image";
				painter->drawImage(boundingRect(),
								   rawImage,
							   QRectF(QPointF(0,0), rawImage.size()));
				drawn = true;
			}
		}
		if(!drawn) {
			qDebug() << m_region << " : drawing scaled image only.";
			if(useRawImage)
			{
				gs->thread()->cacheRawImage(this);
			}
			painter->drawPixmap(boundingRect(),
								m_scaledImage,
								QRectF(QPointF(0,0),
									   m_scaledImage.size()));
		}
	}

	// queue 'non-scaled image' drawing job.
}

void TiledImageGraphicsItem::setThumbnail(QPixmap thumbnail)
{
	if(!m_thumbnailed) {
		m_thumbnailed = true;
		m_scaledImage = thumbnail;
		update();
	}
}
