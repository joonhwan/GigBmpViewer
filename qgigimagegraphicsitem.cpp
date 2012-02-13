#include "qgigimagegraphicsitem.h"
#include "gigbmpimage.h"

#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

QGigImageGraphicsItem::QGigImageGraphicsItem()
{
	m_image = new GigBmpImage;
	m_image->Open("c:/TestBigImage.bmp");
	setFlags(flags() | QGraphicsItem::ItemUsesExtendedStyleOption);
}

//virtual
QGigImageGraphicsItem::~QGigImageGraphicsItem()
{
	delete m_image;
}

//virtual
QRectF QGigImageGraphicsItem::boundingRect() const
{
	QSizeF size = m_image->Size();
	return QRectF(QPointF(0,0), //QPointF(-size.width()/2, -size.height()/2),
				  size);
}

//virtual
void QGigImageGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
								  QWidget* widget)
{
	static int count = 0;
	qDebug() << "index[" << count++ << "]"
			 << "exposed rect: " << option->exposedRect
			 << "exposed rect(scene): " << mapRectToScene(option->exposedRect)
			 << "transform : " << transform();

	m_image->Draw(painter, option->exposedRect, option->exposedRect);
}

