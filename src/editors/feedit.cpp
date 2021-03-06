// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:49:56 2019
//  Last Modified : <190527.0856>
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


/** @page FritzingPartsEditor Common GUI elements
  * @section CoordinateInfo Coordinate Information
  * Along the bottom of each tab pane is information about the coordinate system
  * in use for the current pane.  First (going from left to right) is the 
  * physical size, in either milimeters or inches, then the current viewport, 
  * and then finally the current pointer position in the viewport coordinate
  * system.  Additionally, a dashed box is shown on the drawing area showing the
  * bounds of the viewport coordinate system.
  * @addindex Coordinate System
  * @section ToolButtons Tool Buttons
  * To the right of the drawing display area are a collection of tool buttons.
  * There six buttons for inserting graphical elements.  These buttons are tab
  * specific and their functionallity is described in the tab-specific sections 
  * of this manual.  The bottom three buttons are the same for all three tabs.
  * The bottom threse buttons are:
  * <dl>
  * <dt>Size</dt><dd>The \b Side tool button changes the coordinate system. It
  * displays a dialog box asking for new values for the width, height, units,
  * and viewport.</dd>
  * <dt>ShrinkWrap</dt><dd>The \b ShrinkWrap tool button "shrink wraps" a 
  * (presumably) finished part.  It recomputes a viewport that just encloses the 
  * part.  This includes computing the exact size of the part.</dd>
  * <dt>Zoom</dt><dd>The \b Zoom tool button lets you zoom in or out. See 
  * \ref bindings.</dd>
  * </dl>
  * @section bindings Keyboard and mouse bindings
  * There are some common keyboard and pointer bindings. There is a context menu
  * bound to the right pointer button.  Right-clicking on a graphical element 
  * brings up a menu of things you can do to that element: delete or edit, with
  * keyboard accelerators Delete and E, respectively.
  *
  * Additionally, the F1 key is bound to zoom in, the F2 key is bound to zoom out,
  * and the F3 is bound to zoom 1:1.
  */ 



static const char rcsid[] = "@(#) : $Id$";

#include <iostream>
#include <QDebug>
#include <QtWidgets>
#include <QToolBar>
#include <QToolButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QList>
#include <QScrollArea>
#include <QMenu>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRectF>
#include <QVariant>
#include <QDomNodeList>
#include <QDomNode>

#include "feedit.h"
#include "../support/commonDialogs.h"
#include "setsizedialog.h"

ToolMenuButton::ToolMenuButton(QWidget *parent) : QToolButton(parent)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setArrowType(Qt::DownArrow);
    setToolButtonStyle(Qt::ToolButtonTextOnly);
    QObject::connect(this, SIGNAL(triggered(QAction*)),this, 
                     SLOT(setDefaultAction(QAction*)));
}

void FEGraphicsScene::moveItems(qreal dx, qreal dy)
{
    ItemList allitems = items();
    for (items_constIterator i = allitems.begin(); i != allitems.end(); i++) {
        QGraphicsItem *item = *i;
        switch (item->type()) {
        case QGraphicsEllipseItem::Type:
            {
                QGraphicsEllipseItem *eitem = (QGraphicsEllipseItem *) item;
                QRectF rect = eitem->rect();
                rect.adjust(dx,dy,dx,dy);
                eitem->setRect(rect);
                break;
            }
        case QGraphicsRectItem::Type:
            {
                QGraphicsRectItem *eitem = (QGraphicsRectItem *) item;
                QRectF rect = eitem->rect();
                rect.adjust(dx,dy,dx,dy);
                eitem->setRect(rect);
                break;
            }
        case QGraphicsLineItem::Type:
            {
                QGraphicsLineItem *eitem = (QGraphicsLineItem *) item;
                QLineF line = eitem->line();
                line.setLine(line.x1()+dx,line.y1()+dy,line.x2()+dx,line.y2()+dy);
                eitem->setLine(line);
                break;
            }
        case QGraphicsPathItem::Type:
            {
                QGraphicsPathItem *eitem = (QGraphicsPathItem *) item;
                QPainterPath path = eitem->path();
                for (int i = 0; i < path.elementCount(); i++) {
                    QPainterPath::Element pEle = path.elementAt(i);
                    pEle.x += dx;
                    pEle.y += dy;
                }
                eitem->setPath(path);
                break;
            }
        case QGraphicsPolygonItem::Type:
            {
                QGraphicsPolygonItem *eitem = (QGraphicsPolygonItem *) item;
                QPolygonF points = eitem->polygon();
                for (QPolygonF::iterator ip = points.begin();
                     ip !=  points.end();
                     ip++) {
                    QPointF p = *ip;
                    p.setX(p.x()+dx);
                    p.setY(p.y()+dy);
                }
                eitem->setPolygon(points);
                break;
            }
        case QGraphicsSimpleTextItem::Type:
            {
                QGraphicsSimpleTextItem *titem = (QGraphicsSimpleTextItem *) item;
                QPointF p = titem->pos();
                p.setX(p.x()+dx);
                p.setY(p.y()+dy);
                titem->setPos(p);
                break;
            }
        default:
            break;
        }
    }
}

FEEdit::FEEdit(SizeAndVP::UnitsType units, double width, 
               double height, 
               const QRectF &viewport, 
               QWidget *parent) 
{
    gid = 0;
    pinno = 0;
    _zoomScale = 1.0;
    _vpscale   = 1.0;
    canvas = new FEGraphicsScene;
    canvasView = new FEGraphicsView(canvas,this);
    connect(canvasView,SIGNAL(mouseMoved(QMouseEvent *)),this,SLOT(mouseMoved(QMouseEvent *)));
    connect(canvasView,SIGNAL(mousePressed(QMouseEvent *)),this,SLOT(mousePressed(QMouseEvent *)));
    connect(canvasView,SIGNAL(keyPressed(QKeyEvent *)),this,SLOT(keyPressed(QKeyEvent *)));
    connect(canvasView,SIGNAL(resized(QResizeEvent *)),this,SLOT(resized(QResizeEvent *)));
    toolbuttons = new QToolBar(this);
    toolbuttons->setOrientation(Qt::Vertical);
    toolbuttons->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sizeAndVP = new SizeAndVP(units,width,height,viewport,this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(canvasView,0,0);
    layout->addWidget(toolbuttons,0,1);
    layout->addWidget(sizeAndVP,1,0,1,2);
    setLayout(layout);
    createToolButtons();
    createContextMenus();
    setsizedialog = new SetSizeDialog;
    sizeAndVP->updateZoom(_zoomScale);
    _vpRect = NULL;
    makeVpRect();
    setClean();
    connect(canvas,SIGNAL(changed(const QList<QRectF> &)),this,SLOT(makeDirty(const QList<QRectF> &)));
}

FEEdit::~FEEdit()
{
}

void FEEdit::createContextMenus()
{
    itemcontextmenu = new QMenu(tr("Item nnn"));
    canvascontextmenu = new FEContextMenu(tr("Select item"));
}

void FEEdit::createToolButtons()
{
    addPinAct = new QAction(QIcon(":/resources/images/small_addpin.png"), tr("Add Pin"), this);
    addPinAct->setStatusTip(tr("Add a pin"));
    connect(addPinAct, SIGNAL(triggered()), this, SLOT(addPin()));
    toolbuttons->addAction(addPinAct);
    
    addRectAct = new QAction(QIcon(":/resources/images/small_addrect.png"), tr("Add Rect"), this);
    addRectAct->setStatusTip(tr("Add a rectangle"));
    connect(addRectAct, SIGNAL(triggered()), this, SLOT(addRect()));
    toolbuttons->addAction(addRectAct);
    
    addLineAct = new QAction(QIcon(":/resources/images/small_addline.png"), tr("Add Line"), this);
    addLineAct->setStatusTip(tr("Add a line"));
    connect(addLineAct, SIGNAL(triggered()), this, SLOT(addLine()));
    toolbuttons->addAction(addLineAct);
    
    addCircAct = new QAction(QIcon(":/resources/images/small_addcirc.png"), tr("Add Circ"), this);
    addCircAct->setStatusTip(tr("Add a circle"));
    connect(addCircAct, SIGNAL(triggered()), this, SLOT(addCirc()));
    toolbuttons->addAction(addCircAct);
    
    addArcAct = new QAction(QIcon(":/resources/images/small_addarc.png"), tr("Add Arc"), this);
    addArcAct->setStatusTip(tr("Add a arc"));
    connect(addArcAct, SIGNAL(triggered()), this, SLOT(addArc()));
    toolbuttons->addAction(addArcAct);
    
    addPolyAct = new QAction(QIcon(":/resources/images/small_addpoly.png"), tr("Add Poly"), this);
    addPolyAct->setStatusTip(tr("Add a polyline/polygon"));
    connect(addPolyAct, SIGNAL(triggered()), this, SLOT(addPoly()));
    toolbuttons->addAction(addPolyAct);
    
    addTextAct = new QAction(QIcon(":/resources/images/small_addtext.png"), tr("Add Text"), this);
    addTextAct->setStatusTip(tr("Add a text"));
    connect(addTextAct, SIGNAL(triggered()), this, SLOT(addText()));
    toolbuttons->addAction(addTextAct);
    
    setsizeAct = new QAction(QIcon(":/resources/images/small_setsize.png"), tr("Size"), this);
    setsizeAct->setStatusTip(tr("Change the size"));
    connect(setsizeAct, SIGNAL(triggered()), this, SLOT(setsize()));
    toolbuttons->addAction(setsizeAct);
    
    shrinkWrapAct = new QAction(QIcon(":/resources/images/small_shrinkwrap.png"), tr("Shrink Wrap"), this);
    shrinkWrapAct->setStatusTip(tr("Shrink Wrap"));
    connect(shrinkWrapAct, SIGNAL(triggered()), this, SLOT(shrinkwrap()));
    toolbuttons->addAction(shrinkWrapAct);
    
    createZoomMenu();
    
    zoomButton = new ToolMenuButton;
    zoomButton->setMenu(zoomMenu);
    zoomButton->setDefaultAction(setZoom_1Act);
    toolbuttons->addWidget(zoomButton);
}

void FEEdit::createZoomMenu()
{
    zoomMenu = new QMenu(toolbuttons);
    setZoom_16Act = new QAction(tr("Zoom 16:1"), this);
    zoomMenu->addAction(setZoom_16Act);
    connect(setZoom_16Act, SIGNAL(triggered()), this, SLOT(setZoom_16()));
    setZoom_8Act = new QAction(tr("Zoom 8:1"), this);
    zoomMenu->addAction(setZoom_8Act);
    connect(setZoom_8Act, SIGNAL(triggered()), this, SLOT(setZoom_8()));
    setZoom_4Act = new QAction(tr("Zoom 4:1"), this);
    zoomMenu->addAction(setZoom_4Act);
    connect(setZoom_4Act, SIGNAL(triggered()), this, SLOT(setZoom_4()));
    setZoom_2Act = new QAction(tr("Zoom 2:1"), this);
    zoomMenu->addAction(setZoom_2Act);
    connect(setZoom_2Act, SIGNAL(triggered()), this, SLOT(setZoom_2()));
    setZoom_1Act = new QAction(tr("Zoom 1:1"), this);
    zoomMenu->addAction(setZoom_1Act);
    connect(setZoom_1Act, SIGNAL(triggered()), this, SLOT(setZoom_1()));
    setZoom__5Act = new QAction(tr("Zoom 1:2"), this);
    zoomMenu->addAction(setZoom__5Act);
    connect(setZoom__5Act, SIGNAL(triggered()), this, SLOT(setZoom__5()));
    setZoom__25Act = new QAction(tr("Zoom 1:4"), this);
    zoomMenu->addAction(setZoom__25Act);
    connect(setZoom__25Act, SIGNAL(triggered()), this, SLOT(setZoom__25()));
    setZoom__125Act = new QAction(tr("Zoom 1:8"), this);
    zoomMenu->addAction(setZoom__125Act);
    connect(setZoom__125Act, SIGNAL(triggered()), this, SLOT(setZoom__125()));
    setZoom__0625Act = new QAction(tr("Zoom 1:16"), this);
    zoomMenu->addAction(setZoom__0625Act);
    connect(setZoom__0625Act, SIGNAL(triggered()), this, SLOT(setZoom__0625()));
}

void FEEdit::deleteItem (int gid, const QString & label)
{
    if (!label.isEmpty()) {
        if (CommonDialog::YesNoDialog->draw(QString(tr("Really delete %1?")).arg(label)) == 
            CommonDialog::NO) {return;}
    }
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid));
    for (items_Iterator ii = items.begin();
         ii != items.end();
         ii++) {
        QGraphicsItem *item = *ii;
        canvas->removeItem(item);
        delete item;
    }
}

void FEEdit::clean()
{
    _vpRect = NULL;
    canvas->clear();
    makeVpRect();
    setClean();
}

void FEEdit::setsize()
{
    double width = Width(), height = Height();
    SizeAndVP::UnitsType units = Units();
    QRectF vp;
    Viewport(vp);
    if (setsizedialog->draw(width,height,vp,units)) {
        updateSize(vp,width,height,units);
    }
}

void FEEdit::shrinkwrap()
{
    canvas->removeItem(_vpRect);
    QRectF bbox = canvas->itemsBoundingRect();
    //stdError << " FEEdit::shrinkwrap(): bbox is " << bbox << "\n";
    canvas->moveItems(0-bbox.x(),0-bbox.y());
    //bbox = canvas->itemsBoundingRect();
    //stdError << " FEEdit::shrinkwrap(): bbox (after move) is " << bbox << "\n";
    canvas->addItem(_vpRect);
    QRectF vp;
    sizeAndVP->Viewport(vp);
    double scaleW = bbox.width() / vp.width();
    double scaleH = bbox.height() / vp.height();
    double newwidth = Width()*scaleW;
    double newheight = Height()*scaleH;
    vp.setWidth(bbox.width());
    vp.setHeight(bbox.height());
    updateSize(vp,newwidth,newheight,Units());
    setDirty();
}



void FEEdit::mouseMoved(QMouseEvent * event)
{
    int x = event->x();
    int y = event->y();
    QPointF canvasCoords = canvasView->mapToScene(x,y);
    double xx = canvasCoords.x();
    double yy = canvasCoords.y();
    //xx /= _zoomScale;
    //yy /= _zoomScale;
    sizeAndVP->updatexyposition(xx,yy);
}

void FEEdit::mousePressed(QMouseEvent * event)
{
    Qt::MouseButton b = event->button();
    if (b != Qt::RightButton) return;
    int x = event->x();
    int y = event->y();
    QPointF canvasCoords = canvasView->mapToScene(x,y);
    double xx = canvasCoords.x();
    double yy = canvasCoords.y();
    ItemList nearbyItems = canvas->items(QRectF(xx-10, yy-10, 20, 20));
    if (nearbyItems.size() == 0) return;
    canvascontextmenu->clear();
    int X = event->globalX();
    int Y = event->globalY();
    FEICAction *act = NULL;
    QString label;
    int gid;
    //stdError << "FEEdit::mousePressed()" << '\n';
    for (items_constIterator ii = nearbyItems.begin();
         ii != nearbyItems.end();
         ii++) {
        QGraphicsItem *item = *ii;
        //stdError << "FEEdit::mousePressed(): item is " << item << '\n';
        int newgid = item->data((int)FEGraphicsScene::Gid).toInt();
        //stdError << "FEEdit::mousePressed(): gid = " << gid << '\n';
        FEGraphicsScene::ItemType itemtype = (FEGraphicsScene::ItemType)
              item->data((int)FEGraphicsScene::Type).toInt();
        if (newgid < 1 || itemtype == FEGraphicsScene::Undefined) {
            continue;
        }
        gid = newgid;
        switch (itemtype) {
        case FEGraphicsScene::Pin: {
            int pinno = item->data((int)FEGraphicsScene::Pinno).toInt();
            if (pinno == 0) continue;
            label = QString("Pin %1").arg(pinno);
            if (canvascontextmenu->findAction(label) != NULL) {continue;}
            break;
        }            
        case FEGraphicsScene::Rect: {
            label = QString("Rectangle %1").arg(gid);
            break;
        }
        case FEGraphicsScene::Line: {
            label = QString("Line %1").arg(gid);
            break;
        }
        case FEGraphicsScene::Circ: {
            label = QString("Circle %1").arg(gid);
            break;
        }
        case FEGraphicsScene::Arc: {
            label = QString("Arc %1").arg(gid);
            break;
        }
        case FEGraphicsScene::Poly: {
            label = QString("Polygon %1").arg(gid);
            break;
        }
        case FEGraphicsScene::Text: {
            label = QString("Text %1").arg(gid);
            break;
        }
        default:
            continue;
        }
        act = new FEICAction(gid, X, Y, label, canvascontextmenu);
        canvascontextmenu->insertAction(NULL,act);
        connect(act,SIGNAL(triggered()),act,SLOT(ContextTrigger()));
        connect(act,SIGNAL(ContextTriggerAll(int,const QString &,int,int)),
                this,SLOT(itemContext(int,const QString &,int,int)));
    }
    if (canvascontextmenu->isEmpty()) {return;}
    if (canvascontextmenu->actionCount() == 1) {
        itemContext(gid,label,X,Y);
        return;
    }
    QAction *cancel = canvascontextmenu->addAction(tr("Cancel"));
    connect(cancel,SIGNAL(triggered()),canvascontextmenu,SLOT(close()));
    canvascontextmenu->exec(QPoint(X,Y));
}

void FEEdit::itemContext(int gid, const QString &label, int X, int Y)
{
    //stdError << "FEEdit::itemContext(" << gid << "," << label << "," << X << "," << Y << ")" << '\n';
    itemcontextmenu->setTitle(label);
    itemcontextmenu->clear();
    FEICAction *a = new FEICAction(gid, X, Y, QString(tr("Edit %1")).arg(label), itemcontextmenu);
    itemcontextmenu->insertAction(NULL, a);
    connect(a,SIGNAL(triggered()),a,SLOT(ContextTrigger()));
    connect(a,SIGNAL(ContextTriggerGid(int)),this,SLOT(editItem(int)));
    a = new FEICAction(gid, X, Y,QString(tr("Delete %1")).arg(label), itemcontextmenu);
    itemcontextmenu->insertAction(NULL, a);
    connect(a,SIGNAL(triggered()),a,SLOT(ContextTrigger()));
    connect(a,SIGNAL(ContextTriggerGidLabel(int, const QString &)),this,SLOT(deleteItem(int, const QString&)));
    QAction *cancel = itemcontextmenu->addAction(tr("Cancel"));
    connect(cancel,SIGNAL(triggered()),itemcontextmenu,SLOT(close()));
    itemcontextmenu->exec(QPoint(X,Y));          
}

void FEEdit::editItem(int gid)
{
    //stdError << "FEEdit::editItem(" << gid << ")" << '\n';
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,
                                       QVariant(gid));
    for (items_constIterator i = items.begin(); i != items.end(); i++)
    {
        QGraphicsItem *item = *i;
        FEGraphicsScene::ItemType itemtype = (FEGraphicsScene::ItemType)
              item->data((int)FEGraphicsScene::Type).toInt();
        switch (itemtype) {
        case FEGraphicsScene::Pin:
            editPin(gid); return;
        case FEGraphicsScene::Rect:
            editRect(gid); return;
        case FEGraphicsScene::Line:
            editLine(gid); return;
        case FEGraphicsScene::Circ:
            editCirc(gid); return;
        case FEGraphicsScene::Arc:
            editArc(gid); return;
        case FEGraphicsScene::Poly:
            editPoly(gid); return;
        case FEGraphicsScene::Text:
            editText(gid); return;
        default:
            break;
        }
    }
}

void FEEdit::keyPressed(QKeyEvent * event)
{
    int k = event->key();
    switch (k) {
    case Qt::Key_F1: zoomBy(2.0); break;
    case Qt::Key_F2: zoomBy(.5); break;
    case Qt::Key_F3: setZoom(1.0); break;
    default: return;
    }
}

void FEEdit::resized(QResizeEvent * event)
{
    FEEdit::makeVpRect();
}

void FEEdit::updateSize(const QRectF &vp, qreal width, qreal height, SizeAndVP::UnitsType unit)
{
    sizeAndVP->setViewport(vp);
    sizeAndVP->setUnits(unit);
    sizeAndVP->setWidth(width);
    sizeAndVP->setHeight(height);
    makeVpRect();
}


void FEEdit::makeVpRect()
{
    //stdError << "*** FEEdit::makeVpRect()" << '\n';
    inVPRect = true;
    QRectF vp;
    sizeAndVP->Viewport(vp);
    double vpwidth = vp.width();
    double vpheight = vp.height();
    //stdError << "*** FEEdit::makeVpRect() vp is " << vp << '\n';
    double width = sizeAndVP->Width();
    double height = sizeAndVP->Height();
    //stdError << "*** FEEdit::makeVpRect() width is " << width << ", height is " << height << '\n'; 
    double inv = 1.0/_zoomScale;
    canvasView->scale(inv,inv);
    QSize canvasSize = canvasView->size();
    int ch = canvasSize.height();
    int cw = canvasSize.width();
    //stdError << "*** FEEdit::makeVpRect() ch is " << ch << ", cw is " << cw << '\n';
    double newvpscale;
    if (ch <cw) {
        newvpscale = ((double)ch)/vpheight;
    } else {
        newvpscale = ((double)cw)/vpwidth;
    }
    //stdError << "*** FEEdit::makeVpRect() newvpscale is " << newvpscale << '\n';
    if (_vpRect != NULL) {
        canvas->removeItem(_vpRect);
        delete _vpRect;
        _vpRect = NULL;
    }
    //double inv_vpscale = 1.0 / _vpscale;
    //canvasView->scale(inv_vpscale,inv_vpscale);
    _vpscale = newvpscale;
    //canvasView->scale(_vpscale,_vpscale);
    _vpRect = canvas->addRect(vp,QPen(Qt::DashLine),QBrush(Qt::NoBrush));
    double srx1,srx2, sry1, sry2;
    if (vpwidth < cw) {
        double halfspace = (cw - vpwidth) / 2.0;
        srx1 = 0-halfspace;
        srx2 = vpwidth+halfspace;
    } else {
        srx1 = 0;
        srx2 = vpwidth;
    }
    if (vpheight < ch) {
        double halfspace = (ch - vpheight) / 2.0;
        sry1 = 0-halfspace;
        sry2 = vpheight+halfspace;
    } else {
        sry1 = 0;
        sry2 = vpheight;
    }
    //stdError << "*** FEEdit::makeVpRect() new SR is " << srx1 << " " << sry1 << " " << srx2 << " " << sry2 << '\n';
    canvasView->setSceneRect(srx1,sry1,srx2,sry2);
    canvasView->scale(_zoomScale,_zoomScale);
    updateSR();
}

void FEEdit::setZoom(double zoomFactor)
{
    if (_zoomScale != 1.0) {
        double inv = 1.0 / _zoomScale;
        canvasView->scale(inv,inv);
    }
    canvasView->scale(zoomFactor,zoomFactor);
    _zoomScale = zoomFactor;
    sizeAndVP->updateZoom(_zoomScale);
    //$toolbuttons itemconfigure zoom -text "[formatZoom $_zoomScale]"
    updateSR();
}

void FEEdit::zoomBy(double zoomFactor)
{
    canvasView->scale(zoomFactor,zoomFactor);
    _zoomScale *= zoomFactor;
    sizeAndVP->updateZoom(_zoomScale);
    //$toolbuttons itemconfigure zoom -text "[formatZoom $_zoomScale]"
    updateSR();
}

void FEEdit::updateSR()
{
    bool newSR = false;
    QRectF curSR = canvas->sceneRect();
    QRectF bbox = canvas->itemsBoundingRect();
    if (curSR.width() != bbox.width()) {
        curSR.setWidth(bbox.width());
        newSR = true;
    }
    if (curSR.height() != bbox.height()) {
        curSR.setHeight(bbox.height());
        newSR = true;
    }
    if (newSR) {
        canvas->setSceneRect(curSR);
    }
}

SizeAndVP::SizeAndVP(UnitsType units, double width, double height,            
                     const QRectF &viewport, QWidget *parent)
      : _units(units), _width(width), _height(height), _viewport(viewport)
{
    //QLabel *li, *l2, *vp, *posl, *l3;
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *l1 = new QLabel("Width: ",this);
    layout->addWidget(l1); // check
    w = new QLineEdit(this);
    w->setReadOnly(true);
    //w->setMaxLength(4);
    w->setText(QString::number(_width,'f',2));
    layout->addWidget(w); // check
    wUnits = new QLabel(((_units==mm)?"mm":"in"),this);
    layout->addWidget(wUnits);
    QLabel *l2 = new QLabel(" Height: ",this);
    layout->addWidget(l2);
    h = new QLineEdit(this);
    h->setReadOnly(true);
    //h->setMaxLength(4);
    h->setText(QString::number(_height,'f',2));
    layout->addWidget(h);
    hUnits = new QLabel(((_units==mm)?"mm":"in"),this);
    layout->addWidget(hUnits);
    QLabel *vp = new QLabel(" Viewport: ",this);
    layout->addWidget(vp);
    x1 = new QLineEdit(this);
    x1->setReadOnly(true);
    //x1->setMaxLength(4);
    x1->setText(QString::number(_viewport.x(),'f',2));
    layout->addWidget(x1);
    y1 = new QLineEdit(this);
    y1->setReadOnly(true);
    //y1->setMaxLength(4);
    y1->setText(QString::number(_viewport.y(),'f',2));
    layout->addWidget(y1);
    x2 = new QLineEdit(this);
    x2->setReadOnly(true);
    //x2->setMaxLength(4);
    x2->setText(QString::number(_viewport.width(),'f',2));
    layout->addWidget(x2);
    y2 = new QLineEdit(this);
    y2->setReadOnly(true);
    //y2->setMaxLength(4);
    y2->setText(QString::number(_viewport.height(),'f',2));
    layout->addWidget(y2);
    QLabel *posl = new QLabel(" Position: ",this);
    layout->addWidget(posl);
    xpos = new QLineEdit(this);
    xpos->setReadOnly(true);
    //xpos->setMaxLength(4);
    xpos->setText(QString::number(_xpos,'f',2));
    layout->addWidget(xpos);
    ypos = new QLineEdit(this);
    ypos->setReadOnly(true);
    //ypos->setMaxLength(4);
    ypos->setText(QString::number(_ypos,'f',2));
    layout->addWidget(ypos);
    QLabel *zlab = new QLabel(" Zoom: ",this);
    layout->addWidget(zlab);
    z = new QLineEdit(this);
    z->setReadOnly(true);
    //z->setMaxLength(4); 
    z->setText(formatZoom(_zoom));
    layout->addWidget(z);
}

SizeAndVP::~SizeAndVP()
{
}

void SizeAndVP::setUnits(SizeAndVP::UnitsType u)
{
    _units = u;
    switch (u) {
    case mm:
        wUnits->setText("mm");
        hUnits->setText("mm");
        break;        
    case in:
        wUnits->setText("in");
        hUnits->setText("in");
        break;
    }
}

void SizeAndVP::setViewport(const QRectF &vp)
{
    _viewport = vp;
    x1->setText(QString::number(_viewport.x(),'f',2));
    y1->setText(QString::number(_viewport.y(),'f',2));
    x2->setText(QString::number(_viewport.width(),'f',2));
    y2->setText(QString::number(_viewport.height(),'f',2));
}

void SizeAndVP::setWidth(double ww)
{
    _width = ww;
    w->setText(QString::number(_width,'f',2));
}

void SizeAndVP::setHeight(double hh)
{
    _height = hh;
    h->setText(QString::number(_height,'f',2));
}

void SizeAndVP::updatexyposition(double x, double y)
{
    _xpos = x;
    _ypos = y;
    xpos->setText(QString::number(_xpos,'f',2));
    ypos->setText(QString::number(_ypos,'f',2));
}

void SizeAndVP::updateZoom(double zoom)
{
    _zoom = zoom;
    z->setText(formatZoom(_zoom));
}

QString SizeAndVP::formatZoom(double z)
{
    if (z > 1) {
        return QString("%1:1").arg(z,0,'f',0);
    } else {
        return QString("1:%1").arg(1.0/z,0,'f',0);
    }
}

void FEEdit::processSVGGroup(QDomElement &group)
{
    QDomNodeList childen = group.childNodes();
    for (int i = 0; i < childen.length(); i++) {
        QDomNode cn = childen.at(i);
        if (cn.isElement()) {
            QDomElement c = cn.toElement();
            processSVGTag(c,group);
        }
    }
}
