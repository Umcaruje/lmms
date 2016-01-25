/*
 * SubWindow.cpp - Implementation of QMdiSubWindow that correctly tracks
 *   the geometry that windows should be restored to.
 *   Workaround for https://bugreports.qt.io/browse/QTBUG-256
 *
 * Copyright (c) 2015 Colin Wallace <wallace.colin.a@gmail.com>
 *
 * This file is part of LMMS - http://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include "SubWindow.h"

#include <QIcon>
#include <QMdiArea>
#include <QMoveEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QWidget>
#include "embed.h"




SubWindow::SubWindow(QWidget *parent, Qt::WindowFlags windowFlags)
  : QMdiSubWindow(parent, windowFlags)
{
	// initialize the tracked geometry to whatever Qt thinks the normal geometry currently is.
	// this should always work, since QMdiSubWindows will not start as maximized
	m_trackedNormalGeom = normalGeometry();
    m_activeDecorationColor1 = QColor(77,96,148);
    m_activeDecorationColor2 = QColor(255,116,168);
    m_inactivDecorationColor1 = QColor(119,119,119);
    m_inactivDecorationColor2 = QColor(255,129,129);
	m_leftUpperBorderColor = QColor(164,164,164);
    m_titleTextColor = Qt::white;
    m_titleTextShadowColor = Qt::black;
	m_windowDecorationBorderColor = QColor(57,57,57);
	m_windowBorderColor = Qt::black;
}




void SubWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QRectF rect(0,0,width()-1,20);
    QLinearGradient winDecoGradient(rect.topLeft() ,rect.bottomLeft() );

	if ( SubWindow::mdiArea()->activeSubWindow() == this  )
	{
        winDecoGradient.setColorAt(0,m_activeDecorationColor1);
        winDecoGradient.setColorAt(1,m_activeDecorationColor2);
	}
	else
	{
        winDecoGradient.setColorAt(0,m_inactivDecorationColor1);
        winDecoGradient.setColorAt(1,m_inactivDecorationColor2);
	}

    painter.fillRect(rect,winDecoGradient);

	//window border
	QPen pen;
	pen.setColor(m_windowBorderColor);
	painter.setPen(pen);
	painter.drawRect(0,0,width()-1,height()-1);



	pen.setColor(m_windowDecorationBorderColor);
	painter.setPen(pen);
	painter.drawRect(rect);
	painter.drawLine(2,0,0,2);
	painter.drawLine(width()-3,0,width()-1,2);
	pen.setColor(m_leftUpperBorderColor);
	painter.setPen(pen);
	painter.drawLine(2,1,width()-3,1);
	painter.drawLine(1,2,1,19);



	//Text and Button
    pen.setColor(m_titleTextShadowColor);
	painter.setPen(pen);
    painter.drawText(QRect(0,0,rect.width()+1,rect.height()+1),
                     widget()->windowTitle(),QTextOption(Qt::AlignCenter));
    pen.setColor(m_titleTextColor);
	painter.setPen(pen);
    painter.drawText(rect, widget()->windowTitle(),QTextOption(Qt::AlignCenter));
	QPixmap winicon(widget()->windowIcon().pixmap(QSize(17,17)));
	QPixmap closeBtn(embed::getIconPixmap("close"));
	if(windowFlags() & Qt::WindowMinimizeButtonHint)
	{
		int d = 0;
		if (windowFlags() & Qt::WindowMaximizeButtonHint) { d = 18; }
		QPixmap minimizeBtn(embed::getIconPixmap("minimize"));
		painter.drawPixmap(width()-17-3-1-17-d,3,17,17,minimizeBtn);
	}
	if(windowFlags() & Qt::WindowMaximizeButtonHint)
	{
		QPixmap maximizeBtn(embed::getIconPixmap("maximize"));
		painter.drawPixmap(width()-17-3-1-17,3,17,17,maximizeBtn);
	}

	painter.drawPixmap(3,3,17,17,winicon);
	painter.drawPixmap(width()-17-3,3,17,17,closeBtn);
}




QRect SubWindow::getTrueNormalGeometry() const
{
	return m_trackedNormalGeom;
}




QColor SubWindow::activeDecorationColor1() const
{
    return m_activeDecorationColor1;
}




QColor SubWindow::activeDecorationColor2() const
{
    return m_activeDecorationColor2;
}




QColor SubWindow::inactiveDecorationColor1() const
{
    return m_inactivDecorationColor1;
}




QColor SubWindow::inactiveDecorationColor2() const
{
    return m_inactivDecorationColor2;
}




QColor SubWindow::leftUpperBorderColor() const
{
	return m_leftUpperBorderColor;
}




QColor SubWindow::titleTextColor() const
{
    return m_titleTextColor;
}




QColor SubWindow::titleTextShadowColor() const
{
    return m_titleTextShadowColor;
}




QColor SubWindow::windowBorderColor() const
{
	return m_windowBorderColor;
}




QColor SubWindow::windowDecorationBorderColor() const
{
	return m_windowDecorationBorderColor;
}




void SubWindow::setActiveDecorationColor1(const QColor &c)
{
    m_activeDecorationColor1 = c;
}




void SubWindow::setActiveDecorationColor2(const QColor &c)
{
    m_activeDecorationColor2 = c;
}




void SubWindow::setInactiveDecorationColor1(const QColor &c)
{
    m_inactivDecorationColor1 = c;
}




void SubWindow::setInactiveDecorationColor2(const QColor &c)
{
    m_inactivDecorationColor2 = c;
}




void SubWindow::setLeftUpperBorderColor(const QColor &c)
{
	m_leftUpperBorderColor = c;
}




void SubWindow::setTitleTextColor(const QColor &c)
{
    m_titleTextColor = c;
}




void SubWindow::setTitleTextShadowColor(const QColor &c)
{
    m_titleTextShadowColor = c;
}




void SubWindow::setWindowBorderColor(const QColor &c)
{
	m_windowBorderColor = c;
}




void SubWindow::setWindowDecorationBorderColor(const QColor &c)
{
	m_windowDecorationBorderColor = c;
}




void SubWindow::moveEvent(QMoveEvent * event)
{
	QMdiSubWindow::moveEvent(event);
	// if the window was moved and ISN'T minimized/maximized/fullscreen,
	//   then save the current position
	if(!isMaximized() && !isMinimized() && !isFullScreen())
	{
		m_trackedNormalGeom.moveTopLeft(event->pos());
	}
}




void SubWindow::resizeEvent(QResizeEvent * event)
{
	QMdiSubWindow::resizeEvent(event);
	// if the window was resized and ISN'T minimized/maximized/fullscreen,
	//   then save the current size
	if(!isMaximized() && !isMinimized() && !isFullScreen())
	{
		m_trackedNormalGeom.setSize(event->size());
	}
}

//void SubWindow::mousePressEvent(QMouseEvent *event)
//{

//}
