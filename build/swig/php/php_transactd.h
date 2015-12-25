/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.2
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef PHP_TRANSACTD_H
#define PHP_TRANSACTD_H

extern zend_module_entry transactd_module_entry;
#define phpext_transactd_ptr &transactd_module_entry

#ifdef PHP_WIN32
# define PHP_TRANSACTD_API __declspec(dllexport)
#else
# define PHP_TRANSACTD_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(transactd);
PHP_MSHUTDOWN_FUNCTION(transactd);
PHP_RINIT_FUNCTION(transactd);
PHP_RSHUTDOWN_FUNCTION(transactd);
PHP_MINFO_FUNCTION(transactd);

ZEND_NAMED_FUNCTION(_wrap_new_BOOKMARK);
ZEND_NAMED_FUNCTION(_wrap_BOOKMARK_isEmpty);
ZEND_NAMED_FUNCTION(_wrap_canRecoverNetError);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_all_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_all_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit0_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit0_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit1_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit1_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit2_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit2_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit3_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit3_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit4_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit4_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit5_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit5_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit6_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit6_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit7_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit7_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit8_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit8_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit9_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bit9_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitA_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitA_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitB_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitB_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitC_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitC_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitD_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitD_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitE_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitE_get);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitF_set);
ZEND_NAMED_FUNCTION(_wrap_FLAGS_bitF_get);
ZEND_NAMED_FUNCTION(_wrap_new_FLAGS);
ZEND_NAMED_FUNCTION(_wrap_keySegment_fieldNum_set);
ZEND_NAMED_FUNCTION(_wrap_keySegment_fieldNum_get);
ZEND_NAMED_FUNCTION(_wrap_keySegment_flags_set);
ZEND_NAMED_FUNCTION(_wrap_keySegment_flags_get);
ZEND_NAMED_FUNCTION(_wrap_new_keySegment);
ZEND_NAMED_FUNCTION(_wrap_keydef_segmentCount_set);
ZEND_NAMED_FUNCTION(_wrap_keydef_segmentCount_get);
ZEND_NAMED_FUNCTION(_wrap_keydef_segments_set);
ZEND_NAMED_FUNCTION(_wrap_keydef_segments_get);
ZEND_NAMED_FUNCTION(_wrap_keydef_keyNumber_set);
ZEND_NAMED_FUNCTION(_wrap_keydef_keyNumber_get);
ZEND_NAMED_FUNCTION(_wrap_keydef_segment);
ZEND_NAMED_FUNCTION(_wrap_new_keydef);
ZEND_NAMED_FUNCTION(_wrap_getTypeName);
ZEND_NAMED_FUNCTION(_wrap_getTypeAlign);
ZEND_NAMED_FUNCTION(_wrap_lenByCharnum);
ZEND_NAMED_FUNCTION(_wrap_isStringType);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_type_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_type_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_len_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_len_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_decimals_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_decimals_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_max_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_max_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_min_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_min_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_pos_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_pos_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_ddfid_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_ddfid_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_nullValue_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_nullValue_get);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_keylen_set);
ZEND_NAMED_FUNCTION(_wrap_fielddef_t_my_keylen_get);
ZEND_NAMED_FUNCTION(_wrap_new_fielddef_t_my);
ZEND_NAMED_FUNCTION(_wrap_fielddef_defaultValue);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setName);
ZEND_NAMED_FUNCTION(_wrap_fielddef_typeName);
ZEND_NAMED_FUNCTION(_wrap_fielddef_align);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setLenByCharnum);
ZEND_NAMED_FUNCTION(_wrap_fielddef_codePage);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isStringType);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isPadCharType);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isNumericType);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isDateTimeType);
ZEND_NAMED_FUNCTION(_wrap_fielddef_charNum);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isValidCharNum);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setCharsetIndex);
ZEND_NAMED_FUNCTION(_wrap_fielddef_charsetIndex);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isNullable);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setNullable);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setDefaultValue);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setTimeStampOnUpdate);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isTimeStampOnUpdate);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isDefaultNull);
ZEND_NAMED_FUNCTION(_wrap_fielddef_name);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isTrimPadChar);
ZEND_NAMED_FUNCTION(_wrap_fielddef_isUsePadChar);
ZEND_NAMED_FUNCTION(_wrap_fielddef_setPadCharSettings);
ZEND_NAMED_FUNCTION(_wrap_new_fielddef);
ZEND_NAMED_FUNCTION(_wrap_new_tabledef);
ZEND_NAMED_FUNCTION(_wrap_tabledef_cleanup);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fileName);
ZEND_NAMED_FUNCTION(_wrap_tabledef_tableName);
ZEND_NAMED_FUNCTION(_wrap_tabledef_setFileName);
ZEND_NAMED_FUNCTION(_wrap_tabledef_setTableName);
ZEND_NAMED_FUNCTION(_wrap_tabledef_nullfields);
ZEND_NAMED_FUNCTION(_wrap_tabledef_inUse);
ZEND_NAMED_FUNCTION(_wrap_tabledef_size);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fieldNumByName);
ZEND_NAMED_FUNCTION(_wrap_tabledef_recordlen);
ZEND_NAMED_FUNCTION(_wrap_tabledef_setValidationTarget);
ZEND_NAMED_FUNCTION(_wrap_tabledef_isMysqlNullMode);
ZEND_NAMED_FUNCTION(_wrap_tabledef_id_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_id_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_pageSize_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_pageSize_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_varSize_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_varSize_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_preAlloc_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_preAlloc_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fieldCount_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fieldCount_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_keyCount_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_keyCount_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_version_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_version_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_charsetIndex_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_charsetIndex_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_flags_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_flags_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_primaryKeyNum_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_primaryKeyNum_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_ddfid_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_ddfid_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fixedRecordLen_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fixedRecordLen_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_schemaCodePage_set);
ZEND_NAMED_FUNCTION(_wrap_tabledef_schemaCodePage_get);
ZEND_NAMED_FUNCTION(_wrap_tabledef_fieldDef);
ZEND_NAMED_FUNCTION(_wrap_tabledef_keyDef);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_majorVersion_set);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_majorVersion_get);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_minorVersion_set);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_minorVersion_get);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_type_set);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_type_get);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_moduleVersionShortString);
ZEND_NAMED_FUNCTION(_wrap_btrVersion_moduleTypeString);
ZEND_NAMED_FUNCTION(_wrap_new_btrVersion);
ZEND_NAMED_FUNCTION(_wrap_btrVersions_versions_set);
ZEND_NAMED_FUNCTION(_wrap_btrVersions_versions_get);
ZEND_NAMED_FUNCTION(_wrap_btrVersions_version);
ZEND_NAMED_FUNCTION(_wrap_new_btrVersions);
ZEND_NAMED_FUNCTION(_wrap_getFilterLogicTypeCode);
ZEND_NAMED_FUNCTION(_wrap_nstable_nsdb);
ZEND_NAMED_FUNCTION(_wrap_nstable_tableid);
ZEND_NAMED_FUNCTION(_wrap_nstable_setTableid);
ZEND_NAMED_FUNCTION(_wrap_nstable_isOpen);
ZEND_NAMED_FUNCTION(_wrap_nstable_isUseTransactd);
ZEND_NAMED_FUNCTION(_wrap_nstable_setAccessRights);
ZEND_NAMED_FUNCTION(_wrap_nstable_datalen);
ZEND_NAMED_FUNCTION(_wrap_nstable_stat);
ZEND_NAMED_FUNCTION(_wrap_nstable_keyNum);
ZEND_NAMED_FUNCTION(_wrap_nstable_setKeyNum);
ZEND_NAMED_FUNCTION(_wrap_nstable_canRead);
ZEND_NAMED_FUNCTION(_wrap_nstable_canWrite);
ZEND_NAMED_FUNCTION(_wrap_nstable_canInsert);
ZEND_NAMED_FUNCTION(_wrap_nstable_canDelete);
ZEND_NAMED_FUNCTION(_wrap_nstable_getWriteImageLen);
ZEND_NAMED_FUNCTION(_wrap_nstable_close);
ZEND_NAMED_FUNCTION(_wrap_nstable_update);
ZEND_NAMED_FUNCTION(_wrap_nstable_del);
ZEND_NAMED_FUNCTION(_wrap_nstable_insert);
ZEND_NAMED_FUNCTION(_wrap_nstable_createIndex);
ZEND_NAMED_FUNCTION(_wrap_nstable_dropIndex);
ZEND_NAMED_FUNCTION(_wrap_nstable_recordCount);
ZEND_NAMED_FUNCTION(_wrap_nstable_beginBulkInsert);
ZEND_NAMED_FUNCTION(_wrap_nstable_abortBulkInsert);
ZEND_NAMED_FUNCTION(_wrap_nstable_commitBulkInsert);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekFirst);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekLast);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekPrev);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekNext);
ZEND_NAMED_FUNCTION(_wrap_nstable_seek);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekGreater);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekLessThan);
ZEND_NAMED_FUNCTION(_wrap_nstable_stepFirst);
ZEND_NAMED_FUNCTION(_wrap_nstable_stepLast);
ZEND_NAMED_FUNCTION(_wrap_nstable_stepPrev);
ZEND_NAMED_FUNCTION(_wrap_nstable_stepNext);
ZEND_NAMED_FUNCTION(_wrap_nstable_bookmarkLen);
ZEND_NAMED_FUNCTION(_wrap_nstable_bookmark);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekByBookmark);
ZEND_NAMED_FUNCTION(_wrap_nstable_getPercentage);
ZEND_NAMED_FUNCTION(_wrap_nstable_seekByPercentage);
ZEND_NAMED_FUNCTION(_wrap_nstable_setOwnerName);
ZEND_NAMED_FUNCTION(_wrap_nstable_clearOwnerName);
ZEND_NAMED_FUNCTION(_wrap_nstable_recordLength);
ZEND_NAMED_FUNCTION(_wrap_nstable_stats);
ZEND_NAMED_FUNCTION(_wrap_nstable_unlock);
ZEND_NAMED_FUNCTION(_wrap_nstable_mode);
ZEND_NAMED_FUNCTION(_wrap_nstable_setTimestampMode);
ZEND_NAMED_FUNCTION(_wrap_nstable_getFileName);
ZEND_NAMED_FUNCTION(_wrap_nstable_statMsg);
ZEND_NAMED_FUNCTION(_wrap_nstable_getDirURI);
ZEND_NAMED_FUNCTION(_wrap_nstable_existsFile);
ZEND_NAMED_FUNCTION(_wrap_dbdef_tableCount);
ZEND_NAMED_FUNCTION(_wrap_dbdef_openMode);
ZEND_NAMED_FUNCTION(_wrap_dbdef_tableDefs);
ZEND_NAMED_FUNCTION(_wrap_dbdef_setVersion);
ZEND_NAMED_FUNCTION(_wrap_dbdef_version);
ZEND_NAMED_FUNCTION(_wrap_dbdef_stat);
ZEND_NAMED_FUNCTION(_wrap_dbdef_validateTableDef);
ZEND_NAMED_FUNCTION(_wrap_dbdef_updateTableDef);
ZEND_NAMED_FUNCTION(_wrap_dbdef_insertField);
ZEND_NAMED_FUNCTION(_wrap_dbdef_deleteField);
ZEND_NAMED_FUNCTION(_wrap_dbdef_insertKey);
ZEND_NAMED_FUNCTION(_wrap_dbdef_deleteKey);
ZEND_NAMED_FUNCTION(_wrap_dbdef_insertTable);
ZEND_NAMED_FUNCTION(_wrap_dbdef_deleteTable);
ZEND_NAMED_FUNCTION(_wrap_dbdef_renumberTable);
ZEND_NAMED_FUNCTION(_wrap_dbdef_tableNumByName);
ZEND_NAMED_FUNCTION(_wrap_dbdef_findKeynumByFieldNum);
ZEND_NAMED_FUNCTION(_wrap_dbdef_fieldNumByName);
ZEND_NAMED_FUNCTION(_wrap_dbdef_fieldValidLength);
ZEND_NAMED_FUNCTION(_wrap_dbdef_synchronizeSeverSchema);
ZEND_NAMED_FUNCTION(_wrap_dbdef_statMsg);
ZEND_NAMED_FUNCTION(_wrap_dbdef_reopen);
ZEND_NAMED_FUNCTION(_wrap_dbdef_mode);
ZEND_NAMED_FUNCTION(_wrap_table_tableDef);
ZEND_NAMED_FUNCTION(_wrap_table_valiableFormatType);
ZEND_NAMED_FUNCTION(_wrap_table_blobFieldUsed);
ZEND_NAMED_FUNCTION(_wrap_table_logicalToString);
ZEND_NAMED_FUNCTION(_wrap_table_setLogicalToString);
ZEND_NAMED_FUNCTION(_wrap_table_optionalData);
ZEND_NAMED_FUNCTION(_wrap_table_setOptionalData);
ZEND_NAMED_FUNCTION(_wrap_table_myDateTimeValueByBtrv);
ZEND_NAMED_FUNCTION(_wrap_table_bookmarksCount);
ZEND_NAMED_FUNCTION(_wrap_table_moveBookmarks);
ZEND_NAMED_FUNCTION(_wrap_table_bookmarks);
ZEND_NAMED_FUNCTION(_wrap_table_clearBuffer);
ZEND_NAMED_FUNCTION(_wrap_table_getRecordHash);
ZEND_NAMED_FUNCTION(_wrap_table_smartUpdate);
ZEND_NAMED_FUNCTION(_wrap_table_find);
ZEND_NAMED_FUNCTION(_wrap_table_findFirst);
ZEND_NAMED_FUNCTION(_wrap_table_findLast);
ZEND_NAMED_FUNCTION(_wrap_table_findNext);
ZEND_NAMED_FUNCTION(_wrap_table_findPrev);
ZEND_NAMED_FUNCTION(_wrap_table_statReasonOfFind);
ZEND_NAMED_FUNCTION(_wrap_table_lastFindDirection);
ZEND_NAMED_FUNCTION(_wrap_table_bookmarkFindCurrent);
ZEND_NAMED_FUNCTION(_wrap_table_setFilter);
ZEND_NAMED_FUNCTION(_wrap_table_fieldNumByName);
ZEND_NAMED_FUNCTION(_wrap_table_getFVint);
ZEND_NAMED_FUNCTION(_wrap_table_getFV64);
ZEND_NAMED_FUNCTION(_wrap_table_getFVdbl);
ZEND_NAMED_FUNCTION(_wrap_table_getFVstr);
ZEND_NAMED_FUNCTION(_wrap_table_fields);
ZEND_NAMED_FUNCTION(_wrap_table_getFVNull);
ZEND_NAMED_FUNCTION(_wrap_table_setFVNull);
ZEND_NAMED_FUNCTION(_wrap_table_setFV);
ZEND_NAMED_FUNCTION(_wrap_table_getFVbin);
ZEND_NAMED_FUNCTION(_wrap_table_keyValueDescription);
ZEND_NAMED_FUNCTION(_wrap_table_prepare);
ZEND_NAMED_FUNCTION(_wrap_table_setQuery);
ZEND_NAMED_FUNCTION(_wrap_table_setPrepare);
ZEND_NAMED_FUNCTION(_wrap_table_release);
ZEND_NAMED_FUNCTION(_wrap_queryBase_clearSeekKeyValues);
ZEND_NAMED_FUNCTION(_wrap_queryBase_clearSelectFields);
ZEND_NAMED_FUNCTION(_wrap_queryBase_addSeekKeyValue);
ZEND_NAMED_FUNCTION(_wrap_queryBase_addSeekBookmark);
ZEND_NAMED_FUNCTION(_wrap_queryBase_reserveSeekKeyValueSize);
ZEND_NAMED_FUNCTION(_wrap_queryBase_queryString);
ZEND_NAMED_FUNCTION(_wrap_queryBase_reject);
ZEND_NAMED_FUNCTION(_wrap_queryBase_limit);
ZEND_NAMED_FUNCTION(_wrap_queryBase_direction);
ZEND_NAMED_FUNCTION(_wrap_queryBase_all);
ZEND_NAMED_FUNCTION(_wrap_queryBase_optimize);
ZEND_NAMED_FUNCTION(_wrap_queryBase_bookmarkAlso);
ZEND_NAMED_FUNCTION(_wrap_queryBase_toString);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getDirection);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getReject);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getLimit);
ZEND_NAMED_FUNCTION(_wrap_queryBase_isAll);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getJoinKeySize);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getOptimize);
ZEND_NAMED_FUNCTION(_wrap_queryBase_isBookmarkAlso);
ZEND_NAMED_FUNCTION(_wrap_queryBase_selectCount);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getSelect);
ZEND_NAMED_FUNCTION(_wrap_queryBase_whereTokens);
ZEND_NAMED_FUNCTION(_wrap_queryBase_getWhereToken);
ZEND_NAMED_FUNCTION(_wrap_queryBase_setWhereToken);
ZEND_NAMED_FUNCTION(_wrap_queryBase_reverseAliasName);
ZEND_NAMED_FUNCTION(_wrap_queryBase_joinKeySize);
ZEND_NAMED_FUNCTION(_wrap_queryBase_stopAtLimit);
ZEND_NAMED_FUNCTION(_wrap_queryBase_isStopAtLimit);
ZEND_NAMED_FUNCTION(_wrap_queryBase_isSeekByBookmarks);
ZEND_NAMED_FUNCTION(_wrap_query_reset);
ZEND_NAMED_FUNCTION(_wrap_query_select);
ZEND_NAMED_FUNCTION(_wrap_query_whereIsNull);
ZEND_NAMED_FUNCTION(_wrap_query_whereIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_query_andIsNull);
ZEND_NAMED_FUNCTION(_wrap_query_andIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_query_orIsNull);
ZEND_NAMED_FUNCTION(_wrap_query_orIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_new_query);
ZEND_NAMED_FUNCTION(_wrap_query_where);
ZEND_NAMED_FUNCTION(_wrap_query_and_);
ZEND_NAMED_FUNCTION(_wrap_query_or_);
ZEND_NAMED_FUNCTION(_wrap_query_in);
ZEND_NAMED_FUNCTION(_wrap_new_nsdatabase);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_enableTrn);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_stat);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_clientID);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_openTableCount);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_uri);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_uriMode);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_lockWaitCount);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_lockWaitTime);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setLockWaitCount);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setLockWaitTime);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setLocalSharing);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_dropTable);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_rename);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_swapTablename);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_beginTrn);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_endTrn);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_abortTrn);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_beginSnapshot);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_endSnapshot);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_trxIsolationServer);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_trxLockWaitTimeoutServer);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_statMsg);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_useLongFilename);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setUseLongFilename);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setUseTransactd);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_isTransactdUri);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_isUseTransactd);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_readDatabaseDirectory);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_connect);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_disconnect);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_disconnectForReconnectTest);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_reconnect);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_trnsactionFlushWaitStatus);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setExecCodePage);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_execCodePage);
ZEND_NAMED_FUNCTION(_wrap_nsdatabase_setCheckTablePtr);
ZEND_NAMED_FUNCTION(_wrap_database_dbDef);
ZEND_NAMED_FUNCTION(_wrap_database_rootDir);
ZEND_NAMED_FUNCTION(_wrap_database_setRootDir);
ZEND_NAMED_FUNCTION(_wrap_database_optionalData);
ZEND_NAMED_FUNCTION(_wrap_database_setOptionalData);
ZEND_NAMED_FUNCTION(_wrap_database_tableReadOnly);
ZEND_NAMED_FUNCTION(_wrap_database_setTableReadOnly);
ZEND_NAMED_FUNCTION(_wrap_database_open);
ZEND_NAMED_FUNCTION(_wrap_database___clone);
ZEND_NAMED_FUNCTION(_wrap_database_createTable);
ZEND_NAMED_FUNCTION(_wrap_database_getSqlStringForCreateTable);
ZEND_NAMED_FUNCTION(_wrap_database_create);
ZEND_NAMED_FUNCTION(_wrap_database_drop);
ZEND_NAMED_FUNCTION(_wrap_database_dropTable);
ZEND_NAMED_FUNCTION(_wrap_database_close);
ZEND_NAMED_FUNCTION(_wrap_database_aclReload);
ZEND_NAMED_FUNCTION(_wrap_database_continuous);
ZEND_NAMED_FUNCTION(_wrap_database_assignSchemaData);
ZEND_NAMED_FUNCTION(_wrap_database_copyTableData);
ZEND_NAMED_FUNCTION(_wrap_database_convertTable);
ZEND_NAMED_FUNCTION(_wrap_database_existsTableFile);
ZEND_NAMED_FUNCTION(_wrap_database_getBtrVersion);
ZEND_NAMED_FUNCTION(_wrap_database_isOpened);
ZEND_NAMED_FUNCTION(_wrap_database_mode);
ZEND_NAMED_FUNCTION(_wrap_database_autoSchemaUseNullkey);
ZEND_NAMED_FUNCTION(_wrap_database_setAutoSchemaUseNullkey);
ZEND_NAMED_FUNCTION(_wrap_database_setCompatibleMode);
ZEND_NAMED_FUNCTION(_wrap_database_compatibleMode);
ZEND_NAMED_FUNCTION(_wrap_new_database);
ZEND_NAMED_FUNCTION(_wrap_database_openTable);
ZEND_NAMED_FUNCTION(_wrap_benchmark_start);
ZEND_NAMED_FUNCTION(_wrap_benchmark_stop);
ZEND_NAMED_FUNCTION(_wrap_benchmark_showTimes);
ZEND_NAMED_FUNCTION(_wrap_benchmark_showTimeSec);
ZEND_NAMED_FUNCTION(_wrap_new_benchmark);
ZEND_NAMED_FUNCTION(_wrap_charsize);
ZEND_NAMED_FUNCTION(_wrap_charsetName);
ZEND_NAMED_FUNCTION(_wrap_charsetIndex);
ZEND_NAMED_FUNCTION(_wrap_codePage);
ZEND_NAMED_FUNCTION(_wrap_btrDate_dd_set);
ZEND_NAMED_FUNCTION(_wrap_btrDate_dd_get);
ZEND_NAMED_FUNCTION(_wrap_btrDate_mm_set);
ZEND_NAMED_FUNCTION(_wrap_btrDate_mm_get);
ZEND_NAMED_FUNCTION(_wrap_btrDate_yy_set);
ZEND_NAMED_FUNCTION(_wrap_btrDate_yy_get);
ZEND_NAMED_FUNCTION(_wrap_btrDate_i_set);
ZEND_NAMED_FUNCTION(_wrap_btrDate_i_get);
ZEND_NAMED_FUNCTION(_wrap_new_btrDate);
ZEND_NAMED_FUNCTION(_wrap_btrTime_uu_set);
ZEND_NAMED_FUNCTION(_wrap_btrTime_uu_get);
ZEND_NAMED_FUNCTION(_wrap_btrTime_ss_set);
ZEND_NAMED_FUNCTION(_wrap_btrTime_ss_get);
ZEND_NAMED_FUNCTION(_wrap_btrTime_nn_set);
ZEND_NAMED_FUNCTION(_wrap_btrTime_nn_get);
ZEND_NAMED_FUNCTION(_wrap_btrTime_hh_set);
ZEND_NAMED_FUNCTION(_wrap_btrTime_hh_get);
ZEND_NAMED_FUNCTION(_wrap_btrTime_i_set);
ZEND_NAMED_FUNCTION(_wrap_btrTime_i_get);
ZEND_NAMED_FUNCTION(_wrap_new_btrTime);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_time_set);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_time_get);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_date_set);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_date_get);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_i64_set);
ZEND_NAMED_FUNCTION(_wrap_btrDateTime_i64_get);
ZEND_NAMED_FUNCTION(_wrap_new_btrDateTime);
ZEND_NAMED_FUNCTION(_wrap_btrTimeStamp_i64_set);
ZEND_NAMED_FUNCTION(_wrap_btrTimeStamp_i64_get);
ZEND_NAMED_FUNCTION(_wrap_new_btrTimeStamp);
ZEND_NAMED_FUNCTION(_wrap_btrTimeStamp_toString);
ZEND_NAMED_FUNCTION(_wrap_btrTimeStamp_fromString);
ZEND_NAMED_FUNCTION(_wrap_atobtrd);
ZEND_NAMED_FUNCTION(_wrap_atobtrt);
ZEND_NAMED_FUNCTION(_wrap_btrdtoa);
ZEND_NAMED_FUNCTION(_wrap_btrttoa);
ZEND_NAMED_FUNCTION(_wrap_btrstoa);
ZEND_NAMED_FUNCTION(_wrap_atobtrs);
ZEND_NAMED_FUNCTION(_wrap_getNowDate);
ZEND_NAMED_FUNCTION(_wrap_getNowTime);
ZEND_NAMED_FUNCTION(_wrap_fielddefs___clone);
ZEND_NAMED_FUNCTION(_wrap_fielddefs_indexByName);
ZEND_NAMED_FUNCTION(_wrap_fielddefs_getFielddef);
ZEND_NAMED_FUNCTION(_wrap_fielddefs_checkIndex);
ZEND_NAMED_FUNCTION(_wrap_fielddefs_size);
ZEND_NAMED_FUNCTION(_wrap_new_fielddefs);
ZEND_NAMED_FUNCTION(_wrap_new_field);
ZEND_NAMED_FUNCTION(_wrap_field_type);
ZEND_NAMED_FUNCTION(_wrap_field_len);
ZEND_NAMED_FUNCTION(_wrap_field_c_str);
//ZEND_NAMED_FUNCTION(_wrap_field_a_str);
ZEND_NAMED_FUNCTION(_wrap_field_i);
//ZEND_NAMED_FUNCTION(_wrap_field_i8);
//ZEND_NAMED_FUNCTION(_wrap_field_i16);
ZEND_NAMED_FUNCTION(_wrap_field_i64);
//ZEND_NAMED_FUNCTION(_wrap_field_f);
ZEND_NAMED_FUNCTION(_wrap_field_d);
ZEND_NAMED_FUNCTION(_wrap_field_isNull);
ZEND_NAMED_FUNCTION(_wrap_field_setNull);
ZEND_NAMED_FUNCTION(_wrap_field_setFV);
ZEND_NAMED_FUNCTION(_wrap_field_getBin);
ZEND_NAMED_FUNCTION(_wrap_field_comp);
ZEND_NAMED_FUNCTION(_wrap_Record_isInvalidRecord);
ZEND_NAMED_FUNCTION(_wrap_Record_getField);
ZEND_NAMED_FUNCTION(_wrap_Record_size);
ZEND_NAMED_FUNCTION(_wrap_Record_indexByName);
ZEND_NAMED_FUNCTION(_wrap_Record_fieldDefs);
ZEND_NAMED_FUNCTION(_wrap_Record_clear);
ZEND_NAMED_FUNCTION(_wrap_Record_getFieldByIndexRef);
ZEND_NAMED_FUNCTION(_wrap_Record_getFieldByNameRef);
ZEND_NAMED_FUNCTION(_wrap_memoryRecord_createRecord);
ZEND_NAMED_FUNCTION(_wrap_new_memoryRecord);
ZEND_NAMED_FUNCTION(_wrap_writableRecord_read);
ZEND_NAMED_FUNCTION(_wrap_writableRecord_insert);
ZEND_NAMED_FUNCTION(_wrap_writableRecord_del);
ZEND_NAMED_FUNCTION(_wrap_writableRecord_update);
ZEND_NAMED_FUNCTION(_wrap_writableRecord_save);
ZEND_NAMED_FUNCTION(_wrap_new_connectParams);
ZEND_NAMED_FUNCTION(_wrap_connectParams_setMode);
ZEND_NAMED_FUNCTION(_wrap_connectParams_setType);
ZEND_NAMED_FUNCTION(_wrap_connectParams_uri);
ZEND_NAMED_FUNCTION(_wrap_connectParams_mode);
ZEND_NAMED_FUNCTION(_wrap_connectParams_type);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_reset);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_keyField);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_count);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_getFieldName);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_getValue);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_addValue);
ZEND_NAMED_FUNCTION(_wrap_fieldNames_addValues);
ZEND_NAMED_FUNCTION(_wrap_new_fieldNames);
ZEND_NAMED_FUNCTION(_wrap_sortField_name_set);
ZEND_NAMED_FUNCTION(_wrap_sortField_name_get);
ZEND_NAMED_FUNCTION(_wrap_sortField_asc_set);
ZEND_NAMED_FUNCTION(_wrap_sortField_asc_get);
ZEND_NAMED_FUNCTION(_wrap_new_sortField);
ZEND_NAMED_FUNCTION(_wrap_sortFields_add);
ZEND_NAMED_FUNCTION(_wrap_sortFields_size);
ZEND_NAMED_FUNCTION(_wrap_sortFields_getSortField);
ZEND_NAMED_FUNCTION(_wrap_sortFields_clear);
ZEND_NAMED_FUNCTION(_wrap_new_sortFields);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_reset);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_whenIsNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_whenIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_andIsNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_andIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_orIsNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_orIsNotNull);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_toString);
ZEND_NAMED_FUNCTION(_wrap_new_recordsetQuery);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_when);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_and_);
ZEND_NAMED_FUNCTION(_wrap_recordsetQuery_or_);
ZEND_NAMED_FUNCTION(_wrap_groupFuncBase_targetNames);
ZEND_NAMED_FUNCTION(_wrap_groupFuncBase_resultName);
ZEND_NAMED_FUNCTION(_wrap_groupFuncBase_setResultName);
ZEND_NAMED_FUNCTION(_wrap_groupFuncBase_resultKey);
ZEND_NAMED_FUNCTION(_wrap_groupFuncBase_reset);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_reset);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_addFunction);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_keyField);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_getKeyFields);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_getFunction);
ZEND_NAMED_FUNCTION(_wrap_groupQuery_functionCount);
ZEND_NAMED_FUNCTION(_wrap_new_groupQuery);
ZEND_NAMED_FUNCTION(_wrap_new_sum);
ZEND_NAMED_FUNCTION(_wrap_new_first);
ZEND_NAMED_FUNCTION(_wrap_new_last);
ZEND_NAMED_FUNCTION(_wrap_new_count);
ZEND_NAMED_FUNCTION(_wrap_new_avg);
ZEND_NAMED_FUNCTION(_wrap_new_min);
ZEND_NAMED_FUNCTION(_wrap_new_max);
ZEND_NAMED_FUNCTION(_wrap_Recordset___clone);
ZEND_NAMED_FUNCTION(_wrap_Recordset_getRecord);
ZEND_NAMED_FUNCTION(_wrap_Recordset_size);
ZEND_NAMED_FUNCTION(_wrap_Recordset_count);
ZEND_NAMED_FUNCTION(_wrap_Recordset_clearRecords);
ZEND_NAMED_FUNCTION(_wrap_Recordset_fieldDefs);
ZEND_NAMED_FUNCTION(_wrap_Recordset_clear);
ZEND_NAMED_FUNCTION(_wrap_Recordset_top);
ZEND_NAMED_FUNCTION(_wrap_Recordset_begin);
ZEND_NAMED_FUNCTION(_wrap_Recordset_end);
ZEND_NAMED_FUNCTION(_wrap_Recordset_erase);
ZEND_NAMED_FUNCTION(_wrap_Recordset_removeField);
ZEND_NAMED_FUNCTION(_wrap_Recordset_matchBy);
ZEND_NAMED_FUNCTION(_wrap_Recordset_groupBy);
ZEND_NAMED_FUNCTION(_wrap_Recordset_orderBy);
ZEND_NAMED_FUNCTION(_wrap_Recordset_reverse);
ZEND_NAMED_FUNCTION(_wrap_Recordset_appendField);
ZEND_NAMED_FUNCTION(_wrap_Recordset_unionRecordset);
ZEND_NAMED_FUNCTION(_wrap_Recordset_getRow);
ZEND_NAMED_FUNCTION(_wrap_new_Recordset);
ZEND_NAMED_FUNCTION(_wrap_new_preparedQuery);
ZEND_NAMED_FUNCTION(_wrap_preparedQuery_supplyValue);
ZEND_NAMED_FUNCTION(_wrap_preparedQuery_addValue);
ZEND_NAMED_FUNCTION(_wrap_preparedQuery_resetAddIndex);
ZEND_NAMED_FUNCTION(_wrap_activeTable_alias);
ZEND_NAMED_FUNCTION(_wrap_activeTable_resetAlias);
ZEND_NAMED_FUNCTION(_wrap_activeTable_getWritableRecord);
ZEND_NAMED_FUNCTION(_wrap_activeTable_index);
ZEND_NAMED_FUNCTION(_wrap_activeTable_option);
ZEND_NAMED_FUNCTION(_wrap_activeTable_read);
ZEND_NAMED_FUNCTION(_wrap_activeTable_readMore);
ZEND_NAMED_FUNCTION(_wrap_activeTable_prepare);
ZEND_NAMED_FUNCTION(_wrap_activeTable_join);
ZEND_NAMED_FUNCTION(_wrap_activeTable_outerJoin);
ZEND_NAMED_FUNCTION(_wrap_new_activeTable);
ZEND_NAMED_FUNCTION(_wrap_activeTable_release);
ZEND_NAMED_FUNCTION(_wrap_activeTable_table);
ZEND_NAMED_FUNCTION(_wrap_activeTable_keyValue);
ZEND_NAMED_FUNCTION(_wrap_new_pooledDbManager);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_isUseXa);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_setUseXa);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_c_use);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_unUse);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_reset);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_db);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_uri);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_mode);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_isOpened);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_setOption);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_option);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_beginTrn);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_endTrn);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_abortTrn);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_enableTrn);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_beginSnapshot);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_endSnapshot);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_stat);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_clientID);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_setMaxConnections);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_maxConnections);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_reserve);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_usingCount);
ZEND_NAMED_FUNCTION(_wrap_pooledDbManager_table);
ZEND_NAMED_FUNCTION(_wrap_new_fieldsBase_p_p);
ZEND_NAMED_FUNCTION(_wrap_copy_fieldsBase_p_p);
ZEND_NAMED_FUNCTION(_wrap_delete_fieldsBase_p_p);
ZEND_NAMED_FUNCTION(_wrap_fieldsBase_p_p_assign);
ZEND_NAMED_FUNCTION(_wrap_fieldsBase_p_p_value);
#endif /* PHP_TRANSACTD_H */
