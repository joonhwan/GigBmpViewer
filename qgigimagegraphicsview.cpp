#include "qgigimagegraphicsview.h"
#include "qthreadedgigimagegraphicsscene.h"
#include "gigrenderthread.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QLabel>
#include <QProgressBar>
#include <QPropertyAnimation>

QGigImageGraphicsView::QGigImageGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
	, m_panning(0)
{
	setBackgroundBrush(palette().dark());
	setRenderHint(QPainter::Antialiasing, true);
	setCacheMode(QGraphicsView::CacheBackground);
	setDragMode(QGraphicsView::NoDrag);
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setTransformationAnchor(AnchorUnderMouse);

	pixelInfoLabel = new QLabel(tr(""));
	renderStatus = new QLabel(tr(""));
	renderProgress = new QProgressBar;
	renderProgress->setWindowOpacity(0.0);

	QVBoxLayout* leftLayout = new QVBoxLayout;
	leftLayout->addWidget(pixelInfoLabel);

	QVBoxLayout* rightLayout = new QVBoxLayout;
	rightLayout->addStretch(1);
	leftLayout->addWidget(renderProgress);
	rightLayout->addWidget(renderStatus);

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addLayout(leftLayout);
	layout->addStretch(1);
	layout->addLayout(rightLayout);
	setLayout(layout);
}

QGigImageGraphicsView::~QGigImageGraphicsView()
{

}

void QGigImageGraphicsView::init(QThreadedGigImageGraphicsScene* scene)
{
	this->setScene(scene);
	GigRenderThread* thread = scene->thread();

	connect(thread, SIGNAL(renderStatusChanged(const QString&)),
			renderStatus, SLOT(setText(const QString&)));
	connect(thread, SIGNAL(renderProgress(int,int)),
			SLOT(updateProgress(int,int)));
}

void QGigImageGraphicsView::ZoomIn()
{
    scaleView(qreal(2.));
}

void QGigImageGraphicsView::ZoomOut(void)
{
    scaleView(qreal(1./2.));
}

void QGigImageGraphicsView::ResetZoom(void)
{
	fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void QGigImageGraphicsView::zoom(qreal factor, QPointF centerPoint)
{
    scale(factor, factor);
    centerOn(centerPoint);

	qreal myZoomLevel = zoomLevel();
	scene()->clearSelection(); // caveat in rubberband zooming.
	// without it, one clicking will zoom scene. :(
}

qreal QGigImageGraphicsView::zoomLevel(void) const
{
	QRect visibleViewRect = rect();
	QRectF visibleSceneRect = mapToScene(visibleViewRect).boundingRect();
	qreal factor = visibleViewRect.width() / visibleSceneRect.width();
	return factor;
}

void QGigImageGraphicsView::updateProgress(int doneCount, int totalCount)
{
	if(doneCount==0) {
		renderProgress->setWindowOpacity(1.);
	}

	renderProgress->setMaximum(totalCount);
	renderProgress->setValue(doneCount);

	if(doneCount==totalCount) {
		QPropertyAnimation* ani = new QPropertyAnimation(renderProgress, "windowOpacity");
		ani->setDuration(1000);
		ani->setStartValue(1.0);
		ani->setEndValue(0.);
		ani->start();
	}
}

void QGigImageGraphicsView::wheelEvent ( QWheelEvent * e)
{
	//if(!m_panning)
#define SUPPORT_WHEEL_ZOOM
#ifdef SUPPORT_WHEEL_ZOOM
	if(e->modifiers().testFlag(Qt::ControlModifier))
	{
#ifdef AUTOCAD_STYLE
		// zoom only when CTRL key pressed
		int numSteps = e->delta() / 15 / 8;
		if (numSteps == 0) {
			e->ignore();
			return;
		}
		qreal sc = pow(1.25, numSteps); // I use scale factor 1.25
		this->zoom(sc, mapToScene(e->pos()));
		e->accept();
#else
		e->delta() > 0 ? ZoomIn() : ZoomOut();
#endif
	}
#endif
}

//virtual
void QGigImageGraphicsView::mousePressEvent(QMouseEvent* event)
{
	__super::mousePressEvent(event);
}

//virtual
void QGigImageGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	// from qgraphicsview.cpp /  QGraphicsView::mouseMoveEvent()
	bool wasRubberBanding = (dragMode() == QGraphicsView::RubberBandDrag &&
							 isInteractive());

	__super::mouseReleaseEvent(event);

	if(wasRubberBanding)
	{
		QRectF bandRect = scene()->selectionArea().boundingRect();
		QRectF viewBandRect = mapFromScene(bandRect).boundingRect();

		const qreal bandZoomThreshold = 10.;
		if(viewBandRect.width() > bandZoomThreshold ||
		   viewBandRect.height() > bandZoomThreshold )
		{
			fitInView(bandRect, Qt::KeepAspectRatio);
		}
	}
}

//virtual
void QGigImageGraphicsView::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
	case Qt::Key_Space:
		if(!event->isAutoRepeat())
		{
			m_panning = 1;
			this->setDragMode(QGraphicsView::ScrollHandDrag);
		}
		break;
	case Qt::Key_Plus:
	case Qt::Key_Equal:
		ZoomIn();
		break;
	case Qt::Key_Minus:
		ZoomOut();
		break;
	case Qt::Key_Backslash:
	case Qt::Key_Escape:
		ResetZoom();
		break;
	// case Qt::Key_Up:
	// 	scrollContentsBy(0, 10);
	// 	break;
	// case Qt::Key_Down:
	// 	scrollContentsBy(0, -10);
	// 	break;
	// case Qt::Key_Left:
	// 	scrollContentsBy(-10, 0);
	// 	break;
	// case Qt::Key_Right:
	// 	scrollContentsBy(+10, 0);
	// 	break;
	case Qt::Key_BraceLeft:
		rotate(90.);
		break;
	case Qt::Key_BraceRight:
		rotate(-90.);
		break;
	}
	__super::keyPressEvent(event);
}

//virtual
void QGigImageGraphicsView::keyReleaseEvent(QKeyEvent * event)
{
	if(event->key()==Qt::Key_Space && !event->isAutoRepeat())
	{
		this->setDragMode(QGraphicsView::NoDrag);
		m_panning = 0;
	}
	__super::keyPressEvent(event);
}

void QGigImageGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).
                                        mapRect(QRectF(0, 0, 1, 1)).width();

    // // don't allow to scale up/down too much
    // if (factor < 0.02)
    //     return;

    scale(scaleFactor, scaleFactor);
}
