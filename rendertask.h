#pragma once

#include <QRunnable>

class RenderTask : public QRunnable
{
public:
    RenderTask()
    virtual ~RenderTask();
	virtual void run(void);
};

class ScaleDownTask : public QRunnable
{
public:
	virtual void run(void);
protected:
	TiledImageGraphicsItem* item;
};
