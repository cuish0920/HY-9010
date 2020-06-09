/////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxDisplay.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxDisplay component interface description
// Function and Constant Declarations for displaying of IpxImage 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author: Michael Malykh (MMA), michael.malykh@imperx.com
// Created: 15-April-2015
// Version: 2.2.1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2015 -2017, Imperx Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////

#ifndef IPXDISPLAY_H
#define IPXDISPLAY_H

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifdef _WIN32
       #include <windows.h>
#endif

#ifndef IPXD_CALL 
	#ifdef _WIN32
		#define IPXD_CALL    __stdcall
	#else
		#define IPXD_CALL
	#endif
#endif

#ifndef IPXD_EXTERN_C
    #ifdef __cplusplus
        #define IPXD_EXTERN_C extern "C"
    #else
        #define IPXD_EXTERN_C
    #endif
#endif

// check if static library is wanted
#ifdef IPXD_STATIC
	// define the static library symbol
	#define IPXD_API
	#define IPXD_LOCAL
#else
	#define IPXD_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef IPXD_EXPORT
		// define the export symbol
		#define IPXD_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64 
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxDisplay_ConvertImage=_IpxDisplay_ConvertImage@12")
			#pragma comment(linker, "/EXPORT:IpxDisplay_CreateComponent=_IpxDisplay_CreateComponent@0")
			#pragma comment(linker, "/EXPORT:IpxDisplay_DeleteComponent=_IpxDisplay_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxDisplay_GetComponent=_IpxDisplay_GetComponent@4")
			#pragma comment(linker, "/EXPORT:IpxDisplay_DisplayImage=_IpxDisplay_DisplayImage@12")
			#pragma comment(linker, "/EXPORT:IpxDisplay_DisplayVideo=_IpxDisplay_DisplayVideo@8")
			#pragma comment(linker, "/EXPORT:IpxDisplay_Initialize=_IpxDisplay_Initialize@16")
		#endif
	#else // IPXD_EXPORT
		#ifdef IPXD_CODE
			#define IPXD_API
		#else
			// define the import symbol
			#define IPXD_API LIB_HELPER_DLL_IMPORT
		#endif
	#endif // IPXD_EXPORT
#endif

//////////////////////////////////////////////////////
/// \defgroup ipxdisplay IpxDisplay IpxComponent Header
/// \ingroup ipxdisplay1
///  \brief Display functions and classes with IpxComponent features
///@{
///////

/// \defgroup ipxd_sub Display Component Parameters
/// \ingroup ipxdisplaysub 
///  \brief Defines and Macros for Display Component Parameters
///@{
///////

// PRE-INIT PARAMETERS
/** \addtogroup pre-init Pre-initialization Parameters
 \brief Defines for Pre-Initialization Parameters
 *  @{

<table>
		<caption id="multi_row">PRE-INIT PARAMETERS</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>IDP_BACKGROUND</b><td>"display.bg.color"<td>[int: 0, 1]<td>Background color
		<tr><td rowspan="1"><b>IDP_SMOOTHING</b><td>"processing.smoothing"<td>[int: 0,1]<td>Smoothing
		<tr><td rowspan="1"><b>IDP_OGL_BAYER</b><td>"ogl.processing.bayer.method"<td>[int: 0,1]<td>De\-bayer method for OpenGL mode
		<tr><td rowspan="1"><b>IDP_OGL_TRUESENSE</b><td>"ogl.processing.truesense.method"<td>[int: 0,1]<td>Truesense demosaicing method for OpenGL mode
		<tr><td rowspan="1"><b>IDP_GDI_BAYER<td></b>"gdi.processing.bayer.method"<td>[int: 0-2]<td>De-bayer method for GDI mode
		<tr><td rowspan="1"><b>IDP_GDI_TRUESENSE</b><td>"gdi.processing.truesense.method"<td>[int: 0,1]<td>Truesense demosaicing method for GDI mode
		<tr><td rowspan="1"><b>IDP_COMMAND_WINDOW</b><td>"window.command"<td>[int]<td>Command window handle
		<tr><td rowspan="1"><b>IDP_OVERLAY_FONT_DESC_0</b><td>"overlay.font.desc.0"<td>[char*]<td>Overlay font descriptor for font #0
		<tr><td rowspan="1"><b>IDP_OVERLAY_FONT_DESC_1</b><td>"overlay.font.desc.1"<td>[char*]<td>Overlay font descriptor for font #1
		<tr><td rowspan="1"><b>IDP_OVERLAY_FONT_DESC_2</b><td>"overlay.font.desc.2"<td>[char*]<td>Overlay font descriptor for font #2
		<tr><td rowspan="1"><b>IDP_OVERLAY_FONT_DESC_3</b><td>"overlay.font.desc.3"<td>[char*]<td>Overlay font descriptor for font #3
</table>*/

#define IDP_BACKGROUND          "display.bg.color"                      /*!< Background color \n\n<b>Type/Range</b>   [int: 0, 1] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_INIT_FIT            "display.init.fit"                      /*!< View: default fit mode for new image format \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_INIT_AT_X           "display.init.at.x"                     /*!< View: default fit mode for new image format \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_INIT_AT_Y           "display.init.at.y"                     /*!< View: default fit mode for new image format \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_SMOOTHING           "processing.smoothing"                  /*!< Smoothing \n\n<b>Type/Range</b>   [int: 0,1] \note Used by SetParamInt and GetParamInt [QT: no] */
#define IDP_OGL_BAYER           "ogl.processing.bayer.method"           /*!< De-bayer method for OpenGL mode \n\n<b>Type/Range</b>   [int: 0,1] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OGL_TRUESENSE       "ogl.processing.truesense.method"       /*!< Truesense demosaicing method for OpenGL mode\n\n<b>Type/Range</b>    [int: 0,1] \note Used by SetParamInt and GetParamInt [QT: no] */
#define IDP_GDI_BAYER           "gdi.processing.bayer.method"           /*!< De-bayer method for GDI mode \n\n<b>Type/Range</b>   [int: 0-2] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_GDI_TRUESENSE       "gdi.processing.truesense.method"       /*!< Truesense demosaicing method for GDI mode\n\n<b>Type/Range</b>    [int: 0,1] \note Used by SetParamInt and GetParamInt [QT: no] */
#define IDP_COMMAND_WINDOW      "window.command"                        /*!< Command window handle \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_OVERLAY_FONT_DESC_0 "overlay.font.desc.0"                   /*!< Overlay font descriptor for font #0 \n\n<b>Type/Range</b>   [char*] \note Used by SetParamString and GetParamString [QT: no]*/
#define IDP_OVERLAY_FONT_DESC_1 "overlay.font.desc.1"                   /*!< Overlay font descriptor for font #1 \n\n<b>Type/Range</b>   [char*] \note Used by SetParamString and GetParamString [QT: no]*/
#define IDP_OVERLAY_FONT_DESC_2 "overlay.font.desc.2"                   /*!< Overlay font descriptor for font #2 \n\n<b>Type/Range</b>   [char*] \note Used by SetParamString and GetParamString [QT: no]*/
#define IDP_OVERLAY_FONT_DESC_3 "overlay.font.desc.3"                   /*!< Overlay font descriptor for font #3 \n\n<b>Type/Range</b>   [char*] \note Used by SetParamString and GetParamString [QT: no]*/
/**@}*/

// RUN-TIME PARAMETERS
// View managenent
/** \addtogroup run-time Run-time Parameters
 \brief   Defines for Run-time Parameters (View Management)
@{
<table>
		<caption id="multi_row">RUN-TIME PARAMETERS</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>IDP_VIEW_FIT</b><td>"display.fit"<td>[int: 0-3]<td>View: current fit mode
		<tr><td rowspan="1"><b>IDP_VIEW_X</b><td>"display.x"<td>[real: 0-1]<td>X position
		<tr><td rowspan="1"><b>IDP_VIEW_Y</b><td>"display.y"<td>[real: 0-1]<td>Y position
		<tr><td rowspan="1"><b>IDP_MANAGED_FPS</b><td>"playback.managed.fps"<td>[int]<td>"Managed" state FPS (default 20)
		<tr><td rowspan="1"><b>IDP_MANAGED_STATE</b><td>"playback.managed"<td>[int]<td>"Managed" state flag (default 0)
</table>
*/

#define IDP_SIGNATURE           "system.signature"                      /*!< Component identifier [QT: no] */
#define IDP_VIEW_FIT            "display.fit"                           /*!< View: current fit mode \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_VIEW_X              "display.x"                             /*!< X position \n\n<b>Type/Range</b>   [real: 0-1] [QT: no]*/
#define IDP_VIEW_Y              "display.y"                             /*!< Y position \n\n<b>Type/Range</b>   [real: 0-1] [QT: no]*/
#define IDP_VIEW_SCALE          "display.view.scale"                    /*!< View: current scale [QT: no] */
#define IDP_MANAGED_FPS         "playback.managed.fps"                  /*!< "Managed" state FPS (default 20) \n\n<b>Type/Range</b>   [int] [QT: no]*/
#define IDP_MANAGED_STATE       "playback.managed"                      /*!< "Managed" state flag (default 0) \n\n<b>Type/Range</b>   [int] [QT: no]*/
#define IDP_VIEW_CLR            "display.view.color"                    /*!< Current color \n\n<b>Type/Range</b>   [char*] [QT: no]*/
#define IDP_VIEW_CURSOR_X       "display.view.cursor.x"                 /*!< Current cursor X position in image co-ordinates \n\n<b>Type/Range</b>   [int: 0-width] [QT: no]*/
#define IDP_VIEW_CURSOR_Y       "display.view.cursor.y"                 /*!< Current cursor Y position in image co-ordinates \n\n<b>Type/Range</b>   [int: 0-width] [QT: no]*/
#define IDP_PROC_PROCESSOR      "processor"                             /*!< Image processor pointer [QT: no] */
#define IDP_PROC_PROCESSOR_TYPE "processor.type"                        /*!< Image processor type [QT: no] */
#define IDP_MENU_X              "menu.x"                                /*!< X position for context menu \n\n<b>Type/Range</b>   [int] [QT: no]*/
#define IDP_MENU_Y              "menu.y"                                /*!< Y position for context menu \n\n<b>Type/Range</b>   [int] [QT: no]*/
#define IDP_MENU_CMD            "menu.cmd"                              /*!< Context menu command \n\n<b>Type/Range</b>   [int] [QT: no]*/
/**@}*/


// Software Image Correction Parameters
/** \addtogroup sw Software Image Correction Parameters
 \brief   Defines for Software Image Correction Parameters 
@{
<table>
	<caption id="multi_row">SW IMAGE CORRECTION PARAMETERS</caption>
	<tr><th>Macro<th>Parameter Name<th>Type<th>Description
	<tr><td rowspan="1"><b>IDP_CORR_MODE</b><td>"correction.mode"<td>[int: 0-2]<td>Software Correction: current mode 
	<tr><td rowspan="1"><b>IDP_CORR_GAIN_R</b><td>"correction.gain.r"<td>[real: 0+]<td>Software Correction: Gain for red channel
	<tr><td rowspan="1"><b>IDP_CORR_GAIN_G</b><td>"correction.gain.g"<td>[real: 0+]<td>Software Correction: Gain for green channel
	<tr><td rowspan="1"><b>IDP_CORR_GAIN_B</b><td>"correction.gain.b"<td>[real: 0+]<td>Software Correction: Gain for blue channel
	<tr><td rowspan="1"><b>IDP_CORR_OFFS_R</b><td>"correction.offs.r"<td>[int]<td>Software Correction: Offset for red channel
	<tr><td rowspan="1"><b>IDP_CORR_OFFS_G</b><td>"correction.offs.g"<td>[int]<td>Software Correction: Offset for green channel
	<tr><td rowspan="1"><b>IDP_CORR_OFFS_B</b><td>"correction.offs.b"<td>[int]<td>Software Correction: Offset for blue channel
	<tr><td rowspan="1"><b>IDP_CORR_GAMMA</b><td>"correction.gamma"<td>[real: 0+]<td>Software Correction: Gamma
</table>*/
// Software image correction
#define IDP_CORR_MODE           "correction.mode"                       /*!< Software Correction: current mode \n\n<b>Type/Range</b>   [int: 0-2] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_CORR_GAIN_R         "correction.gain.r"                     /*!< Software Correction: Gain for red channel \n\n<b>Type/Range</b>   [real: 0+] \note Used by SetParamFloat and GetParamFloat [QT: no]*/
#define IDP_CORR_GAIN_G         "correction.gain.g"                     /*!< Software Correction: Gain for green channel \n\n<b>Type/Range</b>   [real: 0+]  \note Used by SetParamFloat and GetParamFloat [QT: no]*/
#define IDP_CORR_GAIN_B         "correction.gain.b"                     /*!< Software Correction: Gain for blue channel \n\n<b>Type/Range</b>   [real: 0+] \note Used by SetParamFloat and GetParamFloat [QT: no]*/
#define IDP_CORR_OFFS_R         "correction.offs.r"                     /*!< Software Correction: Offset for red channel \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_CORR_OFFS_G         "correction.offs.g"                     /*!< Software Correction: Offset for green channel \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_CORR_OFFS_B         "correction.offs.b"                     /*!< Software Correction: Offset for blue channel \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_CORR_GAMMA          "correction.gamma"                      /*!< Software Correction: Gamma \n\n<b>Type/Range</b>   [real: 0+] \note Used by SetParamFloat and GetParamFloat [QT: no]*/
/**@}*/

// Calculations
// White Balance Correction Parameters
/** \addtogroup wb White Balance Correction Parameters
 \brief   Defines for White Balance Correction Parameters 
@{
<table>
	<caption id="multi_row">WHITE BALANCE CORRECTION PARAMETERS</caption>
	<tr><th>Macro<th>Parameter Name<th>Type<th>Description
	<tr><td rowspan="1"><b>IDP_CALC_COEF_R</b><td>"correction.calc.r"<td>[real: 0+]<td>White balance: coef for red channel
	<tr><td rowspan="1"><b>IDP_CALC_COEF_G</b><td>"correction.calc.g"<td>[real: 0+]<td>White balance: coef for green channel
	<tr><td rowspan="1"><b>IDP_CALC_COEF_B</b><td>"correction.calc.b"<td>[real: 0+]<td>White balance: coef for blue channel
</table>*/
#define IDP_CALC_COEF_R         "correction.calc.r"                     /*!< White balance: coef for red channel \n\n<b>Type/Range</b>   [real: 0+] \note Used by SetParamFloat and GetParamFloat [QT: no]*/
#define IDP_CALC_COEF_G         "correction.calc.g"                     /*!< White balance: coef for green channel \n\n<b>Type/Range</b>   [real: 0+] \note Used by SetParamFloat and GetParamFloat [QT: no]*/
#define IDP_CALC_COEF_B         "correction.calc.b"                     /*!< White balance: coef for blue channel \n\n<b>Type/Range</b>   [real: 0+]  \note Used by SetParamFloat and GetParamFloat [QT: no]*/
/**@}*/

// Overlay text Parameters
/** \addtogroup ot Overlay Text Parameters
 \brief   Defines for Overlay Text Parameters 
@{
<table>
	<caption id="multi_row">OVERLAY TEXT PARAMETERS</caption>
	<tr><th>Macro<th>Parameter Name<th>Type<th>Description
	<tr><td rowspan="1"><b>IDP_OVERLAY_INDEX</b><td>"overlay.index"<td>[int: 0-3]<td>Overlay: current index
	<tr><td rowspan="1"><b>IDP_OVERLAY_POS</b><td>"overlay.pos"<td>[int: 0-8]<td>Overlay: position
	<tr><td rowspan="1"><b>IDP_OVERLAY_FONT</b><td>"overlay.font"<td>[int: 0-3]<td>Overlay: font index
	<tr><td rowspan="1"><b>IDP_OVERLAY_COLOR</b><td>"overlay.clr"<td>[int]<td>Overlay: text color
	<tr><td rowspan="1"><b>IDP_OVERLAY_BGMODE</b><td>"overlay.bgmode"<td>[int: 0-3]<td>Overlay: backgound mode
	<tr><td rowspan="1"><b>IDP_OVERLAY_TEXT</b><td>"overlay.text"<td>[char*]<td>Overlay: text
</table>*/
// Overlay text
#define IDP_OVERLAY_INDEX       "overlay.index"                         /*!< Overlay: current index \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OVERLAY_POS         "overlay.pos"                           /*!< Overlay: position \n\n<b>Type/Range</b>   [int: 0-8] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OVERLAY_FONT        "overlay.font"                          /*!< Overlay: font index \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OVERLAY_COLOR       "overlay.clr"                           /*!< Overlay: text color \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OVERLAY_BGMODE      "overlay.bgmode"                        /*!< Overlay: backgound mode \n\n<b>Type/Range</b>   [int: 0-3] \note Used by SetParamInt and GetParamInt [QT: no]*/
#define IDP_OVERLAY_TEXT        "overlay.text"                          /*!< Overlay: text \n\n<b>Type/Range</b>   [char*] \note Used by SetParamString and GetParamString [QT: no]*/
/**@}*/

// Dump Rect Parameters
/** \addtogroup dr Dump Rect Parameters
 \brief   Defines for Dump Rect Parameters 
@{
<table>
	<caption id="multi_row">DUMP RECT PARAMETERS</caption>
	<tr><th>Macro<th>Parameter Name<th>Type<th>Description
	<tr><td rowspan="1"><b>IDP_DUMP_X</b><td>"dump.x"<td>[int: 1-w]<td>Dump rect: x-pos
	<tr><td rowspan="1"><b>IDP_DUMP_Y</b><td>"dump.y"<td>[int: 1-h]<td>Dump rect: y-pos
	<tr><td rowspan="1"><b>IDP_DUMP_W</b><td>"dump.w"<td>[int: 1-(w-x)]<td>Dump rect: width
	<tr><td rowspan="1"><b>IDP_DUMP_H</b><td>"dump.h"<td>[int: 1-(h-y)]<td>Dump rect: height
	<tr><td rowspan="1"><b>IDP_DUMP_COLOR</b><td>"dump.clr"<td>[int]<td>Dump rect: color (optional)
</table>*/
// Dump rect
#define IDP_DUMP_X              "dump.x"                                /*!< Dump rect: x-pos \n\n<b>Type/Range</b>   [int: 1-w] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_DUMP_Y              "dump.y"                                /*!< Dump rect: y-pos \n\n<b>Type/Range</b>   [int: 1-h] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_DUMP_W              "dump.w"                                /*!< Dump rect: width \n\n<b>Type/Range</b>   [int: 1-(w-x)] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_DUMP_H              "dump.h"                                /*!< Dump rect: height \n\n<b>Type/Range</b>   [int: 1-(h-y)] \note Used by SetParamInt and GetParamInt [QT: yes]*/
#define IDP_DUMP_COLOR          "dump.clr"                              /*!< Dump rect: color (optional) \n\n<b>Type/Range</b>   [int] \note Used by SetParamInt and GetParamInt [QT: no]*/
/**@}*/

// IpxDisplay Commands
// IpxDisplay Command Parameters
/** \addtogroup cp IpxDisplay Command Parameters
 \brief   Defines for IpxDisplay Command Parameters 
@{
<table>
	<caption id="multi_row">IPXDISPLAY COMMAND PARAMETERS</caption>
	<tr><th>Macro<th>Parameter Name<th>Description
	<tr><td rowspan="1"><b>IDPC_SET_CORRECTION</b><td>"processing.correction"<td>Parameters IDP_CORR_(XX) should be set before command call
	<tr><td rowspan="1"><b>IDPC_CMD_VIEW_ZOOM_IN</b><td>"display.zoom.in"<td>Zoom in (no params)
	<tr><td rowspan="1"><b>IDPC_CMD_VIEW_ZOOM_OUT</b><td>"display.zoom.out"<td>Zoom out (no params)
	<tr><td rowspan="1"><b>IDPC_CMD_VIEW_ATCENTER</b><td>"display.atcenter"<td>View at the center of image (no params)
	<tr><td rowspan="1"><b>IDPC_CMD_VIEW_AT</b><td>"display.center.at"<td>View at the specific position (IDP_VIEW_X, IDP_VIEW_Y should be set)
	<tr><td rowspan="1"><b>IDPC_CMD_CORR_CALC</b><td>"correction.calc"<td>Results placed in IDP_CALC_GAIN_(XX) parameters
	<tr><td rowspan="1"><b>IDPC_CMD_OVERLAY_SHOW</b><td>"overlay.show"<td>Show current overlay text with current parameters (current index specified by IDP_OVER_INDEX)
	<tr><td rowspan="1"><b>IDPC_CMD_OVERLAY_HIDE</b><td>"overlay.hide"<td>Hide current overlay text (current index specified by IDP_OVER_INDEX)
	<tr><td rowspan="1"><b>IDPC_CMD_MANAGED_ON</b><td>"playback.managed.on"<td>Set "managed" state (try to keep specified FPS value, see IDP_MANAGED_FPS)
	<tr><td rowspan="1"><b>IDPC_CMD_MANAGED_OFF</b><td>"playback.managed.off"<td>Clear "managed" state
	<tr><td rowspan="1"><b>IDPC_CMD_DUMP_ON</b><td>"display.dump.on"<td>Show dump rect (IDP_DUMP_X, IDP_DUMP_Y, IDP_DUMP_W, IDP_DUMP_H)
	<tr><td rowspan="1"><b>IDPC_CMD_DUMP_OFF</b><td>"display.dump.off"<td>Hide dump rect
	<tr><td rowspan="1"><b>IDPC_CMD_FILTER_ADD</b><td>"filter.add"<td>Add processing filter
	<tr><td rowspan="1"><b>IDPC_CMD_FILTER_DEL</b><td>"filter.del"<td>Remove processing filter
</table>*/
#define IDPC_SET_CORRECTION     "processing.correction"                 /*!< Parameters IDP_CORR_(XX) should be set before command call \note Used by RunCommand [QT: no]*/
#define IDPC_CMD_VIEW_ZOOM_IN   "display.zoom.in"                       /*!< Zoom in (no params)\note Used by RunCommand [QT: yes]*/
#define IDPC_CMD_VIEW_ZOOM_OUT  "display.zoom.out"                      /*!< Zoom out (no params)\note Used by RunCommand [QT: yes]*/
#define IDPC_CMD_VIEW_ATCENTER  "display.atcenter"                      /*!< View at the center of image (no params)\note Used by RunCommand [QT: yes]*/
#define IDPC_CMD_VIEW_AT        "display.center.at"                     /*!< View at the specific position (IDP_VIEW_X, IDP_VIEW_Y should be set)\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_VIEW_PARAMS    "display.params.set"                    /*!< Set View parameters \note Used by RunCommand [QT: no]*/
#define IDPC_CMD_CORR_CALC      "correction.calc"                       /*!< Results placed in IDP_CALC_GAIN_(XX) parameters\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_OVERLAY_SHOW   "overlay.show"                          /*!< Show current overlay text with current parameters (current index specified by IDP_OVER_INDEX)\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_OVERLAY_HIDE   "overlay.hide"                          /*!< Hide current overlay text (current index specified by IDP_OVER_INDEX)\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_MANAGED_ON     "playback.managed.on"                   /*!< Set "managed" state (try to keep specified FPS value, see IDP_MANAGED_FPS)\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_MANAGED_OFF    "playback.managed.off"                  /*!< Clear "managed" state\note Used by RunCommand [QT: no]*/
#define IDPC_CMD_DUMP_ON        "display.dump.on"                       /*!< Show dump rect (IDP_DUMP_X, IDP_DUMP_Y, IDP_DUMP_W, IDP_DUMP_H)\note Used by RunCommand [QT: yes]*/
#define IDPC_CMD_DUMP_OFF       "display.dump.off"                      /*!< Hide dump rect\note Used by RunCommand [QT: yes]*/
#define IDPC_CMD_FILTER_ADD     "filter.add"                            /*!< Add processing filter [QT: no]*/
#define IDPC_CMD_FILTER_DEL     "filter.del"                            /*!< Remove processing filter [QT: no]*/
#define IDPC_CMD_PROC_ADD       "processing.proc.add"                   /*!< Add processor [QT: no]*/
#define IDPC_CMD_PROC_DEL       "processing.proc.del"                   /*!< Remove processor [QT: no]*/
#define IDPC_CMD_MENU_SHOW      "display.menu.show"                     /*!< Show context menu at IDP_MENU_X, IDP_MENU_Y, result placed to IDP_MENU_CMD [QT: no]*/


/**@}*/
/**@}*/
/** \addtogroup nt Notifications
 \brief   Defines for Notifications 
@{*/
// NOTIFICATIONS
#ifndef IPXD_NOTIFICATION
#define IPXD_NOTIFICATION       (WM_USER + 0x0100L)             // NOTIFICATION CODES
#define IPXD_LBUTTON_DOWN       0x4002                          /*!< Left mouse button down, param = MAKELONG(cursor.x, cursor.y), processing skipped if return = 1 [QT: yes]*/
#define IPXD_LBUTTON_UP         0x4003                          /*!< Left mouse button up, param = MAKELONG(cursor.x, cursor.y), processing skipped if return = 1 [QT: yes]*/
#define IPXD_RBUTTON_DOWN       0x4004                          /*!< Show context menu, param = MAKELONG(cursor.x, cursor.y) [QT: yes]*/
#define IPXD_CURSOR_MOVED       0x4008                          /*!< Cursor moved, param = MAKELONG(cursor.x, cursor.y) [QT: yes]*/ 
#define IPXD_KEY_DOWN           0x4009                          /*!< Keydown notification, param = MAKELONG(key code, repeat count) [QT: yes]*/ 
#define IPXD_VIEW_CHANGED       0x4010                          /*!< View parameters changed [QT: yes]*/ 
#define IPXD_CCLR_CHANGED       0x4012                          /*!< Current color changed [QT: yes]*/
#define IPXD_PLAYBACK_FAILED    0x4014                          /*!< Playback failed (image format not supported), param 'reason' (IPXD_ERROR-x) [QT: mpno]*/
#define IPXD_ERROR_OPENGL       0x4300                          /*!< Error rendering image with OpenGL, param = reason [QT: no]*/
#endif // IPXD_NOTIFICATION
/**@}*/

/** \addtogroup fl Translate Flags
 \brief   Defines for Translate Flags 
@{*/
// Coordinates
#define IDFL_SCR_IMG            0x0001                          /*!< Translate flags: Screen to image [QT: yes]*/
#define IDFL_IMG_SCR            0x0100                          /*!< Translate flags: Image to screen [QT: yes]*/
/**@}*/

/** \addtogroup fit Fit Modes and Mouse Processing
 \brief   Defines for Fit Modes and Mouse Processing 
@{*/
#ifndef IPXD_FIT_NONE

// FIT MODES
#define IPXD_FIT_NONE           0L                              /*!< Off [QT: yes]*/
#define IPXD_FIT_WINDOW         1L                              /*!< Fit to window [QT: yes]*/
#define IPXD_FIT_FILL           2L                              /*!< Fill window with image [QT: yes]*/
#define IPXD_FIT_FULLSIZE       3L                              /*!< Original size (100%) [QT: yes]*/

// MOUSE PROCESSING
#define IPXD_MOUSE_DEFAULT      0                               /*!< Do default processing [QT: no]*/
#define IPXD_MOUSE_SKIP         1                               /*!< Skip mouse action [QT: no]*/
#define IPXD_MOUSE_LOCK         2                               /*!< Lock mouse (capture) [QT: no]*/

#endif // IPXD_FIT_NONE
/**@}*/


#ifdef __cplusplus
/*! \addtogroup Class_IpxDisplay IpxDisplay C++ Class
 *  \brief C++ Class for IpxDisplay
*  @{
*/

/**  
* A class containing methods for IpxDisplay modules.   
\brief A Class for IpxDisplay modules that contains methods to display IpxImage images.  This class is responsible for displaying video frames and still images.
*/
class IpxDisplay {
public:

    virtual ~IpxDisplay() {};

public:
		
	//! This function creates a IpxComponent and returns the created IpxDisplay instance [QT: yes]
	/*!
	\return Returns the created IpxDisplay object
	*/
    static IPXD_API IpxDisplay* CreateComponent();
		
	//! This function deletes the IpxDisplay component and all associated resources obtained by the IpxDisplay object. [QT: yes]
	/*!
	\param[in] component Pointer to the IpxDisplay object
	\return Returns void
	*/
    static IPXD_API void        DeleteComponent(IpxDisplay* component);

	//! This function returns the pointer to the IpxComponent object.  [QT: yes]
	/*!
	\brief The IpxComponent object will give access to the data member functions shown below:
	\image html componentFunctions.png
	\image latex componentFunctions.png
	\return Returns the Pointer to the IpxComponent object 
	
	The following example will illustrate on how to access the IpxComponent data member function:

	\code
	
	IpxDisplay* m_IpxDisplay = IpxDisplay::CreateComponent();
	
	...
	
	//Sets the IDP_OGL_BAYER
	m_ipxDisplay->GetComponent()->SetParamString(IDP_OGL_BAYER, "0");

	...

	\endcode
	\note Please reference the \ref ipxd_sub section to view the supported parameter names for IpxDisplay
	*/
    virtual IpxComponent*       GetComponent() = 0;
		
	//! This function returns GPU information as text
    /*!
	\param[in] buffer allocated buffer for information
	\param[in] bufferSz size of the buffer
	\param[in] separator optional parameters separator
	\return 
	 - \c If successful, returns true
	 - \c Otherwise, returns false
	*/
    virtual bool                GetSystemInfo(char* buffer, int32_t bufferSz, char* separator = "; ") = 0;

	//! This function initializes the display library for playing videos/still images with the specified mode and image parameters.  
    // [QT: yes NOTE: displayWindow should be a pointer to valid QWidget object]
    /*!
	\param[in] displayWindow pointer to window. If the displayWindow is not specified, it will create a window.
	\param[in] mode Display mode ("GDI", "OpenGL" mode or "auto" (default) for auto-detection)
	\param[in] imageParams pointer to Image Parameters
	\return Returns an error code:
	 - \c If successful, the IpxError code is IPX_ERR_OK and the display library has been initialized.
	 - \c Otherwise, the initialization of the display library failed.
	*/
    virtual IpxError            Initialize(void* displayWindow, char* mode = "auto", IpxImage* imageParams = 0) = 0;

    //! This function initializes video player for specified image parameters
    //! It should be called each time image parameters has been changed
    /*!
	    \param[in] imageParams image with specified width, height and pixel type
        \param[in] mode either "GDI", "OpenGL" mode or "auto" (default) for auto-detection
	\return Returns an error code:
	 - \c If successful, the IpxError code is IPX_ERR_OK and the display is ready to display new pixel format.
	 - \c Otherwise, the call has been failed and successive DisplayVideo() calls will not display any video.
    */
    virtual IpxError            SetVideoMode(IpxImage* imageParams, char* mode = "auto") = 0;

	//! This function displays the video frame. [QT: yes]
	/*!
	@param[in] image source image
	
	@return Returns an error code:
	- \c    If successful, the IpxError code is IPX_ERR_OK and the function displays the video frame.
	- \c    Otherwise, the video frame is not displayed.
	
	*/
    virtual IpxError            DisplayVideo(IpxImage* image) = 0;
		
	//! This function displays the still image. [QT: no]
	/*!
	@param[in] image source image
	@param[in] mode Display mode ("GDI", "OpenGL" mode or "auto" (default) for auto-detection)
	@return Returns an error code:
	- \c  If successful, the IpxError code is IPX_ERR_OK and the function displays the still image.
	- \c  Otherwise, the still image is not displayed.
	
	*/
    
	virtual IpxError            DisplayImage(IpxImage* image, char* mode = "auto") = 0;
		
	//! This function converts the source image to the specified output image. [QT: no]
	/*!
	@param[in] source source image

	\brief The only input source Pixel Types supported are shown in the tables below:
	<center>
	<table>
		<caption id="multi_row">BAYER CFA Pixel Types</caption>
		<tr><th>Bayer Pattern Filter<th>8-bit<th>10-bit<th>12-bit<th>14-bit<th>16-bit
		<tr><td rowspan="1"><b>GR Pattern Filter</b><td>II_PIX_BAYGR8<td>II_PIX_BAYGR10<td>II_PIX_BAYGR12<td>II_PIX_BAYGR14<td>II_PIX_BAYGR16
		<tr><td rowspan="1"><b>RG Pattern Filter</b><td>II_PIX_BAYRG8<td>II_PIX_BAYRG10<td>II_PIX_BAYRG12<td>II_PIX_BAYRG14<td>II_PIX_BAYRG16
		<tr><td rowspan="1"><b>BG Pattern Filter</b><td>II_PIX_BAYBG8<td>II_PIX_BAYBG10<td>II_PIX_BAYBG12<td>II_PIX_BAYBG14<td>II_PIX_BAYBG16
		<tr><td rowspan="1"><b>GB Pattern Filter</b><td>II_PIX_BAYGB8<td>II_PIX_BAYGB10<td>II_PIX_BAYGB12<td>II_PIX_BAYGB14<td>II_PIX_BAYGB16
	</table>
	<table>
		<caption id="multi_row">PACKED BAYER CFA Pixel Types</caption>
		<tr><th>Bayer Pattern Filter<th>10-bit<th>12-bit
		<tr><td rowspan="1"><b>GR Pattern Filter</b><td>II_PIX_BAYGR10_PACKED<td>II_PIX_BAYGR12_PACKED
		<tr><td rowspan="1"><b>RG Pattern Filter</b><td>II_PIX_BAYRG10_PACKED<td>II_PIX_BAYRG12_PACKED 
		<tr><td rowspan="1"><b>GB Pattern Filter</b><td>II_PIX_BAYGB10_PACKED<td>II_PIX_BAYGB12_PACKED 
		<tr><td rowspan="1"><b>GB Pattern Filter</b><td>II_PIX_BAYBG10_PACKED<td>II_PIX_BAYBG12_PACKED 
	</table>
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
	@param[in] output destination image
	@return Returns an error code:
	- \c     If successful, the IpxError code is IPX_ERR_OK and the function converts the image.
	- \c	 Otherwise, the source image is not converted.
	
	*/
    virtual IpxError            ConvertImage(IpxImage* source, IpxImage* output) = 0;
		
	/**
    Returns handle to the default context menu [QT: no]
        
    @param[in] x                mouse x position in windows coords
    @param[in] y                mouse y position in windows coords
     
    @note Menu should be deleted by the Caller

    @return handle of the default menu 
    */
	#ifdef _WIN32
    virtual HMENU               GetContextMenu(int32_t x, int32_t y) { return 0; }
	#endif
		
	 /**
     Shows the context menu and processes it if requested [QT: no]
        
     @param[in] x            mouse x-coordinate position in windows coords
     @param[in] y            mouse y-coordinate position in windows coords
     @param[in] hmenu        menu to show, if NULL, the default menu will be shown
     @param[in] process      if true, the menu will be processed by the library, otherwise ID will be passed back w/o processing
	 @return selected ID
    
	 An example of ShowContextMenu is shown below:

	 \code
	 
	  case IPXD_RBUTTON_DOWN: {
	           HMENU hMenu = m_pIpxDisplay->GetContextMenu(LOWORD(msg->lParam), HIWORD(msg->lParam));
	           *result = m_pIpxDisplay->ShowContextMenu(LOWORD(msg->lParam), HIWORD(msg->lParam), hMenu);
	            return false;
	 }
	 
	 \endcode

	 The image below illustrates the ContextMenu being displayed after right clicking the mouse.

	 \image html contextmenu.png
	 \image latex contextmenu.png
	 \n


	*/
        #ifdef _WIN32
            virtual int32_t             ShowContextMenu(int32_t x, int32_t y, HMENU hmenu = 0, int32_t process = 1) { return 0; }
	#endif
    //! This function translates the display object to the specified coordinates as indicated by the flag. [QT: yes]
	/*!
	\param[in] x x-coordinate position
	\param[in] y y-coordinate position
	\param[in] flags Flag indicating mode to translate coordinates
	- \c <b>IDFL_SCR_IMG</b>  Translate coordinates from screen to image coordinates
	- \c <b>IDFL_IMG_SCR</b>  Translate coordinates from image to screen coordinates
	\return Returns an error code:
	- \c    If successful, the IpxError code is IPX_ERR_OK and the function translates the object.
	- \c	Otherwise, the object failed to translate
	*/
	virtual IpxError            Translate(int32_t* x, int32_t* y, int32_t flags) { return 0; }

};
 /*! @} */
#endif //__cplusplus



/*! \addtogroup C_IpxDisplay IpxDisplay C-Interface Functions
 *  \brief C-interface functions for IpxDisplay
*  @{
*/

//! This C-interface function returns the IpxHandle for the created IpxDisplay instance
/*!
\return Returns the IpxHandle for the created IpxDisplay object
*/
// C-interface
IPXD_EXTERN_C IPXD_API IpxHandle IPXD_CALL IpxDisplay_CreateComponent();
//! This C-interface function deletes the IpxHandle hDisplay component and all associated resources obtained by the IpxDisplay object
/*!
\param[in] hDisplay Pointer to the IpxHandle for the IpxDisplay instance
\return Void
*/
IPXD_EXTERN_C IPXD_API void IPXD_CALL IpxDisplay_DeleteComponent(IpxHandle hDisplay);
//! This C-interface function returns the IpxHandle for the created IpxImageConverter instance
/*!
\param[in] hDisplay Pointer to the IpxHandle for the IpxDisplay instance
\return Returns the IpxHandle for the IpxDisplay object component
*/
IPXD_EXTERN_C IPXD_API IpxComponent* IPXD_CALL IpxDisplay_GetComponent(IpxHandle hDisplay);
//! This C-interface function initializes the display library for playing videos/still images with the specified mode and image parameters. 
/*!
\param[in] hDisplay pointer to the IpxHandle for the IpxDisplay instance
\param[in] displayWindow pointer to window. If the displayWindow is not specified, it will create a window.
\param[in] mode Display mode ("GDI", "OpenGL" mode or "auto" (default) for auto-detection)
\param[in] imageParams pointer to Image Parameters
	
@return Returns an error code:
- \c If successful, the IpxError code is IPX_ERR_OK and the display library has been initialized.
- \c Otherwise, the initialization of the display library failed.\param[in] hDisplay Pointer to the IpxHandle for the IpxDisplay 
*/
IPXD_EXTERN_C IPXD_API IpxError IPXD_CALL IpxDisplay_Initialize(IpxHandle hDisplay, void* displayWindow, char* mode, IpxImage* imageParams);
//! This C-interface function displays the video frame 
/*!
@param[in] hDisplay pointer to the IpxHandle for the IpxDisplay instance
@param[in] pImage Pointer to the IpxImage
	
@return Returns an error code:
- \c If successful, the IpxError code is IPX_ERR_OK and the function displays the video frame.
- \c Otherwise, the video frame is not displayed	
*/
IPXD_EXTERN_C IPXD_API IpxError IPXD_CALL IpxDisplay_DisplayVideo(IpxHandle hDisplay, const IpxImage* pImage);
//! This C-interface function displays the still image 
/*!
@param[in] hDisplay pointer to the IpxHandle for the IpxDisplay instance
@param[in] pImage Pointer to the IpxImage image
@param[in] mode Display mode ("GDI", "OpenGL" mode or "auto" (default) for auto-detection)
	
@return Returns an error code:
- \c If successful, the IpxError code is IPX_ERR_OK and the function displays the still image.
- \c Otherwise, the video frame is not displayed.
*/
IPXD_EXTERN_C IPXD_API IpxError IPXD_CALL IpxDisplay_DisplayImage(IpxHandle hDisplay, const IpxImage* pImage, char* mode);
//! This C-interface function converts the input source IpxImage to the targeted output destination 
/*!
\param[in] hDisplay Pointer to the IpxHandle for the IpxDisplay 
\param[in] pSrc Pointer to the source IpxImage
\param[out] pDst Pointer to the output destination IpxImage
\return Returns the error code:
- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
*/
IPXD_EXTERN_C IPXD_API IpxError IPXD_CALL IpxDisplay_ConvertImage(IpxHandle hDisplay, const IpxImage* pSrc, IpxImage* pDst);
 /*! @} */
 /*! @} */
#endif // IPXDISPLAY_H
