#include "qgigimageviewer.h"
#include "gigbmpimage.h"

#include <QDebug>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>

class MyLabel : public QLabel
{
public:
	MyLabel(QWidget* widget=0)
		: QLabel(widget)
	{
		image = new GigBmpImage(this);
		image->Open("c:/TestBigImage.bmp");
	}
	virtual ~MyLabel()
	{}
	virtual QSize sizeHint(void) const
	{
		return image->Size();
	}
	void SetScrollArea(QScrollArea* s)
	{
		scrollArea = s;
	}
protected:
	virtual void paintEvent(QPaintEvent* e)
	{
		QPainter painter(this);
		QScrollBar* hb = scrollArea->horizontalScrollBar();
		QScrollBar* vb = scrollArea->verticalScrollBar();

		QPointF viewableTopLeft(hb->value(), vb->value());
		QSizeF viewableSize = scrollArea->size() -
							 QSizeF(vb->sizeHint().width(),
								   hb->sizeHint().height()) -
							 QSizeF(2*scrollArea->frameWidth(),
								   2*scrollArea->frameWidth());

		// QMatrix mat = scrollArea->matrix().inverted();
		QRectF viewableRect(viewableTopLeft, viewableSize);

		qDebug() << "drawing Rect : " << e->rect()
				 << ", viewable rect : " << viewableRect
				 << ", viewport: " << painter.viewport()
				 << ", window: " << painter.window();

		//QRectF rect = viewableRect;// e->rect());
		QRectF rect = e->rect();
		// rect.adjust(-1,-1,1,1);
		if(rect.x()<0) {
			rect.setX(0);
		}
		if(rect.y()<0) {
			rect.setY(0);
		}
		QRectF sourceRect = rect;
		QRectF targetRect = rect;
		image->Draw(&painter, rect, targetRect);

		__super::paintEvent(e);
	}
	QScrollArea* scrollArea;
	GigBmpImage* image;
	qreal zoomLevel;
};


QGigImageViewer::QGigImageViewer(QWidget *parent, Qt::WFlags flags)
	: QScrollArea(parent)
{
	MyLabel* label = new MyLabel;
	label->SetScrollArea(this);

	this->setBackgroundRole(QPalette::Dark);
	this->setWidget(label);
	this->setWindowFlags(flags);
}

QGigImageViewer::~QGigImageViewer()
{
}
