/****************************************************************************/
/*                                                                          */
/*      �ļ���:    TEA.h                                                */
/*      ��������:  2014��1��2��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

extern bool TEAEncode(const BYTE * pIn,BYTE * pOut,UINT DataLen,const BYTE * pKey,int Cycle);
extern bool TEADecode(const BYTE * pIn,BYTE * pOut,UINT DataLen,const BYTE * pKey,int Cycle);