#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const CString& strFullPath);
	static void ExtractDirectory(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoLst);
	static int ExtractFileCount(const TCHAR* pFullPath);
};

