#ifndef BZS_DB_PROTOCOL_TDAP_CLIENT_REQUSET_H
#define BZS_DB_PROTOCOL_TDAP_CLIENT_REQUSET_H
/* =================================================================
 Copyright (C) 2013 BizStation Corp All rights reserved.

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
#pragma warning(disable : 4005) //BOOST_ASIO_ERROR_CATEGORY_NOEXCEPT redefine bug 
#include <bzs/db/protocol/tdap/tdapRequest.h>
#include <bzs/netsvc/client/iconnection.h>
#pragma warning(default : 4005)

#ifdef USE_DATA_COMPRESS
#include <bzs/rtl/lzss.h>
#endif

namespace bzs
{

namespace db
{
namespace engine{ namespace mysql { class table; } }

namespace protocol
{
namespace tdap
{
namespace client
{

short errorCode(const boost::system::error_code& e);

class request : public bzs::db::protocol::tdap::request, 
                public bzs::netsvc::client::idirectReadHandler
{

    unsigned int handleError(const boost::system::error_code& e)
    {
        result = errorCode(e);
        return 0;
    }
public:
    clientID* cid;
    request() : bzs::db::protocol::tdap::request(), cid(NULL){};

    unsigned int onRead(unsigned int size, bzs::netsvc::client::connection* c) // orverride
    {
        unsigned int readlen = 0;
        readlen += c->directRead(&paramMask, sizeof(uint_td)); // paramMask and result
        if (c->error()) return handleError(c->error());

        if (P_MASK_POSBLK & paramMask)
        {
            readlen += c->directRead(pbk, TD_POSBLK_TRANSMIT_SIZE);
            if (c->error()) return handleError(c->error());
        }
        if (P_MASK_DATALEN & paramMask)
        {
            uint_td tmp;
            readlen += c->directRead(&tmp, sizeof(uint_td));
            if (c->error()) return handleError(c->error());

            if (*datalen < tmp)
            {
                result = STATUS_BUFFERTOOSMALL;
                return readlen;
            }
            else
            {
                if (pbk->allocFunc && pbk->tb)
                    data = pbk->allocFunc(pbk->tb, tmp);
                *datalen = tmp;
                if (P_MASK_DATA & paramMask)
                {
                    readlen += c->directRead(data, *datalen);
                    if (c->error()) return handleError(c->error());
                }
            }
        }

        if (P_MASK_KEYBUF & paramMask)
        {
            keylen_td tmp;
            readlen += c->directRead(&tmp, sizeof(keylen_td));
            if (c->error()) return handleError(c->error());
            if (keylen < tmp)
            {
                result = STATUS_KEYBUFFERTOOSMALL;
                return readlen;
            }
            memset(keybuf, 0, keylen);
            keylen = tmp;
            readlen += c->directRead(keybuf, keylen);
            if (c->error()) return handleError(c->error());
        }
        if (P_MASK_KEYNUM & paramMask)
        {
            readlen += c->directRead(&keyNum, sizeof(char_td));
            if (c->error()) return handleError(c->error());
        }
        if (paramMask & P_MASK_BLOBBODY)
        {
            blobHeader =
                (const bzs::db::blobHeader*)c->directReadRemain(size - readlen);
            if (c->error()) return handleError(c->error());
            readlen = size;
            if (blobHeader->rows)
                blobHeader->resetCur();
        }
        else
            blobHeader = NULL;
        assert(readlen == size);
        return readlen;
    }

    inline void parse(const char* p, bool ex/*, unsigned int segmentDataLen,
                      unsigned short rows*/)
    {
        p += sizeof(unsigned int);        //  4 byte read length
        paramMask = *((ushort_td*)p);     //  2 byte paramMask
        p += sizeof(ushort_td);

        result = *((ushort_td*)p);        //  2 byte result
        p += sizeof(ushort_td);

        if (P_MASK_POSBLK & paramMask)    //  4 byte pbk
        {
            memcpy(pbk, p, TD_POSBLK_TRANSMIT_SIZE);
            p += TD_POSBLK_TRANSMIT_SIZE;
            if (P_MASK_PB_BOOKMARK & paramMask)
            {
                unsigned char len =  *((unsigned char*)p);
                // copy sizeByte and bookmark
                pbk->bookmarkLen = len;
                pbk->lock = ((paramMask & P_MASK_PB_LOCKED) != 0);
                memcpy(pbk->bookmark, ++p , len);
                p += len;
            }
        }
        if (P_MASK_PB_ERASE_BM & paramMask)
             pbk->bookmarkLen = 0; 

        if (P_MASK_DATALEN & paramMask)
        {
            uint_td tmp = *((uint_td*)p);
            if (*datalen < tmp)
            {
                result = STATUS_BUFFERTOOSMALL;
                return ;
            }
            else
                *datalen = tmp;
            p += sizeof(uint_td);
        }

        /*if (P_MASK_FINALDATALEN & paramMask)
        {
            memset(data, 0, *datalen);
            if (*datalen < segmentDataLen)
                result = STATUS_BUFFERTOOSMALL;
            else
                *datalen = segmentDataLen;
        }*/
#ifdef USE_DATA_COMPRESS
        if (P_MASK_USELZSS & paramMask)
        {
            unsigned int compSize;
            memcpy(&compSize, p, sizeof(unsigned int));
            p += sizeof(unsigned int);
            unsigned int decompSize = bzs::rtl::lzssDecode(p, data);
            p += compSize;
        }
        else
#endif
        if (P_MASK_DATA & paramMask)
        {
            if (ex)
            {
                if (pbk->allocFunc && pbk->tb)
                    data = pbk->allocFunc(pbk->tb, *datalen);
            }
            memcpy(data, p, *datalen);
            p += *datalen;
            /*if (P_MASK_FINALDATALEN & paramMask)
            {
                memcpy(data, &rows, 2);
                p += sizeof(unsigned int);
            }*/
        }
        if (P_MASK_KEYBUF & paramMask)
        {
            if (keylen < *((keylen_td*)p))
            {
                result = STATUS_KEYBUFFERTOOSMALL;
                return;
            }
            memset(keybuf, 0, keylen);
            keylen = *((keylen_td*)p);
            p += sizeof(keylen_td);
            memcpy(keybuf, p, keylen);
            p += keylen;
        }
        if (P_MASK_KEYNUM & paramMask)
        {
            keyNum = *((char_td*)p);
            p += sizeof(char_td);
        }

        /*if (P_MASK_FINALRET & paramMask)
        {
            result = *((ushort_td*)p);
            p += sizeof(ushort_td);
        }*/

        if (paramMask & P_MASK_BLOBBODY)
        {
            blobHeader = (const bzs::db::blobHeader*)p;
            if (blobHeader->rows)
                blobHeader->resetCur();
        }
        else
            blobHeader = NULL;
    }

    inline unsigned int sendLenEstimate()
    {
        if (P_MASK_EX_SENDLEN & paramMask)
        {
            unsigned int v = *((unsigned int*)data);
            v &= 0xFFFFFFF; // 28bit
            return v + 2048;
        }
        else if (P_MASK_DATA & paramMask)
            return *datalen + 2048;
        return 2048;
    }

    inline unsigned int serialize(char* buf)
    {
        char* p = buf;
        unsigned int dataSize = 0;

        if (P_MASK_EX_SENDLEN & paramMask)
        {
            dataSize = *((unsigned int*)data);
            dataSize &= 0xFFFFFFF;
        }
        else if (P_MASK_DATA & paramMask)
            dataSize = *datalen;
// add lzss infomation
#ifdef USE_DATA_COMPRESS
        if (dataSize > 1500)
            paramMask |= P_MASK_USELZSS;
#endif
        p += sizeof(unsigned int);

        memcpy(p, &paramMask, sizeof(ushort_td));
        p += sizeof(ushort_td);

        memcpy(p, &op, sizeof(ushort_td));
        p += sizeof(ushort_td);

        if (P_MASK_POSBLK & paramMask)
        {
            memcpy(p, pbk, TD_POSBLK_TRANSMIT_SIZE);
            p += TD_POSBLK_TRANSMIT_SIZE;
        }

        if (P_MASK_DATALEN & paramMask)
        {
            memcpy(p, datalen, sizeof(uint_td));
            p += sizeof(uint_td);
        }
#ifdef USE_DATA_COMPRESS
        if (P_MASK_USELZSS & paramMask)
        {
            uint_td compSize =
                bzs::rtl::lzssEncode(data, dataSize, p + sizeof(uint_td));
            memcpy(p, &compSize, sizeof(uint_td));
            p += compSize + sizeof(uint_td);
        }
        else
#endif
        if (P_MASK_EX_SENDLEN & paramMask)
        {
            unsigned int v = *((unsigned int*)data);
            v &= 0xFFFFFFF; // 28bit
            memcpy(p, data, v);
            p += v;
        }
        else if (P_MASK_DATA & paramMask)
        {
            memcpy(p, data, *datalen);
            p += *datalen;
        }

        if (P_MASK_KEYBUF & paramMask)
        {
            memcpy(p, &keylen, sizeof(keylen_td));
            p += sizeof(keylen_td);

            memcpy(p, keybuf, keylen);
            p += keylen;
            // increment 1 byte
            *p = 0x00;
            ++p;
        }
        if (P_MASK_KEYNUM & paramMask)
        {
            memcpy(p, &keyNum, sizeof(char_td));
            p += sizeof(char_td);
        }

        memcpy(p, &cid->id, CLIENTID_SIZE);
        p += CLIENTID_SIZE;

        unsigned int totallen = (unsigned int)(p - buf);
        memcpy(buf, &totallen, sizeof(unsigned int));
        return totallen;
    }
};

} // namespace client
} // namespace protocol
} // namespace tdap
} // namespace db
} // namespace bzs

#endif // BZS_DB_PROTOCOL_TDAP_CLIENT_REQUSET_H
