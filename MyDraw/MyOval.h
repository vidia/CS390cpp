#pragma once

#include "Figure.h"

// Subclass for a Oval. 
// Inherits most methods from Figure. 
class MyOval : public Figure
{
public:
	// Constructor/destructor for a Rectangle
	MyOval(int x0, int y0, int x1, int y1);
	~MyOval(void);

	// Draw a line using graphic context pDC
	void draw(CDC* pDC);

	// Return true if Rectangle can be selected by clicking on coordinates (x,y)
	//bool selectFigureAt(int x, int y);

	// Return true if line is close to coordinates (x,y)
	bool isCloseTo(int x, int y);
};

