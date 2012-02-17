#include "gigrenderthread.h"
#include "gigbmpimage.h"
#include "tiledimagegraphicsitem.h"

#include <QDebug>
#include <limits.h>

namespace {

const int MAX_CACHE_COUNT = 50;

}

GigRenderThread::GigRenderThread(QObject *parent)
	: QThread(parent)
	, m_image(0)
{

}

GigRenderThread::~GigRenderThread()
{

}

void GigRenderThread::render(TiledImageGraphicsItem* item, int priority)
{
	RenderQueueItem queueItem;
	queueItem.code = RenderQueueItem::CACHE_SCALED;
	queueItem.priority = priority;
	queueItem.graphicsItem = item;
	m_eventQueue.push(queueItem);
}

QImage GigRenderThread::cachedRawImageOf(TiledImageGraphicsItem* item)
{
	QMutexLocker locker(&m_cacheLock);

	QImage image;
	CacheDataList::iterator it = m_cache.begin();
	for(; it!=m_cache.end(); ++it) {
		if((*it).item==item) {
			image = (*it).image;
			break;
		}
	}

	qDebug() << "search cache out of " << m_cache.size() << " -- > null ? :" << image.isNull();

	return image;
}

void GigRenderThread::cacheRawImage(TiledImageGraphicsItem* item)
{
	int recachePriority = 10000;
	RenderQueueItem queueItem;
	queueItem.code = RenderQueueItem::CACHE_RAW;
	queueItem.priority = recachePriority;
	queueItem.graphicsItem = item;
	m_eventQueue.push(queueItem);
}

bool GigRenderThread::startService(const QString& filePath)
{
	m_stopped = false;
	if(m_image) {
		delete m_image;
		m_image = 0;
	}
	m_image = new GigBmpImage;
	emit renderStatusChanged(tr("Opening new image..."));
	if(m_image->Open(filePath)) {
		emit renderStatusChanged(tr("Caching..."));
		reset();
		start();
	} else {
		emit renderStatusChanged(tr("Unable to open image!"));
		delete m_image;
		m_image = 0;
	}
	return (m_image!=0);
}

void GigRenderThread::stopService(unsigned long waitTime)
{
	m_stopped = true;

	RenderQueueItem queueItem;
	queueItem.priority = INT_MAX;
	queueItem.graphicsItem = 0;
	m_eventQueue.push(queueItem);

	wait(waitTime);
	m_eventQueue.clear();
}

void GigRenderThread::reset(void)
{
	m_eventQueue.clear();
}

void GigRenderThread::resetItemCount(int count)
{
	emit renderProgress(0, count);
	m_itemCount = count;
	m_renderedItemCount = 0;
}

GigBmpImage* GigRenderThread::image(void)
{
	return m_image;
}

//virtual
void GigRenderThread::run(void)
{
	const int scaleDownRatio = 8; //256;

	forever {
		RenderQueueItem queueItem;
		if(m_eventQueue.waitAndPop(queueItem)) {
			TiledImageGraphicsItem* item = queueItem.graphicsItem;
			switch(queueItem.code)
			{
			case RenderQueueItem::CACHE_SCALED:
				if(item && !item->thumbnailed()) {
					QSize scaleDownSize = item->boundingRect().size().toSize();
					scaleDownSize/=scaleDownRatio;
					QImage image = m_image->Image(item->region());
					qDebug() << "region[" << item->region() << "] done!";
					emit renderedImage(item, image.scaled(scaleDownSize));
					emit renderProgress(++m_renderedItemCount, m_itemCount);
				}
				break;
			case RenderQueueItem::CACHE_RAW:
				if(item) {
					QSize scaleDownSize = item->boundingRect().size().toSize();
					scaleDownSize/=scaleDownRatio;
					QImage image = m_image->Image(item->region());
					qDebug() << "region[" << item->region() << "] done!";
					addCache(item, image);
				}
				break;
			}
		}
		if(m_stopped) {
			break;
		}
	}
}

void GigRenderThread::addCache(TiledImageGraphicsItem* item, QImage image)
{
	QMutexLocker locker(&m_cacheLock);

	if(m_cache.size() > MAX_CACHE_COUNT) {
		qDebug() << "remove a cache data";
		m_cache.takeFirst();
	}
	CacheData data;
	data.item = item;
	data.image = image;
	m_cache.push_back(data);
	emit cachedImage(item);

	qDebug() << "new cache is added. count : " << m_cache.size();
}

