// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 19 08:21:59 2019
//  Last Modified : <190522.1203>
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

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QIcon>

#include "TitledDialog.h"
#include "error.xpm"
#include "warning.xpm"
#include "info.xpm"
#include "questhead.xpm"

TitledDialog::TitledDialog(TitledDialog::IconType type, QWidget *parent)
      : QDialog(parent)
{
    switch (type) {
    case Info:     _createDialog(QPixmap(info)); break;
    case Warning:  _createDialog(QPixmap(warning)); break;
    case Question: _createDialog(QPixmap(questhead)); break;
    case Error:    _createDialog(QPixmap(error)); break;
    }
}

TitledDialog::TitledDialog(const QPixmap &userPixmap, QWidget *parent)
      : QDialog(parent)
{
    _createDialog(userPixmap);
}

TitledDialog::TitledDialog(const QIcon &userIcon, QWidget *parent)
      : QDialog(parent)
{
    _createDialog(userIcon.pixmap(32,32));
}


void TitledDialog::_createDialog(const QPixmap &pixmap)
{
    QWidget *heading = new QWidget(this);
    body = new QWidget(this);
    QLabel *bitmapLB = new QLabel(heading);
    bitmapLB->setPixmap(pixmap);
    titleLB = new QLabel(heading);
    QHBoxLayout *hlayout = new QHBoxLayout(heading);
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    hlayout->addWidget(bitmapLB);
    hlayout->addWidget(titleLB);
    heading->setLayout(hlayout);
    mainlayout->addWidget(heading);
    mainlayout->addWidget(body);
    QDialog::setLayout(mainlayout);
}
