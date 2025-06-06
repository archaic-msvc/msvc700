/*=--------------------------------------------------------------------------=
 * ocdbid.h
 *=--------------------------------------------------------------------------=
 * defines all the guids for OLE Controls Data Binding Interfaces interfaces.
 *
 * Copyright (c) 1994-2001 Microsoft Corporation, All Rights Reserved.
 *
 */

/* {9F6AA700-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_ICursor,
0x9f6aa700, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {ACFF0690-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_ICursorMove,
0xacff0690, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {BB87E420-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_ICursorScroll,
0xbb87e420, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {D14216A0-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_ICursorUpdateARow,
0xd14216a0, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {DB526CC0-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_INotifyDBEvents,
0xdb526cc0, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {E01D7850-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_ICursorFind,
0xe01d7850, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {E4D19810-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(IID_IEntryID,
0xe4d19810, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {E8BF1170-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(_GUID_NAMEONLY,
0xe8bf1170, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {F6304BB0-D188-11cd-AD48-00AA003C9CB6} */
DEFINE_GUID(_DBBMKGUID,
0xf6304bb0, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

/* {FE284700-D188-11cd-AD48-00AA003C9CB6} */
//DEFINE_GUID(_DBCIDGUID,
//0xfe284700, 0xd188, 0x11cd, 0xad, 0x48, 0x0, 0xaa, 0x0, 0x3c, 0x9c, 0xb6);

// OLEDB interface guids

/* {0x0c733a30,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ISequentialStream,
0x0c733a30,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a8c,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IAccessor,
0x0c733a8c,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a7c,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowset,
0x0c733a7c,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a55,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetInfo,
0x0c733a55,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a7d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetLocate,
0x0c733a7d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a84,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetResynch,
0x0c733a84,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a7e,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetScroll,
0x0c733a7e,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a05,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetChange,
0x0c733a05,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a6d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetUpdate,
0x0c733a6d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a09,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetIdentity,
0x0c733a09,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a83,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetNotify,
0x0c733a83,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a82,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetIndex,
0x0c733a82,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a63,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommand,
0x0c733a63,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a90,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IMultipleResults,
0x0c733a90,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a88,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IConvertType,
0x0c733a88,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a26,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandPrepare,
0x0c733a26,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a79,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandProperties,
0x0c733a79,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a27,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandText,
0x0c733a27,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a64,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandWithParameters,
0x0c733a64,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a10,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IColumnsRowset,
0x0c733a10,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a11,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IColumnsInfo,
0x0c733a11,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a1d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBCreateCommand,
0x0c733a1d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a5d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBCreateSession,
0x0c733a5d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a1e,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ISourcesRowset,
0x0c733a1e,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a8a,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBProperties,
0x0c733a8a,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a8b,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBInitialize,
0x0c733a8b,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a89,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBInfo,
0x0c733a89,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a7a,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBDataSourceAdmin,
0x0c733a7a,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a85,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ISessionProperties,
0x0c733a85,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a68,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IIndexDefinition,
0x0c733a68,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a86,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ITableDefinition,
0x0c733a86,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a69,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IOpenRowset,
0x0c733a69,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a7b,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBSchemaRowset,
0x0c733a7b,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a67,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IErrorRecords,
0x0c733a67,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a66,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IErrorLookup,
0x0c733a66,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a74,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ISQLErrorInfo,
0x0c733a74,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a75,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IGetDataSource,
0x0c733a75,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a5f,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ITransactionLocal,
0x0c733a5f,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a5e,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ITransactionJoin,
0x0c733a5e,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a60,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ITransactionObject,
0x0c733a60,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

//@@@+ V2.0
#if( OLEDBVER >= 0x0200 )

/* {0x0c733a7f,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetExactScroll,
0x0c733a7f,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a72,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetNextRowset,
0x0c733a72,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a71,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetNewRowAfter,
0x0c733a71,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a6e,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetWithParameters,
0x0c733a6e,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a0d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetFind,
0x0c733a0d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a0f,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetAsynch,
0x0c733a0f,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a12,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetKeys,
0x0c733a12,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a73,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetWatchAll,
0x0c733a73,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a44,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetWatchNotify,
0x0c733a44,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a45,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetWatchRegion,
0x0c733a45,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a6b,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IRowsetCopyRows,
0x0c733a6b,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a6a,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IReadData,
0x0c733a6a,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a4e,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandCost,
0x0c733a4e,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a87,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandTree,
0x0c733a87,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a18,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ICommandValidate,
0x0c733a18,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a51,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IQuery,
0x0c733a51,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a77,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_ITableRename,
0x0c733a77,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a50,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBSchemaCommand,
0x0c733a50,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a62,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IDBSecurityInfo,
0x0c733a62,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

/* {0x0c733a4d,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}} */
DEFINE_GUID(IID_IProvideMoniker,
0x0c733a4d,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);

#endif // OLEDBVER >= 0x0200
