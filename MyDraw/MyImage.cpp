#include "stdafx.h"
#include "MyImage.h"

IMPLEMENT_SERIAL(MyImage, Figure, 1);

MyImage::MyImage()
{
}

MyImage::MyImage(CString path, int x, int y) : Figure(FigureType::Image, NULL)
{
	filepath = path; 

	CImage ci;
	ci.Load((CString)filepath);


	controlPoints.push_back(new ControlPoint(this, x, y));
	controlPoints.push_back(new ControlPoint(this, x + ci.GetWidth(), y + ci.GetHeight()));

}

MyImage::MyImage(const MyImage & other) : Figure(other)
{
	filepath = other.filepath;
}

MyImage::~MyImage()
{
}

bool MyImage::isCloseTo(int x, int y)
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

void MyImage::draw(CDC* pDC) {
	CImage ci;

	ci.Load((CString)filepath);

	CDC *dc = pDC;

	HDC hdc = *dc;

	ControlPoint * p0 = controlPoints.at(0);
	ControlPoint * p1 = controlPoints.at(1);

	// Find minx, miny, maxx, maxy
	int minX = (p0->getX()<p1->getX()) ? p0->getX() : p1->getX();
	int minY = (p0->getY()<p1->getY()) ? p0->getY() : p1->getY();
	int maxX = (p0->getX()<p1->getX()) ? p1->getX() : p0->getX();
	int maxY = (p0->getY()<p1->getY()) ? p1->getY() : p0->getY();

	
	ci.Draw(hdc, minX, minY, maxX - minX, maxY - minY);
}

MyImage * MyImage::clone() const {
	return new MyImage(*this);
}

void
MyImage::Serialize(CArchive & ar) {

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << figureColor;
		ar << figureType;

		ar << controlPoints.size();
		for each (ControlPoint * cp in controlPoints) {
			ar << cp->getX() << cp->getY();
		}
		ar << filepath.GetLength();
		for (int i = 0; i < filepath.GetLength(); i++) {
			ar << filepath[i];
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
		filepath = *string;

	}

}

