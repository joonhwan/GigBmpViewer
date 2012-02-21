#ifndef DECORATEDSIMPLELABEL_H
#define DECORATEDSIMPLELABEL_H

#include <QFrame>
#include <QPen>
#include <QBrush>
#include <QPainterPath>

class QPropertyAnimation;

class DecoratedSimpleLabel : public QFrame
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen)
	Q_PROPERTY(QBrush fillBrush READ fillBrush WRITE setFillBrush)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
	DecoratedSimpleLabel(QWidget *parent=0);
	virtual ~DecoratedSimpleLabel();
	virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

	QString text(void) const;
	QPen borderPen(void) const;
	QBrush fillBrush(void) const;
	qreal opacity(void) const;
	void showAlways(bool enabled);
public Q_SLOTS:
	void setText(const QString& text);
	void setBorderPen(const QPen& pen);
	void setFillBrush(const QBrush& brush);
	void setOpacity(qreal opacity);
protected:
	virtual void paintEvent(QPaintEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	void updatePath(void);

	QString m_text;
	QPen m_borderPen;
	QBrush m_fillBrush;
	qreal m_opacity;
	QPainterPath m_textPath;
	bool m_showAlways;
	QPropertyAnimation* m_opaqueAnimation;
};

#endif // DECORATEDSIMPLELABEL_H
