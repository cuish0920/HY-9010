////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxImageConverter.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxImageConverter component interface description
// Function and Constant Declarations for conversion functionality of IpxImage  
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Created:	15-APR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2013-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef IPXIMAGECONVERTER_H
#define IPXIMAGECONVERTER_H

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifndef IPXC_EXTERN_C
    #ifdef __cplusplus
        #define IPXC_EXTERN_C extern "C"
    #else
        #define IPXC_EXTERN_C
    #endif
#endif // IPXC_EXTERN_C

#ifndef IPXC_CALL
	#ifdef _WIN32
		#define IPXC_CALL __stdcall
	#else
		#define IPXC_CALL
	#endif
#endif // IPXC_CALL

// check if static library is wanted
#ifdef IPXIMAGECONVERTER_STATIC
	// define the static library symbol
	#define IPXC_API
	#define IPXC_LOCAL
#else
	#define IPXC_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef IPXIMAGECONVERTER_EXPORTS
		// define the export symbol
		#define IPXC_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxImageConverter_CreateComponent=_IpxImageConverter_CreateComponent@0")
			#pragma comment(linker, "/EXPORT:IpxImageConverter_DeleteComponent=_IpxImageConverter_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageConverter_GetComponent=_IpxImageConverter_GetComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageConverter_ConvertImage=_IpxImageConverter_ConvertImage@12")
			#pragma comment(linker, "/EXPORT:IpxImageConverter_IIConvert=_IpxImageConverter_IIConvert@16")
		#endif
	#else // IPXIMAGECONVERTER_EXPORTS
		#ifdef IPXC_CODE
			#define IPXC_API
		#else
			// define the import symbol
			#define IPXC_API LIB_HELPER_DLL_IMPORT	
		#endif
	#endif // IPXIMAGECONVERTER_EXPORTS
#endif

//////////////////////////////////////////////////////
/// \defgroup ipxic_ref Image Converter Reference
/// The following items are exists in Image Converter
///@{
////////

#ifdef __cplusplus
/**  
* A class containing methods for IpxImageConverter modules.   
\brief A Class for IpxImageConverter modules that contains methods to convert IpxImage images
*/
class IpxImageConverter {
public:

	//! This function returns the created IpxImageConverter instance
	/*!
	\return Returns the created IpxImageConverter object
	*/
	static IPXC_API IpxImageConverter*  CreateComponent();
	
	//! This function deletes the IpxImageConverter component and all associated resources obtained by the IpxImageConverter object.
	/*!
	\param[in] component Pointer to the IpxImageConverter object
	\return Returns void
	*/
    static IPXC_API void                DeleteComponent(IpxImageConverter* component);
		
	//! This function returns the pointer to the IpxComponent object.
	/*!
	\return Returns the Pointer to the IpxComponent object 
	*/
    virtual IpxComponent*               GetComponent() = 0;

    //! This function converts the input source IpxImage to the targeted output destination IpxImage
	/*!
	\param[in] source Pointer to the input source IpxImage 
	\param[out] output Pointer to the output destination IpxImage
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
	*/
	virtual IpxError                    ConvertImage(IpxImage* source, IpxImage* output) = 0;

	//! This function converts the input sorce IpxImage to the targeted output destination IpxImage based on the output pixel type.
	/*!
	\param[in] image_in input source IpxImage image
	\param[in] outPixelType Output pixel type
	\param[out] image_out Pointer to the output destination IpxImage 
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage based on the output pixel type.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
	*/
    virtual IpxError                    IIConvert(IpxImage* image_in, unsigned long outPixelType, IpxImage** image_out) = 0;

};
#endif //__cplusplus 

/*! \addtogroup C_IpxImageConverter IpxImageConverter C-Interface Functions
 *  Additional documentation for C-interface functions for 'IpxImageConverter'
*  @{
*/

//! This C-interface function returns the IpxHandle for the created IpxImageConverter instance
/*!
\return Returns the IpxHandle for the created IpxImageConverter object
*/
IPXC_EXTERN_C IPXC_API IpxHandle IPXC_CALL IpxImageConverter_CreateComponent();

//! This C-interface function deletes the IpxHandle hImageConverter component and all associated resources obtained by the IpxImageConverter object
/*!
\param[in] hImageConverter Pointer to the IpxHandle for the IpxImageConverter instance
\return void
*/
IPXC_EXTERN_C IPXC_API void IPXC_CALL IpxImageConverter_DeleteComponent(IpxHandle hImageConverter);
//! This C-interface function returns the IpxHandle for the created IpxImageConverter instance
/*!
\param[in] hImageConverter Pointer to the IpxHandle for the IpxImageConverter instance
\return Returns the IpxHandle for the IpxImageConverter object component
*/
IPXC_EXTERN_C IPXC_API IpxHandle IPXC_CALL IpxImageConverter_GetComponent(IpxHandle hImageConverter);
//! This C-interface function converts the input source IpxImage to the targeted output destination 
/*!
\param[in] hImageConverter Pointer to the IpxHandle for the IpxImage Converter 
\param[in] source Pointer to the source IpxImage
\param[out] output Pointer to the output destination IpxImage
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
*/
IPXC_EXTERN_C IPXC_API IpxError IPXC_CALL IpxImageConverter_ConvertImage(IpxHandle hImageConverter, IpxImage* source, IpxImage* output);
//! This C-interface function converts the input source IpxImage to the targeted output destination IpxImage based on the output pixel type.
/*!
\param[in] hImageConverter Pointer of the IpxHandle for the IpxImage Converter
\param[in] image_in Pointer to the source IpxImage
\param[in] outPixelType Output pixel type
\param[out] image_out Pointer to the output destination IpxImage 
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage based on the output pixel type.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage  
*/
IPXC_EXTERN_C IPXC_API IpxError IPXC_CALL IpxImageConverter_IIConvert(IpxHandle hImageConverter, IpxImage* image_in, unsigned long outPixelType, IpxImage** image_out);
 /*! @} */
 /*! @} */
#endif // IPXIMAGECONVERTER_H
