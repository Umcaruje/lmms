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

#include "export.h"
#include "PixmapButton.h"

class QMoveEvent;
class QResizeEvent;
class QWidget;


class EXPORT SubWindow : public QMdiSubWindow
{
	Q_OBJECT
    Q_PROPERTY(QColor activeDecorationColor1 READ activeDecorationColor1 WRITE setActiveDecorationColor1)
    Q_PROPERTY(QColor activeDecorationColor2 READ activeDecorationColor2 WRITE setActiveDecorationColor2)
    Q_PROPERTY(QColor inactiveDecorationColor1 READ inactiveDecorationColor1 WRITE setInactiveDecorationColor1)
    Q_PROPERTY(QColor inactiveDecorationColor2 READ inactiveDecorationColor2 WRITE setInactiveDecorationColor2)
	Q_PROPERTY(QColor leftUpperBorderColor READ leftUpperBorderColor WRITE setLeftUpperBorderColor)
    Q_PROPERTY(QColor titleTextColor READ titleTextColor WRITE setTitleTextColor)
    Q_PROPERTY(QColor titleTextShadowColor READ titleTextShadowColor WRITE setTitleTextShadowColor)
	Q_PROPERTY(QColor windowBorderColor READ windowBorderColor WRITE setWindowBorderColor)
	Q_PROPERTY(QColor windowDecorationBorderColor READ windowDecorationBorderColor WRITE setWindowDecorationBorderColor)

public:
	SubWindow(QWidget *parent = NULL, Qt::WindowFlags windowFlags = 0);
	// same as QWidet::normalGeometry, but works properly under X11 (see https://bugreports.qt.io/browse/QTBUG-256)
	QRect getTrueNormalGeometry() const;
    QColor activeDecorationColor1() const;
    QColor activeDecorationColor2() const;
    QColor inactiveDecorationColor1() const;
    QColor inactiveDecorationColor2() const;
	QColor leftUpperBorderColor() const;
    QColor titleTextColor() const;
    QColor titleTextShadowColor() const;
	QColor windowBorderColor() const;
	QColor windowDecorationBorderColor() const;
    void setActiveDecorationColor1(const QColor& c);
    void setActiveDecorationColor2(const QColor& c);
    void setInactiveDecorationColor1(const QColor &c);
    void setInactiveDecorationColor2(const QColor &c);
	void setLeftUpperBorderColor(const QColor &c);
    void setTitleTextColor(const QColor &c);
    void setTitleTextShadowColor(const QColor &c);
	void setWindowBorderColor(const QColor &c);
	void setWindowDecorationBorderColor(const QColor &c);

protected:
	// hook the QWidget move/resize events to update the tracked geometry
	virtual void moveEvent(QMoveEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent *);
private:
	bool m_closeHover;
    QColor m_activeDecorationColor1;
    QColor m_activeDecorationColor2;
    QColor m_inactivDecorationColor1;
    QColor m_inactivDecorationColor2;
	QColor m_leftUpperBorderColor;
    QColor m_titleTextColor;
    QColor m_titleTextShadowColor;
	QColor m_windowBorderColor;
	QColor m_windowDecorationBorderColor;
	QPoint m_position;
	QRect m_trackedNormalGeom;
};

#endif
