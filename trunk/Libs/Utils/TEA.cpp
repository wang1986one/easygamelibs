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

#include "StdAfx.h"

void encrypt(unsigned int *v, const unsigned int *k,int Cycle) 
{
	unsigned int y=v[0], z=v[1], sum=0, i;         /* set up */
	unsigned int delta=0x9e3779b9;                 /* a key schedule constant */
	unsigned int a=k[0], b=k[1], c=k[2], d=k[3];   /* cache key */
	for (i=0; i < Cycle; i++) 
	{                        /* basic cycle start */
		sum += delta;
		y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
		z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);/* end cycle */
	}
	v[0]=y;
	v[1]=z;
 }
 
void decrypt(unsigned int *v, const unsigned int *k,int Cycle) 
{
	unsigned int y=v[0], z=v[1], sum=0, i; /* set up */
	unsigned int delta=0x9e3779b9;                  /* a key schedule constant */
	unsigned int a=k[0], b=k[1], c=k[2], d=k[3];    /* cache key */

	if (Cycle == 32)
		sum = 0xC6EF3720; /* delta << 5*/
	else if (Cycle == 16)
		sum = 0xE3779B90; /* delta << 4*/
	else
		sum = delta * Cycle;


	for(i=0; i<Cycle; i++) 
	{                            /* basic cycle start */
		 z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
		 y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
		 sum -= delta;                                /* end cycle */
	}
	v[0]=y;
	v[1]=z;
}

bool TEAEncode(const BYTE * pIn,BYTE * pOut,UINT DataLen,const BYTE * pKey,int Cycle)
{
	if((DataLen%8)!=0)
		return false;

	memcpy(pOut,pIn,DataLen);
	UINT BlockCount=DataLen/8;
	for(UINT i=0;i<BlockCount;i++)
	{
		encrypt((unsigned int *)(pOut+i*8),(const unsigned int *)pKey,Cycle);
	}
	return true;
}

bool TEADecode(const BYTE * pIn,BYTE * pOut,UINT DataLen,const BYTE * pKey,int Cycle)
{
	if((DataLen%8)!=0)
		return false;

	memcpy(pOut,pIn,DataLen);
	UINT BlockCount=DataLen/8;
	for(UINT i=0;i<BlockCount;i++)
	{
		decrypt((unsigned int *)(pOut+i*8),(const unsigned int *)pKey,Cycle);
	}
	return true;
}