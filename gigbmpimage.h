#pragma once

#include "bmpdata.h"
#include "memoryfilemapper.h"

#include <QByteArray>
#include <QImage>
#include <QList>
#include <QObject>
#include <QRect>
#include <QRgb>
#include <QSize>
#include <QVector>

// gray scale image only
class GigBmpImage : public QObject
{
	Q_OBJECT
public:
	struct CachedItem
	{
		QRect region;
		QByteArray buffer;
	};
	struct DrawItem
	{
		QRect sourceRegion;
		QRect targetRegion;
		QImage image;
	};
	GigBmpImage(QObject *parent=0);
	~GigBmpImage();

	bool Open(const QString& imageFilePath);
	void Close(void);
	bool Draw(QPainter* painter, const QRectF& _sourceRegion, const QRectF& targetRegion);
	QSize Size(void) const;
private:
	BITMAPINFOHEADER m_fileBitmapInfo;
	QVector<QRgb> m_colorTable;
	MemoryFileMapper::FileSizeType m_fileOffestToImageBuffer;
	MemoryFileMapper m_fileMapper;
	QList<CachedItem> m_cachedData;
	CachedItem EnsureMap(const QRect& sourceRegion);
};
