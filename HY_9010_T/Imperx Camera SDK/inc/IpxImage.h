/////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxImage.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxImage data type declarations and inline functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Created: 26-March-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2015 -2016, Imperx Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////

#ifndef _IPX_IMAGE_H_
#define _IPX_IMAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IpxUserData.h"
#include "IpxPixelType.h"

//////////////////////////////////////////////////////////////////////
/// \defgroup ref_img IpxImage Header
/// \ingroup ref_data
///  \brief Defines, macros, and functions for IpxImage 
///@{

//////////////////////////////////////////////////////////////////////
/** \brief Defines major version of image data. */
#define IPX_IMAGE_MAJOR_VERSION				2
/** \brief Defines minor version of image data. */
#define IPX_IMAGE_MINOR_VERSION				0
/** \brief Defines whole version of image data. */
#define IPX_IMAGE_VERSION					((IPX_IMAGE_MAJOR_VERSION<<16)|IPX_IMAGE_MINOR_VERSION)
/** \brief Gets major version of image data. */
#define IPX_GET_MAJOR_VERSION(version)			(version>>16)
/** \brief Gets minor version of image data. */
#define IPX_GET_MINOR_VERSION(version)			((version<<16)>>16)

//////////////////////////////////////////////////////////////////////

/// Fills descriptor on base value of pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* @param descr Descriptor of pixel format.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
*/
IPX_INLINE bool IpxInitPixelTypeDescr(uint32_t pixelType, IpxPixelTypeDescr* descr)
{
	::memset(descr, 0, sizeof(IpxPixelTypeDescr));
	const IpxColorModelDescription* colorModDescr = IpxGetColorModelDescription(pixelType);
	if (!colorModDescr)
		return false;
	descr->pixelType = pixelType;
	descr->depth = colorModDescr->depth;
	descr->pixAlign = II_GET_PIXEL_ALIGNMENT(pixelType);
	descr->channels = colorModDescr->channels;
	descr->pixSize = II_GET_PIXEL_BITS_SIZE(pixelType);
	return true;
}
///@}

/// Data structure for description of Imperx Image.
//================================================================================
/**
* \note
* IpxImage stores the image data in a char array. The number of bytes per pixel channel
* is defined by Pixel Type. The field 'imageDataOrigin' is used by IpxImage API for memory management
* and should not be changed by user.
*
* \see IpxPixelTypeDescr<BR/>II_PIXEL_TYPE_DEFINES<BR/>IpxCreateImage<BR/>IpxCreateImageHeader<BR/>IpxReleaseImage
*/
typedef struct _IpxImage
{
#ifndef LINUX	
	_IpxImage()	{nSize = sizeof(IpxImage);
#else
	_IpxImage()	{nSize = sizeof(struct _IpxImage);
#endif		
				version = IPX_IMAGE_VERSION;
				width = height = imageSize = rowSize = 0; 
				imageData = imageDataOrigin = nullptr;
				origin = IIPL_ORIGIN_TL;
                imageID = timestamp = 0; userData = nullptr;}
	uint32_t nSize;                     /**<  Size of the IpxImage structure  */
    uint32_t version;				    /**< Version of data structure for image. */
	IpxPixelTypeDescr pixelTypeDescr;	/**< Descriptor of pixel format for image. */
    int32_t  origin;					/**< Origin of Image coordinate system: 0 - top-left origin: IIPL_ORIGIN_TL; 1 - bottom-left origin: IIPL_ORIGIN_BL (Windows bitmaps style). */
    uint32_t width;				        /**< Image width in pixels. */
    uint32_t height;				    /**< Image height in pixels. */
    uint32_t imageSize;			        /**< Image data size in bytes ( equals image->height*image->rowSize in case of interleaved data). */
    uint32_t rowSize;				    /**< Size of aligned image row in bytes	(not necessarily aligned) -	needed for correct deallocation. */
	uint64_t timestamp;					/**< Image timestamp. */
	uint64_t imageID;                   /**< Image identifier. For U3V and GEV - block_id field of Leader packet */
	IpxUserData* userData;		     	/**< User data, linked with this image. */
    char *imageData;					/**< Pointer to aligned image data. */
    char *imageDataOrigin;				/**< Pointer to very origin of image data. */
} IpxImage;


/** \brief Checks whether data is IpxImage type. */
#define IPX_IS_IMAGE_HDR(iiData) \
    ((iiData) != NULL && ((const IpxData*)(iiData))->nSize == sizeof(IpxImage))

/** \brief Gets pointer to data of first pixel. */
#define IPX_GET_FIRST_PIXEL_DATA(image)				image->imageData

/** \brief Gets pointer to data of defined pixel. */
#define IPX_GET_PIXEL_DATA(image, w, h, c)			image->imageData + h * image->rowSize\
													+ w * image->pixelTypeDescr.pixSize / 8\
													+ c * image->pixelTypeDescr.depth / 8


//////////////////////////////////////////////////////////////////////




///@}
//////////////////////////////////////////////////////////////////////
#endif // _IPX_IMAGE_H_
