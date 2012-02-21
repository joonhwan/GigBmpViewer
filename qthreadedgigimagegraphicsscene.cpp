#include "qthreadedgigimagegraphicsscene.h"
#include "tiledimagegraphicsitem.h"
#include "gigbmpimage.h"
#include "gigrenderthread.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QtCore>

QThreadedGigImageGraphicsScene::QThreadedGigImageGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_thread(0)
	, m_roi(0)
	, m_rubberBand(0)
{
	m_thread = new GigRenderThread(this);
	qRegisterMetaType<QPixmap>("QPixmap");
	connect(m_thread, SIGNAL(renderedImage(TiledImageGraphicsItem*,QImage)),
			SLOT(updateCachedImage(TiledImageGraphicsItem*,QImage)));
	connect(m_thread, SIGNAL(cachedImage(TiledImageGraphicsItem*)),
			SLOT(updateCachedImage(TiledImageGraphicsItem*)));
}

QThreadedGigImageGraphicsScene::~QThreadedGigImageGraphicsScene()
{
	m_thread->stopService();
}

void QThreadedGigImageGraphicsScene::updateCachedImage(TiledImageGraphicsItem* item,
													   QImage image)
{
	// do it in gui thread
	QPixmap pixmap;
	pixmap.convertFromImage(image);
	item->setThumbnail(pixmap);
}

void QThreadedGigImageGraphicsScene::updateCachedImage(TiledImageGraphicsItem* item)
{
	item->update();
}

void QThreadedGigImageGraphicsScene::finished(void)
{
	// something to do?
}

bool QThreadedGigImageGraphicsScene::loadImage(const QString& filePath)
{
    bool done = false;

	m_thread->stopService();

	QList<QGraphicsItem*> allItems = this->items();
	foreach(QGraphicsItem* item, allItems) {
		removeItem(item);
	}
	if(m_roi) {
		removeItem(m_roi);
		delete m_roi;
		m_roi = 0;
	}
	QBrush brush[2] = {
		QBrush(Qt::red),
		QBrush(Qt::blue)
	};
	if(m_thread->startService(filePath) &&
	   m_thread->image()) {
		int index = 0;
		QSize imageSize = m_thread->image()->Size();
		int itemWidth = imageSize.width();///16;
		int itemHeight = 1024;//128*4;
		QList<TiledImageGraphicsItem*> itemsCreated;
		for(int x=0; x<imageSize.width(); x+=itemWidth) {
			for(int y=0; y<imageSize.height(); y+=itemHeight) {
				QRectF region(x,y,
							  qMin(itemWidth, imageSize.width()-x), qMin(itemHeight, imageSize.height()-y));
				TiledImageGraphicsItem* item = new TiledImageGraphicsItem(region);
				item->setIndex(index);
				item->setBrush(brush[index%2]); ++index;
				addItem(item);

				itemsCreated << item;
			}
		}

		m_thread->resetItemCount(itemsCreated.size());
		foreach(TiledImageGraphicsItem* item, itemsCreated) {
			m_thread->render(item);
		}

		QColor brushColor = Qt::yellow;
		brushColor.setAlpha(100);
		m_roi = addRect(QRectF());
		m_roi->setBrush(brushColor);
		m_roi->setPen(QPen());

		done = true;
	}

	return done;
}

//virtual
void QThreadedGigImageGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
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
void QThreadedGigImageGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF currPosF = mouseEvent->scenePos() + QPointF(-0.5, -0.5);
	// qDebug() << "currPos:" << currPosF;

	QList<QGraphicsItem*> itemsOver = items(currPosF);
	foreach(QGraphicsItem* item, itemsOver) {
		if(TiledImageGraphicsItem* tile=qgraphicsitem_cast<TiledImageGraphicsItem*>(item)) {
			QColor color = tile->colorAt(currPosF);
			QPoint p = currPosF.toPoint();
			emit colorDetected(p.x(), p.y(), color);
		}
	}
	if(m_rubberBand) {
		QRectF movingRect = QRectF(m_rubberBandOrigin,
								   mouseEvent->scenePos()).normalized();
		m_rubberBand->setRect(sceneRect().intersected(movingRect));
	}
	__super::mouseMoveEvent(mouseEvent);
}

//virtual
void QThreadedGigImageGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
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

