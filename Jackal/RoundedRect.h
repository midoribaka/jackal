#pragma once
#include <QGraphicsItem>
#include <QFile>
#include <QLayout>
#include <QPixmap>
#include <QRect>
#include <QPainter>

class RoundedRect : public QGraphicsItem
{
public:
	RoundedRect(QRectF _draw_rect, size_t _radius = 0, QGraphicsItem* _parent = 0) : QGraphicsItem(_parent), draw_rect(_draw_rect)
	{
	}

	void set_image(const QPixmap& _pixmap)
	{
		image = _pixmap;
		update();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(QColor(0, 0, 0, 64));					//transparent background
		_painter->drawRoundRect(boundingRect(), radius, radius);

		if (!image.isNull()) 
		{
			_painter->drawPixmap(boundingRect(), image, image.rect());	//autoscale //todo довольно затратно
		}
	}

	virtual QRectF boundingRect() const override
	{
		//origin at 0,0 top left
		return draw_rect;
	}

	virtual ~RoundedRect()
	{
	}

protected:
	QPixmap image;
	QRectF draw_rect;
	size_t radius;
};