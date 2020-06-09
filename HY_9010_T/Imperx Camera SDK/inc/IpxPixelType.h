////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxPixelType.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Description of pixel types and color models, supported by IpxImage
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Created:	17-APR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2015-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef _IPX_PIXEL_TYPE_H
#define _IPX_PIXEL_TYPE_H

#include "IpxToolsBase.h"
#include <string.h>

#ifdef LINUX

	#ifndef strtok_s
		#define strtok_s(a,b,c) strtok_r(a, b, c)
	#endif
	#ifndef strncpy_s
		#define strncpy_s(a,b,c,d) strncpy(a, c, d)
	#endif
#endif

#define IIPL_ORIGIN_TL 0
#define IIPL_ORIGIN_BL 1

/** \brief Maximum number of image channels (colors) */
#define	MAX_IMAGE_CHANNELS		4

/** \brief Default image row alignment size (in bytes) */
#define  II_DEFAULT_IMAGE_ROW_ALIGN  4

//////////////////////////////////////////////////////////////////////
/// \defgroup ref_pixel IpxPixelType Header
/// \ingroup ref_data
/// \brief Defines, macros for IpxPixelTypes
/// \image html IpxPixelType.png
/// <center><b>IpxPixelType Headers</b></center>
///@{

/// Define pixel alignment.
//================================================================================
/**
* \note
* Pixel alignment defines order of bits placement.   
*
* \see II_PIXEL_TYPE_DEFINES
*/
typedef enum : uint32_t
{
	II_ALIGN_8				=	0x00000100, /**< 8-bit unsigned.  Value range: 0 to 255 */
	II_ALIGN_10				=	0x00000200, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12				=	0x00000300, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_14				=	0x00000400, /**< 14-bit unsigned. Value range: 0 to 16383 */
	II_ALIGN_16				=	0x00000500, /**< 16-bit unsigned. Value range: 0 to 65535 */
	II_ALIGN_10_PACKED_GEV	=	0x00001200, /**< 10-bit unsigned. Value range: 0 to 1023 - GigE Vision Mono10Packed, BayerXX10Packed alignment */
	II_ALIGN_12_PACKED_GEV	=	0x00001300, /**< 12-bit unsigned. Value range: 0 to 4095 - GigE Vision Mono12Packed, BayerXX12Packed  alignment */
	II_ALIGN_10_PACKED_PFNC	=	0x00002200, /**< 10-bit unsigned. Value range: 0 to 1023 - PFNC Mono10p, BayerXX10p alignment, used in U3V */
	II_ALIGN_12_PACKED_PFNC	=	0x00002300, /**< 12-bit unsigned. Value range: 0 to 4095 - PFNC Mono12p, BayerXX12p alignment, used in U3V */
	II_ALIGN_8_PACKED_FLEX	=	0x00003100, /**< 8-bit unsigned. Value range: 0 to 255  - Alignment scheme, used in Framelink Express grabber */
	II_ALIGN_10_PACKED_FLEX	=	0x00003200, /**< 10-bit unsigned. Value range: 0 to 1023 - Alignment scheme, used in Framelink Express grabber */
	II_ALIGN_12_PACKED_FLEX	=	0x00003300, /**< 12-bit unsigned. Value range: 0 to 4095 - Alignment scheme, used in Framelink Express grabber */

} II_PIXEL_ALIGNMENT;

/** \brief Mask to get pixel alignment. */
#define	II_PIXEL_ALIGNMENT_MASK	0x0000FF00

/** \brief Mask to get pixel packing. */
#define	II_PIXEL_ALIGNMENT_PACK_MASK	0x0000F000

#define II_ALIGN_UNPACKED    0x00000000
#define II_ALIGN_PACKED_GEV  0x00001000
#define II_ALIGN_PACKED_PFNC 0x00002000
#define II_ALIGN_PACKED_FLEX 0x00003000

/// Define pixel chromaticity.
//================================================================================
/**
* \note
* Pixel chromaticity defines number of color channels in an image.   
*
* \see II_PIXEL_TYPE_DEFINES
*/
typedef enum : uint32_t
{
	II_PIX_MONO				=	0x01000000,	/**< Monochrome pixel. */
	II_PIX_COLOR			=	0x02000000,	/**< Colored RGB pixel. */
	II_PIX_BAYER_CFA		=	0x03000000,	/**< Bayer CFA pixel. */
	II_PIX_SPARSE_CFA		=	0x04000000,	/**< Sparse TRUESENSE CFA pixel. */
	II_PIX_YUV      		=	0x05000000,	/**< YUV, YCbCr pixel. */
	II_PIX_CUSTOM			=	0x80000000,	/**< Custom defined pixel type. */
} II_PIXEL_CHROMATICITY;

/** \brief Mask to get pixel chromaticity. */
#define	II_PIXEL_COLOR_MASK		0xFF000000

/// Define effective number of bits occupied by the pixel (including padding).
//================================================================================
/**
* \note
* This value can be used to quickly compute the amount of memory required
* to store an image using pixel type.
*
* \see II_PIXEL_TYPE_DEFINES
*/
typedef enum : uint32_t
{
	II_PIX_OCCUPY_1_BIT		=	0x00010000,	/**< Pixel size: 1 bits */
	II_PIX_OCCUPY_2_BIT		=	0x00020000,	/**< Pixel size: 2 bits */
	II_PIX_OCCUPY_4_BIT		=	0x00040000,	/**< Pixel size: 4 bits */
	II_PIX_OCCUPY_8_BIT		=	0x00080000,	/**< Pixel size: 8 bits */
	II_PIX_OCCUPY_10_BIT    =	0x000A0000,	/**< Pixel size: 10 bits */
	II_PIX_OCCUPY_12_BIT	=	0x000C0000,	/**< Pixel size: 12 bits */
	II_PIX_OCCUPY_16_BIT	=	0x00100000,	/**< Pixel size: 16 bits */
	II_PIX_OCCUPY_20_BIT	=	0x00140000, /**< Pixel size: 20 bits */
	II_PIX_OCCUPY_24_BIT	=	0x00180000,	/**< Pixel size: 24 bits */
	II_PIX_OCCUPY_32_BIT	=	0x00200000,	/**< Pixel size: 32 bits */
	II_PIX_OCCUPY_36_BIT	=	0x00240000,	/**< Pixel size: 36 bits */
	II_PIX_OCCUPY_48_BIT	=	0x00300000,	/**< Pixel size: 48 bits */
}II_PIXEL_BITS;

/** \brief Mask to get pixel chromaticity. */
#define	II_PIXEL_SIZE_MASK		0x00FF0000

/** \brief Mask to get shift of pixel size. */
#define	II_PIXEL_SIZE_SHIFT		16

#define II_MONO_ID_MIN       0x01
#define II_MONO_ID_MAX       II_MONO_ID_MIN + 0x0B

#define II_BAYER_CFA_ID_MIN  II_MONO_ID_MAX + 1
#define II_BAYER_CFA_ID_MAX  II_BAYER_CFA_ID_MIN + 0x23

#define II_SPARCE_CFA_ID_MIN II_BAYER_CFA_ID_MAX + 1
#define II_SPARCE_CFA_ID_MAX II_SPARCE_CFA_ID_MIN + 0x3F

#define II_RGB_ID_MIN        II_SPARCE_CFA_ID_MAX + 1
#define II_RGB_ID_MAX        II_RGB_ID_MIN + 0x0E

#define II_YUV_ID_MIN        II_RGB_ID_MAX + 1
#define II_YUV_ID_MAX        II_YUV_ID_MIN + 0x05

/// Definition of Pixel Types for Images which are processed in IpxImage.
//================================================================================
/**
* \note
* Each pixel type is represented by a 32-bit value. The upper 8-bit indicates the 
* pixel chromaticity. The second upper 8-bit indicates the number of bit accupied by a pixel
* (including any padding). This can be used to quickly compute the amount of memory required
* to store an image using pixel type. Next 8-bit indicates pixel alignment that defines order
* of bits placement. Lower 8-bit indicates the pixel type identificator (pixel ID).
* Thus, pixel type contains main information about pixel structure. But pixel type don't
* define such parameters as color depth and channel order.   
*
* \see II_PIXEL_CHROMATICITY<BR/>II_PIXEL_BITS<BR/>II_PIXEL_ALIGNMENT
*/

typedef enum : uint32_t
{
// MONOCHROME (Pixel ID = [0...0x09])
    II_PIX_MONO8			 =	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			  | (II_MONO_ID_MIN+0x00)),
/**< That and next types define grayscale pixels  */
    II_PIX_MONO10			 =	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			  | (II_MONO_ID_MIN+0x01)),
    II_PIX_MONO10_PACKED_PFNC=	(II_PIX_MONO | II_PIX_OCCUPY_10_BIT	| II_ALIGN_10_PACKED_PFNC | (II_MONO_ID_MIN+0x02)),
    II_PIX_MONO10_PACKED_GEV =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED_GEV  | (II_MONO_ID_MIN+0x03)),
    II_PIX_MONO10_PACKED_FLEX =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED_FLEX | (II_MONO_ID_MIN+0x04)),
    II_PIX_MONO12			 =	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			  | (II_MONO_ID_MIN+0x05)),
    II_PIX_MONO12_PACKED_PFNC=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED_PFNC | (II_MONO_ID_MIN+0x06)),
    II_PIX_MONO12_PACKED_GEV =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED_GEV  | (II_MONO_ID_MIN+0x07)),
    II_PIX_MONO12_PACKED_FLEX =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED_FLEX | (II_MONO_ID_MIN+0x08)),
    II_PIX_MONO14			 =	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			  | (II_MONO_ID_MIN+0x09)),
    II_PIX_MONO16			 =	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			  | (II_MONO_ID_MIN+0x0A)),
    II_PIX_MONO8_PACKED_FLEX =	(II_PIX_MONO  | II_PIX_OCCUPY_8_BIT | II_ALIGN_8_PACKED_FLEX  | (II_MONO_ID_MIN+0x0B)),

// BAYER CFA (Pixel ID = [0x0A...0x2D])
    II_PIX_BAYGR8			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_8_BIT	 | II_ALIGN_8		  | (II_BAYER_CFA_ID_MIN+0x00)),
/**< That and next types define Bayer pixels */
    II_PIX_BAYRG8			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_8_BIT	 | II_ALIGN_8		  | (II_BAYER_CFA_ID_MIN+0x01)),
    II_PIX_BAYGB8			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_8_BIT	 | II_ALIGN_8		  | (II_BAYER_CFA_ID_MIN+0x02)),
    II_PIX_BAYBG8			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_8_BIT	 | II_ALIGN_8		  | (II_BAYER_CFA_ID_MIN+0x03)),
    II_PIX_BAYGR10			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_10		  | (II_BAYER_CFA_ID_MIN+0x04)),
    II_PIX_BAYRG10			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_10		  | (II_BAYER_CFA_ID_MIN+0x05)),
    II_PIX_BAYGB10			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_10		  | (II_BAYER_CFA_ID_MIN+0x06)),
    II_PIX_BAYBG10			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_10		  | (II_BAYER_CFA_ID_MIN+0x07)),
    II_PIX_BAYGR12			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_12		  | (II_BAYER_CFA_ID_MIN+0x08)),
    II_PIX_BAYRG12			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_12		  | (II_BAYER_CFA_ID_MIN+0x09)),
    II_PIX_BAYGB12			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_12		  | (II_BAYER_CFA_ID_MIN+0x0A)),
    II_PIX_BAYBG12			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_12		  | (II_BAYER_CFA_ID_MIN+0x0B)),
    II_PIX_BAYGR10_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_10_BIT | II_ALIGN_10_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x0C)),
    II_PIX_BAYGR10_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_10_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x0D)),
    II_PIX_BAYRG10_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_10_BIT | II_ALIGN_10_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x0E)),
    II_PIX_BAYRG10_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_10_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x0F)),
    II_PIX_BAYGB10_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_10_BIT | II_ALIGN_10_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x10)),
    II_PIX_BAYGB10_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_10_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x11)),
    II_PIX_BAYBG10_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_10_BIT | II_ALIGN_10_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x12)),
    II_PIX_BAYBG10_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_10_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x13)),
    II_PIX_BAYGR12_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x14)),
    II_PIX_BAYGR12_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x15)),
    II_PIX_BAYRG12_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x16)),
    II_PIX_BAYRG12_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x17)),
    II_PIX_BAYGB12_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x18)),
    II_PIX_BAYGB12_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x19)),
    II_PIX_BAYBG12_PACKED_PFNC=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_PFNC	| (II_BAYER_CFA_ID_MIN+0x1A)),
    II_PIX_BAYBG12_PACKED_GEV =	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_GEV 	| (II_BAYER_CFA_ID_MIN+0x1B)),
    II_PIX_BAYGR14			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_14			| (II_BAYER_CFA_ID_MIN+0x1C)),
    II_PIX_BAYRG14			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_14			| (II_BAYER_CFA_ID_MIN+0x1D)),
    II_PIX_BAYGB14			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_14			| (II_BAYER_CFA_ID_MIN+0x1E)),
    II_PIX_BAYBG14			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_14			| (II_BAYER_CFA_ID_MIN+0x1F)),
    II_PIX_BAYGR16			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_16			| (II_BAYER_CFA_ID_MIN+0x20)),
    II_PIX_BAYRG16			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_16			| (II_BAYER_CFA_ID_MIN+0x21)),
    II_PIX_BAYGB16			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_16			| (II_BAYER_CFA_ID_MIN+0x22)),
    II_PIX_BAYBG16			=	(II_PIX_BAYER_CFA | II_PIX_OCCUPY_16_BIT | II_ALIGN_16			| (II_BAYER_CFA_ID_MIN+0x23)),
																													  
// TRUESENSE SPARCE CFA (Pixel ID = [0x51...0x90])
    II_PIX_TS_BGGR_WBBW0_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x00)),
/**< That and next types define Sparse CFA pixels */          
    II_PIX_TS_BGGR_WBBW1_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x01)),
    II_PIX_TS_BGGR_WBBW2_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x02)),
    II_PIX_TS_BGGR_WBBW3_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x03)),
    II_PIX_TS_GBRG_WGGW0_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x04)),
    II_PIX_TS_GBRG_WGGW1_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x05)),
    II_PIX_TS_GBRG_WGGW2_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x06)),
    II_PIX_TS_GBRG_WGGW3_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x07)),
    II_PIX_TS_GRBG_WGGW0_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x08)),
    II_PIX_TS_GRBG_WGGW1_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x09)),
    II_PIX_TS_GRBG_WGGW2_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0A)),
    II_PIX_TS_GRBG_WGGW3_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0B)),
    II_PIX_TS_RGGB_WRRW0_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0C)),
    II_PIX_TS_RGGB_WRRW1_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0D)),
    II_PIX_TS_RGGB_WRRW2_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0E)),
    II_PIX_TS_RGGB_WRRW3_8	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8		| (II_SPARCE_CFA_ID_MIN+0x0F)),
																													   
    II_PIX_TS_BGGR_WBBW0_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x10)),
    II_PIX_TS_BGGR_WBBW1_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x11)),
    II_PIX_TS_BGGR_WBBW2_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x12)),
    II_PIX_TS_BGGR_WBBW3_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x13)),
    II_PIX_TS_GBRG_WGGW0_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x14)),
    II_PIX_TS_GBRG_WGGW1_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x15)),
    II_PIX_TS_GBRG_WGGW2_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x16)),
    II_PIX_TS_GBRG_WGGW3_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x17)),
    II_PIX_TS_GRBG_WGGW0_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x18)),
    II_PIX_TS_GRBG_WGGW1_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x19)),
    II_PIX_TS_GRBG_WGGW2_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1A)),
    II_PIX_TS_GRBG_WGGW3_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1B)),
    II_PIX_TS_RGGB_WRRW0_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1C)),
    II_PIX_TS_RGGB_WRRW1_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1D)),
    II_PIX_TS_RGGB_WRRW2_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1E)),
    II_PIX_TS_RGGB_WRRW3_10	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10		| (II_SPARCE_CFA_ID_MIN+0x1F)),

    II_PIX_TS_BGGR_WBBW0_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x20)),
    II_PIX_TS_BGGR_WBBW1_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x21)),
    II_PIX_TS_BGGR_WBBW2_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x22)),
    II_PIX_TS_BGGR_WBBW3_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x23)),
    II_PIX_TS_GBRG_WGGW0_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x24)),
    II_PIX_TS_GBRG_WGGW1_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x25)),
    II_PIX_TS_GBRG_WGGW2_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x26)),
    II_PIX_TS_GBRG_WGGW3_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x27)),
    II_PIX_TS_GRBG_WGGW0_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x28)),
    II_PIX_TS_GRBG_WGGW1_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x29)),
    II_PIX_TS_GRBG_WGGW2_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2A)),
    II_PIX_TS_GRBG_WGGW3_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2B)),
    II_PIX_TS_RGGB_WRRW0_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2C)),
    II_PIX_TS_RGGB_WRRW1_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2D)),
    II_PIX_TS_RGGB_WRRW2_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2E)),
    II_PIX_TS_RGGB_WRRW3_12	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12		| (II_SPARCE_CFA_ID_MIN+0x2F)),
																													   
    II_PIX_TS_BGGR_WBBW0_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x30)),
    II_PIX_TS_BGGR_WBBW1_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x31)),
    II_PIX_TS_BGGR_WBBW2_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x32)),
    II_PIX_TS_BGGR_WBBW3_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x33)),
    II_PIX_TS_GBRG_WGGW0_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x34)),
    II_PIX_TS_GBRG_WGGW1_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x35)),
    II_PIX_TS_GBRG_WGGW2_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x36)),
    II_PIX_TS_GBRG_WGGW3_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x37)),
    II_PIX_TS_GRBG_WGGW0_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x38)),
    II_PIX_TS_GRBG_WGGW1_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x39)),
    II_PIX_TS_GRBG_WGGW2_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3A)),
    II_PIX_TS_GRBG_WGGW3_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3B)),
    II_PIX_TS_RGGB_WRRW0_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3C)),
    II_PIX_TS_RGGB_WRRW1_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3D)),
    II_PIX_TS_RGGB_WRRW2_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3E)),
    II_PIX_TS_RGGB_WRRW3_14	=	(II_PIX_SPARSE_CFA | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14		| (II_SPARCE_CFA_ID_MIN+0x3F)),

//	RGB PIXEL TYPE	(Pixel ID = [0x91...0xB0]																										  
    II_PIX_RGB8				=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| (II_RGB_ID_MIN+0x00)),
/**< That and next types define RGB-BGR pixels */
    II_PIX_BGR8				=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| (II_RGB_ID_MIN+0x01)),
    II_PIX_RGBA8			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			| (II_RGB_ID_MIN+0x02)),
    II_PIX_BGRA8			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			| (II_RGB_ID_MIN+0x03)),
    II_PIX_RGB10			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_10			| (II_RGB_ID_MIN+0x04)),
    II_PIX_BGR10			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_10			| (II_RGB_ID_MIN+0x05)),
    II_PIX_RGB12			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_12			| (II_RGB_ID_MIN+0x06)),
    II_PIX_BGR12			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_12			| (II_RGB_ID_MIN+0x07)),
    II_PIX_RGB14			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_14			| (II_RGB_ID_MIN+0x08)),
    II_PIX_BGR14			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_14			| (II_RGB_ID_MIN+0x09)),
    II_PIX_RGB16			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_16			| (II_RGB_ID_MIN+0x0A)),
    II_PIX_BGR16	        =	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT |   II_ALIGN_16			| (II_RGB_ID_MIN+0x0B)),
//  Framelink Express RGB pixel types
    II_PIX_RGB8_PACKED_FLEX	=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT | II_ALIGN_8_PACKED_FLEX   | (II_RGB_ID_MIN+0x0C)),
    II_PIX_RGB10_PACKED_FLEX=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT | II_ALIGN_10_PACKED_FLEX	| (II_RGB_ID_MIN+0x0D)),
    II_PIX_RGB12_PACKED_FLEX=	(II_PIX_COLOR | II_PIX_OCCUPY_36_BIT | II_ALIGN_12_PACKED_FLEX	| (II_RGB_ID_MIN+0x0E)),
	
//	YUV PIXEL TYPE	(Pixel ID = [0xB1...0xD0]	
    II_PIX_YUV422_8_UYVY	=	(II_PIX_YUV | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| (II_YUV_ID_MIN+0x00)),
/**< That and next types define YUV and TCbCr packed pixels */
    II_PIX_YUV422_8			=	(II_PIX_YUV | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			  | (II_YUV_ID_MIN+0x01)),
    II_PIX_YUV422_10		=	(II_PIX_YUV | II_PIX_OCCUPY_32_BIT	| II_ALIGN_10			  | (II_YUV_ID_MIN+0x02)),
    II_PIX_YUV422_10_PACKED	=	(II_PIX_YUV | II_PIX_OCCUPY_20_BIT	| II_ALIGN_10_PACKED_FLEX | (II_YUV_ID_MIN+0x03)),
    II_PIX_YUV444_8			=	(II_PIX_YUV | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			  | (II_YUV_ID_MIN+0x04)),
    II_PIX_YCBCR422_8		=	(II_PIX_YUV | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			  | (II_YUV_ID_MIN+0x05)),
//
	II_PIX_NONE_TYPE		= 0
/**< The label for undefined pixel type */
}II_PIXEL_TYPE_DEFINES;

/** \brief Mask to get pixel ID value. */
#define	II_PIXEL_ID_MASK		0x000000FF

//////////////////////////////////////////////////////////////////////////////////////////////////////

/** \brief Returns aligned row size for defined pixel type and number of pixels in row. */
#define	II_GET_ROW_SIZE(pixType, width)				IpxGetRowSize(pixType, width)
/** \brief Returns pixel alignment for defined pixel type. */
#define	II_GET_PIXEL_ALIGNMENT(pixType)				(pixType & II_PIXEL_ALIGNMENT_MASK)
/** \brief Returns pixel chromaticity for defined pixel type. */
#define	II_GET_PIXEL_CHROMATICITY(pixType)			(pixType & II_PIXEL_COLOR_MASK)
/** \brief Returns 'true' if pixel is colored. */
#define	II_IS_COLOR_PIXEL(pixType)					((pixType & II_PIXEL_COLOR_MASK) == II_PIX_COLOR)
/** \brief Returns 'true' if pixel is custom. */
#define	II_IS_CUSTOM_PIXEL(pixType)					((pixType & II_PIXEL_COLOR_MASK) == II_PIX_CUSTOM)
/** \brief Returns size of pixel in bits for defined pixel type. */
#define	II_GET_PIXEL_BITS_SIZE(pixType)				((pixType & II_PIXEL_SIZE_MASK)>>II_PIXEL_SIZE_SHIFT)
/** \brief Returns identificator of pixel type for defined pixel type. */
#define	II_GET_PIXEL_ID(pixType)					(pixType & II_PIXEL_ID_MASK)
/** \brief Returns index of pixel type for defined pixel type. */
#define	II_GET_PIXEL_TYPE_INDEX(pixType)			((pixType & II_PIXEL_ID_MASK) - 1)
/** \brief Returns aligned image size for defined pixel type, width and height. */
#define	II_GET_IMAGE_SIZE(pixType, width, height)	(height * II_GET_ROW_SIZE(pixType, width))
/** \brief Returns 'true' if pixel is packed. */
#define	II_IS_PACKED_PIXEL(pixType)					( II_PIXEL_ALIGNMENT_PACK_MASK & pixType )
/** \brief Returns 'true' if pixel is packed, accorsing PFNC scheme */
#define	II_IS_PACKED_PIXEL_PFNC(pixType)				( (II_PIXEL_ALIGNMENT_PACK_MASK & pixType) == II_ALIGN_PACKED_PFNC )
/** \brief Returns 'true' if pixel is packed, accorsing GEV scheme */
#define	II_IS_PACKED_PIXEL_GEV(pixType)					( (II_PIXEL_ALIGNMENT_PACK_MASK & pixType) == II_ALIGN_PACKED_GEV )
/** \brief Returns 'true' if pixel type is Bayer CFA pattern. */
#define	II_IS_BAYER_CFA_PIXEL(__pixType__)		(  II_GET_PIXEL_CHROMATICITY(__pixType__) == II_PIX_BAYER_CFA )	
/** \brief Returns 'true' if pixel type is Monochrome. */
#define	II_IS_MONO_PIXEL(__pixType__)		(  II_GET_PIXEL_CHROMATICITY(__pixType__) == II_PIX_MONO )	
/** \brief Returns 'true' if pixel type is Color RGB or BGR. */
#define	II_IS_COLOR_RGB_PIXEL(__pixType__)		(  II_GET_PIXEL_CHROMATICITY(__pixType__) == II_PIX_COLOR )	



/// Returns row size for defined pixel type and number of pixels in row.
//================================================================================
/**
* @param pixType Pixel type.
* @param width Number of pixels in row.
* \return 
* The return value is row size.
* \note 
* Row size is aligned for effective memory using.
*
*/
IPX_INLINE  uint32_t IpxGetRowSize(uint32_t pixType, uint32_t width)
{
	uint32_t rowSize = width * II_GET_PIXEL_BITS_SIZE(pixType);
	rowSize = (rowSize % 8) ? ((rowSize >> 3) + 1) : (rowSize >> 3);
	return (rowSize + II_DEFAULT_IMAGE_ROW_ALIGN - 1) & (~(II_DEFAULT_IMAGE_ROW_ALIGN - 1));
}


/// Returns the size of unalligned row for defined pixel type and number of pixels
//================================================================================
/**
* @param pixType Pixel type.
* @param width Number of pixels in row.
* \return 
* The return value is row size.
* \note 
* Row size is aligned for effective memory using.
* \req
* \table2{Header, IpxPixelType.h}
* \endtable
*/

IPX_INLINE  uint32_t IpxGetRowSizeUnaligned(uint32_t pixType, uint32_t width)
{
    //TODO check specs for row alignment for packed formats
    if (((pixType & II_PIXEL_COLOR_MASK) != II_PIX_COLOR) && ((pixType & II_PIXEL_ALIGNMENT_MASK) == II_ALIGN_10_PACKED_PFNC)) 
		return (width * 5L / 4L);
	else if (((pixType & II_PIXEL_COLOR_MASK) != II_PIX_COLOR) && ( ((pixType & II_PIXEL_ALIGNMENT_MASK) == II_ALIGN_12_PACKED_PFNC)|| 
		                                                            ((pixType & II_PIXEL_ALIGNMENT_MASK) == II_ALIGN_12_PACKED_GEV) || 
																	((pixType & II_PIXEL_ALIGNMENT_MASK) == II_ALIGN_10_PACKED_GEV)   )) 
		return (width * 3L / 2L);
    uint32_t rowSize = width * II_GET_PIXEL_BITS_SIZE(pixType);
	rowSize = (rowSize % 8) ? ((rowSize >> 3) + 1) : (rowSize >> 3);
	return (rowSize);
}


///@}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////            Names of Color Models
//////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Names for Mono pixel format
																   
#define	II_PTN_MONO_8					"Mono8"
#define	II_PTN_MONO_10					"Mono10"
#define	II_PTN_MONO_10_PACKED			"Mono10Packed"
#define	II_PTN_MONO_10_PACKED_PFNC		"Mono10p"
#define	II_PTN_MONO_10_PACKED_FLEX		"Mono10_FLExPacked"
#define	II_PTN_MONO_12					"Mono12"
#define	II_PTN_MONO_12_PACKED			"Mono12Packed"
#define	II_PTN_MONO_12_PACKED_FLEX		"Mono12_FLExPacked"
#define	II_PTN_MONO_12_PACKED_PFNC		"Mono12p"
#define	II_PTN_MONO_14					"Mono14"
#define	II_PTN_MONO_16					"Mono16"
#define	II_PTN_MONO_8_PACKED_FLEX	    "Mono8_FLExPacked"
//
// Names for Bayer pixel format
#define	II_PTN_BAYER_GR8				"BayerGR8"
#define	II_PTN_BAYER_RG8				"BayerRG8"
#define	II_PTN_BAYER_GB8				"BayerGB8"
#define	II_PTN_BAYER_BG8				"BayerBG8"
#define	II_PTN_BAYER_GR10				"BayerGR10"
#define	II_PTN_BAYER_RG10				"BayerRG10"
#define	II_PTN_BAYER_GB10				"BayerGB10"
#define	II_PTN_BAYER_BG10				"BayerBG10"
#define	II_PTN_BAYER_GR12				"BayerGR12"
#define	II_PTN_BAYER_RG12				"BayerRG12"
#define	II_PTN_BAYER_GB12				"BayerGB12"
#define	II_PTN_BAYER_BG12				"BayerBG12"
#define	II_PTN_BAYER_GR10_PACKED_GEV		"BayerGR10Packed"
#define	II_PTN_BAYER_RG10_PACKED_GEV		"BayerRG10Packed"
#define	II_PTN_BAYER_GB10_PACKED_GEV		"BayerGB10Packed"
#define	II_PTN_BAYER_BG10_PACKED_GEV		"BayerBG10Packed"
#define	II_PTN_BAYER_GR12_PACKED_GEV		"BayerGR12Packed"
#define	II_PTN_BAYER_RG12_PACKED_GEV		"BayerRG12Packed"
#define	II_PTN_BAYER_GB12_PACKED_GEV		"BayerGB12Packed"
#define	II_PTN_BAYER_BG12_PACKED_GEV		"BayerBG12Packed"
#define	II_PTN_BAYER_GR10_PACKED_PFNC		"BayerGR10p"
#define	II_PTN_BAYER_RG10_PACKED_PFNC		"BayerRG10p"
#define	II_PTN_BAYER_GB10_PACKED_PFNC		"BayerGB10p"
#define	II_PTN_BAYER_BG10_PACKED_PFNC		"BayerBG10p"
#define	II_PTN_BAYER_GR12_PACKED_PFNC		"BayerGR12p"
#define	II_PTN_BAYER_RG12_PACKED_PFNC		"BayerRG12p"
#define	II_PTN_BAYER_GB12_PACKED_PFNC		"BayerGB12p"
#define	II_PTN_BAYER_BG12_PACKED_PFNC		"BayerBG12p"

#define	II_PTN_BAYER_GR14				"BayerGR14"
#define	II_PTN_BAYER_RG14				"BayerRG14"
#define	II_PTN_BAYER_GB14				"BayerGB14"
#define	II_PTN_BAYER_BG14				"BayerBG14"
#define	II_PTN_BAYER_GR16				"BayerGR16"
#define	II_PTN_BAYER_RG16				"BayerRG16"
#define	II_PTN_BAYER_GB16				"BayerGB16"
#define	II_PTN_BAYER_BG16				"BayerBG16"
//
// True Sense pixel format defines
#define	II_PTN_TS_BGGR_WBBW_0_8			"TrueSense_BGGR_WBBW_0_8"
#define	II_PTN_TS_BGGR_WBBW_1_8			"TrueSense_BGGR_WBBW_1_8"
#define	II_PTN_TS_BGGR_WBBW_2_8			"TrueSense_BGGR_WBBW_2_8"
#define	II_PTN_TS_BGGR_WBBW_3_8			"TrueSense_BGGR_WBBW_3_8"
#define	II_PTN_TS_GBRG_WGGW_0_8			"TrueSense_GBRG_WGGW_0_8"
#define	II_PTN_TS_GBRG_WGGW_1_8			"TrueSense_GBRG_WGGW_1_8"
#define	II_PTN_TS_GBRG_WGGW_2_8			"TrueSense_GBRG_WGGW_2_8"
#define	II_PTN_TS_GBRG_WGGW_3_8			"TrueSense_GBRG_WGGW_3_8"
#define	II_PTN_TS_GRBG_WGGW_0_8			"TrueSense_GRBG_WGGW_0_8"
#define	II_PTN_TS_GRBG_WGGW_1_8			"TrueSense_GRBG_WGGW_1_8"
#define	II_PTN_TS_GRBG_WGGW_2_8			"TrueSense_GRBG_WGGW_2_8"
#define	II_PTN_TS_GRBG_WGGW_3_8			"TrueSense_GRBG_WGGW_3_8"
#define	II_PTN_TS_RGGB_WRRW_0_8			"TrueSense_RGGB_WRRW_0_8"
#define	II_PTN_TS_RGGB_WRRW_1_8			"TrueSense_RGGB_WRRW_1_8"
#define	II_PTN_TS_RGGB_WRRW_2_8			"TrueSense_RGGB_WRRW_2_8"
#define	II_PTN_TS_RGGB_WRRW_3_8			"TrueSense_RGGB_WRRW_3_8"

#define	II_PTN_TS_BGGR_WBBW_0_10		"TrueSense_BGGR_WBBW_0_10"
#define	II_PTN_TS_BGGR_WBBW_1_10		"TrueSense_BGGR_WBBW_1_10"
#define	II_PTN_TS_BGGR_WBBW_2_10		"TrueSense_BGGR_WBBW_2_10"
#define	II_PTN_TS_BGGR_WBBW_3_10		"TrueSense_BGGR_WBBW_3_10"
#define	II_PTN_TS_GBRG_WGGW_0_10		"TrueSense_GBRG_WGGW_0_10"
#define	II_PTN_TS_GBRG_WGGW_1_10		"TrueSense_GBRG_WGGW_1_10"
#define	II_PTN_TS_GBRG_WGGW_2_10		"TrueSense_GBRG_WGGW_2_10"
#define	II_PTN_TS_GBRG_WGGW_3_10		"TrueSense_GBRG_WGGW_3_10"
#define	II_PTN_TS_GRBG_WGGW_0_10		"TrueSense_GRBG_WGGW_0_10"
#define	II_PTN_TS_GRBG_WGGW_1_10		"TrueSense_GRBG_WGGW_1_10"
#define	II_PTN_TS_GRBG_WGGW_2_10		"TrueSense_GRBG_WGGW_2_10"
#define	II_PTN_TS_GRBG_WGGW_3_10		"TrueSense_GRBG_WGGW_3_10"
#define	II_PTN_TS_RGGB_WRRW_0_10		"TrueSense_RGGB_WRRW_0_10"
#define	II_PTN_TS_RGGB_WRRW_1_10		"TrueSense_RGGB_WRRW_1_10"
#define	II_PTN_TS_RGGB_WRRW_2_10		"TrueSense_RGGB_WRRW_2_10"
#define	II_PTN_TS_RGGB_WRRW_3_10		"TrueSense_RGGB_WRRW_3_10"

#define	II_PTN_TS_BGGR_WBBW_0_12		"TrueSense_BGGR_WBBW_0_12"
#define	II_PTN_TS_BGGR_WBBW_1_12		"TrueSense_BGGR_WBBW_1_12"
#define	II_PTN_TS_BGGR_WBBW_2_12		"TrueSense_BGGR_WBBW_2_12"
#define	II_PTN_TS_BGGR_WBBW_3_12		"TrueSense_BGGR_WBBW_3_12"
#define	II_PTN_TS_GBRG_WGGW_0_12		"TrueSense_GBRG_WGGW_0_12"
#define	II_PTN_TS_GBRG_WGGW_1_12		"TrueSense_GBRG_WGGW_1_12"
#define	II_PTN_TS_GBRG_WGGW_2_12		"TrueSense_GBRG_WGGW_2_12"
#define	II_PTN_TS_GBRG_WGGW_3_12		"TrueSense_GBRG_WGGW_3_12"
#define	II_PTN_TS_GRBG_WGGW_0_12		"TrueSense_GRBG_WGGW_0_12"
#define	II_PTN_TS_GRBG_WGGW_1_12		"TrueSense_GRBG_WGGW_1_12"
#define	II_PTN_TS_GRBG_WGGW_2_12		"TrueSense_GRBG_WGGW_2_12"
#define	II_PTN_TS_GRBG_WGGW_3_12		"TrueSense_GRBG_WGGW_3_12"
#define	II_PTN_TS_RGGB_WRRW_0_12		"TrueSense_RGGB_WRRW_0_12"
#define	II_PTN_TS_RGGB_WRRW_1_12		"TrueSense_RGGB_WRRW_1_12"
#define	II_PTN_TS_RGGB_WRRW_2_12		"TrueSense_RGGB_WRRW_2_12"
#define	II_PTN_TS_RGGB_WRRW_3_12		"TrueSense_RGGB_WRRW_3_12"

#define	II_PTN_TS_BGGR_WBBW_0_14		"TrueSense_BGGR_WBBW_0_14"
#define	II_PTN_TS_BGGR_WBBW_1_14		"TrueSense_BGGR_WBBW_1_14"
#define	II_PTN_TS_BGGR_WBBW_2_14		"TrueSense_BGGR_WBBW_2_14"
#define	II_PTN_TS_BGGR_WBBW_3_14		"TrueSense_BGGR_WBBW_3_14"
#define	II_PTN_TS_GBRG_WGGW_0_14		"TrueSense_GBRG_WGGW_0_14"
#define	II_PTN_TS_GBRG_WGGW_1_14		"TrueSense_GBRG_WGGW_1_14"
#define	II_PTN_TS_GBRG_WGGW_2_14		"TrueSense_GBRG_WGGW_2_14"
#define	II_PTN_TS_GBRG_WGGW_3_14		"TrueSense_GBRG_WGGW_3_14"
#define	II_PTN_TS_GRBG_WGGW_0_14		"TrueSense_GRBG_WGGW_0_14"
#define	II_PTN_TS_GRBG_WGGW_1_14		"TrueSense_GRBG_WGGW_1_14"
#define	II_PTN_TS_GRBG_WGGW_2_14		"TrueSense_GRBG_WGGW_2_14"
#define	II_PTN_TS_GRBG_WGGW_3_14		"TrueSense_GRBG_WGGW_3_14"
#define	II_PTN_TS_RGGB_WRRW_0_14		"TrueSense_RGGB_WRRW_0_14"
#define	II_PTN_TS_RGGB_WRRW_1_14		"TrueSense_RGGB_WRRW_1_14"
#define	II_PTN_TS_RGGB_WRRW_2_14		"TrueSense_RGGB_WRRW_2_14"
#define	II_PTN_TS_RGGB_WRRW_3_14		"TrueSense_RGGB_WRRW_3_14"
//
// RGB Packed pixel format defines
#define	II_PTN_RGB_8					"RGB8"                               
#define	II_PTN_BGR_8					"BGR8"								 
#define	II_PTN_RGBA_8					"RGBa8"								 
#define	II_PTN_BGRA_8					"BGRa8"								 
#define	II_PTN_RGB_10					"RGB10"								 
#define	II_PTN_BGR_10					"BGR10"								 
#define	II_PTN_RGB_12					"RGB12"								 
#define	II_PTN_BGR_12					"BGR12"								 
#define	II_PTN_RGB_14					"RGB14"								 
#define	II_PTN_BGR_14					"BGR14"								 
#define	II_PTN_RGB_16					"RGB16"								 
#define	II_PTN_BGR_16					"BGR16"								 
#define	II_PTN_RGB_8_PACKED_FLEX		"RGB_8_FLEX_Packed"					 
#define	II_PTN_RGB_10_PACKED_FLEX		"RGB_10_FLEX_Packed"				 
#define	II_PTN_RGB_12_PACKED_FLEX		"RGB_12_FLEX_Packed"				 
																			 	
//																			
// YUV Packed pixel format defines											
#define	II_PTN_YUV411_8_UYYVYY			"YUV411_8_UYYVYY"					
#define	II_PTN_YUV422_8_UYVY			"YUV422_8_UYVY"						
#define	II_PTN_YUV422_8					"YUV422_8"							
#define	II_PTN_YUV422_10				"YUV422_10"
#define	II_PTN_YUV422_10_PACKED			"YUV422_10_PACKED"
#define II_PTN_YUV444_8					"YUV444_8"
#define	II_PTN_YUV8_UYV					"YUV8_UYV"
#define	II_PTN_YCBCR8_CBYCR				"YCbCr8_CbYCr"
#define	II_PTN_YCBCR422_8				"YCbCr422_8"
#define	II_PTN_YCBCR422_8_CBYCRY		"YCbCr422_8_CbYCrY"
#define	II_PTN_YCBCR411_8_CBYYCRYY		"YCbCr411_8_CbYYCrYY"
#define	II_PTN_YCBCR601_8_CBYCR			"YCbCr601_8_CbYCr"
#define	II_PTN_YCBCR601_422_8			"YCbCr601_422_8"
#define	II_PTN_YCBCR601_422_8_CBYCRY	"YCbCr601_422_8_CbYCrY"
#define	II_PTN_YCBCR601_411_8_CBYYCRYY	"YCbCr601_411_8_CbYYCrYY"
#define	II_PTN_YCBCR709_8_CBYCR			"YCbCr709_8_CbYCr"
#define	II_PTN_YCBCR709_422_8			"YCbCr709_422_8"
#define	II_PTN_YCBCR709_422_8_CBYCRY	"YCbCr709_422_8_CbYCrY"
#define	II_PTN_YCBCR709_411_8_CBYYCRYY	"YCbCr709_411_8_CbYYCrYY"

//
//FleX packed pixel format defines
#define	II_PTN_RGB_8_PACKED			"RGB8_Packed"
#define	II_PTN_RGB_10_PACKED		"RGB_10_Packed"
#define	II_PTN_RGB_12_PACKED		"RGB_12_Packed"



//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////            Group Names of Pixel Types
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define	II_GNPT_MONO					"Mono"
#define	II_GNPT_BAYER					"Bayer"
#define	II_GNPT_TRUE_SENSE				"TrueSense"
#define	II_GNPT_RGB						"RGB"
#define	II_GNPT_BGR						"BGR"
#define	II_GNPT_YUV						"YUV"
#define	II_GNPT_YCBCR					"YCbCr"


typedef	int32_t GetIndexChannelTYPE(int16_t chName);

typedef enum : int16_t
{
	IIPC_NONE = 0,
	IIPC_ALFA = 'A',
	IIPC_GRAY = 'Y',
	IIPC_RED = 'R',
	IIPC_GREEN = 'G',
	IIPC_GREEN_2 = '2'<<8 | 'G',    // "G2"
	IIPC_BLUE = 'B',
	IIPC_X = 'X',
	IIPC_Y = 'Y',
	IIPC_Z = 'Z',
	IIPC_U = 'U',
	IIPC_V = 'V',
	IIPC_W = 'W',
	IIPC_CB = 'b'<<8 | 'C',
	IIPC_CR = 'r'<<8 | 'C',
	IIPC_YE = 'e'<<8 | 'Y',
	IIPC_IR = 'r'<<8 | 'I',
}II_PIXEL_CHANNELS;

static char* s_undefColorModel = "Undefined Color Model";

typedef struct _IpxColorModelDescription
{
    const char*				modelName;
    const char*				channelSequence;	// also defines converter type - source or destination
	int32_t					channels;
	int32_t					depth;
    GetIndexChannelTYPE*	GetIndex;
	int32_t					startPos;
	uint32_t			    pixelType;
} IpxColorModelDescription;

//==================================================================
IPX_INLINE int32_t GetIndexMONO (int16_t chName)
{
	switch(chName)
	{
	case IIPC_GRAY:
			return 0;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexRGB (int16_t chName)
{
	switch(chName)
	{
	case IIPC_RED:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_BLUE:
			return 2;
	default:
		return  -1;
	}
}

//==================================================================
IPX_INLINE int32_t GetIndexBGR (int16_t chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexRGBA (int16_t chName)
{
	switch(chName)
	{
	case IIPC_RED:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_BLUE:
			return 2;
	case IIPC_ALFA:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexBGRA (int16_t chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	case IIPC_ALFA:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexBAYER (int16_t chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	case IIPC_GREEN_2:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexXYZ (int16_t chName)
{
	switch(chName)
	{
	case IIPC_X:
			return 0;
	case IIPC_Y:
			return 1;
	case IIPC_Z:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
IPX_INLINE int32_t GetIndexYUV (int16_t chName)
{
	switch(chName)
	{
	case IIPC_Y:
			return 0;
	case IIPC_U:
			return 1;
	case IIPC_V:
			return 2;
	default:
		return  -1;
	}
}

static IpxColorModelDescription s_colorModelDescription[] =
{
	// MONO Pixel Types
	{II_PTN_MONO_8,              "Y", 1, 8,  GetIndexMONO, 0, II_PIX_MONO8},                       
	{II_PTN_MONO_10,             "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10},						
	{II_PTN_MONO_10_PACKED_PFNC, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10_PACKED_PFNC},			
	{II_PTN_MONO_10_PACKED,      "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10_PACKED_GEV},		
	{II_PTN_MONO_10_PACKED_FLEX, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10_PACKED_FLEX},
	{II_PTN_MONO_12,             "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12},						
	{II_PTN_MONO_12_PACKED_PFNC, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO12_PACKED_PFNC},			
	{II_PTN_MONO_12_PACKED,      "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12_PACKED_GEV},	
	{II_PTN_MONO_12_PACKED_FLEX, "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12_PACKED_FLEX},
	{II_PTN_MONO_14,             "Y", 1, 14, GetIndexMONO, 0, II_PIX_MONO14},								
	{II_PTN_MONO_16,             "Y", 1, 16, GetIndexMONO, 0, II_PIX_MONO16},						
	{II_PTN_MONO_8_PACKED_FLEX,  "Y", 1, 8,  GetIndexMONO, 0, II_PIX_MONO8_PACKED_FLEX},					

    // BAYER CFA Pixel Types                                                                                 
	{II_PTN_BAYER_GR8,"G R G2 B", 1, 8, GetIndexMONO,	 0, II_PIX_BAYGR8},									 
	{II_PTN_BAYER_RG8,"R G B G2", 1, 8, GetIndexMONO,	 0, II_PIX_BAYRG8},									 
	{II_PTN_BAYER_GB8,"G B G2 R", 1, 8, GetIndexMONO,	 0, II_PIX_BAYGB8},									 
	{II_PTN_BAYER_BG8,"B G R G2", 1, 8, GetIndexMONO,	 0, II_PIX_BAYBG8},									 
	{II_PTN_BAYER_GR10,"G R G2 B", 1, 10, GetIndexMONO,	 0, II_PIX_BAYGR10},								 
	{II_PTN_BAYER_RG10,"R G B G2", 1, 10, GetIndexMONO,	 0, II_PIX_BAYRG10},								 
	{II_PTN_BAYER_GB10,"G B G2 R", 1, 10, GetIndexMONO,	 0, II_PIX_BAYGB10},								 
	{II_PTN_BAYER_BG10,"B G R G2", 1, 10, GetIndexMONO,	 0, II_PIX_BAYBG10},								 
	{II_PTN_BAYER_GR12,"G R G2 B", 1, 12, GetIndexMONO,	 0, II_PIX_BAYGR12},								 
	{II_PTN_BAYER_RG12,"R G B G2", 1, 12, GetIndexMONO,	 0, II_PIX_BAYRG12},								 
	{II_PTN_BAYER_GB12,"G B G2 R", 1, 12, GetIndexMONO,	 0, II_PIX_BAYGB12},								 
	{II_PTN_BAYER_BG12,"B G R G2", 1, 12, GetIndexMONO,	 0, II_PIX_BAYBG12},								 
	{II_PTN_BAYER_GR10_PACKED_PFNC,"G R G2 B", 1, 10, GetIndexMONO,	0, II_PIX_BAYGR10_PACKED_PFNC},			 
	{II_PTN_BAYER_GR10_PACKED_GEV,    "G R G2 B", 1, 10, GetIndexMONO,	0, II_PIX_BAYGR10_PACKED_GEV},				 
	{II_PTN_BAYER_RG10_PACKED_PFNC,"R G B G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYRG10_PACKED_PFNC},			 
	{II_PTN_BAYER_RG10_PACKED_GEV,    "R G B G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYRG10_PACKED_GEV},				 
	{II_PTN_BAYER_GB10_PACKED_PFNC,"G B G2 R", 1, 10, GetIndexMONO,	0, II_PIX_BAYGB10_PACKED_PFNC},			 
	{II_PTN_BAYER_GB10_PACKED_GEV,    "G B G2 R", 1, 10, GetIndexMONO,	0, II_PIX_BAYGB10_PACKED_GEV},				 
	{II_PTN_BAYER_BG10_PACKED_PFNC,"B G R G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYBG10_PACKED_PFNC},			 
	{II_PTN_BAYER_BG10_PACKED_GEV,    "B G R G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYBG10_PACKED_GEV},				 
	{II_PTN_BAYER_GR12_PACKED_PFNC,"G R G2 B", 1, 12, GetIndexMONO,	0, II_PIX_BAYGR12_PACKED_PFNC},			 
	{II_PTN_BAYER_GR12_PACKED_GEV,    "G R G2 B", 1, 12, GetIndexMONO,	0, II_PIX_BAYGR12_PACKED_GEV},				 
	{II_PTN_BAYER_RG12_PACKED_PFNC,"R G B G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYRG12_PACKED_PFNC},			 
	{II_PTN_BAYER_RG12_PACKED_GEV,    "R G B G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYRG12_PACKED_GEV},				 
	{II_PTN_BAYER_GB12_PACKED_PFNC,"G B G2 R", 1, 12, GetIndexMONO,	0, II_PIX_BAYGB12_PACKED_PFNC},			 
	{II_PTN_BAYER_GB12_PACKED_GEV,    "G B G2 R", 1, 12, GetIndexMONO,	0, II_PIX_BAYGB12_PACKED_GEV},				 
	{II_PTN_BAYER_BG12_PACKED_PFNC,"B G R G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYBG12_PACKED_PFNC},			 
	{II_PTN_BAYER_BG12_PACKED_GEV,    "B G R G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYBG12_PACKED_GEV},				 
	{II_PTN_BAYER_GR14,"G R G2 B", 1, 14, GetIndexMONO,	0, II_PIX_BAYGR14},									 
	{II_PTN_BAYER_RG14,"R G B G2", 1, 14, GetIndexMONO,	0, II_PIX_BAYRG14},									 
	{II_PTN_BAYER_GB14,"G B G2 R", 1, 14, GetIndexMONO,	0, II_PIX_BAYGB14},									 
	{II_PTN_BAYER_BG14,"B G R G2", 1, 14, GetIndexMONO,	0, II_PIX_BAYBG14},									 
	{II_PTN_BAYER_GR16,"G R G2 B", 1, 16, GetIndexMONO,	0, II_PIX_BAYGR16},									 
	{II_PTN_BAYER_RG16,"R G B G2", 1, 16, GetIndexMONO,	0, II_PIX_BAYRG16},									 
	{II_PTN_BAYER_GB16,"G B G2 R", 1, 16, GetIndexMONO,	0, II_PIX_BAYGB16},									 
	{II_PTN_BAYER_BG16,"B G R G2", 1, 16, GetIndexMONO,	0, II_PIX_BAYBG16},									 

    // TRUESENSE SPARCE CFA Pixel Types                                                                                   
	{II_PTN_TS_BGGR_WBBW_0_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_8},			  
	{II_PTN_TS_BGGR_WBBW_1_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_8},			  
	{II_PTN_TS_BGGR_WBBW_2_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_8},			  
	{II_PTN_TS_BGGR_WBBW_3_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_8},			  
	{II_PTN_TS_GBRG_WGGW_0_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_8},			  
	{II_PTN_TS_GBRG_WGGW_1_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_8},			  
	{II_PTN_TS_GBRG_WGGW_2_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_8},			  
	{II_PTN_TS_GBRG_WGGW_3_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_8},			  
	{II_PTN_TS_GRBG_WGGW_0_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_8},			  
	{II_PTN_TS_GRBG_WGGW_1_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_8},			  
	{II_PTN_TS_GRBG_WGGW_2_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_8},			  
	{II_PTN_TS_GRBG_WGGW_3_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_8},			  
	{II_PTN_TS_RGGB_WRRW_0_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_8},			  
	{II_PTN_TS_RGGB_WRRW_1_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_8},			  
	{II_PTN_TS_RGGB_WRRW_2_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_8},			  
	{II_PTN_TS_RGGB_WRRW_3_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_8},			  
																														  
	{II_PTN_TS_BGGR_WBBW_0_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_10},		  
	{II_PTN_TS_BGGR_WBBW_1_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_10},		  
	{II_PTN_TS_BGGR_WBBW_2_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_10},		  
	{II_PTN_TS_BGGR_WBBW_3_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_10},		  
	{II_PTN_TS_GBRG_WGGW_0_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_10},		  
	{II_PTN_TS_GBRG_WGGW_1_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_10},		  
	{II_PTN_TS_GBRG_WGGW_2_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_10},		  
	{II_PTN_TS_GBRG_WGGW_3_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_10},		  
	{II_PTN_TS_GRBG_WGGW_0_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_10},		  
	{II_PTN_TS_GRBG_WGGW_1_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_10},		  
	{II_PTN_TS_GRBG_WGGW_2_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_10},		  
	{II_PTN_TS_GRBG_WGGW_3_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_10},		  
	{II_PTN_TS_RGGB_WRRW_0_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_10},		  
	{II_PTN_TS_RGGB_WRRW_1_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_10},		  
	{II_PTN_TS_RGGB_WRRW_2_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_10},		  
	{II_PTN_TS_RGGB_WRRW_3_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_10},		  

	{II_PTN_TS_BGGR_WBBW_0_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_12},		  
	{II_PTN_TS_BGGR_WBBW_1_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_12},		  
	{II_PTN_TS_BGGR_WBBW_2_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_12},		  
	{II_PTN_TS_BGGR_WBBW_3_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_12},		  
	{II_PTN_TS_GBRG_WGGW_0_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_12},		  
	{II_PTN_TS_GBRG_WGGW_1_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_12},		  
	{II_PTN_TS_GBRG_WGGW_2_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_12},		  
	{II_PTN_TS_GBRG_WGGW_3_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_12},		  
	{II_PTN_TS_GRBG_WGGW_0_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_12},		  
	{II_PTN_TS_GRBG_WGGW_1_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_12},		  
	{II_PTN_TS_GRBG_WGGW_2_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_12},		  
	{II_PTN_TS_GRBG_WGGW_3_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_12},		  
	{II_PTN_TS_RGGB_WRRW_0_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_12},		  
	{II_PTN_TS_RGGB_WRRW_1_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_12},		  
	{II_PTN_TS_RGGB_WRRW_2_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_12},		  
	{II_PTN_TS_RGGB_WRRW_3_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_12},		  
																														  
	{II_PTN_TS_BGGR_WBBW_0_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_14},		  
	{II_PTN_TS_BGGR_WBBW_1_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_14},		  
	{II_PTN_TS_BGGR_WBBW_2_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_14},		  
	{II_PTN_TS_BGGR_WBBW_3_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_14},		  
	{II_PTN_TS_GBRG_WGGW_0_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_14},		  
	{II_PTN_TS_GBRG_WGGW_1_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_14},		  
	{II_PTN_TS_GBRG_WGGW_2_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_14},		  
	{II_PTN_TS_GBRG_WGGW_3_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_14},		  
	{II_PTN_TS_GRBG_WGGW_0_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_14},		  
	{II_PTN_TS_GRBG_WGGW_1_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_14},		  
	{II_PTN_TS_GRBG_WGGW_2_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_14},		  
	{II_PTN_TS_GRBG_WGGW_3_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_14},		  
	{II_PTN_TS_RGGB_WRRW_0_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_14},		  
	{II_PTN_TS_RGGB_WRRW_1_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_14},		  
	{II_PTN_TS_RGGB_WRRW_2_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_14},		  
	{II_PTN_TS_RGGB_WRRW_3_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_14},		  

	// RGB Pixel Types                                                                                
	{II_PTN_RGB_8,              "R G B",   3, 8,  GetIndexRGB,	0, II_PIX_RGB8},		      	                		 
	{II_PTN_BGR_8,              "B G R",   3, 8,  GetIndexBGR,	0, II_PIX_BGR8},									 
	{II_PTN_RGBA_8,             "R G B A", 4, 8,  GetIndexRGBA,	0, II_PIX_RGBA8},										 
	{II_PTN_BGRA_8,             "B G R A", 4, 8,  GetIndexBGRA,	0, II_PIX_BGRA8},										 
	{II_PTN_RGB_10,             "R G B",   3, 10, GetIndexRGB,	0, II_PIX_RGB10},										 
	{II_PTN_BGR_10,             "B G R",   3, 10, GetIndexBGR,	0, II_PIX_BGR10},										 
	{II_PTN_RGB_12,             "R G B",   3, 12, GetIndexRGB,	0, II_PIX_RGB12},										 
	{II_PTN_BGR_12,             "B G R",   3, 12, GetIndexBGR,	0, II_PIX_BGR12},										 
	{II_PTN_RGB_14,             "R G B",   3, 14, GetIndexRGB,	0, II_PIX_RGB14},			  							 
	{II_PTN_BGR_14,             "B G R",   3, 14, GetIndexBGR,	0, II_PIX_BGR14},										 
	{II_PTN_RGB_16,             "R G B",   3, 16, GetIndexRGB,  0, II_PIX_RGB16},										 
	{II_PTN_BGR_16,             "B G R",   3, 16, GetIndexBGR,  0, II_PIX_BGR16},									     
	{II_PTN_RGB_8_PACKED_FLEX,  "R G B",   3, 8,  GetIndexRGB,  0, II_PIX_RGB8_PACKED_FLEX},						 
	{II_PTN_RGB_10_PACKED_FLEX, "R G B",   3, 10, GetIndexRGB,  0, II_PIX_RGB10_PACKED_FLEX},					
	{II_PTN_RGB_12_PACKED_FLEX, "R G B",   3, 12, GetIndexRGB,  0, II_PIX_RGB12_PACKED_FLEX},					
		
	// YUV Pixel Type                                                                            
	{II_PTN_YUV422_8_UYVY, "U Y V Y",    3, 8, GetIndexYUV, 0, II_PIX_YUV422_8_UYVY},				 
	{II_PTN_YUV422_8,      "Y U Y V",    3, 8, GetIndexYUV, 0, II_PIX_YUV422_8},					 
	{II_PTN_YUV422_10,     "Y U Y V",    3, 10,GetIndexYUV, 0, II_PIX_YUV422_10},						 
	{II_PTN_YUV422_10_PACKED, "Y U Y V", 3, 10,GetIndexYUV, 0, II_PIX_YUV422_10_PACKED},		 
	{II_PTN_YUV444_8,      "Y U V A",    4, 8, GetIndexYUV, 0, II_PIX_YUV444_8},						 
	{II_PTN_YCBCR422_8,    "Y Cb Y Cr",  3, 8, GetIndexYUV, 0, II_PIX_YCBCR422_8}					 
	
};


/**< Number of pixel types used by IpxTools libraries */
static uint32_t II_PIX_TYPES_NUMBER = sizeof(s_colorModelDescription) / sizeof(IpxColorModelDescription);

#ifdef _DEBUG
	#include <Windows.h>
	#include <crtdbg.h>
#endif

//////////////////////////////////////////////////////////////////////
/// Base type of data for description of IpxImage and other image data types.
//================================================================================

/// Data structure for description of Region of interest.
//================================================================================
/**
* \note
* IpxROI structure defines the Region of interest (ROI) of the image.
*
* \see IpxImage<BR/>IpxGetActualROI
*/
//typedef struct _IpxROI
//{
//	_IpxROI()	{xOffset = yOffset = width = height = 0;};
//	_IpxROI(int32_t x, int32_t y, int32_t w, int32_t h )	{ xOffset = x; yOffset = y; width = w; height = h; };
//    int32_t		xOffset; /**< X Offset of ROI's rectangle */
//    int32_t		yOffset; /**< Y Offset of ROI's rectangle */
//    int32_t		width;   /**< Width of ROI's rectangle  */
//    int32_t		height;  /**< Height of ROI's rectangle  */
//}IpxROI;
//


/// Data structure for description of Channel of interest.
//================================================================================
/**
* \note
* IpxCOI structure defines the Channel of interest (COI) of the image.
*
* \see IpxImage<BR/>IpxGetIndexesFromCOI<BR/>IpxConvertChannelStr
*/
//typedef struct _IpxCOI
//{
//	_IpxCOI()	{ indNumber = 0; };
//    int32_t		indexes[MAX_IMAGE_CHANNELS];	/**<  indexes of color channels  */
//    int32_t		indNumber;						/**<  the number of indexes  */ 
//}IpxCOI;
//
//  
/// Data structure for description of pixel format.
//================================================================================
/**
* \note
* IpxPixelTypeDescr stores parameters of the pixel format.
*
* \see II_PIXEL_TYPE_DEFINES<BR/>IpxCreateImage<BR/>IpxCreateImageHeader<BR/>IpxReleaseImage
*/
typedef struct _IpxPixelTypeDescr
{
	_IpxPixelTypeDescr()	{pixelType = depth = pixAlign = channels = pixSize = 0; pixSigned = false;};
	uint32_t	pixelType;   /**< Pixel type. */
	uint32_t	depth;	     /**< Bit depth of channels. */
	bool		pixSigned;   /**< true for signed pixel. */
	uint32_t	pixAlign;    /**< Pixel packing (packed/normalized). */
	uint32_t	channels;    /**< Number of channels. */
	uint32_t	pixSize;     /**< Pixel size in bits. */
} IpxPixelTypeDescr;


//////////////////////////////////////////////////////////////////////
/// \defgroup ref_pixel IpxPixelType Header
/// \ingroup ref_data 
///@{

/// Returns the number of Pixel Types (Color Models) that are supported by this header file.
//================================================================================
/**
* \return 
* The return value is the number of Pixel Types.
*
*/
IPX_INLINE int32_t IpxGetPixelTypesNumber()
{
	return II_PIX_TYPES_NUMBER;
}

/// Defines whether the number is the pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is 'true' if pixType is pixel type.
*
*/
IPX_INLINE bool IpxIsPixelType(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return false;
	else if (s_colorModelDescription[descrIndex].pixelType == pixelType)
		return true;
	else
		return false;
}

/// Defines whether the pixel type is a member of a group.
//================================================================================
/**
* @param groupName Group name or some substring in Color Model Name.
* @param pixelType Pixel type.
* \return 
* The return value is 'true' if pixType is a member of group.
*
*/
IPX_INLINE bool IpxIsGroup(char* groupName, uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return false;
	else 
		return (::strstr(s_colorModelDescription[descrIndex].modelName, groupName)) ? true : false;
}

/// Defines color model descriptor by Pixel Type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is pointer to color model descriptor.
*
*/
IPX_INLINE const IpxColorModelDescription* IpxGetColorModelDescription(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return NULL;
	else
		return &s_colorModelDescription[descrIndex];
}

/// Defines color model descriptor by an index.
//================================================================================
/**
* @param index descriptor index.
* \return 
* The return value is pointer to color model descriptor.
*
*/
IPX_INLINE const IpxColorModelDescription* IpxGetColorModelDescr(uint32_t index)
{
	if (index >= II_PIX_TYPES_NUMBER)
		return NULL;
	else
		return &s_colorModelDescription[index];
}

/// Defines pixel type by name of color model.
//================================================================================
/**
* @param colorModelName Name of color model.
* \return 
* The return value is pixel type.
*
*/
IPX_INLINE uint32_t IpxGetPixelType(char* colorModelName)
{
	if (colorModelName)
	{
		for (uint32_t i=0; i<II_PIX_TYPES_NUMBER; i++)
		{
			if (!::strcmp(s_colorModelDescription[i].modelName, colorModelName))
				return s_colorModelDescription[i].pixelType;
		}
	}

	return II_PIX_NONE_TYPE;
}

/// Defines name of color model by pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is name of color model.
*
*/
IPX_INLINE const char* IpxGetColorModelName(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return s_undefColorModel;
	else
		return s_colorModelDescription[descrIndex].modelName;
}

/// Defines sequence of channels.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is sequence of channels.
*
*/
IPX_INLINE const char* IpxGetChannelSequence(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return s_undefColorModel;
	else
		return s_colorModelDescription[descrIndex].channelSequence;
}

/// Defines number of channels.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is number of channels.
*
*/
IPX_INLINE int32_t IpxGetChannelsNumber(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].channels;
}

/// Defines depth of color channel.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is depth of color channel.
*
*/
IPX_INLINE int32_t IpxGetChannelsDepth(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].depth;
}

/// Defines start position in a CFA.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is start position in a CFA.
*
*/
IPX_INLINE int32_t IpxGetStartPosition(uint32_t pixelType)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].startPos;
}

/// Defines index of color channel.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chName Channel name.
* \return 
* The return value is index of color channel.
*
*/
IPX_INLINE int32_t IpxGetChannelIndex(uint32_t pixelType, int16_t chName)
{
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].GetIndex(chName);
}

/// Checks channel names.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chNames Array of channel names.
* @param channels Number of checked names.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is -1.
*
*/
IPX_INLINE IpxError IpxCheckChannelNames(uint32_t pixelType, int16_t* chNames, int32_t channels)
{
	GetIndexChannelTYPE*	GetIndex = NULL;
	uint32_t descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		GetIndex = s_colorModelDescription[descrIndex].GetIndex;

	for (int32_t i=0; i<channels; i++)
	{
		if (GetIndex(chNames[i]) < 0)
			return -1;
	}
	return 0;
}

/// Converts string to array of channel names.
//================================================================================
/**
* @param nameStr String of channel names.
* @param sep Separator of channel names in the string.
* @param chNames Array of channel names.
* @param channels Number of channel names.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is -1.
*
*/
IPX_INLINE IpxError IpxConvertChannelStr(char* nameStr, char* sep, int16_t* chNames, int32_t* channels)
{
	if (!nameStr || !chNames || !channels)
		return -1;
	int32_t i = 0;
	int32_t ii = *channels;
	char *token1, *next_token1;

	token1 = strtok_s(nameStr, sep, &next_token1);

	while (token1 != NULL)
	{
		if (i < ii)
			chNames[i++] = *((int16_t*)token1);

		token1 = strtok_s(NULL, sep, &next_token1);

	}
	*channels = i;
	return 0;
}

/// Gets channel name.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chnlIndx Channel index.
* \return 
* The return value is channel name.
*
*/
IPX_INLINE int16_t IpxGetChannelName(uint32_t pixelType, int32_t chnlIndx)
{
	const char* tmpChnlStr = IpxGetChannelSequence(pixelType);
	char chnlStr[16];

	::strncpy_s(chnlStr, 16, tmpChnlStr, 16);

	int16_t chNames[MAX_IMAGE_CHANNELS];
	int32_t channels	= MAX_IMAGE_CHANNELS;
	if (0 != IpxConvertChannelStr(chnlStr, " ", chNames, &channels))
	{
		return IIPC_NONE;
	}
	if (0 > chnlIndx || chnlIndx >= channels)
		return IIPC_NONE;
	else
		return chNames[chnlIndx];
}

///// Converts string to Channels Of Interest (COI).
////================================================================================
///**
//* @param pixelType Pixel type.
//* @param nameStr String of channel names.
//* @param sep Separator of channel names in the string.
//* @param coi Channels of intrest.
//* \return 
//* If the function succeeds, the return value is 0.
//* If the function fails, the return value is -1.
//*
//*/
//IPX_INLINE IpxError IpxConvertChannelStr(uint32_t pixelType, char* nameStr, char* sep, IpxCOI* coi)
//{
//	char *token1, *next_token1;
//	int16_t chName = IIPC_NONE;
//	int32_t indx, ii, i = 0;
//	if (!nameStr || !sep || !coi || !IpxIsPixelType(pixelType))
//		return -1;
//	ii = IpxGetChannelsNumber(pixelType);
//
//	token1 = strtok_s(nameStr, sep, &next_token1);
//
//	while (token1 != NULL)
//	{
//		if (i >= ii)
//			break;
//		chName = *((int16_t*)token1);
//		indx = IpxGetChannelIndex(pixelType, chName);
//		if (-1 == indx)
//		{
//			coi->indNumber = 0;
//			return -1;
//		}
//		coi->indexes[i++] = indx;
//
//		token1 = strtok_s(NULL, sep, &next_token1);
//
//	}
//	coi->indNumber = i;
//	return 0;
//}
//
///// Converts array of channel names to Channels Of Interest (COI).
////================================================================================
///**
//* @param coi Channels of interest.
//* @param chNames Array of channel names.
//* @param channels Number of channel names.
//* @param pixelType Pixel type.
//* \return 
//* If the function succeeds, the return value is 'true'.
//* If the function fails, the return value is 'false'.
//*
//*/
//IPX_INLINE bool IpxCOIFrom(IpxCOI* coi, int16_t* chNames, int32_t* channels, uint32_t pixelType)
//{
//	int32_t indx, ii, i;
//	if (!chNames || !channels || !coi || !IpxIsPixelType(pixelType))
//		return false;
//	ii = IpxGetChannelsNumber(pixelType);
//	ii = (*channels < ii)? (*channels) : ii;
//
//	for (i=0; i<ii; i++)
//	{
//		indx = IpxGetChannelIndex(pixelType, chNames[i]);
//		if (-1 == indx)
//		{
//			coi->indNumber = 0;
//			return false;
//		}
//		coi->indexes[i] = indx;
//	}
//	coi->indNumber = ii;
//	
//	return true;
//}
//
///// Defines Channels Of Interest for pixel type.
////================================================================================
///**
//* @param coi Channels of interest.
//* @param pixelType Pixel type.
//* \return 
//* If the function succeeds, the return value is 'true'.
//* If the function fails, the return value is 'false'.
//*
//*/
//IPX_INLINE bool IpxCOIFrom(IpxCOI* coi, uint32_t pixelType)
//{
//	int32_t ii, i;
//	if (!coi || !IpxIsPixelType(pixelType))
//		return false;
//	ii = IpxGetChannelsNumber(pixelType);
//	for (i=0; i<ii; i++)
//	{
//		coi->indexes[i] = i;
//	}
//	coi->indNumber = ii;
//	
//	return true;
//}
//
///// Defines Channels Of Interest for array of indexes
////================================================================================
///**
//* @param coi Channels of interest.
//* @param indexes  Pointer to buffer with indexes.
//* @param size  Number or of elements in <b>indexes</b> buffer.
//* \return 
//* If the function succeeds, the return value is 'true'.
//* If the function fails, the return value is 'false'.
//*
//*/
//IPX_INLINE  bool IpxCOIFrom(IpxCOI* coi, int32_t* indexes, int32_t size)
//{
//	int32_t ii, i;
//	if (!indexes || !size || !coi)
//		return false;
//	ii = (size < MAX_IMAGE_CHANNELS)?size : MAX_IMAGE_CHANNELS;
//	for (i=0; i<ii; i++)
//	{
//		if (indexes[i] >= MAX_IMAGE_CHANNELS)
//		{
//			coi->indNumber = 0;
//			return false;
//		}
//		else
//		{
//			coi->indexes[i] = indexes[i];
//		}
//	}
//	coi->indNumber = ii;
//	
//	return true;
//}
//
//
///// Obtains the array of indexes from COI
////================================================================================
///**
//* @param coi Pointer to input COI of image.
//* @param indexes  Pointer to output buffer with indexes.
//* @param size  Number or of elements in output <b>indexes</b> buffer.
//* \return 
//* If the function succeeds, the return value is 'true'.
//* If the function fails, the return value is 'false'.
//*/
//IPX_INLINE  bool IpxGetIndexesFromCOI(const IpxCOI* coi, int32_t* indexes, int32_t* size)
//{
//	int32_t ii, i;
//	if (!indexes || !size)
//		return false;
//	ii = coi->indNumber;
//	if (*size < ii)
//	{
//		*size = ii;
//		return false;
//	}
//	for (i=0; i<ii; i++)
//	{
//		indexes[i] = coi->indexes[i];
//	}
//	*size = ii;
//
//	return true;
//}
//


///@}

//////////////////////////////////////////////////////////////////////
#endif // _IPX_PIXEL_TYPE_H
