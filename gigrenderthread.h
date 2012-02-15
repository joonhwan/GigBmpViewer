#pragma once

#include "renderqueue.h"
#include <QThread>
#include <QHash>
#include <QImage>
#include <QMutex>
#include <QList>

class GigBmpImage;

class GigRenderThread : public QThread
{
	Q_OBJECT
	struct CacheData
	{
		TiledImageGraphicsItem* item;
		QImage image;
	};
	typedef QList<CacheData> CacheDataList;
public:
	GigRenderThread(QObject *parent);
	~GigRenderThread();

	void render(TiledImageGraphicsItem* item, int priority=0);
	QImage cachedRawImageOf(TiledImageGraphicsItem* item);
	void cacheRawImage(TiledImageGraphicsItem* item);
	bool startService(const QString& filePath);
	void stopService(unsigned long waitTime=ULONG_MAX);
	void reset(void);
	GigBmpImage* image(void);
signals:
	void renderedImage(TiledImageGraphicsItem* item, QImage scaleDown);
	void cachedImage(TiledImageGraphicsItem* item);
protected:
	virtual void run(void);
	void addCache(TiledImageGraphicsItem* item, QImage image);

	bool m_stopped;
	GigBmpImage* m_image;
	QWaitCondition m_queueNotEmpty;
	RenderQueue m_eventQueue;
	mutable QMutex m_cacheLock;
	CacheDataList m_cache;
};
