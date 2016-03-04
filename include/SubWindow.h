/*
 * SubWindow.h - Implementation of QMdiSubWindow that correctly tracks
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
#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QEvent>
#include <QMdiSubWindow>
#include <QToolBar>

#include "export.h"
#include "PixmapButton.h"

class QMoveEvent;
class QResizeEvent;
class QWidget;


class EXPORT SubWindow : public QMdiSubWindow
{
	Q_OBJECT
    Q_PROPERTY(QBrush activeColor READ activeColor WRITE setActiveColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor textShadowColor READ textShadowColor WRITE setTextShadowColor)
		Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

public:
	SubWindow(QWidget *parent = NULL, Qt::WindowFlags windowFlags = 0);
	// same as QWidet::normalGeometry, but works properly under X11 (see https://bugreports.qt.io/browse/QTBUG-256)
	QRect getTrueNormalGeometry() const;
	QBrush activeColor() const;
	QColor textColor() const;
	QColor textShadowColor() const;
	QColor borderColor() const;
	void setActiveColor(const QBrush & b);
	void setTextColor(const QColor &c);
	void setTextShadowColor(const QColor &c);
	void setBorderColor(const QColor &c);

protected:
	// hook the QWidget move/resize events to update the tracked geometry
	virtual void moveEvent( QMoveEvent * event );
	virtual void resizeEvent( QResizeEvent * event );
	virtual void paintEvent( QPaintEvent * pe );
	
private:
	QToolBar * m_toolbar;
	bool m_closeHover;
	QBrush m_activeColor;
  QColor m_textColor;
	QColor m_textShadowColor;
	QColor m_borderColor;
	QPoint m_position;
	QRect m_trackedNormalGeom;
};

#endif
