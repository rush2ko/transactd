﻿/*=================================================================
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
#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG
#endif // _MSC_VER
#include <bzs/db/protocol/tdap/client/activeTable.h>
#include <bzs/rtl/benchmark.h>
#include <bzs/example/queryData.h>
#include <iostream>
#include <locale.h>

using namespace bzs::db::protocol::tdap::client;
using namespace bzs::db::protocol::tdap;

void showConsole(recordset& rowset)
{
    const fielddefs& fields = *rowset.fieldDefs();
    for (int j = 0; j < (int)fields.size(); ++j)
        std::tcout << fields[j].name() << _T("\t");
    std::tcout << _T("\n");

    for (int i = 0; i < (int)rowset.size(); ++i)
    {
        row& m = rowset[i];
        for (int j = 0; j < (int)m.size(); ++j)
        {
            std::tcout << m[(short)j].c_str() << _T("\t");
            if (j == (int)m.size() - 1)
                std::tcout << _T("\n");
        }
    }
}

bool btest(recordset* rsp, activeTable* atup, activeTable* atgp,
           activeTable* atep, int kind, int n)
{
#ifdef LINUX
    const char* fd_name = "名前";
#else
#ifdef _UNICODE
    const wchar_t* fd_name = L"名前";
#else
    char fd_name[30];
    WideCharToMultiByte(CP_UTF8, 0, L"名前", -1, fd_name, 30, NULL, NULL);
#endif
#endif

    activeTable& atu = *atup;
    activeTable& atg = *atgp;
    activeTable& ate = *atep;
    recordset& rs = *rsp;
    query q;
    q.select(_T("id"), _T("name"), _T("group"))
                .where(_T("id"), _T("<="), _T("?"));
    atu.alias(fd_name, _T("name"));
    pq_handle stmt = atu.prepare(q, true);
    if (!stmt) return false;

    q.reset().select(_T("comment")).optimize(queryBase::joinHasOneOrHasMany);
    pq_handle stmt2 = ate.prepare(q, true);
    if (!stmt2) return false;

    atg.alias(_T("name"), _T("group_name"));
    q.reset().select(_T("group_name"));
    pq_handle stmt3 = atg.prepare(q, true);            
    if (!stmt3) return false;

    for (int i = 0; i < n; ++i)
    {
        rs.clear();
        if (kind & 1)
        {
            supplyValue(stmt, 0, i + 15000);
            atu.index(0).keyValue(i + 1).read(rs, stmt);
            // Join extention::comment
            if (kind & 2)
            {
                ate.index(0).join(rs, stmt2, _T("id"));
            }
            if (kind & 4)
            {
                // Join group::name
                atg.index(0).join(rs, stmt3, _T("group"));
            }
        }
        if (kind & 8)
            std::tcout << "." << std::flush;
    }
    if (kind & 8)
        std::tcout << std::endl;
    return true;
}

#pragma warning(disable : 4101)
#pragma argsused
int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _UNICODE
    std::locale::global(std::locale(""));
#endif
    database_ptr db = createDatabaseObject();
    try
    {
        {
        const _TCHAR* host = _T("localhost");
        int kind = 15;
        int n = 100;
        bool makeDatabase = true;

        if (argc >= 5)
            n = _ttol(argv[4]);
        if (argc >= 4)
            kind = _ttol(argv[3]);
        if (argc >= 3)
            host = argv[2];
        if (argc >= 2)
            makeDatabase = (_ttol(argv[1]) != 0);
#ifndef USE_PSQL_DATABASE
        connectParams param(_T("tdap"), host, _T("querytest"), _T("test"));
#else
        connectParams param(_T("btrv"), host, _T("querytest"), _T("test"));
#endif
        param.setMode(TD_OPEN_NORMAL);
        if (prebuiltData(db, param, makeDatabase))
        {
            std::tcout << "The query data build error." << std::endl;
            return 1;
        }

        activeTable atu(db, _T("user"));
        activeTable atg(db, _T("groups"));
        activeTable ate(db, _T("extention"));

        recordset rs;

        bzs::rtl::benchmark bm;
        bm.report(boost::bind(btest, &rs, &atu, &atg, &ate, kind, n),
                  "exec time ");
        }
        db.reset();
#ifdef _MSC_VER
        _CrtDumpMemoryLeaks();
#endif
        return 0;
    }

    catch (bzs::rtl::exception& e)
    {
        std::tcout << *bzs::rtl::getMsg(e) << std::endl;
    }
    return 1;
}
#pragma warning(default : 4101)
