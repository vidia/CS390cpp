/*
  Copyright (c) 2011
  Gustavo Rodriguez-Rivera
  All rights reserved.

  This work was developed by the author(s) at Purdue University
  during 2011.

  Redistribution and use in source and binary forms are permitted provided that
  this entire copyright notice is duplicated in all such copies.  No charge,
  other than an "at-cost" distribution fee, may be charged for copies,
  derivations, or distributions of this material without the express written
  consent of the copyright holders. Neither the name of the University, nor the
  name of the author may be used to endorse or promote products derived from
  this material without specific prior written permission.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR ANY PARTICULAR PURPOSE.
  */

#include "StdAfx.h"
#include "Drawing.h"
#include "Line.h"
#include "Figure.h"
#include "Rectangle.h"
#include "MyOval.h"
#include "Group.h"
#include "TextDialog.h"
#include "MyText.h"
#include "MyImage.h"

// Constructor/Destructor
Drawing::Drawing(void)
{
	// Initial mode is selectMode
	this->editMode = SelectMode;

	// Initial state for mouse button is not pressed.
	this->previousMouseMode = Drawing::NoButtonPressed;
}

Drawing::~Drawing(void)
{
}

// Draw the figures in the Drawing.
// Called by the draw method in MyDrawView
void
Drawing::draw(CDC* pDC)
{
	// For each figure in vector "figures" draw the figure with control points.
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->drawWithControlPoints(pDC);
	}

	// Draw the selection rectangle if enabled.
	this->drawSelectionRectangle(pDC);
}

// Set the edit mode for the editor.
void
Drawing::setEditMode(EditMode mode)
{
	this->editMode = mode;
}

void
Drawing::deleteSelected(CView * cview) {

	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			Figure * f = figures[i];
			figures.erase(figures.begin() + i);
			delete f;
		}
		else
			i++; 
	}

	// Redraw window. This will call the draw method.
	cview->RedrawWindow();
	pushUndoStack();
}

void
Drawing::copySelected(CView * cview) {
	copyBuffer.clear(); 

	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			copyBuffer.push_back(figures[i]->clone());
		}
		i++;
	}
}

void 
Drawing::setCurrentColor(CView * cview, COLORREF color) {
	currentColor = color; 
	
	bool changed = false; 
	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			figures[i]->setColor(color);
			changed = true; 
		}
		i++;
	}

	if (changed) {
		cview->RedrawWindow();
		pushUndoStack(); 
	}
}

void
Drawing::cutSelected(CView * cview) {
	copySelected(cview);
	deleteSelected(cview);
}

void
Drawing::pasteCopyBuffer(CView * cview) {

	selectAll(false); 

	bool changed = false; 
	for each (Figure * figure in copyBuffer) {
		changed = true; 
		Figure * copy = figure->clone(); 

		copy->select(true); 
		copy->dragSelectedControlPoints(10, 10);

		figures.push_back(copy); 
	}

	if (changed) {
		pushUndoStack();
		cview->RedrawWindow();
	}
}

void
Drawing::groupSelected(CView * cview) {
	vector<Figure *> togroup; 

	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			togroup.push_back(figures[i]->clone());
		}
		i++;
	}

	figures.push_back(new Group(togroup)); 

	deleteSelected(cview);
}

void
Drawing::ungroupSelected(CView * cview) {
	vector<Figure *> ungrouped; 

	bool changed = false;
	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			if (figures[i]->figureType == Figure::FigureType::Group)
			{
				changed = true; 
				Group * group = dynamic_cast<Group*>(figures[i]); 
				for each (Figure * figure in group->getFigures())
				{
					ungrouped.push_back(figure->clone()); 
				}

				Figure * f = figures[i];
				figures.erase(figures.begin() + i);
				delete f;
				i--; 
			}
		}
		i++;
	}

	for each (Figure * figure in ungrouped) {
		figures.push_back(figure); 
	}

	if (changed) {
		pushUndoStack();
		cview->RedrawWindow();
	}
}

void 
Drawing::bringSelectionToFront(CView * cview) {
	vector<Figure *> tomove;

	bool changed = false; 
	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			changed = true; 
			tomove.push_back(figures[i]);

			figures.erase(figures.begin() + i);
			i--;
		}
		i++;
	}

	figures.insert(figures.end(), tomove.begin(), tomove.end());

	if (changed) {
		pushUndoStack();
		cview->RedrawWindow(); 
	}
}

void
Drawing::sendSelectionToBack(CView * cview) {
	vector<Figure *> tomove;

	bool changed = false;
	std::vector<int>::size_type i = 0;
	while (i < figures.size()) {
		if (figures[i]->isSelected()) {
			changed = true; 
			tomove.push_back(figures[i]);

			figures.erase(figures.begin() + i);
			i--;
		}
		i++;
	}

	figures.insert(figures.begin(), tomove.begin(), tomove.end()); 

	if (changed) {
		cview->RedrawWindow();
		pushUndoStack();
	}
}

void
Drawing::undo(CView * cview) {
	if (undoStack.size()) {
		figures = undoStack[undoStack.size() - 1];
		undoStack.pop_back();
		//TODO: Delete figs in undo stack. 
		cview->RedrawWindow();
	}
}

void
Drawing::pushUndoStack() {
	vector<Figure*> curState; 
	for each (Figure * figure in figures) {
		Figure * clone = figure->clone();
		curState.push_back(clone); 
	}
	undoStack.push_back(curState); 
}

// Call back when the mouse is pressed, moved, or released.
// It is called by the mouse call backs in MyDrawView
void 
Drawing::OnMouse(CView * cview, int nFlags, CPoint point) {

	// Check if the mouse butto is pressed now
	bool mousePressedNow = ((nFlags & MK_LBUTTON)!=0);

	if ( mousePressedNow ) {
		// Mouse left button is currently pressed

		if (this->previousMouseMode == Drawing::NoButtonPressed) {
			// Button has just been pressed

			// Update previous mouse mode
			this->previousMouseMode = Drawing::ButtonPressed;

			// Check edit mode

			if (this->editMode == Drawing::NewLineMode) {
				// Edit mode is NewLineMode. 
				// This is because the user just selected the Figure->Line menu

				// Create a new line.
				Line * line = new Line(this->currentColor, point.x, point.y, point.x, point.y);

				// Add to the list of figures
				this->figures.push_back(line);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				line->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will clal the draw method.
				cview->RedrawWindow();

				pushUndoStack();
			}
			else if (this->editMode == Drawing::NewRectangleMode) {

				// Edit mode is NewRectangleMode. 
				// This is because the user just selected the Figure->Rectangle menu

				// Create a new rectangle.
				MyRectangle * rect = new MyRectangle(this->currentColor, point.x, point.y, point.x, point.y);

				// Add to the list of figures
				this->figures.push_back(rect);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				rect->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will call the draw method.
				cview->RedrawWindow();

				pushUndoStack();
			}

			else if (this->editMode == Drawing::NewTextMode) {

				// Edit mode is NewRectangleMode. 
				// This is because the user just selected the Figure->Rectangle menu

				// Create a new rectangle.
				
				TextDialog dialog = new TextDialog(cview);
				dialog.DoModal(); 

				OutputDebugString(dialog.ItemText); 
				
				MyText * text = new MyText(this->currentColor, dialog.ItemText, point.x, point.y);

				// Add to the list of figures
				this->figures.push_back(text);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				//text->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will call the draw method.
				cview->RedrawWindow();

				pushUndoStack();
			}
			else if (this->editMode == Drawing::NewImageMode) {

				// Edit mode is NewRectangleMode. 
				// This is because the user just selected the Figure->Rectangle menu

				// Create a new rectangle.

				CString FileFormatSelection;
				CFileDialog FileDialog(TRUE); 
				FileDialog.DoModal();

				OutputDebugString(FileDialog.GetPathName());

				MyImage * image = new MyImage(FileDialog.GetPathName(), point.x, point.y);

				// Add to the list of figures
				this->figures.push_back(image);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				//text->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will call the draw method.
				cview->RedrawWindow();

				pushUndoStack();
			}
			else if (this->editMode == Drawing::NewOvalMode) {

				// Edit mode is NewRectangleMode. 
				// This is because the user just selected the Figure->Rectangle menu

				// Create a new rectangle.
				MyOval * oval = new MyOval(this->currentColor, point.x, point.y, point.x, point.y);

				// Add to the list of figures
				this->figures.push_back(oval);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				oval->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will call the draw method.
				cview->RedrawWindow();

				pushUndoStack();
			}
			else if (this->editMode == Drawing::SelectMode) {

				// The edit mode is selection.

				// Check first if there is a selected control point in 
				// mouse coordinates point.x,point.y
				ControlPoint * controlPoint = findControlPoint(point.x, point.y);
				if (controlPoint != NULL && controlPoint->isSelected()) {
					// Found a control point selected at this x, y

					// Deselect all control points
					this->selectAll(false);

					// Select the control point that was found.
					controlPoint->select(true);

					// Update previous mouse coordinates
					this->previousX = point.x;
					this->previousY = point.y;
				}
				else {
					// No selected control point was found.

					Figure * f;

					// Check if any of the figures selected is close to this coordinate
					if (isAnySelectedFigureCloseTo(point.x, point.y)) {
						// Yes. Update previous mouse coordinates so all the control points selected can be dragged.
						this->previousX = point.x;
						this->previousY = point.y;
					}

					// Check if there is a figure close to point.x,point.y even if it is 
					// not selected.
					else if ((f=isAnyFigureCloseTo(point.x, point.y))!=NULL) {
						// Yes. Deselect anything selected before and select this figure.
						this->selectAll(false);

						// Select this figure
						f->select(true);

						// Update previous mouse coordinates
						this->previousX = point.x;
						this->previousY = point.y;

						// Redraw window
						cview->RedrawWindow();
					}
					else {
						// No figure selected. Deselect all control points.
						this->selectAll(false);

						// Redraw window
						cview->RedrawWindow();

						// Store previous mouse coordinates
						this->previousX = point.x;
						this->previousY = point.y;
					}
				}
			}
		}
		else {
			// Mouse button was already pressed.

			// Check if any of the control points is selected.
			if (isSelected()) {
				// There are control points selected.

				// Mouse is being dragged. Drag control points
				dragSelectedControlPoints(point.x - this->previousX, point.y - this->previousY);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;
			}
			else {
				// There are no control points selected and mouse is being dragged.

				// Update selection rectangle
				updateSelectionRectangle(this->previousX, this->previousY, point.x, point.y);
			}		

			cview->RedrawWindow();
		}
	}
	else {
		// Button is not pressed.

		if (this->previousMouseMode == Drawing::ButtonPressed) {
			// Mouse button has just been released.

			if (this->selectionRectangleEnabled) {
				// Selection rectangle was enabled

				// Select figures inside the selection area.
				this->selectFiguresInArea(this->previousX, this->previousY, point.x, point.y);

				// Erase selection rectangle
				this->disableSelectionRectangle();
			}

			if (this->previousX == point.x && this->previousY == point.y) {
				//just clicked. 
			}
			else {
				pushUndoStack(); 
			}

			cview->RedrawWindow();
		}

		this->previousMouseMode = Drawing::NoButtonPressed;
	}
}

// Move an increment dx, dy the selected control points.
void Drawing::dragSelectedControlPoints( int dx, int dy ) {
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->dragSelectedControlPoints(dx, dy);
	}
}

// Select/deselct all figures in the drawing
void Drawing::selectAll(bool selected)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->select(selected);
	}
}

// Return true if there is a selected figure close to (x,y)
bool Drawing::isAnySelectedFigureCloseTo(int x, int y) 
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isSelected() && f->isCloseTo(x,y) ) {
			return true;
		}
	}

	return false;
}

// Return the figure selected or unselected that is close to the coordinate (x,y)
Figure * Drawing::isAnyFigureCloseTo(int x, int y) 
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isCloseTo(x,y) ) {
			return f;
		}
	}

	return NULL;
}

// Return true if any of the control points in the figures is selected
bool Drawing::isSelected() {
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isSelected() ) {
			return true;
		}
	}

	return false;
}

// Find the control point that encloses the coordinates (x,y)
ControlPoint * Drawing::findControlPoint(int x, int y)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		ControlPoint * c = f->findControlPoint(x,y);
		if ( c != NULL ) {
			return c;
		}
	}

	return NULL;
}

// Select control points that are inside a rectangular area
void Drawing::selectFiguresInArea(int x0, int y0, int x1, int y1)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if (f->isInsideArea(x0, y0, x1, y1)) {
			f->select(true);
		}
	}
}

// Enable and update coordinates of selection rectangle
void Drawing::updateSelectionRectangle(int x0, int y0, int x1, int y1)
{
	this->selectionRectangleX0 = x0;
	this->selectionRectangleY0 = y0;
	this->selectionRectangleX1 = x1;
	this->selectionRectangleY1 = y1;
	this->selectionRectangleEnabled = true;
}

// Disable selection rectangle
void Drawing::disableSelectionRectangle() 
{
	this->selectionRectangleEnabled = false;
}

// Draw selection rectangle if enabled
void Drawing::drawSelectionRectangle(CDC *pDC)
{
	if (!this->selectionRectangleEnabled) {
		return;
	}

	// Draw selection triangle
	CPen pen( PS_SOLID, 0, RGB( 255, 0, 0 ) );
	CPen* pOldPen = pDC->SelectObject( &pen );

	pDC->MoveTo(this->selectionRectangleX0, this->selectionRectangleY0);
	pDC->LineTo(this->selectionRectangleX1, this->selectionRectangleY0);
	pDC->LineTo(this->selectionRectangleX1, this->selectionRectangleY1);
	pDC->LineTo(this->selectionRectangleX0, this->selectionRectangleY1);
	pDC->LineTo(this->selectionRectangleX0, this->selectionRectangleY0);
}

void Drawing::Serialize(CArchive& ar) {

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << figures.size(); 
		for each (Figure * figure in figures) {
			ar << figure; 
		}
	}
	else
	{
		// TODO: add loading code here
		int num; 
		ar >> num; 
		for (int i = 0; i < num; i++) {
			Figure * figure; 
			ar >> figure; 
			figures.push_back(figure); 
		}
	}
}

/*
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << currentColor; 
		ar << figures.size(); 
		for each (Figure * figure in figures) {
			Figure::FigureType type = figure->getFigureType(); 
			ar << type; 

			if (type == Figure::FigureType::Group) {
				Group * group = dynamic_cast<Group *>(figure); 
				
				vector<Figure *> gfigs = group->getFigures(); 

				ar << gfigs.size(); 

				for each (Figure * figure in gfigs) {
					ar << figure->getFigureType(); 
					ar << figure->getColor();
					for each (ControlPoint * cp in figure->getControlPoints()) {
						ar << cp->getX() << cp->getY();
					}
				}
			}
			else {
				ar << figure->getColor(); 
				for each (ControlPoint * cp in figure->getControlPoints()) {
					ar << cp->getX() << cp->getY(); 
				}
			}
			
		}
	}
	else
	{
		ar >> currentColor; 
		int numFigures; 
		ar >> numFigures; 

		for (int i = 0; i < numFigures; i++) {

			int foo;
			ar >> foo;
			Figure::FigureType type = (Figure::FigureType) foo;

			if (type == Figure::FigureType::Group) {
				popGroupOffArchiveTo(ar,type, figures); 
			}
			else {
				popFigureOffArchiveTo(ar, figures); 
			}
			
		}
	}
}

void Drawing::popFigureOffArchiveTo(CArchive& ar, Figure::FigureType type, vector<Figure *> vect) {
	COLORREF color;
	ar >> color;
	int x, y, xx, yy;
	ar >> x >> y >> xx >> yy;
	Figure * fig;
	switch (type) {
	case Figure::FigureType::Circle:
		fig = new MyOval(color, x, y, xx, yy);
		break;
	case Figure::FigureType::Line:
		fig = new Line(color, x, y, xx, yy);
		break;
	case Figure::FigureType::Rectangle:
		fig = new MyRectangle(color, x, y, xx, yy);
		break;

	}
	vect.push_back(fig);
}

void Drawing::popGroupOffArchiveTo(CArchive& ar, vector<Figure *> vect) {
	int gsize;
	ar >> gsize;
	for (int v = 0; v < gsize; v++) {

		vector<Figure *> gfigs; 
		
		Figure::FigureType type = 


	}
}*/