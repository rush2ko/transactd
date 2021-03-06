# coding : utf-8
=begin ============================================================
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
===================================================================
=end
require 'transactd'

require 'rbconfig'
IS_WINDOWS = (RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)

def getEnv(valuename)
  return ENV[valuename] if ENV[valuename] != nil
  return ''
end

def getHost()
  hostname = getEnv('TRANSACTD_RSPEC_HOST')
  hostname = '127.0.0.1/' if hostname == ''
  hostname = hostname + '/' unless (hostname =~ /\/$/)
  return hostname
end

HOSTNAME = getHost()
USERNAME = getEnv('TRANSACTD_RSPEC_USER')
USERPART = USERNAME == '' ? '' : USERNAME + '@'
PASSWORD = getEnv('TRANSACTD_RSPEC_PASS')
PASSPART = PASSWORD == '' ? '' : '&pwd=' + PASSWORD
URL = 'tdap://' + USERPART + HOSTNAME + 'test?dbfile=test.bdf' + PASSPART
URL_KANJI = 'tdap://' + USERPART + HOSTNAME + 'テスト?dbfile=構成.bdf' + PASSPART
FDI_ID = 0
FDN_ID = '番号'.encode('UTF-8')
FDI_NAME = 1
FDN_NAME = '名前'.encode('UTF-8')


def testDropDatabase(db, url)
  db.open(url)
  expect(db.stat()).to eq 0
  db.drop()
  expect(db.stat()).to eq 0
end

def testCreateDatabase(url)
  db = Transactd::Database.new()
  db.create(url)
  if db.stat() == Transactd::STATUS_TABLE_EXISTS_ERROR
    testDropDatabase(db, url)
    db.create(url)
  end
  expect(db.stat()).to eq 0
  db.close()
end

def testOpenDatabase(db, url)
  db.open(url, Transactd::TYPE_SCHEMA_BDF, Transactd::TD_OPEN_NORMAL)
  expect(db.stat()).to eq 0
end

def testCreateTable(db, tableid, tablename)
  dbdef = db.dbDef()
  expect(dbdef).not_to be nil
  td = Transactd::Tabledef.new()
  ### Set table schema codepage to UTF-8
  #     - codepage for field NAME and tableNAME
  td.schemaCodePage = Transactd::CP_UTF8
  td.setTableName(tablename.encode('UTF-8'))
  td.setFileName((tablename + '.dat').encode('UTF-8'))
  ### Set table default charaset index
  #     - default charset for field VALUE
  td.charsetIndex = Transactd::charsetIndex(Transactd::CP_UTF8)
  ###
  td.id = tableid
  td.pageSize = 2048
  dbdef.insertTable(td)
  expect(dbdef.stat()).to eq 0
  fd = dbdef.insertField(tableid, FDI_ID)
  fd.setName(FDN_ID)
  fd.type = Transactd::Ft_integer
  fd.len = 4
  dbdef.updateTableDef(tableid)
  expect(dbdef.stat()).to eq 0
  fd = dbdef.insertField(tableid, FDI_NAME)
  fd.setName(FDN_NAME)
  fd.type = Transactd::Ft_zstring
  fd.len = 33
  ### Set field charset index
  #     - charset for each field VALUE
  # fd.setCharsetIndex(Transactd::charsetIndex(Transactd::CP_UTF8))
  dbdef.updateTableDef(tableid)
  expect(dbdef.stat()).to eq 0
  kd = dbdef.insertKey(tableid, 0)
  kd.segment(0).fieldNum = 0
  kd.segment(0).flags.bit8 = 1
  kd.segment(0).flags.bit1 = 1
  kd.segmentCount = 1
  dbdef.updateTableDef(tableid)
  expect(dbdef.stat()).to eq 0
end

def testOpenTable(db, tablename)
  tb = db.openTable(tablename.encode('UTF-8'))
  expect(db.stat()).to eq 0
  return tb
end

def testInsert(db, tablename)
  tb = testOpenTable(db, tablename)
  expect(tb).not_to be nil
  tb.clearBuffer()
  tb.setFV(FDN_ID, 1)
  tb.setFV(FDN_NAME, '小坂'.encode('UTF-8'))
  tb.insert()
  expect(tb.stat()).to eq 0
  tb.clearBuffer()
  tb.setFV(FDN_ID, 2)
  tb.setFV(FDN_NAME, '矢口'.encode('UTF-8'))
  tb.insert()
  expect(tb.stat()).to eq 0
  tb.clearBuffer()
  tb.setFV(FDN_ID, 3)
  tb.setFV(FDN_NAME, 'ビズステーション'.encode('UTF-8'))
  tb.insert()
  expect(tb.stat()).to eq 0
  tb.close()
end

def testGetEqual(db, tablename)
  tb = testOpenTable(db, tablename)
  expect(tb).not_to be nil
  tb.clearBuffer()
  tb.setFV(FDN_ID, 1)
  tb.seek()
  expect(tb.getFVstr(FDN_NAME)).to eq '小坂'.encode('UTF-8')
  tb.close()
end

def testFind(db, tablename)
  tb = testOpenTable(db, tablename)
  expect(tb).not_to be nil
  tb.setKeyNum(0)
  tb.clearBuffer()
  tb.setFilter('番号 >= 1 and 番号 < 3'.encode('UTF-8'), 1, 0)
  tb.setFV(FDN_ID, 1)
  tb.find(Transactd::Table::FindForword)
  expect(tb.stat()).to eq 0
  expect(tb.getFVint(FDN_ID)).to eq 1
  expect(tb.getFVstr(FDN_NAME)).to eq '小坂'.encode('UTF-8')
  tb.findNext(true)
  expect(tb.stat()).to eq 0
  expect(tb.getFVint(FDN_ID)).to eq 2
  expect(tb.getFVstr(FDN_NAME)).to eq '矢口'.encode('UTF-8')
  tb.findNext(true)
  expect(tb.stat()).to eq Transactd::STATUS_EOF
  tb.close()
end

def testWhole(tableid, tablename, url)
  db = Transactd::Database.new()
  tablename = tablename.encode('UTF-8') # table name must be UTF-8
  testOpenDatabase(db, url)
  testCreateTable(db, tableid, tablename)
  testOpenTable(db, tablename)
  testInsert(db, tablename)
  testGetEqual(db, tablename)
  testFind(db, tablename)
  db.close()
end


describe Transactd do
  it 'create database' do
    testCreateDatabase(URL.encode('UTF-8'))
  end
  
  it 'table which has kanji-named field' do
    testWhole(1, 'kanji-field', URL.encode('UTF-8'))
  end
  
  it 'kanji-named table' do
    testWhole(2, '漢字テーブル', URL)
  end
  
  it 'create kanji-named database' do
    testCreateDatabase(URL_KANJI.encode('UTF-8')) # URL must be UTF-8
  end
 
  it 'table which has kanji-named field' do
    testWhole(1, 'kanji-field', URL_KANJI.encode('UTF-8'))
  end
 
  it 'kanji-named table' do
    testWhole(2, '漢字テーブル', URL_KANJI.encode('UTF-8'))
  end
  
  it 'drop database' do
    db = Transactd::Database.new()
    testDropDatabase(db, URL_KANJI.encode('UTF-8'))
  end
end
