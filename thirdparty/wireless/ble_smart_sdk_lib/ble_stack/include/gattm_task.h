/*
 * gattm_task.h
 *
 *  Created on: Sep 16, 2013
 *      Author: delsissy
 */
#ifndef GATTM_TASK_H_
#define GATTM_TASK_H_

#include "cmn_defs.h"

/// maximum number of handles returned
#define GATT_MAX_HDL_LIST                            0x08

/// Local database maximum size
#define GATT_DB_MAX_SIZE                            0x50

/// size for 128-bit UUID
#define GATT_UUID_16_OCTET_LEN                      0x10
#define MAX_USED_BUFFERS    5
extern uint32_t u32usedBuffers;


struct gattm_att_desc
{
    /** Attribute UUID (LSB First) */
    uint8_t uuid[ATT_UUID_128_LEN];
    /**
     *  Attribute Permission (@see attm_perm_mask)
     */
    uint16_t perm;

    /**
     *  15   14   13   12   11   10   9    8    7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     * | RI |                               MAX_LEN                                    |
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     *
     * Bit [0-14]: Maximum Attribute Length
     * Bit [15]  : Trigger Read Indication (0 = Value present in Database,
     *                                      1 = Value not present in Database)
     *
     * For Included Services and Characteristic Declarations, this field contains targeted
     * handle.
     *
     * For Characteristic Extended Properties, this field contains 2 byte value
     *
     * Not used Client Characteristic Configuration and Server Characteristic Configuration,
     * this field is not used.
     */
    uint16_t max_len;
};
struct gattm_svc_desc
{
    /// Attribute Start Handle (0 = dynamically allocated)
    uint16_t start_hdl;
    /// Task identifier that manages service
    uint16_t task_id;

    /**
     *  7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+
     * |RFU | P  |UUID_LEN |  AUTH   |EKS | MI |
     * +----+----+----+----+----+----+----+----+
     *
     * Bit [0]  : Task that manage service is multi-instantiated (Connection index is conveyed)
     * Bit [1]  : Encryption key Size must be 16 bytes
     * Bit [2-3]: Service Permission      (0 = Disable, 1 = Enable, 2 = UNAUTH, 3 = AUTH)
     * Bit [4-5]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
     * Bit [6]  : Primary Service         (1 = Primary Service, 0 = Secondary Service)
     * Bit [7]  : Reserved for future use
     */
    uint8_t perm;

    /// Number of attributes
    uint8_t nb_att;

    /** Service  UUID */
    uint8_t uuid[ATT_UUID_128_LEN];
    /**
     * List of attribute description present in service.
     */
    struct gattm_att_desc  atts[1];
};

void db_init(uint8_t *memPool, uint32_t memSize);

uint8_t gattm_add_svc_req_handler(struct gattm_svc_desc *svc_desc);

uint8_t gattm_add_attribute_req_handler(struct gattm_svc_desc *svc_desc, uint16_t max_data_size, uint16_t perm,
                                        uint8_t uuid_len, uint8_t *uuid, uint16_t *handle);
uint8_t gattm_att_set_value_req_handler(uint16_t handle, uint16_t length, uint8_t *value);
uint8_t gattm_att_get_value_req_handler(uint16_t handle, uint16_t *length, uint8_t *value);

uint8_t gatt_notify_req_handler(uint16_t conhdl, uint16_t charhdl);
uint8_t gatt_indicate_req_handler(uint16_t conhdl, uint16_t charhdl);
uint8_t gatt_write_attribute_req_handler(uint16_t length, uint16_t attr_hdl, uint16_t perm,
        uint8_t *val);
uint8_t gatt_clear_attribute_req_handler(uint16_t attr_hdl);
uint8_t gatt_read_attribute_req_handler(uint16_t attr_hdl, uint16_t *len, uint16_t *perm, uint8_t *uuid_len,
                                        uint8_t *uuid, uint8_t *value);
void gatt_resource_access_req_handler(uint16_t con_hdl);
void gatt_resource_access_resp_handler(uint16_t con_hdl);

void gatt_disc_svc_req_handler(uint8_t u8ReqType, uint16_t u16ConHdl, uint16_t u16StartHdl, uint16_t u16EndHdl,
                               uint16_t u16AttDesValSize, uint8_t *pu8AttDesVal);
void gatt_disc_char_req_handler(uint8_t u8ReqType, uint16_t u16ConHdl, uint16_t u16StartHdl, uint16_t u16EndHdl,
                                uint16_t u16AttDesValSize, uint8_t *pu8AttDesVal);
void gatt_read_char_req_handler(uint8_t u8ReqType, uint16_t u16Offset, uint16_t u16ConHdl, uint16_t u16StartHdl,
                                uint16_t u16EndHdl, uint16_t u16NbOfUUID, uint8_t u8UUIDSize, uint8_t u8RespSize,
                                uint16_t u16UUID);
uint8_t gatt_write_char_req_handler(uint16_t u16ConHdl, uint16_t u16CharHdl, uint16_t u16WrOffset, uint16_t u16ValLen,
                                    uint8_t u8ReqType, uint8_t u8AutoExec, uint8_t *pu8Val);
/// GATT Task messages
enum gattm_msg_id
{
    /* Database Management */
    /// Add service in database request
    GATTM_ADD_SVC_REQ = ((uint16_t)(TASK_GATTM << 8)),
    /// Add service in database response
    GATTM_ADD_SVC_RSP,


    /* Service management */
    /// Get permission settings of service request
    GATTM_SVC_GET_PERMISSION_REQ,
    /// Get permission settings of service response
    GATTM_SVC_GET_PERMISSION_RSP,
    /// Set permission settings of service request
    GATTM_SVC_SET_PERMISSION_REQ,
    /// Set permission settings of service response
    GATTM_SVC_SET_PERMISSION_RSP,

    /* Attribute Manipulation */
    /// Get permission settings of attribute request
    GATTM_ATT_GET_PERMISSION_REQ,
    /// Get permission settings of attribute response
    GATTM_ATT_GET_PERMISSION_RSP,
    /// Set permission settings of attribute request
    GATTM_ATT_SET_PERMISSION_REQ,
    /// Set permission settings of attribute response
    GATTM_ATT_SET_PERMISSION_RSP,

    /// Get attribute value request
    GATTM_ATT_GET_VALUE_REQ,
    /// Get attribute value response
    GATTM_ATT_GET_VALUE_RSP,
    /// Set attribute value request
    GATTM_ATT_SET_VALUE_REQ,
    /// Set attribute value response
    GATTM_ATT_SET_VALUE_RSP,

    /* Debug messages */
    /// DEBUG ONLY: Destroy Attribute database request
    GATTM_DESTROY_DB_REQ,
    /// DEBUG ONLY: Destroy Attribute database response
    GATTM_DESTROY_DB_RSP,
    /// DEBUG ONLY: Retrieve list of services request
    GATTM_SVC_GET_LIST_REQ,
    /// DEBUG ONLY: Retrieve list of services response
    GATTM_SVC_GET_LIST_RSP,
    /// DEBUG ONLY: Retrieve information of attribute request
    GATTM_ATT_GET_INFO_REQ,
    /// DEBUG ONLY: Retrieve information of attribute response
    GATTM_ATT_GET_INFO_RSP
};


/**
 *  15   14   13   12   11   10   9    8    7    6    5    4    3    2    1    0
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 * |UUID_LEN | WR | WS | WC |EKS | B  |EXT |    N    |    I    |   WP    |    RD   |
 * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
 *
 * Bit [0-1]  : Read Permission         (0 = Disable, 1 = Enable, 2 = UNAUTH, 3 = AUTH)
 * Bit [2-3]  : Write Permission        (0 = Disable, 1 = Enable, 2 = UNAUTH, 3 = AUTH)
 * Bit [4-5]  : Indication Permission   (0 = Disable, 1 = Enable, 2 = UNAUTH, 3 = AUTH)
 * Bit [6-7]  : Notification Permission (0 = Disable, 1 = Enable, 2 = UNAUTH, 3 = AUTH)
 * Bit [8]    : Extended properties present (only relevant for a characteristic value)
 * Bit [9]    : Broadcast permission        (only relevant for a characteristic value)
 * Bit [10]   : Encryption key Size must be 16 bytes
 * Bit [11]   : Write Command accepted
 * Bit [12]   : Write Signed accepted
 * Bit [13]   : Write Request accepted
 * Bit [15-14]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
 */
enum attm_perm_mask
{
    /// retrieve all permission info
    PERM_MASK_ALL          = 0x0000,
    /// Read Access Mask
    PERM_MASK_RD           = 0x0003,
    PERM_POS_RD            = 0,
    /// Write Access Mask
    PERM_MASK_WR           = 0x000C,
    PERM_POS_WR            = 2,
    /// Indication Access Mask
    PERM_MASK_IND          = 0x0030,
    PERM_POS_IND           = 4,
    /// Notification Access Mask
    PERM_MASK_NTF          = 0x00C0,
    PERM_POS_NTF           = 6,
    /// Extended properties descriptor present
    PERM_MASK_EXT           = 0x0100,
    PERM_POS_EXT            = 8,
    /// Broadcast descriptor present
    PERM_MASK_BROADCAST     = 0x0200,
    PERM_POS_BROADCAST      = 9,
    /// Check Encryption key size Mask
    PERM_MASK_EKS           = 0x0400,
    PERM_POS_EKS            = 10,
    /// Write Command Enabled attribute Mask
    PERM_MASK_WRITE_COMMAND = 0x0800,
    PERM_POS_WRITE_COMMAND  = 11,
    /// Write Signed Enabled attribute Mask
    PERM_MASK_WRITE_SIGNED  = 0x1000,
    PERM_POS_WRITE_SIGNED   = 12,
    /// Write Request Enabled attribute Mask
    PERM_MASK_WRITE_REQ     = 0x2000,
    PERM_POS_WRITE_REQ      = 13,
    /// UUID Length
    PERM_MASK_UUID_LEN      = 0xC000,
    PERM_POS_UUID_LEN       = 14,
};
enum
{
    /// Disable access
    PERM_RIGHT_16_BIT  = 0,
    /// Enable access
    PERM_RIGHT_32_BIT   = 1,
    /// Access Requires Unauthenticated link
    PERM_RIGHT_128_BIT   = 2,
    /// Access Requires Authenticated link
    PERM_RIGHT_RFU     = 3,
};

/// request type - application interface
enum
{
    /*              Attribute Client Flags              */
    /*      Operation flags for discovery operation     */
    /* ************************************************ */
    /// Discover all services
    GATT_DISC_ALL_SVC           = 0x00,
    /// Discover services by UUID
    GATT_DISC_BY_UUID_SVC,
    /// Discover included services
    GATT_DISC_INCLUDED_SVC,
    /// Discover all characteristics
    GATT_DISC_ALL_CHAR,
    /// Discover characteristic by UUID
    GATT_DISC_BY_UUID_CHAR,
    /// Discover characteristic descriptor
    GATT_DISC_DESC_CHAR,
    /* Operation flags for reading attributes           */
    /* ************************************************ */
    /// Read characteristic
    GATT_READ_CHAR,
    /// Read characteristic by UUID
    GATT_READ_BY_UUID_CHAR,
    /// Read long characteristic
    GATT_READ_LONG_CHAR,
    /// Read multiple long characteristic
    GATT_READ_MULT_LONG_CHAR,
    /// Read descriptor
    GATT_READ_DESC,
    /// Read long descriptor
    GATT_READ_LONG_DESC,
    /* Operation flags for writing/modifying attributes */
    /* ************************************************ */
    /// Write no response
    GATT_WRITE_NO_RESPONSE,
    /// Write signed
    GATT_WRITE_SIGNED,
    /// Write characteristic
    GATT_WRITE_CHAR,
    /// Write long characteristic
    GATT_WRITE_LONG_CHAR,
    /// Write reliable characteristic
    GATT_WRITE_RELIABLE_CHAR,
    /// Write descriptor
    GATT_WRITE_DESC,
    /// Write long descriptor
    GATT_WRITE_LONG_DESC,
    /* Cancel write flag                                */
    /* ************************************************ */
    /// Write cancel characteristic
    GATT_WRITE_CANCEL_CHAR,
    /*              Attribute Server Flags              */
    /*      Operation flags for attribute notification  */
    /* ************************************************ */
    /// Handle notification: signed or unsigned
    GATT_HDL_NTF,
    /*      Operation flag for attribute indication     */
    /* ************************************************ */
    /// Handle indication
    GATT_HDL_IND,
    /// No operation
    GATT_NO_OP_REQ,
    /// Last GATT flag
    GATT_LAST
};

#define PERM_POS_RD_ENABLE (0x01 << PERM_POS_RD)
#define PERM_POS_RD_AUTH   (0x03 << PERM_POS_RD)
#define PERM_POS_WR_ENABLE (0x01 << PERM_POS_WR)
#define PERM_POS_WR_AUTH   (0x03 << PERM_POS_WR)
#define PERM_POS_IND_ENABLE (0x01 << PERM_POS_IND)
#define PERM_POS_IND_AUTH (0x03 << PERM_POS_IND)
#define PERM_POS_NTF_ENABLE (0x01 << PERM_POS_NTF)
#define PERM_POS_NTF_AUTH (0x03 << PERM_POS_NTF)
#define PERM_POS_EXT_ENABLE (0x01 << PERM_POS_EXT)
#define PERM_POS_BROADCAST_ENABLE (0x01 << PERM_POS_BROADCAST)
#define PERM_POS_EKS_ENABLE (0x01 << PERM_POS_EKS)
#define PERM_POS_WRITE_COMMAND_ENABLE (0x01 << PERM_POS_WRITE_COMMAND)
#define PERM_POS_WRITE_SIGNED_ENABLE (0x01 << PERM_POS_WRITE_SIGNED)
#define PERM_POS_WRITE_REQ_ENABLE (0x01 << PERM_POS_WRITE_REQ)
#define PERM_POS_UUID_LEN_SET(x) ((x & 0x03) << PERM_POS_UUID_LEN)

#if 0
/// reliable structure
struct gatt_reliable_write
{
    /// characteristic handle
    uint16_t elmt_hdl;
    /// value size
    uint16_t size;
    /// value holder
    uint8_t value[ATTM_RELIABLE_WRITE];
};

/// attribute MTU exchange request Structure
struct gatt_exc_mtu_req
{
    /// connection handle
    uint16_t conhdl;
};

/// Service Discovery Request Structure
struct gatt_disc_svc_req
{
    /// GATT request type
    uint8_t req_type;
    /// connection handle
    uint16_t conhdl;
    /// start handle range
    uint16_t start_hdl;
    /// start handle range
    uint16_t end_hdl;
    /// desired service: if 0x0000, discover all
    struct att_desired_type desired_svc;
};

/// Characteristic Discovery Request Structure
struct gatt_disc_char_req
{
    /// GATT request type
    uint8_t req_type;
    /// connection handle
    uint16_t conhdl;
    /// start handle range
    uint16_t start_hdl;
    /// end handle range
    uint16_t end_hdl;
    /// desired UUID in disc service char
    struct att_desired_type desired_char;
};

/// Characteristic Descriptor Discovery Request Structure
struct gatt_disc_char_desc_req
{
    /// connection handle
    uint16_t conhdl;
    /// start handle range
    uint16_t start_hdl;
    /// end handle range
    uint16_t end_hdl;
};

/// Read Characteristic Values and Descriptor Request Structure
struct gatt_read_char_req
{
    /// request type
    uint8_t req_type;
    /// read offset
    uint16_t offset;
    /// connection handle
    uint16_t conhdl;
    /// start handle range
    uint16_t start_hdl;
    /// end handle range
    uint16_t end_hdl;
    /// number of UUID
    uint16_t nb_uuid;
    /// characteristic UUID
    struct att_uuid_type uuid[ATT_NB_MULT_HDLS];
};

/// Write Reliable Characteristic Request Structure
struct gatt_write_reliable_req
{
    /// number of reliable writes
    uint8_t nb_writes;
    /// automatic execute write
    uint8_t auto_execute;
    /// connection handle
    uint16_t conhdl;
    /// number of reliable
    struct gatt_reliable_write reliable_write[1];
};

/// Write Characteristic Request Structure
struct gatt_write_char_req
{
    /// connection handle
    uint16_t conhdl;
    /// valid characteristic handle
    uint16_t charhdl;
    /// offset to write
    uint16_t wr_offset;
    /// size of the value data
    uint16_t val_len;
    /// request type
    uint8_t req_type;
    /// execute write
    uint8_t auto_execute;
    /// check, maybe union type is required
    uint8_t value[1];
};

/// execute write characteristic request Structure
struct gatt_execute_write_char_req
{
    /// connection handle
    uint16_t conhdl;
    /// flag to describe if write or cancel
    uint8_t exe_wr_ena;
};

/// notify request Structure
struct gatt_notify_req
{
    /// connection handle
    uint16_t conhdl;
    /// connection handle
    uint16_t charhdl;
};

/// Indicate Request Structure
struct gatt_indicate_req
{
    /// connection handle
    uint16_t conhdl;
    /// characteristic handle
    uint16_t charhdl;
};

/// Data list for service result
struct gatt_svc_list
{
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// attribute handle
    uint16_t attr_hdl;
};

/// Data list for service result
struct gatt_svc_128_list
{
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// attribute handle
    uint8_t attr_hdl[GATT_UUID_16_OCTET_LEN];
};
/// Data list for characteristic result - 16-bit
struct gatt_char_list
{
    /// database element handle
    uint16_t attr_hdl;
    /// properties
    uint8_t prop;
    /// pointer handle to UUID
    uint16_t pointer_hdl;
    /// characteristic UUID
    uint16_t uuid;
};

/// Data list for characteristic result - 128-bit
struct gatt_char_128_list
{
    /// database element handle
    uint16_t attr_hdl;
    /// properties
    uint8_t prop;
    /// pointer handle to UUID
    uint16_t pointer_hdl;
    /// characteristic UUID
    uint8_t uuid[GATT_UUID_16_OCTET_LEN];
};

/// Data list for include result
struct gatt_incl_list
{
    /// element handle
    uint16_t attr_hdl;
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// included service UUID
    uint16_t uuid;
};

/// Special data list for include result - 128
struct gatt_incl_128_list
{
    /// element handle
    uint16_t attr_hdl;
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// included 128-bit service UUID
    uint8_t uuid[GATT_UUID_16_OCTET_LEN];
};

/// Special data list for descriptor result - 128
struct gatt_char_desc_list
{
    /// database element handle
    uint16_t attr_hdl;
    /// descriptor UUID
    uint16_t desc_hdl;
};

/// Special data list for descriptor result - 128
struct gatt_char_desc_128_list
{
    /// database element handle
    uint16_t attr_hdl;
    /// included 128-bit service UUID
    uint8_t uuid[GATT_UUID_16_OCTET_LEN];
};

/// Discover All Complete Event Structure
struct gatt_disc_svc_all_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of value pairs
    uint8_t nb_resp;
    /// contain data list
    struct gatt_svc_list list[GATT_MAX_HDL_LIST];
};


/// Discover All Complete Event Structure
struct gatt_disc_svc_all_128_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of value pairs
    uint8_t nb_resp;
    /// contain data list
    struct gatt_svc_128_list list;
};

/// service range
struct gatt_svc_range_list
{
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
};
/// Discover by UUID Complete Event Structure
struct gatt_disc_svc_by_uuid_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of value pairs
    uint8_t nb_resp;
    /// list of found services
    struct gatt_svc_range_list list[GATT_MAX_HDL_LIST];
};

/// Discover Service Include Complete Event Structure
struct gatt_disc_svc_incl_cmp_evt
{
    /// number of found includes
    uint8_t nb_entry;
    /// size of the entry
    uint8_t entry_len;
    /// include entries
    union incl_list_tag
    {
        /// one include with 128-bit entry
        struct gatt_incl_128_list incl;
        /// list of include declarations
        struct gatt_incl_list list[GATT_MAX_HDL_LIST];
    } incl_list;
};

/// Discovery All Characteristic Complete Event Structure
struct gatt_disc_char_all_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of entries
    uint8_t nb_entry;
    /// list of found characteristics
    struct gatt_char_list list[GATT_MAX_HDL_LIST];
};

/// Discovery All Characteristic Complete Event Structure
struct gatt_disc_char_all_128_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of entries
    uint8_t nb_entry;
    /// list of found characteristics
    struct gatt_char_128_list list;
};

/// Discover Characteristic by UUID Complete Event Structure
struct gatt_disc_char_by_uuid_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of entries
    uint8_t nb_entry;
    /// list of found characteristics with UUID
    struct gatt_char_list list[GATT_MAX_HDL_LIST];
};

/// Discover Characteristic by UUID Complete Event Structure
struct gatt_disc_char_by_uuid_128_cmp_evt
{
    /// complete event status
    uint8_t status;
    /// number of entries
    uint8_t nb_entry;
    /// found 128-bit characteristics with UUID
    struct gatt_char_128_list list;
};

/// Discovery Characteristic Descriptor Complete Event Structure
struct gatt_disc_char_desc_cmp_evt
{
    /// number of found includes
    uint8_t nb_entry;
    /// list of include declarations
    struct gatt_char_desc_list list[GATT_MAX_HDL_LIST];
};

/// Discovery Characteristic Descriptor Complete Event Structure
struct gatt_disc_char_desc_128_cmp_evt
{
    /// number of found includes
    uint8_t nb_entry;
    /// include entries
    struct gatt_char_desc_128_list list_128;
};

/// Read Characteristic Response Structure
struct gatt_read_char_resp
{
    /// complete event status
    uint8_t status;
    /// holder of retrieved data
    struct att_info_data data;
};

/// Query result for multiple responses
struct gatt_query_result
{
    /// length of value
    uint8_t len;
    /// data result from query
    uint8_t value[ATTM_MAX_VALUE];
};

/// Read Characteristic Multiple Responses Structure
struct gatt_read_char_mult_resp
{
    /// complete event status
    uint8_t status;
    /// size of the response data
    uint8_t val_len;
    /// actual value pairs
    struct gatt_query_result data[ATT_NB_MULT_HDLS];
};

/// Read Characteristic Descriptor Response Structure
struct gatt_read_char_desc_resp
{
    /// complete event status
    uint8_t status;
    /// size of the value data
    uint8_t val_len;
    /// actual value pairs
    uint8_t value[ATTM_MAX_VALUE];
};

/// Read Characteristic Long Descriptor Response Structure
struct gatt_read_char_long_desc_resp
{
    /// complete event status
    uint8_t status;
    /// size of the value data
    uint8_t val_len;
    /// handle
    uint16_t attr_hdl;
    /// actual value pairs
    uint8_t value[1];

};

/// Write Characteristics Response Structure
struct gatt_write_char_resp
{
    /// complete event status
    uint8_t status;
};

/// Write Reliable Response Structure
struct gatt_write_reliable_resp
{
    /// complete event status
    uint8_t status;
};

/// Confirm Response Structure
struct gatt_confirm_resp
{
    /// complete event status
    uint8_t status;
};

/// GATT Complete Event Structure
struct gatt_cmp_evt
{
    /// complete event status
    uint8_t status;
};

/// GATT Discovery Complete Event Structure
struct gatt_disc_cmp_evt
{
    /// complete event status
    uint8_t status;
};

/* Attribute server manipulation */
/// Add or write (update) an attribute to Attribute Server Database
struct gatt_write_attribute_req
{
    /// size of the data
    uint16_t length;
    /// attribute handle
    uint16_t attr_hdl;
    /// attribute permission
    uint16_t perm;
    /// value to write
    uint8_t val[ATTM_MAX_VALUE];
};

/// Remove an attribute to Attribute Server Database
struct gatt_clear_attribute_req
{
    /// attribute handle
    uint16_t attr_hdl;
};

/// Read an attribute to Attribute Server Database
struct gatt_read_attribute_req
{
    /// attribute handle
    uint16_t attr_hdl;
};

/// Add or write (update) an attribute to Attribute Server Database
struct gatt_write_attribute_cmp_evt
{
    /// complete event status
    uint8_t status;
};

/// Remove an attribute to Attribute Server Database
struct gatt_clear_attribute_cmp_evt
{
    /// complete event status
    uint8_t status;
};

/// Read an attribute to Attribute Server Database
struct gatt_read_attribute_cmp_evt
{
    /// attribute handle
    uint16_t elmt_hdl;
    /// length of the data
    uint16_t len;
    /// permission
    uint16_t perm;
    /// complete event status
    uint8_t status;
    /// attribute type
    uint8_t uuid_len;
    /// attribute type
    uint8_t uuid[ATT_UUID_128_LEN];
    /// attribute value
    uint8_t value[GATT_DB_MAX_SIZE];
};

/// Handle value notification
struct gatt_handle_value_notif
{
    /// connection handle
    uint16_t conhdl;
    /// attribute handle
    uint16_t charhdl;
    /// size of data
    uint8_t size;
    /// value
    uint8_t value[ATTM_MAX_VALUE];
};

/// Handle value indication
struct gatt_handle_value_ind
{
    /// connection handle
    uint16_t conhdl;
    /// attribute handle
    uint16_t charhdl;
    /// size of data
    uint8_t size;
    /// value
    uint8_t value[ATTM_MAX_VALUE];
};

/// Handle value confirmation
struct gatt_handle_value_cfm
{
    /// element handle
    uint16_t handle;
    /// status
    uint8_t status;
};

/// Parameters of the @ref GATT_SIGN_KEY_EVT message
struct gatt_sign_key_evt
{
    /// Signature
    uint8_t sign[ATT_SIGNATURE_LEN];
};

/// write command indication
struct gatt_write_cmd_ind
{
    /// Connection handle
    uint16_t conhdl;
    /// element handle
    uint16_t handle;
    /// length
    uint16_t length;
    /// offset
    uint16_t offset;
    /// Flag used to know if peer device shall receive write response
    uint8_t response;
    /// Flag used to know if peer the indication is the latest
    NMI_Bool last;
    /// value written
    uint8_t value[ATTM_MAX_VALUE];
};

///Parameters for @ref GATT_SVC_REG2PRF_REQ message
struct gatt_svc_reg2prf_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Service start handle
    uint16_t svc_shdl;
    /// Service end handle
    uint16_t svc_ehdl;
};

///Parameters for @ref GATT_SVC_UNREG2PRF_REQ message
struct gatt_svc_unreg2prf_req
{
    /// Connection handle
    uint16_t conhdl;
    /// Service start handle
    uint16_t svc_shdl;
    /// Service end handle
    uint16_t svc_ehdl;
};

///Parameters for @ref GATT_NOTIFY_CMP_EVT message
struct  gatt_notify_cmp_evt
{
    /// attribute handle
    uint16_t handle;
    /// status
    uint8_t status;
};

/// Peer device request access of device resources
struct gatt_resource_access_req
{
    /// device connection handle
    uint16_t    conhdl;
};

/// Current device is ready to access peer device requests
struct gatt_resource_access_rsp
{
    /// device connection handle
    uint16_t    conhdl;
};

/////Parameters for @ref GATT_SVC_CHANGED_IND_SEND_REQ message
struct gatt_svc_changed_ind_send_req
{
    /// Start of Affected Attribute Handle Range
    uint16_t svc_shdl;
    /// End of Affected Attribute Handle Range
    uint16_t svc_ehdl;

    /// Connection handle, used only if bonded != CO_ERROR_NO_ERROR
    uint16_t conhdl;
    /**
     * If bonded != CO_ERROR_NO_ERROR, a Service Changed indication will be sent to
     * all connected peer device, else this message will be sent only to the device
     * defined by the provided connection handle.
     */
    uint8_t bonded;
};

#endif

#endif // GATT_TASK_H_
