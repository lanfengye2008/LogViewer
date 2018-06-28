#pragma once

//#ifndef UNICODE
//#  error Ŀǰֻ֧��Unicode����
//#endif

#include "..\LogViewerDefine.h"
#include "LogViewerConfig.h"

enum LogItemContentType  //��������
{
    type_Sequence = 0,
    type_ProcessId,
    type_ThreadId,
    type_Time,
    type_ElapseTime,
	type_TraceLevel,
    type_ModuleName,
	type_FunName,
    type_FileName,
    type_TraceInfo,

    type_SortCount
};

enum FilterType {
	ftAll,
	ftAny,
	ftNone
};
struct SortContent
{
    LogItemContentType  contentType;
    BOOL                bSortAscending;
};

struct LogItemFilter;
class CLogManager
{
    friend struct LogItemFilter;
public:
    CLogManager(void);
    ~CLogManager(void);
    LONG GenerateLog(LPCTSTR pszFilePath, LONG logCount);
    BOOL SetLogFiles(const CStringArray &logFilePaths);
    BOOL ClearAllLogItems();
	BOOL ReloadLogItems();
    LONG GetLogFileCount() const;
	LONG GetProcessCount() const;
    LONG GetThreadCount() const;
	LONG GetSelectedThreadCount() const;

    //! bAll Ϊ TRUE ��ʾ�������е���־
    //! bAll Ϊ FALSE ��ʾ������ʾ����־(������ʾ��˳����б���)
    BOOL SaveLogItems(LPCTSTR pszFilePath, BOOL bAll = FALSE);

    LONG GetDisplayLogItemCount() const;
    const LogItemPointer GetDisplayLogItem(LONG index) const;
	void setActiveItemIndex(LONG index);
	CString getActiveItemTraceInfo();

	LONG GetThreadIds(AllThreadIdContainer & allThreadIds) const;
	LONG GetProcessIds(AllProcessIdContainer & allProcessIds) const;
    BOOL SortDisplayItem(LogItemContentType SortContentType, BOOL bSortAscending);

    //�Ƿ�ѡ�� -- ���й���
	BOOL SetProcessIdChecked(PROCESS_ID_TYPE processId, BOOL bChecked);
	BOOL IsProcessIdChecked(PROCESS_ID_TYPE processId) const;

	BOOL SetThreadIdChecked(THREAD_ID_TYPE threadId, BOOL bChecked);
    BOOL IsThreadIdChecked(THREAD_ID_TYPE threadId) const;
    BOOL SetTraceLevelDisplay(TraceLevel level, BOOL bDisplay);
    

    BOOL DoFilterLogItems();
	void SetCodepage(UINT codepage){ m_codePage = codepage; }
    BOOL SetLogInfoFilterString(LPCTSTR pszFilterString, FilterType filterType);
    CLogViewerConfig	m_logConfig;
protected:
    typedef std::vector<LogItemPointer>     LogItemArrayType;
    typedef LogItemArrayType::iterator      LogItemArrayIterator;
    typedef std::list<LogItemPointer>       LogItemListType;
    //typedef std::back_insert_iterator< LogItemArrayType > back_ins_itr;

	UINT	m_codePage;
    mutable CFCriticalSection   m_CsLockObj;
	CStringArray				m_logFilePaths;
    std::list<LogItemPointer>   m_allInitLogItems;  //��ʼ��ʱ������ȡLogItem����ֹʹ��vectorʱƵ�������ڴ�
    LogItemArrayType        m_AllLogItems;
    LogItemArrayType        m_DisplayLogItems;
    AllThreadIdContainer         m_AllLogThreadIds;
	std::map<THREAD_ID_TYPE, BOOL>    m_AllLogThreadIdsChecked;

	AllProcessIdContainer		 m_AllLogProcessIds;
	std::map<PROCESS_ID_TYPE, BOOL>    m_AllLogProcessIdsChecked;

    typedef  std::map<PIdTIdType, LONGLONG>  ThreadExecuteTimeContainer;  //����ID+�߳�ID,��һ�ε�ʱ��ֵ
    ThreadExecuteTimeContainer m_threadExecuteTimes;

    CString             m_strLogInfoFilterString;
	FilterType          m_filterType;
    //BOOL                m_isIncludeText;
    LONG                m_fileCount;
	LONG                m_activeItemIndex;
    //�����˰��ն�������������� -- ���ж���߳��Ҷ��߳�ID����ʱ������������������־��������
    //�Ƿ�Ӧ����UI�����ӱ�ʾ��
    SortContent m_SortContents[type_SortCount];
    BOOL        m_TraceLevelDisplay[tlEnd];         ///< ����Filter
protected:
    //typedef BOOL (__thiscall * ReadLogFileProc)(CLogManager*pThis, LPCTSTR pszFilePath);

    BOOL CheckFTLogFiles(const CStringArray &logFilePaths);
    BOOL ReadFTLogFile(LPCTSTR pszFilePath);
    LogItemPointer ParseFTLLogItem(CString& strLogItem);

    //LogItemPointer ParseTraceLog(std::string& strOneLog);
    BOOL ReadTraceLogFile(LPCTSTR pszFilePath);

	LogItemPointer ParseRegularTraceLog(std::string& strOneLog, const std::tr1::regex& reg, const LogItemPointer& preLogItem);
    BOOL _CalcThreadElpaseTime(LogItemArrayType& logItems);
	int _ConvertItemInfo(const std::string& srcInfo, LPCTSTR& pszDest, UINT codePage);
};