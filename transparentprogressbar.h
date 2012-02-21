#pragma once

#include <QProgressBar>

class QPropertyAnimation;

class TransparentProgressBar : public QProgressBar
{
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
	TransparentProgressBar(QWidget *parent=0);
	~TransparentProgressBar();
	qreal opacity(void) const
	{
		return m_opacity;
	}
	void setOpacity(qreal opacity);
protected Q_SLOTS:
	void onValueChanged(int value);
protected:
	virtual void paintEvent(QPaintEvent* e);
private:
	qreal m_initialOpacity;
	qreal m_opacity;
	QPropertyAnimation* m_opaqueAnimation;
};
