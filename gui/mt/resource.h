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

#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <QObject>
#include <QThread>

#include "worker.h"
#include "tasklist.h"

class ResourceManager;
class TreeModel;

class Resource : public QObject
{
    Q_OBJECT
public:
    Resource(TreeModel& model);

    void lock();

    void unlock();

    TaskList& taskList();

private:
    TaskList _taskList;
    Worker _worker;
    QThread _thread;
    QMutex _mutex;

};

#endif // RESSOURCE_H