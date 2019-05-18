// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:50:41 2019
//  Last Modified : <190517.2229>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <QDebug>
#include <QRectF>

#include "fseedit.h"

FESchematicEditor::FESchematicEditor(SizeAndVP::UnitsType units,double width,double height,const QRectF &viewport,QWidget *parent)
      : FEEdit(units,width,height,viewport,parent)
{
}

FESchematicEditor::~FESchematicEditor()
{
}


void FESchematicEditor::addPin()
{
    qDebug() << "FESchematicEditor::addPin()";
}

void FESchematicEditor::editPin(int gid)
{
}

void FESchematicEditor::addRect()
{
}

void FESchematicEditor::editRect(int gid)
{
}

void FESchematicEditor::addLine()
{
}

void FESchematicEditor::editLine(int gid)
{
}

void FESchematicEditor::addCirc()
{
}

void FESchematicEditor::editCirc(int gid)
{
}

void FESchematicEditor::addArc()
{
}

void FESchematicEditor::editArc(int gid)
{
}

void FESchematicEditor::addPoly()
{
}

void FESchematicEditor::editPoly(int gid)
{
}

void FESchematicEditor::addText()
{
}

void FESchematicEditor::editText(int gid)
{
}


