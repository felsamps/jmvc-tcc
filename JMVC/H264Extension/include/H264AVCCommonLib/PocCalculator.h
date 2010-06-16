#if !defined(AFX_POCCALCULATOR_H__5D005452_8A6E_4482_B502_D20DA8FAE6B0__INCLUDED_)
#define AFX_POCCALCULATOR_H__5D005452_8A6E_4482_B502_D20DA8FAE6B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



H264AVC_NAMESPACE_BEGIN



class H264AVCCOMMONLIB_API PocCalculator
{
public:
  PocCalculator();

  static  ErrVal  create  ( PocCalculator*& rpcPocCalculator );
  ErrVal  copy            ( PocCalculator*& rpcPocCalculator );
  ErrVal  destroy         ();

#ifdef   LF_INTERLACE
  ErrVal  initSPS         ( const SequenceParameterSet&  rcSequenceParameterSet );
#endif //LF_INTERLACE

  ErrVal  setPoc          ( SliceHeader& rcSliceHeader, Int iContFrameNumber );
  ErrVal  calculatePoc    ( SliceHeader& rcSliceHeader );

protected:
  ErrVal  xInitSPS        ( const SequenceParameterSet& rcSPS );

private:
#ifdef   LF_INTERLACE
    UInt    m_iLastIdrFieldNum;
#else //!LF_INTERLACE
  UInt    m_iLastIdrFrameNum;
#endif //LF_INTERLACE
  Int     m_iBitsLsb;
  Int     m_iTop2BotOffset;

  Int     m_iPrevRefPocMsb;
  Int     m_iPrevRefPocLsb;
  Int     m_iMaxPocLsb;
  Int     m_iFrameNumOffset;
  Int     m_iRefOffsetSum;
  Int     m_iPrevFrameNum;
};


H264AVC_NAMESPACE_END


#endif // !defined(AFX_POCCALCULATOR_H__5D005452_8A6E_4482_B502_D20DA8FAE6B0__INCLUDED_)
