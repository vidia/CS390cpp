#include "stdafx.h"
#include "Group.h"
#include "MyOval.h"
#include "Rectangle.h"
#include "Line.h"


Group::Group(vector<Figure *> figs)
	:Figure(Figure::FigureType::Group, RGB(0,0,0))
{
	figures = figs;

	int minx, miny, maxx, maxy; 
	for each (Figure * figure in figures) {
		for each (ControlPoint * cp in figure->getControlPoints()) {
			figControlPoints.push_back(cp);
		}
	}

	minx = maxx = figControlPoints[0]->getX();
	miny = maxy = figControlPoints[0]->getY();
	for  each (ControlPoint * cp in figControlPoints) {
		int x = cp->getX(); 
		int y = cp->getY(); 
		if (x < minx) {
			minx = x; 
		}
		else if (x > maxx) {
			maxx = x; 
		}

		if (y < miny) {
			miny = y; 
		}
		else if (y > maxy) {
			maxy = y; 
		}
	}

	controlPoints.push_back(new ControlPoint(this, minx, miny));
	controlPoints.push_back(new ControlPoint(this, maxx, maxy));
}

Group::~Group()
{
	while (figures.size()) {
		Figure * fig = figures[figures.size() - 1]; 
		delete fig; 
	}
}

void
Group::draw(CDC* pDC) {
	for each (Figure * figure in figures) {
		figure->draw(pDC); 
	}
}

vector<Figure *> & 
Group::getFigures() {
	return figures; 
}

Group * 
Group::clone() const {
	vector<Figure *> copies;
	for each (Figure * figure in figures) {
		copies.push_back(figure->clone());
	}
	return new Group(copies); 
}

void
Group::setColor(COLORREF color) {
	for each (Figure * fig in figures) {
		fig->setColor(color); 
	}
}

bool 
Group::isCloseTo(int x, int y) {
	for each (Figure * figure in figures) {
		if (figure->isCloseTo(x, y))
			return true; 
	}
	return false; 
}

void Group::dragSelectedControlPoints(int dx, int dy)
{
	int count = 0; 
	for each (ControlPoint * cp in controlPoints) {
		if (cp->isSelected()) {
			count++; 
		}
	}

	if (count == 2) {
		for each (Figure * fig in figures) {
			fig->select(true);
			fig->dragSelectedControlPoints(dx, dy); 
			fig->select(false); 
		}
		/*for (unsigned i = 0; i < this->figControlPoints.size(); i++) {
			ControlPoint * c = figControlPoints.at(i);
			c->move(c->getX() + dx, c->getY() + dy);
		}*/
		
		for (unsigned i = 0; i < this->controlPoints.size(); i++) {
			ControlPoint * c = controlPoints.at(i);
			if (c->isSelected()) {
				c->move(c->getX() + dx, c->getY() + dy);
			}
		}
	}
	else if (count == 1) {

		float width = abs(controlPoints[0]->getX() - controlPoints[1]->getX());
		float height = abs(controlPoints[0]->getY() - controlPoints[1]->getY());

		for (unsigned i = 0; i < this->controlPoints.size(); i++) {
			ControlPoint * c = controlPoints.at(i);
			if (c->isSelected()) {
				c->move(c->getX() + dx, c->getY() + dy);
			}
		}

		double sx = (width + dx) / width;
		double sy = (height + dy) / height;

		for (unsigned i = 0; i < this->figControlPoints.size(); i++) {
			ControlPoint * c = figControlPoints.at(i);
			double newx = (c->getX() - controlPoints[0]->getX()) * 1.0 * sx + controlPoints[0]->getX();
			double newy = (c->getY() - controlPoints[0]->getY()) * 1.0 * sy + controlPoints[0]->getY(); 
			c->move(round(newx), round(newy));
		}
	}

	//TODO: To do scaling properly, track the original locations of all of the points 
	//and the percent size that it is currently. Do all calculations on the original
	//points and not the current.

	/*
	Make a groups it's own coord system. Change figures to match the new coords. 
	When translating, only the group cp's need updating. When drawing refer to those
	cp's for a percentage and draw with the scale from the figure's new points. 
	If possible, do not overwrite the grouped figure's coords and calculate on the fly.*/
}
