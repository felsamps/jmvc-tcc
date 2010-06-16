#if !defined(AFX_HEADERSYMBOLREADIF_H__6CF9086D_DB71_40C0_91BD_6F205082CB8C__INCLUDED_)
#define AFX_HEADERSYMBOLREADIF_H__6CF9086D_DB71_40C0_91BD_6F205082CB8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


H264AVC_NAMESPACE_BEGIN

class HeaderSymbolReadIf
{
protected:
	HeaderSymbolReadIf()  {}
	virtual ~HeaderSymbolReadIf() {}

public:
  virtual ErrVal  getSvlc     ( Int& riCode, Char* pcTraceString ) = 0;
  virtual ErrVal  getUvlc     ( UInt& ruiCode, Char* pcTraceString ) = 0;
  virtual ErrVal  getCode     ( UInt& ruiCode, UInt uiLength, Char* pcTraceString ) = 0;
  virtual ErrVal  getFlag     ( Bool& rbFlag, Char* pcTraceString ) = 0;
  virtual ErrVal  readByteAlign() = 0;
  virtual ErrVal  readZeroByteAlign() = 0; // SEI LSJ
  virtual Bool    moreRBSPData() = 0;
};


H264AVC_NAMESPACE_END


#endif // !defined(AFX_HEADERSYMBOLREADIF_H__6CF9086D_DB71_40C0_91BD_6F205082CB8C__INCLUDED_)
