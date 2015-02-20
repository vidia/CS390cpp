#pragma once
#include "Figure.h"
class MyImage :
	public Figure
{
private:
	CString filepath; 
public:
	MyImage(CString path, int x, int y);
	MyImage();
	MyImage(const MyImage &);
	~MyImage(void);

	// Draw a line using graphic context pDC
	void draw(CDC* pDC);

	// Return true if Rectangle can be selected by clicking on coordinates (x,y)
	//bool selectFigureAt(int x, int y);

	// Return true if line is close to coordinates (x,y)
	bool isCloseTo(int x, int y);

	MyImage * clone() const;


	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MyImage);
};

