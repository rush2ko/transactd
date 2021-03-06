/* =================================================================
 Copyright (C) 2012 2013 BizStation Corp All rights reserved.

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

#include "sqlBuilder.h"
#include <bzs/rtl/stl_uty.h>
#include <algorithm>
#include <stdio.h>
#include <bzs/db/protocol/tdap/mysql/characterset.h>
#include <stdlib.h>
#include <bzs/env/crosscompile.h>
#include "stringConverter.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <bzs/env/mbcswchrLinux.h>
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

/** Remove extention
 *  To lower_case
 */
std::string getFileName(const char* name)
{
    const char* st = name;
    const char* en = st + strlen(name);
    const char* p = strrchr(name, '.');
    if (p)
        en = p;
    p = strrchr(name, PSEPARATOR_C);
    if (p)
        st = p + 1;
    p = strrchr(name, '=');
    if (p && p + 1 > st)
        st = p + 1;

    char tableName[MYSQL_TBNAME_SIZE] = { 0x00 };
    strncpy_s(tableName, MYSQL_TBNAME_SIZE, st, en - st);
    _strlwr_s(tableName, MYSQL_TBNAME_SIZE);
    return tableName;
}

#define TMP_BUFSIZE 100
char g_buf[TMP_BUFSIZE];

#define NOTE_TYPE 12
#define VAR_TYPE 13

const char* getFieldTypeName(const fielddef& fd, int size, bool nobinary,
                             const char* charsetName, int decimals)
{
    const char* bin_ptr = nobinary ? "" : "binary";
    char charsetString[128] = { " CHARACTER SET " };
    if (charsetName[0])
        strcat_s(charsetString, 128, charsetName);
    else
        charsetString[0] = 0x00;
    switch (fd.type)
    {
    case ft_integer:
    case ft_autoinc:
    case ft_currency:
    case ft_date:
    case ft_time:
    case ft_datetime:
    case ft_timestamp:
        if (size == 1)
            return "TINYINT";
        if (size == 2)
            return "SMALLINT";
        if (size == 3)
            return "MEDIUMINT";
        if (size == 4)
            return "INT";
        if (size == 8)
            return "BIGINT";
    case ft_bit:
        sprintf_s(g_buf, TMP_BUFSIZE, "BIT(%d)", size * 8);
        return g_buf;
    case ft_enum:
    case ft_set:
    case ft_uinteger:
    case ft_autoIncUnsigned:
        if (size == 1)
            return "TINYINT UNSIGNED";
        if (size == 2)
            return "SMALLINT UNSIGNED";
        if (size == 3)
            return "MEDIUMINT UNSIGNED";
        if (size == 4)
            return "INT UNSIGNED";
        if (size == 8)
            return "BIGINT UNSIGNED";
    case ft_logical:
        if (size == 1)
            return "TINYINT UNSIGNED";
        if (size == 2)
            return "SMALLINT UNSIGNED";
    case ft_myyear:
        return "YEAR";
    case ft_mydate:
        return "DATE";
    case ft_mytime:
        if(fd.isLegacyTimeFormat())
            sprintf_s(g_buf, TMP_BUFSIZE, "TIME");
        else
            sprintf_s(g_buf, TMP_BUFSIZE, "TIME(%d)", decimals);
        return g_buf;
    case ft_mydatetime:
        if(fd.isLegacyTimeFormat())
            sprintf_s(g_buf, TMP_BUFSIZE, "DATETIME");
        else
            sprintf_s(g_buf, TMP_BUFSIZE, "DATETIME(%d)", decimals);
        return g_buf;
    case ft_mytimestamp:
        if(fd.isLegacyTimeFormat())
            sprintf_s(g_buf, TMP_BUFSIZE, "TIMESTAMP");
        else
            sprintf_s(g_buf, TMP_BUFSIZE, "TIMESTAMP(%d)", decimals);
        return g_buf;
    case ft_mytext:
        if (size - 8 == 4)
            return "LONGTEXT";
        else if (size - 8 == 3)
            return "MEDIUMTEXT";
        else if (size - 8 == 2)
            return "TEXT";
        return "TINYTEXT";
    case ft_myblob:
        if (size - 8 == 4)
            return "LONGBLOB";
        else if (size - 8 == 3)
            return "MEDIUMBLOB";
        else if (size - 8 == 2)
            return "BLOB";
        return "TINYBLOB";
    case ft_myjson:
         return "JSON";
    case ft_mygeometry:
         return "GEOMETRY";
    case ft_float:
        if (size == 4)
            return "FLOAT";
        if (size == 8)
            return "DOUBLE";
    case ft_mydecimal:
        sprintf_s(g_buf, TMP_BUFSIZE, "DECIMAL(%d, %d)", fd.digits, decimals);
        return g_buf;
    case ft_string:
    case ft_wstring:
        sprintf_s(g_buf, TMP_BUFSIZE, "BINARY(%d)", size);
        return g_buf;
    case ft_zstring:
    case ft_wzstring:
        sprintf_s(g_buf, TMP_BUFSIZE, "BINARY(%d)", size);
        return g_buf;
    case ft_note: // note can not use key
    case ft_lvar: // var
        sprintf_s(g_buf, TMP_BUFSIZE, "VARBINARY(%d)", size);
        return g_buf;
    case ft_lstring:
    case ft_myvarbinary:
    case ft_myfixedbinary:
        sprintf_s(g_buf, TMP_BUFSIZE, "VARBINARY(%d)", size);
        return g_buf;
    case ft_mywvarbinary:
        sprintf_s(g_buf, TMP_BUFSIZE, "VARBINARY(%d)", size);
        return g_buf;
    case ft_mywvarchar:
        sprintf_s(g_buf, TMP_BUFSIZE, "VARCHAR(%d) %s CHARACTER SET utf16le",
                  size, bin_ptr);
        return g_buf;
    case ft_myvarchar:
        sprintf_s(g_buf, TMP_BUFSIZE, "VARCHAR(%d) %s%s", size, bin_ptr,
                  charsetString);
        return g_buf;
    case ft_mychar:
        sprintf_s(g_buf, TMP_BUFSIZE, "CHAR(%d) %s%s", size, bin_ptr,
                  charsetString);
        return g_buf;
    case ft_mywchar:
        sprintf_s(g_buf, TMP_BUFSIZE, "CHAR(%d) %s CHARACTER SET utf16le", size,
                  bin_ptr);
        return g_buf;
    case ft_decimal:
    case ft_money:
    case ft_numeric:
    case ft_bfloat:
    case ft_numericsts:
    case ft_numericsa:
    case ft_guid:
        sprintf_s(g_buf, TMP_BUFSIZE, "BINARY(%d)", size);
        return g_buf;
    }
    return "";
}

FLAGS getKeyFlags(const tabledef* table, short fieldNum)
{
    for (int i = 0; i < table->keyCount; i++)
    {
        const keydef& key = table->keyDefs[i];
        for (int j = 0; j < key.segmentCount; j++)
        {
            if (key.segments[j].fieldNum == fieldNum)
                return key.segments[j].flags;
        }
    }
    FLAGS a;
    a.all = 0;
    return a;
}

bool isNumericFieldName(const char* name)
{
    if (name)
    {
        if (name[0] >= '0' && name[0] <= '9')
            return true;
    }
    return false;
}

char* removeTimeStampOptionDecimal(const char* s, char* buf, size_t size)
{
    strcpy_s(buf, size, s);
    char* p = strrchr(buf, '(');
    if (p)
        *p = 0x00;
    return buf;
}

char* getBitDefalutValue(char* buf, size_t size, unsigned __int64 v)
{
    char* p = buf;
    for (int i = 0; i< 64;++i)
    {
        if (v & (1ULL << (63 - i)))
            *(p++) = '1';
        else if (p != buf)
            *(p++) = '0';
    }
    *p = 0x00;
    return buf;
}

std::string sqlBuilder::getFieldList(const tabledef* table, std::vector<std::string>& fdl, const clsrv_ver* ver)
{
    std::string s;
    int len;
    char timestamp_tmp[64];
    for (int i = 0; i < table->fieldCount; i++)
    {
        const fielddef& fd = table->fieldDefs[i];
        s += "`";
        s += fdl[i];
        s += "` ";
        len = fd.len - fd.varLenBytes();
        if (fd.type == ft_myfixedbinary)
            len -= 2;
        /*
         Although len is a number of bytes in BDF, it becomes the number of
         characters in MySQL.
         Moreover, unicode cannot be specified by charset of the field. */
        if (fd.charsetIndex() == 0)
            const_cast<fielddef&>(fd).setCharsetIndex(table->charsetIndex);
        if ((fd.type == ft_myvarchar) || (fd.type == ft_mychar))
            len /= mysql::charsize(fd.charsetIndex());
        else if ((fd.type == ft_mywvarchar) || (fd.type == ft_mywchar))
            len /= mysql::charsize(CHARSET_UTF16LE);

        FLAGS f = getKeyFlags(table, i);
        const char* charsetName = "";
        if (fd.charsetIndex() != table->charsetIndex)
            charsetName = mysql::charsetName(fd.charsetIndex());

        s += getFieldTypeName(fd, len, f.bitA, charsetName, fd.decimals);
        const char* p = fd.defaultValue_str();
        if ((fd.defaultValue() == DFV_TIMESTAMP_DEFAULT) && 
            ((fd.type == ft_mytimestamp) || (fd.type == ft_mydatetime)))
            p = timeStampDefaultStr(fd, timestamp_tmp, 64);
        if (fd.isNullable())
        {
            if (fd.isDefaultNull())
                s += " NULL DEFAULT NULL";
            else if (p[0])
            {
                if ((fd.type == ft_mytimestamp) || 
                    (fd.type == ft_mydatetime))
                {  
                    s += " NULL DEFAULT ";
                    if (!ver->isSupportDateTimeTimeStamp())
                    {
                        char tmp[100];
                        s += removeTimeStampOptionDecimal(p, tmp, 100);
                    }
                    else
                        s += p;
                }else if (fd.type == ft_bit)
                {
                    s += " NULL DEFAULT b'";
                    char tmp[100] = {NULL};
                    s += getBitDefalutValue(tmp, 100, (unsigned __int64)fd.defaultValue64());
                    s += "'";
                }
                else
                {
                    s += " NULL DEFAULT '";
                    s += p;
                    s += "'";
                }
            }else
            {
                if (isStringTypeForIndex(fd.type))
                    s += " NULL DEFAULT ''";
                else if (fd.type == ft_mydatetime || fd.type == ft_mytimestamp)
                    s += " NULL DEFAULT '0000-00-00 00:00:00'";
                else if (fd.type == ft_mydate)
                    s += " NULL DEFAULT '0000-00-00'";
                else if (fd.type == ft_mytime)
                    s += " NULL DEFAULT '00:00:00'";
                else if (fd.isNumericType() || fd.isDateTimeType()/*P.SQL date time*/ )
                    s += " NULL DEFAULT '0'";
                else
                    s += " NULL";
            }
        }
        else
        {
            s += " NOT NULL ";
            if (p[0])
            {
                if ((fd.type == ft_mytimestamp) ||
                    (fd.type == ft_mydatetime))
                {   
                    s += "DEFAULT ";
                    if (!ver->isSupportDateTimeTimeStamp())
                    {
                        char tmp[100];
                        s += removeTimeStampOptionDecimal(p, tmp, 100);
                    }
                    else
                        s += p;
                }else if (fd.type == ft_bit)
                {
                    s += "DEFAULT b'";
                    char tmp[100] = {NULL};
                    s += getBitDefalutValue(tmp, 100, (unsigned __int64)fd.defaultValue());
                    s += "'";
                }
                else
                {
                    s += "DEFAULT '";
                    s += p;
                    s += "'";
                }
            }    
        }
        if (fd.isTimeStampOnUpdate())
        {
            char buf[64];
            updateTimeStampStr(&fd, buf, 64);
            if (!ver->isSupportDateTimeTimeStamp())
            {
                char tmp[100];
                s += removeTimeStampOptionDecimal(buf, tmp, 100);
            }
            else
                s += buf;
        }

        if ((fd.type == ft_autoinc) || (fd.type == ft_autoIncUnsigned))
            s += " AUTO_INCREMENT";
        s += ",";
    }
    return s;
}

void sqlBuilder::insertNisFields(const tabledef* td, std::vector<std::string>& fdl,
                     std::string& s)
{
    char buf[20];
    for (int i = 0; i < td->keyCount; i++)
    {
        _ltoa_s(i, buf, 20, 10);
        std::string fddef = "";
        const keydef& key = td->keyDefs[i];
        if (td->isNullKey(key))
        {
            if (td->isNeedNis(key))
            {
                if (key.segments[0].flags.bit9)
                    fddef = std::string("`") + "$nfn" + buf +
                            "` TINYINT UNSIGNED NULL,";
                else if (key.segments[0].flags.bit3)
                    fddef = std::string("`") + "$nfa" + buf +
                            "` TINYINT UNSIGNED NULL,";
            }
        }
        if (fddef != "")
            s += fddef;
    }
}

std::string& sqlBuilder::getKey(const tabledef* td, std::vector<std::string>& fdl, 
                                    int index, std::string& s, bool specifyKeyNum)
{
    char buf[20];
    const keydef& key = td->keyDefs[index];
    if (specifyKeyNum)
        _ltoa_s(key.keyNumber, buf, 20, 10);     
    else
        _ltoa_s(index, buf, 20, 10);
    if ((td->primaryKeyNum == index) &&
        (fdl[key.segments[0].fieldNum] == "auto_id_field"))
        s += " PRIMARY KEY ";
    else
    {
        if (key.segments[0].flags.bit0 == false)
            s += " UNIQUE ";
        else
            s += " INDEX ";
        s += "key";

        s += buf;
    }
    s += "(";

    // "nf" segment is added to a head.
    if (td->isNeedNis(key))
    {
        if (key.segments[0].flags.bit9)
            s += std::string("`") + "$nfn" + buf + "`,";
        else if (key.segments[0].flags.bit3)
            s += std::string("`") + "$nfa" + buf + "`,";
    }
    for (int j = 0; j < key.segmentCount; j++)
    {
        s += "`";
        s += fdl[key.segments[j].fieldNum];
        s += "`";

        // part key
        const fielddef& fd = td->fieldDefs[key.segments[j].fieldNum];
        if (fd.keylen && ((fd.keylen != fd.len) || fd.blobLenBytes()))
        {
            sprintf_s(buf, 20, "(%d)", fd.keylen);
            s += buf;
        }

        if (key.segments[j].flags.bit6)
            s += " DESC";
        s += ",";
    }
    s.erase(s.end() - 1);
    s += "),";
    return s;
}

std::string sqlBuilder::getKeyList(const tabledef* table, std::vector<std::string>& fdl)
{
    std::string s;
    for (int i = 0; i < table->keyCount; i++)
        getKey(table, fdl, i, s);
    return s;
}

std::string sqlBuilder::convertString(unsigned int toPage, unsigned int fromPage,
                          const char* p)
{
    std::string s;
    stringConverter cv(toPage, fromPage);
    int osize = (int)strlen(p) * 3;
    char* srvchar = new char[osize];
    size_t len = cv.convert(srvchar, osize, p, strlen(p));
    srvchar[len] = 0x00;
    s = srvchar;
    delete[] srvchar;
    return s;
}

// suffix added names list
void makeSuffixNamesList(const tabledef* table, std::vector<std::string>& fds)
{
    std::vector<std::string> fdl;// lower case names list.
    char tmp[256];
    char num[10];
    for (int i = 0; i < table->fieldCount; i++)
    {
        fielddef& fd = table->fieldDefs[i];
        strcpy_s(tmp, 256, fd.nameA());
        _strlwr_s(tmp, 256);
        if (std::find(fdl.begin(), fdl.end(), tmp) != fdl.end())
        {
            _ltoa_s(i, num, 10, 10);
            strcat_s(tmp, 256, num);
            fds.push_back(std::string(fd.nameA()) + num);
        }
        else
            fds.push_back(fd.nameA());
        fdl.push_back(tmp);
    }
}

std::string sqlBuilder::sqlCreateTable(const char* name /* utf8 */, tabledef* td,
                           uchar_td charsetIndexServer, const clsrv_ver* ver)
{
    // Duplication of a name is inspected and, in duplication, _1 is added.
    // It does not correspond to two or more duplications.
    std::string s = "CREATE TABLE `";

    std::vector<std::string> fds;// suffix added names list
    makeSuffixNamesList(td, fds);

    uint_td schemaCodePage =
        td->schemaCodePage ? td->schemaCodePage : GetACP();
    if ((name && name[0]))
    {
        std::string name2 = name;
        if (schemaCodePage != CP_UTF8)
            name2 = convertString(schemaCodePage, CP_UTF8, name);
        s += getFileName(name2.c_str()) + "` (";
    }
    else
        s += getFileName(td->fileNameA()) + "` (";
    s += getFieldList(td, fds, ver);
    insertNisFields(td, fds, s);
    s += getKeyList(td, fds);
    if (s[s.size() - 1] == ',')
        s.erase(s.end() - 1);
    std::string compress;
    if (td->flags.bit3)
        compress = "ROW_FORMAT=COMPRESSED "; 
    s += ") ENGINE=InnoDB " + compress + "default charset=" + std::string(mysql::charsetName(td->charsetIndex));
         

    // create statement charset must be server default charset.
    // server default charset writen in my.cnf.
    if (schemaCodePage != mysql::codePage(charsetIndexServer))
        s = convertString(mysql::codePage(charsetIndexServer), schemaCodePage,
                          s.c_str());
    return s;
}

std::string sqlBuilder::sqlCreateIndex(const tabledef* td, int keyNum,
        bool specifyKeyNum, uchar_td charsetIndexServer, const clsrv_ver* ver)
{
    std::string s;
    std::vector<std::string> fds;// suffix added names list
    makeSuffixNamesList(td, fds);
    uint_td schemaCodePage =
        td->schemaCodePage ? td->schemaCodePage : GetACP();
    s += getFieldList(td, fds, ver);
    insertNisFields(td, fds, s);
    s = "`";
    s+= getFileName(td->fileNameA());
    s += "` ADD";
    getKey(td, fds, keyNum, s, specifyKeyNum); 
    s.erase(s.end() - 1);
    // create statement charset must be server default charset.
    // server default charset writen in my.cnf.
    if (schemaCodePage != mysql::codePage(charsetIndexServer))
        s = convertString(mysql::codePage(charsetIndexServer), schemaCodePage,
                          s.c_str());
    return s;
}

int findFieldNum(std::vector<fielddef>& fds, int pos)
{
    for (int i = 0; i < (int)fds.size(); i++)
    {
        if (fds[i].pos == pos)
            return i;
    }
    return -1;
}

void makeField(std::vector<fielddef>& fds, keySpec* ks, int keynum, int seg)
{
    char buf[100];
    fielddef fd;
    memset(&fd, 0x00, sizeof(fielddef));
    sprintf_s(buf, 100, "key%d_%d", keynum, seg);
    fd.setNameA(buf);
    fd.len = ks->keyLen;
    fd.type = ks->keyType;
    fd.pos = ks->keyPos;
    fds.push_back(fd);
}

bool field_sort(const fielddef& l, const fielddef& r)
{
    if (l.pos < r.pos)
        return 1;
    return 0;
}

void completeFields(std::vector<fielddef>& fds, int reclen, bool valiableLen)
{
    char buf[100];
    int pos = 1;
    for (int i = 0; i < (int)fds.size(); i++)
    {
        if (fds[i].pos > pos)
        {
            fielddef fd;
            memset(&fd, 0x00, sizeof(fielddef));
            sprintf_s(buf, 100, "field%d", i);
            fd.setNameA(buf);
            fd.pos = pos;
            fd.len = fds[i].pos - pos;
            fd.type = (fd.len > 255) ? ft_note : ft_zstring;
            fds.insert(fds.begin() + i, fd);
            i++;
        }
        pos = fds[i].pos + fds[i].len;
    }
    if (pos <= reclen)
    {
        fielddef fd;
        memset(&fd, 0x00, sizeof(fielddef));

        sprintf_s(buf, 100, "field%d", (int)fds.size());
        fd.setNameA(buf);
        fd.pos = pos;
        fd.len = reclen - pos + 1;
        fd.type = (fd.len > 255) ? ft_note : ft_zstring;
        if (fd.type == NOTE_TYPE)
            fd.len = 57000 - pos + 1;
        fds.push_back(fd);
        pos = fd.pos + fd.len;
    }
    // In the case of a variable-length record, the last must have a variable
    // length field.
    if (valiableLen)
    {
        if (fds[fds.size() - 1].type != ft_note)
        {
            fielddef fd;
            memset(&fd, 0x00, sizeof(fielddef));
            sprintf_s(buf, 100, "field%d", (int)fds.size());
            fd.setNameA(buf);
            fd.pos = pos;
            fd.len = 57000 - pos + 1;
            fd.type = NOTE_TYPE;
            fds.push_back(fd);
        }
    }
}

void makeTableDef(tabledef* TableDef, fileSpec* fs, std::vector<fielddef>& fds)
{
    TableDef->keyCount = (uchar_td)fs->indexCount;
    TableDef->flags.all = fs->fileFlag.all;
    TableDef->fixedRecordLen = fs->recLen;

    int k = 0;
    int seg = 0;
    int keynum = 0;
    while (keynum < TableDef->keyCount)
    {
        keySpec* ks = &(fs->keySpecs[k++]);
        // key flag
        TableDef->keyDefs[keynum].segments[seg].flags.all = ks->keyFlag.all;
        TableDef->keyDefs[keynum].segmentCount = seg + 1;

        // field info
        if (findFieldNum(fds, ks->keyPos) == -1)
            makeField(fds, ks, keynum, seg);
        seg++;
        if (ks->keyFlag.bit4 == false)
        {
            keynum++;
            seg = 0;
        }
    }
    // Sort by position.
    std::sort(fds.begin(), fds.end(), field_sort);

    // Fields are added to non key spaces
    completeFields(fds, fs->recLen, (fs->fileFlag.all & 1));
    // Assignment of a field number
    k = 0;
    seg = 0;
    keynum = 0;
    while (keynum < TableDef->keyCount)
    {
        keySpec* ks = &(fs->keySpecs[k++]);
        TableDef->keyDefs[keynum].segments[seg].fieldNum =
            findFieldNum(fds, ks->keyPos);
        seg++;
        if (ks->keyFlag.bit4 == false)
        {
            keynum++;
            seg = 0;
        }
    }

    // Copy of field info.
    TableDef->fieldDefs = &fds[0];
    TableDef->fieldCount = (ushort_td)fds.size();
}

std::string sqlBuilder::sqlCreateTable(const char* fileName, fileSpec* fs,
                           uchar_td charsetIndexServer, const clsrv_ver* ver)
{
    tabledef table;
    memset(&table, 0, sizeof(tabledef));
    table.setFileNameA(fileName);
    table.charsetIndex = mysql::charsetIndex(GetACP());
    table.schemaCodePage = CP_UTF8;
    std::vector<fielddef> fds;
    std::vector<keydef> kds;
    for (int i = 0; i < fs->indexCount; i++)
        kds.push_back(keydef());
    if (fs->indexCount)
        table.keyDefs = &kds[0];
    makeTableDef(&table, fs, fds);

    return sqlCreateTable(fileName, &table, charsetIndexServer, ver);
}

} // namespace client
} // namespace tdap
} // namespace protocol
} // namespace db
} // namespace bzs
