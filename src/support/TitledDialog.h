// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 19 07:51:56 2019
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

#ifndef __TITLEDDIALOG_H
#define __TITLEDDIALOG_H

#include <QDialog>
#include <QLabel>
class QWidget;

class TitledDialog : public QDialog
{
        Q_OBJECT
public:
    enum IconType {Info, Warning, Question, Error};
    TitledDialog(IconType type = Question, QWidget *parent = NULL);
    TitledDialog(const QIcon &userIcon, QWidget *parent = NULL);
    TitledDialog(const QPixmap &userPixmap, QWidget *parent = NULL);
    inline QString title() const {return titleLB->text();}
    inline void setTitle(const QString &title) {titleLB->setText(title);}
    inline QWidget* getFrame() {return body;}
    inline void setLayout(QLayout *layout) {body->setLayout(layout);}
private:
    void _createDialog(const QPixmap &pixmap);
    QWidget *body;
    QLabel *titleLB;
};

#endif // __TITLEDDIALOG_H

