#ifndef BZS_DB_PROTOCOL_TDAP_CLIENT_NSDATABASE_H
#define BZS_DB_PROTOCOL_TDAP_CLIENT_NSDATABASE_H
/* =================================================================
 Copyright (C) 2000-2013 BizStation Corp All rights reserved.

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
 ================================================================= */

#include <bzs/db/protocol/tdap/tdapSchema.h>
#include "export.h"
#ifdef _WIN32
#include <windows.h>
#endif

#if (__BCPLUSPLUS__ || _MSC_VER)
#ifdef TRDCL_AUTOLINK
#include "trdclcppautolink.h"
#endif
#endif

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

class dbdef;
class nstable;

/** @cond INTERNAL */
DLLLIB void setBtrvEntryPoint(BTRCALLID_PTR p);
DLLLIB BTRCALLID_PTR getBtrvEntryPoint();
DLLLIB void setTrnsctdEntryPoint(BTRCALLID_PTR p);
DLLLIB BTRCALLID_PTR getTrnsctdEntryPoint();
/** @endcond */

class DLLLIB nsdatabase
{
    friend class nstable;
    friend class dbdef;
/** @cond INTERNAL */
    friend bool reconnectSharedConnection(const void* ptr);
/** @endcond */
    struct nsdbimpl* m_nsimpl;
    nsdatabase(const nsdatabase&);
    static unsigned int m_execCodepage;
    bool doReopenTables();
protected:
    BTRCALLID_PTR m_btrcallid;
    short m_stat;

    static const char* toServerUri(char* buf, int buflen, const _TCHAR* src,
                                   bool trd);
    virtual bool setUri(const _TCHAR* uri);
    void reset();
    nstable** tables();
    nsdatabase* clone() const;
    nsdatabase& operator=(const nsdatabase&);
    virtual ~nsdatabase();
    void unregisterTable(nstable* tb);
    void registerTable(nstable* tb);
    bool findTable(nstable* tb);
    void addref();
    void internalRelease() { nsdatabase::release(); }
    void doReconnect(nstable* tb);
    virtual bool doReopenDatabaseSchema(){ return true; }
    virtual void* getExtendBufferForOpen(uint_td& size){ return NULL; };
public:
    nsdatabase();
    virtual void release();
    int refCount() const;

    inline BTRCALLID_PTR btrvFunc() { return m_btrcallid; }
    int enableTrn() const;
    short stat() const;
    uchar_td* clientID() const;
    short openTableCount() const;
    _TCHAR* uri() const;
    bool uriMode() const;
    short lockWaitCount() const;
    short lockWaitTime() const;
    bool localSharing() const;

    void setLockWaitCount(short v);
    void setLockWaitTime(short v);
    void setLocalSharing(bool v);

    void createTable(fileSpec* pfs, uint_td len, const _TCHAR* uri,
                     short_td mode = -1);
    virtual void dropTable(const _TCHAR* uri);
    void rename(const _TCHAR* oldUri, const _TCHAR* newUri);
    void swapTablename(const _TCHAR* uri1, const _TCHAR* uri2);
    void beginTrn(short bias = SINGLELOCK_READ_COMMITED +
                               NOWAIT_WRITE); // NoWit SingleLock
    void endTrn();
    void abortTrn();
    void beginSnapshot(short bias = CONSISTENT_READ);
    void endSnapshot();
    ushort_td trxIsolationServer() const ;
    ushort_td trxLockWaitTimeoutServer() const ;
    short_td tdapErr(HWND hWnd, _TCHAR* retbuf = NULL);
    inline _TCHAR* statMsg(_TCHAR* retbuf)
    {
        tdapErr(0, retbuf);
        return retbuf;
    }
    bool useLongFilename();
    void setUseLongFilename(bool value);
    void getBtrVersion(btrVersions* versions, uchar_td* posblk);
    bool setUseTransactd();
    bool isTransactdUri(const _TCHAR* uri);
    bool isUseTransactd() const;
    void readDatabaseDirectory(_TCHAR* retbuf, uchar_td len);
    bool connect(const _TCHAR* uri, bool newConnection = false);
    bool disconnect(const _TCHAR* uri = _T(""));
    bool disconnectForReconnectTest(); //for connection brokn emulate
	bool reconnect();
    bool isReconnected() const;

    static const int maxtables = 50;
    static bool trnsactionFlushWaitStatus();
    static void setExecCodePage(unsigned int codepage);
    static unsigned int execCodePage();
    /** @cond INTERNAL */
    void setTestPtrIgnore(bool v);
    bool isTestPtrIgnore() const;
    static WIN_TPOOL_SHUTDOWN_PTR getWinTPoolShutdownFunc();
    static bool testTablePtr(nstable* ptr);
    static void setCheckTablePtr(bool v);
    /** @endcond */
};

} // namespace client
} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs

#endif // BZS_DB_PROTOCOL_TDAP_CLIENT_NSDATABASE_H
