#pragma once
#include <QGraphicsObject>
#include <QFile>
#include <QLayout>
#include <QPixmap>
#include <QRect>
#include <QPainter>

class RoundedRect : public QGraphicsObject
{
public:
	RoundedRect() : draw_rect(QRect()), corner_radius(0), color(QColor(0,0,0,0))
	{
	}

	void set_color(const QColor& _color)
	{
		color = _color;
	}

	void set_geometry(const QRectF& _draw_rect, size_t _radius = 0)
	{
		draw_rect = _draw_rect;
		corner_radius = _radius;
	}

	void set_image(const QPixmap& _pixmap)
	{
		image = _pixmap;
		update();
	}

	virtual void paint(QPainter *_painter, const QStyleOptionGraphicsItem *_option, QWidget *_widget = Q_NULLPTR) override
	{
		_painter->setBrush(color);
		_painter->setPen(Qt::NoPen);	// no border
		_painter->drawRoundRect(boundingRect(), corner_radius, corner_radius);

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
	size_t corner_radius;
	QColor color;
};