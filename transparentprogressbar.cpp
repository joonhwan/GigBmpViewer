#include "transparentprogressbar.h"
#include <QStylePainter>
#include <QStyleOptionProgressBarV2>
#include <QPlastiqueStyle>
#include <QPropertyAnimation>
#include <QDebug>

TransparentProgressBar::TransparentProgressBar(QWidget *parent)
	: QProgressBar(parent)
	, m_initialOpacity(0.6)
{
	setAutoFillBackground(false);
	setStyle(new QPlastiqueStyle()); // to show text inside of progressbar
	setOpacity(m_initialOpacity);
	m_opaqueAnimation = new QPropertyAnimation(this, "opacity");
	m_opaqueAnimation->setStartValue(1.0);
	m_opaqueAnimation->setEndValue(0.0);

	connect(this, SIGNAL(valueChanged(int)),
			this, SLOT(onValueChanged(int)));
}

TransparentProgressBar::~TransparentProgressBar()
{
}

void TransparentProgressBar::setOpacity(qreal opacity)
{
	opacity = qMin(opacity, 1.);
	opacity = qMax(0., opacity);
	if(m_opacity!=opacity) {
		m_opacity = opacity;
		update();
	}
}

void TransparentProgressBar::onValueChanged(int value)
{
	// qDebug() << "onValuChanged:" << value << "/" << maximum();
	if(value==0) {
		setOpacity(m_initialOpacity);
		m_opaqueAnimation->stop();
	} else if(value==maximum()) {
		m_opaqueAnimation->start();
	} else {
		m_opaqueAnimation->stop();
	}
}

//virtual
void TransparentProgressBar::paintEvent(QPaintEvent* e)
{
	QStylePainter paint(this);
	paint.setOpacity(m_opacity);
    QStyleOptionProgressBarV2 opt;
    initStyleOption(&opt);
    paint.drawControl(QStyle::CE_ProgressBar, opt);

	// QSize oldSize = size();
	// resize(QSize(0,0));
	// __super::paintEvent(e);
	// resize(oldSize);
}



