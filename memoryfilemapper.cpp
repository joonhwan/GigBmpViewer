#include "memoryfilemapper.h"
#include <QDataStream>
#include <QFile>

namespace {

//MUST be a multiple of the OS allocation granularity
//(e.g. this will normally be 64Kb)
const qint32 MEM_BLOCK_SIZE = 0x10000*2;	//actually, this is 128Kb

}

MemoryFileMapper::MemoryFileMapper(QObject *parent)
	: QObject(parent)
{
	m_file = new QFile(this);
}

MemoryFileMapper::~MemoryFileMapper()
{
	Close();
}

bool MemoryFileMapper::Open(const QString& filePath)
{
	bool done = false;

	do
	{
		m_file->close();
		m_file->setFileName(filePath);
		if(!m_file->open(QIODevice::ReadOnly)) {
			break;
		}

		done = true;
	} while(0);

	return done;
}

bool MemoryFileMapper::Close(void)
{
	bool done = false;
	do
	{
		if(m_file) {
			m_file->close();
		}

		done = true;
	} while(0);

	return done;
}

//
//	Return the logical size of the MemoryFileMapper.
//
MemoryFileMapper::FileSizeType MemoryFileMapper::Size()
{
	FileSizeType s = 0;
	if(m_file) {
		s = m_file->size();
	}
	return s;
}

bool MemoryFileMapper::CopyContent(MemoryFileMapper::FileSizeType offset,
								   MemoryFileMapper::FileSizeType length,
								   QByteArray& to)
{
	// If we had the entire file mapped into memory at once, then
	// we could do a simple memcpy to render the data in one go.
	//
	// However, we have to do the copies in sections. When
	// we get a pointer to the file with AdjustedAddress, this
	// pointer only covers a range -MEM_BLOCK_SIZE/4 to +MEM_BLOCK_SIZE/4,
	// so we have to break the copies up into smaller units.
	//
	int oldBufferSize = to.size();

	QDataStream stream(&to, QIODevice::ReadWrite);

	int copiedLength = 0;
	while(copiedLength < length)
	{
		int len = qMin((length-copiedLength), FileSizeType(MEM_BLOCK_SIZE / 4));

		uchar* from = m_file->map(offset, len);
		if(!from) {
			break;
		}

		int r = stream.writeRawData((const char*)from, len);
		m_file->unmap(from);

		if(-1==r) {
			break;
		}
		Q_ASSERT(r==len);
		if(r!=len) {
			break;
		}

		copiedLength += len;
		offset += len;	//advance the adjustedAddress position
	}

	// target buffer won't be resized
	Q_ASSERT(oldBufferSize ==  to.size());

	return (copiedLength==length);
}
