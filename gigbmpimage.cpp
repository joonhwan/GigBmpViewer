#include "gigbmpimage.h"

#include <QDebug>
#include <QDataStream>
#include <QFileInfo>
#include <QIODevice>
#include <QPainter>

GigBmpImage::GigBmpImage(QObject *parent)
	: QObject(parent)
{
	// init gray scale color index table
	for(int i=0; i<=0xFF; ++i) {
		m_colorTable << qRgb(i,i,i);
	}
}

GigBmpImage::~GigBmpImage()
{

}

bool GigBmpImage::Open(const QString& imageFilePath)
{
	Close();

	bool done = false;

	do
	{
		// check file
		QFileInfo fileInfo(imageFilePath);
		if(!fileInfo.exists()) {
			break;
		}

		// probing file format
		QFile file(imageFilePath);
		if(!file.open(QIODevice::ReadOnly)) {
			break;
		}
		quint64 readLength = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		QByteArray tempBuffer(readLength, 0);
		QDataStream stream(&file);
		char* p = tempBuffer.data();
		if(readLength!=stream.readRawData(p, readLength)) {
			break;
		}
		BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)p;
		BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)(p + sizeof(BITMAPFILEHEADER));
		const char* tag = (const char*)&bfh->bfType;
		if(tag[0] != 'B' ||
		   tag[1] != 'M')
		{
			// ooops not a bmp file format!
			break;
		}
		if(bih->biCompression!=BI_RGB)
		{
			// we cannot handle any other format than non-compressed rgb format
			break;
		}

		// keep file bitmap information
		::memcpy(&m_fileBitmapInfo, bih, sizeof(m_fileBitmapInfo));
		m_fileOffestToImageBuffer = bfh->bfOffBits;

		if(!m_fileMapper.Open(imageFilePath)) {
			break;
		}

		done = true;
	} while(0);

	if(!done) {
		Close();
	}

	return done;
}

void GigBmpImage::Close(void)
{
	m_fileMapper.Close();
}

bool GigBmpImage::Draw(QPainter* painter, const QRectF& _sourceRegion, const QRectF& targetRegion)
{
	bool done = false;

	do
	{
		QRectF sourceRegion = _sourceRegion.normalized();

		// if not cached
		CachedItem cached = EnsureMap(sourceRegion.toRect());
		if(cached.region.isNull()) {
			break;
		}

		// adjust source rect in relative coordinate
		sourceRegion.translate(-1*cached.region.topLeft());

		bool flip = m_fileBitmapInfo.biHeight > 0;

		// if(flip)
		// {
		// 	// 'top' is actually bottom position
		// 	int sourceRegionHeight = cached.region.height();
		// 	sourceRegion.setTop(cached.region.height() - sourceRegion.bottom());
		// 	sourceRegion.setHeight(sourceRegionHeight);
		// }

		qint32 width = m_fileBitmapInfo.biWidth;
		qint32 height = m_fileBitmapInfo.biHeight;
		m_fileBitmapInfo.biBitCount = 8;
		qint32 bpp = m_fileBitmapInfo.biBitCount;
		qint32 stride = (width * (bpp / 8) + (sizeof(quint32)-1)) & ~(sizeof(quint32)-1);
		QSize cachedSize = QSize(cached.region.width(),
								 cached.region.height());

		QImage cachedImage((const uchar*)cached.buffer.constData(),
						   cached.region.width(),
						   cached.region.height(),
						   stride,
						   QImage::Format_Indexed8);
		if(flip) {
			QImage flippedImage = cachedImage.mirrored();
			flippedImage.setColorTable(m_colorTable);
			painter->drawImage(targetRegion,
							   flippedImage,
							   sourceRegion);
		} else {
			cachedImage.setColorTable(m_colorTable);
			painter->drawImage(targetRegion,
							   cachedImage,
							   sourceRegion);
		}
		done = true;
	} while(0);

	if(!done)
	{
		// draw a special image for letting user know there is no
		// image buffer available.
	}

	return done;
}

QSize GigBmpImage::Size(void) const
{
	QSize size(m_fileBitmapInfo.biWidth,
			   m_fileBitmapInfo.biHeight);
	return size;
}

GigBmpImage::CachedItem GigBmpImage::EnsureMap(const QRect& requestedRegion)
{
	bool done = false;
	CachedItem mapped;
	do
	{
		// cache to accomdate request region
		qint32 width = m_fileBitmapInfo.biWidth;
		qint32 rightMostPos = width - 1;
		qint32 height = m_fileBitmapInfo.biHeight;
		qint32 bottomMostPos = height - 1;

		bool flip = height > 0;
		if(!flip)
		{
			height = -1 * height;
		}
		Q_ASSERT(height >= 0);

		// clipping TODO CHECKME
		mapped.region.setLeft(0);
		mapped.region.setTop( qMax(requestedRegion.top()/1024 - 1, 0) * 1024 );
		mapped.region.setBottom( qMin( (requestedRegion.bottom()/1024 + 1)*1024 - 1, bottomMostPos) );
		mapped.region.setRight(rightMostPos);

		// TODO
		//
		// update cache for 'mapped.region'.
		//
		// note : it might not be possible to create ALL cached data
		//        for it.  In this case, client should call
		//        'EnsureMap' again for the uncached region.
		//

		if(!m_cachedData.empty()) {
			const CachedItem& cached = m_cachedData.first();
			if(cached.region.contains(requestedRegion)) {
				qDebug() << "cache hits!";
				mapped = cached;
				done = true;
				break;
			}
		}

		qint32 bpp = 8;// (qint32)m_fileBitmapInfo.biBitCount;
		qint32 stride = (width * (bpp / 8) + (sizeof(quint32)-1)) & ~(sizeof(quint32)-1);
		MemoryFileMapper::FileSizeType offset = m_fileOffestToImageBuffer;
		if(flip)
		{
			int bottom = mapped.region.bottom();
			int endPosition = offset + height * stride;
			offset =  (endPosition - stride) - bottom * stride;
		}
		else
		{
			offset = offset + stride  * mapped.region.top();
		}

		int mappedImageHeight = mapped.region.height();
		quint32 len = (quint32)(stride  * mappedImageHeight);
		if(mapped.buffer.size() < len) {
			mapped.buffer.resize(len);
		}
		Q_ASSERT(mapped.buffer.size() >= len);

		// regen bitmap
		if(!m_fileMapper.CopyContent(offset, len, mapped.buffer))
		{
			mapped.buffer.clear();
			break;
		}

		m_cachedData.clear();
		m_cachedData.push_back(mapped);

		qDebug() << "newly cached!";

	} while(0);

	return mapped;
}
