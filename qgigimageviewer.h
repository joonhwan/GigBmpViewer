#ifndef QGIGIMAGEVIEWER_H
#define QGIGIMAGEVIEWER_H

#include <QScrollArea>

class QGigImageViewer : public QScrollArea
{
	Q_OBJECT
public:
	QGigImageViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QGigImageViewer();
protected:

private:
};

#endif // QGIGIMAGEVIEWER_H
