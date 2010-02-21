#pragma once

struct FLOAT_RECT
{
	FLOAT left,top,right,bottom;

	FLOAT_RECT()
	{
		top=0.0f;
		left=0.0f;
		right=0.0f;
		bottom=0.0f;
	}
	FLOAT_RECT(FLOAT l,FLOAT t,FLOAT r,FLOAT b)
	{
		top=t;
		left=l;
		right=r;
		bottom=b;
	}
	FLOAT_RECT(long l,long t,long r,long b)
	{
		top=(FLOAT)t;
		left=(FLOAT)l;
		right=(FLOAT)r;
		bottom=(FLOAT)b;
	}
	FLOAT_RECT(int l,int t,int r,int b)
	{
		top=(FLOAT)t;
		left=(FLOAT)l;
		right=(FLOAT)r;
		bottom=(FLOAT)b;
	}
	FLOAT_RECT(RECT& rect)
	{
		top=(FLOAT)rect.top;
		left=(FLOAT)rect.left;
		right=(FLOAT)rect.right;
		bottom=(FLOAT)rect.bottom;
	}
	FLOAT_RECT(FLOAT_RECT& rect)
	{
		top=rect.top;
		left=rect.left;
		right=rect.right;
		bottom=rect.bottom;
	}	
	operator RECT()
	{
		RECT rc;
		rc.left=(LONG)left;
		rc.top=(LONG)top;
		rc.right=(LONG)right;
		rc.bottom=(LONG)bottom;
		return rc;
	}	
	bool operator==(FLOAT_RECT& rect)
	{
		if(left==rect.left&&top==rect.top&&right==rect.right&&bottom==rect.bottom)
			return true;
		return false;
	}
	bool operator!=(FLOAT_RECT& rect)
	{
		if(left!=rect.left||top!=rect.top||right!=rect.right||bottom!=rect.bottom)
			return true;
		return false;
	}
	void MoveTo(FLOAT x,FLOAT y)
	{
		top+=y;
		left+=x;
		right+=x;
		bottom+=y;
	}
	void SetPos(FLOAT x,FLOAT y)
	{
		right=right-left+x;
		bottom=bottom-top+y;
		left=x;
		bottom=y;
	}
	bool IsIn(FLOAT x,FLOAT y)
	{
		if(x>=left&&x<=right&&y>=top&&y<=bottom)
			return true;
		return false;
	}
	FLOAT Width()
	{
		return right-left;
	}
	FLOAT Height()
	{
		return bottom-top;
	}
	void SetWidth(FLOAT Width)
	{
		right=left+Width;
	}
	void SetHeight(FLOAT Height)
	{
		bottom=top+Height;
	}
	void SetSize(FLOAT Width,FLOAT Height)
	{
		right=left+Width;
		bottom=top+Height;
	}
};