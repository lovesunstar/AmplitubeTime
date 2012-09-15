#ifndef __DIRECTORY_STREAM_H
#define __DIRECTORY_STREAM_H
//
#include <QtGui>
#include "data_stream.h"
//
class DirectoryStream : public DataStream
{
Q_OBJECT

protected:

public:
	DirectoryStream(QString dir, QObject * parent=0);
	virtual ~DirectoryStream();
	
};
#endif
