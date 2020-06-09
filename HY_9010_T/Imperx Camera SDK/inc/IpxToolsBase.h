///////////////////////////////////////////////////////////////////////////////////////////////////
// Imperx Imaging API SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxToolsBase.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Basic Declarations of Imperx Imaging API
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Author:	Nikolay Bitkin (Kola), kola@imperx.com
// Created:	24-MAR-2015
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2007-2015 Imperx Inc. All rights reserved. 
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \mainpage Main Page

The Imperx Tools is designed to provide software developers with API for ease of integrating Imperx camera's images 
into their software application. The API includes several component modules implementing the imaging functions. 
  
The API consist of several main classes that implement base IpxComponent class. The main classes are :
  
-    <b>\ref ipxbayer</b> - A header file containing C++ Class and C-Interface functions for IpxBayer component that contains methods to convert Bayer CFA Demosaic images.<br>
-    <b>\ref ipxdisplay</b> - A header file containing C++ Class and C-Interface functions for IpxDisplay component that contains methods to convert and display images.<br>
-    <b>\ref ipximageserializer</b> - A header file containing C++ Class and C-Interface functions for IpxImageSerializer component that contains methods to serialize IpxImage images.<br>
-    <b>\ref ipxtruesense</b> - A header file containing C++ Class and C-Interface functions for IpxTrueSense component that contains methods to convert TrueSense images.<br>
 
The Core features consist of defines, macros and functions used for the Imperx Image camera's image manipulation.

-	<b>\ref ref_img</b>		- A header file containing defines, macros, functions, and data structure for the description of Imperx Images
-   <b>\ref ref_imgapi</b>	- A header file containing Core image functions 
-	<b>\ref ref_pixel</b> - A header file containing the Image Pixel Types
-	<b>\ref tb</b> - A header file containing the defines/macros for errors and the base IpxComponent class
-	<b>\ref ref_usr</b>  - A header file containing the user data structure intended to store additional information about the image

\image html IpxComponents.png
\image latex IpxComponents.png
<center><b>IpxTools IpxComponents and Core Headers</b></center>

 */

#ifndef _IPX_TOOLS_BASE
#define _IPX_TOOLS_BASE

// MSVC++ 9.0 _MSC_VER == 1500 (Visual Studio 2008)
#if (_MSC_VER == 1500)
    #include "_stdint.h"
#else
    #include <stdint.h>
#endif

#ifndef _WIN32
	#include <cstddef>
#endif

#ifndef IPX_INLINE
	#if defined __cplusplus
		#define IPX_INLINE inline
	#else
		#define IPX_INLINE static
	#endif
#endif /* IPX_INLINE */

// generic definitions for library support
#if defined _WIN32 || defined __CYGWIN__
	#define LIB_HELPER_DLL_IMPORT __declspec(dllimport)
	#define LIB_HELPER_DLL_EXPORT __declspec(dllexport)
	#define LIB_HELPER_DLL_LOCAL
#elif __GNUC__ >= 4
    #define LIB_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define LIB_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define LIB_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
    #define LIB_HELPER_DLL_IMPORT
    #define LIB_HELPER_DLL_EXPORT
    #define LIB_HELPER_DLL_LOCAL
#endif

//////////////////////////////////////////////////////////////////////
/// \defgroup tb IpxToolBase Header
/// \brief Macros, defines, structures for IpxToolBase and IpxComponent Class 
///@{

/*! 
* @typedef IpxHandle
* @brief IpxHandle defines the handle of IpxTools component's instance
*/
typedef void* IpxHandle;

/**
* @brief The IpxRect structure defines a rectangle by the coordinates of its upper-left corner and width, height.
*/
typedef struct _IpxRect
{
	_IpxRect(){ x=y=width=height=0; };
	_IpxRect(int _x, int _y, int _w, int _h){ x=_x; y=_y; width=_w; height=_h; }
    int x; /**< Specifies the x-coordinate of the upper-left corner of the rectangle. */ 
    int y; /**< Specifies the y-coordinate of the upper-left corner of the rectangle. */ 
    int width; /**< Specifies the width of the rectangle. */ 
    int height; /**< Specifies the height of the rectangle. */ 
} IpxRect;

/**
* @brief The IpxSize structure specifies the width and height of a rectangle.
*/
typedef struct _IpxSize 
{	_IpxSize(){ width=height=0; }
	_IpxSize(int _w, int _h){ width=_w; height=_h; }
    int width;/**< Specifies the width of the rectangle. */ 
    int height;/**< Specifies the height of the rectangle. */ 
} IpxSize;


/*! 
* @typedef IpxError
* @brief Error definitions
*/
typedef uint32_t IpxError;


// IpxTools error has following structure:
// bytes 0, 1 - error code 
// byte  2    - component code
// byte  3    - severity: 00 - success, 80 - error
// Negative value - ERROR
// Positive value - WARNING
// Zero - SUCCESS

/**@brief Imperx Error Macro
*/
//#define IPX_ERR(__component__,__code__ ) (IpxError)(0x80000000 | (__component__<<16) | __code__)
#define IPX_ERR(__component__,__code__ ) (__code__==0) ? (IPX_ERR_OK) : ((0x80000000 | (__component__<<16) | __code__))
/**@brief Imperx Warning Macro
*/
#define IPX_WRN(__component__,__code__ ) (IpxError)((__component__<<16) | __code__)
/**@brief Imperx Error Code Succeeded Macro
*/
#define IPX_ERR_SUCCEEDED(__code__) ((__code__ & 0xFFFF) ==0)
/**@brief Imperx Error Code Failed Macro
*/
#define IPX_ERR_FAILED(__code__) (__code__>0x80000000)
/**@brief Imperx Error Code Warning Macro
*/
#define IPX_ERR_WARNING(__code__) ((__code__<0x80000000) && (__code__!=0))

/** @defgroup Common Error Codes
* \brief Common Error Codes
* This is the Common error codes
*/
// @{

/** @brief This error code occurs when the function was successful*/
#define IPX_ERR_OK               0
/** @brief This error code occurs when the function is not successful*/
#define IPX_ERR_UNKNOWN          1
/** @brief This error code occurs when the file is not found */
#define IPX_ERR_FILE_NOTFOUND    2
/** @brief This error code occurs when the parameter uses or functionality is not supported */
#define IPX_ERR_NOT_SUPPORTED    3
/** @brief This error code occurs when access is denied */
#define IPX_ERR_ACCESS_DENIED    4
/** @brief This error code occurs when the parameter valid set is out of range */
#define IPX_ERR_OUT_OF_RANGE     5
/** @brief This error code occurs when the buffer is too small */
#define IPX_ERR_BUFFER_TOO_SMALL   6
/** @brief This error code occurs when the argument passed in is an invalid argument */
#define IPX_ERR_INVALID_ARGUMENT   7
/** @brief This error code occurs when the parameter, source image, or destination image are unable to be created causing a null pointer */
#define IPX_ERR_NULL_POINTER       8
/** @brief This error code occurs when not enough memory was declared for the destination image */
#define IPX_ERR_NOT_ENOUGH_MEMORY  9
/** @brief This error code occurs when the function, parameter, or feature has not been implemented */
#define IPX_ERR_NOT_IMPLEMENTED    10
//@}  

// IpxTools Library component base class

// Components Type IDs
/** @defgroup Component Component Type IDs
* \brief Component Type IDs
*/
// @{

/** @brief IpxSerializer Component Type */
#define IPX_CMP_IMG_SERIALIZER      0x01
/** @brief IpxDemosaic Component Type */
#define IPX_CMP_BAYER_DEMOSAICING   0x05
/** @brief IpxDemosaic Component Type */
#define IPX_CMP_TS_DEMOSAICING      0x06
/** @brief IpxDisplay Component Type*/
#define IPX_CMP_DISPLAY             0x07
/** @brief IpxImageConverter Component Type*/
#define IPX_CMP_IMG_CONVERTER       0x08
/** @brief IpxImageUnacker Component Type */
#define IPX_CMP_IMG_UNPACKER        0x09
//@}

// Internal components
#define IPX_CMP_PARAM_ST            0x81

#define IPX_CMP_UNKNOWN             0xFF

// C++ interface
#ifdef __cplusplus  

/**  
* A class containing methods for IpxComponent modules.   
\brief A Class for IpxComponent modules that contains methods for setting/getting/executing Component features
*/

class IpxComponent
{
public:
	//! This function releases the resources obtained by the IpxComponent object.
	/*!
	\return Destructor of IpxComponent   
	*/
    virtual ~IpxComponent() {}
	
	// Component info methods
	//! This function returns the component type ID.
	/*!
	\return This function returns the component type ID.  
	*/
	virtual uint8_t  GetComponentTypeID()= 0;
	
	//virtual uint32_t GetComponentID()= 0;
	
	// Parameters methods
	
	//! This function returns the parameter count of the IpxComponent.
	/*!
	\return Returns the parameter count  
	*/
	virtual size_t GetParamCount() = 0;

	//! This function returns the parameter name associated with the index.
	/*!
	\param[in] index Parameter index
	\param[out] name Name of parameter
	\param[in] size input size 
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the name of the parameter of the specified index  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	virtual IpxError GetParamName(uint32_t index, char* name, uint32_t* size) = 0;
	
	//! This function gets the requested data information for the corresponding parameter name. This output information is a 'char' type variable.
	/*!
	\param[in] name Parameter name
	\param[out] param Name of parameter value that was requested and returned in a char string format.
	\param[in] size input size 
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully gets the name parameter requested data information of the specified index  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
    virtual IpxError GetParamAsString(char* name, char* param, uint32_t* size, const char *format=nullptr ) = 0;
	
	//! This function sets the named parameter with the parameter data information. The parameter data information is a 'char' type variable.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter data information is in a char string format.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamAsString(char* name, char* param ) = 0;

	//! This function sets the named bool parameter with the bool parameter data information. The parameter data information is a 'boolean' type variable.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter data information is a 'boolean' type variable.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamBool(char* name, bool param) = 0;
    
	//! This function sets the named integer parameter with the parameter data information. The parameter data information is a 'int64_t' type variable.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter data information is a 'int64_t' type variable.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamInt(char* name, int64_t param) = 0;
    
	//! This function sets the named float parameter with the parameter data information. The parameter data information is a 'double' type variable.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter data information is a 'double' type variable.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified   
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamFloat(char* name, double param) = 0;
   
	//! This function sets the named string parameter with the parameter data information. The parameter data information is in char string format.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter data information is in a char string format.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamString(char* name, char* param) = 0;

	//! This function sets the named array parameter with the parameter data information. The parameter data information is pointer to memory buffer.
	/*!
	\param[in] name Parameter name
	\param[in] param Parameter value that is being set. The parameter is pointer to memory buffer.
	\size[in] size Size of the memory buffer, specified in param argument, in bytes.
	\return Returns the error code:
		- \c IPX_ERR_OK Successfully sets the name parameter requested data information specified  
		- \c If IpxError code < 0,  then it returns a negative error code indicating the named parameter was not found
	*/
	virtual IpxError SetParamArray(char* name, void* param, uint32_t size) = 0;
	
	//! This function retrieves the bool parameter data information for the specified named parameter.
	/*!
	\param[in]  name Name of parameter
	\param[out] param retrieves the bool parameter data information 
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the named bool parameter data information  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	
	virtual IpxError GetParamBool(char* name, bool* param) = 0;
    	
	//! This function retrieves the integer parameter data information for the specified named parameter.
	/*!
	\param[in]  name Name of parameter
	\param[out] param retrieves the integer parameter data information 
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the named integer parameter data information  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	virtual IpxError GetParamInt(char* name, int64_t* param) = 0;
    
	//! This function retrieves the float parameter data information for the specified named parameter.
	/*!
	\param[in]  name Name of parameter
	\param[out] param retrieves the float parameter data information 
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the named float parameter data information  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	virtual IpxError GetParamFloat(char* name, double* param) = 0;
    
	//! This function retrieves the string parameter data information for the specified named parameter.
	/*!
	\param[in]  name Name of parameter
	\param[out] param retrieves the string parameter data information 
	\param[in] size Size of param string being retrieved.
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the named string parameter data information  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	
	virtual IpxError GetParamString(char* name, char* param, uint32_t* size) = 0;

	//! This function retrieves the string parameter data information for the specified named parameter.
	/*!
	\param[in]  name Name of parameter
	\param[out] param Pointer to memory buffer for parameter data information 
	\param[in] size Size of the memory buffer being retrieved, in bytes
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully gets the named string parameter data information  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	
	virtual IpxError GetParamArray(char* name, void* param, uint32_t *size) = 0;
	
	//! This function runs the command parameter specified.
	/*!
	\param[in]  name Name of parameter
	\return Returns the error code:
		- \c  IPX_ERR_OK Successfully runs the command parameter specified  
		- \c  If IpxError error code < 0, then it returns a negative error code indicating the named parameter was not found  
	*/
	virtual IpxError RunCommand(char* name ) = 0;

};
#endif //__cplusplus

// C-interface
// TODO: Implement C-interface here

///@}

#endif // _IPX_TOOLS_BASE
