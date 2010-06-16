#if !defined(AFX_FRAME_H__F0945458_9AA5_4D1A_9A9E_BFAAA9C416EF__INCLUDED_)
#define AFX_FRAME_H__F0945458_9AA5_4D1A_9A9E_BFAAA9C416EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "H264AVCCommonLib/YuvPicBuffer.h"


H264AVC_NAMESPACE_BEGIN


class FrameUnit;
class QuarterPelFilter;


class H264AVCCOMMONLIB_API Frame
{
public:
#ifdef   LF_INTERLACE
  class PocOrder
  {
  public:
      __inline Int operator() ( const Frame* pcFrame1, const Frame* pcFrame2 )
      {
          return pcFrame1->getPOC() < pcFrame2->getPOC();
      }
  };
#endif //LF_INTERLACE

#ifdef   LF_INTERLACE
  Frame( YuvBufferCtrl& rcYuvFullPelBufferCtrl, YuvBufferCtrl& rcYuvHalfPelBufferCtrl, PicType ePicType );
#else //!LF_INTERLACE
  Frame( YuvBufferCtrl& rcYuvFullPelBufferCtrl, YuvBufferCtrl& rcYuvHalfPelBufferCtrl );
#endif //LF_INTERLACE
  virtual ~Frame();
  ErrVal init( Pel* pucYuvBuffer, FrameUnit* pcFrameUnit );
  ErrVal uninit();
#ifdef   LF_INTERLACE
  ErrVal extendFrame( QuarterPelFilter* pcQuarterPelFilter, Bool bFrameMbsOnly, Bool bFGS );
#else //!LF_INTERLACE
  ErrVal extendFrame( QuarterPelFilter* pcQuarterPelFilter );
#endif //LF_INTERLACE

  FrameUnit*       getFrameUnit()         { return m_pcFrameUnit; }
  const FrameUnit* getFrameUnit()   const { return m_pcFrameUnit; }
  YuvPicBuffer* getFullPelYuvBuffer()     { return &m_cFullPelYuvBuffer; }
  YuvPicBuffer* getHalfPelYuvBuffer()     { return &m_cHalfPelYuvBuffer; }

  Bool  isPOCAvailable()            const { return m_bPOCisSet; }
  Int   getPOC        ()            const { return m_iPOC; }
  Void  setPOC        ( Int iPOC )        { m_iPOC = iPOC; m_bPOCisSet = true; }

#ifdef   LF_INTERLACE
  PicType getPicType()              const { return m_ePicType; }
  Bool isShortTerm()                const;
  //Int   getPOC        (PicType ePicType)  const {return ePicType==FRAME?m_iPOC:(ePicType==TOP_FIELD?m_cTopField.setPOC( iPoc ):m_cBotField.setPOC( iPoc ));}
#endif //LF_INTERLACE

  const Int stamp()                 const { return m_iStamp; }
  Int& stamp()                            { return m_iStamp; }

  const Bool isUsed()               const;

  UInt  getViewId     ()            const { return m_uiViewId; }
  Void  setViewId     ( UInt uiViewId )   { m_uiViewId = uiViewId; }
	
	Bool	getInterViewFlag ()					const { return m_uiInterviewflag;}  //JVT-W056
	Void	setInterViewFlag (Bool InterViewFlag) {m_uiInterviewflag = InterViewFlag;}//JVT-W056

protected:
  YuvPicBuffer  m_cFullPelYuvBuffer;
  YuvPicBuffer  m_cHalfPelYuvBuffer;
  FrameUnit*    m_pcFrameUnit;
  Bool          m_bPOCisSet;
  Int           m_iPOC;
  Int           m_iStamp;
  UInt          m_uiViewId;
	Bool					m_uiInterviewflag; //JVT-W056
#ifdef   LF_INTERLACE
    const PicType m_ePicType;
#endif //LF_INTERLACE
};



class RefPic
{
public:
  RefPic( const Frame* pcFrame = NULL, Int iStamp = 0 ):m_pcFrame(pcFrame), m_iStamp(iStamp){}
  const Bool operator==( const RefPic& rcRefPic ) const { return ((m_pcFrame == rcRefPic.m_pcFrame) && (m_iStamp == rcRefPic.m_iStamp)); }
  const Bool operator!=( const RefPic& rcRefPic ) const { return ((m_pcFrame != rcRefPic.m_pcFrame) || (m_iStamp != rcRefPic.m_iStamp)); }
  const Int  getStamp()                           const { return m_iStamp; }
  const Frame* getFrame()                         const { /*AOF_DBG(isAvailable());*/ return m_pcFrame; }
  const Frame& getPic()                           const { AOF_DBG(isAvailable()); return *m_pcFrame; }
  const Bool isAvailable()                        const { return ((m_pcFrame != NULL) && (m_iStamp == m_pcFrame->stamp())); }
  const Bool isAssigned()                         const { return (m_pcFrame != NULL); }
  Void setFrame( const Frame* pcFrame)                  { m_pcFrame = pcFrame;   m_iStamp  = ( pcFrame == 0 ? 0 : m_pcFrame->stamp() ); }

private:
  const Frame* m_pcFrame;
  Int          m_iStamp;
};


H264AVC_NAMESPACE_END


#endif // !defined(AFX_FRAME_H__F0945458_9AA5_4D1A_9A9E_BFAAA9C416EF__INCLUDED_)
