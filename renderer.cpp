#include "renderer.h"

Renderer::Renderer(QObject *parent)
	: QObject(parent)
{

}

Renderer::~Renderer()
{

}

bool Renderer::openImage(const QString& filePath)
{
	if(m_image) {
		delete m_image;
		m_image = 0;
	}
	m_image = new GigBmpImage;
	if(m_image->Open(filePath)) {
		reset();
	} else {
		delete m_image;
		m_image = 0;
	}
	return (m_image!=0);
}

void Renderer::closeImage(void)
{
}

void Renderer::render(TiledImageGraphicsItem* item, int priority)
{
	RenderQueueItem queueItem;
	queueItem.code = RenderQueueItem::CACHE_SCALED;
	queueItem.priority = priority;
	queueItem.graphicsItem = item;
	m_eventQueue.push(queueItem);
}

QImage Renderer::cachedRawImageOf(TiledImageGraphicsItem* item)
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

	// qDebug() << "search cache out of " << m_cache.size() << " -- > null ? :" << image.isNull();

	return image;
}

void Renderer::cacheRawImage(TiledImageGraphicsItem* item)
{
	int recachePriority = 10000;
	RenderQueueItem queueItem;
	queueItem.code = RenderQueueItem::CACHE_RAW;
	queueItem.priority = recachePriority;
	queueItem.graphicsItem = item;
	m_eventQueue.push(queueItem);
}
