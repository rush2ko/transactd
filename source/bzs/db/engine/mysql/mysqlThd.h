#ifndef BZS_DB_ENGINE_MYSQL_MYSQLTHD_H
#define BZS_DB_ENGINE_MYSQL_MYSQLTHD_H
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

extern unsigned int g_openDatabases;

class THD;
void initThread(THD* thd);
THD* attachThd(THD* thd);
THD* createThdForThread();
void deleteThdForThread(THD* thd);
void endThread();

#if (defined(_WIN32) || defined(__APPLE__))
#define USETLS
#endif

#endif // BZS_DB_ENGINE_MYSQL_MYSQLTHD_H
