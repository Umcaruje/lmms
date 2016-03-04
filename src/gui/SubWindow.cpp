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
#include <QPushButton>
#include <QWidget>

#include "embed.h"




SubWindow::SubWindow(QWidget *parent, Qt::WindowFlags windowFlags)
  : QMdiSubWindow(parent, windowFlags)
{
	// initialize the tracked geometry to whatever Qt thinks the normal geometry currently is.
	// this should always work, since QMdiSubWindows will not start as maximized
  	m_trackedNormalGeom = normalGeometry();
    m_activeColor = Qt::SolidPattern;
    m_textColor = Qt::white;
    m_textShadowColor = Qt::black;
  	m_borderColor = Qt::black;
    m_toolbar = new QToolBar( this );
    QPushButton * closeBtn = new QPushButton( embed::getIconPixmap( "close" ), QString::null, this );
    closeBtn->resize(17, 17);
    m_toolbar->addWidget(closeBtn);
    m_toolbar->resize(24,24);
    //setStyle( QApplication::style() );
    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}




void SubWindow::paintEvent(QPaintEvent *)
{
  QPainter p( this );
  const int titleBarHeight = 24;
  QRect rect( 0, 0, width() , titleBarHeight );
  bool isActive = SubWindow::mdiArea()->activeSubWindow() == this;
  
  p.fillRect(rect, isActive ? activeColor() : p.pen().brush() );

	// window border
  QPen pen;
	p.setPen( borderColor() );

  // bottom, left, and right lines
  p.drawLine( 0, height() - 1, width(), height() - 1 );
  p.drawLine( 0, titleBarHeight, 0, height() - 1 );
  p.drawLine( width() - 1, titleBarHeight, width() - 1, height() - 1 );

	// window title
  /*if( m_windowTitle.text() != widget()->windowTitle() )
  {
    m_windowTitle.setText( widget()->windowTitle() );
  }*/
  // the width of the window buttons
  
  /*const int buttonWidth = 20;  
  int textWidth = rect.width() - ( titleBarHeight + buttonWidth );
  QFontMetrics metrics( p.font() );
  QString clippedText = metrics.elidedText(m_windowTitle.text(), Qt::ElideRight, textWidth);
  m_windowTitle.setText(clippedText);*/
  //m_windowTitle.setTextOption( Qt::AlignCenter );
  m_toolbar -> move( width()-m_toolbar->width(), 0 );
  QRect textRect(24, 0, rect.width()-24, rect.height() );
  p.setPen( textShadowColor() );
  textRect.translate(1,1);
  p.drawText( textRect, widget()->windowTitle(), QTextOption(Qt::AlignCenter) );
	p.setPen( textColor() );
  textRect.translate(-1,-1);
  p.drawText( textRect, widget()->windowTitle(), QTextOption(Qt::AlignCenter) );
  
  
  QPixmap winicon(widget()->windowIcon().pixmap(QSize(17,17)));
	if( windowFlags() & Qt::WindowMinimizeButtonHint)
	{
		int d = 0;
		if ( windowFlags() & Qt::WindowMaximizeButtonHint) { d = 18; }
		QPixmap minimizeBtn(embed::getIconPixmap("minimize"));
		p.drawPixmap(width()-17-3-1-17-d,3,17,17,minimizeBtn);
	}
	if( windowFlags() & Qt::WindowMaximizeButtonHint)
	{
		QPixmap maximizeBtn(embed::getIconPixmap("maximize"));
		p.drawPixmap(width()-17-3-1-17,3,17,17,maximizeBtn);
	}

	p.drawPixmap(3,3,17,17,winicon);
}




QRect SubWindow::getTrueNormalGeometry() const
{
	return m_trackedNormalGeom;
}




QBrush SubWindow::activeColor() const
{
    return m_activeColor;
}

QColor SubWindow::textColor() const
{
    return m_textColor;
}

QColor SubWindow::textShadowColor() const
{
    return m_textShadowColor;
}

QColor SubWindow::borderColor() const
{
	return m_borderColor;
}




void SubWindow::setActiveColor( const QBrush & b )
{
    m_activeColor = b;
}

void SubWindow::setTextColor(const QColor & c)
{
    m_textColor = c;
}

void SubWindow::setTextShadowColor(const QColor & c)
{
    m_textShadowColor = c;
}

void SubWindow::setBorderColor(const QColor &c)
{
	m_borderColor = c;
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
