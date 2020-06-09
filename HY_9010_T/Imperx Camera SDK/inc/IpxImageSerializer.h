////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxImageSerializer.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxImageSerializer component interface description
// Function and Constant Declarations for serialization of IpxImage object(s)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author: Michael Malykh (MMA), michael.malykh@imperx.com
// Created:	19-MAY-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2015-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef IPXIMAGESERIALISER_H
#define IPXIMAGESERIALISER_H

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifndef IPXS_EXTERN_C
    #ifdef __cplusplus
        #define IPXS_EXTERN_C extern "C"
    #else
        #define IPXS_EXTERN_C
    #endif
#endif // IPXS_EXTERN_C

#ifndef IPXS_CALL
	#ifdef _WIN32
		#define IPXS_CALL __stdcall
	#else
		#define IPXS_CALL
	#endif
#endif // IPXS_CALL

// check if static library is wanted
#ifdef IPXIMAGESERIALIZER_STATIC
	// define the static library symbol
	#define IPXS_API
	#define IPXS_LOCAL
#else
	#define IPXS_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef IPXIMAGESERIALIZER_EXPORTS
		// define the export symbol
		#define IPXS_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_CreateComponent=_IpxImageSerializer_CreateComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_DeleteComponent=_IpxImageSerializer_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_GetComponent=_IpxImageSerializer_GetComponent@4")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_StartSeriesRecord=_IpxImageSerializer_StartSeriesRecord@12")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_StartMovieRecord=_IpxImageSerializer_StartMovieRecord@20")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_FinishRecord=_IpxImageSerializer_FinishRecord@4")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_Save=_IpxImageSerializer_Save@12")
			#pragma comment(linker, "/EXPORT:IpxImageSerializer_Load=_IpxImageSerializer_Load@12")
		#endif
	#else // IPXIMAGESERIALIZER_EXPORTS
		#ifdef IPXS_CODE
			#define IPXS_API
		#else
			// define the import symbol
			#define IPXS_API LIB_HELPER_DLL_IMPORT	
		#endif
	#endif // IPXIMAGESERIALIZER_EXPORTS
#endif

//////////////////////////////////////////////////////
/// \defgroup ipximageserializer IpxImageSerializer IpxComponent Header
/// \ingroup serializer
/// \brief IpxImageSerializer vunctions and classes with IpxComponent features 
///@{
//////////////////////////////////////////////////////

// Serializer Parameters

/** \addtogroup ts_ser IpxSerializer Parameters
\brief Defines for IpxSerializer Parameters
*  @{
<table>
		<caption id="multi_row">IpxSerializer Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type<th>Description
		<tr><td rowspan="1"><b>ISP_NO_REALLOC</b><td>"NoRealloc"<td>[int: 0, 1]<td>does not allow to realloc buffers on runtime
		<tr><td rowspan="1"><b>ISP_JPEG_QUALITY</b><td>"jpeg.quality"<td>[int: 1,100]<td>jpeg quality (1..100; 85 by default)
		<tr><td rowspan="1"><b>ISP_MOVIE_COMPRESSOR</b><td>"movie.compressor"<td>[char*]<td>movie compressor (char*; "Uncompressed" by default)
		<tr><td rowspan="1"><b>ISP_MOVIE_HAS_OPTIONS</b><td>"movie.options"<td>[int: 0,1]<td>// 1 if movie has options, 0 otherwise
		<tr><td rowspan="1"><b>ISP_MOVIE_COMPRESSORS</b><td>"movie.compressors"<td>[char*]<td>// char* list of available compressors separated by |
		<tr><td rowspan="1"><b>ISP_PARENT_WINDOW</b><td>"parent.window"<td>[char*]<td>// (optional) parent window handle for options dialog (%p)
		<tr><td rowspan="1"><b>ISC_SHOW_OPTIONS</b><td>"show.options"<td>[N/A]<td>show options dialog
</table>*/

#define ISP_NO_REALLOC              "NoRealloc"                 /*!<Does not allow to realloc buffers on runtime\n\n<b>Type/Range</b>   [int: 0, 1]\note Used by SetParamInt and GetParamInt*/
#define ISP_JPEG_QUALITY            "jpeg.quality"              /*!<Jpeg quality (1..100; 85 by default)\n\n<b>Type/Range</b>   [int: 1,100]\note Used by SetParamInt and GetParamInt*/
#define ISP_MOVIE_COMPRESSOR        "movie.compressor"          /*!<Movie compressor (char*; "Uncompressed" by default)\n\n<b>Type/Range</b>   [char*]\note Used by SetParamString and GetParamString*/
#define ISP_MOVIE_HAS_OPTIONS       "movie.options"             /*!<1 if movie has options, 0 otherwise\n\n<b>Type/Range</b>   [int: 0,1]\note Used by SetParamInt and GetParamInt*/
#define ISP_MOVIE_COMPRESSORS       "movie.compressors"         /*!<char* list of available compressors separated by |\n\n<b>Type/Range</b>   [char*]\note Used by SetParamString and GetParamString*/
#define ISP_PARENT_WINDOW           "parent.window"             /*!<(optional) parent window handle for options dialog (%p)\n\n<b>Type/Range</b>   [char*]\note Used by SetParamString and GetParamString*/

#define ISC_SHOW_OPTIONS            "show.options"              /*!<show options dialog\n\n<b>Type/Range</b>   [N/A]\note Used by RunCommand*/
/**@}*/


#ifdef __cplusplus

/*! \addtogroup Class_IpxImageSerializer IpxImageSerializer C++ Class
 * \brief C++ Class for IpxImageSerializer
*  @{
*/
/**
	IpxImageSerializer
	@brief IpxComponent to save IpxImage to disk
*/

class IpxImageSerializer {
public:
	//! This function returns the IpxHandle for the created IpxImageSerializer instance
	/*!
	\param[in] enableMovies flag to enable Movies
	\return Returns the IpxHandle for the created IpxImageSerializer object
	*/
	static IPXS_API IpxImageSerializer* CreateComponent(bool enableMovies = true);
	//! This function returns the IpxHandle for the created IpxImageSerializer instance
	/*!
	\param[in] component Pointer to IpxImageSerializer component
	\return void
	*/
    static IPXS_API void                DeleteComponent(IpxImageSerializer* component);
	
	//! This function returns the pointer to the IpxComponent object.
	/*!
	\brief The IpxComponent object will give access to the data member functions shown below:
	\image html componentFunctions.png
	\image latex componentFunctions.png
	\return Returns the Pointer to the IpxComponent object 
	
	The following example will illustrate on how to access the IpxComponent data member function:
	
	\code

    IpxImageSerializer* serializer = IpxImageSerializer::CreateComponent();
    IpxError            res = IPX_ERR_UNKNOWN;

    if (serializer) {
         (serializer->GetComponent())->SetParamInt(ISP_JPEG_QUALITY, mJpgQuality);
         res = serializer->Save(image, fileName);
    }
          
    ...

    IpxImageSerializer::DeleteComponent(serializer);
	
	\endcode

	*/


    virtual IpxComponent*               GetComponent() = 0;
	//! This function starts the recording session for series of images of the same format
	/*!
	\param[in] pSrc input source Imperx Image
	\param[in] format Image Format Type
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully starts the recording series.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem starting the recording series. 
	*/
    // Start recording session for series of images of the same format
    virtual IpxError                    StartSeriesRecord(IpxImage* pSrc, char* format) = 0;

	//! This function starts the recording session for movies
	/*!
	\param[in] pSrc input source Imperx Image
	\param[in] fileName file name
	\param[in] fps frames per second
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully starts the recording session for movies.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem starting the recording session for movie 
	*/
    virtual IpxError                    StartMovieRecord(IpxImage* pSrc, char* fileName, double fps) = 0;
	//! This function ends the recording session
	/*!
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully ends the recording session.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem ending the recording session 
	*/
    virtual IpxError                    FinishRecord() = 0;
	//! This function saves the standalone image or puts the image to recording session if StartSeriesRecord() was called
	/*!
	\param[in] image input source Imperx Image
	\param[in] fileName file name
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully saves the standalone image
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem saving the record 
	*/
    virtual IpxError                    Save(IpxImage* image, char* fileName = 0) = 0;
	//! This function reads and loads the standalone image record
	/*!
	\param[in] image input source Imperx Image
	\param[in] fileName file name
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully allocates the IpxImage data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems reading the standalone image 
	*/
    virtual IpxError                    Load(IpxImage* image, char* fileName) = 0;
	//! This function frees the image loaded with IpxImageSerializer
	/*!
	\param[in] image input source Imperx Image
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully frees the allocates memory of the IpxImage image.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems freeing the loaded image 
	*/
    virtual IpxError                    Free(IpxImage* image) = 0;
};
 /*! @} */
#endif //__cplusplus 

// C-interface

/*! \addtogroup C_IpxImageSerializer IpxImageSerializer C-Interface Functions
*  \brief C-interface functions for IpxImageSerializer
*  @{
*/
//! This C-interface function returns the IpxHandle for the created IpxImageSerializer instance
/*!
\param[in] enableMovies flag to enable Movies
\return Returns the IpxHandle for the created IpxImageSerializer object
*/
// C-interface
IPXS_EXTERN_C IPXS_API IpxHandle IPXS_CALL IpxImageSerializer_CreateComponent(bool enableMovies);

//! This C-interface function returns the IpxHandle for the created IpxImageSerializer instance
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\return void
*/
IPXS_EXTERN_C IPXS_API void IPXS_CALL IpxImageSerializer_DeleteComponent(IpxHandle hImageSerializer);
//! This C-interface function returns the IpxHandle for the created IpxImageSerializer instance
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\return Returns the IpxHandle for the IpxImageSerializer object component
*/
IPXS_EXTERN_C IPXS_API IpxHandle IPXS_CALL IpxImageSerializer_GetComponent(IpxHandle hImageSerializer);
//! This C-interface function starts the series record
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\param[in] pSrc input source Imperx Image
\param[in] format Image Format Type
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully starts the series record.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem starting the series record 
*/
IPXS_EXTERN_C IPXS_API IpxError IPXS_CALL IpxImageSerializer_StartSeriesRecord(IpxHandle hImageSerializer, IpxImage* pSrc, char* format);
//! This C-interface function starts the series record
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\param[in] pSrc input source Imperx Image
\param[in] fileName file name
\param[in] fps frames per second
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully starts movie record.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem starting the movie record 
*/
IPXS_EXTERN_C IPXS_API IpxError IPXS_CALL IpxImageSerializer_StartMovieRecord(IpxHandle hImageSerializer, IpxImage* pSrc, char* fileName, double fps);
//! This C-interface function finishes the record
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully finishes the record.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem finishing the record 
*/
IPXS_EXTERN_C IPXS_API IpxError IPXS_CALL IpxImageSerializer_FinishRecord(IpxHandle hImageSerializer);
//! This C-interface function saves the record
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\param[in] image input source Imperx Image
\param[in] fileName file name
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully saves the record
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problem saving the record 
*/
IPXS_EXTERN_C IPXS_API IpxError IPXS_CALL IpxImageSerializer_Save(IpxHandle hImageSerializer, IpxImage* image, char* fileName);
//! This C-interface function loads the record
/*!
\param[in] hImageSerializer Pointer to the IpxHandle for the IpxImageSerializer instance
\param[in] image input source Imperx Image
\param[in] fileName file name
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully allocates the IpxImage data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems allocating IpxImage data 
*/
IPXS_EXTERN_C IPXS_API IpxError IPXS_CALL IpxImageSerializer_Load(IpxHandle hImageSerializer, IpxImage* image, char* fileName);
 /*! @} */
 /*! @} */
 
/////////////////////////////////////////////////////////////////////
//      Usage example:
//      
//      IpxImageSerializer* serializer = IpxImageSerializer::CreateComponent();
//      IpxError            res = IPX_ERR_UNKNOWN;
//
//      if (serializer) {
//          if (mFormat == FORMAT_JPG) (serializer->GetComponent())->SetParamInt(ISP_JPEG_QUALITY, mJpgQuality);
//          res = serializer->Save(image, fileName);
//      }
//          
//      if (!IPX_ERR_SUCCEEDED(res)) {
//          //TODO: Process error
//      }
//
//      IpxImageSerializer::DeleteComponent(serializer);
//      
// Copyright (C) 2015 Imperx Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////
#endif // IPXIMAGESERIALISER_H
