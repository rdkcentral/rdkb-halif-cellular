/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
/**
* @file cellular_hal.h
* @brief For CCSP Component: Cellular Manager HAL Layer
*
*/
/**
 * @defgroup CELLULAR_HAL CELLULAR HAL
 *
 * This module provides the function call prototypes used for the Cellular Manager abstraction layer..
 *
 * @defgroup CELLULAR_HAL_TYPES   CELLULAR HAL Data Types
 * @ingroup  CELLULAR_HAL
 *
 * @defgroup CELLULAR_HAL_APIS   CELLULAR HAL APIs
 * @ingroup  CELLULAR_HAL
 *
 **/
#ifndef _CELLULAR_HAL_H_
#define _CELLULAR_HAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <glib-unix.h>
#ifndef CELLULAR_MGR_LITE
#include <libqmi-glib.h>
#endif
/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
/**
* @addtogroup CELLULAR_HAL_TYPES
* @{
*/
/** For now we are supporting QMI library */
#ifndef CELLULAR_MGR_LITE
#define QMI_SUPPORT
#endif
#define RETURN_ERROR        (-1)
#define RETURN_OK           (0)
#ifndef TRUE
#define TRUE                (1)
#endif
#ifndef FALSE
#define FALSE               (0)
#endif
#ifndef BUFLEN_8
#define BUFLEN_8            (8)
#endif
#ifndef BUFLEN_32
#define BUFLEN_32           (32)
#endif
#ifndef BUFLEN_64
#define BUFLEN_64           (64)
#endif
#ifndef BUFLEN_128
#define BUFLEN_128          (128)
#endif
#ifndef BUFLEN_256
#define BUFLEN_256          (256)
#endif
#define CELLULAR_PROFILE_ID_UNKNOWN               (-1)          //!< Profile ID is not present
#define CELLULAR_SLOT_ID_UNKNOWN                  (-1)          //!< Slot ID is not present 
#define CELLULAR_PDP_CONTEXT_UNKNOWN              (-1)          //!< Packet data protocol context is not present
#define CELLULAR_PACKET_DATA_INVALID_HANDLE       (0xFFFFFFFF)  //!< Invalid packet data handle

/*
* TODO (Enhance Error Reporting):
*   - Replace the generic `RETURN_ERR` with a more descriptive error code enumeration.
*   - Define specific error codes to pinpoint various failure scenarios, including:
*       - Invalid input parameters (e.g., null pointers, out-of-range values)
*       - Resource allocation failures (e.g., out-of-memory)
*       - Communication or timeout issues with external systems (e.g., backend services)
*       - File system errors (e.g., file not found, permission denied)
*       - Internal errors within the HAL
*       - Specific DOCSIS-related errors (refer to DOCSIS specifications)
*   - Document the new error codes thoroughly in the header file and any relevant guides.
*/

/**! Represents the operational status of a cellular interface. */
typedef enum _CellularInterfaceStatus_t {
    IF_UP = 1,         /**!< Interface is up and running. */
    IF_DOWN,           /**!< Interface is down (not running). */
    IF_UNKNOWN,        /**!< Interface status is unknown. */
    IF_DORMANT,        /**!< Interface is in a dormant state. */
    IF_NOTPRESENT,     /**!< Interface is not present in the system. */
    IF_LOWERLAYERDOWN, /**!< The underlying network layer is down. */
    IF_ERROR           /**!< General error condition in the interface. */
} CellularInterfaceStatus_t;

/**! Represents the preferred IP family for a cellular interface. */
typedef enum _CellularIpFamilyPref_t {
    IP_FAMILY_UNKNOWN = 1, /**!< IP family is unknown. */
    IP_FAMILY_IPV4,       /**!< IPv4 is preferred. */
    IP_FAMILY_IPV6,       /**!< IPv6 is preferred. */
    IP_FAMILY_IPV4_IPV6  /**!< Either IPv4 or IPv6 is acceptable. */
} CellularIpFamilyPref_t;

/**!
 * @brief Represents the preferred access technology for a cellular interface.
 */
typedef enum _CellularPrefAccessTechnology_t {
    PREF_GPRS = 1,         /**!< GSM with GPRS. */
    PREF_EDGE,             /**!< GSM with EDGE. */
    PREF_UMTS,             /**!< UMTS. */
    PREF_UMTSHSPA,         /**!< 3GPP-HSPA. */
    PREF_CDMA2000OneX,     /**!< CDMA2000 1x. */
    PREF_CDMA2000HRPD,     /**!< CDMA2000 HRPD. */
    PREF_LTE,             /**!< LTE. */
    PREF_NR               /**!< 5G New Radio. */
} CellularPrefAccessTechnology_t;

/**!
 * @brief Represents the Packet Data Protocol (PDP) type for a cellular interface.
 * 
 * This specifies the type of IP address assigned to the PDP context in cellular communication.
 */
typedef enum _CellularPDPType_t {
    CELLULAR_PDP_TYPE_IPV4,       /**!< IPv4 address. */
    CELLULAR_PDP_TYPE_PPP,        /**!< Point-to-Point Protocol (PPP) address. */
    CELLULAR_PDP_TYPE_IPV6,       /**!< IPv6 address. */
    CELLULAR_PDP_TYPE_IPV4_OR_IPV6 /**!< Either IPv4 or IPv6 address. */
} CellularPDPType_t;

/**! Represents the Packet Data Protocol (PDP) authentication types for a cellular interface. */
typedef enum _CellularPDPAuthentication_t {
    CELLULAR_PDP_AUTHENTICATION_NONE = 0, /**!< No authentication. */
    CELLULAR_PDP_AUTHENTICATION_PAP,      /**!< Password Authentication Protocol (PAP). */
    CELLULAR_PDP_AUTHENTICATION_CHAP      /**!< Challenge-Handshake Authentication Protocol (CHAP). */
} CellularPDPAuthentication_t;

/**!<  Represents the type of cellular profile for a cellular interface. */
typedef enum _CellularProfileType_t {
    CELLULAR_PROFILE_TYPE_3GPP,  /**!< 3rd Generation Partnership Project (3GPP) profile. */
    CELLULAR_PROFILE_TYPE_3GPP2  /**!< 3rd Generation Partnership Project 2 (3GPP2) profile. */
} CellularProfileType_t;

/**! Represents the network configuration type for a cellular interface's Packet Data Protocol (PDP) context. */
typedef enum _CellularPDPNetworkConfig_t {
    CELLULAR_PDP_NETWORK_CONFIG_NAS = 1, /**!< Network provided by the Network Attachment Subsystem (NAS). */
    CELLULAR_PDP_NETWORK_CONFIG_DHCP   /**!< Network provided by the Dynamic Host Configuration Protocol (DHCP). */
} CellularPDPNetworkConfig_t;

/**! Represents commands for controlling a cellular modem's operating configuration. */
typedef enum _CellularModemOperatingConfiguration_t {
    CELLULAR_MODEM_SET_ONLINE = 1,      /**!< Set the modem to online mode. */
    CELLULAR_MODEM_SET_OFFLINE,        /**!< Set the modem to offline mode. */
    CELLULAR_MODEM_SET_LOW_POWER_MODE, /**!< Set the modem to low power mode. */
    CELLULAR_MODEM_SET_RESET,         /**!< Reset the modem. */
    CELLULAR_MODEM_SET_FACTORY_RESET  /**!< Reset the modem to factory defaults. */
} CellularModemOperatingConfiguration_t; 

/**! Represents the registered service types of a cellular modem. */
typedef enum _CellularModemRegisteredServiceType_t
{
    CELLULAR_MODEM_REGISTERED_SERVICE_NONE = 0, /**!< No service registered. */
    CELLULAR_MODEM_REGISTERED_SERVICE_PS,      /**!< Packet-switched service registered. */
    CELLULAR_MODEM_REGISTERED_SERVICE_CS,      /**!< Circuit-switched service registered. */
    CELLULAR_MODEM_REGISTERED_SERVICE_CS_PS    /**!< Both circuit-switched and packet-switched services registered. */
}
CellularModemRegisteredServiceType_t;

/**!< Represents the registration status of a cellular device. */
typedef enum _CellularRegistrationStatus_t
{
    DEVICE_REGISTERED = 1, /**!< Device is registered on the network. */
    DEVICE_NOT_REGISTERED  /**!< Device is not registered on the network. */
} CellularRegistrationStatus_t;

/**! Represents a cellular profile configuration. */
typedef struct
{
    int ProfileID;                 /**!< Unique identifier for the profile. */
    CellularProfileType_t ProfileType;  /**!< Type of cellular profile (3GPP or 3GPP2). */

    int PDPContextNumber;           /**!< Packet Data Protocol (PDP) context number. */
    CellularPDPType_t PDPType;       /**!< PDP type (IPv4, IPv6, PPP, or IPv4/IPv6). */
    CellularPDPAuthentication_t PDPAuthentication; /**!< PDP authentication type (none, PAP, or CHAP). */
    CellularPDPNetworkConfig_t PDPNetworkConfig;  /**!< PDP network configuration type (NAS or DHCP). */

    char ProfileName[64];          /**!< Vendor-specific profile name.
                                          The string is zero-terminated and the terminator is included in the size. */
    char APN[64];                   /**!< Access Point Name (APN) for the gateway (vendor-specific). 
                                          The string is zero-terminated and the terminator is included in the size. */
    char Username[256];             /**!< Username for authentication (vendor-specific).
                                          The string is zero-terminated and the terminator is included in the size. */
    char Password[256];             /**!< Password for authentication (vendor-specific).
                                          The string is zero-terminated and the terminator is included in the size. */
    char Proxy[45];                 /**!< IP address of the proxy server (vendor-specific).
                                          The string is zero-terminated and the terminator is included in the size. */
    unsigned int ProxyPort;        /**!< Port number for the proxy server (vendor-specific). */

    unsigned char bIsNoRoaming;     /**!< Indicates whether roaming is disabled (1 = true, 0 = false). */
    unsigned char bIsAPNDisabled;    /**!< Indicates whether APN is disabled (1 = true, 0 = false). */
    unsigned char bIsThisDefaultProfile; /**!< Indicates if this is the default profile (1 = true, 0 = false). */
} CellularProfileStruct;

/**!<  Represents input parameters for initializing a cellular interface. */
typedef struct
{
    CellularIpFamilyPref_t enIPFamilyPreference; /**!< Preferred IP family (IPv4, IPv6, or both). */
    CellularProfileStruct stIfInput;             /**!< Interface-specific configuration details. */
    CellularPrefAccessTechnology_t enPreferenceTechnology; /**!< Preferred access technology (e.g., LTE, 5G). */
} CellularContextInitInputStruct;

/**!< Represents the IP address type used by a cellular network. */
typedef enum _CellularNetworkIPType_t
{
    CELLULAR_NETWORK_IP_FAMILY_UNKNOWN = 0,   /**!< IP address type is unknown. */
    CELLULAR_NETWORK_IP_FAMILY_IPV4,         /**!< IPv4 address. */
    CELLULAR_NETWORK_IP_FAMILY_IPV6,         /**!< IPv6 address. */
    CELLULAR_NETWORK_IP_FAMILY_UNSPECIFIED   /**!< IP address type is not specified. */
} CellularNetworkIPType_t;

/**! Represents a cellular network connection's details. */
typedef  struct
{
    char WANIFName[16];                       /**!< It represents the name of the WAN interface.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "wwan0" */

    char IPAddress[128];                      /**!< It represents IP address of the device. The address should always be in the dotted-decimal format.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "192.168.1.10" */

    CellularNetworkIPType_t IPType;           /**!< It represents the type of IP address that the cellular network uses.
                                                   Possible value is 0,1,2 and 3. */

    char SubnetMask[128];                     /**!< It represents the subnet mask of the device.The address should always be in the dotted-decimal format.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "255.255.255.0" */

    char DefaultGateWay[128];                 /**!< It represents the IP address of the default gateway. The address should always be in the dotted-decimal format.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "192.168.1.1" */

    char DNSServer1[128];                     /**!< It represents the IP address of the primary DNS server. The address should always be in the dotted-decimal format.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "8.8.8.8" */

    char DNSServer2[128];                     /**!< It represents the IP address of the secondary DNS server. The address should always be in the dotted-decimal format.
                                                   The string is zero-terminated and the terminator is included in the size. Example:  "1.1.1.1" */

    char Domains[256];                        /**!< It represents the domain names that the device is associated with.
                                                   The string is zero-terminated and the terminator is included in the size. Example: "hsd.pa.crnrstn.comcast.net" */

    unsigned int MTUSize;                     /**!< It represents stores the maximum transmission unit size of the device.
                                                   The possible range of acceptable values is 1280 to 9000. */
} CellularIPStruct;

/**!< Represents data transfer statistics for a cellular network connection. */
typedef struct
{
    unsigned long BytesSent;          /**!< Total bytes sent over the cellular connection. */
    unsigned long BytesReceived;       /**!< Total bytes received over the cellular connection. */
    unsigned long PacketsSent;        /**!< Total packets sent over the cellular connection. */
    unsigned long PacketsReceived;     /**!< Total packets received over the cellular connection. */
    unsigned long PacketsSentDrop;    /**!< Total packets dropped during transmission. */
    unsigned long PacketsReceivedDrop; /**!< Total packets dropped during reception. */
    unsigned long UpStreamMaxBitRate;  /**!< Maximum upstream bit rate (bits/second). */
    unsigned long DownStreamMaxBitRate; /**!< Maximum downstream bit rate (bits/second). */
} CellularPacketStatsStruct;


/**!< Represents the form factors of a Universal Integrated Circuit Card (UICC). */
typedef enum _CellularUICCFormFactor_t {
    CELLULAR_UICC_FORM_FACTOR_1FF = 0, /**!< Full-size SIM card. */
    CELLULAR_UICC_FORM_FACTOR_2FF,    /**!< Mini SIM card. */
    CELLULAR_UICC_FORM_FACTOR_3FF,    /**!< Micro SIM card. */
    CELLULAR_UICC_FORM_FACTOR_4FF     /**!< Nano SIM card. */
} CellularUICCFormFactor_t; 

/**! Represents the status of a Universal Integrated Circuit Card (UICC). */
typedef enum _CellularUICCStatus_t
{
    CELLULAR_UICC_STATUS_VALID = 0,   /**!< UICC is valid. */
    CELLULAR_UICC_STATUS_BLOCKED,     /**!< UICC is blocked. */
    CELLULAR_UICC_STATUS_ERROR,       /**!< UICC is in an error state. */
    CELLULAR_UICC_STATUS_EMPTY        /**!< UICC has no data. */
} CellularUICCStatus_t;

/**! Represents the type of application on a Universal Integrated Circuit Card (UICC). */
typedef enum _CellularUICCApplication_t {
    CELLULAR_UICC_APPLICATION_USIM = 0, /**!< Universal Subscriber Identity Module (USIM) for 3G/4G networks. */
    CELLULAR_UICC_APPLICATION_ISIM,    /**!< IP Multimedia Services Identity Module (ISIM) for IMS networks. */
    CELLULAR_UICC_APPLICATION_ESIM    /**!< Embedded SIM (eSIM) application. */
} CellularUICCApplication_t;

/**!<  Represents information about a Universal Integrated Circuit Card (UICC) slot. */
typedef struct
{
    unsigned char SlotEnable;        /**!< Whether the slot is enabled (1) or disabled (0). */
    unsigned char IsCardPresent;     /**!< Whether a card is present in the slot (1) or not (0). */
    unsigned char CardEnable;        /**!< Whether the card in the slot is enabled (1) or disabled (0). */
    CellularUICCFormFactor_t FormFactor; /**!< UICC form factor (e.g., 1FF, 2FF, 3FF, 4FF). */
    CellularUICCApplication_t Application; /**!< Type of UICC application (USIM, ISIM, eSIM). */
    CellularUICCStatus_t Status;        /**!< Current status of the UICC (valid, blocked, error, empty). */
    char MnoName[32];                 /**!< Mobile Network Operator (MNO) name. */
    char iccid[20];                   /**!< Integrated Circuit Card Identifier (ICCID). */
    char msisdn[20];                  /**!< Mobile Subscriber Integrated Services Digital Network Number (MSISDN). */
} CellularUICCSlotInfoStruct;

/**! Represents cellular signal strength information for a UICC slot. */
typedef  struct
{
    int RSSI;                     /**!< It represents the strength of a signal.
                                       The value ranges from -30 to -90 and is expressed in dBm. */

    int RSRQ;                     /**!< It represents the quality of a received signal.
                                       The value ranges from -3 to -19.5 and is expressed in dBm. */

    int RSRP;                     /**!< It represents the average received power of a single RS resource element.
                                       The value ranges from -140 to -44 and is expressed in dBm. */

    int SNR;                      /**!< It represents the Signal-to-noise ratio.
                                       The value ranges from -20 to 30 and is expressed in dBm. */

    int TXPower;                  /**!< It represents the signal level leaving from that device within the transmitter power range.
                                       The value ranges from 0 to 30 and is expressed in dBm. */
} CellularSignalInfoStruct;

/**!< Represents location information based on a cellular cell. */
typedef struct
{
    unsigned int globalCellId;  /**!< Unique identifier of the cell (0 - (0xFFFFFFFFF)). */
    unsigned int bandInfo;      /**!< Frequency band used by the cell (0 - 0x7FFF). */
    unsigned int servingCellId; /**!< Unique identifier of the serving cell (0 - (0xFFFFFFFFF)). */
} CellLocationInfoStruct;

/**! Represents current Public Land Mobile Network (PLMN) information for a UICC slot. */
typedef struct
{
    char plmn_name[32];                 /**!< PLMN network name. */
    unsigned int MCC;                  /**!< Mobile Country Code (000 - 999). */
    unsigned int MNC;                  /**!< Mobile Network Code (000 - 999). */
    CellularRegistrationStatus_t registration_status; /**!< Device registration status (Registered or Not Registered). */
    CellularModemRegisteredServiceType_t registered_service; /**!< Registered service type (None, PS, CS, or CS & PS). */
    unsigned char roaming_enabled;          /**!< Roaming status (1: enabled, 0: disabled). */
    unsigned int area_code;              /**!< Area code. */
    unsigned long cell_id;                /**!< Cell ID. */
} CellularCurrentPlmnInfoStruct;

/**!< Represents network scan result information. */
typedef struct
{
    char network_name[32];  /**!< Name of the network. */
    unsigned int MCC;       /**!< Mobile Country Code (000 - 999). */
    unsigned int MNC;       /**!< Mobile Network Code (000 - 999). */
    unsigned char network_allowed_flag; /**!< Indicates if the network is allowed (1 = true, 0 = false). */
} CellularNetworkScanResultInfoStruct;

/**! Indicates the detection status of a cellular device. */
typedef enum _CellularDeviceDetectionStatus_t {
    DEVICE_DETECTED = 1, /**!< Device is detected. */
    DEVICE_REMOVED     /**!< Device is removed. */
} CellularDeviceDetectionStatus_t;

/**! Represents the open status of a cellular device. */
typedef enum _CellularDeviceOpenStatus_t {
    DEVICE_OPEN_STATUS_NOT_READY = 1, /**!< Not ready to open. */
    DEVICE_OPEN_STATUS_INPROGRESS,   /**!< Opening in progress. */
    DEVICE_OPEN_STATUS_READY,        /**!< Ready to open. */
} CellularDeviceOpenStatus_t;

/**! Represents the slot status of a cellular device. */
typedef enum _CellularDeviceSlotStatus_t
{
    DEVICE_SLOT_STATUS_NOT_READY = 1, /**!< Slot is not ready. */
    DEVICE_SLOT_STATUS_SELECTING,   /**!< Slot is being selected. */
    DEVICE_SLOT_STATUS_READY,        /**!< Slot is ready for use. */
} CellularDeviceSlotStatus_t;

/**! Represents the Network Attachment Subsystem (NAS) status of a cellular device. */
typedef enum _CellularDeviceNASStatus_t
{
    DEVICE_NAS_STATUS_NOT_REGISTERED = 1, /**!< Not registered. */
    DEVICE_NAS_STATUS_REGISTERING,        /**!< Registering. */
    DEVICE_NAS_STATUS_REGISTERED        /**!< Registered. */
} CellularDeviceNASStatus_t;

/**! Represents the Network Attachment Subsystem (NAS) roaming status of a cellular device. */
typedef enum _CellularDeviceNASRoamingStatus_t {
    DEVICE_NAS_STATUS_ROAMING_OFF = 1, /**!< NAS roaming is off. */
    DEVICE_NAS_STATUS_ROAMING_ON,      /**!< NAS roaming is on. */
} CellularDeviceNASRoamingStatus_t;

/**! Represents the status of a cellular context profile. */
typedef enum _CellularContextProfileStatus_t {
    PROFILE_STATUS_INACTIVE = 1, /**!< Profile is inactive. */
    PROFILE_STATUS_ACTIVE       /**!< Profile is active. */
} CellularContextProfileStatus_t;

/**! Represents the profile selection status of a cellular device. */
typedef enum _CellularDeviceProfileSelectionStatus_t {
    DEVICE_PROFILE_STATUS_NOT_READY = 1, /**!< Profile selection not ready. */
    DEVICE_PROFILE_STATUS_CONFIGURING,  /**!< Profile selection is being configured. */
    DEVICE_PROFILE_STATUS_READY,       /**!< Profile selection is ready. */
    DEVICE_PROFILE_STATUS_DELETED     /**!< Profile has been deleted. */
} CellularDeviceProfileSelectionStatus_t;

/**!
 * @brief Represents the IP readiness status of a cellular device. 
 */
typedef enum _CellularDeviceIPReadyStatus_t {
    DEVICE_NETWORK_IP_NOT_READY = 1, /**!< IP configuration is not ready. */
    DEVICE_NETWORK_IP_READY        /**!< IP configuration is ready. */
} CellularDeviceIPReadyStatus_t; 

/**! Represents the network packet connection status of a cellular device. */
typedef enum _CellularNetworkPacketStatus_t
{
    DEVICE_NETWORK_STATUS_DISCONNECTED = 1, /**!< Network is disconnected. */
    DEVICE_NETWORK_STATUS_CONNECTED      /**!< Network is connected. */
} CellularNetworkPacketStatus_t;

/** @} */  //END OF GROUP CELLULAR_HAL_TYPES
/**
 * @addtogroup CELLULAR_HAL_APIS
 * @{
 */

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/*
 * TODO:
 *
 * 1. Extend the return codes by listing out the possible reasons of failure, to improve the interface in the future.
 *    This was reported during the review for header file migration to opensource github.
 *
 */

/**
* @brief - Returns Modem Device Available Status
*
* @return The status of the operation
* @retval TRUE if modem device presents
* @retval FALSE if modem device not presents
*
*/
unsigned int cellular_hal_IsModemDevicePresent(void);

/**
* @brief - Initialise the Cellular HAL
*
* @param[in] pstCtxInputStruct variable is the Input structure to pass to cellular hal initialization function described by the CellularContextInitInputStruct.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_init(CellularContextInitInputStruct *pstCtxInputStruct);

/**
* @brief - This callback sends to upper layer when after successfully open cellular device context.
*
* @param[in] device_name variable is a character pointer points the modem device name.
*                      \n It is a vendor specific value.
* @param[in] wan_ifname variable is a character pointer points the WAN interface name.
*                      \n Example: "wwan0"
* @param[in] device_open_status variable is from the enumerated datatype.
*                              \n It can hold any value from enum CellularDeviceOpenStatus_t.
* @param[in] modem_operating_mode variable is from the enumerated datatype.
*                              \n It can hold any value from enum CellularModemOperatingConfiguration_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_open_status_api_callback)( char *device_name, char *wan_ifname, CellularDeviceOpenStatus_t device_open_status, CellularModemOperatingConfiguration_t modem_operating_mode );

/**
* @brief - This callback sends to upper layer when after successfully removed modem from device
*
* @param[in] device_name variable is a character pointer points the modem device name.
*                      \n It is a vendor specific value.
* @param[in] device_detection_status variable is from the enumerated datatype.
*                                   \n It can hold any value from enum CellularDeviceDetectionStatus_t.  
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_removed_status_api_callback)( char *device_name, CellularDeviceDetectionStatus_t device_detection_status );

typedef  struct
{
    cellular_device_open_status_api_callback device_open_status_cb;        /**< Open cellular device context.
*                                                                              device_name - variable is a character pointer points the modem device name.
*                                                                               It is a vendor specific value.
*                                                                              wan_ifname - variable is a character pointer points the WAN interface name.
*                                                                               Example: "wwan0"
*                                                                              device_open_status variable is from the enumerated datatype.
*                                                                               Possible values of device_open_status:
*                                                                               It can hold any value from the enum CellularDeviceOpenStatus_t.               
*                                                                              modem_operating_mode variable is from the enumerated datatype.
*                                                                               It can hold any value from the enum CellularModemOperatingConfiguration_t. */

    cellular_device_removed_status_api_callback device_remove_status_cb;    /**< Removed modem from device.
*                                                                               device_name variable is a character pointer points the modem device name.
*                                                                                It is a vendor specific value.
*                                                                               device_detection_status variable is from the enumerated datatype.
*                                                                                Possible values of device_detection_status:
*                                                                                It can hold any value from the enum CellularDeviceDetectionStatus_t. */
} CellularDeviceContextCBStruct;

/**
* @brief - This API inform lower layer to create/open device.
*
* @param[in] pstDeviceCtxCB variable is the structure CellularDeviceContextCBStruct receives function pointers for device open/remove status response from driver.
*                        \n It has two members to open and remove the modem from the device
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_open_device(CellularDeviceContextCBStruct *pstDeviceCtxCB);

/**
* @brief - Returns Modem Control Interface Opened or Not
*
* @return The status of the operation
* @retval TRUE if modem device opened
* @retval FALSE if modem device not opened
*
*/
unsigned char cellular_hal_IsModemControlInterfaceOpened( void );

/**
* @brief - This callback sends to upper layer when after successfully select cellular device slot
*
* @param[in] slot_name variable is a character pointer points the slot name. The buffer size should be atleast 64 bytes long. It is a vendor specific value.
* @param[in] slot_type variable is a character pointer points the slot type. The buffer size should be atleast 64 bytes long. It is a vendor specific value.
* @param[in] slot_num variable is a integer datatype contains the slot num. It is a vendor specific value.
* @param[in] device_slot_status variable is from the enumerated datatype.
*                              \n It can hold any value from the enum CellularDeviceSlotStatus_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_slot_status_api_callback)( char *slot_name, char *slot_type, int slot_num, CellularDeviceSlotStatus_t device_slot_status );

/**
* @brief - This API inform lower layer to select slot for opened device.
*
* @param[in] device_slot_status_cb variable is the function pointer which receives device slot status response from driver.
*                               \n The members of the function cellular_device_slot_status_api_callback are:    
*                               \n slot_name - variable is a character pointer points the slot name. The buffer size should be atleast 64 bytes long. It is a vendor specific value.
*                               \n slot_type - variable is a character pointer points the slot type. The buffer size should be atleast 64 bytes long. It is a vendor specific value.
*                               \n slot_num - variable is a integer datatype contains the slot num. It is a vendor specific value.
*                               \n device_slot_status - variable is from the enumerated datatype.
*                               \n It can hold any value from the enum CellularDeviceSlotStatus_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_select_device_slot(cellular_device_slot_status_api_callback device_slot_status_cb);

/**
* @brief - This API perform to enable/disable SIM power from particular slot
*
* @param[in] slot_id variable is a unsigned integer will intimate to lower layer to slot id to be enable/disable
*                      \n The possible values can be 1 or 2 and depends on number of SIM slots provided by vendor.
* @param[in] enable variable is a unsigned character will intimate to lower layer to enable/disable UICC power.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_sim_power_enable(unsigned int slot_id, unsigned char enable);

/**
* @brief - This API get UICC total slots count from modem
*
* @param[out] total_count variable is a unsigned integer pointer which points the total count of UICC slot.
*                         \n The possible values can be 1 or 2 and depends on number of SIM slots provided by vendor.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_total_no_of_uicc_slots(unsigned int *total_count);

/**
* @brief - This API get UICC slot information from modem
*
* @param[in] slot_index variable is a unsigned integer which contains index of UICC slot. The possible range sould not exceed (2^32)-1.
* @param[out] pstSlotInfo - Pointer to structure CellularUICCSlotInfoStruct that needs to be updated.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_uicc_slot_info(unsigned int slot_index, CellularUICCSlotInfoStruct *pstSlotInfo);

/**
* @brief - This API get current active card status information from modem.
*
* @param[out] card_status variable is from the enumerated datatype CellularUICCStatus_t.
*             \n card_status returns the SIM card status. It can be valid / blocked / error / empty.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_active_card_status(CellularUICCStatus_t *card_status);

/**
* @brief - This callback sends to upper layer when after successfully registered modem with network.
*
* @param[in] device_registration_status variable is from the enumerated datatype CellularDeviceNASStatus_t.
* @param[in] roaming_status variable is from the enumerated datatype CellularDeviceNASRoamingStatus_t.
* @param[in] registered_service variable is from the enumerated datatype CellularModemRegisteredServiceType_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_registration_status_callback)( CellularDeviceNASStatus_t device_registration_status ,
                                                             CellularDeviceNASRoamingStatus_t roaming_status,
                                                             CellularModemRegisteredServiceType_t registered_service );

/**
* @brief - This API inform lower layer to monitor device registration
*
* @param[in] device_registration_status_cb is a function pointer which receives device registration status response from lower layer.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_monitor_device_registration(cellular_device_registration_status_callback device_registration_status_cb);

/**
* @brief - This callback sends to upper layer when after successfully create/modify/select
*
* @param[in] profile_id variable is a character pointer which contains the profile ID. The buffer size should be atleast 64 bytes long. It is a vendor specific value.
* @param[in] PDPType variable is from the enumerated datatype CellularPDPType_t.
* @param[in] device_profile_status variable is from the enumerated datatype CellularDeviceProfileSelectionStatus_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_profile_status_api_callback)( char *profile_id, CellularPDPType_t  PDPType, CellularDeviceProfileSelectionStatus_t device_profile_status );

/**
* @brief - This API inform lower layer to create profile based on valid pstProfileInput. If NULL then select default profile.
*
* @param[in] pstProfileInput is a Profile structure from CellularProfileStruct which needs to pass when creating a profile.
* @param[in] device_profile_status_cb is a function pointer which receives device profile create status response from driver.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_profile_create(CellularProfileStruct *pstProfileInput, cellular_device_profile_status_api_callback device_profile_status_cb);

/**
* @brief - This API inform lower layer to delete profile based on valid pstProfileInput.
*
* @param[in] pstProfileInput is a Profile structure from CellularProfileStruct which needs to pass when deleting a profile.
* @param[in] device_profile_status_cb - The function pointer which receives device profile delete status response from driver.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_profile_delete(CellularProfileStruct *pstProfileInput, cellular_device_profile_status_api_callback device_profile_status_cb);

/**
* @brief - This API inform lower layer to modify profile based on valid pstProfileInput. If NULL then return error.
*
* @param[in] pstProfileInput is a Profile structure from CellularProfileStruct which needs to pass when modifying a profile.
* @param[in] device_profile_status_cb - The function pointer which receives device profile modify status response from driver.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_profile_modify(CellularProfileStruct *pstProfileInput, cellular_device_profile_status_api_callback device_profile_status_cb);

/**
* @brief - This API get list of profiles from Modem
*
* @param[out] ppstProfileOutput - List of profiles needs to be return from the structure CellularProfileStruct.
* @param[out] profile_count variable is a integer pointer, in that total profile count needs to be return.
*                           \n The possible range of acceptable values is 0 to (2^31)-1
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_profile_list(CellularProfileStruct **ppstProfileOutput, int *profile_count);

/**
* @brief - This callback sends to upper layer when after getting packet service status after start network.
*
* @param[in] device_name variable is a character pointer contains the modem device name.
*                    \n The buffer size should be atleast 16 bytes long.
*                    \n It is a vendor specific value.
* @param[in] ip_type - The enum which receives IP configuration.
*                   \n It can hold any value from the enum CellularNetworkIPType_t.
* @param[in] packet_service_status is from the enumerated datatype CellularNetworkPacketStatus_t.
*                               \n It can hold any value from the enum CellularNetworkPacketStatus_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_network_packet_service_status_api_callback)( char *device_name, CellularNetworkIPType_t ip_type, CellularNetworkPacketStatus_t packet_service_status );

/**
* @brief - This callback sends IP information to upper layer when after successfully getting ip configuration from driver
*
* @param[in] pstIPStruct variable is a structure pointer from CellularIPStruct.
* @param[in] ip_ready_status variable is from the enumerated datatype CellularDeviceIPReadyStatus_t.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
typedef int (*cellular_device_network_ip_ready_api_callback)( CellularIPStruct *pstIPStruct, CellularDeviceIPReadyStatus_t ip_ready_status );

typedef  struct
{
    cellular_device_network_ip_ready_api_callback device_network_ip_ready_cb;        /**< To get IP configuration from driver.
*                                                                                        pstIPStruct - variable is a structure pointer from CellularIPStruct.
*                                                                                        ip_ready_status - variable is from the enumerated datatype.
*                                                                                         It can hold any value from the enum CellularDeviceIPReadyStatus_t. */
    cellular_network_packet_service_status_api_callback packet_service_status_cb;    /**< Packet service status after start network.
*                                                                                        device_name - variable is a character pointer contains the modem device name.
*                                                                                         The buffer size should be atleast 16 bytes long.
*                                                                                         It is a vendor specific value.
*                                                                                        ip_type - The enum which receives IP configuration.
*                                                                                         It can hold any value from the enum CellularNetworkIPType_t.
*                                                                                        packet_service_status is from the enumerated datatype CellularNetworkPacketStatus_t
*                                                                                         It can hold any value from the enum CellularNetworkPacketStatus_t. */
} CellularNetworkCBStruct;

/**
* @brief - This API inform lower layer to start network based on IP Type and Passed profile input. If NULL then start based on default profile.
*
* @param[in] ip_request_type variable is from the enumerated datatype CellularNetworkIPType_t which receives IP configuration for stopped network from driver.
* @param[in] pstProfileInput - Here needs to pass profile from structure CellularProfileStruct to start network. If NULL then it should take it default profile otherwise start based on input.
* @param[in] pstCBStruct - Here needs to fill CB function pointer for packet and ip status from the structure CellularNetworkCBStruct.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_start_network( CellularNetworkIPType_t ip_request_type, CellularProfileStruct *pstProfileInput, CellularNetworkCBStruct *pstCBStruct );

/**
* @brief - This API inform lower layer to stop network based on valid ip request type.
*
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_stop_network(CellularNetworkIPType_t ip_request_type);

/**
* @brief - This API get current signal information from Modem
*
* @param[out] signal_info variable is a pointer, needs to parse CellularSignalInfoStruct structure to get signal information.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_signal_info(CellularSignalInfoStruct *signal_info);

/**
* @brief - This API gets cell location information. 
*
* @param[out] loc_info variable is a pointer, needs to parse CellLocationInfoStruct structure to get location information.                          
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*/
int cellular_hal_get_cell_location_info(CellLocationInfoStruct *loc_info);

/**
* @brief - This API inform lower layer to configure modem operating mode.
* @param[in] modem_operating_config variable is from the enumerated datatype needs to pass CellularModemOperatingConfiguration_t to configure modem state.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_set_modem_operating_configuration(CellularModemOperatingConfiguration_t modem_operating_config);

/**
* @brief - Returns Modem Device IMEI information
*
* @param[out] imei variable is a character pointer, needs to return Modem IMEI value on this input.
*                  \n The buffer size should be atleast 16 bytes long. Example: "010928/00/389023/36".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_device_imei ( char *imei );

/**
* @brief - Returns Modem Device IMEI Software Version
*
* @param[out] imei_sv variable is a character pointer, needs to return Modem IMEI Software Version value on this input.
*                     \n The buffer size should be atleast 16 bytes long. Example: 36.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_device_imei_sv ( char *imei_sv );

/**
* @brief - Returns Modem Device Current ICCID Information
*
* @param[out] iccid variable is a character pointer, needs to return currently choosed ICCID value on this input.
*                   \n The buffer size should be atleast 21 bytes long. Example: "8901260410032962638F".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_current_iccid ( char *iccid );

/**
* @brief - Returns Modem Device Current MSISDN Information
*
* @param[out] msisdn variable is a character pointer, needs to return currently choosed MSISDN value on this input.
*                    \n The buffer size should be atleast 20 bytes long. Example: "9386720110".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_current_msisdn ( char *msisdn );

/**
* @brief - This API get current network packet statistics from modem
*
* @param[out] network_packet_stats variable is a pointer, needs to parse CellularPacketStatsStruct structure to get packet statistics information.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_packet_statistics( CellularPacketStatsStruct *network_packet_stats );

/**
* @brief - This API get current modem registration status
*
* @param[out] status variable is a pointer to the structure CellularInterfaceStatus_t, needs to assign modem current registration status.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_current_modem_interface_status( CellularInterfaceStatus_t *status );

/**
* @brief - This API to attach modem with network registration
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_set_modem_network_attach( void );

/**
* @brief - This API to detach modem with network registration
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_set_modem_network_detach( void );

/**
* @brief - This API get current firmware version of modem
*
* @param[out] firmware_version variable is a character pointer contains firmware version of modem.
*                                \n The buffer size should be atleast 128 bytes long. Example: "v2.1.3".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_firmware_version(char *firmware_version);

/**
* @brief - This API get current plmn information from modem
*
* @param[in] plmn_info is a pointer to structure CellularCurrentPlmnInfoStruct that receives function pointers for current plmn network information response from modem.
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_current_plmn_information(CellularCurrentPlmnInfoStruct *plmn_info);

/**
* @brief - This API get current active card status information from modem
*
* @param[out] network_info is a structure CellularNetworkScanResultInfoStruct filled with available networks information from Modem.
* @param[out] total_network_count variable is a unsigned integer pointer filled with total no of available networks.
*                                   \n The possible range of acceptable values is 0 to 1100000. Example: 5
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_available_networks_information(CellularNetworkScanResultInfoStruct **network_info, unsigned int *total_network_count);

/**
* @brief - Returns Modem preferred Radio Technologies
*
* @param[out] preferred_rat variable is a character array contains preferred technology.
*                             \n The buffer size sholud be atleast 128 bytes long.
*                             \n Possible combination of strings: AUTO, CDMA20001X, EVDO, GSM, UMTS, LTE
*                             \n Example: "{UMTS,LTE / WCDMA,LTE}".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_preferred_radio_technology( char *preferred_rat );

/**
* @brief - sets Modem preferred Radio Technologies
*
* @param[in] preferred_rat variable is a character array contains preferred technology. Should be part of supported RAT otherwise AUTO will be set.
*                            \n The buffer size sholud be atleast 128 bytes long.
*                            \n Possible combination of strings: AUTO, CDMA20001X, EVDO, GSM, UMTS, LTE
*                            \n Example: "{LTE / AUTO}".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_set_modem_preferred_radio_technology( char *preferred_rat );

/**
* @brief - Returns Modem current Radio Technologies
*
* @param[out] current_rat variable is a character pointer contains current technology used for data.
*                             \n The buffer size sholud be atleast 128 bytes long.
*                             \n Possible strings: AUTO, CDMA20001X, EVDO, GSM, UMTS, LTE
*                             \n Example: "LTE".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_current_radio_technology( char *current_rat );

/**
* @brief - Returns Modem supported Radio access Technologies
*
* @param[out] supported_rat variable is a character pointer contains information about supported RAT.
*                             \n The buffer size sholud be atleast 128 bytes long.
*                             \n Possible combination of strings: AUTO, CDMA20001X, EVDO, GSM, UMTS, LTE
*                             \n Example: "UMTS, LTE".
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_get_modem_supported_radio_technology ( char *supported_rat );

/**
* @brief - This API to factory reset the modem
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_modem_factory_reset( void );

/**
* @brief - This API to reset the modem
*
* @return The status of the operation
* @retval RETURN_OK if successful
* @retval RETURN_ERR if any error is detected
*
*/
int cellular_hal_modem_reset( void );
/** @} */  //END OF GROUP CELLULAR_HAL_APIS
#endif //_CELLULAR_HAL_H_
