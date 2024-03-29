#pragma once

#include "Figure.h"
class Group : public Figure
{
private: 
	vector<Figure *> figures; 
	vector<ControlPoint *> figControlPoints;

public:
	// Constructor/destructor for a line
	Group(vector<Figure *> figures);
	Group(); 
	~Group(void);

	// Draw a line using graphic context pDC
	void draw(CDC* pDC);

	vector<Figure *> & getFigures(); 

	// Return true if line is close to coordinates (x,y)
	bool isCloseTo(int x, int y);

	Group * clone() const;

	void dragSelectedControlPoints(int, int); 
	void setColor(COLORREF color); 

	void Serialize(CArchive& archive);

	DECLARE_SERIAL(Group);
};

