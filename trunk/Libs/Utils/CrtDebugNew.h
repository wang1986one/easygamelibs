/****************************************************************************/
/*                                                                          */
/*      �ļ���:    CrtDebugNew.h                                            */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


extern void* operator new( size_t size, LPCSTR file, int line );
extern void operator delete( void* address, LPCSTR file, int line );
extern void* operator new[]( size_t size, LPCSTR file, int line );
extern void operator delete[]( void* address , LPCSTR file, int line );

#define new new( __FILE__, __LINE__ )

