#include "StdAfx.h"
#include "MyOval.h"

MyOval::MyOval(COLORREF color, int x0, int y0, int x1, int y1)
	:Figure(Figure::FigureType::Rectangle, color)
{
	controlPoints.push_back(new ControlPoint(this, x0, y0));
	controlPoints.push_back(new ControlPoint(this, x1, y1));
}

MyOval::~MyOval(void)
{
}

// Draw a MyOval using graphic context pDC
void MyOval::draw(CDC* pDC)
{
	ControlPoint * p0 = controlPoints.at(0);
	ControlPoint * p1 = controlPoints.at(1);

	// Find minx, miny, maxx, maxy
	int minX = (p0->getX()<p1->getX()) ? p0->getX() : p1->getX();
	int minY = (p0->getY()<p1->getY()) ? p0->getY() : p1->getY();
	int maxX = (p0->getX()<p1->getX()) ? p1->getX() : p0->getX();
	int maxY = (p0->getY()<p1->getY()) ? p1->getY() : p0->getY();

	int rx = (maxX - minX) / 2; 
	int ry = (maxY - minY) / 2; 
	int cx = (maxX + minX) / 2; 
	int cy = (maxY + minY) / 2; 

	CPen pen(PS_SOLID, 0, this->figureColor);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo((maxX-minX)/2 + minX, minY);

	for (float i = 0; i <= 2*3.14159; i += .01) {
		int x = cx + cos(i) * rx; 
		int y = cy + sin(i) * ry; 
		
		if( i == 0 ) pDC->MoveTo(x, y);
		else 
			pDC->LineTo(x, y); 
	}
}

// Return true if MyOval is close to coordinates (x,y)
//Defaults to rectangle prox. Change to calc angle then 
//check distance
bool MyOval::isCloseTo(int x, int y)
{
	ControlPoint * p0 = controlPoints.at(0);
	ControlPoint * p1 = controlPoints.at(1);

	// Find minx, miny, maxx, maxy
	int minX = (p0->getX()<p1->getX()) ? p0->getX() : p1->getX();
	int minY = (p0->getY()<p1->getY()) ? p0->getY() : p1->getY();
	int maxX = (p0->getX()<p1->getX()) ? p1->getX() : p0->getX();
	int maxY = (p0->getY()<p1->getY()) ? p1->getY() : p0->getY();


	int rx = (maxX - minX) / 2;
	int ry = (maxY - minY) / 2;
	int cx = (maxX + minX) / 2;
	int cy = (maxY + minY) / 2;

	float angle = atan2(y - cy, x - cx);

	int _x = cx + cos(angle) * rx;
	int _y = cy + sin(angle) * ry;

	double dist = sqrt((_x - x)*(_x - x) + (_y - y)*(_y - y));
	dist = (dist>0) ? dist : -dist;

	if (dist < Figure::smallDistance) {
		return true; 
	}

	return false;
}

MyOval * MyOval::clone() const {
	return new MyOval(*this); 
}
