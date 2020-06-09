#ifndef IPX_CAMERA_ERR_H
#define IPX_CAMERA_ERR_H

// GenTL error codes
#define INC_GEN_TL_ERROR_CODES
#ifdef INC_GEN_TL_ERROR_CODES
// Error codes from GenTL.h, just for reference
enum GC_ERROR_LIST
{
    GC_ERR_SUCCESS             = 0,
    GC_ERR_ERROR               = -1001,
    GC_ERR_NOT_INITIALIZED     = -1002,
    GC_ERR_NOT_IMPLEMENTED     = -1003,
    GC_ERR_RESOURCE_IN_USE     = -1004,
    GC_ERR_ACCESS_DENIED       = -1005,
    GC_ERR_INVALID_HANDLE      = -1006,
    GC_ERR_INVALID_ID          = -1007,
    GC_ERR_NO_DATA             = -1008,
    GC_ERR_INVALID_PARAMETER   = -1009,
    GC_ERR_IO                  = -1010,
    GC_ERR_TIMEOUT             = -1011,
    GC_ERR_ABORT               = -1012, /* GenTL v1.1 */
    GC_ERR_INVALID_BUFFER      = -1013, /* GenTL v1.1 */
    GC_ERR_NOT_AVAILABLE       = -1014, /* GenTL v1.2 */
    GC_ERR_INVALID_ADDRESS     = -1015, /* GenTL v1.3 */
    GC_ERR_BUFFER_TOO_SMALL    = -1016, /* GenTL v1.4 */
    GC_ERR_INVALID_INDEX       = -1017, /* GenTL v1.4 */
    GC_ERR_PARSING_CHUNK_DATA  = -1018, /* GenTL v1.4 */
    GC_ERR_INVALID_VALUE       = -1019, /* GenTL v1.4 */
    GC_ERR_RESOURCE_EXHAUSTED  = -1020, /* GenTL v1.4 */
    GC_ERR_OUT_OF_MEMORY       = -1021, /* GenTL v1.4 */
    
    GC_ERR_CUSTOM_ID           = -10000
};
#endif //INC_GEN_TL_ERROR_CODES

// Imperx CameraAPI error codes
#ifdef __cplusplus

enum IpxCamErr
{
    IPX_CAM_ERR_OK = 0,
    IPX_CAM_ERR_UNKNOWN = -40001,
    IPX_CAM_ERR_FILE_NOT_FOUND = -40002,
    IPX_CAM_ERR_FILE_READ = -40003,
    IPX_CAM_ERR_WRONG_CONFIGURATION = -40004,
    IPX_CAM_ERR_INVALID_INDEX = -40005,
	IPX_CAM_ERR_NO_DEVICE = -40006,
	IPX_CAM_ERR_INVALID_ARGUMENT = -40007,
	    
    IPX_CAM_ERR_INVALID_STATE = -40051,
    
    IPX_CAM_ERR_FLASH_ERASE = -40101,
    
	IPX_CAM_GENICAM_GENERIC_ERROR = -40200,
    IPX_CAM_GENICAM_TREE_ERROR = -40201,
    IPX_CAM_GENICAM_ACCESS_ERROR = -40202,
    IPX_CAM_GENICAM_TYPE_ERROR = -40203,
	IPX_CAM_GENICAM_OUT_OF_RANGE = -40204,
	IPX_CAM_GENICAM_UNKNOWN_PARAM = -40205,
	IPX_CAM_GENICAM_INVALID_ARGUMENT = -40206,
	
} ;



#else

typedef int32_t IpxCamErr;
#define IPX_CAM_ERR_OK      0
#define IPX_CAM_ERR_UNKNOWN -40001

#endif

#endif //IPX_CAMERA_ERR_H