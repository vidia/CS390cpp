#include "stdafx.h"
#include "MyText.h"

IMPLEMENT_SERIAL(MyText, Figure, 1);

MyText::MyText(COLORREF color, CString string, int x, int y) : Figure(FigureType::Text, color)
{
	text = string; 
	controlPoints.push_back(new ControlPoint(this, x, y)); 
	controlPoints.push_back(new ControlPoint(this, x +10, y +10));
}


MyText::MyText()
{
}

MyText::MyText(const MyText & other) : Figure(other)
{
	text = other.text; 
}


MyText::~MyText()
{
}

bool MyText::isCloseTo(int x, int y)
{
	ControlPoint * p0 = controlPoints.at(0);
	ControlPoint * p1 = controlPoints.at(1);

	// Find minx, miny, maxx, maxy
	int minX = (p0->getX()<p1->getX()) ? p0->getX() : p1->getX();
	int minY = (p0->getY()<p1->getY()) ? p0->getY() : p1->getY();
	int maxX = (p0->getX()<p1->getX()) ? p1->getX() : p0->getX();
	int maxY = (p0->getY()<p1->getY()) ? p1->getY() : p0->getY();

	// Check if (x,y) is close to any of the borders.
	if (Figure::distancePointToLine(x, y, minX, minY, maxX, minY) < Figure::smallDistance ||
		Figure::distancePointToLine(x, y, maxX, minY, maxX, maxY) < Figure::smallDistance ||
		Figure::distancePointToLine(x, y, minX, maxY, maxX, maxY) < Figure::smallDistance ||
		Figure::distancePointToLine(x, y, minX, minY, minX, maxY) < Figure::smallDistance) {
		return true;
	}

	return false;
}

void MyText::draw(CDC* pDC) {
	CRect rect = new CRect( *new CPoint(controlPoints[0]->getX(), controlPoints[0]->getY()), *new CPoint(controlPoints[1]->getX(), controlPoints[1]->getY())); 
	pDC->SetTextColor(figureColor); 
	pDC->DrawText(text, rect, NULL);
}

MyText * MyText::clone() const {
	return new MyText(*this); 
}

void 
MyText::Serialize(CArchive & ar) {
	
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << figureColor;
		ar << figureType;

		ar << controlPoints.size();
		for each (ControlPoint * cp in controlPoints) {
			ar << cp->getX() << cp->getY();
		}
		ar << text.GetLength(); 
		for (int i = 0; i < text.GetLength(); i++) {
			ar << text[i]; 
		}
	}
	else
	{
		// TODO: add loading code here
		ar >> figureColor;

		int foo;
		ar >> foo;
		FigureType type = (FigureType)foo;

		int size;
		ar >> size;
		for (int i = 0; i < size; i++) {
			int x, y;
			ar >> x >> y;
			controlPoints.push_back(new ControlPoint(this, x, y));
		}
		
		ar >> size; 
		CString * string = new CString(); 
		for (int i = 0; i < size; i++) {
			wchar_t ch; 
			ar >> ch; 
			string->AppendChar(ch); 
		}
		text = *string; 

	}

}