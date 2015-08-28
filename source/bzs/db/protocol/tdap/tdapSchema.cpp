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

#include <tchar.h>
#pragma hdrstop

#include <bzs/db/protocol/tdap/tdapSchema.h>
#include <bzs/db/protocol/tdap/mysql/characterset.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef LINUX
#include <bzs/env/mbcswchrLinux.h>
using namespace bzs::env;
#endif
#pragma package(smart_init)

#undef USETLS
#if ((defined(_WIN32) && _MSC_VER) || (__APPLE__ && !defined(__BCPLUSPLUS__)))
#ifdef __APPLE__
#include <pthread.h>
#endif
#define USETLS
#endif

#ifdef USETLS
extern tls_key g_tlsiID_SC1;
#else
wchar_t __THREAD g_nameBuf[266] = { 0 };
#endif

namespace bzs
{
namespace db
{
namespace protocol
{
namespace tdap
{

inline wchar_t* namebuf()
{
#ifdef USETLS
    wchar_t* p = (wchar_t*)tls_getspecific(g_tlsiID_SC1);
    if (p == NULL)
    {
        p = (wchar_t*)new wchar_t[256];
        tls_setspecific(g_tlsiID_SC1, p);
    }
    return p;
#else
    return g_nameBuf;
#endif
}

inline char* namebufA()
{
    return (char*)namebuf();
}

#ifdef _UNICODE

const wchar_t* fielddef::name() const
{
    return name(namebuf());
}

const wchar_t* fielddef::name(wchar_t* buf) const
{
    MultiByteToWideChar(m_schemaCodePage,
                        (m_schemaCodePage == CP_UTF8) ? 0 : MB_PRECOMPOSED,
                        m_name, -1, buf, MYSQL_FDNAME_SIZE);
    return buf;
}

const wchar_t* fielddef::chainChar() const
{
    wchar_t* p = namebuf();
    MultiByteToWideChar(m_schemaCodePage,
                        (m_schemaCodePage == CP_UTF8) ? 0 : MB_PRECOMPOSED,
                        m_chainChar, -1, p, 2);
    return p;
}

void fielddef::setName(const wchar_t* s)
{
    WideCharToMultiByte(m_schemaCodePage,
                        (m_schemaCodePage == CP_UTF8) ? 0 : WC_COMPOSITECHECK,
                        s, -1, m_name, MYSQL_FDNAME_SIZE, NULL, NULL);
}

void fielddef::setChainChar(const wchar_t* s)
{
    WideCharToMultiByte(m_schemaCodePage,
                        (m_schemaCodePage == CP_UTF8) ? 0 : WC_COMPOSITECHECK,
                        s, -1, m_chainChar, 2, NULL, NULL);
}

const wchar_t* tabledef::fileName() const
{
    wchar_t* p = namebuf();
    MultiByteToWideChar(schemaCodePage,
                        (schemaCodePage == CP_UTF8) ? 0 : MB_PRECOMPOSED,
                        m_fileName, -1, p, FILE_NAME_SIZE);
    return p;
}

const wchar_t* tabledef::tableName() const
{
    wchar_t* p = namebuf();
    MultiByteToWideChar(schemaCodePage,
                        (schemaCodePage == CP_UTF8) ? 0 : MB_PRECOMPOSED,
                        m_tableName, -1, p, TABLE_NAME_SIZE);
    return p;
}

const char* tabledef::toChar(char* buf, const wchar_t* s, int size)
{
    WideCharToMultiByte(schemaCodePage,
                        (schemaCodePage == CP_UTF8) ? 0 : WC_COMPOSITECHECK, s,
                        -1, buf, size, NULL, NULL);
    return buf;
}

void tabledef::setFileName(const wchar_t* s)
{
    WideCharToMultiByte(schemaCodePage,
                        (schemaCodePage == CP_UTF8) ? 0 : WC_COMPOSITECHECK, s,
                        -1, m_fileName, FILE_NAME_SIZE, NULL, NULL);
}

void tabledef::setTableName(const wchar_t* s)
{
    WideCharToMultiByte(schemaCodePage,
                        (schemaCodePage == CP_UTF8) ? 0 : WC_COMPOSITECHECK, s,
                        -1, m_tableName, TABLE_NAME_SIZE, NULL, NULL);
}
#else // NOT _UNICODE

const char* fielddef::name() const
{
#ifdef LINUX
    if (m_schemaCodePage != CP_UTF8)
    {
        char* p = namebufA();
        mbctou8(m_name, strlen(m_name), p, MYSQL_FDNAME_SIZE);
        return p;
    }
#endif
    return m_name;
}

const char* fielddef::name(char* buf) const
{
#ifdef LINUX
    if (m_schemaCodePage != CP_UTF8)
    {
        mbctou8(m_name, strlen(m_name), buf, MYSQL_FDNAME_SIZE);
        return buf;
    }
#endif
    return m_name;
}

const char* fielddef::chainChar() const
{
#ifdef LINUX
    if (m_schemaCodePage != CP_UTF8)
    {
        char* p = namebufA();
        mbctou8(m_chainChar, strlen(m_chainChar), p, 2);
        return p;
    }
#endif
    return m_chainChar;
}

void fielddef::setName(const char* s)
{
#ifdef LINUX
    if (m_schemaCodePage != CP_UTF8)
        u8tombc(s, strlen(s), m_name, FIELD_NAME_SIZE);
    else
#endif
        strncpy_s(m_name, FIELD_NAME_SIZE, s, sizeof(m_name) - 1);
}

void fielddef::setChainChar(const char* s)
{
#ifdef LINUX
    if (m_schemaCodePage != CP_UTF8)
        u8tombc(s, strlen(s), m_chainChar, 2);
    else
#endif
        strncpy_s(m_chainChar, 2, s, sizeof(m_chainChar) - 1);
}

const char* tabledef::fileName() const
{
#ifdef LINUX
    if (schemaCodePage != CP_UTF8)
    {
        char* p = namebufA();
        mbctou8(m_fileName, strlen(m_fileName), p, FILE_NAME_SIZE);
        return p;
    }
#endif
    return m_fileName;
}

const char* tabledef::tableName() const
{
#ifdef LINUX
    if (schemaCodePage != CP_UTF8)
    {
        char* p = namebufA();
        mbctou8(m_tableName, strlen(m_tableName), p, TABLE_NAME_SIZE);
        return p;
    }
#endif
    return m_tableName;
}

void tabledef::setFileName(const char* s)
{
#ifdef LINUX
    if (schemaCodePage != CP_UTF8)
        u8tombc(s, strlen(s), m_fileName, FILE_NAME_SIZE);
    else
#endif
        setFileNameA(s);
}

void tabledef::setTableName(const char* s)
{
#ifdef LINUX
    if (schemaCodePage != CP_UTF8)
        u8tombc(s, strlen(s), m_tableName, TABLE_NAME_SIZE);
    else
#endif
        setTableNameA(s);
}

const char* tabledef::toChar(char* buf, const char* s, int size)
{
#ifdef LINUX
    if (schemaCodePage != CP_UTF8)
    {
        char* p = namebufA();
        u8tombc(s, strlen(s), p, size);
        return p;
    }
#endif
    strncpy_s(buf, size, s, size - 1);
    return buf;
}

#endif // NOT _UNICODE



bool fielddef::isStringType() const
{
    return tdap::isStringType(type);
}

unsigned int fielddef::charNum(/* int index */) const
{
    if (type == ft_mychar)
        return (unsigned int)len / mysql::charsize(m_charsetIndex);
    else if (type == ft_mywchar)
        return (unsigned int)len / mysql::charsize(CHARSET_UTF16LE);
    else if (type == ft_myvarchar)
        return (unsigned int)(len - varLenBytes()) /
               mysql::charsize(m_charsetIndex);
    else if (type == ft_mywvarchar)
        return (unsigned int)(len - varLenBytes()) /
               mysql::charsize(CHARSET_UTF16LE);
    return len;
}

uint_td tabledef::unPack(char* ptr, size_t size) const
{
    char* pos = ptr;
    const char* end = pos + size;
    const char* max = pos + maxRecordLen;
    int movelen;
    for (int i = 0; i < fieldCount; i++)
    {
        fielddef& fd = fieldDefs[i];
        if (fd.type == ft_myfixedbinary)
        {
            int dl = *((unsigned short*)(pos));
            memmove(pos, pos + 2, dl);
            pos += fd.len - 2;
            *((unsigned short*)(pos)) = 0x00;
            ;
            pos += 2;
        }
        else
        {
            int blen = fd.varLenBytes();
            int dl = fd.len; // length
            if (blen == 1)
                dl = *((unsigned char*)(pos)) + blen;
            else if (blen == 2)
                dl = *((unsigned short*)(pos)) + blen;
            if ((movelen = fd.len - dl) != 0)
            {
                if (max < end + movelen)
                    return 0;
                char* src = pos + dl;
                memmove(pos + fd.len, src, end - src);
                memset(src, 0, movelen);
                end += movelen;
            }
            pos += fd.len;
        }
    }
    return (uint_td)(pos - ptr);
}

uint_td tabledef::pack(char* ptr, size_t size) const
{
    char* pos = ptr;
    char* end = pos + size;
    int movelen;
    for (int i = 0; i < fieldCount; i++)
    {
        fielddef& fd = fieldDefs[i];
        if (fd.type == ft_myfixedbinary)
        {
            memmove(pos + 2, pos, fd.len - 2); // move as size pace in the field
            *((unsigned short*)(pos)) = fd.len - 2; // fixed size
            pos += fd.len;
        }
        else
        {
            int blen = fd.varLenBytes();
            int dl = fd.len; // length
            if (blen == 1)
                dl = *((unsigned char*)(pos)) + blen;
            else if (blen == 2)
                dl = *((unsigned short*)(pos)) + blen;
            pos += dl;
            if ((movelen = fd.len - dl) != 0)
            {
                end -= movelen;
                memmove(pos, pos + movelen, end - pos);
            }
        }
    }
    return (uint_td)(pos - ptr);
}

ushort_td lenByCharnum(uchar_td type, uchar_td charsetIndex, ushort_td charnum)
{
    ushort_td len = charnum;
    if ((type == ft_wstring) || (type == ft_wzstring) || (type == ft_mywchar) ||
                    (type == ft_mywvarchar)|| (type == ft_mywvarbinary))
        len = (ushort_td)(mysql::charsize(CHARSET_UTF16LE) * charnum);
    else if (charsetIndex && ((type == ft_mychar) || (type == ft_myvarchar)|| 
            (type == ft_string) || (type == ft_zstring)|| (type == ft_myvarbinary) ))
        len = (ushort_td)(mysql::charsize(charsetIndex) * charnum);
    if ((type == ft_myvarchar) || (type == ft_mywvarchar) || (type == ft_myvarbinary) || (type == ft_mywvarbinary))
        len += ((len >= 256) ? 2 : 1);
    else
        len = std::min<ushort_td>(len, 255);
    return len;
}

const _TCHAR* getTypeName(short type)
{
    switch (type)
    {
    case ft_string:
        return _T("String");
    case ft_integer:
        return _T("Integer");
    case ft_float:
        return _T("Float");
    case ft_date:
        return _T("Date");
    case ft_time:
        return _T("Time");
    case ft_decimal:
        return _T("Decimal");
    case ft_money:
        return _T("Money");
    case ft_logical:
        return _T("Logical");
    case ft_numeric:
        return _T("Numeric");
    case ft_bfloat:
        return _T("BFloat");
    case ft_lstring:
        return _T("LString");
    case ft_zstring:
        return _T("ZString");
    case ft_note:
        return _T("Note");
    case ft_lvar:
        return _T("Lvar");
    case ft_uinteger:
        return _T("Unsigned Binary");
    case ft_autoinc:
        return _T("AutoIncrement");
    case ft_bit:
        return _T("Bit");
    case ft_numericsts:
        return _T("Numericsts");
    case ft_numericsa:
        return _T("Numericsa");
    case ft_currency:
        return _T("Currency");
    case ft_timestamp:
        return _T("TimeStamp");
    case ft_blob:
        return _T("Blob");
    case ft_wstring:
        return _T("WString");
    case ft_wzstring:
        return _T("WZstring");
    case ft_datetime:
        return _T("DateTime");
    case ft_guid:
        return _T("Guid");
    case ft_myvarchar:
        return _T("myVarChar");
    case ft_myvarbinary:
        return _T("myVarBinary");
    case ft_mywvarchar:
        return _T("myWVarChar");
    case ft_mywvarbinary:
        return _T("myWVarBinary");
    case ft_mychar:
        return _T("myChar");
    case ft_mywchar:
        return _T("myWChar");
    case ft_mydate:
        return _T("myDate");
    case ft_mytime:
        return _T("myTime");
    case ft_mydatetime:
        return _T("myDateTime");
    case ft_mytimestamp:
        return _T("myTimeStamp");
    case ft_mytext:
        return _T("myText");
    case ft_myblob:
        return _T("myBlob");
    case ft_autoIncUnsigned:
        return _T("AutoIncUnsigned");
    case ft_myfixedbinary:
        return _T("myFixedBinary");
    case ft_nullindicator:
        return _T("Nullindicator");
    default:
        return _T("Unknown");
    }
}

int getTypeAlign(short type)
{
    switch (type)
    {
    case ft_string:
    case ft_date:
    case ft_time:
    case ft_lstring:
    case ft_zstring:
    case ft_note:
    case ft_timestamp:
    case ft_wstring:
    case ft_wzstring:
    case ft_myvarchar:
    case ft_myvarbinary:
    case ft_mychar:
    case ft_mywchar:
    case ft_mywvarchar:
    case ft_mywvarbinary:
    case ft_mydate:
    case ft_mytime:
    case ft_mydatetime:
    case ft_mytimestamp:
    case ft_myblob:
    case ft_myfixedbinary:
    case ft_mytext:
        return BT_AL_LEFT;
    }
    return BT_AL_RIGHT;
}

const _TCHAR* btrVersion::moduleTypeString()
{
    switch (type)
    {
    case 'N':
        return _T("Requester");
    case 'D':
        return _T("DOS Workstation MicroKernel");
    case 'W':
        return _T("Windows Workstation MicroKernel");
    case 'O':
        return _T("OS/2 WorkStation MicroKernel");
    case '3':
    case '9':
        return _T("Windows 32Bit Workstation MicroKernel");
    case 'S':
        return _T("Netware Server MicroKernel");
    case 'T':
        return _T("Windows Server MicroKernel");
    case 'L':
        return _T("Linux Server MicroKernel");
    case 'F':
        return _T("FileSharing WorkStation MicroKernel");
    }
    return _T("Unknown Type");
}

const _TCHAR* btrVersion::moduleVersionShortString(_TCHAR* buf)
{
#pragma warning(disable : 4996)
    switch (type)
    {
    case 'N':
        _stprintf(buf, _T("Reqster %d.%d"), majorVersion, minorVersion);
        break;
    case 'D':
        _stprintf(buf, _T("DosLocal %d.%d"), majorVersion, minorVersion);
        break;
    case 'W':
        _stprintf(buf, _T("W1Local %d.%d"), majorVersion, minorVersion);
        break;
    case 'O':
        _stprintf(buf, _T("OS/2Local %d.%d"), majorVersion, minorVersion);
        break;
    case '3':
    case '9':
        _stprintf(buf, _T("W3Local %d.%d"), majorVersion, minorVersion);
        break;
    case 'S':
        _stprintf(buf, _T("NServer %d.%d"), majorVersion, minorVersion);
        break;
    case 'T':
        _stprintf(buf, _T("Server %d.%d"), majorVersion, minorVersion);
        break;
    case 'L':
        _stprintf(buf, _T("Lerver %d.%d"), majorVersion, minorVersion);
        break;
    default:
        return _T("Unknown Type");
    }
#pragma warning(default : 4996)
    return buf;
}

PACKAGE uchar_td getFilterLogicTypeCode(const _TCHAR* cmpstr)
{
    if (_tcscmp(cmpstr, _T("=")) == 0)
        return (uchar_td)eEqual;
    else if (_tcscmp(cmpstr, _T(">")) == 0)
        return (uchar_td)eGreater;
    else if (_tcscmp(cmpstr, _T("<")) == 0)
        return (uchar_td)eLess;
    else if (_tcscmp(cmpstr, _T("<>")) == 0)
        return (uchar_td)eNotEq;
    else if (_tcscmp(cmpstr, _T("=>")) == 0)
        return (uchar_td)eGreaterEq;
    else if (_tcscmp(cmpstr, _T(">=")) == 0)
        return (uchar_td)eGreaterEq;
    else if (_tcscmp(cmpstr, _T("=<")) == 0)
        return (uchar_td)eLessEq;
    else if (_tcscmp(cmpstr, _T("<=")) == 0)
        return (uchar_td)eLessEq;
    else if (_tcscmp(cmpstr, _T("&")) == 0)
        return (uchar_td)eBitAnd;
    else if (_tcscmp(cmpstr, _T("!&")) == 0)
        return (uchar_td)eNotBitAnd;
    else if (_tcscmp(cmpstr, _T("=i")) == 0)
        return (uchar_td)eEqual | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T(">i")) == 0)
        return (uchar_td)eGreater | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T("<i")) == 0)
        return (uchar_td)eLess | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T("<>i")) == 0)
        return (uchar_td)eNotEq | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T("=>i")) == 0)
        return (uchar_td)eGreaterEq | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T(">=i")) == 0)
        return (uchar_td)eGreaterEq | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T("=<i")) == 0)
        return (uchar_td)eLessEq | CMPLOGICAL_CASEINSENSITIVE;
    else if (_tcscmp(cmpstr, _T("<=i")) == 0)
        return (uchar_td)eLessEq | CMPLOGICAL_CASEINSENSITIVE;
    return 255;
}

} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs
