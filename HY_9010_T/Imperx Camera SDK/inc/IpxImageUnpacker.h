////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxImageUnpacker.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxImageUnpacker component interface description
// Function and Constant Declarations for unpacking of packed pixel data 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author: Michael Malykh (MMA), michael.malykh@imperx.com
// Created:	17-APR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2015-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef IPXIMAGEUNPACKER_H
#define IPXIMAGEUNPACKER_H

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifndef IPXU_EXTERN_C
    #ifdef __cplusplus
        #define IPXU_EXTERN_C extern "C"
    #else
        #define IPXU_EXTERN_C
    #endif
#endif // IPXU_EXTERN_C

#ifndef IPXU_CALL
	#ifdef _WIN32
		#define IPXU_CALL __stdcall
	#else
		#define IPXU_CALL
	#endif
#endif // IPXU_CALL

// check if static library is wanted
#ifdef IPXIMAGEUNPACKER_STATIC
	// define the static library symbol
	#define IPXU_API
	#define IPXU_LOCAL
#else
	#define IPXU_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef IPXIMAGEUNPACKER_EXPORTS
		// define the export symbol
		#define IPXU_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxImageUnpacker_CreateComponent=_IpxImageUnpacker_CreateComponent@0")
			#pragma comment(linker, "/EXPORT:IpxImageUnpacker_DeleteComponent=_IpxImageUnpacker_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageUnpacker_GetComponent=_IpxImageUnpacker_GetComponent@4")
		//#pragma comment(linker, "/EXPORT:IpxImageUnpacker_Unpack=_IpxImageUnpacker_Unpack@12")
		#endif
	#else // IPXIMAGEUNPACKER_EXPORTS
		#ifdef IPXU_CODE
			#define IPXU_API
		#else
			// define the import symbol
			#define IPXU_API LIB_HELPER_DLL_IMPORT
		#endif
	#endif // IPXIMAGEUNPACKER_EXPORTS
#endif

///! IpxImageUnpacker component error codes
#define IPX_ERR_UNP_INVALID_ARGUMENT IPX_ERR(IPX_CMP_IMG_UNPACKER, IPX_ERR_INVALID_ARGUMENT)
#define IPX_ERR_UNP_UNKNOWN          IPX_ERR(IPX_CMP_IMG_UNPACKER, IPX_ERR_UNKNOWN)

#define IIU_CAMERATYPE              "cameraType" 
#define IIU_TAPSCOUNT				"tapsCount" 
#define IIU_WIDTH					"width" 
#define IIU_HEIGHT					"height" 
#define IIU_PIXELTYPE				"pixelType" 

///////////////////////////////////////////////////////////////////////////////
/// \defgroup ipxiu_ref Image Unpacker Reference
/// The following items exist in IpxImageUnpacker
///@{
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

/**
	IpxImageUnpacker
	@brief IpxComponent to unpack images
*/
class IpxImageUnpacker {
public:

	//! This function returns the created IpxImageUnpacker instance
	/*!
	\return Returns the created IpxImageUnpacker object
	*/
	static IPXU_API IpxImageUnpacker*   CreateComponent();
		
	//! This function deletes the IpxImageUnpacker component and all associated resources obtained by the IpxImageUnpacker object.
	/*!
	\param[in] component Pointer to the IpxImageUnpacker object
	\return Returns void
	*/
    static IPXU_API void                DeleteComponent(IpxImageUnpacker* component);
		
	//! This function returns the pointer to the IpxComponent object.
	/*!
	\return Returns the Pointer to the IpxComponent object 
	*/
    virtual IpxComponent*               GetComponent() = 0;
	
	//! This function transforms the packed source image to the unpacked one
	/*!
	* @param source Pointer to source IpxImage.
	* @param output Pointer to destination IpxImage.
	* \return 
	* If the function succeeds, the return value is 0.
	* If the function fails, the return value is non-zero.
	* \note 
	* This function transforms the source RAW image to the destination image by unpacking the image and deinterlacing if neccessary.
	* Also, it allocates the destination output image memory if the user didn't pre-allocation the destination image. 
	*
	* Fo example:
	*  
	* \code
 
	    IpxImageUnpacker*   unpacker = IpxImageUnpacker::CreateComponent();
	    IpxError            res = IPX_ERR_UNKNOWN;
	
	    ...
	
	    if (unpacker) res = unpacker->Unpack(source, output);
	          
	    if (!IPX_ERR_SUCCEEDED(res)) {
	          //TODO: Process error
	    }
	 
	    IpxImageUnpacker::DeleteComponent(unpacker);
	
	\endcode
	*/
    virtual IpxError Unpack(IpxImage* source, IpxImage* output, void* ptr=0) = 0;
	virtual IpxError Unpack(char* aPath, IpxImage** output) = 0;
	virtual IpxError deinterlace(IpxImage* source, IpxImage* output) = 0;
};
#endif //__cplusplus 

/*! \addtogroup C_IpxImageUnpacker IpxImageUnpacker C-Interface Functions
 *  Additional documentation for C-interface functions for 'IpxImageUnpacker'
*  @{
*/

//! This C-interface function returns the IpxHandle for the created IpxImageUnpacker instance
/*!
\return Returns the IpxHandle for the created IpxImageUnpacker object
*/
// C-interface
IPXU_EXTERN_C IPXU_API IpxHandle IPXU_CALL IpxImageUnpacker_CreateComponent();

//! This C-interface function deletes the IpxHandle hImageUnpacker component and all associated resources obtained by the IpxImageUnpacker object
/*!
\param[in] hImageUnpacker Pointer to the IpxHandle for the IpxImageUnpacker instance
\return void
*/
IPXU_EXTERN_C IPXU_API void IPXU_CALL IpxImageUnpacker_DeleteComponent(IpxHandle hImageUnpacker);

//! This C-interface function returns the IpxHandle for the created IpxImageUnpacker instance
/*!
\param[in] hImageUnpacker Pointer to the IpxHandle for the IpxImageUnpacker instance
\return Returns the IpxHandle for the IpxImageUnpacker object component
*/
IPXU_EXTERN_C IPXU_API IpxHandle IPXU_CALL IpxImageUnpacker_GetComponent(IpxHandle hImageUnpacker);

//! This C-interface function unpacks the input raw source IpxRAWImage to the targeted output destination IpxImage.
/*!
\param[in] hImageUnpacker Pointer of the IpxHandle for the IpxUnpacker
\param[in] source Pointer to the raw source IpxRawImage
\param[out] output Pointer to the output destination IpxImage 
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully unpacks the source IpxImage to the targeted output destination IpxImage.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems unpacking the IpxImage  
*/
IPXU_EXTERN_C IPXU_API IpxError IPXU_CALL IpxImageUnpacker_Unpack(IpxHandle hImageUnpacker, IpxImage* source, IpxImage* output);
 /*! @} */
 /*! @} */


#endif // IPXIMAGEUNPACKER_H
