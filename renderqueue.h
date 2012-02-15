#pragma once

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

class TiledImageGraphicsItem;

struct RenderQueueItem
{
	enum {
		CACHE_SCALED = 0,
		CACHE_RAW,
	};
	int code;
	int priority;
	TiledImageGraphicsItem* graphicsItem;
};

class RenderQueue : protected QQueue<RenderQueueItem>
{
	typedef QQueue<RenderQueueItem> Container;
public:
	void push(const RenderQueueItem& newItem)
	{
		QMutexLocker locker(&m_queueLock);
		bool inserted = false;
		Container::iterator it=Container::begin();
		for(;it!=Container::end(); ++it) {
			const RenderQueueItem& item = *it;
			if(item.priority <= newItem.priority) {
				Container::insert(it, newItem);
				inserted = true;
				break;
			}
		}
		if(!inserted) {
			Container::push_back(newItem);
		}
		Q_ASSERT(Container::size());
		m_queueNotEmpty.wakeOne();
	}
	bool empty(void) const
	{
		QMutexLocker locker(&m_queueLock);
		return Container::empty();
	}
	bool tryPop(RenderQueueItem& item)
	{
		QMutexLocker locker(&m_queueLock);
		bool popped = false;
		if(!Container::empty()) {
			item = Container::dequeue();
			popped = true;
		}
		return popped;
	}
	bool waitAndPop(RenderQueueItem& item)
	{
		QMutexLocker locker(&m_queueLock);
		bool popped = false;
		while(Container::empty()) {
			m_queueNotEmpty.wait(&m_queueLock);
		}
		if(!Container::empty()) {
			item = Container::dequeue();
			popped = true;
		}
		return popped;
	}
	void clear(void)
	{
		QMutexLocker locker(&m_queueLock);
		Container::clear();
	}
private:
	mutable QMutex m_queueLock;
	QWaitCondition m_queueNotEmpty;
	bool closed;
};
