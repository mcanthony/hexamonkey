//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef HEXFILEDOCKWIDGET_H
#define HEXFILEDOCKWIDGET_H

#include <QDockWidget>
#include "hexfilewidget.h"

/*!
 * \brief The HexFileDockWidget class
 */
class HexFileDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit HexFileDockWidget(QWidget *parent = 0);
    void setWidget(HexFileWidget *widget);

    HexFileWidget* widget;

signals:
    void focusedIn();
    void focusedOut();
    
public slots:

protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void windowActivationChange(bool oldActive);
    
};

#endif // HEXFILEDOCKWIDGET_H
