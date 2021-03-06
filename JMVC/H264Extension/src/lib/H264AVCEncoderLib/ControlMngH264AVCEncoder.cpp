#include "H264AVCEncoderLib.h"
#include "ControlMngH264AVCEncoder.h"


H264AVC_NAMESPACE_BEGIN


ControlMngH264AVCEncoder::ControlMngH264AVCEncoder():
  m_pcFrameMng            ( NULL ),
  m_pcSliceEncoder        ( NULL ),
  m_pcControlMng          ( NULL ),
  m_pcBitWriteBuffer      ( NULL ),
  m_pcNalUnitEncoder      ( NULL ),
  m_pcUvlcWriter          ( NULL ),
  m_pcUvlcTester          ( NULL ),
  m_pcMbCoder             ( NULL ),
  m_pcLoopFilter          ( NULL ),
  m_pcMbEncoder           ( NULL ),
  m_pcQuarterPelFilter    ( NULL ),
  m_pcCodingParameter     ( NULL ),
  m_pcParameterSetMng     ( NULL ),
  m_pcSampleWeighting     ( NULL ),
  m_pcCabacWriter         ( NULL ),
  m_pcXDistortion         ( NULL ),
  m_pcMotionEstimation    ( NULL ),
  m_pcRateDistortion      ( NULL ),
  m_pcMbDataCtrl          ( NULL ),
  m_pcMbSymbolWriteIf     ( NULL )
{
  ::memset( m_apcYuvFullPelBufferCtrl, 0x00, MAX_LAYERS*sizeof(Void*) );
  ::memset( m_apcYuvHalfPelBufferCtrl, 0x00, MAX_LAYERS*sizeof(Void*) );
  ::memset( m_apcPocCalculator,        0x00, MAX_LAYERS*sizeof(Void*) );
  ::memset( m_auiMbXinFrame,           0x00, MAX_LAYERS*sizeof(UInt ) );
  ::memset( m_auiMbYinFrame,           0x00, MAX_LAYERS*sizeof(UInt ) );
}

ControlMngH264AVCEncoder::~ControlMngH264AVCEncoder()
{
}


ErrVal
ControlMngH264AVCEncoder::initParameterSets( const SequenceParameterSet&  rcSPS,
                                             const PictureParameterSet&   rcPPSLP,
                                             const PictureParameterSet&   rcPPSHP )
{
  UInt  uiLayer             = rcSPS.getLayerId          ();
  UInt  uiMbX               = rcSPS.getFrameWidthInMbs  ();
  UInt  uiMbY               = rcSPS.getFrameHeightInMbs ();
  m_auiMbXinFrame[uiLayer]  = uiMbX;
  m_auiMbYinFrame[uiLayer]  = uiMbY;

  //===== initialize buffer controls and MCTFEncoder =====
  RNOK( m_apcYuvFullPelBufferCtrl[uiLayer]->initSPS( uiMbY<<4, uiMbX<<4, YUV_Y_MARGIN, YUV_X_MARGIN    ) );
  RNOK( m_apcYuvHalfPelBufferCtrl[uiLayer]->initSPS( uiMbY<<4, uiMbX<<4, YUV_Y_MARGIN, YUV_X_MARGIN, 1 ) );

  return Err::m_nOK;
}


ErrVal ControlMngH264AVCEncoder::create( ControlMngH264AVCEncoder*& rpcControlMngH264AVCEncoder )
{
  rpcControlMngH264AVCEncoder = new ControlMngH264AVCEncoder;

  ROT( NULL == rpcControlMngH264AVCEncoder );

  return Err::m_nOK;
}

ErrVal ControlMngH264AVCEncoder::destroy()
{
  delete this;
  return Err::m_nOK;
}


ErrVal ControlMngH264AVCEncoder::uninit()
{
  m_pcFrameMng = NULL;
  m_pcSliceEncoder = NULL;
  m_pcControlMng = NULL;
  m_pcBitWriteBuffer = NULL;
  m_pcBitCounter = NULL;
  m_pcNalUnitEncoder = NULL;
  m_pcUvlcWriter = NULL;
  m_pcUvlcTester = NULL;
  m_pcMbCoder = NULL;
  m_pcLoopFilter = NULL;
  m_pcMbEncoder = NULL;
  m_pcTransform = NULL;
  m_pcIntraPrediction = NULL;
  m_pcQuarterPelFilter = NULL;
  m_pcCodingParameter = NULL;
  m_pcParameterSetMng = NULL;
  m_pcSampleWeighting = NULL;
  m_pcCabacWriter = NULL;
  m_pcXDistortion = NULL;
  m_pcMotionEstimation = NULL;
  m_pcRateDistortion = NULL;


  for( UInt uiLayer = 0; uiLayer < MAX_LAYERS; uiLayer++ )
  {
    m_apcYuvFullPelBufferCtrl [uiLayer] = 0;
    m_apcYuvHalfPelBufferCtrl [uiLayer] = 0;
    m_apcPocCalculator        [uiLayer] = 0;
  }


  return Err::m_nOK;
}

ErrVal ControlMngH264AVCEncoder::init( FrameMng*               pcFrameMng,
                                    SliceEncoder*           pcSliceEncoder,
                                    ControlMngH264AVCEncoder*  pcControlMng,
                                    BitWriteBuffer*         pcBitWriteBuffer,
                                    BitCounter*             pcBitCounter,
                                    NalUnitEncoder*         pcNalUnitEncoder,
                                    UvlcWriter*             pcUvlcWriter,
                                    UvlcWriter*             pcUvlcTester,
                                    MbCoder*                pcMbCoder,
                                    LoopFilter*             pcLoopFilter,
                                    MbEncoder*              pcMbEncoder,
                                    Transform*              pcTransform,
                                    IntraPredictionSearch*  pcIntraPrediction,
                                    YuvBufferCtrl*          apcYuvFullPelBufferCtrl [MAX_LAYERS],
                                    YuvBufferCtrl*          apcYuvHalfPelBufferCtrl [MAX_LAYERS],
                                    QuarterPelFilter*       pcQuarterPelFilter,
                                    CodingParameter*        pcCodingParameter,
                                    ParameterSetMng*        pcParameterSetMng,
                                    PocCalculator*          apcPocCalculator        [MAX_LAYERS],
                                    SampleWeighting*        pcSampleWeighting,
                                    CabacWriter*            pcCabacWriter,
                                    XDistortion*            pcXDistortion,
                                    MotionEstimation*       pcMotionEstimation,
                                    RateDistortion*         pcRateDistortion )
{
  ROT( NULL == pcFrameMng);
  ROT( NULL == pcSliceEncoder);
  ROT( NULL == pcControlMng);
  ROT( NULL == pcBitWriteBuffer);
  ROT( NULL == pcBitCounter);
  ROT( NULL == pcNalUnitEncoder);
  ROT( NULL == pcUvlcWriter);
  ROT( NULL == pcUvlcTester);
  ROT( NULL == pcMbCoder);
  ROT( NULL == pcLoopFilter);
  ROT( NULL == pcMbEncoder);
  ROT( NULL == pcTransform);
  ROT( NULL == pcIntraPrediction);
  ROT( NULL == pcQuarterPelFilter);
  ROT( NULL == pcCodingParameter);
  ROT( NULL == pcParameterSetMng);
  ROT( NULL == pcSampleWeighting);
  ROT( NULL == pcCabacWriter);
  ROT( NULL == pcXDistortion);
  ROT( NULL == pcMotionEstimation);
  ROT( NULL == pcRateDistortion);


  m_pcFrameMng          = pcFrameMng;
  m_pcSliceEncoder      = pcSliceEncoder;
  m_pcControlMng        = pcControlMng;
  m_pcBitWriteBuffer    = pcBitWriteBuffer;
  m_pcBitCounter        = pcBitCounter;
  m_pcNalUnitEncoder    = pcNalUnitEncoder;
  m_pcUvlcWriter        = pcUvlcWriter;
  m_pcUvlcTester        = pcUvlcTester;
  m_pcMbCoder           = pcMbCoder;
  m_pcLoopFilter        = pcLoopFilter;
  m_pcMbEncoder         = pcMbEncoder;
  m_pcTransform         = pcTransform;
  m_pcIntraPrediction   = pcIntraPrediction;
  m_pcQuarterPelFilter  = pcQuarterPelFilter;
  m_pcCodingParameter   = pcCodingParameter;
  m_pcParameterSetMng   = pcParameterSetMng;
  m_pcSampleWeighting   = pcSampleWeighting;
  m_pcCabacWriter       = pcCabacWriter;
  m_pcXDistortion       = pcXDistortion;
  m_pcMotionEstimation  = pcMotionEstimation;
  m_pcRateDistortion    = pcRateDistortion;


  ROT( NULL == apcPocCalculator );
  ROT( NULL == apcYuvFullPelBufferCtrl );
  ROT( NULL == apcYuvHalfPelBufferCtrl );

  for( UInt uiLayer = 0; uiLayer < MAX_LAYERS; uiLayer++ )
  {
    ROT( NULL == apcPocCalculator       [uiLayer] );
    ROT( NULL == apcYuvFullPelBufferCtrl[uiLayer] );
    ROT( NULL == apcYuvHalfPelBufferCtrl[uiLayer] );

    m_apcPocCalculator        [uiLayer] = apcPocCalculator        [uiLayer];
    m_apcYuvFullPelBufferCtrl [uiLayer] = apcYuvFullPelBufferCtrl [uiLayer];
    m_apcYuvHalfPelBufferCtrl [uiLayer] = apcYuvHalfPelBufferCtrl [uiLayer];
  }

  m_bMVCMode = ( pcCodingParameter->getMVCmode() != 0 );

  return Err::m_nOK;
}




ErrVal ControlMngH264AVCEncoder::finishSlice( const SliceHeader& rcSH, Bool& rbPicDone, Bool& rbFrameDone )
{
  ROT( NULL == m_pcMbSymbolWriteIf ) ;

  RNOK( m_pcMbSymbolWriteIf->finishSlice() );

  m_pcMbSymbolWriteIf = NULL;

  rbPicDone   = m_pcMbDataCtrl->isPicDone( rcSH );
  rbFrameDone = m_pcMbDataCtrl->isFrameDone( rcSH );

  return Err::m_nOK;
}



ErrVal ControlMngH264AVCEncoder::initSliceForCoding( const SliceHeader& rcSH )
{
  m_uiCurrLayer   = rcSH.getSPS().getLayerId();

  Bool bCabac = rcSH.getPPS().getEntropyCodingModeFlag();
  if( bCabac )
  {
    m_pcMbSymbolWriteIf = m_pcCabacWriter;
  }
  else
  {
    m_pcMbSymbolWriteIf = m_pcUvlcWriter;
  }

  RNOK( m_pcMbSymbolWriteIf   ->startSlice( rcSH ) );
  RNOK( m_pcMbEncoder         ->initSlice ( rcSH ) )
  RNOK( m_pcMbCoder           ->initSlice ( rcSH, m_pcMbSymbolWriteIf, m_pcRateDistortion ) );
  RNOK( m_pcMotionEstimation  ->initSlice ( rcSH ) );
  RNOK( m_pcSampleWeighting   ->initSlice ( rcSH ) );

  setFMO( rcSH.getFMO());

  return Err::m_nOK;
}


ErrVal ControlMngH264AVCEncoder::initSliceForFiltering( const SliceHeader& rcSH )
{
  m_uiCurrLayer   = rcSH.getSPS().getLayerId();

  return Err::m_nOK;
}


ErrVal ControlMngH264AVCEncoder::initSlice( SliceHeader& rcSH, ProcessingState eProcessingState )
{
  m_uiCurrLayer   = rcSH.getSPS().getLayerId();
  m_pcMbDataCtrl = rcSH.getFrameUnit()->getMbDataCtrl();

  RNOK( m_pcMbDataCtrl          ->initSlice( rcSH, eProcessingState, false, NULL ) );
  RNOK( m_pcMotionEstimation    ->initSlice( rcSH ) );
  RNOK( m_pcSampleWeighting     ->initSlice( rcSH ) );
  RNOK( m_pcMbEncoder           ->initSlice( rcSH ) );

  if( ENCODE_PROCESS == eProcessingState )
  {
    Bool bCabac = rcSH.getPPS().getEntropyCodingModeFlag();
    if( bCabac )
    {
      m_pcMbSymbolWriteIf = m_pcCabacWriter;
    }
    else
    {
      m_pcMbSymbolWriteIf = m_pcUvlcWriter;
    }

    RNOK( m_pcMbSymbolWriteIf->startSlice( rcSH ) );
    RNOK( m_pcMbCoder->initSlice( rcSH, m_pcMbSymbolWriteIf, m_pcRateDistortion ) );
  }

  return Err::m_nOK;
}


#ifdef LF_INTERLACE
ErrVal ControlMngH264AVCEncoder::initMbForCoding( MbDataAccess& rcMbDataAccess, UInt uiMbY, UInt uiMbX, Bool bMbAff, Bool bFieldFlag )
{
#else
ErrVal ControlMngH264AVCEncoder::initMbForCoding( MbDataAccess& rcMbDataAccess, UInt uiMbIndex )
{
#endif
    ROF( m_uiCurrLayer < MAX_LAYERS );
#ifdef LF_INTERLACE
    if( bMbAff )
    {
        rcMbDataAccess.setFieldMode( bFieldFlag );
    }
    else
    {
        //rpcMbDataAccess->getMbData().setFieldFlag( false ); // now: done above
        rcMbDataAccess.getMbMotionData( LIST_0 ).setFieldMode( false );
        rcMbDataAccess.getMbMotionData( LIST_1 ).setFieldMode( false );
    }

    RNOK( m_apcYuvFullPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX, bMbAff ) );
    RNOK( m_apcYuvHalfPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX, bMbAff ) );

    RNOK( m_pcMotionEstimation->initMb( uiMbY, uiMbX, rcMbDataAccess ) );
#else

  ROF( m_uiCurrLayer < MAX_LAYERS );

  UInt  uiMbY = uiMbIndex         / m_auiMbXinFrame[ m_uiCurrLayer ];
  UInt  uiMbX = uiMbIndex - uiMbY * m_auiMbXinFrame[ m_uiCurrLayer ];

  RNOK( m_apcYuvFullPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX ) );
  RNOK( m_apcYuvHalfPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX ) );

  RNOK( m_pcMotionEstimation->initMb( uiMbY, uiMbX, rcMbDataAccess ) );
#endif

  return Err::m_nOK;
}

#ifdef LF_INTERLACE
ErrVal ControlMngH264AVCEncoder::initMbForFiltering( MbDataAccess*& rpcMbDataAccess, UInt uiMbY, UInt uiMbX, Bool bMbAff )
#else
ErrVal ControlMngH264AVCEncoder::initMbForFiltering( MbDataAccess*& rpcMbDataAccess, UInt uiMbIndex )
#endif
{
  ROF( m_uiCurrLayer < MAX_LAYERS );

#ifdef LF_INTERLACE
//  m_pcMbDataCtrl->initMb( rpcMbDataAccess, uiMbY, uiMbX );//test

  RNOK( m_apcYuvFullPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX, bMbAff ) );
#else
  UInt uiMbY, uiMbX;

  uiMbY = uiMbIndex         / m_auiMbXinFrame[ m_uiCurrLayer ];
  uiMbX = uiMbIndex - uiMbY * m_auiMbXinFrame[ m_uiCurrLayer ];

  m_pcMbDataCtrl->initMb( rpcMbDataAccess, uiMbY, uiMbX );

  RNOK( m_apcYuvFullPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX ) );
#endif


  return Err::m_nOK;
}

#ifndef LF_INTERLACE
ErrVal ControlMngH264AVCEncoder::initMbForFiltering( UInt uiMbIndex )
{
  ROF( m_uiCurrLayer < MAX_LAYERS );

  UInt uiMbY, uiMbX;

  uiMbY = uiMbIndex         / m_auiMbXinFrame[ m_uiCurrLayer ];
  uiMbX = uiMbIndex - uiMbY * m_auiMbXinFrame[ m_uiCurrLayer ];

  RNOK( m_apcYuvFullPelBufferCtrl[m_uiCurrLayer]->initMb( uiMbY, uiMbX ) );

  return Err::m_nOK;
}
#endif

//TMM_WP
ErrVal ControlMngH264AVCEncoder::initSliceForWeighting ( const SliceHeader& rcSH)
{
   return m_pcSampleWeighting->initSlice( rcSH ); 
}
//TMM_WP

H264AVC_NAMESPACE_END
