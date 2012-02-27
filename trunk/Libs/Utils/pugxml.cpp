/****************************************************************************/
/*                                                                          */
/*      �ļ���:    pugxml.cpp                                      */
/*      ��������:  2012��02��02��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


//#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
//#undef new
//#endif

#include <sstream>

//#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
//#define new new( __FILE__, __LINE__ )
//#endif

int xml_parser::FILE_CHANNEL=FILE_CHANNEL_NORMAL1;

void xml_node::SaveToString(CEasyString& String)
{
#ifdef UNICODE
	std::wstringstream os;
#else
	std::stringstream os;		
#endif

	os << (*this);
	os<<'\0';

	String=os.str().c_str();

	String.Replace(_T("\n"),_T("\r\n"));

	
}
bool xml_node::SaveToFile(int FileChannel,LPCTSTR FileName)
{
	IFileAccessor * pFile=CFileSystemManager::GetInstance()->CreateFileAccessor(FileChannel);

	if(pFile==NULL)
		return false;

	if(pFile->Open(FileName,IFileAccessor::modeCreateAlways|IFileAccessor::modeWrite))
	{
		CEasyString XMLString;

		SaveToString(XMLString);

#ifdef UNICODE
		BYTE	UNICODEHeader[]={0xFF,0xFE};
		pFile->Write(UNICODEHeader,sizeof(UNICODEHeader));
#endif
		pFile->Write(XMLString.GetBuffer(),XMLString.GetLength()*sizeof(TCHAR));
		SAFE_RELEASE(pFile);
		return true;
	}
	else
	{
		SAFE_RELEASE(pFile);
		return false;
	}
}