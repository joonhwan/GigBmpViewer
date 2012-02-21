#include "decoratedsimplelabel.h"

#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>

DecoratedSimpleLabel::DecoratedSimpleLabel(QWidget *parent)
	: QFrame(parent)
	, m_text("")
	, m_showAlways(false)
{
	m_borderPen = QPen(palette().text().color());
	m_fillBrush = QBrush(palette().window().color());
	m_opacity = 1.0;
	m_opaqueAnimation = new QPropertyAnimation(this, "opacity");
	m_opaqueAnimation->setDuration(2000);
	// m_opaqueAnimation->setStartValue(1.0);
	// m_opaqueAnimation->setEndValue(0.0);
	m_opaqueAnimation->setKeyValueAt(0, 1.0);
	m_opaqueAnimation->setKeyValueAt(0.5, 0.8);
	m_opaqueAnimation->setKeyValueAt(1, 0.);

	setText(tr(""));
}

DecoratedSimpleLabel::~DecoratedSimpleLabel()
{
	delete m_opaqueAnimation;
}

//virtual
QSize DecoratedSimpleLabel::sizeHint() const
{
	return m_textPath.boundingRect().size().toSize();
}

//virtual
QSize DecoratedSimpleLabel::minimumSizeHint() const
{
	return QSize(0,0);
}

QString DecoratedSimpleLabel::text(void) const
{
	return m_text;
}

QPen DecoratedSimpleLabel::borderPen(void) const
{
	return m_borderPen;
}

QBrush DecoratedSimpleLabel::fillBrush(void) const
{
	return m_fillBrush;
}

qreal DecoratedSimpleLabel::opacity(void) const
{
	return m_opacity;
}

void DecoratedSimpleLabel::showAlways(bool enabled)
{
	m_showAlways = enabled;
}

void DecoratedSimpleLabel::setText(const QString& text)
{
	static int ll = 0;

	bool changed = (m_text!=text);
	if(changed) {
		qDebug() << "setText[" << ll++ << "] : text = " << text;
		m_text = text;
		updatePath();
	}
	if(m_showAlways || changed) {
		qDebug() << "setText[" << ll++ << "] : restart animation...";
		m_opaqueAnimation->stop();
		m_opaqueAnimation->start();
	}
}

void DecoratedSimpleLabel::setBorderPen(const QPen& pen)
{
	m_borderPen = pen;
	update();
}

void DecoratedSimpleLabel::setFillBrush(const QBrush& brush)
{
	m_fillBrush = brush;
	update();
}

void DecoratedSimpleLabel::setOpacity(qreal opacity)
{
	m_opacity = opacity;
	update();
}

//virtual
void DecoratedSimpleLabel::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setOpacity(m_opacity);

	drawFrame(&painter);

	painter.setRenderHints(painter.renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(m_borderPen);
	painter.setBrush(m_fillBrush);
	painter.drawPath(m_textPath);
}

//virtual
void DecoratedSimpleLabel::mousePressEvent(QMouseEvent* e)
{
	__super::mousePressEvent(e);
}

void DecoratedSimpleLabel::updatePath(void)
{
	QPainterPath path;
	path.addText(0, 0, font(), m_text);

	QRectF box = path.boundingRect();
	path.translate(-box.topLeft());

	m_textPath = path;

	updateGeometry();
	update();
}


