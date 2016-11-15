#pragma once

#include <QLabel>

class PlayItem : public QLabel
{
public:
	void set_size(size_t _w, size_t _h)
	{
		width = _w;
		height = _h;
	}

	virtual ~PlayItem() {};

protected:
	size_t width;
	size_t height;
};