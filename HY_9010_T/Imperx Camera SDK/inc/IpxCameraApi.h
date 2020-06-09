////////////////////////////////////////////////////////////////////////////////
// Imperx Camera SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// File: IpxCameraApi.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Imperx Camera API Function and Constant Declarations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Created:	14-OCT-2014
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2013-2016 Imperx Inc. All rights reserved. 
////////////////////////////////////////////////////////////////////////////////

#ifndef IPX_CAMERA_API_H
#define IPX_CAMERA_API_H

// TODO: generic definitions - should be defined in one common header file
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
//////////////////////////////////////////////////////////////////////////

#ifndef IPXCAM_EXTERN_C
    #ifdef __cplusplus
        #define IPXCAM_EXTERN_C extern "C"
    #else
        #define IPXCAM_EXTERN_C
    #endif
#endif // IPXCAM_EXTERN_C

#ifndef IPXCAM_CALL
    #ifdef _WIN32
        #define IPXCAM_CALL __stdcall
    #else
        #define IPXCAM_CALL
    #endif
#endif // IPXCAM_CALL

// check if static library is wanted
#ifdef IPXCAM_STATIC
    // define the static library symbol
    #define IPX_CAMERA_API
    #define IPX_CAMERA_LOCAL
#else
    #define IPX_CAMERA_LOCAL LIB_HELPER_DLL_LOCAL
    #ifdef IPX_CAMERA_API_EXPORTS
        // define the export symbol
        #define IPX_CAMERA_API LIB_HELPER_DLL_EXPORT

        #if defined _WIN32 && !defined _M_AMD64
            // For 32b DLL only. If method name is mangled, add unmangled name below.
            //#pragma comment(linker, "/EXPORT:IpxCam_GetSystem=_IpxCam_GetSystem()@0")
        #endif
    #else // IPX_CAMERA_API_EXPORTS
        // define the import symbol
        #define IPX_CAMERA_API LIB_HELPER_DLL_IMPORT
    #endif // IPX_CAMERA_API_EXPORTS
#endif // IPXCAM_STATIC

// Note For MSVC < 2010
// The C99 standard includes definitions of several new integer types to 
// enhance the portability of programs[2]. The already available basic 
// integer types were deemed insufficient, because their actual sizes are 
// implementation defined and may vary across different systems. The new 
// types are especially useful in embedded environments where hardware supports
// usually only several types and that support varies from system to system. 

// Unfortunately Microsoft Visual C++ before 2010 did not support C99 statndard
// In this case you can download this file from
// https://msinttypes.googlecode.com/svn/trunk/stdint.h

#include <stddef.h>
#include <stdint.h>

#include "IpxCameraErr.h"

#ifdef __cplusplus

namespace GenApi
{
    struct INodeMap;
    struct INode;
}

typedef struct _IpxImage IpxImage;

/*! \namespace IpxGenParam
    \brief A namespace providing scope to the GenICam features using enum, classes, etc.

    A more detailed class description.
*/
namespace IpxGenParam
{
	//! An enum of Parameter Types. Parameter Node Types that can access the node object's programming interface.
	enum ParamType : uint32_t
	{
		ParamUnknown,   /*!< Enum value ParamUnknown. Unknown Parameter.  */  
		ParamInt, 	    /*!< Enum value ParamInt will access node object's of IInteger interface.*/  
		ParamFloat,     /*!< Enum value ParamFloat will access node object's of IFloat interface.*/  
		ParamString,    /*!< Enum value ParamString will access node object's of IString interface.*/  
		ParamEnum,      /*!< Enum value ParamEnum will access node object's of IEnumeration interface.*/  
		ParamEnumEntry, /*!< Enum value ParamEnumEntry.*/  
		ParamBoolean,   /*!< Enum value ParamBoolean will access node object's of IBoolean interface.*/  
		ParamCommand,   /*!< Enum value ParamCommand will access node object's of ICommand interface.*/  
		ParamCategory   /*!< Enum value ParamCategory will access node object's of ICategory interface.*/  
	};

	//! An enum of NameSpace. Parameter Node Namespace. 
	enum NameSpace : uint32_t
	{
		NameSpaceStandard = 0, /*!< Enum value NameSpaceStandard. Identifies the standard namespace used in the file.  */
		NameSpaceCustom,       /*!< Enum value NameSpaceCustom. Identifies the custom namespace used in the file.  */
		NameSpaceUndefined=999 /*!< Enum value NameSpaceUndefined. Unknown namespace.  */
	};
	
    //! An enum of Visibility. This element defines the type of user that has access to the feature.
	enum Visibility	: uint32_t
	{
		VisBeginner = 0,   /*!< Enum value VisBeginner. User has visibility to all the basic features of the device.*/
		VisExpert,         /*!< Enum value VisExpert. User has visibility to more advance features of the device.*/
		VisGuru,           /*!< Enum value VisGuru. User has visibility to even more advance features that if set improperly can cause device to be in an improper state.*/
		VisInvisible,      /*!< Enum value VisInvisible. Not visible.  */  
		VisUndefined = 99, /*!< Enum value VisUndefined. Object unknown.  */
	};

	class Param;

	/**  
	*  A Class for ParamEventSink. 
	\brief An Event Sink class designed to receive incoming events from Parameter Node Updates
	*/
	class ParamEventSink
	{
	public:

        //! A destructor of the ParamEventSink class.
        /*!
        \brief Destructor. Destroys the ParamEventSink and all its descendants.
        */
        virtual ~ParamEventSink(){}

		//! Update Parameter Node 
		/*!
		\param[in] param The pointer to the Param class node 
		return Void.
		*/
        virtual void OnParameterUpdate( Param *param ) = 0;
	};

	/**  
	*  A Class for Param Properties.
	\brief A Class for accessing the GenICam feature node parameters of the Camera Descriptor File
	*/
	class Param
	{
	public:

        //! A destructor of the Param class.
        /*!
        \brief Destructor. Destroys the Param and all its descendants.
        */
        virtual ~Param(){}

        //! This method returns the Parameter Node Type
		/*!
        \return If the method succeeds, it will return the parameter type. Otherwise, it will return a nullptr.
		*/
		virtual ParamType GetType() = 0;

        //! This method returns the parameter node name.
		/*!
        \return If the method succeeds, it will return the parameter node name. Otherwise, it will return a nullptr.
		*/
        virtual const char* GetName() = 0;

        //! This method returns a short description of the parameter node.
		/*!
        \return If the method succeeds, it will return a short description of the parameter node. Otherwise, it will return a nullptr.
		*/
        virtual const char* GetToolTip() = 0;

        //! This method returns a long description of the parameter node.
		/*!
        \return If the method succeeds, it will return a long description of the parameter node. Otherwise, it will return a nullptr.
		*/
        virtual const char* GetDescription() = 0;

        //! This method returns the name of the display.
		/*!
        \return If the method succeeds, it will return the name of the display. Otherwise, it will return a nullptr.
		*/
        virtual const char* GetDisplayName() = 0;

        //! This method returns the visibility of the node.
		/*!
		\return It will return the visibility setting of the parameter node. It will be either Basic, Expert, or Guru. 
		*/
		virtual Visibility GetVisibility()  = 0;

        //! This method checks if the parameter node is cached.
		/*!
        \return True if the value is cached. Otherwise, the value is not cached.
		*/
		virtual bool IsValueCached() = 0;

        //! This method checks if parameter node is available.
		/*!
        \return True if the parameter node is available. Otherwise, it is not available.
		*/
		virtual bool IsAvailable() = 0;

        //! This method checks if parameter node is writeable.
		/*!
        \return True if the parameter node is writeable. Otherwise, it is not writeable.
		*/
		virtual bool IsWritable() = 0;

        //! This method checks if the parameter node is readable.
		/*!
        \return True if the parameter node is readable. Otherwise, it is not readable.
        */
		virtual bool IsReadable() = 0;

        //! This method checks if the parameter node is streamable.
		/*!
        \return True if the parameter node is streamable. Otherwise, it is not streamable.
        */
		virtual bool IsStreamable() = 0;

        //! This method checks if the node is visible.
		/*!
		\param[in] vis Visibility of the parameter node
        \return True if the parameter node is visible. Otherwise, it is not visible.
		*/
        virtual bool IsVisible( Visibility vis ) = 0;

        //! This method registers the event.
		/*!
		\param[out] aEventSink returns Parameter Event Sink
		\return Returns the Error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully registers event sink
		*/
        virtual IpxCamErr RegisterEventSink( ParamEventSink *aEventSink ) = 0;

        //! This method un-registers the event.
		/*!
		\param[out] aEventSink returns Paramet Event Sink
		\return returns the Error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully unregisters event sink
		*/
		virtual IpxCamErr UnregisterEventSink( ParamEventSink *aEventSink ) = 0;

        //! This method returns the callback of the node registered.
		/*!
        \return If the method succeeds, it will return the pointer to the node of the callback that is registered. Otherwise, it will return a value of nullptr.
		*/		
		virtual GenApi::INode* GetNode() = 0;
	};
	
	/**  
	*  A class containing methods for GenICam Category Properties. 
	\brief A class containing methods that the user can access the categories GenICam features.  It will access the node object's of an ICategory interface. 
	Each feature of a device will be placed in a <b>Category</b>.  The Category feature is used to present the user with a group of features for the named 
	category.  
		
	For example, the mapping below will illustrate the ICategory interfaces categories such as DeviceControl and EventControl. 
	
	\image html Category.png
	*/
	class Category : virtual public Param
	{
	public:

        //! This method returns the node object Category type.
		/*!
		\return Returns the node object Category type 
		*/
		virtual ParamType GetType() { return ParamCategory; }

        //! This method returns the number of categories in the array.
        /*!
        \return Returns the number of categories in the array
        */
		virtual uint32_t GetCount() = 0;
		
        //! This method returns the Parameter by Index.
		/*!
		\param[in] idx index
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to the parameter specified by the node name
		- \c IpxCamErr::IPX_CAM_ERR_INVALID_INDEX - an invalid index for node
		\return returns the pointer to the parameter object
		*/
        virtual Param* GetParamByIndex( uint32_t idx, IpxCamErr *err ) = 0;
	};

	/**  
	*  A class containing methods for GenICam Boolean Properties. 
	\brief A class containing methods that map the integer element value of a GenICam IBoolean interface feature to true or false.
	
	For example, the mapping below will illustrate the IBoolean interfaces of a <b>LUTEnable</b> feature. 
	
	\image html Boolean.png
	*/
	class Boolean : virtual public Param
	{
    public:

        //! This method returns the node object Boolean type.
		/*!
		\return Returns the node object Boolean type  
		*/
		virtual ParamType GetType() { return ParamBoolean; }

        //! This method can be used to set the node value to true or false.
        /*!
        \param[in] val The node value to set such as true or false
		\return Returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Boolean value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range 
        */
        virtual IpxCamErr SetValue( bool val ) = 0;

        //! This method returns the node value. It can return a true or false value.
        /*!
        \param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the value of the Boolean node
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
        \return The node value read.
	    */
        virtual bool GetValue( IpxCamErr *err = nullptr ) = 0;
	};

	/**  
	* A class containing methods for GenICam Command Properties.   
	\brief A Class for GenICam Command properties contains methods that lets the user submit a command for execution as well as poll the status.
	
	For example, the mapping below will illustrate the ICommand interface for AcquisitionStart. This feature starts the
	Acquisition of the device.
	
	\image html Command.png
	*/
	class Command : virtual public Param
	{
	public:

        //! This method returns the node object Command type.
		/*!
		\return returns the node object Command type 
		*/
        virtual ParamType GetType(){ return ParamCommand; }

        //! This method executes the command.
		/*!
		\return the error code 
		*/
		virtual IpxCamErr Execute() = 0;

        //! This method queries whether the command is executed.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully determined that state of execute command
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
        - \c IpxCamErr::IPX_CAM_GENICAM_TREE_ERROR - Unable to access tree
		\return If set to TRUE, the Execute command has finished. Otherwise, it returns FALSE.
		*/
        virtual bool IsDone( IpxCamErr *err = nullptr ) = 0;
	};

	/**  
	*  A GenICam EnumEntry class. 
	\brief A Class for GenICam Enum Entries has methods to access the Enumeration node Enum Entry of the GenICam.
	
	For example, the mapping below illustrates entries of the IEnumeration interface for the AOI2_Select feature. 
	This feature can select the mode of operation for Slave AOI #2.  The enumeration entries coudl be "Off", "Include", and
	"Exclude".
	
	\image html EnumEntry.png
	*/
	class EnumEntry : virtual public Param
	{
	public:

        //! This method returns the node object EnumEntry type.
		/*!
        \return If the method succeeds, it returns the ParamType object type of the EnumEntry.
		*/
		virtual ParamType GetType() { return ParamEnumEntry; }

        //! This method gets the EnumEntry numerical value.
        /*!
        \param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully indicates EnumEntry value was retrieved
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
        \return If the method succeeds, it returns the value read of the EnumEntry.
        */
        virtual int64_t GetValue( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the EnumEntry String value.
        /*!
        \param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully indicates EnumEntry string value was retrieved
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
        \return If the method succeeds, it returns the String value read of the EnumEntry.
        */
        virtual const char* GetValueStr( IpxCamErr *err = nullptr ) = 0;
	};

	/**  
	*  A Enum class. 
	\brief Interface Class for Enumeration properties. 
	
	This class is used to map the name of the Enumeration interface type for the drop down box. 

	For example, the mapping below illustrates the enumeration "WhiteBalanceMode". 
	
	\image html Enum.png
	*/
	class Enum : virtual public Param
	{
	public:

        //! This method returns the node object Enum type.
		/*!
        \return If the method succeeds, it will returns the Enum parameter type.
		*/
        virtual ParamType GetType(){ return ParamEnum; }

        //! This method gets the number of entry nodes.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the number of EnumEntries
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		\return Returns the number of enum entry nodes. 
		*/
        virtual size_t GetEnumEntriesCount( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the Enum Entry node by the Index number.
		/*!
		\param[in] aIndex Index number 
        \return If the method succeeds, it returns the Enum Entry node.
		*/
        virtual EnumEntry* GetEnumEntryByIndex( size_t aIndex ) = 0;

        //! This method gets the Enum Entry node by Name.
		/*!
		\param[in] name Entry Name 
        \return If the method succeeds, it returns the Enum Entry node.
		*/
        virtual EnumEntry* GetEnumEntryByName( const char *name ) = 0;

        //! This method gets the Enum Entry node by Value.
		/*!
		\param[in] val Entry Value 
        \return If the method succeeds, it returns the Enum Entry node.
		*/
        virtual EnumEntry* GetEnumEntryByValue( int64_t val ) = 0;

        //! This method gets the Enum Entry node value.
		/*!
		\param[in] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Enum Entry node value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
        \return If the method succeeds, it returns the Enum Entry node value.
		*/
        virtual int64_t GetValue( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the Enum Entry node String.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully get the Enum Entry node string 
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
        \return If the method succeeds, it returns the Enum Entry node string.
		*/
        virtual const char* GetValueStr( IpxCamErr *err = nullptr ) = 0;

        //! This method sets the Enum Entry node value.
		/*!
		\param[in] val Enum Entry node value 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Enum value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		*/
		virtual IpxCamErr SetValue( int64_t val ) = 0;

        //! This method sets the Enum Entry node String.
		/*!
		\param[in] val Enum Entry node String 
		\return returns the error code 
		*/
        virtual IpxCamErr SetValueStr( const char *val ) = 0;
	};

	/**  
	*  A GenICam Float class. 
	\brief Interface Class for GenICam Float properties. 
	
	For example, mapping a slider with value, min, and max plus a physical unit 
	*/
	class Float : virtual public Param
	{
	public:

        //! This method returns the node object Float type.
		/*!
		\return returns the parameter type 
		*/
        virtual ParamType GetType(){ return ParamFloat; }

        //! This method sets the node value.
        /*!
        \param[in]  val The value to set
        \return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Float value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range 
		*/
        virtual IpxCamErr SetValue( double val ) = 0;

        //! This method gets the Float node value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully get the Float value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Gets the Float node value 
		*/
        virtual double GetValue( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the minimum value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Minimum float value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Returns the minimum 
		*/
        virtual double GetMin( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the maximum value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Maximum float value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Returns the maximum 
		*/
        virtual double GetMax( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the Unit.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the units
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Returns the unit 
		*/
        virtual const char* GetUnit( IpxCamErr *err = nullptr )  = 0;
	};

	/**  
	*  A GenICam Int class. 
	\brief Interface Class for GenICam Int properties. 
	
	For example, the mapping below illustrates the ability to set the "Width" value. 
	
	\image html Enum.png
	*/
	class Int : virtual public Param
	{
	public:

        //! This method returns the node object Int type.
		/*!
		\return returns the parameter type 
		*/
        virtual ParamType GetType(){ return ParamInt; }

        //! This method sets the Int node value.
		/*!
		\param[in] val Int node value 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Int value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range  
		*/
        virtual IpxCamErr SetValue( int64_t val ) = 0;

        //! This method gets the Int node value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Int value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return returns the Int node value 
		*/
        virtual int64_t GetValue( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the minimum value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Minimum int value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		\return Returns the minimum 
		*/
        virtual int64_t GetMin( IpxCamErr *err = nullptr )  = 0;

        //! This method gets the maximum value.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Maximum int value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Returns the maximum 
		*/
        virtual int64_t GetMax( IpxCamErr *err = nullptr )  = 0;

        //! This method gets the Increment value.
		/*!
		\param[out] err returns error code :
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the increment value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Returns the increment 
		*/
        virtual int64_t GetIncrement( IpxCamErr *err = nullptr ) = 0;
	};

	/**  
	*  A GenICam String class. 
	\brief Interface Class for GenICam String properties. 
	
	For example, mapping to an edit box showing a string
	*/
	class String : virtual public Param
	{
	public:

        //! This method returns the node object Command type.
		/*!
		\return The parameter type 
		*/
        virtual ParamType GetType(){ return ParamString; }

        //! This method gets the Maximum Length of the string.
		/*!
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the maximum length value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return gets the maximum length of the string
		*/
        virtual size_t GetMaxLength( IpxCamErr *err = nullptr ) = 0;

        //! This method gets the value of the string node.
		/*!
		\param[out] len return the length of the string
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the string 
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return returns the value
		*/
        virtual const char* GetValue( size_t *len = nullptr, IpxCamErr *err = nullptr ) = 0;

        //! This method sets the value of the string node.
		/*!
		\param[in] val Set the value of the string node
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the string 
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		*/
        virtual IpxCamErr SetValue( const char *val ) = 0;
	};

	/**  
	*  An Array class. 
	\brief This class contains functions that can access each node from the camera descriptor XML file by type and name.
	*/
	class Array
	{
	public:

        //! A destructor of the Array class.
        /*!
        \brief Destructor. Destroys the Array and all its descendants.
        */
        virtual ~Array(){}

        //! This method gets the pointer to the Param class for the specified node name from the node map declared in the camera descriptor XML file
		/*!
		\param[in] name Unique name of a node in node map.  
		\param[out] err Returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Param class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - specified node name not found in camera descriptor XML file
        \return If the method succeeds, it returns the pointer to the Param class for the specific node name. Otherwise, it returns a nullptr.
		*/
        virtual Param* GetParam( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the Boolean class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name A unique name of Boolean type node in the camera descriptor XML file.  
		\param[out] err Returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Boolean class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file
        \return If the method succeeds, it returns the pointer to the Boolean class for the specific node name. Otherwise, it returns a nullptr.
		*/
        virtual Boolean* GetBoolean( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the Command class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name Unique name of Command type node in the camera descriptor XML file. 
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Command class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file
        \return If method succeeds, it returns the pointer to the Command class for the specific node name. Otherwise, it returns a nullptr.
		*/
        virtual Command* GetCommand( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the Enum class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name Unique name of Enumeration type node in the camera descriptor XML file.
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Enum class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file 
        \return If the method succeeds, it returns the pointer to the Enum parameter class for the specific node name. Otherwise, it returns a nullptr.
		*/
        virtual Enum* GetEnum( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the Float class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name Unique name of Float type node in the camera descriptor XML file. 
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Float class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file 
        \return If the method succeeds, it returns the pointer to the Float parameter class for the specific node name
		*/
        virtual Float* GetFloat( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the Int class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name Unique name of Integer type node in the camera descriptor XML file.
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Int class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file 
        \return If the method succeeds, it returns the pointer to the Int class for the specific node name
		*/
        virtual Int* GetInt( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the String class for the specified node name of the camera descriptor XML file
		/*!
		\param[in] name Unique name of String type node in the camera descriptor XML file. 
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to String class of the specified node name
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified node name not found in camera descriptor XML file  
        \return If the method succeeds, it returns the pointer to the String class for the specific node name
		*/
        virtual String* GetString( const char *name, IpxCamErr *err ) = 0;

        //! This method gets the pointer to the root node class. The Root node is considered a special node. It has no parent node. In the topology graph, it is the top node which connects to at least one child node. The child node may connect to the device node that provides the connection to the transport layer.
		/*!
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Category class
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - specified Root node name not found in camera descriptor XML file  
		\return returns the pointer to the Category (root node) class
		*/
        virtual Category* GetRootCategory( IpxCamErr *err ) = 0;

        //! This method gets the pointer to the NodeMap interface. The NodeMap interface will provide methods to retrieves all nodes in the node map.
		/*! 
		\param[out] err returns an error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to GenApi::INodeMap class
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR - the node map does not exist  
		\return nodemap returns the pointer to the NodeMap interface
		*/
        virtual GenApi::INodeMap* GetNodeMap( IpxCamErr *err ) = 0;

        //! This method gets the number of nodes.
		/*! 
        \return The number of nodes. This number should be greater than 0.
		*/
		virtual uint32_t GetCount() = 0;

        //! This method gets the parameter by index.
		/*! 
		\param[in] idx Index
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully returns pointer to Param class
		- \c IpxCamErr::IPX_CAM_ERR_INVALID_INDEX  - entered invalid index
		\return Returns param pointer to Parameter class of the specified node referenced by the index value 
		*/
        virtual Param* GetParamByIndex( uint32_t idx, IpxCamErr *err ) = 0;

        //! This method sets the Boolean value of the Boolean node.
		/*! 
		\param[in] name Unique name of Boolean node to set
		\param[in] aValue Boolean value to set 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully set the Boolean value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR  -  Unable to access genicam specified node
		*/
        virtual IpxCamErr SetBooleanValue( const char *name, bool aValue ) = 0;

        //! This method gets the Boolean value of the Boolean node.
		/*! 
		\param[in] name Unique name of Boolean node to get
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Boolean value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node 
		\return Returns the Boolean Value 
		*/
        virtual bool GetBooleanValue( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method sets the Enum node maps and the Enum interface to a name and index value. Each of the enum entries are represented by a name and index pair. This method sets the Enum value String of the corresponding node. The enum nodes map to a drop down box.
		/*! 
		\param[in] name Name of Enum entry node to set
		\param[in] val Enum node value set 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Enum Value string
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR  -  Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the const value entered is out of range
		*/
        virtual IpxCamErr SetEnumValueStr( const char *name, const char *val ) = 0;

        //! This method sets the Enum value of the enum node.
		/*! 
		\param[in] name Unique ame of Enum entry to set
		\param[in] val Enum entry value to set 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Enum value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range
		*/		
        virtual IpxCamErr SetEnumValue( const char *name, int64_t val ) = 0;

        //! This method gets the Enum value string of the current set Enum value entry.
		/*! 
		\param[in] name Unique name of Enum entry 
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Enum string value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		\return Get the Enum value String of the current set Enum Value Entry 
		*/				
        virtual const char* GetEnumValueStr( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method gets the Enum value of the Enum node.
		/*! 
		\param[in] name Unique name of Enum type node in the camera descriptor XML file. 
		\param[out] err returns error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Enum value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		\return Returns the Enum Value 
		*/				
        virtual int64_t GetEnumValue( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method sets the Float value of the Float node.
		/*! 
		\param[in] name Unique name of Float type node in the camera descriptor XML file. 
		\param[in] val Float value to set 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Float value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range 
		*/		
        virtual IpxCamErr SetFloatValue( const char *name, double val ) = 0;

        //! This method gets the Float value of the Float node.
		/*! 
		\param[in] name Unique name of Float type node in the camera descriptor XML file. 
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Float value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type  
		\return Returns the Float value  
		*/		
        virtual double GetFloatValue( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method sets the Integer value of the Integer node.
		/*! 
		\param[in] name Unique name of Integer type node in the camera descriptor XML file. 
		\param[in] val Integer value to set 
		\return returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the Integer value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range  
		*/		
        virtual IpxCamErr SetIntegerValue( const char *name, int64_t val ) = 0;

        //! This method gets the Integer value of the Integer node.
		/*! 
		\param[in] name Unique name of Integer type node in the camera descriptor XML file. 
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the Integer value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		\return Returns the Integer value 
		*/		
        virtual int64_t GetIntegerValue( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method sets the String value of the String node.
		/*! 
		\param[in] name Unique name of String type node in the camera descriptor XML file. 
		\param[in] val String value to set 
		\return Returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully sets the String value
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type
		- \c IpxCamErr::IPX_CAM_GENICAM_OUT_OF_RANGE - the value entered is out of range 
		*/		
        virtual IpxCamErr SetStringValue( const char *name, const char *val ) = 0;

        //! This method gets the String value of the String node.
		/*! 
		\param[in] name Unique name of String type node in the camera descriptor XML file. 
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully gets the String value
		- \c IpxCamErr::IPX_CAM_GENICAM_UNKNOWN_PARAM - unknown parameter
		- \c IpxCamErr::IPX_CAM_GENICAM_TYPE_ERROR  -  Unable to access genicam specified node type 
		\return Returns the String value 
		*/		
        virtual const char* GetStringValue( const char *name, IpxCamErr *err = nullptr ) = 0;

        //! This method executes/submits the command.
		/*! 
		\param[in] name Unique name of Command type node in the camera descriptor XML file.  
		\return Returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully determines state of executed command.
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		*/		
        virtual IpxCamErr ExecuteCommand( const char *name ) = 0;

        //! This method polls the corresponding executed command to see if the executed command is done or not.
		/*! 
		\param[in] name Unique name of Command type node in the camera descriptor XML file. 
		\param[out] err returns the error code:
		- \c IpxCamErr::IPX_CAM_ERR_OK  -  Successfully determines state of executed command.
		- \c IpxCamErr::IPX_CAM_GENICAM_ACCESS_ERROR - Unable to access genicam specified node  
		\return Returns true if the Execute command has finished. Otherwise, returns false. 
		*/		
        virtual bool IsCommandDone( const char *name, IpxCamErr *err = nullptr ) = 0;
	};
} // end of namespace IpxGenParam


/*! \namespace IpxCam
    \brief A namespace providing scope to the GenICam GenTL transport layer interface to acquire images with an Imperx Camera using enum, classes, etc.

    A more detailed class description.
*/
namespace IpxCam
{
	/**  
	\brief The List class is used to list the specified template type objects. 
	\details The supported template type objects are <b>Interface</b>,<b>Device</b>,<b>DeviceInfo</b>,<b>Stream</b>, and <b>Buffer</b>.  
		
	They can be declared as follows:

	<table style="background-color: #87ACBA"  border="1">
	 <tr><th>IpxCam::List<Interface> *interfaceList</th><th>This class represents the list of Interface objects.</th></tr>
	 <tr><th>IpxCam::List<Device>  *deviceList</th><th>This class represents the list of Device objects.</th></tr>
	 <tr><th>IpxCam::List<DeviceInfo> *deviceInfoList</th><th>This class represents the list of DeviceInfo objects.</th></tr>
	 <tr><th>IpxCam::List<Stream> *streamList</th><th>This class represents the list of Data Stream objects</th></tr>
	 <tr><th>IpxCam::List<Buffer> *bufferList</th><th>This class represents the list of Buffer objects</th></tr>
	 </table>

	 Alternatively, you can also use the declared typedef(aliases for specific objects) provided in the IpxCam namespace as shown below:

	\include snippets/ListTypedefs.cpp

	They can be declared as follows:

	<table style="background-color: #87ACBA"  border="1">
	 <tr><th>InterfaceList *interfaceList</th><th>This class represents the list of Interface objects.</th></tr>
	 <tr><th>DeviceInfoList *deviceInfoList</th><th>This class represents the list of DeviceInfo objects.</th></tr>
	 </table>

	This class can be used to search through the list of objects discovered.
	
	<b>Example using DeviceInfoList</b>

    In this example, you will see how to use the DeviceInfoList. An example is shown below that demonstrates on how to use the list class methods. The <b>deviceInfoList->GetCount()</b> method
	is used retrieve the number of devices connected. We confirm that at least one device is available.  Next, the for loop will loop 
	from the first device information listed using the <b>deviceInfoList->GetFirst()</b> funtion to the end of the list. During each 
	iteration the <b>deviceInfoList->GetNext()</b> will increment to the next deviceInfo available.   In the example, you will notice 
	that we search for a specified device model name.  Once, the specified device is found, we will release the <b>deviceInfoList->Release()</b> 
    and the create the specified device using the <b>IpxCam::IpxCam_CreateDevice()</b> method.

	\include snippets/DeviceInfoList.cpp

	<b> Example using InterfaceList</b>

	In this example, you will see how to use the InteraceList. You will retrieve the interfaces available for this system. Next, the for
    loop will loop from the first interface available using the <b>list->GetFirst()</b> method to the end of the list. During each
	ineration the <b>list->GetNext()</b> will increment to the next interface available.

	\include snippets/InterfaceListEx.cpp
	*/
	template<class _T>
	class List
	{
	public:

        //! A destructor of the List class.
        /*!
        \brief Destructor. Destroys the List and all its descendants.
        */
        virtual ~List(){}

		/** Element Type*/
		typedef _T elem_type;

		/*! \fn "virtual void" Release()
        \brief This method releases the instance of the list of the specified object.
		\return Void.
		*/
		virtual void Release() = 0;

		//! This functions gets the number of items in the specified list object 
		/*! 
		\return Returns the number of items in the specified list object.  
		*/		
		virtual size_t GetCount() = 0;

        //! This method retrieves the first element in the specified list object
		/*! 
		\return Returns the first element in the specified list object.
		*/		
		virtual elem_type* GetFirst() = 0;

        //! This method retrieves the next element in the specified list object
		/*! 
		\return Returns the next element in the specified list object.  
		*/		
		virtual elem_type* GetNext() = 0;
	};
	
	//! An enum of Interface Types. Interface Node Types representing physical interface in the system. 
	enum InterfaceType : uint32_t
	{
        USB3HostController = 1, /*!< Enum value USB3HostController. */
        NetworkAdapter     = 2, /*!< Enum value NetworkAdapter. */
        CameraLinkGrabber  = 3, /*!< Enum value CameraLinkGrabber. */
        CoaxPressGrabber   = 4, /*!< Enum value CoaxPressGrabber. */
        HdSdiGrabber       = 5, /*!< Enum value HdSdiGrabber. */
        AllInterfaces      = 0xff, /*!< Enum value AllInterfaces. */
	};

	//! An enum of Flush Operations. Flush Operations Types. 
	enum FlushOperation : uint32_t
	{
		Flush_OutputDiscard   = 1, /*!< Enum value Flush_OutputDiscard.  Discards all buffers in the output queue and if necessary remove the entries from the event data queue. */
		Flush_AllToInput      = 2, /*!< Enum value Flush_AllToInput.  Puts all buffers in the input pool. Even those in the output queue and discard entries in the event data queue. */
		Flush_UnqueuedToInput = 3, /*!< Enum value Flush_UnqueuedToInput. Puts all buffers that are not in the input pool or the output queue in the input pool. */
		Flush_AllDiscard      = 4  /*!< Enum value Flush_AllDiscard. Discards all buffers in the input pool and output queue. */
	};

	class Interface;
	class DeviceInfo;
	class Device;
	class Stream;
	class Buffer;
	
	/** Interface List
	*/
	typedef List<Interface>    InterfaceList;

    /** Device Info List
	*/
	typedef List<DeviceInfo>   DeviceInfoList;

    /** Device List
	*/
	typedef List<Device>       DeviceList;

    /** EventCallback
	param[in] eventData pointer to event Data
	param[in] eventSize event Size
	param[in] pPrivate  pointer to the context Data
	*/
    typedef void IPXCAM_CALL EventCallback(const void *eventData, size_t eventSize, void *pPrivate);

	/**  
    \brief The System class represents an abstraction of the system module of the GenTL module hierarchy. The System class is the entry point to the GenTL Producer software driver.
	
	\details  This class provides member functions to enumerate and instantiate the available interfaces reachable. It also provides a method for the configuration of the device module. 
	This system module is the root of the GenTL Module hierarchy. 
	<b>IpxCam::System</b>  class has member functions to to find all the interfaces,display the user readable name and producer version of the GenTL system.
	The <b>IpxCam::System</b> class can return <b>IpxCam::InterfaceList</b>, <b>IpxCam::Interface</b>, and <b>IpxCam::Device</b> objects.

	The following is an example on how to use some of the public Member Functions.

	\include snippets/IpxStreamInfo.cpp 
	*/
	class System
	{
	public:

        //! A destructor of the System class.
        /*!
        \brief Destructor. Destroys the System and all its descendants.
        */
        virtual ~System(){}

		/*! \fn "virtual void" Release()
        \brief This method releases the instance of the system object.
		*
		\return Void.

        *The following shows an example on how to use the <b>Release</b> method to release the system object instantiated.
		*\include snippets/ReleaseExample.cpp 
		*/
		virtual void Release() = 0;

        //! This method returns all the interface list of the system object.
		/*! 
		\brief It lists all the available hardware interfaces with the transport layers technologies that are supported.  
		\param[in] type interface type
		\return returns the interface list
		
        The following is an example on how to use the <b>GetInterfaceList</b> method.
		\include snippets/InterfaceListEx.cpp 
		*/		
		virtual InterfaceList* GetInterfaceList( InterfaceType type = AllInterfaces ) = 0;
		
        //! This method returns the interface by unique string identifier of the system object.
		/*!
		\brief Get interface specified by interface identifier. 
		\param[in] ifaceId Interface identifier
		\return returns the Interface or nullptr if no such interface is found

		For example, the const char *ifaceId interface identification name could be as shown below:

		\include snippets/GetInterfaceById.cpp
		
        This method will retrieve the available interface list of the system.
		*/
        virtual Interface* GetInterfaceById( const char *ifaceId ) = 0;

        //! This method displays the name of the GenTL Producer.
		/*! 
        \brief This method returns the User readable name of the GenTL Producer of the system object.
		\return returns the Display Name

        The following is an example on how to use the GetDisplayName method
		\include snippets/SystemDisplayName.cpp
		*/		
        virtual const char* GetDisplayName() = 0;

        //! This method returns the version of the GenTL Producer of the system object.
		/*! 
        \brief This method returns the GenTL Producer version.
		\return returns the Version  

        The following is an example on how to use the GetVersion method
		\include snippets/SystemVersion.cpp
		*/		
        virtual const char* GetVersion() = 0;

        //! This method configures and sets up the device using the information retrieved from the configuration file
		/*!
		\brief TODO make a description
        \param[in] fileName Configuration file to open
		\param[out] err returns the error code
		\return returns Device or nullptr if device cannot be instantiated
		*/
        virtual Device* CreateDeviceFromConfig( const char *fileName, IpxCamErr *err = nullptr ) = 0;

        //! This method adds 3rd party GenTL module to system
		/*!
		\brief TODO make a description
        \param[in] fileName path to GenTL CTI file to add
		\return returns the error code
		*/
        virtual IpxCamErr RegisterGenTLProvider( const char *fileName ) = 0;
	};

	/*! \fn "IPX_CAMERA_API System*" IpxCam_GetSystem()
    * \brief This method returns the System module. It is the entry point to the GenTL Module hierarchy.
	* \return returns the pointer to system... system is being created as soon as DLL is loaded
	*/
    IPXCAM_EXTERN_C IPX_CAMERA_API System* IpxCam_GetSystem();

	/**  
	*\brief The Interface class represents a interface module in the GenTL module hierarchy. 
	\details It represents an individual physical interface. For
	example, a network interface card (NIC), a frame grabber board, U3V, and GEV in the GenTL system. 
	This Interface class gets the enumeration and available devices on the physical interface in the system.
	*/
	class Interface
	{
    public:

        //! A destructor of the Interface class.
        /*!
        \brief Destructor. Destroys the Interface and all its descendants.
        */
        virtual ~Interface(){}

        //! This method gets the device info list of the available devices.
		/*! 
		\return returns the Device Info List of the available devices on this interface  

		For example,

		\include snippets/DeviceInfoList.cpp
		*/		
		virtual DeviceInfoList* GetDeviceInfoList() = 0;

        //! This method retrieves the first device info in the list.
		/*! 
		\return returns the first device on interface

		For example, 

		\code{.cpp}
		//Retrieve the first device available for the specified interface.
		lDeviceInfo = iface->GetFirstDeviceInfo();
		
		std::cout << "First Device Info ModelName" << lDeviceInfo->GetModel() << endl;
		
		\endcode
		*/		
		virtual DeviceInfo* GetFirstDeviceInfo() = 0;

        //! This method retrieves the device info for the specified device identifier.
		/*!
		\param[in] deviceId Device identifier
		\return returns the requested device identifier or null if no such device found

		*/
        virtual DeviceInfo* GetDeviceInfoById( const char *deviceId ) = 0;

        //!  This method re-enumerates the devices.
		/*! 
        \brief The ReEnumerateDevices method allows the user to re-enumerate the devices connected to the Interface and update the DeviceInfoList object returned by subsequent GetDeviceInfoList() method calls.
		\param[in] pChanged Change in Device
		\param[in] iTimeout Timeout allowed to search for available devices
		\return returns error code
		*/		
        virtual IpxCamErr ReEnumerateDevices( bool *pChanged, uint64_t iTimeout ) = 0;

        //! This method gets the description of the interface object.
		/*! 
		\brief Returns the user readable name of the interface
		\return returns the Description of the interface  
		*/		
        virtual const char* GetDescription() = 0;

        //! This method gets the type of interface object.
		/*! 
		\brief Returns the Transport Layer Technology that is supported
		\return returns Interface Type

		The interface type return can be the following:

		\include snippets\InterfaceType.cpp
		*/		
        virtual InterfaceType GetType() = 0;

        //! This method gets the identifier of the interface object.
		/*! 
		\brief Returns the interface identifier that could be used to instantinate the interface
		\return returns interface identifier
		*/		
        virtual const char* GetId() = 0;

        //! This method gets the version of Interface driver
		/*! 
		\brief Returns the version of interface driver
		\return returns version of interface driver
		*/		
        virtual const char* GetVersion() = 0;

        //! This method registers the Interface class method as a callback method to be called when a eventType occurs.
		/*! 
		\param[in] eventType Event Type
		TODO - define event ids here!!!!
        \param[out] eventCallback pointer to event CallBack method
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr RegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method unregisters the Interface class callback method for the eventType.
		/*! 
		\param[in] eventType Event Type
        \param[out] eventCallback pointer to event CallBack method
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr UnRegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method returns the parameter array used to control the Imperx Camera device.
		/*! 
		\param[out] err returns error code
		\return returns the parameter array used to control the Imperx Camera device  
		*/		
        virtual IpxGenParam::Array* GetParameters( IpxCamErr *err = nullptr ) = 0;

        //! This method creates the device based on the information specified in the configuration file.
		/*!
		\brief TODO make a description
        \param[in] fileName Configuration file to open
		\param[out] err returns error code
		\return returns Device or nullptr if device cannot be instantiated
		*/
        virtual Device *CreateDeviceFromConfig( const char *fileName, IpxCamErr *err = nullptr ) = 0;
	};

	/**  
	*\brief DeviceInfo class provides the information about the device.
	\details The device info class can be used to retrieve information about the device.
	*/
	class DeviceInfo
	{
    public:

        //! A destructor of the DeviceInfo class.
        /*!
        \brief Destructor. Destroys the DeviceInfo and all its descendants.
        */
        virtual ~DeviceInfo(){}

        //! This method returns the interface of the device object.
		/*! 
		\brief Returns the Interface of the device
		\return returns the Interface   
		*/		
        virtual Interface* GetInterface() = 0;

        //! This method returns the unique device identifier string for the Imperx Camera device object.
		/*!
		\return returns the unique device identifier string for the Imperx Camera device
		*/
        virtual const char* GetID() = 0;

        //! This method returns the device vendor name of the device object.
		/*! 
		\return returns the Imperx Camera device vendor name  
		*/		
        virtual const char* GetVendor() = 0;

        //! This method returns the device model name of the device object.
		/*! 
		\return returns the Device model name
		*/		
        virtual const char* GetModel() = 0;

        //! This method returns the display name of the device object.
		/*! 
		\return returns the user readable name of the device
		*/		
        virtual const char* GetDisplayName() = 0;

        //! This method returns the username of the device object.
		/*! 
		\return returns the user defined name of the device
		*/		
        virtual const char* GetUserDefinedName() = 0;

        //! This method returns the serial number of the device object.
		/*! 
		\return returns the serial number of the device 
		*/		
        virtual const char* GetSerialNumber() = 0;

        //! This method returns the device version of the device object.
		/*! 
		\return returns the Device version 
		*/		
        virtual const char* GetVersion() = 0;

        //! This method returns the information about the current device acecss status
		/*! 
		\brief Returns the information about the current device acecss status
		\return Status Access Code, can receive one of the following values:
		- \c <b>AccessStatusUnknown</b> [0]  - The current availability of the device is unknown.
		- \c <b>AccessStatusReadWrite</b> [1] - The device is available for Read/Write access
		- \c <b>AccessStatusReadOnly</b> [2] - The device is available for Read only access
		- \c <b>AccessStatusNoAccess</b> [3] - The device is not available either because it is already open or because it is not reachable.
		*/		
        virtual int32_t GetAccessStatus() = 0;

        //! This method returns the information about USB3 host controller where the camera is connected to.
		/*! 
		\brief Returns the information about USB3 host controller
        \return returns the pointer to string structure or nullptr for non-USB camera
		*/		
        virtual const char* GetUSB3HostInfo() = 0;

        static const uint32_t AccessStatusUnknown   = 0;
        static const uint32_t AccessStatusReadWrite = 1;
        static const uint32_t AccessStatusReadOnly  = 2;
        static const uint32_t AccessStatusNoAccess  = 3;
        static const uint32_t IpSubnetMismatch      = 1001;
	};

	//! An enum of Service File Types. Service File Types
	enum ServiceFileType : uint32_t
	{
        FileLUT = 1, /*!< Enum value FileLUT. */
        FileDPC = 2, /*!< Enum value FileDPC. */
        FileHPC = 3, /*!< Enum value FileHPC. */
        FileFFC = 4  /*!< Enum value FileFFC. */
    };

	//! An enum of Device Access.
	enum DeviceAccess : uint32_t
	{
		ReadOnly = 0, /*!< Enum value ReadOnly. */ 
		Control = 1,  /*!< Enum value Control. */ 
		Exclusive = 2 /*!< Enum value Exclusive. */ 
	};

    /*! \fn "IPX_CAMERA_API Device*" IpxCam_CreateDevice(DeviceInfo* info = nullptr, DeviceAccess access = Exclusive);
	* \param[in] info
	  \param[in] access
	  \param[out] err returns the error code
	  \return returns the pointer to Device
      \brief If info is nullptr, first default device created
	*/
    IPXCAM_EXTERN_C IPX_CAMERA_API Device* IpxCam_CreateDevice(DeviceInfo *info = nullptr, DeviceAccess access = Exclusive, IpxCamErr *err = nullptr);

    /**
    \brief The Device class represents the device module in the GenTL module hierarchy.
    \details This Device class provides methods to enable the communication and control of the Imperx device and enumerate/instantiate
    data stream objects. The methods can be used to enumerate and instantiate the Data Stream module objects. The device must
    must correspond to the interface transport layer technology. For example,the device could be an Imperx U3V Camera and the transport layer
    technology would be U3V. The Device class can be used to retrieve data information about the device by returning the pointer to the DeviceInfo class.
    It can be used to retrieve the pointer to the Stream object and upload/save the camera configurations.
    */
	class Device
	{
	public:

        enum UploadEventType : uint32_t
        {
            FlashSectorErase,/*!< Enum value FlashSectorErase. */
            FlashPageWrite,/*!< Enum value FlashPagewrite. */
            FlashPageRead/*!< Enum value FlashPageRead. */
        };

        struct UploadEventData
        {
            uint64_t addr_min;
            uint64_t addr_curr;
            uint64_t addr_max;
            uint64_t error;
        };

        typedef void IPXCAM_CALL UploadEventCallback(UploadEventType, UploadEventData);

        //! A destructor of the Device class.
        /*!
        \brief Destructor. Destroys the Device and all its descendants.
        */
        virtual ~Device(){}

		/*! \fn "virtual void" Release()
        \brief This method releases the instance of the device object.
        * This method releases the device object.
		*/
		virtual void Release() = 0;

        //! This method retrieves the number of data streams.
		/*! 
		\return retrieves the number of data streams   
		*/		
		virtual uint32_t GetNumStreams() = 0;

        //! This method retrieves the stream by index.
		/*! 
		\param[in] idx index value
		\return retrieves the pointer to the stream by index   
		*/		
        virtual Stream* GetStreamByIndex( uint32_t idx = 0 ) = 0;

        //! This method gets the stream by id
		/*! 
		\param[in] id Id 
		\return retrieves the pointer to the stream by id   
		*/		
        virtual Stream* GetStreamById( const char *id ) = 0;

        //! This method returns a pointer to the device info object.
		/*! 
		\return returns the pointer to the DeviceInfo class   
		*/		
		virtual DeviceInfo* GetInfo() = 0;

        //! This method reads a number of bytes from a given address.
		/*! 
		\param[in] addr Byte address to read from
		\param[out] data pointer to a user allocated byte data buffer
		\param[out] len  size of the amount of bytes to read from the register map address
		\return returns ErrorCode 
		*/		
        virtual IpxCamErr ReadMem( uint64_t addr, void *data, size_t len ) = 0;

        //! This method writes a number of bytes at a given address.
		/*! 
		\param[in] addr Byte address to read from
		\param[out] data pointer to a user allocated byte data buffer
		\param[out] len  size of the amount of bytes to read from the register map address
		\param[out] written size of bytes written
		\return returns ErrorCode  
		*/		
        virtual IpxCamErr WriteMem( uint64_t addr, const void *data, size_t len, size_t *written ) = 0;

        //!This method uploads the service type file.
		/*! 
		\param[in] type ServiceFile Type
        \param[in] fileName Name of the ServiceFile name
		\param[in] flags flag
		\param[out] pCallback pointer to the event Callback
		\return returns ErrorCode   
		*/	
        virtual IpxCamErr UploadFile( ServiceFileType type, const char *fileName, uint64_t flags = 0, UploadEventCallback *pCallback = nullptr ) = 0;

        //! This method registers the Device class method as a callback method to be called when a eventType occurs.
		/*! 
		\param[in] eventType Event Type, can receive one of the following values:
		- \c <b>GenICamEvent</b> [1002] - this event occurs, if GenICam event was triggered by the camera
		- \c <b>CameraConnected</b> [1003] - this event occurs, when camera was connected to computer
		- \c <b>CameraDisconnected</b> [1004] - this event occurs, when camera was disconnected from computer
		\param[out] eventCallback event CallBack
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr RegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method un-registers the Interface class callback method for the eventType.
		/*! 
		\param[in] eventType Event Type, can receive one of the following values:
		- \c <b>GenICamEvent</b> [1002] - this event occurs, if GenICam event was triggered by the camera
		- \c <b>CameraConnected</b> [1003] - this event occurs, when camera was connected to computer
		- \c <b>CameraDisconnected</b> [1004] - this event occurs, when camera was disconnected from computer
		\param[out] eventCallback event CallBack
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr UnRegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method returns the transport parameters of the device object.
		/*! 
		\param[out] err returns error code
		\return returns the Transport parameter array   
		*/		
        virtual IpxGenParam::Array* GetTransportParameters( IpxCamErr *err = nullptr ) = 0;

        //! This method returns the camera parameters of the device object.
		/*! 
		\param[out] err returns error code
		\return returns the Camera Parameters array   
		*/		
        virtual IpxGenParam::Array* GetCameraParameters( IpxCamErr *err = nullptr ) = 0;

        //! This method saves the camera configuration file.
		/*!
        \param[in] fileName Configuration file name
		\return returns Error code
		*/
        virtual IpxCamErr SaveConfiguration( const char *fileName ) = 0;

        //! This method loads the configuration from file.
		/*!
        \param[in] fileName Configuration file name
		\return returns Error code
		*/
        virtual IpxCamErr LoadConfiguration( const char *fileName ) = 0;

        static const uint32_t GenICamEvent       = 1002; //! This event occurs, if GenICam event was triggered by the camera
        static const uint32_t CameraConnected    = 1003; //! This event occurs, when camera was connected to computer
        static const uint32_t CameraDisconnected = 1004; //! This event occurs, when camera was disconnected from computer
	};

	/**  
	*  
	\brief The Stream class represents the data stream module in the GenTL module hierarchy.
	\details This data stream class provides buffer methods. This data stream class purpose is to 
	access the buffer data acquirement from the Acquisition engine.
	*/
	class Stream
	{
	public:

        //! A destructor of the Stream class.
        /*!
        \brief Destructor. Destroys the Stream and all its descendants.
        */
        virtual ~Stream(){}

		/*! \fn "virtual void" Release()
        \brief This method releases the instance of the stream object.
		\return Void.
        * This method is used to release the stream object.
		*/
		virtual void Release() = 0;

        //! This method is used to create the buffers of the data stream object.
		/*! 
		\brief Allocates the memory for a buffer and announces this buffer to the data stream
		\param[in]  iSize  Size of the buffer 
		\param[out] pPrivate returns pointer to private data which will be passed to the GenTL Consumer
		\param[out] err returns Error code   
		\return returns buffer module handle of the announced buffer
        */
        virtual IpxCam::Buffer* CreateBuffer( size_t iSize, void *pPrivate, IpxCamErr *err ) = 0;

        //! This method is used to set the buffers to allocate and announce to the data stream.
		/*! 
		\brief Sets buffers to allocate and announce to the data stream
		\param[in] pBuffer buffer
		\param[in] iSize size of Buffer
		\param[out] pPrivate pointer to pPrivate
		\param[out] err returns Error code   
		\returns phBuffer buffer
		*/		
        virtual IpxCam::Buffer* SetBuffer( void *pBuffer, size_t iSize, void *pPrivate, IpxCamErr *err ) = 0;

        //! This method revokes any announced buffers.
		/*! 
		\brief From the acquisition engine, it removes the specified announced buffer
		\param[in] hBuffer handle of buffer
		\return returns Error code   
		*/		
        virtual IpxCamErr RevokeBuffer( IpxCam::Buffer *hBuffer ) = 0;

        //! This method queues specified buffers.
		/*! 
        \brief During acquisition, this method queues the specified buffer.
		\param[in] hBuffer handle of buffer
		\return returns Error code   
		*/		
        virtual IpxCamErr QueueBuffer( IpxCam::Buffer *hBuffer ) = 0;

        //! This method retrieves the buffer object.
		/*! 
		\brief Retrieve the next event data entry from the event data queued
		\param[in] iTimeout timeout in ms
		\param[in] err  error code
		\return returns the pointer of the buffer    
		*/		
        virtual IpxCam::Buffer* GetBuffer( uint64_t iTimeout, IpxCamErr *err = nullptr ) = 0;

        //! This method cancels any previously registered buffer events that have been waiting to be performed.
		/*! 
		\brief Terminates the waiting operation on a previously registered Buffer Event
		\return returns Error code   
		*/		
        virtual IpxCamErr CancelBuffer() = 0;

        //! This method flushes the buffers of the data stream object.
		/*! 
		\brief Flushes the Flush Operation specified internal buffer pool or queue. Operations defined in FlushOperations enum.
		\param[out] operation FlushOperation
		\return returns Error code   
		*/		
		virtual IpxCamErr FlushBuffers( FlushOperation operation ) = 0;

        //! This method sends the start command to start the acquistion of image data.
		/*! 
		\brief Starts the Acquisition Engine
		\param[in] iNumFramesToAcquire number of Frames to Acquire
		\param[in] flags flags
		\return returns Error code   
		*/		
        virtual IpxCamErr StartAcquisition( uint64_t iNumFramesToAcquire = UINT64_MAX, uint32_t flags = 0 ) = 0;

        //! This method sends the stop command to stop the acquistion of the any more image data.
		/*! 
		\brief The acquisition on the remote device is stopped after finishing acquiring image data
		\param[in] flags flags
		\return returns Error code   
		*/		
        virtual IpxCamErr StopAcquisition( uint32_t flags = 0 ) = 0;

        //! This method allocations the buffer queue of the data stream object.
		/*! 
		\param[out] pPrivate private
		\param[in] iNum number of Buffers to allocate
		\return returns Error code   
		*/		
		virtual IpxCamErr AllocBufferQueue( void *pPrivate, size_t iNum ) = 0;

        //! This method releases the buffer queue of the data stream object.
		/*! 
		\brief Release the Buffer Queue
		\return returns Error code   
		*/		
        virtual IpxCamErr ReleaseBufferQueue() = 0;

		//! This functions get the buffer queue size of the data stream object.
		/*! 
		\return returns the Buffer Queue size   
		*/		
        virtual size_t GetBufferQueueSize() = 0;

        //! This method registers the data Stream class method as a callback method to be called when a eventType occurs.
		/*! 
		\param[in] eventType Event Type 
		TODO - define event type ids here!!!!
		\param[out] eventCallback event CallBack
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr RegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method un-registers the data Stream class callback method for the eventType
		/*! 
		\param[in] eventType Event Type
		\param[out] eventCallback event CallBack
		\param[out] pPrivate returns pointer to Private variable
		\return returns Error code
		*/		
        virtual IpxCamErr UnRegisterEvent( uint32_t eventType, IpxCam::EventCallback *eventCallback, void *pPrivate ) = 0;

        //! This method returns an array of parameters for the data stream object
		/*! 
		\param[out] err returns the error code
		\return returns the data stream parameters array   
		*/		
        virtual IpxGenParam::Array* GetParameters( IpxCamErr *err = nullptr ) = 0;
	
        //! This method returns the number of delivered buffers since the start of the last acquisition.
		/*! 
		\return returns the number of delivered buffers since the start of the last acquisition   
		*/		
		virtual uint64_t GetNumDelivered() = 0;

        //! This method returns the number of lost frames due to the queue being underrun.
		/*! 
		\return returns the number of lost frames due to queue underrun   
		*/		
		virtual uint64_t GetNumUnderrun() = 0;

        //! This method returns the number of announced buffers in the data stream object.
		/*! 
		\return returns number of announced buffers   
		*/		
		virtual size_t GetNumAnnounced() = 0;

        //! This method returns the number of queued buffers in the data stream object.
		/*! 
		\return returns the number of buffers in the input pool and the number of buffers currently being filled   
		*/		
		virtual size_t GetNumQueued() = 0;

        //! This method returns the number of buffers awaiting delivery of the data stream object.
		/*! 
		\return returns the number of buffers in the output buffer queue   
		*/		
		virtual size_t GetNumAwaitDelivery() = 0;

        //! This method returns the buffer size of the data stream object.
		/*! 
		\return returns the buffer size   
		*/		
		virtual size_t GetBufferSize() = 0;

        //! This method returns a flag indicating if the data stream is grabbing or not.
		/*! 
		\return Flag indicating the state of the acquisition engine. If true, acquisition engine has stared. Otherwise, the acquistion engine is off.   
		*/		
        virtual bool IsGrabbing() = 0;

        //! This method returns the minimum number of buffers.
		/*! 
		\return returns the minimum number of buffers to announce   
		*/		
		virtual size_t GetMinNumBuffers() = 0;

        //! This method returns the alignment size of the stream object.
		/*! 
		\return returns the alignment size in bytes of the buffer passed   
		*/		
		virtual size_t GetBufferAlignment() = 0;
	};

	/**  
	*\brief The Buffer class represents the buffer module in the GenTL module hierarchy. 
	\details This Buffer class contains a method that can be used to temporarily hold a chuck of memory 
	from the acquisition engine. It also contains methods that can retrieve 
	information of the received data (held data) such as pointer to an image, image size,  
	and pixel format.
	*/
	class Buffer
	{
	public:

        //! A destructor of the Stream class.
        /*!
        \brief Destructor. Destroys the Stream and all its descendants.
        */
        virtual ~Buffer(){}

        //! This method returns the pointer to the memory of the image object.
		/*! 
		\return returns the pointer to the memory of the image object   
		*/		
        virtual IpxImage* GetImage() = 0;

        //! This method returns the pointer to the memory of the buffer object.
		/*! 
		\return returns the pointer to the memory of Buffer Object   
		*/		
        virtual void* GetBufferPtr() = 0;

        //! This method returns the image offset of the image.
		/*! 
		\return returns the Image Offset of the Image   
		*/		
		virtual size_t GetImageOffset() = 0;

        //! This method returns the buffer size of the allocated memory.
		/*! 
		\return returns the number of bytes written into the buffers   
		*/		
		virtual size_t GetBufferSize() = 0;

        //! This method returns the pixel format of the buffer object.
		/*! 
		\return returns the Pixel Format of the Buffer Object  
		*/		
		virtual uint64_t GetPixelFormat() = 0;

        //! This method returns the user data buffer pointer to memory of the buffer object.
		/*! 
		\return returns the user data buffer pointer to memory of the Buffer Object   
		*/		
        virtual void* GetUserPtr() = 0;

        //! This method returns the timestamp of the buffer that was acquired.
		/*! 
		\return returns the timestamp of the buffer that was acquired   
		*/		
		virtual uint64_t GetTimestamp() = 0;

        //! This method returns the frame id of the image stream block id of the buffer object.
		/*! 
		\return returns the image stream block id of the buffer object 
		*/		
		virtual uint64_t GetFrameID() = 0;

        //! This method returns a flag indicating if the buffer data has been transferred or an incomplete transfer.
		/*! 
		\return Is true, if an error occurred during the transferring of the buffer data   
		*/		
		virtual bool IsIncomplete() = 0;

        //! This method gets the width of the buffer data in number of pixels.
		/*! 
		\return returns the width of the data in the buffer in number of pixels   
		*/		
		virtual size_t GetWidth() = 0;

        //! This method gets the height of the data buffer.
		/*! 
		\return returns the height of the data in the buffer   
		*/		
		virtual size_t GetHeight() = 0;

        //! This method returns the XOffset of the data in the buffer.
		/*! 
		\return returns the XOffset of the data in the buffer in number of pixels from the image origin   
		*/		
		virtual size_t GetXOffset() = 0;

        //! This method returns the YOffset of the data in the buffer.
		/*! 
		\return returns the the YOffset of the data in the buffer in number of lines from the image orgin   
		*/		
		virtual size_t GetYOffset() = 0;

        //! This method returns the number of extra bytes padded in the X direction.
		/*! 
		\return returns the XPadding of the data in the buffer in number of bytes   
		*/		
		virtual size_t GetXPadding() = 0;

        //! This method returns the number of extra bytes padded in the Y direction.
		/*! 
		\return returns the YPadding of the data in the buffer in number of bytes   
		*/		
		virtual size_t GetYPadding() = 0;

        //! This method returns the actual height of delivered data.
        /*!
        \return returns the actual height of delivered data
        */
		virtual size_t GetDeliveredHeight() = 0;

        //! This method indicates if this buffer is Frame A or Frame B, acquired from Cheetah camera with KAC-12040 or KAC-06040 CMOS sensor.
		/*! 
		\return returns true for Frame B, false - otherwise   
		*/		
		virtual bool IsKacFrameB() = 0;
	};

} // end of namespace IpxCam

#endif // __cplusplus

#endif // IPX_CAMERA_API_H
