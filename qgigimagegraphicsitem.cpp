#include "qgigimagegraphicsitem.h"
#include "gigbmpimage.h"

#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QRubberBand>
#include <QGraphicsSceneMouseEvent>

QGigImageGraphicsItem::QGigImageGraphicsItem(GigBmpImage* image)
	: m_image(image)
	// , m_rubberBand(0)
{
	setFlags(flags() | QGraphicsItem::ItemUsesExtendedStyleOption);
}

//virtual
QGigImageGraphicsItem::~QGigImageGraphicsItem()
{
	delete m_image;
	// delete m_rubberBand;
}

//virtual
QRectF QGigImageGraphicsItem::boundingRect() const
{
	QSizeF size(0.,0.);
	if(m_image) {
		size = m_image->Size();
	}
	return QRectF(QPointF(0,0), //QPointF(-size.width()/2, -size.height()/2),
				  size);
}

//virtual
void QGigImageGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
								  QWidget* widget)
{
	static int count = 0;
	int y = option->exposedRect.top();
	qDebug() << "index[" << count++ << "]"
			 << "exposed rect: " << option->exposedRect
			 << "exposed rect(scene): " << mapRectToScene(option->exposedRect)
			 // << "transform : " << transform()
			 << "y : " << y;
	if(m_image) {
		m_image->Draw(painter, option->exposedRect, option->exposedRect);
	}
}

// void QGigImageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
// {
// 	m_rubberBandOrigin = event->screenPos();
// 	if (!m_rubberBand)
// 		m_rubberBand = new QRubberBand(QRubberBand::Rectangle,0);
// 	m_rubberBand->setGeometry(QRect(m_rubberBandOrigin, QSize()));
// 	m_rubberBand->show();
// }

// void QGigImageGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event )
// {
// 	QRectF box = this->boundingRect();
// 	QPointF mapPoint = mapFromScene(event->pos());
// 	if(box.contains(mapPoint)) {
// 		QRect rb = QRect(m_rubberBandOrigin, event->screenPos());
// 		qDebug() << "rb: " << rb;
// 		m_rubberBand->setGeometry(rb.normalized());
// 	}
// }

// void QGigImageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event )
// {
// 	QRect bandBox = m_rubberBand->mapFromGlobal(m_rubberBand->geometry());
// 	QGraphicsRectItem* item = scene()->addRect(bandBox, QPen(Qt::red));
// 	m_rubberBand->hide();
// }
