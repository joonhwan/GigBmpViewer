#ifndef QGIGIMAGEJOBMANAGER_H
#define QGIGIMAGEJOBMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include <QQueue>

class QGigImageJob
{
public:
	virtual void doJob(void) = 0;
};

typedef QSharedPointer<QGigImageJob> QGigImageJobPtr;

class QGigImageJobManager : public QObject
{
	Q_OBJECT
public:
	QGigImageJobManager(QObject *parent);
	~QGigImageJobManager();

	void queue(QGigImageJobPtr& job);
private:
	QQueue<QGigImageJobPtr> m_jobs;
};

#endif // QGIGIMAGEJOBMANAGER_H
