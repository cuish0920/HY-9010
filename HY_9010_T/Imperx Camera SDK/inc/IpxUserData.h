// IpxImage.h: Definition of data structure for Image, that is supported by IIPL. Basic definitions for the IIPL.

//
//////////////////////////////////////////////////////////////////////

#if !defined(IpxUserData_H__INCLUDED_)
#define IpxUserData_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <stdio.h>

#include "IpxPixelType.h"


//////////////////////////////////////////////////////////////////////
/// \defgroup ref_usr IpxUserData Header
/// \brief Defines for user data types for Images
/// \ingroup ref_usr1
///@{

/// Definition of user data types for Images which are processed.
//================================================================================
/**
* \note
* The User data are intended to store additional information about the image
* \see IpxUserData<BR/>IpxCreateUserData<BR/>IpxReleaseUserData
*/
typedef enum : unsigned long
{
	IPX_NOT_DATA	= 0,		/**< Type of user data is undefined. */
	IPX_HASHTABLE_DATA,		    /**< User data are placed into hashtable. */
	IPX_XML_DATA,			    /**< User data have XML format. */
	IPX_CUSTOM_DATA,			/**< Format of user data is defined by customer. */
}IPX_USER_DATA;

//================================================================================
/// Data structure for description of User Data linked with Imperx Image.
/**
* \note
* IpxTools Library provides only base operation for handling of user data.
* \see IpxCreateUserData<BR/>IpxReleaseUserData
*/
typedef struct _IpxUserData
{
	unsigned long	type;		/**< <B>type</B> Type of user data. (II_NOT_DATA, II_HASHTABLE, II_XML_DATA, II_CUSTOM_DATA) */
	unsigned long	id;			/**< <B>id</B> ID of user data (must be > 0). */
	unsigned long	size;		/**< <B>size</B> Size of user data. */
	void*			data;		/**< <B>data</B> User data. */
	bool			createdIpx;	/**< <B>createdIpx</B>  Indicates if user data was created by IpxTools. If true, then the user data is created by IpxTools. */
	_IpxUserData*   pNext;      /**< <B>pNext</B> Pointer to next User data block, or nullptr if next block does not exist. */
} IpxUserData;

/// Allocates and initializes the User Data.
//================================================================================
/**
* @param userData Pointer to the User Data.
* @param type Type of user data.
* @param id ID of user data (must be > 0).
* @param dataSize Size of user data.
* \return 
* If the function succeeds, the return boolean value is 'true'.
* If the function fails, the return boolean value is 'false'.
* \note 
* This function allocates memory for the User Data and cleans the allocated memory.
* \see  IpxReleaseUserData<BR/>IpxLoadUserData<BR/>IpxSaveUserData
*/
static bool IpxCreateUserData(IpxUserData** userData, unsigned long type, unsigned long id, unsigned long dataSize)
{
	if (!userData)
		return false;
	size_t totalSize = dataSize + sizeof(IpxUserData);
	char* buff = (char*)malloc(totalSize);
	if (!buff)
	{
		return false;
	}
	memset(buff, 0, totalSize);
	IpxUserData* tmpData = (IpxUserData*)buff;
	tmpData->createdIpx = true;
	tmpData->id = id;
	tmpData->type = type;
	tmpData->size = dataSize;
	if (dataSize > 0)
		tmpData->data = buff + sizeof(IpxUserData);
	else
		tmpData->data = 0;

	*userData = tmpData;
	return true;
}

/// Deallocates memory associated with the User Data.
//================================================================================
/**
* @param userData Pointer to the User Data.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function cleans and deallocates memory associated with the User Data structure.
* \see  IpxCreateUserData<BR/>IpxLoadUserData<BR/>IpxSaveUserData
*/
static bool IpxReleaseUserData(IpxUserData** userData)
{
	if (!userData || !(*userData))
		return true;
	if (!(*userData)->createdIpx)
		return false;
	free(*userData);
	*userData = 0;
	return true;
}

/// Save User Data to stream.
//================================================================================
/**
* @param userData Pointer to the User Data.
* @param file Pointer to data stream.
* \return 
* If the function succeeds, the return boolean value is 'true'.
* If the function fails, the return boolean value is 'false'.
* \note 
* This function copies the User Data information into the data stream file.
* \see  IpxLoadUserData<BR/>IpxCreateUserData<BR/>IpxReleaseUserData
*/
static bool IpxSaveUserData(IpxUserData* userData, FILE* file)
{
	if (!file)
		return false;
	if(!userData)
		return true; 
	if(!userData->data)
		userData->size = 0;
	if (1 != ::fwrite(userData, sizeof(IpxUserData), 1, file))
	{
		return false;
	}
	if (userData->size > 0)
	{
		if (1 != ::fwrite(userData->data, userData->size, 1, file))
			return false;
	}
	return true;
}

/// Load User Data from stream.
//================================================================================
/**
* @param userData Pointer to new User Data.
* @param file Pointer to data stream.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function allocates memory for the new User Data and copies the stream data information from the file to the IpxUserData structure.
* \see  IpxSaveUserData<BR/>IpxCreateUserData<BR/>IpxReleaseUserData
*/
static bool IpxLoadUserData(IpxUserData** userData, FILE* file)
{
	if (!userData || !file)
		return false;
	IpxUserData tmpUData;
	if (1 != ::fread(&tmpUData, sizeof(IpxUserData), 1, file))
	{
		return false;
	}

	if (!IpxCreateUserData(userData, tmpUData.type, tmpUData.id, tmpUData.size))
		return false;
	if (tmpUData.size > 0 && (1 != ::fread((*userData)->data, tmpUData.size, 1, file)))
	{
		IpxReleaseUserData(userData);
		return false;
	}
	return true;
}

///@}

//////////////////////////////////////////////////////////////////////
#endif // !defined(IpxUserData_H__INCLUDED_)
