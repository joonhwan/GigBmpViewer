#include "qgigimagegraphicsscene.h"
#include "qgigimagegraphicsitem.h"
#include "gigbmpimage.h"
#include "tiledimagegraphicsitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

QGigImageGraphicsScene::QGigImageGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_rubberBand(0)
	, m_roi(0)
	, m_imageItem(0)
{
}

QGigImageGraphicsScene::~QGigImageGraphicsScene()
{
}

bool QGigImageGraphicsScene::loadImage(const QString& filePath)
{
    bool done = false;

	if(m_imageItem) {
		removeItem(m_imageItem);
		delete m_imageItem;
		m_imageItem = 0;
	}
	if(m_roi) {
		removeItem(m_roi);
		delete m_roi;
		m_roi = 0;
	}
	GigBmpImage* image = new GigBmpImage;
	if(image->Open(filePath)) {
		m_imageItem = new QGigImageGraphicsItem(image); // attach 'image' to the new item
		addItem(m_imageItem);
		QColor brushColor = Qt::yellow;
		brushColor.setAlpha(100);
		m_roi = addRect(QRectF());
		m_roi->setBrush(brushColor);
		m_roi->setPen(QPen());
		done = true;
	} else {
		delete image;
	}

	return done;
}

//virtual
void QGigImageGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(!m_rubberBand) {
		QColor brushColor = Qt::yellow;
		brushColor.setAlpha(100);
		m_rubberBand = new QGraphicsRectItem;
		m_rubberBand->setPen(QPen(Qt::red));
		m_rubberBand->setBrush(brushColor);
		m_rubberBandOrigin = mouseEvent->scenePos();
		m_rubberBand->setRect(QRectF(m_rubberBandOrigin,
									 m_rubberBandOrigin));
		addItem(m_rubberBand);
	}
	__super::mousePressEvent(mouseEvent);
}

//virtual
void QGigImageGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(m_rubberBand) {
		QRectF movingRect = QRectF(m_rubberBandOrigin,
								   mouseEvent->scenePos()).normalized();
		m_rubberBand->setRect(sceneRect().intersected(movingRect));
	}
	QPointF posF = mouseEvent->pos();
	QList<QGraphicsItem*> itemsOver = items(posF);
	foreach(QGraphicsItem* item, itemsOver) {
		if(TiledImageGraphicsItem* tile=qgraphicsitem_cast<TiledImageGraphicsItem*>(item)) {
			QColor color = tile->colorAt(posF);
			QPoint pos = posF.toPoint();
			emit colorDetected(pos.x(), pos.y(), color);
		}
	}
	__super::mouseMoveEvent(mouseEvent);
}

//virtual
void QGigImageGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if(m_rubberBand) {
		if(m_roi) {
			m_roi->setRect(m_rubberBand->boundingRect());
		}
		removeItem(m_rubberBand);
		delete m_rubberBand;
		m_rubberBand = 0;
	}
	__super::mouseReleaseEvent(mouseEvent);
}

