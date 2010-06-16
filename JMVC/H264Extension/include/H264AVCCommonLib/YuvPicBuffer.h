#if !defined(AFX_YUVBUFFER_H__9B273B48_A740_4E7E_8076_4FCCE69FBA98__INCLUDED_)
#define AFX_YUVBUFFER_H__9B273B48_A740_4E7E_8076_4FCCE69FBA98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "H264AVCCommonLib/YuvBufferCtrl.h"

H264AVC_NAMESPACE_BEGIN

class YuvMbBuffer;
class IntYuvMbBuffer;

class H264AVCCOMMONLIB_API YuvPicBuffer
{
public:
#ifdef   LF_INTERLACE
	YuvPicBuffer( YuvBufferCtrl& rcYuvBufferCtrl, PicType ePicType );
#else //!LF_INTERLACE
  YuvPicBuffer( YuvBufferCtrl& rcYuvBufferCtrl );
#endif //LF_INTERLACE
	virtual ~YuvPicBuffer();

  Pel* getBuffer()      { return m_pucYuvBuffer; }

  Pel* getLumBlk()      { return m_pPelCurr; }
  Pel* getYBlk( LumaIdx cIdx )
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getYBlk( cIdx ); }
  const Int getLStride()      const { return m_iStride; }
  Void set4x4Block( LumaIdx cIdx )
  {
    m_pPelCurr = m_pucYuvBuffer + m_rcBufferParam.getYBlk( cIdx );
  }
  Pel* getMbLumAddr()
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getMbLum(); }

  Pel* getUBlk( LumaIdx cIdx )
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getUBlk( cIdx ); }
  Pel* getVBlk( LumaIdx cIdx )
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getVBlk( cIdx ); }
  Pel* getMbCbAddr()
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getMbCb(); }
  Pel* getMbCrAddr()
  { AOF_DBG(m_pucYuvBuffer); return m_pucYuvBuffer + m_rcBufferParam.getMbCr(); }
  const Int getCStride()    const { return m_iStride>>1;}


  const Int getLWidth()     const { return m_rcBufferParam.getWidth(); }
  const Int getLHeight()    const { return m_rcBufferParam.getHeight(); }

  const Int getCWidth()     const { return m_rcBufferParam.getWidth()>>1; }
  const Int getCHeight()    const { return m_rcBufferParam.getHeight()>>1; }

  const Int getLXMargin()   const { return m_rcYuvBufferCtrl.getXMargin(); }
  const Int getLYMargin()   const { return m_rcYuvBufferCtrl.getYMargin(); }
  const Int getCXMargin()   const { return m_rcYuvBufferCtrl.getXMargin()>>1; }
  const Int getCYMargin()   const { return m_rcYuvBufferCtrl.getYMargin()>>1; }

#ifdef   LF_INTERLACE
  PicType getPicType() const { return m_ePicType; }
#endif //LF_INTERLACE

  ErrVal loadBuffer( YuvPicBuffer *pcSrcYuvPicBuffer ); //TMM_EC
  ErrVal loadBuffer( YuvMbBuffer *pcYuvMbBuffer );
  ErrVal loadBuffer( IntYuvMbBuffer *pcYuvMbBuffer );
  ErrVal fillMargin();
#ifdef   LF_INTERLACE
  ErrVal loadBufferAndFillMargin( YuvPicBuffer *pcSrcYuvPicBuffer );
#endif //LF_INTERLACE

  ErrVal init( Pel*& rpucYuvBuffer );
  ErrVal uninit();

  Bool isValid()        { return NULL != m_pucYuvBuffer; }

#ifdef   LF_INTERLACE
  Pel* getLumOrigin()      const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getLumOrigin( m_ePicType ); }
  Pel* getCbOrigin()       const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getCbOrigin ( m_ePicType ); }
  Pel* getCrOrigin()       const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getCrOrigin ( m_ePicType ); }
#else //!LF_INTERLACE
  Pel* getLumOrigin()      const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getLumOrigin(); }
  Pel* getCbOrigin()       const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getCbOrigin (); }
  Pel* getCrOrigin()       const { return m_pucYuvBuffer + m_rcYuvBufferCtrl.getCrOrigin (); }
#endif //LF_INTERLACE

  ErrVal copy( YuvPicBuffer* pcPicBuffer ); // HS: decoder robustness
protected:
    Void xFillPlaneMargin( Pel *pucDest, Int iHeight, Int iWidth, Int iStride, Int iXMargin, Int iYMargin );
#ifdef   LF_INTERLACE
    Void xCopyFillPlaneMargin( Pel *pucSrc, Pel *pucDest, Int iHeight, Int iWidth, Int iStride, Int iXMargin, Int iYMargin );
#endif //LF_INTERLACE
    Void xDump( FILE* hFile, Pel* pPel, Int iHeight, Int iWidth, Int iStride );

protected:
    const YuvBufferCtrl::YuvBufferParameter& m_rcBufferParam;
    //TMM_EC {{
public:
    YuvBufferCtrl& m_rcYuvBufferCtrl;
protected:	
    //TMM_EC }}
    Int  m_iStride;
    Pel* m_pPelCurr;
    Pel* m_pucYuvBuffer;
    Pel* m_pucOwnYuvBuffer;
#ifdef   LF_INTERLACE
    PicType m_ePicType;
#endif //LF_INTERLACE
};


H264AVC_NAMESPACE_END

#endif // !defined(AFX_YUVBUFFER_H__9B273B48_A740_4E7E_8076_4FCCE69FBA98__INCLUDED_)
