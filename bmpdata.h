#pragma once

#include <QtGlobal>

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L
#if (_WIN32_WINNT >= _WIN32_WINNT_NT4)
#endif

#pragma pack(push,2)
struct BITMAPFILEHEADER
{
  quint16 bfType;
  quint32 bfSize;
  quint16 bfReserved1;
  quint16 bfReserved2;
  quint32 bfOffBits;
};

struct BITMAPINFOHEADER
{
    quint32 biSize;
    qint32 biWidth;
    qint32 biHeight;
    quint16 biPlanes;
    quint16 biBitCount;
    quint32 biCompression;
    quint32 biSizeImage;
    qint32 biXPelsPerMeter;
    qint32 biYPelsPerMeter;
    quint32 biClrUsed;
    quint32 biClrImportant;
};

#pragma pack(pop)
