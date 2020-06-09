////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxBayer.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IpxBayer component interface description
// Function and Constant Declarations for Bayer CFA demosaicing 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author: Maxim Bokov (maxb@imperx.com)
// Created:	13-APR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2013-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef _IPX_BAYER_H_
#define _IPX_BAYER_H_

#include "IpxImage.h"
#include "IpxToolsBase.h"

#ifndef BAYER_EXTERN_C
	#ifdef __cplusplus
		#define BAYER_EXTERN_C extern "C"
	#else
		#define BAYER_EXTERN_C
	#endif
#endif // BAYER_EXTERN_C

#ifndef BAYER_CALL
	#ifdef _WIN32 
		#define BAYER_CALL __stdcall
	#else
		#define BAYER_CALL
	#endif // _WIN32
#endif // BAYER_CALL

// check if static library is wanted
#ifdef BAYER_STATIC
	// define the static library symbol
	#define BAYER_API
	#define BAYER_LOCAL
#else
	#define BAYER_LOCAL LIB_HELPER_DLL_LOCAL
	#ifdef BAYER_EXPORTS
		// define the export symbol
		#define BAYER_API LIB_HELPER_DLL_EXPORT

		#if defined _WIN32 && !defined _M_AMD64
			// For 32b DLL only. If function name is mangled, add unmangled name below.
			#pragma comment(linker, "/EXPORT:IpxBayer_ConvertImage=_IpxBayer_ConvertImage@12")
			#pragma comment(linker, "/EXPORT:IpxBayer_CreateComponent=_IpxBayer_CreateComponent@0")
			#pragma comment(linker, "/EXPORT:IpxBayer_DeleteComponent=_IpxBayer_DeleteComponent@4")
			#pragma comment(linker, "/EXPORT:IpxBayer_GetComponent=_IpxBayer_GetComponent@4")
			#pragma comment(linker, "/EXPORT:IpxBayer_AllocData=_IpxBayer_AllocData@12")
			#pragma comment(linker, "/EXPORT:IpxBayer_ReleaseData=_IpxBayer_ReleaseData@4")
		#endif
	#else // BAYER_EXPORTS
		// define the import symbol
		#define BAYER_API LIB_HELPER_DLL_IMPORT
	#endif // BAYER_EXPORTS
#endif

///! IpxBayer component error codes
#define IPX_ERR_BAYER_INVALID_ARGUMENT IPX_ERR(IPX_CMP_BAYER_DEMOSAICING, IPX_ERR_INVALID_ARGUMENT)
#define IPX_ERR_BAYER_UNKNOWN          IPX_ERR(IPX_CMP_BAYER_DEMOSAICING, IPX_ERR_UNKNOWN)
#define IPX_ERR_BAYER_NO_MEMORY        IPX_ERR(IPX_CMP_BAYER_DEMOSAICING, IPX_ERR_NOT_ENOUGH_MEMORY)

/////////////////////////////////////////////////////////////////
/// \defgroup ipxbayer IpxBayer IpxComponent Header
/// \ingroup ipxdemosaicing
///  \brief Bayer functions and classes with IpxComponent features 
///@{
//////////////////////////////////////////////////////////////////

/** \addtogroup debayer DeBayer Parameters
 \brief Defines for DeBayer Parameters
 *  @{
<table>
		<caption id="multi_row">DeBayer Parameters</caption>
		<tr><th>Macro<th>Parameter Name<th>Type and Range<th>Description
		<tr><td rowspan="1"><b>DEBAYER_ALGO_TYPE</b><td>"BayerAlgType"<td>[int: 0,4]<td>Bayer Algorithm Type
		<tr><td rowspan="1"><b>DEBAYER_NOREALLOCT</b><td>"NoRealloc"<td>[int: 0,1]<td>No Realloc enabled
</table>
*/

#define DEBAYER_ALGO_TYPE	"BayerAlgType"	/*!< Bayer Algorithm Type\n\n<b>Type/Range</b>    [int: 0, 4]  \note Used by SetParamInt and GetParamInt*/
#define DEBAYER_NOREALLOCT 	"NoRealloc"		/*!< No Realloc enabled\n\n<b>Type/Range</b>    [int: 0,1]  \note Used by SetParamInt and GetParamInt*/
/**@}*/

/// Type of DeBayer Algorithms
/** \addtogroup debayer1 DeBayer Algorithms
 \brief Defines DeBayer Algorithms 
 \note These parameters are used in the SetIntParam function to program the DEBAYER_ALGO_TYPE parameter\n
 \n
 For example,
 \n
 \n
 \code 

 pDeBayer->GetComponent()->SetParamInt(DEBAYER_ALGO_TYPE, BAYER_AHD);
 \endcode
 *  @{*/
#define BAYER_SIMPLE		0	/**< Simple algorithm. Average quality, high speed. */
#define BAYER_GRADIENT		1	/**< Gradient Based algorithm. High quality, medium speed. */
#define BAYER_AHD			2	/**< The adaptive homogeneity-directed demosaicing (AHD) algorithm. High quality, slow speed. */
#define BAYER_OPENGL_MHC    3	/**< OpenGL MHC Algorithm. */
#define BAYER_OPENGL_MMA    4	/**< OpenGL MMA Algorithm. */
/**@}*/

#ifdef __cplusplus

/*! \addtogroup Class_IpxBayer IpxBayer C++ Class
 *   \brief C++ Class for IpxBayer
*  @{
*/

/**  
* A class containing methods for IpxBayer modules.   
\brief A Class for IpxBayer modules that contains methods to convert Bayer CFA (Color Filter Array) images
*/
class IpxBayer 
{
public:

	//! This function returns the created IpxBayer instance
	/*!
	\return Returns the created IpxBayer object
	*/
	static BAYER_API IpxBayer* CreateComponent();
	
	//! This function deletes the IpxBayer component and all associated resources obtained by the IpxBayer object.
	/*!
	\param[in] in Pointer to the IpxBayer object
	\return Returns void
	*/
	static BAYER_API void DeleteComponent(IpxBayer* in);

	//! This function returns the pointer to the IpxComponent object.
	/*!
	\brief The IpxComponent object will give access to the data member functions shown below:
	\image html componentFunctions.png
	\image latex componentFunctions.png
	\return Returns the Pointer to the IpxComponent object 
	
	The following example will illustrate on how to access the IpxComponent data member function:
	
	\code
	
	//Create the IpxBayer Component
	IpxBayer *pDeBayer = IpxBayer::CreateComponent();

	//Access the set parameter data member function using the GetComponent function. 
	//Set the "BayerAlgType" parameter to '2'.
	pDeBayer->GetComponent()->SetParamInt("BayerAlgType", 2);
	
	\endcode
	\n
	*/

	virtual IpxComponent* GetComponent()=0;

    //! This function Bayer CFA (Color Filter Array) Demosaicing converts the input source IpxImage to the targeted output destination IpxImage
	/*!
	
	\param[in] pSrc Pointer to the input source IpxImage 
	
	\brief The only input source Pixel Types supported are shown below:
	<center>
	<table>
		<caption id="multi_row">Input Source Supported Types</caption>
		<tr><th>Pixel Types
		<tr><td rowspan="1">II_PIX_BAYGR8
		<tr><td rowspan="1">II_PIX_BAYGR10
		<tr><td rowspan="1">II_PIX_BAYGR12
		<tr><td rowspan="1">II_PIX_BAYGR14
		<tr><td rowspan="1">II_PIX_BAYGR16
		<tr><td rowspan="1">II_PIX_BAYRG8
		<tr><td rowspan="1">II_PIX_BAYRG10
		<tr><td rowspan="1">II_PIX_BAYRG12
		<tr><td rowspan="1">II_PIX_BAYRG14
		<tr><td rowspan="1">II_PIX_BAYRG16
		<tr><td rowspan="1">II_PIX_BAYBG8
		<tr><td rowspan="1">II_PIX_BAYBG10
		<tr><td rowspan="1">II_PIX_BAYBG12
		<tr><td rowspan="1">II_PIX_BAYBG14
		<tr><td rowspan="1">II_PIX_BAYBG16
		<tr><td rowspan="1">II_PIX_BAYGB8
		<tr><td rowspan="1">II_PIX_BAYGB10
		<tr><td rowspan="1">II_PIX_BAYGB12
		<tr><td rowspan="1">II_PIX_BAYGB14
		<tr><td rowspan="1">II_PIX_BAYGB16
	</table>
	</center>
	\image html bayer_conversion.png "Example of a Bayer Conversion Process"
	\image latex bayer_conversion.png "Example of a Bayer Conversion Process"
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
#endif //__cplusplus

// C-interface

/*! \addtogroup C_IpxBayer IpxBayer C-Interface Functions
 *   \brief C-interface functions for IpxBayer
*  @{
*/

//! This C-interface function returns the IpxHandle for the created IpxBayer instance
/*!
\return Returns the IpxHandle for the created IpxBayer object
*/
BAYER_EXTERN_C BAYER_API IpxHandle BAYER_CALL IpxBayer_CreateComponent();

//! This C-interface function deletes the IpxHandle hBayer component and all associated resources obtained by the IpxBayer object
/*!
\param[in] hBayer Pointer to the IpxHandle for the IpxBayer instance
\return void
*/
BAYER_EXTERN_C BAYER_API void BAYER_CALL IpxBayer_DeleteComponent(IpxHandle hBayer);

//! This C-interface function returns the IpxHandle for the created IpxBayer instance
/*!
\param[in] hBayer Pointer to the IpxHandle for the IpxBayer instance
\return Returns the IpxHandle for the IpxBayer object component
*/
BAYER_EXTERN_C BAYER_API IpxHandle BAYER_CALL IpxBayer_GetComponent(IpxHandle hBayer);

//! This C-interface function converts the input source IpxImage to the targeted output destination 
/*!

\param[in] hBayer Pointer to the IpxHandle for the IpxBayer Component 
\param[in] pSrc Pointer to the source IpxImage
\param[out] pDst Pointer to the output destination IpxImage
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully converts the source IpxImage to the targeted output destination IpxImage  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems converting the IpxImage

*/
BAYER_EXTERN_C BAYER_API IpxError BAYER_CALL IpxBayer_ConvertImage(IpxHandle hBayer, const IpxImage* pSrc, IpxImage* pDst);

//! This C-interface function allocates the data
/*!
\param[in] hBayer Pointer of the IpxHandle for the IpxBayer Component
\param[in] pSrc Pointer to the source IpxImage
\param[in] pDst Pointer to the output destination IpxImage
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully allocates the IpxImage data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems allocating IpxImage data 
*/
BAYER_EXTERN_C BAYER_API IpxError BAYER_CALL IpxBayer_AllocData(IpxHandle hBayer, const IpxImage* pSrc, IpxImage* pDst);

//! This C-interface function release the IpxHandle to the IpxBayer data
/*!
\param[in] hBayer Pointer of the IpxHandle for the IpxBayer data
\return Returns the error code:
		- \c  IPX_ERR_OK Successfully releases the IpxBayer data.  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating problems releasing the IpxBayer data  
*/
BAYER_EXTERN_C BAYER_API void BAYER_CALL IpxBayer_ReleaseData(IpxHandle hBayer);
 /*! @} */


 /*! @} *///End of defgroup (ipxb_ref Bayer IpxComponent Header)
#endif // _IPX_BAYER_H_
