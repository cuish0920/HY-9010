////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxImageApi.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Function and Constant Declarations for IpxImage instantiation and manipulations 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Created:	04-OCT-2013
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2013-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef _IPX_IMAGE_API_H_
#define _IPX_IMAGE_API_H_

#include "IpxImage.h"
#include "IpxToolsBase.h"

//////////////////////////////////////////////////////////////////////
// check if static library is wanted
#ifdef IPXIMAGE_STATIC
	// define the static library symbol
	#define IPXIMAGE_API
	#define IPXIMAGE_LOCAL
#else
	#define IPXIMAGE_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef IPXIMAGEAPI_EXPORTS
		// define the export symbol
		#define IPXIMAGE_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			//#pragma comment(linker, "/EXPORT:IpxSetMemoryManager=_IpxSetMemoryManager@12"))
		#endif
	#else // IPXIMAGEAPI_EXPORTS
		// define the import symbol
		#define IPXIMAGE_API LIB_HELPER_DLL_IMPORT
	#endif // IPXIMAGEAPI_EXPORTS
#endif

#if defined _WIN32 || defined _WIN64
    #define IPX_CDECL __cdecl
    #define IPX_STDCALL __stdcall
#else
    #define IPX_CDECL
    #define IPX_STDCALL
#endif

#ifdef __cplusplus
	extern "C" {			
#endif

//////////////////////////////////////////////////////////////////////
/// \defgroup ref_imgapi IpxImageApi Header
/// \ingroup ref_data
///  \brief Defines, macros, and functions for IpxImageApi
///@{
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
////////               Memory management                            ///////// 
/////////////////////////////////////////////////////////////////////////////

/// Signature of function that allocates a memory block.
/** \ingroup ref_mem */
typedef void*		(IPX_CDECL *PAllocFunc)(size_t size);

/// Signature of function that deallocates a memory block.
/** \ingroup ref_mem */
typedef int			(IPX_CDECL *PFreeFunc)(void* ptr);

//================================================================================
/// Sets user-defined memory management functions.
/**
* \ingroup ref_mem 
* @param allocFunc Pointer to the function that allocates a memory block.
* @param freeFunc Pointer to the function that deallocates a memory block.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note 
* It sets user-defined memory managment functions (substitutors for malloc and free) that
* will be called by IpxAlloc, IpxFree and higher-level functions (e.g. IpxCreateImage).
* If the user wants to use the default functions, then it should call IpxSetMemoryManager(NULL, NULL).
*
* For example:
*
* \code
	IpxSetMemoryManager(NULL, NULL);
	. . . . . 
	void* ptr = NULL;
	if (IPX_ERR_OK != IpxAlloc(&ptr, 12345))
	{
		IpxError error;
		IpxGetLastError(&error);
		::_ftprintf_s(file, _T("%s: %d; %s\n"), error.severity, error.code, error.description);
		return error.code;
	}
	. . . . .
	if (IPX_ERR_OK != IpxFree(&ptr))
	{
		IpxError error;
		IpxGetLastError(&error);
		::_ftprintf_s(file, _T("%s: %d; %s\n"), error.severity, error.code, error.description);
		return error.code;
	}
 \endcode
*/
IPXIMAGE_API IpxError IpxSetMemoryManager(PAllocFunc allocFunc, PFreeFunc freeFunc);

//================================================================================
/// Allocates a memory block.
/**
* \ingroup ref_mem
* @param ptr Pointer to the allocated space.
* @param size Number of bytes to allocate.
* \return  Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note 
* This function is<malloc> wrapper. if there is no enough memory, the function raises an error.
*
* See ::IpxSetMemoryManager for usage example.
* \see IpxFree
*/
IPXIMAGE_API IpxError IpxAlloc(void** ptr, size_t size);

//======================================================================================
/// Deallocates a memory block.
/**
* \ingroup ref_mem
* @param ptr Previously allocated memory block to be freed.
* \return   Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function is a free wrapper. Passing pointer to NULL pointer is Ok: nothing happens in this case
* Test requirements
* 
* See ::IpxSetMemoryManager for usage example.
* \see IpxAlloc
*/
IPXIMAGE_API IpxError IpxFree(void** ptr);

/** 
* \ingroup ref_mem
* \brief That is ::IpxFree wrapper.
*/
#define	IPX_FREE(ptr) IpxFree((void**)(ptr))  

/** 
* \ingroup ref_mem
* \brief That is ::IpxAlloc wrapper.
*/
#define	IPX_ALLOC(ptr, size) IpxAlloc((void**)(ptr), size)  


/////////////////////////////////////////////////////////////////////////////
/////      Core image functions                                        //////    
/////////////////////////////////////////////////////////////////////////////
/// \ingroup refimage Core Image Functions
///@{

/// Allocates memory for IpxImage header
//================================================================================
/**
* @param image Pointer to image header, that will be created.
* \return  Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function don't allocate memory for image data and don't set parameters in image header.
* \see  IpxCreateImageHeader<BR/>IpxReleaseImageHeader<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCreateEmptyImageHeader(IpxImage** image);


/// Allocates memory for IpxImage header and initializes it.
//================================================================================
/**
* @param image Pointer to image header, that will be created.
* @param size Horizontal and vertical size of image.
* @param pixelType Type of image pixel.
* @param imageData Pointer to image data.
* @param rowSize Size of image row in bytes.
* @param origin Origin of image coordinate system.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is a non-zero code.
* 
* \note
* This function uses a given image data.
* For example:
* \code
	IpxImage* img = NULL;
	IpxSize size;
	size.height = 480;
	size.width = 640;
	if (IPX_ERR_OK == IpxCreateImageHeader(&img, size, II_PIX_RGB8, NULL, 0))
	{
		IpxError error;
		IpxGetLastError(&error);
		return error.code;
	}
	. . . . .
	IpxReleaseImage(&img);
 \endcode//
* \see  IpxCreateImage<BR/>IpxReleaseImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCreateImageHeader(IpxImage** image, IpxSize size, uint32_t pixelType, char* imageData, uint32_t rowSize, int origin);


/// Initializes IpxImage header.
//================================================================================
/**
* @param image Pointer to image header.
* @param size Horizontal and vertical size of image.
* @param pixelType Type of image pixel.
* @param imageData Pointer to image data.
* @param rowSize Size of image row in bytes.
* @param origin Origin of image coordinate system.
* \return  Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function uses a given image data.
* For example:
* \code
	IpxImage* img = NULL;
	if (IPX_ERR_OK == IpxCreateEmptyImageHeader(&img))
	{
		IpxError error;
		IpxGetLastError(&error);
		return error.code;
	}
	IpxSize size;
	size.height = 480;
	size.width = 640;
	if (IPX_ERR_OK == IpxInitImageHeader(img, size, II_PIX_RGB8, NULL, 0))
	{
		IpxReleaseImageHeader(&img);
		IpxError error;
		IpxGetLastError(&error);
		return error.code;
	}
	. . . . .
	IpxReleaseImage(&img);
 \endcode
* \see  IpxCreateImage<BR/>IpxReleaseImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxInitImageHeader(IpxImage* image, IpxSize size, uint32_t pixelType, char* imageData, uint32_t rowSize, int origin);


/// Allocates memory for IpxImage data
//================================================================================
/**
* @param image Pointer to image header.
* \return  Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function allocates memory for image data in accordance to parameters of image header.
* \see  IpxCreateImageHeader<BR/>IpxReleaseImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCreateImageData(IpxImage* image);


/// Allocates memory for IpxImage header and data
//================================================================================
/**
* @param image Pointer to IpxImage, that will be created.
* @param size Horizontal and vertical size of image.
* @param pixelType Type of image pixel.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function allocates memory for image header and data.
* \see  IpxCreateImageHeader<BR/>IpxReleaseImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCreateImage(IpxImage** image, IpxSize size, uint32_t pixelType);

/// Releases memory of IpxImage header
//================================================================================
/**
* @param image Pointer to IpxImage image, that will be created.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function deallocates memory of image header, but not memory of image data.
*
* \see  IpxCreateImageHeader<BR/>IpxReleaseImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxReleaseImageHeader(IpxImage** image);

/// Releases memory of IpxImage header and data
//================================================================================
/**
* @param image Pointer to IpxImage image, that will be created.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note
* This function deallocates memory of image header and data.
*
* \see  IpxCreateImageHeader<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxReleaseImage(IpxImage** image);

/// Creates a new copy of IpxImage
//================================================================================
/**
* @param clone Pointer to new image.
* @param image Pointer to source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note 
* This function allocates memory for new image and copies source image to it.
*
* \see  IpxCloneImageExt<BR/>IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCloneImage(IpxImage** clone, const IpxImage* image);

/// Creates a new copy of IpxImage with restriction by ROI and COI 
//================================================================================
/**
* @param clone Pointer to new image.
* @param image Pointer to source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully creates a new copy of IpxImage with restriction by ROI and COI
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note 
* This function allocates memory for the new image and copies the image data that is restricted by ROI and COI.
*
* \see  IpxCloneImage<BR/>IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCloneImageExt(IpxImage** clone, const IpxImage* image);

/// Copies source image header to destination image
//================================================================================
/**
* @param dstImage Pointer to destination image.
* @param srcImage Pointer to source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies source image header to destination image
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
*
* \note 
* This function copies source image header to destination image.
*
* \see  IpxCloneMatrix<BR/>IpxCreateMatrixFromImage
*/
IPXIMAGE_API IpxError IpxCopyImageHeader(IpxImage* dstImage, const IpxImage* srcImage);

/// Copy source image to destination image
//================================================================================
/**
* @param dstImage Pointer to destination image.
* @param srcImage Pointer to source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies source image to destination image
* - \c IPX_ERR_NULL_POINTER  - No srcImage or dstImage.
* - \c IPX_ERR_INVALID_ARGUMENT - invalid argument. For example, dstSize is less than srcImage size
*
* \note 
* This function checks coincidence of size and pixel type in source and destination.
* 
* \see  IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCopyImage(IpxImage* dstImage, const IpxImage* srcImage);

/// Copies a color channel of source image to a chars array
//================================================================================
/**
* @param dst Pointer to destination array.
* @param dstSize Pointer to the value of array size.
* @param srcImage Pointer to source image.
* @param channel Channel number of source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies a color channel of source image to a char array
* - \c IPX_ERR_NULL_POINTER  - Unable to copy the image Channels.
* - \c IPX_ERR_INVALID_ARGUMENT - invalid argument. For example, dstSize is less than srcImage size
*
* \note 
* This function copies a color channel of source image to a char array.
* If dst = NULL or *dstSize = 0, then the function returns the required size of destination array in dstSize value.
* Otherwise, the function returns the size of image row in pixels.
* 
* \see  IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCopyImageChannelChar(unsigned char* dst, int* dstSize, const IpxImage* srcImage, const int channel);

/// Copies a color channel of source image to a short array
//================================================================================
/**
* @param dst Pointer to destination array.
* @param dstSize Pointer to the value of array size.
* @param srcImage Pointer to source image.
* @param channel Channel number of source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies a color channel of source image to a short array
* - \c IPX_ERR_NULL_POINTER  - Unable to copy the image Channels.
* - \c IPX_ERR_INVALID_ARGUMENT - invalid argument. For example, dstSize is less than srcImage size
*
* \note 
* This function copies a color channel of source image to a shorts array.
* If dst = NULL or *dstSize = 0, then the function returns the required size of the destination array in dstSize value.
* Otherwise, the function returns the size of image row in pixels.
*
* \see  IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCopyImageChannelShort(unsigned short* dst, int* dstSize, const IpxImage* srcImage, const int channel);

/// Copies a color channel of source image to a integer array
//================================================================================
/**
* @param dst Pointer to destination array.
* @param dstSize Pointer to the value of array size.
* @param srcImage Pointer to source image.
* @param channel Channel number of source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies a color channel of source image to an integer array
* - \c IPX_ERR_NULL_POINTER  - Unable to copy the image Channels.
* - \c IPX_ERR_INVALID_ARGUMENT - invalid argument. For example, dstSize is less than srcImage size
*
* \note 
* This function copies a color channel of source image to an integer array.
* If dst = NULL or *dstSize = 0, then the function returns the required size of the destination array in dstSize value.
* Otherwise, the function returns the size of the image row in pixels.
*
* \see  IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCopyImageChannelInt(int* dst, int* dstSize, const IpxImage* srcImage, const int channel);

/// Copies a color channel of source image to a float array
//================================================================================
/**
* @param dst Pointer to destination array.
* @param dstSize Pointer to the value of array size.
* @param srcImage Pointer to source image.
* @param channel Channel number of source image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully copies a color channel of source image to a float array
* - \c IPX_ERR_NULL_POINTER  - Unable to copy the image Channels.
* - \c IPX_ERR_INVALID_ARGUMENT - invalid argument. For example, dstSize is less than srcImage size
*
* \note 
* This function copies a color channel of source image to a floats array.
* If dst = NULL or *dstSize = 0, then the function returns the required size of destination array in dstSize value.
* Otherwise, the function returns the size of the image row in pixels.
*
* \see  IpxCopyImage<BR/>IpxCreateImage<BR/>IpxImage
*/
IPXIMAGE_API IpxError IpxCopyImageChannelFloat(float* dst, int* dstSize, const IpxImage* srcImage, const int channel);

/// Sets a Channels Of Interest (COI) for image
//================================================================================
/**
* @param image Pointer to image.
* @param coi Pointer to Channels Of Interest.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully resets the image Channels of Interest (COI)
* - \c IPX_ERR_NULL_POINTER  - Unable to set Channels of Interest.
*
* \note 
* This function sets a Channels Of Interest (COI) for image. 
* Some functions support COI in image proccessing (for example: IpxCopyImage extracts the selected channel
* and/or put it back.)
*
* \see  IpxCreateImage<BR/>IpxImage
*/
//IPXIMAGE_API IpxError IpxSetImageCOI(IpxImage* image, const IpxCOI* coi);

/// Resets a Channels Of Interest (COI) for image
//================================================================================
/**
* @param image Pointer to image.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully resets the image Channel of Interest (COI)
* - \c IPX_ERR_NULL_POINTER  - Unable to set Region of Interest.
*
* \note 
* This function resets a Channels Of Interest (COI) i.e. sets COI to all color channels. 
*
* \see  IpxCreateImage<BR/>IpxImage
*/
//IPXIMAGE_API IpxError IpxResetImageCOI(IpxImage* image);

/// Sets a Region Of Interest (ROI) for image
//================================================================================
/**
* @param image Pointer to image.
* @param roi Pointer to Region Of Interest.
* \return Returns the error code:
* - \c IPX_ERR_OK  -  Successfully resets the image Region of Interest (ROI)
* - \c IPX_ERR_NULL_POINTER  - Unable to set Region of Interest.
*
* \note 
* This function sets a Region Of Interest (ROI) for image. 
* Some functions support ROI in image proccessing.
*
* \see  IpxCreateImage<BR/>IpxImage
*/
//IPXIMAGE_API IpxError IpxSetImageROI(IpxImage* image, const IpxROI* roi);

/// Resets a Region Of Interest (ROI) for image
//================================================================================
/**
* @param image Pointer to image.
* \return  Returns the error code:
* - \c IPX_ERR_OK  -  Successfully resets the image Region of Interest (ROI)
* - \c IPX_ERR_NULL_POINTER  - Unable to set Region of Interest.
*
* \note 
* This function resets a Region Of Interest (ROI) i.e. sets ROI to width and height of image. 
*
* \see  IpxCreateImage<BR/>IpxImage
*/
//IPXIMAGE_API IpxError IpxResetImageROI(IpxImage* image);


/*! @} */
///@}
#ifdef __cplusplus
	}				
#endif

//////////////////////////////////////////////////////////////////////
#endif //_IPX_IMAGE_API_H_

