#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>

class Renderer : public QObject
{
	Q_OBJECT
public:
	Renderer(QObject *parent);
	~Renderer();

	bool openImage(const QString& filePath);
	void closeImage(void);
	void render(TiledImageGraphicsItem* item, int priority=0);
	QImage cachedRawImageOf(TiledImageGraphicsItem* item);
	void cacheRawImage(TiledImageGraphicsItem* item);
signals:
	void renderedImage(TiledImageGraphicsItem* item, QImage scaleDown);
	void cachedImage(TiledImageGraphicsItem* item);

private:
	GigBmpImage* m_image;
};

#endif // RENDERER_H
