#pragma once
#include "Figure.h"
class MyText :
	public Figure
{
private: 
	CString text; 
public:
	MyText(COLORREF color, CString string, int x, int y); 
	MyText();
	MyText(const MyText &);
	~MyText(void);

	// Draw a line using graphic context pDC
	void draw(CDC* pDC);

	// Return true if Rectangle can be selected by clicking on coordinates (x,y)
	//bool selectFigureAt(int x, int y);

	// Return true if line is close to coordinates (x,y)
	bool isCloseTo(int x, int y);

	MyText * clone() const;


	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MyText);
};


