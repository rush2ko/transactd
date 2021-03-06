/*=================================================================
   Copyright (C) 2014 BizStation Corp All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.
=================================================================*/
#pragma hdrstop

#include "activeTable.h"
#include "activeTableImple.h"

#pragma package(smart_init)

namespace bzs
{
namespace db
{
namespace protocol
{
namespace tdap
{
namespace client
{

query* query::create()
{
    return new query();
}

activeTable::activeTable(idatabaseManager* mgr, const _TCHAR* tableName)
    : m_imple(new activeTableImple(mgr, tableName))
{
}

activeTable::activeTable(dbmanager_ptr& mgr, const _TCHAR* tableName)
    : m_imple(new activeTableImple(mgr.get(), tableName))
{
}

activeTable::activeTable(database_ptr& db, const _TCHAR* tableName,
                                            short mode)
    : m_imple(new activeTableImple(db, tableName, mode))
{
}

activeTable::activeTable(database* db, const _TCHAR* tableName,
                                            short mode)
    : m_imple(new activeTableImple(db, tableName, mode))
{
}

activeTable::activeTable(database_ptr& db, short tableIndex,
                                            short mode)
    : m_imple(new activeTableImple(db, tableIndex, mode))
{
}

activeTable::activeTable(database* db, short tableIndex,
                                            short mode)
    : m_imple(new activeTableImple(db, tableIndex, mode))
{
}

activeTable::~activeTable()
{
    delete m_imple;
}

table_ptr activeTable::table() const
{
    return m_imple->table();
}

activeTable& activeTable::alias(const _TCHAR* src, const _TCHAR* dst)
{
    m_imple->alias(src, dst);
    return *this;
}

activeTable& activeTable::resetAlias()
{
    m_imple->resetAlias();
    return *this;
}

writableRecord& activeTable::getWritableRecord()
{
    return m_imple->getWritableRecord();
}

recordset& activeTable::join(recordset& rs, queryBase& q, const _TCHAR* name1,
                               const _TCHAR* name2, const _TCHAR* name3,
                               const _TCHAR* name4, const _TCHAR* name5,
                               const _TCHAR* name6, const _TCHAR* name7,
                               const _TCHAR* name8)
{
    m_imple->join(*rs.m_imple, q, name1, name2, name3, name4, name5, name6,
                  name7, name8);
    return rs;
}

recordset& activeTable::outerJoin(recordset& rs, queryBase& q,
                                    const _TCHAR* name1, const _TCHAR* name2,
                                    const _TCHAR* name3, const _TCHAR* name4,
                                    const _TCHAR* name5, const _TCHAR* name6,
                                    const _TCHAR* name7, const _TCHAR* name8)
{
    m_imple->outerJoin(*rs.m_imple, q, name1, name2, name3, name4, name5, name6,
                       name7, name8);
    return rs;
}

recordset& activeTable::join(recordset& rs, pq_handle& q, const _TCHAR* name1,
                               const _TCHAR* name2, const _TCHAR* name3,
                               const _TCHAR* name4, const _TCHAR* name5,
                               const _TCHAR* name6, const _TCHAR* name7,
                               const _TCHAR* name8)
{
    m_imple->join(*rs.m_imple, q, name1, name2, name3, name4, name5, name6,
                  name7, name8);
    return rs;
}

recordset& activeTable::outerJoin(recordset& rs, pq_handle& q,
                                    const _TCHAR* name1, const _TCHAR* name2,
                                    const _TCHAR* name3, const _TCHAR* name4,
                                    const _TCHAR* name5, const _TCHAR* name6,
                                    const _TCHAR* name7, const _TCHAR* name8)
{
    m_imple->outerJoin(*rs.m_imple, q, name1, name2, name3, name4, name5, name6,
                       name7, name8);
    return rs;
}

activeTable& activeTable::index(int v)
{
    m_imple->index(v);
    return *this;
}

activeTable& activeTable::option(int v)
{
    m_imple->option(v);
    return *this;
}

pq_handle activeTable::prepare(queryBase& q, bool serverPrepare)
{
    return m_imple->prepare(q, serverPrepare);
}

recordset& activeTable::readMore(recordset& rs)
{
    rs.clear();
    m_imple->readMore(*rs.m_imple);
    return rs;
}

recordset& activeTable::read(recordset& rs, queryBase& q)
{
    rs.clear();
    m_imple->read(*rs.m_imple, q);
    return rs;
}

recordset& activeTable::read(recordset& rs, queryBase& q, validationFunc func)
{
    rs.clear();
    m_imple->read(*rs.m_imple, q, func);
    return rs;
}

recordset& activeTable::read(recordset& rs, pq_handle& q)
{
    rs.clear();
    m_imple->read(*rs.m_imple, q);
    return rs;
}

recordset& activeTable::read(recordset& rs, pq_handle& q, validationFunc func)
{
    rs.clear();
    m_imple->read(*rs.m_imple, q, func);
    return rs;
}

activeTable* activeTable::create(idatabaseManager* mgr, const _TCHAR* tableName)
{
    return new activeTable(mgr, tableName);
}

activeTable* activeTable::create(dbmanager_ptr& mgr, const _TCHAR* tableName)
{
    return new activeTable(mgr, tableName);
}

activeTable* activeTable::create(database_ptr& db, const _TCHAR* tableName, short mode)
{
    return new activeTable(db, tableName, mode);
}

activeTable* activeTable::create(database* db, const _TCHAR* tableName, short mode)
{
    return new activeTable(db, tableName, mode);
}

activeTable* activeTable::create(database_ptr& db, short tableIndex, short mode)
{
    return new activeTable(db, tableIndex, mode);
}

activeTable* activeTable::create(database* db, short tableIndex, short mode)
{
    return new activeTable(db, tableIndex, mode);
}

void activeTable::release()
{
    delete this;
}

void activeTable::releaseTable()
{
    m_imple->releaseTable();
}

} // namespace client
} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs
