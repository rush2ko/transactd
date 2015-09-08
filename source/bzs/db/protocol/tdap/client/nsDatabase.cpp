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
#include "nsDatabase.h"
#include "sharedData.h"
#include "nsTable.h"
#include "stringConverter.h"
#include <sys/stat.h>
#include <stdio.h>
#include <boost/thread/mutex.hpp>

#ifdef LINUX
#include <dlfcn.h>
#include <cstddef>
#include <bzs/env/crosscompile.h>
typedef void* HANDLE;
typedef void* HINSTANCE;
#endif
#include <bzs/db/protocol/tdap/tdapRequest.h>
#include <bzs/db/transactd/connectionRecord.h>

#pragma package(smart_init)

#ifdef __BCPLUSPLUS__
#ifndef _WIN64
#define BZS_LINK_BOOST_THREAD
#endif
#define BZS_LINK_BOOST_FILESYSTEM
#define BZS_LINK_BOOST_SYSTEM
#include <bzs/env/boost_bcb_link.h>
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
extern EnginsFunc engins;
unsigned int g_lastTrnTime = 0;
unsigned int nsdatabase::m_execCodepage = GetACP();
bool g_checkTablePtr = false;

PACKAGE void registEnginsPtr(EnginsFunc func)
{
    engins = func;
}

BTRCALLID_PTR BTRCALLIDX = NULL;
BTRCALLID_PTR MYTICALLID = NULL;

HANDLE hBtrvDLL = NULL;
HANDLE hTrsdDLL = NULL;

void setTrnsctdEntryPoint(BTRCALLID_PTR p)
{
    MYTICALLID = p;
}

BTRCALLID_PTR getTrnsctdEntryPoint()
{
    if (MYTICALLID)
        return MYTICALLID;

    if (hTrsdDLL == NULL)
        hTrsdDLL = LoadLibraryA(LIB_PREFIX TDCLC_LIBNAME);

    if (hTrsdDLL)
    {
        MYTICALLID =
            (BTRCALLID_PTR)GetProcAddress((HINSTANCE)hTrsdDLL, "BTRCALLID");
    }

    return MYTICALLID;
}

int smartLoadLibrary()
{
    int ret = 0;
    if (hBtrvDLL == NULL)
        hBtrvDLL = LoadLibrary(_T("W3Btrv7"));

    if (hBtrvDLL == NULL)
        hBtrvDLL = LoadLibrary(_T("WBtrv32"));

    if (hBtrvDLL)
        BTRCALLIDX =
            (BTRCALLID_PTR)GetProcAddress((HINSTANCE)hBtrvDLL, "BTRCALLID");
    if (BTRCALLIDX)
        ret = 1;
    MYTICALLID = getTrnsctdEntryPoint();
    if (MYTICALLID)
        ret = 2;
    return ret;
}

void smartFreeLibrary()
{
    if (hBtrvDLL)
        FreeLibrary((HINSTANCE)hBtrvDLL);

    if (hTrsdDLL)
        FreeLibrary((HINSTANCE)hTrsdDLL);
    hBtrvDLL = NULL;
    hTrsdDLL = NULL;
    BTRCALLIDX = NULL;
    MYTICALLID = NULL;
}

void setBtrvEntryPoint(BTRCALLID_PTR p)
{
    BTRCALLIDX = p;
}

BTRCALLID_PTR getBtrvEntryPoint()
{
    if (hBtrvDLL == NULL)
        smartLoadLibrary();
    return BTRCALLIDX;
}

struct nsdbimpl
{
    int refCount;
    int tranCount;
    unsigned short id;
    short snapShotCount;
    nstable* tables[nsdatabase::maxtables];
    uchar_td cidPtr[16];
    uchar_td* cid() { return cidPtr; }
    _TCHAR bdfPath[MAX_PATH];
    short tableCount;
    short lockWaitCount;
    short lockWaitTime;
    bool uriMode;
    bool uselongFilename;
    bool localSharing;
    bool ignoreTestPtr;
    bool reconnected;
    nsdbimpl()
        : refCount(1), tranCount(0), id(0), snapShotCount(0), tableCount(0),
          lockWaitCount(10), lockWaitTime(100), uriMode(false),
          uselongFilename(false), localSharing(false), ignoreTestPtr(false),
          reconnected(false)
    {
    }

    void setId(unsigned short id_)
    {
        id = id_;

        // make client id
        memset(cidPtr, 0, 12);
        cidPtr[12] = 'G';
        cidPtr[13] = 'X';
        memcpy(&cidPtr[14], &id, 2);
        bdfPath[0] = 0x00;
    }

    nsdbimpl& operator=(const nsdbimpl& rt)
    {
        if (&rt != this)
        {
            lockWaitCount = rt.lockWaitCount;
            lockWaitTime = rt.lockWaitCount;
            uselongFilename = rt.uselongFilename;
            uriMode = rt.uriMode;
        }
        return *this;
    }
};

boost::mutex g_mutex;
static int g_maxEnginIndex = -1;

nsdatabase::nsdatabase() : m_stat(0)
{

    int type = 0;
    if (hBtrvDLL == 0x00)
        type = smartLoadLibrary();

    m_btrcallid = getBtrvEntryPoint();
    if (m_btrcallid == NULL)
        m_btrcallid = getTrnsctdEntryPoint();
    if (!m_btrcallid)
        nstable::throwError(_T("Can't load C Interface library"),
                            ERROR_LOAD_CLIBRARY);

    m_nsimpl = new nsdbimpl();
    if ((type == 2) || MYTICALLID)
        setUseLongFilename(true);
    else
    {
        btrVersions v;
        memset(&v, 0, sizeof(btrVersions));
        uchar_td posblk[POS_BLOCK_SIZE] = { 0x00 };
        getBtrVersion(&v, posblk);
        if ((v.versions[1].majorVersion >= 9) ||
            (v.versions[0].majorVersion >= 9))
            setUseLongFilename(true);
    }

    for (int i = 0; i < maxtables; i++)
        m_nsimpl->tables[i] = NULL;

    boost::mutex::scoped_lock lck(g_mutex);
    // serach empty
    int index;
    for (index = 0; index < MAX_BTRENGIN; index++)
        if (engins()[index] == NULL)
            break;

    // no empty
    if (MAX_BTRENGIN == index)
    {
        m_stat = -1;
        return;
    }
    engins()[index] = this;
    g_maxEnginIndex = std::max<int>(index, g_maxEnginIndex);
    m_nsimpl->setId((unsigned short)index + 1);
}

int nsdatabase::refCount() const
{
    return m_nsimpl->refCount;
}

void nsdatabase::addref()
{
    ++m_nsimpl->refCount;
}

void nsdatabase::release()
{
    if (--m_nsimpl->refCount == 0)
        delete this;
}

nsdatabase::~nsdatabase()
{
    reset();

    boost::mutex::scoped_lock lck(g_mutex);
    if (m_nsimpl->id != 0)
        engins()[m_nsimpl->id - 1] = NULL;
    delete m_nsimpl;
    m_nsimpl = 0x00;
#ifdef _WIN32
    OutputDebugString(_T("delete database\n"));
#endif
}

nsdatabase* nsdatabase::clone() const
{
    nsdatabase* p = new nsdatabase();
    *p = *this;
    return p;
}

nsdatabase& nsdatabase::operator=(const nsdatabase& rt)
{
    if (&rt != this)
    {
        *m_nsimpl = *rt.m_nsimpl;
        setUri(rt.uri());
        m_btrcallid = rt.m_btrcallid;
    }
    return *this;
}

int nsdatabase::enableTrn() const
{
    if (m_nsimpl)
        return m_nsimpl->tranCount;
    return 0;
}

bool nsdatabase::isReconnected() const
{
    return m_nsimpl->reconnected;
}

short nsdatabase::stat() const
{
    return m_stat;
}

uchar_td* nsdatabase::clientID() const
{
    return m_nsimpl->cidPtr;
}

short nsdatabase::openTableCount() const
{
    return m_nsimpl->tableCount;
}

_TCHAR* nsdatabase::uri() const
{
    return m_nsimpl->bdfPath;
}

bool nsdatabase::uriMode() const
{
    return m_nsimpl->uriMode;
}

nstable** nsdatabase::tables()
{
    return m_nsimpl->tables;
}

short nsdatabase::lockWaitCount() const
{
    return m_nsimpl->lockWaitCount;
}

void nsdatabase::setLockWaitCount(short v)
{
    m_nsimpl->lockWaitCount = v;
}

short nsdatabase::lockWaitTime() const
{
    return m_nsimpl->lockWaitTime;
}

void nsdatabase::setLockWaitTime(short v)
{
    m_nsimpl->lockWaitTime = v;
}

bool nsdatabase::localSharing() const
{
    return m_nsimpl->localSharing;
}

void nsdatabase::setLocalSharing(bool v)
{
    m_nsimpl->localSharing = v;
}

bool nsdatabase::setUri(const _TCHAR* Path)
{
#ifdef _WIN32
    _TCHAR buf[MAX_PATH];
    _TCHAR* lpFilePart;
    if (useLongFilename() == false)
    {
        GetFullPathName(Path, MAX_PATH, buf, &lpFilePart);
        GetShortPathName(buf, m_nsimpl->bdfPath, MAX_PATH);
        _tcsmupr((_TUCHAR*)m_nsimpl->bdfPath);
    }
    else
#endif
        _tcscpy(m_nsimpl->bdfPath, Path);

    m_nsimpl->uriMode = false;
    if (_tcsstr(m_nsimpl->bdfPath, _T("btrv://")) ||
        _tcsstr(m_nsimpl->bdfPath, _T("tdap://")))
        m_nsimpl->uriMode = true;
#ifdef _WIN32
    else
    {
        struct _stat statbuf;
        if (_tstat(m_nsimpl->bdfPath, &statbuf) == -1)
            return false;
    }
#endif
    return true;
}

void nsdatabase::createTable(fileSpec* pfs, uint_td len,
                             const _TCHAR* pFullPath, short_td mode)
{
    _TCHAR buf[MAX_PATH];
    _TCHAR posblk[128] = { 0x00 };
#ifdef _WIN32
    if ((useLongFilename() == false) && _tcsstr(pFullPath, _T(" ")))
    {
        GetShortPathName(pFullPath, buf, MAX_PATH);
    }
    else
#endif
    {
        _tcscpy(buf, pFullPath);
    }
    // tdap
    if (isTransactdUri(buf))
    {
        if (setUseTransactd() == false)
            return;
    }

    char buf2[MAX_PATH] = { 0x00 };
    ;
    const char* p = toServerUri(buf2, MAX_PATH, buf, isUseTransactd());

    m_stat =
        m_btrcallid(TD_CREATETABLE, posblk, pfs, &len, (void*)p,
                    (uchar_td)strlen(p), (char_td)mode, m_nsimpl->cidPtr);
}

void nsdatabase::dropTable(const _TCHAR* pFullPath)
{
    _TCHAR buf[MAX_PATH];
    _TCHAR posblk[128] = { 0x00 };
#ifdef _WIN32
    if ((useLongFilename() == false) && _tcsstr(pFullPath, _T(" ")))
    {
        GetShortPathName(pFullPath, buf, MAX_PATH);
    }
    else
#endif
    {
        _tcscpy(buf, pFullPath);
    }
    // tdap
    if (isTransactdUri(buf))
    {
        if (setUseTransactd() == false)
            return;
    }

    char buf2[MAX_PATH] = { 0x00 };
    const char* p = toServerUri(buf2, MAX_PATH, buf, isUseTransactd());

    m_stat =
        m_btrcallid(TD_CREATETABLE, posblk, NULL, NULL, (void*)p,
                    (uchar_td)strlen(p) + 1, CR_SUBOP_DROP, m_nsimpl->cidPtr);
}

void nsdatabase::swapTablename(const _TCHAR* Name1, const _TCHAR* Name2)
{
    _TCHAR posblk[128] = { 0x00 };

    char buf1[MAX_PATH] = { 0x00 };
    char buf2[MAX_PATH] = { 0x00 };
    const char* p = toServerUri(buf1, MAX_PATH, Name1, isUseTransactd());
    const char* p2 = toServerUri(buf2, MAX_PATH, Name2, isUseTransactd());
    uint_td len = (uint_td)strlen(p);

    m_stat = m_btrcallid(TD_CREATETABLE, posblk, (void*)p, &len, (void*)p2,
                         (uchar_td)strlen(p2), CR_SUBOP_SWAPNAME,
                         m_nsimpl->cidPtr);
}

void nsdatabase::rename(const _TCHAR* pFullPath, const _TCHAR* newName)
{
    _TCHAR buf[MAX_PATH];
    _TCHAR posblk[128] = { 0x00 };
#ifdef _WIN32
    if ((useLongFilename() == false) && _tcsstr(pFullPath, _T(" ")))
    {
        GetShortPathName(pFullPath, buf, MAX_PATH);
    }
    else
#endif
    {
        _tcscpy(buf, pFullPath);
    }
    if (isTransactdUri(buf))
    {
        if (setUseTransactd() == false)
            return;
    }

    char buf2[MAX_PATH] = { 0x00 };
    const char* p = toServerUri(buf2, MAX_PATH, buf, isUseTransactd());
    uint_td len = (uint_td)strlen(p);

    char bufNew[MAX_PATH] = { 0x00 };
#ifdef _WIN32
    if ((useLongFilename() == false) && _tcsstr(newName, _T(" ")))
        GetShortPathName(newName, buf, MAX_PATH);
    else
#endif
        _tcscpy(buf, newName);
    toServerUri(bufNew, MAX_PATH, newName, isUseTransactd());

    m_stat = m_btrcallid(TD_CREATETABLE, posblk, (void*)p, &len, (void*)bufNew,
                         (uchar_td)strlen(bufNew), CR_SUBOP_RENAME,
                         m_nsimpl->cidPtr);
}

void nsdatabase::registerTable(nstable* tb)
{
    for (int i = 0; i < maxtables; i++)
    {
        if (m_nsimpl->tables[i] == NULL)
        {
            m_nsimpl->tables[i] = tb;
            m_nsimpl->tableCount++;
            break;
        }
    }
}

void nsdatabase::unregisterTable(nstable* table)
{
    for (int i = 0; i < maxtables; i++)
    {
        if (m_nsimpl->tables[i] == table)
        {
            m_nsimpl->tables[i] = NULL;
            m_nsimpl->tableCount--;
            break;
        }
    }
}

bool nsdatabase::findTable(nstable* tb)
{
    if (m_nsimpl)
    {
        for (int i = 0; i < maxtables; i++)
        {
            if (m_nsimpl->tables[i] == tb)
                return true;
        }
    }
    return false;
}

void nsdatabase::reset()
{
    int i;

    if (m_nsimpl->tranCount)
    {
#if (defined(_WIN32) && defined(_DEBUG))
#ifdef LIB_TDCLCPP
        int ret = MessageBox(NULL, _T("Is an uncompleted transaction aborted?"),
                             NULL, 33);
#else
        int ret = 2;
#endif
#else
        int ret = 2;
#endif

        m_nsimpl->tranCount = 1;
        if (ret == 1)
            abortTrn();
        else
            endTrn();
    }

    for (i = 0; i < maxtables; i++)
    {
        if (m_nsimpl->tables[i] != NULL)
        {
            m_nsimpl->tables[i]->destroy();
            m_nsimpl->tables[i] = NULL;
        }
    }
    m_nsimpl->lockWaitCount = 10;
    m_nsimpl->lockWaitTime = 100;
    m_nsimpl->tableCount = 0;
    m_nsimpl->bdfPath[0] = 0x00;
    if (m_btrcallid)
    {
        m_stat = m_btrcallid(TD_RESET_CLIENT, NULL, NULL, NULL, NULL, 0, 0,
                             m_nsimpl->cidPtr);
        m_stat = m_btrcallid(TD_STOP_ENGINE, NULL, NULL, NULL, NULL, 0, 0,
                             m_nsimpl->cidPtr);
        if (m_stat == ERROR_TD_NOT_CONNECTED)
            m_stat = STATUS_SUCCESS;
    }
    if (getBtrvEntryPoint())
        m_btrcallid = getBtrvEntryPoint();
}

void nsdatabase::beginSnapshot(short bias)
{
    if (m_nsimpl->snapShotCount == 0)
    {
        m_stat = m_btrcallid(TD_BEGIN_SHAPSHOT + bias, NULL, NULL, NULL, NULL, 0, 0,
                             m_nsimpl->cidPtr);
#ifdef TEST_RECONNECT
        if (canRecoverNetError(m_stat))
        {
            reconnect();
            if (m_stat) return;
            m_stat = m_btrcallid(TD_BEGIN_SHAPSHOT + bias, NULL, NULL, NULL, NULL,
                                     0, 0, m_nsimpl->cidPtr);
        }
#endif
        if (m_stat == 0)
            m_nsimpl->snapShotCount++;
    }
    else
        m_nsimpl->snapShotCount++;
}

void nsdatabase::endSnapshot()
{
    m_nsimpl->snapShotCount--;
    if (m_nsimpl->snapShotCount == 0)
        m_stat = m_btrcallid(TD_END_SNAPSHOT, NULL, NULL, NULL, NULL, 0, 0,
                             m_nsimpl->cidPtr);
    if (m_nsimpl->snapShotCount < 0)
        m_nsimpl->snapShotCount = 0;
}

void nsdatabase::beginTrn(short BIAS)
{
    if (m_nsimpl->tranCount == 0)
    {
        m_stat = m_btrcallid((ushort_td)(BIAS + TD_BEGIN_TRANSACTION), NULL,
                             NULL, NULL, NULL, 0, 0, m_nsimpl->cidPtr);
        if (m_stat == 0)
            m_nsimpl->tranCount++;
    }
    else
        m_nsimpl->tranCount++;
}

void nsdatabase::endTrn()
{

    m_nsimpl->tranCount--;
    if (m_nsimpl->tranCount == 0)
    {
        m_stat = m_btrcallid(TD_END_TRANSACTION, NULL, NULL, NULL, NULL, 0, 0,
                             m_nsimpl->cidPtr);

#ifdef _WIN32
        g_lastTrnTime = GetTickCount();
#endif
    }
    if (m_nsimpl->tranCount < 0)
        m_nsimpl->tranCount = 0;
}

void nsdatabase::abortTrn()
{
    m_stat = m_btrcallid(TD_ABORT_TRANSACTION, NULL, NULL, NULL, NULL, 0, 0,
                         m_nsimpl->cidPtr);

    m_nsimpl->tranCount = 0;
#ifdef _WIN32
    g_lastTrnTime = GetTickCount();
#endif
}

ushort_td nsdatabase::trxIsolationServer() const 
{
    if (!isUseTransactd())
        return 0xFFFF;
    return *((ushort_td*)(m_nsimpl->cidPtr + sizeof(char*)));
}

ushort_td nsdatabase::trxLockWaitTimeoutServer() const
{
    if (!isUseTransactd())
        return 0xFFFF;
    return *((ushort_td*)(m_nsimpl->cidPtr + sizeof(char*) + sizeof(ushort_td)));
}


short_td nsdatabase::tdapErr(HWND hWnd, _TCHAR* retbuf)
{
    return nstable::tdapErr(hWnd, m_stat, _T("Engin"), retbuf);
}

void nsdatabase::getBtrVersion(btrVersions* Vers, uchar_td* posblk)
{

    uchar_td posblkTmp[128] = { 0x00 };
    if (posblk == NULL)
        posblk = posblkTmp;

    uint_td datalen = sizeof(btrVersions);

    m_stat = m_btrcallid(TD_VERSION, posblk, Vers, &datalen, NULL, 0, 0,
                         m_nsimpl->cidPtr);
    {
        bool remote = false;
        if (uriMode())
            remote = true;
#ifdef _WIN32
        else if (_tcsstr(m_nsimpl->bdfPath, PSEPARATOR PSEPARATOR) ==
                 m_nsimpl->bdfPath)
            remote = true;
        else
        {
            _TCHAR drive[3] = { 0x00 };
            _tcsncpy(drive, m_nsimpl->bdfPath, 2);
            if (DRIVE_REMOTE == GetDriveType(drive))
                remote = true;
        }
#endif
        if (remote)
        {
            // faile shareing
            if (datalen / 5 == 2)
            {
                Vers->versions[2] = Vers->versions[1];
                Vers->versions[2].type = 'F';
            }
        }
        else
            memset(&Vers->versions[2], 0, sizeof(btrVersion));
    }
}

bool nsdatabase::useLongFilename()
{
    return m_nsimpl->uselongFilename;
}

void nsdatabase::setUseLongFilename(bool value)
{
    m_nsimpl->uselongFilename = value;
}

bool nsdatabase::setUseTransactd()
{
    m_btrcallid = getTrnsctdEntryPoint();
    if (m_btrcallid == NULL)
        m_stat = STATUS_REQUESTER_DEACTIVE; // can not load db engin;
    else
    {
        m_nsimpl->uriMode = true;
        setLockWaitCount(0);
        setLockWaitTime(0);
    }
    return (m_btrcallid != NULL);
}

bool nsdatabase::isTransactdUri(const _TCHAR* uri)
{
    return (_tcsstr(uri, _T("tdap://")) != NULL);
}

bool nsdatabase::isUseTransactd() const
{
    return (m_btrcallid == getTrnsctdEntryPoint());
}

void nsdatabase::readDatabaseDirectory(_TCHAR* retbuf, uchar_td buflen)
{
    // keynum is drive name A=1 B=2 C=3 0=default
    char tmp[128];
    m_stat = m_btrcallid(TD_GETDIRECTORY, NULL, NULL, NULL, tmp, 128, 0,
                         m_nsimpl->cidPtr);
    toTCharCopy(retbuf, tmp, buflen);
}

bool nsdatabase::connect(const _TCHAR* URI, bool newConnection)
{
    if (isTransactdUri(URI))
    {
        if (!setUseTransactd())
            return false;
    }else
    {
        m_stat = 0;
        if (_tcsstr(URI, _T("://")) == NULL)
            return true;

    }
    uint_td datalen = 0;

    char uri_a[MAX_PATH] = { 0x00 };
    const char* p = toServerUri(uri_a, MAX_PATH, URI, isUseTransactd());
    char_td keyNum = (isUseTransactd() == false) ? 0 : newConnection ? 3 : 0;
    m_stat = m_btrcallid(TD_CONNECT, NULL, NULL, &datalen, (void*)p,
                         (keylen_td)(strlen(p) + 1), keyNum, clientID());
    if (m_stat)
        return false;
    return true;
}

bool nsdatabase::disconnect(const _TCHAR* URI)
{
    if (!URI || (URI[0] == 0x00) || isTransactdUri(URI))
        if (!setUseTransactd())
            return false;
    uint_td datalen = 0;

    //Transactd not use uri.
    m_stat = m_btrcallid(TD_CONNECT, NULL, NULL, &datalen, (void*)URI,
                         (keylen_td)(_tcslen(URI) + 1), LG_SUBOP_DISCONNECT,
                         clientID());
    if (m_stat)
        return false;
    return true;
}


bool nsdatabase::disconnectForReconnectTest()
{
    //Transactd only
    if (!isUseTransactd())
        return false;

    uint_td datalen = 0;
    m_stat = m_btrcallid(TD_CONNECT, NULL, NULL, &datalen, NULL,
                         0, LG_SUBOP_DISCONNECT_EX,
                         clientID());
    if (m_stat)
        return false;
    return true;
}

/* TD_RECONNECT data buffer structure

   1 byte keynum
   1 byte bookmark size
   n byte bookmark

*/
void nsdatabase::doReconnect(nstable* tb)
{
    uint_td datalen = 0;
    char uri_a[MAX_PATH] = { 0x00 };
    tb->abortBulkInsert();
    datalen = tb->buflen();
    tdap::posblk* pb = (tdap::posblk*)tb->posblk();
    char* databuf = new char[datalen];
    databuf[0] = tb->keyNum();
    memcpy(databuf + 1, &pb->bookmarkLen, pb->bookmarkLen + 1);
    const char* p = toServerUri(uri_a, MAX_PATH, tb->uri(), true);
    short offset = (pb->lock) ? ROW_LOCK_X : 0;
    m_stat = m_btrcallid(TD_RECONNECT + offset, pb, databuf, &datalen, (void*)p,
                    (keylen_td)(strlen(p) + 1), tb->mode(), clientID());
    delete [] databuf;
}

bool nsdatabase::doReopenTables()
{
    // Indicate reconnected, that serverPrepared are invalid.
    m_nsimpl->reconnected = true;
    if (!doReopenDatabaseSchema()) return false;
    //Whole table, restore position.
    for (int i = 0 ;i <= m_nsimpl->tableCount; ++i)
    {
        nstable* tb = m_nsimpl->tables[i];
        if (tb && tb->isOpen())
        {
            doReconnect(tb);
            if (m_stat != 0) return false;
        }
    }
    return (m_stat == 0);
}

bool reconnectSharedConnection(const void* ptr)
{
    boost::mutex::scoped_lock lck(g_mutex);
    for (int i = 0; i < MAX_BTRENGIN; ++i)
    {
        if (engins()[i])
        {
            void* p = (*((void**)engins()[i]->m_nsimpl->cidPtr));
            if (p == ptr)
            {
                if (!engins()[i]->doReopenTables())
                    return false;
            }
        }
    }
    return true;
}

bool nsdatabase::reconnect()
{
    //Transactd only
    if (!isUseTransactd())
        return false;

    m_nsimpl->tranCount = 0;
    m_nsimpl->snapShotCount = 0;
    uint_td datalen = 0;
    char uri_a[MAX_PATH] = { 0x00 };
    const char* p = toServerUri(uri_a, MAX_PATH, m_nsimpl->bdfPath, true);
    m_stat = m_btrcallid(TD_CONNECT, NULL, NULL, &datalen, (void*)p,
                         (keylen_td)(strlen(p) + 1),
                         LG_SUBOP_RECONNECT, clientID());
    if (m_stat) return false;
    return reconnectSharedConnection((*(void**)m_nsimpl->cidPtr));

}

bool nsdatabase::trnsactionFlushWaitStatus()
{
    bool ret = false;
#ifdef _WIN32
    if (g_lastTrnTime)
        ret = ((GetTickCount() - g_lastTrnTime) < 8000);
    else
#endif
        g_lastTrnTime = 0;
    return ret;
}

void nsdatabase::setExecCodePage(unsigned int codepage)
{
    m_execCodepage = codepage;
}

unsigned int nsdatabase::execCodePage()
{
    return m_execCodepage;
}

const char* nsdatabase::toServerUri(char* buf, int buflen, const _TCHAR* src,
                                    bool trd)
{
#ifdef _UNICODE
    if (trd)
    {
        stringConverter cv(CP_UTF8, GetACP());
        cv.convert(buf, buflen, src, strlen_t(src));
        return buf;
    }
#endif
    return toChar(buf, src, buflen);
}

void nsdatabase::setTestPtrIgnore(bool v)
{
    m_nsimpl->ignoreTestPtr = v;
}

bool nsdatabase::isTestPtrIgnore() const
{
    return m_nsimpl->ignoreTestPtr;
}

bool nsdatabase::testTablePtr(nstable* ptr)
{
    if (g_checkTablePtr)
    {
        boost::mutex::scoped_lock lck(g_mutex);
        for (int i = 0; i <= g_maxEnginIndex; i++)
        {
            nsdatabase* db = engins()[i];
            if (db != NULL)
            {
                if (db->findTable(ptr))
                {
                    if (db->isTestPtrIgnore())
                    {
                        db->setTestPtrIgnore(false);
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}

void nsdatabase::setCheckTablePtr(bool v)
{
    g_checkTablePtr = v;
}

WIN_TPOOL_SHUTDOWN_PTR nsdatabase::getWinTPoolShutdownFunc()
{
    if (hTrsdDLL == NULL)
        hTrsdDLL = LoadLibraryA(LIB_PREFIX TDCLC_LIBNAME);
    if (hTrsdDLL)
        return (WIN_TPOOL_SHUTDOWN_PTR)GetProcAddress((HINSTANCE)hTrsdDLL,
                                                     "BeginWinThreadPoolShutdown");
    return NULL;

}

} // namespace client
} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs
