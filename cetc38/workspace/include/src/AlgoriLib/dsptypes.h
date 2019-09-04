/*****************************************************************
  Copyright (C), 
  File name:      CALU.h
  Author:       Version: 
                Date: 
  Description:    // ???????????????????                 // ????????��????                  // ?????5??
  Others:         // ??????
  Function List:  // ?????????(??????
    1. ....
  History:        // ????????????(??????                 // ?????? 


       

       3. Date:	2007-11-20 
          Author:	Baohua
          Modification: delete struct DSP_BIT40
           
       4. Date:	2012-8-10
          Author:	GRui
          Modification: Add DSP_BIT48
           
       5. Date:	2012-8-23
          Author:	GRui
          Modification: Add SHF_RAM
*****************************************************************/
//---------------------------------------------------------------------------
#ifndef DSP_TYPES_H
#define DSP_TYPES_H

// undefine SC_ISS_DBG in release version
#define SC_ISS_DBG
//#define SC_ISS_RUN_ALONE  // ���ε�������ش��룬��������ģ������

//---------------------------------------------------------------------------
//????????��????????????
#define DSP_LINUX 
//#define DSP_WIN32

#ifdef DSP_WIN32	//data types in windows
	typedef unsigned char		DSPU8;
	typedef unsigned short		DSPU16;
	typedef unsigned int		DSPU32;
	typedef unsigned _int64		DSPU64;

    typedef signed char		    DSPI8;
	typedef short				DSPI16;
	typedef int                 DSPI32;
	typedef _int64		    	DSPI64;
	
	typedef float				DSPF32;
	typedef double				DSPF64;
#else				//data types in linux
    #ifdef DSP_LINUX
	typedef unsigned char		DSPU8;
	typedef unsigned short		DSPU16;
	typedef unsigned int		DSPU32;
	typedef unsigned long long	DSPU64;

    typedef signed char		    DSPI8;
	typedef short				DSPI16;
	typedef int	                DSPI32;
	typedef long long			DSPI64;
	
	typedef float				DSPF32;
	typedef double				DSPF64;
	#endif
#endif

//---------------------------------------------------------------------------
typedef union CUSTOM_F32	//32��????DSP????
{
	DSPU32 u32Value;
    struct SPLIT_F32 
    {
    	DSPU32    	u23Frac: 23;	//��?
    	DSPU32    	u8Exp: 8;		//??
    	DSPU32   	u1Nega: 1;		//?   	     	
  	} split_f32;
} CUSTOM_F32;

//---------------------------------------------------------------------------
typedef struct DSP_BIT80		//80��??????DSP????????
{
	DSPU64 u64Low;				//?��
	DSPU16 u16High;				//?��
   
} DSP_BIT80;
//---------------------------------------------------------------------------
typedef struct NEW_DSP_BIT80		//80��??????DSP????????
{
	DSPU32 u32Low;
	DSPU32 u32Mid;
	DSPU16 u16High;

} NEW_DSP_BIT80;
//---------------------------------------------------------------------------
typedef struct _DSP_BIT128
{
	DSPU32 u32LL;    //[31��0]
	DSPU32 u32HL;    //[63��32]
	DSPU32 u32LH;    //[95��64]
	DSPU32 u32HH;    //[127��96]
}DSP_BIT128;
//---------------------------------------------------------------------------
typedef struct DSP_BIT64		//80��??????DSP????????
{
	DSPU32 u32Low;				
	DSPU32 u32High;				
   
} DSP_BIT64;

//---------------------------------------------------------------------------
typedef struct DSP_BIT96		//80��??????DSP????????
{
	DSPU32 u32Low;	
	DSPU32 u32Mid;	
	DSPU32 u32High;				
   
} DSP_BIT96;

//---------------------------------------------------------------------------
typedef struct DSP_BIT40		//40��??????DSP???????
{
	//DSPU8  u8Exp;  ///Ӧ�����Ҫ�����Ӹó�Ա20140123
	//u8Exp�������ȷ�Ͽ���ɾ��20150116
	DSPU32 u32Low;				//?��
	DSPU8  u8High;				//??
   
} DSP_BIT40;

//---------------------------------------------------------------------------
typedef struct DSP_BIT48
{
	DSPU32 u32Low;			
	DSPU8  u8Mid;
	DSPU8  u8High;
   
} DSP_BIT48;

//---------------------------------------------------------------------------
typedef struct DSP_BIT20		//20��??????DSP???????
{
	DSPU16   u16Low ;				//?��
	DSPU8    u4High: 4;				//??
   
} DSP_BIT20;

//---------------------------------------------------------------------------

typedef union DSP_CFix16	//16��????
{
	DSPU32 u32Value;
    struct SPLIT
    {
    	DSPU32    	u16Imag: 16;	//?          
    	DSPU32    	u16Real: 16;	//?           
  	} split;
} DSP_CFix16;


//---------------------------------------------------------------------------
typedef struct DSP_CFix32		//32��????
{
	DSPU32 	u32Real;			//?
	DSPU32  u32Imag;			//?
   
} DSP_CFix32;

//---------------------------------------------------------------------------
typedef struct DSP_CFlo32		//32��????
{
	CUSTOM_F32 	flo32Real;			//?
	CUSTOM_F32  flo32Imag;			//?
   
} DSP_CFlo32;

//---------------------------------------------------------------------------
typedef struct
{
	DSPU32 arrElem[4][256];
   
} SHF_RAM; 

//add by yfl for ALU
typedef struct DSP_DF64
{
	DSPU32  u11exp ;    
   	DSPU32  u32High;
    DSPU32  u32Low;

} DSP_DF64;

//---------------------------------------------------------------------------
#endif	//DSP_TYPES_H

