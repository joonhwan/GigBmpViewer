#include "qgigimagegraphicsitem.h"
#include "gigbmpimage.h"

#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

QGigImageGraphicsItem::QGigImageGraphicsItem(GigBmpImage* image)
	: m_image(image)
{
	setFlags(flags() | QGraphicsItem::ItemUsesExtendedStyleOption);
}

//virtual
QGigImageGraphicsItem::~QGigImageGraphicsItem()
{
	if(m_image) {
		delete m_image;
	}
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

