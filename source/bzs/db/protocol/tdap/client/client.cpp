/*=================================================================
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
=================================================================*/
#include "client.h"
#include "sqlBuilder.h"
#include "stringConverter.h"

using namespace bzs::db::protocol::tdap::client;

bzs::netsvc::client::connections* m_cons = NULL;


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

#if( defined(_WIN32) && defined(_MSC_VER))
DWORD g_tlsiID = 0;
#else
__THREAD client* __THREAD_BCB g_client=NULL;
#endif


bool client::readServerCharsetIndex()
{
	if (m_charsetIndexServer == -1)
	{
		request req = m_req;
		uint_td len = 256;
		char tmp[256]={0x00};
		req.op = TD_GETSERVER_CHARSET;
		req.data = tmp;
		req.datalen = &len;

		mutex::scoped_lock lck(m_mutex);
		char* p = con()->sendBuffer();
		unsigned int size = req.serialize(p);
		p = con()->asyncWriteRead(size);
		req.parse(p);
		if (req.result==0)
		{
			m_charsetIndexServer = mysql::charsetIndex((const char*)req.data);
			return true;
		}
		return false;
	}
	return true;
}

void client::addSecondCharsetData(unsigned int destCodePage, std::string& src)
{
	stringConverter cv(destCodePage, CP_UTF8);
	int osize = (int)src.size()*3;
	char* srvchar = new char[osize];
	
	size_t len = cv.convert(srvchar, osize, src.c_str(), src.size());
	srvchar[len]  = 0x00;
	src += std::string("\t")  + srvchar;
	delete [] srvchar;
}

bool client::buildDualChasetKeybuf()
{
	if (!readServerCharsetIndex())
		return false;
	//m_serverCharData = std::string((char*)m_req.keybuf, m_req.keylen);
	m_serverCharData = (char*)m_req.keybuf;
	if (CHARSET_UTF8 != m_charsetIndexServer)
		addSecondCharsetData(mysql::codePage(m_charsetIndexServer), m_serverCharData);
	else
		m_serverCharData += std::string("\t")  + (char*)m_req.keybuf;
	
	m_req.keybuf = (void_td*)m_serverCharData.c_str();
	m_req.keylen = (keylen_td)m_serverCharData.size() + 1;
    return true;
}

}//namespace client
}//namespace tdap
}//namespace protocol
}//namespace db
}//namespace bzs

