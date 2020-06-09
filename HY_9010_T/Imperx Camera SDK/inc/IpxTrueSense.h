////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxTrueSense.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxTrueSense component interface description
// Function and Constant Declarations for TRUESENSE SPARCE CFA demosaicing 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author: Maxim Bokov (maxb@imperx.com)
// Created:	13-APR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2013-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef _TRUE_SENSE_H_
#define _TRUE_SENSE_H_

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifndef TS_EXTERN_C
	#ifdef __cplusplus
		#define TS_EXTERN_C extern "C"
	#else
		#define TS_EXTERN_C
	#endif
#endif // TS_EXTERN_C

#ifndef TS_CALL
	#ifdef _WIN32 
		#define TS_CALL __stdcall
	#else
		#define TS_CALL
	#endif // _WIN32
#endif // TS_CALL

// check if static library is wanted
#ifdef TS_STATIC
	// define the static library symbol
	#define TS_API
	#define TS_LOCAL
#else
	#define TS_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef TS_EXPORTS
		// define the export symbol
		#define TS_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxTrueSense_ConvertImage=_IpxTrueSense_ConvertImage@12")
			#pragma comment(linker, "/EXPORT:IpxTrueSense_CreateComponent=_IpxTrueSense_CreateComponent@0")
			#pragma comment(linker, "/EXPORT:IpxTrueSense_DeleteComponent=_IpxTrueSense_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxTrueSense_GetComponent=_IpxTrueSense_GetComponent@4")
			#pragma comment(linker, "/EXPORT:IpxTrueSense_AllocData=_IpxTrueSense_AllocData@12")
			#pragma comment(linker, "/EXPORT:IpxTrueSense_ReleaseData=_IpxTrueSense_ReleaseData@4")
		#endif
	#else // TS_EXPORTS
		// define the import symbol
		#define TS_API LIB_HELPER_DLL_IMPORT
	#endif // TS_EXPORTS
#endif

///! IpxTrueSense component error codes
#define IPX_ERR_TS_INVALID_ARGUMENT IPX_ERR(IPX_CMP_TS_DEMOSAICING, IPX_ERR_INVALID_ARGUMENT)
#define IPX_ERR_TS_UNKNOWN          IPX_ERR(IPX_CMP_TS_DEMOSAICING, IPX_ERR_UNKNOWN)
#define IPX_ERR_TS_OUT_OF_RANGE        IPX_ERR(IPX_CMP_TS_DEMOSAICING, IPX_ERR_OUT_OF_RANGE)

//////////////////////////////////////////////////////
/// \defgroup ipxtruesense IpxTrueSense IpxComponent Header
/// \ingroup ipxdemosaicing
/// \brief TrueSense functions and classes with IpxComponent features
/// The following items are exists in IpxTrueSense
///@{
//////////////////////////////////////////////////////

/** \addtogroup ts_alg TS CFA Demosaicing algorithm Parameters
\brief Defines for TS CFA Demosaicing algorithms
*  @{
<table>
		<caption id="multi_row">TS CFA Demosaicing Algorithm Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_ALGO_TYPE</b><td>"TrueSenseAlgType"<td>[int: 0, TS_ALGO_NUM - 1]<td>TrueSense Algorithm Type
		<tr><td rowspan="1"><b>TS_NOREALLOC</b><td>"NoRealloc"<td>[int: 0,1]<td>NoRealloc Enabled
</table>*/

#define TS_ALGO_TYPE	"TrueSenseAlgType"  /*!< TrueSense Algorithm Type \n\n<b>Type/Range</b>    [int: 0, TS_ALGO_NUM - 1] \note Used by SetParamInt and GetParamInt*/
#define TS_NOREALLOC	"NoRealloc"			/*!< NoRealloc Enabled \n\n<b>Type/Range</b>    [int: 0,1] \note Used by SetParamInt and GetParamInt*/
/**@}*/

/** \addtogroup ts_algtype2 TS CFA Algorithm Types
\brief Defines for TS CFA Algorithm Types
*  @{*/
#define TS_ALGO_NUM			7		/*!< Number of Algorithms Supported */
#define TSASIMPLEF			0		/*!< Simple algorithm. Average quality, high speed. You can set tonescale table in TrueSenseParam structure. */
#define TSASIMPLES			1		/*!< Simple Quality algorithm. Average quality, high speed. You can set tonescale table and white balance coefficients in TrueSenseParam structure. */
#define TSABAYERLIKE		2		/*!< Simple Bayer-like algorithm. Average quality, high speed. You can set tonescale table in TrueSenseParam structure. */
#define TSAMEDIUM			3		/*!< High Quality algorithm. High quality, medium speed. You can set all of the adjusting parameters in TrueSenseParam structure. */
#define TSAQUALITY			4		/*!< High Quality algorithm. High quality, very low speed. You can set all of the adjusting parameters in TrueSenseParam structure.  */
#define TRUES_OPENGL_MHC    5		/*!< OpenGL MHC algorithm. */
#define TRUES_OPENGL_MMA    6		/*!< OpenGL MMA algorithm. */
/**@}*/

/** \addtogroup ts_1 TS Misc Parameters
\brief Defines for TS Misc parameters
*  @{
<table>
		<caption id="multi_row">TS Misc Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_THREADS_NUM</b><td>"threads_num"<td>[int: 0-32]<td>Quantity of threads used in calculation. Default value is 0, it means maximum number of available threads
		<tr><td rowspan="1"><b>TS_NORM_EN</b><td>"normalizationEnable"<td>[int: 0,1]<td>Enable normalization. 0 - off, 1 - on. Default value is off
		<tr><td rowspan="1"><b>TS_HORIZ_MIRRORED</b><td>"horMirrored"<td>[int: 0,1]<td>If image is mirrored horizontally. Default value is 0. 
		<tr><td rowspan="1"><b>TS_VER_MIRRORED</b><td>"verMirrored"<td>[int: 0,1]<td>If image is mirrored vertically. Default value is 0.
		<tr><td rowspan="1"><b>TS_MONO_ENABLED</b><td>"monoEnable"<td>[int: 0,1]<td>Switch on monochrome processing instead of color processing. 0 - color, 1 - monochrome. Reserved.
		<tr><td rowspan="1"><b>TS_IMP_FILTER_ENABLED</b><td>"impulseFilterEnable"<td>[int: 0,1]<td>Enable the impulse filter processing. 0 - off, 1 - on.
		<tr><td rowspan="1"><b>TS_SHARPNESS_ENABLED</b><td>"sharpnessEnable"<td>[int: 0,1]<td>Enable the sharpness processing. 0 - off, 1 - on.
		<tr><td rowspan="1"><b>TS_DARKFLOOR</b><td>"darkFloor"<td>[int: 0-4096]<td>Dark floor of raw image, fetched from raw file header.
</table>*/
#define TS_THREADS_NUM			"threads_num"			/*!< Quantity of threads used in calculation. Default value is 0, it means maximum number of available threads \n\n<b>Type/Range</b>    [int: 0-32]\note Used by SetParamInt and GetParamInt*/
#define TS_NORM_EN				"normalizationEnable"	/*!< Enable normalization. 0 - off, 1 - on. Default value is off. \n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_HORIZ_MIRRORED		"horMirrored"			/*!< If image is mirrored horrizontally. Default value is 0. \n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_VER_MIRRORED			"verMirrored"			/*!< If image is mirrored vertically. Default value is 0. \n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_MONO_ENABLED			"monoEnable"			/*!< Switch on monochrome processing instead of color processing. 0 - color, 1 - monochrome. Reserved. \n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_IMP_FILTER_ENABLED 	"impulseFilterEnable"	/*!< Enable the impulse filter processing. 0 - off, 1 - on.\n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_SHARPNESS_ENABLED	"sharpnessEnable"		/*!< Enable the sharpness processing. 0 - off, 1 - on.\n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define TS_DARKFLOOR			"darkFloor"				/*!< Dark floor of raw image, fetched from raw file header. \n\n<b>Type/Range</b>    [int: 0,1]\note Used by SetParamInt and GetParamInt*/
/**@}*/

/** \addtogroup ts_2 TS Gain Parameters
\brief Defines for TS gain parameters
*  @{
<table>		
		<caption id="multi_row">TS Gain Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_RED_GAIN</b><td>"redGain"<td>[float: DBL_MIN-DBL_MAX]<td>Red gain of white balance.
		<tr><td rowspan="1"><b>TS_GREEN_GAIN</b><td>"greenGain"<td>[float: DBL_MIN-DBL_MAX]<td>Green gain of white balance.
		<tr><td rowspan="1"><b>TS_BLUE_GAIN</b><td>"blueGain"<td>[float: DBL_MIN-DBL_MAX]<td>Blue gain of white balance.
		<tr><td rowspan="1"><b>TS_PAN_GAIN</b><td>"panGain"<td>[float: DBL_MIN-DBL_MAX]<td>Panchromatic gain of white balance. It should be set as 1 currently.
		<tr><td rowspan="1"><b>TS_GLOBAL_GAIN</b><td>"globalGain"<td>[float: DBL_MIN-DBL_MAX]<td>Digital gain. It will be applied to processing if more than 1.0
		<tr><td rowspan="1"><b>TS_ANALOG_GAIN</b><td>"analogGain"<td>[float: DBL_MIN-DBL_MAX]<td>Actual sensor gain of raw image, fetched from raw file header. 
		<tr><td rowspan="1"><b>TS_ISO_ANALOGGAIN_0</b><td>"ISOAnalogGain_0"<td>[float: DBL_MIN-DBL_MAX]<td>Sensor gain array of typical ISO levels, used to interpolate intermediate noise variation slope and intercept.
		<tr><td rowspan="1"><b>TS_ISO_ANALOGGAIN_1</b><td>"ISOAnalogGain_1"<td>[float: DBL_MIN-DBL_MAX]<td>Sensor gain array of typical ISO levels, used to interpolate intermediate noise variation slope and intercept.
		<tr><td rowspan="1"><b>TS_ISO_ANALOGGAIN_2</b><td>"ISOAnalogGain_2"<td>[float: DBL_MIN-DBL_MAX]<td>Sensor gain array of typical ISO levels, used to interpolate intermediate noise variation slope and intercept.
		<tr><td rowspan="1"><b>TS_ISO_ANALOGGAIN_3</b><td>"ISOAnalogGain_3"<td>[float: DBL_MIN-DBL_MAX]<td>Sensor gain array of typical ISO levels, used to interpolate intermediate noise variation slope and intercept.
		<tr><td rowspan="1"><b>TS_ISO_ANALOGGAIN_4</b><td>"ISOAnalogGain_4"<td>[float: DBL_MIN-DBL_MAX]<td>Sensor gain array of typical ISO levels, used to interpolate intermediate noise variation slope and intercept.
</table>*/
#define TS_RED_GAIN				"redGain"				/*!< Red gain of white balance. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_GREEN_GAIN			"greenGain"				/*!< Green gain of white balance. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_BLUE_GAIN			"blueGain"				/*!< Blue gain of white balance. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_GAIN				"panGain"				/*!< Panchromatic gain of white balance. It should be set as 1 currently. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_GLOBAL_GAIN			"globalGain"			/*!< Digital gain. It will be applied to processing if more than 1.0. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_ANALOG_GAIN			"analogGain"			/*!< Actual sensor gain of raw image, fetched from raw file header. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_ISO_ANALOGGAIN_0		"ISOAnalogGain_0"		/*!< Sensor gain array of typical ISO levels\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope and intercept.	Ex: ISO400_AnalogGain = 11.04 dB*/
#define TS_ISO_ANALOGGAIN_1		"ISOAnalogGain_1"		/*!< Sensor gain array of typical ISO levels\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope and intercept.	Ex: ISO800_AnalogGain = 17.69 dB*/
#define TS_ISO_ANALOGGAIN_2		"ISOAnalogGain_2"		/*!< Sensor gain array of typical ISO levels\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope and intercept.	Ex: ISO1600_AnalogGain = 23.96 dB*/
#define TS_ISO_ANALOGGAIN_3		"ISOAnalogGain_3"		/*!< Sensor gain array of typical ISO levels\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope and intercept.	Ex: ISO3200_AnalogGain = 29.91 dB*/
#define TS_ISO_ANALOGGAIN_4		"ISOAnalogGain_4"		/*!< Sensor gain array of typical ISO levels\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope and intercept.	Ex:	ISO6400_AnalogGain = 36.77 dB*/
/**@}*/

/** \addtogroup ts_3 TS ISO Panchromatic Channel Parameters
\brief Defines for TS ISO Panchromatic channel parameters
*  @{
<table>		
		<caption id="multi_row">TS ISO Panchromatic Channel Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_ISO_PANSLOPE_0</b><td>"ISOPanSlope_0"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANSLOPE_1</b><td>"ISOPanSlope_1"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANSLOPE_2</b><td>"ISOPanSlope_2"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANSLOPE_3</b><td>"ISOPanSlope_3"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANSLOPE_4</b><td>"ISOPanSlope_4"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANINTERCEPT_0</b><td>"ISOPanSlope_0"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANINTERCEPT_1</b><td>"ISOPanSlope_1"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANINTERCEPT_2</b><td>"ISOPanSlope_2"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANINTERCEPT_3</b><td>"ISOPanSlope_3"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of panchromatic channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_PANINTERCEPT_4</b><td>"ISOPanSlope_4"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of panchromatic channel at typical sensor gains
</table>*/
#define TS_ISO_PANSLOPE_0		"ISOPanSlope_0" /*!< Noise variation slope of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO400_PanSlope = 0.31793097*/
#define TS_ISO_PANSLOPE_1		"ISOPanSlope_1" /*!< Noise variation slope of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO800_PanSlope = 0.6009852*/
#define TS_ISO_PANSLOPE_2		"ISOPanSlope_2" /*!< Noise variation slope of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO1600_PanSlope = 1.16587611*/
#define TS_ISO_PANSLOPE_3		"ISOPanSlope_3" /*!< Noise variation slope of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO3200_PanSlope = 2.26059552*/
#define TS_ISO_PANSLOPE_4		"ISOPanSlope_4" /*!< Noise variation slope of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO6400_PanSlope = 5.11044291*/
#define TS_ISO_PANINTERCEPT_0	"ISOPanIntercept_0" /*!< Noise variation intercept of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO400_PanIntercept = -25.07685652*/
#define TS_ISO_PANINTERCEPT_1	"ISOPanIntercept_1" /*!< Noise variation intercept of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO800_PanIntercept = 17.01752105*/
#define TS_ISO_PANINTERCEPT_2	"ISOPanIntercept_2" /*!< Noise variation intercept of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO1600_PanIntercept = 185.43026*/
#define TS_ISO_PANINTERCEPT_3	"ISOPanIntercept_3" /*!< Noise variation intercept of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO3200_PanIntercept = 831.07495077*/
#define TS_ISO_PANINTERCEPT_4	"ISOPanIntercept_4" /*!< Noise variation intercept of panchromatic channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO6400_PanIntercept = 4154.73883603*/
/**@}*/
	
/** \addtogroup ts_4 TS ISO Color Slope Parameters
\brief Defines for TS ISO Color Slope parameters
*  @{
<table>		
		<caption id="multi_row">TS ISO Color Slope Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_ISO_COLORSLOPE_0</b><td>"ISOColorSlope_0"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORSLOPE_1</b><td>"ISOColorSlope_1"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORSLOPE_2</b><td>"ISOColorSlope_2"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORSLOPE_3</b><td>"ISOColorSlope_3"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORSLOPE_4</b><td>"ISOColorSlope_4"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation slope of color channel at typical sensor gains
</table>*/
#define TS_ISO_COLORSLOPE_0     "ISOColorSlope_0"   /*!< Noise variation slope of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO400_ColorSlope = 0.16289523 */
#define TS_ISO_COLORSLOPE_1     "ISOColorSlope_1"   /*!< Noise variation slope of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO800_ColorSlope = 0.30242107 */
#define TS_ISO_COLORSLOPE_2     "ISOColorSlope_2"   /*!< Noise variation slope of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO1600_ColorSlope = 0.58180185 */
#define TS_ISO_COLORSLOPE_3     "ISOColorSlope_3"   /*!< Noise variation slope of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO3200_ColorSlope = 1.15281985 */
#define TS_ISO_COLORSLOPE_4     "ISOColorSlope_4"   /*!< Noise variation slope of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation slope. Ex: ISO6400_ColorSlope = 2.53400236 */
/**@}*/    										

/** \addtogroup ts_5 TS ISO Color Intercept Parameters
\brief Defines for TS ISO Color Intercept parameters
*  @{
<table>		
		<caption id="multi_row">TS ISO Color Intercept Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_ISO_COLORINTERCEPT_0</b><td>"ISOColorIntercept_0"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORINTERCEPT_1</b><td>"ISOColorIntercept_1"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORINTERCEPT_2</b><td>"ISOColorIntercept_2"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORINTERCEPT_3</b><td>"ISOColorIntercept_3"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of color channel at typical sensor gains
		<tr><td rowspan="1"><b>TS_ISO_COLORINTERCEPT_4</b><td>"ISOColorIntercept_4"<td>[float: DBL_MIN-DBL_MAX]<td>Noise variation intercept of color channel at typical sensor gains
</table>*/
#define TS_ISO_COLORINTERCEPT_0 "ISOColorIntercept_0"	/*!< Noise variation intercept of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO400_ColorIntercept = -2.97408598*/
#define TS_ISO_COLORINTERCEPT_1 "ISOColorIntercept_1"	/*!< Noise variation intercept of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO800_ColorIntercept = 15.97559859*/
#define TS_ISO_COLORINTERCEPT_2 "ISOColorIntercept_2"	/*!< Noise variation intercept of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO1600_ColorIntercept = 92.84640595*/
#define TS_ISO_COLORINTERCEPT_3 "ISOColorIntercept_3"	/*!< Noise variation intercept of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO3200_ColorIntercept = 399.49923562*/
#define TS_ISO_COLORINTERCEPT_4 "ISOColorIntercept_4"	/*!< Noise variation intercept of color channel at typical sensor gains\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, used to interpolate intermediate noise variation intercept. Ex: ISO6400_ColorIntercept = 2080.24259272*/
/**@}*/

/** \addtogroup ts_6 TS Sigma Filter Parameters
\brief Defines for TS Sigma Filter parameters
*  @{
<table>		
		<caption id="multi_row">TS ISO Panchromatic Channel Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_PAN_RADIUS0</b><td>"panRadius0"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of first round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_PAN_RADIUS1</b><td>"panRadius1"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of second round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_PAN_RADIUS2</b><td>"panRadius2"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of third round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_PAN_SIGMA0</b><td>"panSigma0"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of first round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_PAN_SIGMA1</b><td>"panSigma1"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of second round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_PAN_SIGMA2</b><td>"panSigma2"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of third round panchromatic channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_RADIUS0</b><td>"colorRadius0"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of first round color channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_RADIUS1</b><td>"colorRadius1"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of second round color channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_RADIUS2</b><td>"colorRadius2"<td>[float: DBL_MIN-DBL_MAX]<td>Pixel radius for the sigma filter of third round color channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_SIGMA0</b><td>"colorSigma0"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of first round color channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_SIGMA1</b><td>"colorSigma1"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of second round color channel noise cleaning
		<tr><td rowspan="1"><b>TS_COLOR_SIGMA2</b><td>"colorSigma2"<td>[float: DBL_MIN-DBL_MAX]<td>Scalar for the sigma filter of third round color channel noise cleaning
</table>*/
#define TS_PAN_RADIUS0		"panRadius0"	/*!< Pixel radius for the sigma filter of first round panchromatic channel noise cleaning, 0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_RADIUS1		"panRadius1"	/*!< Pixel radius for the sigma filter of second round panchromatic channel noise cleaning, 0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_RADIUS2		"panRadius2"	/*!< Pixel radius for the sigma filter of third round panchromatic channel noise cleaning, 0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_SIGMA0		"panSigma0"		/*!< Scalar for the sigma filter of first round panchromatic channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_SIGMA1		"panSigma1"		/*!< Scalar for the sigma filter of second round panchromatic channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_PAN_SIGMA2		"panSigma2"		/*!< Scalar for the sigma filter of third round panchromatic channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_RADIUS0	"colorRadius0"  /*!< Pixel radius for the sigma filter of first round color channel noise cleaning,	0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_RADIUS1	"colorRadius1"  /*!< Pixel radius for the sigma filter of second round color channel noise cleaning, 0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_RADIUS2	"colorRadius2"  /*!< Pixel radius for the sigma filter of third round color channel noise cleaning, 0 means bypass current round cleaning. \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_SIGMA0		"colorSigma0"   /*!< Scalar for the sigma filter of first round color channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_SIGMA1		"colorSigma1"   /*!< Scalar for the sigma filter of second round color channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_COLOR_SIGMA2		"colorSigma2"   /*!< Scalar for the sigma filter of third round color channel noise cleaning \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
/**@}*/

/** \addtogroup ts_7 TS Coefficients Parameters
\brief Defines for TS Coefficients parameters
*  @{
<table>		
		<caption id="multi_row">TS Coefficients Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_RR_COEFF</b><td>"RR"<td>[float: DBL_MIN-DBL_MAX]<td>Red-red coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_RG_COEFF</b><td>"RG"<td>[float: DBL_MIN-DBL_MAX]<td>Red-green coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_RB_COEFF</b><td>"RB"<td>[float: DBL_MIN-DBL_MAX]<td>Red-blue coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_GR_COEFF</b><td>"GR"<td>[float: DBL_MIN-DBL_MAX]<td>Green-red coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_GG_COEFF</b><td>"GG"<td>[float: DBL_MIN-DBL_MAX]<td>Green-green coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_GB_COEFF</b><td>"GB"<td>[float: DBL_MIN-DBL_MAX]<td>Green-blue coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_BR_COEFF</b><td>"BR"<td>[float: DBL_MIN-DBL_MAX]<td>Blue-red coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_BG_COEFF</b><td>"BG"<td>[float: DBL_MIN-DBL_MAX]<td>Blue-green coefficient of color correction matrix
		<tr><td rowspan="1"><b>TS_BB_COEFF</b><td>"BB"<td>[float: DBL_MIN-DBL_MAX]<td>blue-blue coefficient of color correction matrix
</table>*/
#define TS_RR_COEFF			"RR"			/*!< Red-red coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: 1.657 */
#define TS_RG_COEFF			"RG"			/*!< Red-green coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: -0.5325 */
#define TS_RB_COEFF			"RB"			/*!< Red-blue coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: -0.1245 */
#define TS_GR_COEFF			"GR"			/*!< Green-red coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: -0.106 */
#define TS_GG_COEFF			"GG"			/*!< Green-green coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: 1.443 */
#define TS_GB_COEFF			"GB"			/*!< Green-blue coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: -0.337 */
#define TS_BR_COEFF			"BR"			/*!< Blue-red coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: 0.131 */
#define TS_BG_COEFF			"BG"			/*!< Blue-green coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: -0.445 */
#define TS_BB_COEFF			"BB"			/*!< Blue-blue coefficient of color correction matrix\n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat, Example: 1.314 */
/**@}*/	

/** \addtogroup ts_8 TS Sharpen Parameters
\brief Defines for TS Sharpen parameters
*  @{
<table>		
		<caption id="multi_row">TS Sharpen Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_SHARPEN_PARAM</b><td>"sharpenParam"<td>[float: DBL_MIN-DBL_MAX]<td>Sharp parameter
		<tr><td rowspan="1"><b>TS_MAX_SHARPEN</b><td>"maxSharpen"<td>[float: DBL_MIN-DBL_MAX]<td>Sharp maximal threshold
</table>*/
#define TS_SHARPEN_PARAM	"sharpenParam"	/*!< Sharp parameter \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_MAX_SHARPEN		"maxSharpen"	/*!< Sharp maximal threshold \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
/**@}*/	

/** \addtogroup ts_9 TS Noise Threshold Parameters
\brief Defines for TS Noise Threshold parameters
*  @{
<table>		
		<caption id="multi_row">TS Noise Threshold Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>TS_SHARPEN_PARAM</b><td>"highLumaNoise"<td>[float: DBL_MIN-DBL_MAX]<td>High Noise threshold
		<tr><td rowspan="1"><b>TS_MAX_SHARPEN</b><td>"lowLumaNoise""<td>[float: DBL_MIN-DBL_MAX]<td>Low Noise threshold
</table>*/
#define TS_HIGH_LUMA_NOISE	"highLumaNoise"	/*!< High Noise threshold \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
#define TS_LOW_LUMA_NOISE	"lowLumaNoise"	/*!< Low Noise threshold \n\n<b>Type/Range</b>    [float: DBL_MIN-DBL_MAX]\note Used by SetParamFloat and GetParamFloat*/
/**@}*/

#ifdef __cplusplus
/*! \addtogroup Class_IpxTrueSense IpxTrueSense C++ Class
* \brief C++ Class for IpxTrueSense
*  @{
*/

/**  
* A class containing methods for IpxTrueSense modules.   
\brief A Class for IpxTrueSense modules that contains methods to convert IpxImage images
*/
class IpxTrueSense 
{
public:
	// Factory 
	
	//! This function returns the created IpxTrueSense instance
	/*!
	\return Returns the created IpxTrueSense object
	*/
	static TS_API IpxTrueSense* CreateComponent();
		
	//! This function deletes the IpxTrueSense component and all associated resources obtained by the IpxTrueSense object.
	/*!
	\param[in] in Pointer to the IpxTrueSense object
	\return Returns void
	*/
	static TS_API void DeleteComponent(IpxTrueSense* in);
		
	//! This function returns the pointer to the IpxComponent object.
	/*!
	\brief The IpxComponent object will give access to the data member functions shown below:
	\image html componentFunctions.png
	\image latex componentFunctions.png
	\return Returns the Pointer to the IpxComponent object 
	
	The following example will illustrate on how to access the IpxComponent data member function:
	
	\code

	IpxTrueSense *pDeTS = IpxTrueSense::CreateComponent();
	
	IpxError err  = pDeTS->GetComponent()->SetParamInt("TrueSenseAlgType", 3);
	
	pDeTS->ConvertImage(imageIN, imageOUT);

	IpxTrueSense::DeleteComponent(pDeTS);

	\endcode

	*/

	virtual IpxComponent* GetComponent()=0;
	   
	//! This TrueSense CFA Demosaicing function converts the input source IpxImage to the targeted output destination IpxImage
	/*!
	\param[in] pSrc Pointer to the input source IpxImage 
		
	\brief The only input source Pixel Types supported are shown in the tables below:
	<center>
	<table>
	<caption id="multi_row">TrueSense CFA Pixel Types</caption>
		<tr><th>TS Pattern Filter<th>8-bit<th>10-bit<th>12-bit<th>14-bit
		<tr><td rowspan="1"><b>BGGR w/ WBBW0</b><td>II_PIX_TS_BGGR_WBBW0_8<td>II_PIX_TS_BGGR_WBBW0_10<td>II_PIX_TS_BGGR_WBBW0_12<td>II_PIX_TS_BGGR_WBBW0_14
		<tr><td rowspan="1"><b>BGGR w/ WBBW1</b><td>II_PIX_TS_BGGR_WBBW1_8<td>II_PIX_TS_BGGR_WBBW1_10<td>II_PIX_TS_BGGR_WBBW1_12<td>II_PIX_TS_BGGR_WBBW1_14
		<tr><td rowspan="1"><b>BGGR w/ WBBW2</b><td>II_PIX_TS_BGGR_WBBW2_8<td>II_PIX_TS_BGGR_WBBW2_10<td>II_PIX_TS_BGGR_WBBW2_12<td>II_PIX_TS_BGGR_WBBW2_14
		<tr><td rowspan="1"><b>BGGR w/ WBBW3</b><td>II_PIX_TS_BGGR_WBBW3_8<td>II_PIX_TS_BGGR_WBBW3_10<td>II_PIX_TS_BGGR_WBBW3_12<td>II_PIX_TS_BGGR_WBBW3_14
		<tr><td rowspan="1"><b>GBRG w/ WGGW0</b><td>II_PIX_TS_GBRG_WGGW0_8<td>II_PIX_TS_GBRG_WGGW0_10<td>II_PIX_TS_GBRG_WGGW0_12<td>II_PIX_TS_GBRG_WGGW0_14
		<tr><td rowspan="1"><b>GBRG w/ WGGW1</b><td>II_PIX_TS_GBRG_WGGW1_8<td>II_PIX_TS_GBRG_WGGW1_10<td>II_PIX_TS_GBRG_WGGW1_12<td>II_PIX_TS_GBRG_WGGW1_14
		<tr><td rowspan="1"><b>GBRG w/ WGGW2</b><td>II_PIX_TS_GBRG_WGGW2_8<td>II_PIX_TS_GBRG_WGGW2_10<td>II_PIX_TS_GBRG_WGGW2_12<td>II_PIX_TS_GBRG_WGGW2_14
		<tr><td rowspan="1"><b>GBRG w/ WGGW3</b><td>II_PIX_TS_GBRG_WGGW3_8<td>II_PIX_TS_GBRG_WGGW3_10<td>II_PIX_TS_GBRG_WGGW3_12<td>II_PIX_TS_GBRG_WGGW3_14 
		<tr><td rowspan="1"><b>GRBG w/ WGGW0</b><td>II_PIX_TS_GRBG_WGGW0_8<td>II_PIX_TS_GRBG_WGGW0_10<td>II_PIX_TS_GRBG_WGGW0_12<td>II_PIX_TS_GRBG_WGGW0_14
		<tr><td rowspan="1"><b>GRBG w/ WGGW1</b><td>II_PIX_TS_GRBG_WGGW1_8<td>II_PIX_TS_GRBG_WGGW1_10<td>II_PIX_TS_GRBG_WGGW1_12<td>II_PIX_TS_GRBG_WGGW1_14
		<tr><td rowspan="1"><b>GRBG w/ WGGW2</b><td>II_PIX_TS_GRBG_WGGW2_8<td>II_PIX_TS_GRBG_WGGW2_10<td>II_PIX_TS_GRBG_WGGW2_12<td>II_PIX_TS_GRBG_WGGW2_14
		<tr><td rowspan="1"><b>GRBG w/ WGGW3</b><td>II_PIX_TS_GRBG_WGGW3_8<td>II_PIX_TS_GRBG_WGGW3_10<td>II_PIX_TS_GRBG_WGGW3_12<td>II_PIX_TS_GRBG_WGGW3_14
		<tr><td rowspan="1"><b>RGGB w/ WRRW0</b><td>II_PIX_TS_RGGB_WRRW0_8<td>II_PIX_TS_RGGB_WRRW0_10<td>II_PIX_TS_RGGB_WRRW0_12<td>II_PIX_TS_RGGB_WRRW0_14
		<tr><td rowspan="1"><b>RGGB w/ WRRW1</b><td>II_PIX_TS_RGGB_WRRW1_8<td>II_PIX_TS_RGGB_WRRW1_10<td>II_PIX_TS_RGGB_WRRW1_12<td>II_PIX_TS_RGGB_WRRW1_14
		<tr><td rowspan="1"><b>RGGB w/ WRRW2</b><td>II_PIX_TS_RGGB_WRRW2_8<td>II_PIX_TS_RGGB_WRRW2_10<td>II_PIX_TS_RGGB_WRRW2_12<td>II_PIX_TS_RGGB_WRRW2_14
		<tr><td rowspan="1"><b>RGGB w/ WRRW3</b><td>II_PIX_TS_RGGB_WRRW3_8<td>II_PIX_TS_RGGB_WRRW3_10<td>II_PIX_TS_RGGB_WRRW3_12<td>II_PIX_TS_RGGB_WRRW3_14 
	</table>
	</center>

	\param[out] pDst Pointer to the output destination IpxImage
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
	*/
	virtual IpxError ConvertImage(const IpxImage* pSrc, IpxImage* pDst) = 0;

	//! This function allocates memory
	/*!
	\param[in] pSrc Pointer to the input source IpxImage 
	\param[in] pDst Pointer to the output destination IpxImage
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully allocates data   
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem allocating memory  
	*/
	virtual IpxError AllocData(const IpxImage* pSrc, IpxImage* pDst) = 0;

	//! This function releases the allocated memory
	/*!
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully releases the allocated data   
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem releasing the data allocated
	*/    
	virtual void ReleaseData() = 0;
};
 /*! @} */
#endif 

/*! \addtogroup C_IpxTrueSense IpxTrueSense C-Interface Functions
 *  \brief C-interface functions for IpxTrueSense
*  @{
*/

//! This C-interface function returns the IpxHandle for the created IpxIrueSense instance
/*!
\return Returns the IpxHandle for the created IpxTrueSense object
*/

//! This C-interface function returns the IpxHandle for the created IpxTrueSense instance
/*!
\return Returns the IpxHandle for the created IpxTrueSense object
*/
TS_EXTERN_C TS_API IpxHandle TS_CALL IpxTrueSense_CreateComponent();
//! This C-interface function deletes the IpxHandle hTrueSense component and all associated resources obtained by the IpxTrueSense object
/*!
\param[in] hTrueSense Pointer to the IpxHandle for the IpxTrueSense instance
\return void
*/
TS_EXTERN_C TS_API void TS_CALL IpxTrueSense_DeleteComponent(IpxHandle hTrueSense);
//! This C-interface function returns the IpxHandle for the IpxTrueSense component
/*!
\param[in] hTrueSense Pointer to the IpxHandle for the IpxTrueSense object
\return Returns the IpxHandle for the IpxTrueSense component
*/
TS_EXTERN_C TS_API IpxHandle TS_CALL IpxTrueSense_GetComponent(IpxHandle hTrueSense);
//! This C-interface function converts the input source IpxImage to the targeted output destination 
/*!
\param[in] hTrueSense Pointer to the IpxHandle for the IpxTrueSense Component 
\param[in] pSrc Pointer to the source IpxImage
\param[out] pDst Pointer to the output destination IpxImage
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
*/
TS_EXTERN_C TS_API IpxError TS_CALL IpxTrueSense_ConvertImage(IpxHandle hTrueSense, const IpxImage* pSrc, IpxImage* pDst);
//! This C-interface function allocates the data
/*!
\param[in] hTrueSense Pointer of the IpxHandle for the IpxTrueSense Component
\param[in] pSrc Pointer to the source IpxImage
\param[in] pDst Pointer to the output destination IpxImage
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully allocates the IpxImage data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems allocating IpxImage data 
*/
TS_EXTERN_C TS_API IpxError TS_CALL IpxTrueSense_AllocData(IpxHandle hTrueSense, const IpxImage* pSrc, IpxImage* pDst);

//! This C-interface function release the IpxHandle to the IpxTrueSense data
/*!
\param[in] hTrueSense Pointer of the IpxHandle for the IpxTrueSense data
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully releases the IpxTrueSense data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems releasing the IpxTrueSense data  
*/
TS_EXTERN_C TS_API void TS_CALL IpxTrueSense_ReleaseData(IpxHandle hTrueSense);
 /*! @} */
 /*! @} */
/////////////////////////////////////////////////////////////////////////////
#endif // _TRUE_SENSE_H_
