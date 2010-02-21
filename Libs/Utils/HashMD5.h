#pragma once



class CHashMD5 
{
protected:
	/* typedef a 32 bit type */
	typedef unsigned long int UINT4;

	/* Data structure for MD5 (Message Digest) computation */
	typedef struct {
		UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
		UINT4 buf[4];                                    /* scratch buffer */
		unsigned char in[64];                              /* input buffer */
		unsigned char digest[16];     /* actual digest after MD5Final call */
	} MD5_CTX;

	MD5_CTX m_mdContext;

public:
	CHashMD5(void);
	~CHashMD5(void);
	
	void AddData(BYTE * inBuf, unsigned int inLen);
	void MD5Final(void);
	CEasyString GetHashCodeString(void);

protected:
	void Transform(UINT4 * buf, UINT4 * in);
};
