#ifndef BZS_DB_PROTOCOL_TDAP_CLIENT_TABLE_H
#define BZS_DB_PROTOCOL_TDAP_CLIENT_TABLE_H
/* =================================================================
 Copyright (C) 2000-2016 BizStation Corp All rights reserved.

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
#include "nsTable.h"
#include <vector>
#include <stdio.h>
#include <boost/shared_ptr.hpp>

namespace bzs
{

namespace db
{
struct blobHeader;
struct blob;
namespace protocol
{
namespace tdap
{
namespace client
{

/** @cond INTERNAL */
class database;
class queryBase;
class fields;
class table;
/** @endcond */

#if (defined(__BORLANDC__) && !defined(__APPLE__) && !defined(__clang__))
typedef void __stdcall(* recordCountFn)(table* tb, int count, bool& complate);
#else
/** @cond INTERNAL */
/** Callback function on a record was record count. */
typedef void(__STDCALL* recordCountFn)(table* tb, int count, bool& complate);
#endif

#define null_str _T("")

#pragma warning(disable : 4251)

static const int mra_nojoin = 0;
static const int mra_first = 0;
static const int mra_nextrows = 1;
static const int mra_innerjoin = 2;
static const int mra_outerjoin = 4;
static const int mra_current_block = -1;

class multiRecordAlocator
{

public:
    virtual ~multiRecordAlocator() {}
    virtual void init(size_t recordCount, size_t recordLen, int addType,
                      const class table* tb) = 0;
    virtual unsigned char* allocBlobBlock(size_t size) = 0;
    virtual unsigned char* ptr(size_t row, int stat) = 0;
    virtual void setJoinType(int v) = 0;
    virtual void setInvalidMemblock(size_t row, bool v) = 0;
    virtual void setJoinRowMap(
        const std::vector<std::vector<int> >* v /*, size_t size*/) = 0;
    virtual const std::vector<std::vector<int> >* joinRowMap() const = 0;
    virtual void duplicateRow(int row, int count) = 0;
    virtual void removeLastMemBlock(int row) = 0;
    virtual int joinType() const = 0;
};

class filter;
typedef boost::shared_ptr<filter> pq_handle; 

/** @endcond */


class DLLLIB table : public nstable
{
    static void* __STDCALL DDBA(client::table* tb, uint_td size);

    friend class recordCache;
    friend class database;
    friend class filter;
    friend class fields;
    friend class writableRecord;
    friend struct logic;

    struct tbimpl* m_impl;
    class fielddefs* m_fddefs;
    tabledef** m_tableDef;

    uchar_td charset() const;
    bool checkIndex(short index) const;
    void getKeySpec(keySpec* ks, bool SpecifyKeyNum = false);
    unsigned char* setBlobFieldPointer(char* ptr, const ::bzs::db::blobHeader* p,
                                        unsigned char* to=NULL);
    void addSendBlob(const bzs::db::blob* blob);
    void addBlobEndRow();
    void resetSendBlob();
    void btrvGetExtend(ushort_td op);
    void getRecords(ushort_td op);
    uint_td doGetWriteImageLen(); // orverride
    void doUpdate(bool ncc = false); // orverride
    ushort_td doCommitBulkInsert(bool autoCommit = false); // orverride
    void doAbortBulkInsert(); // orverride
    void doCreateIndex(bool SpecifyKeyNum = false); // orverride
    uint_td doRecordCount(bool estimate, bool fromCurrent); // orverride
    short_td doBtrvErr(HWND hWnd, _TCHAR* retbuf = NULL); // orverride
    void doFind(ushort_td op, bool notIncCurrent);
    void* getExtendBufferForOpen(uint_td& size); // orverride
    bool setSeekValueField(int row);
    void btrvSeekMulti();
    bool doSeekMultiAfter(int row);
    void* doDdba(uint_td size);
    bool isReadContinue(ushort_td& op);
    void incTabledefRefCount(tabledef* td, bool mysqlMullmode);

protected:
    explicit table(nsdatabase* pbe); // Inheritance is impossible
    virtual ~table();
    void* dataBak() const;
    void* reallocDataBuffer(uint_td v);
    int dataBufferLen() const;
    uint_td unPack(char* ptr, size_t size);
    uint_td pack(char* ptr, size_t size);

    keylen_td writeKeyData(); // orverride
    keylen_td writeKeyDataTo(uchar_td* to, int keySize);

    void writeRecordData(){};

    void onReadAfter(); // orverride
    bool onUpdateCheck(eUpdateType type); // orverride

    int onUpdateBefore() { return 0; }; // orverride

    void onUpdateAfter(int beforeResult); // orverride
    bool onDeleteCheck(bool inkey); // orverride

    int onInsertBefore() { return 0; }; // orverride

    void onInsertAfter(int beforeResult); // orverride
    bool isUniqeKey(char_td keynum); // orverride
    void init(tabledef** def, short filenum, bool regularDir, bool mysqlnull);
    void* attachBuffer(void* newPtr, bool unpack = false, size_t size = 0);
    void dettachBuffer();
    bool doPrepare();

    inline unsigned int nullBytes() const
    {
        return (m_tableDef && (*m_tableDef)->isMysqlNullMode()) ?
                          (*m_tableDef)->nullbytes() : 0;
    }

    virtual void doInit(tabledef** def, short filenum, bool regularDir, bool mysqlnull);

    virtual void onRecordCounting(size_t count, bool& cancel);

    virtual void setNoUpdateTimeStamp(bool v){};

public:
    using nstable::eFindType;
    enum eNullReset{clearNull, defaultNull};

    inline const tabledef* tableDef() const { return *m_tableDef; };
    inline const tabledef** tableDefPtr() const
    {
        return const_cast<const tabledef**>(m_tableDef);
    }

    inline bool valiableFormatType() const
    {
        return (*m_tableDef)->optionFlags.bitA || (*m_tableDef)->m_nullbytes;
    }

    inline bool blobFieldUsed() const { return (*m_tableDef)->optionFlags.bitB; }

    bool logicalToString() const;
    void setLogicalToString(bool v);
    void* optionalData() const;
    void setOptionalData(void* v);
    bool myDateTimeValueByBtrv() const;
    void insertBookmarks(unsigned int start, void* data, ushort_td count);
    int bookmarksCount() const;
    void moveBookmarks(unsigned int index);
    bookmark_td bookmarks(unsigned int index) const;
    void clearBuffer(eNullReset resetType = defaultNull);
    unsigned int getRecordHash();
    void smartUpdate();

    void setMra(multiRecordAlocator* p);
    multiRecordAlocator* mra() const;

    void find(eFindType type = findForword);
    void findFirst();
    void findLast();
    void findNext(bool notIncCurrent = true);
    void findPrev(bool notIncCurrent = true);
    short statReasonOfFind() const ;
    eFindType lastFindDirection() const;
    bookmark_td bookmarkFindCurrent() const;
    pq_handle setQuery(const queryBase* query, bool serverPrepare=false);
    pq_handle prepare(const queryBase* query, bool serverPrepare=false)
    {
        return setQuery(query, serverPrepare);
    }
    void setPrepare(const pq_handle stmt);

    void setFilter(const _TCHAR* str, ushort_td rejectCount,
                   ushort_td cacheCount, bool autoEscape = true);
    short fieldNumByName(const _TCHAR* name) const ;
    unsigned char getFVbyt(short index) const;
    short getFVsht(short index) const;
    int getFVint(short index) const;
    int getFVlng(short index) const;
    __int64 getFV64(short index) const;
    float getFVflt(short index) const;
    double getFVdbl(short index) const;
    const char* getFVAstr(short index) const;
    void* getFVbin(short index, uint_td& size) const;
    unsigned char getFVbyt(const _TCHAR* fieldName) const;
    short getFVsht(const _TCHAR* fieldName) const;
    int getFVint(const _TCHAR* fieldName) const;
    int getFVlng(const _TCHAR* fieldName) const;
    __int64 getFV64(const _TCHAR* fieldName) const;
    float getFVflt(const _TCHAR* fieldName) const;
    double getFVdbl(const _TCHAR* fieldName) const;
    const char* getFVAstr(const _TCHAR* fieldName) const;
    void* getFVbin(const _TCHAR* fieldName, uint_td& size) const;
    bool getFVNull(short index) const ;
    bool getFVNull(const _TCHAR* fieldName) const;

    inline bitset getFVbits(short index) const
    {
        return bitset(getFV64(index));
    }

    inline bitset getFVbits(const _TCHAR* fieldName) const
    {
        return bitset(getFV64(fieldName));
    }

    void setFV(short index, double data);
    void setFV(short index, float data);
    void setFV(short index, unsigned char data);
    void setFV(short index, short data);
    void setFVA(short index, const char* data);
    void setFV(short index, int data);
    void setFV(short index, const void* data, uint_td size);
    void setFV(const _TCHAR* fieldName, int data);
    void setFVA(const _TCHAR* fieldName, const char* data);
    void setFVNull(short index, bool v);
    void setFVNull(const _TCHAR* fieldName, bool v);

#ifdef _WIN32
    const wchar_t* getFVWstr(const _TCHAR* fieldName) const;
    const wchar_t* getFVWstr(short index) const;
    void setFVW(short index, const wchar_t* data);
    void setFVW(const _TCHAR* fieldName, const wchar_t* data);
#endif

#ifdef _UNICODE
    inline const wchar_t* getFVstr(short index) const { return getFVWstr(index); };
    inline const wchar_t* getFVstr(const wchar_t* fieldName) const
    {
        return getFVWstr(fieldName);
    };
#else
    inline const char* getFVstr(short index) const { return getFVAstr(index); };
    inline const char* getFVstr(const char* fieldName) const
    {
        return getFVAstr(fieldName);
    };
#endif

    inline void setFV(short index, const char* data) { setFVA(index, data); };
    inline void setFV(const _TCHAR* fieldName, const char* data)
    {
        setFVA(fieldName, data);
    };
#ifdef _WIN32
    inline void setFV(short index, const wchar_t* data)
    {
        setFVW(index, data);
    };
    inline void setFV(const _TCHAR* fieldName, const wchar_t* data)
    {
        setFVW(fieldName, data);
    };
#endif
    
    inline void setFV(short index, const bitset& bits)
    {
        setFV(index, bits.internalValue());
    }

    inline void setFV(const _TCHAR* fieldName, const bitset& bits)
    {
        setFV(fieldName, bits.internalValue());
    }
    

    void setFV(const _TCHAR* fieldName, double data);
    void setFV(const _TCHAR* fieldName, float data);
    void setFV(const _TCHAR* fieldName, unsigned char data);
    void setFV(const _TCHAR* fieldName, short data);
    void setFV(const _TCHAR* fieldName, const void* data, uint_td size);
    void setFV(short index, __int64 data);
    void setFV(const _TCHAR* fieldName, __int64 data);
    void* fieldPtr(short index) const;
    void keyValueDescription(_TCHAR* buf, int bufsize);
    short getCurProcFieldCount() const;
    short getCurProcFieldIndex(short index) const;
    void setOnRecordCount(const recordCountFn v);
    recordCountFn onRecordCount() const;
    client::fields& fields();
};

#define KEYVALUE_PTR 0
#define KEYVALUE_STR 1
#define KEYVALUE_NEED_COPY 2
#define KEYVALUE_STR_NEED_COPY 3

/** @cond INTERNAL */

struct DLLLIB keyValuePtr
{

    const void* ptr;
    ushort_td len;
    short type;
    keyValuePtr(const void* p, ushort_td l, short typeStr);
    ~keyValuePtr();
};
/** @endcond */

class DLLLIB queryBase
{
    friend class filter;
    struct impl* m_impl;

protected:
    const std::vector<std::_tstring>& getSelects() const;
    const std::vector<std::_tstring>& getWheres() const;
    const std::vector<std::_tstring>& getSeekKeyValues() const;
    const std::vector<keyValuePtr>& getSeekValuesPtr() const;

public:
    enum eOptimize
    {
        none = 0,
        joinHasOneOrHasMany = 1,
        combineCondition = 2
    };

    queryBase();
    queryBase(const queryBase& r);
    queryBase& operator=(const queryBase& r);

    virtual ~queryBase();
    void reset();
    void clearSeekKeyValues();
    void clearSelectFields();
    void addField(const _TCHAR* name);
    void addLogic(const _TCHAR* name, const _TCHAR* logic, const _TCHAR* value);
    void addLogic(const _TCHAR* combine, const _TCHAR* name,
                  const _TCHAR* logic, const _TCHAR* value);
    void addSeekKeyValue(const _TCHAR* value, bool reset = false);
    void addSeekBookmark(bookmark_td& bm, ushort_td len, bool reset = false);
    void addSeekKeyValuePtr(const void* value, ushort_td len, short typeStr,
                            bool reset = false);
    void reserveSeekKeyValueSize(size_t v);
    void reserveSeekKeyValuePtrSize(size_t v);
    queryBase& queryString(const _TCHAR* str, bool autoEscape = false);
    queryBase& reject(int v);
    queryBase& limit(int v);
    queryBase& direction(table::eFindType v);
    queryBase& all();
    queryBase& optimize(eOptimize v);
    queryBase& bookmarkAlso(bool v);
    queryBase& joinKeySize(int v);
    queryBase& stopAtLimit(bool v);
    const _TCHAR* toString() const;
    table::eFindType getDirection() const;
    int getReject() const;
    int getLimit() const;
    bool isAll() const;
    int getJoinKeySize() const;
    eOptimize getOptimize() const;
    bool isStopAtLimit() const;
    bool isBookmarkAlso() const;
    bool isSeekByBookmarks() const;
    short selectCount() const;
    const _TCHAR* getSelect(short index) const;
    short whereTokens() const;
    const _TCHAR* getWhereToken(short index) const;
    void setWhereToken(short index, const _TCHAR* v);
    void reverseAliasName(const _TCHAR* alias, const _TCHAR* src);
    void release(); // don't virtual
    static queryBase* create();
};

/** @cond INTERNAL */

inline std::_tstring lexical_cast(__int64 v)
{
    _TCHAR tmp[256];
    _i64tot_s(v, tmp, 256, 10);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(int v)
{
    _TCHAR tmp[256];
    _ltot_s(v, tmp, 256, 10);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(short v)
{
    _TCHAR tmp[256];
    _ltot_s((int)v, tmp, 256, 10);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(char v)
{
    _TCHAR tmp[256];
    _ltot_s((int)v, tmp, 256, 10);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(double v)
{
    _TCHAR tmp[256];
    _stprintf_s(tmp, 256, _T("%.*f"), 16, v);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(float v)
{
    _TCHAR tmp[256];
    _stprintf_s(tmp, 256, _T("%.*f"), 16, v);
    return std::_tstring(tmp);
}

inline std::_tstring lexical_cast(const _TCHAR* v)
{
    if (v)
        return std::_tstring(v);
    THROW_BZS_ERROR_WITH_CODEMSG(STATUS_FILTERSTRING_ERROR,
                                         _T("Invalid the value, The value is NULL."));
    return std::_tstring(_T(""));
}


class qlogic
{
    std::_tstring m_name;
    std::_tstring m_value;
    std::_tstring m_type;
    combineType m_next;

public:
    template <class T>
    qlogic(const _TCHAR* name, const _TCHAR* type, T value, combineType next)
        : m_name(name), m_type(type), m_next(next)
    {
        m_value = lexical_cast(value);
    }
};
/** @endcond */

class DLLLIB query : public queryBase
{
public:
    query() : queryBase() {}
    query(const query& r) : queryBase(r) {}

    virtual ~query() {}

    query& reset()
    {
        queryBase::reset();
        return *this;
    }

    query& select(const _TCHAR* name, const _TCHAR* name1 = NULL,
                  const _TCHAR* name2 = NULL, const _TCHAR* name3 = NULL,
                  const _TCHAR* name4 = NULL, const _TCHAR* name5 = NULL,
                  const _TCHAR* name6 = NULL, const _TCHAR* name7 = NULL,
                  const _TCHAR* name8 = NULL, const _TCHAR* name9 = NULL,
                  const _TCHAR* name10 = NULL)
    {
        if (_tcscmp(name, _T("*")) == 0)
        {
            clearSelectFields();
            return *this;
        }
        addField(name);
        if (name1)
            addField(name1);
        if (name2)
            addField(name2);
        if (name3)
            addField(name3);
        if (name4)
            addField(name4);
        if (name5)
            addField(name5);
        if (name6)
            addField(name6);
        if (name7)
            addField(name7);
        if (name8)
            addField(name8);
        if (name9)
            addField(name9);
        if (name10)
            addField(name10);
        return *this;
    }

    template <class T>
    query& where(const _TCHAR* name, const _TCHAR* qlogic, T value)
    {
        addLogic(name, qlogic, lexical_cast(value).c_str());
        return *this;
    }

    query& whereIsNull(const _TCHAR* name)
    {
        addLogic(name, _T("<==>"), _T(""));
        return *this;
    }

    query& whereIsNotNull(const _TCHAR* name)
    {
        addLogic(name, _T("<!=>"), _T(""));
        return *this;
    }

    template <class T>
    query& and_(const _TCHAR* name, const _TCHAR* qlogic, T value)
    {
        if (whereTokens() == 0)
            THROW_BZS_ERROR_WITH_CODEMSG(STATUS_FILTERSTRING_ERROR,
                                         _T("Invalid function call."));

        addLogic(_T("and"), name, qlogic, lexical_cast(value).c_str());
        return *this;
    }

    query& andIsNull(const _TCHAR* name)
    {
        addLogic(_T("and"), name, _T("<==>"), _T(""));
        return *this;
    }

    query& andIsNotNull(const _TCHAR* name)
    {
        addLogic(_T("and"), name, _T("<!=>"), _T(""));
        return *this;
    }

    template <class T>
    query& or_(const _TCHAR* name, const _TCHAR* qlogic, T value)
    {
        if (whereTokens() == 0)
            THROW_BZS_ERROR_WITH_CODEMSG(STATUS_FILTERSTRING_ERROR,
                                         _T("Invalid function call."));

        addLogic(_T("or"), name, qlogic, lexical_cast(value).c_str());
        return *this;
    }

    query& orIsNull(const _TCHAR* name)
    {
        addLogic(_T("or"), name, _T("<==>"), _T(""));
        return *this;
    }

    query& orIsNotNull(const _TCHAR* name)
    {
        addLogic(_T("or"), name, _T("<!=>"), _T(""));
        return *this;
    }


    template <class T0, class T1, class T2, class T3, class T4, class T5,
              class T6, class T7>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2, const T3 kv3,
              const T4 kv4, const T5 kv5, const T6 kv6, const T7 kv7)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        addSeekKeyValue(lexical_cast(kv3).c_str());
        addSeekKeyValue(lexical_cast(kv4).c_str());
        addSeekKeyValue(lexical_cast(kv5).c_str());
        addSeekKeyValue(lexical_cast(kv6).c_str());
        addSeekKeyValue(lexical_cast(kv7).c_str());
        return *this;
    }
    template <class T0, class T1, class T2, class T3, class T4, class T5,
              class T6>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2, const T3 kv3,
              const T4 kv4, const T5 kv5, const T6 kv6)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        addSeekKeyValue(lexical_cast(kv3).c_str());
        addSeekKeyValue(lexical_cast(kv4).c_str());
        addSeekKeyValue(lexical_cast(kv5).c_str());
        addSeekKeyValue(lexical_cast(kv6).c_str());
        return *this;
    }

    template <class T0, class T1, class T2, class T3, class T4, class T5>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2, const T3 kv3,
              const T4 kv4, const T5 kv5)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        addSeekKeyValue(lexical_cast(kv3).c_str());
        addSeekKeyValue(lexical_cast(kv4).c_str());
        addSeekKeyValue(lexical_cast(kv5).c_str());
        return *this;
    }

    template <class T0, class T1, class T2, class T3, class T4>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2, const T3 kv3,
              const T4 kv4)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        addSeekKeyValue(lexical_cast(kv3).c_str());
        addSeekKeyValue(lexical_cast(kv4).c_str());
        return *this;
    }

    template <class T0, class T1, class T2, class T3>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2, const T3 kv3)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        addSeekKeyValue(lexical_cast(kv3).c_str());
        return *this;
    }

    template <class T0, class T1, class T2>
    query& in(const T0 kv0, const T1 kv1, const T2 kv2)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        addSeekKeyValue(lexical_cast(kv2).c_str());
        return *this;
    }

    template <class T0, class T1> query& in(const T0 kv0, const T1 kv1)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        addSeekKeyValue(lexical_cast(kv1).c_str());
        return *this;
    }

    template <class T0> query& in(const T0 kv0)
    {
        addSeekKeyValue(lexical_cast(kv0).c_str());
        return *this;
    }

    inline query& segmentsForInValue(int v) { joinKeySize(v); return *this;}

    static query* create(); // implemet int activeTable.cpp
};

/** @cond INTERNAL */
int DLLLIB makeSupplyValues(/*in out*/const _TCHAR* values[], int size,
                         const _TCHAR* value, const _TCHAR* value1 = NULL,
                         const _TCHAR* value2 = NULL, const _TCHAR* value3 = NULL,
                         const _TCHAR* value4 = NULL, const _TCHAR* value5 = NULL,
                         const _TCHAR* value6 = NULL, const _TCHAR* value7 = NULL,
                         const _TCHAR* value8 = NULL, const _TCHAR* value9 = NULL,
                         const _TCHAR* value10 = NULL);
/** @endcond */

bool DLLLIB supplyValues(pq_handle& filter, const _TCHAR* values[], int size);
bool DLLLIB supplyValue(pq_handle& filter, int index, const _TCHAR* v);
bool DLLLIB supplyValue(pq_handle& filter, int index, short v);
bool DLLLIB supplyValue(pq_handle& filter, int index, int v);
bool DLLLIB supplyValue(pq_handle& filter, int index, __int64 v);
bool DLLLIB supplyValue(pq_handle& filter, int index, float v);
bool DLLLIB supplyValue(pq_handle& filter, int index, double v);


#pragma warning(default : 4251)

} // namespace client
} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs

#endif // BZS_DB_PROTOCOL_TDAP_CLIENT_TABLE_H
