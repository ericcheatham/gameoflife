#include <QtGui>
#include <iostream>
#include "gamegui.h"

GameGui::GameGui(QWidget *parent ) 
	:QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	
	curColor = Qt::black;
	zoom = 8;
	
	image = QImage(16, 16, QImage::Format_ARGB32);
	image.fill(qRgba(0, 0, 0, 0));	
}
	
QSize GameGui::sizeHint() const
{
	QSize size = zoom * image.size();
	if(zoom >= 3)
		size += QSize(1, 1);
	return size;
}	

void GameGui::setPenColor(const QColor &newColor)
{
	curColor = newColor;
}

void GameGui::setIconImage(const QImage &newImage)
{
	if(newImage != image)
	{
		image = newImage.convertToFormat(QImage::Format_ARGB32);
		update();
		updateGeometry();
	}
}

void GameGui::setZoomFactor(int newZoom)
{
	if(newZoom < 1) newZoom = 0;
	if(newZoom != zoom)
	{
		zoom = newZoom;
		update();
		updateGeometry();
	}
}

void GameGui::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	
	if(zoom >=3)
	{
		painter.setPen(palette().foreground().color());
		for( int i= 0; i <= image.width(); i++)
			painter.drawLine(zoom * i, 0, 
					 zoom * i, zoom * image.height());	
		for( int j= 0; j <= image.height(); j++)
			painter.drawLine(0, zoom * j,
					 zoom * image.width(), zoom *j);		
	}
		
	for( int i = 0; i <= image.width(); i++)
		for( int j= 0; j <= image.height(); i++)
		{
			QRect rect = pixelRect(i , j);
			if( !event -> region().intersect(rect).isEmpty())
			{
				QColor color = QColor::fromRgba(image.pixel(i, j));
				painter.fillRect(rect, color);
			}			
		}	
			
}
		
		
QRect GameGui::pixelRect(int i , int j) const
{
	if(zoom >= 3)
		return QRect(zoom * i + 1, zoom * j + 1, zoom -1, zoom -1);
	else
		return QRect(zoom * i, zoom * j, zoom, zoom); //Mazda commerical

}		
	
		
void GameGui::mousePressEvent(QMouseEvent * event)
{
	if( event -> buttons() & Qt::LeftButton) 
		setImagePixel(event -> pos(), true);
	else if( event -> buttons() & Qt::RightButton) 
		setImagePixel(event -> pos(), false);
}		
		
void GameGui::mouseMoveEvent(QMouseEvent * event)
{
	/*if( event -> buttons() & Qt::LeftButton) 
		setImagePixel(event -> pos(), true);
	else if( event -> buttons() & Qt::RightButton) 
		setImagePixel(event -> pos(), false);	*/
}

void GameGui::setImagePixel(const QPoint &pos, bool opaque)
{
	int x = pos.x()/zoom;
	int y = pos.y()/zoom;
	
	if(image.rect().contains(x, y))
	{
		if(opaque)
			image.setPixel(x, y, penColor().rgba());
		//else
		//	image.setPixel(x, y, qRgba(0,0,0,0));
		
		
		update(pixelRect(x, y));
	}
}		
		
		
		
		

