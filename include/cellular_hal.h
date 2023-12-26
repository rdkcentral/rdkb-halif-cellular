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
/**
 * @brief Represents the status of cellular interface.
 *
 * It consists of seven members with each representing a different status for the interface.
 *
 */
typedef enum _CellularInterfaceStatus_t {
    IF_UP = 1,           /**< The interface is up and running. */
    IF_DOWN,             /**< The interface is down and not running. */
    IF_UNKNOWN,          /**< The status of the interface is unknown. */
    IF_DORMANT,          /**< The status of the interface is dormant. */
    IF_NOTPRESENT,       /**< The status of the interface is not present. */
    IF_LOWERLAYERDOWN,   /**< The lower layer of the interface is down. */  
    IF_ERROR             /**< Error in the interface. */
}CellularInterfaceStatus_t;

/**
 * @brief Represents the IP family for the cellular interface.
 *
 * It consists of four members with each representing a different preferred IP family.
 *
 */
typedef enum _CellularIpFamilyPref_t {
    IP_FAMILY_UNKNOWN = 1,   /**< The preferred IP family is unknown. */
    IP_FAMILY_IPV4,          /**< The preferred IP family is IPV4. */
    IP_FAMILY_IPV6,          /**< The preferred IP family is IPV6. */
    IP_FAMILY_IPV4_IPV6      /**< The preferred IP family is either IPV4 or IPV6. */
}CellularIpFamilyPref_t;

/**
 * @brief Represents the Access Technology for the cellular interface.
 *
 * It consists of eight members with each representing a different preferred access technology.
 *
 */
typedef enum _CellularPrefAccessTechnology_t {
    PREF_GPRS          = 1,                     /**< GSM with GPRS. */
    PREF_EDGE,                                  /**< GSM with EDGE. */
    PREF_UMTS,                                  /**< UMTS. */
    PREF_UMTSHSPA,                              /**< 3GPP-HSPA. */
    PREF_CDMA2000OneX,                          /**< CDMA2000OneX. */
    PREF_CDMA2000HRPD,                          /**< CDMA2000HRPD. */
    PREF_LTE,                                   /**< LTE. */
    PREF_NR                                     /**< 5G New Radio. */
}CellularPrefAccessTechnology_t;

/**
 * @brief Represents the Packet Data Protocol types for the cellular interface.
 *
 * It represents the type of IP address assigned to a Packet Data Protocol context in cellular communication protocols. 
 *
 */
typedef enum _CellularPDPType_t
{
    CELLULAR_PDP_TYPE_IPV4         = 0,    /**< IP address assigned to the PDP context is an IPv4 address. */    
    CELLULAR_PDP_TYPE_PPP,                 /**< IP address assigned to the PDP context is a PPP(Point-to-point Protocol) address. */
    CELLULAR_PDP_TYPE_IPV6,                /**< IP address assigned to the PDP context is an IPv6 address. */
    CELLULAR_PDP_TYPE_IPV4_OR_IPV6         /**< IP address assigned to the PDP context is either IPv4 or IPV6 address. */
}CellularPDPType_t;

/**
 * @brief Represents the Packet Data Protocol authentication types for the cellular interface.
 *
 * It consists of 3 members with each representing a different type of authentication. 
 *
 */
typedef enum _CellularPDPAuthentication_t
{
    CELLULAR_PDP_AUTHENTICATION_NONE    = 0,    /**< No authentication is used for the PDP context. */
    CELLULAR_PDP_AUTHENTICATION_PAP ,           /**< Password Authentication Protocol authentication is used for the PDP context. */
    CELLULAR_PDP_AUTHENTICATION_CHAP,           /**< Challenge-Handshake Authentication Protocol is used for the PDP context. */
}CellularPDPAuthentication_t;
/**
 * @brief Represents the type of cellular profile for the cellular interface.
 *
 * It consists of two members with each representing a different type of cellular profile.
 *
 */
typedef enum _CellularProfileType_t
{
    CELLULAR_PROFILE_TYPE_3GPP    = 0,    /**< Cellular profile is of type 3GPP. */
    CELLULAR_PROFILE_TYPE_3GPP2           /**< Cellular profile is of type 3GPP2. */
} CellularProfileType_t;

/**
 * @brief Represents the different types of network configuration for the cellular interface.
 *
 * It provides the network configuration for a Packet Data Protocol (PDP) context in cellular communication protocols.
 *
 */
typedef enum _CellularPDPNetworkConfig_t
{
   CELLULAR_PDP_NETWORK_CONFIG_NAS = 1,    /**< Network is provided by the Network Attachment Subsystem. */
   CELLULAR_PDP_NETWORK_CONFIG_DHCP        /**< Network is provided by the Dynamic Host Configuration Protocol. */
}
CellularPDPNetworkConfig_t;

/**
 * @brief Represents the modem operating configurations.
 *
 * It consists of two members with each representing a different type of cellular profile.
 *
 */
typedef enum _CellularModemOperatingConfiguration_t
{
    CELLULAR_MODEM_SET_ONLINE     = 1,    /**< Set the modem to online mode. */
    CELLULAR_MODEM_SET_OFFLINE,           /**< Set the modem to offline mode. */
    CELLULAR_MODEM_SET_LOW_POWER_MODE,    /**< Set the modem to low power mode. */
    CELLULAR_MODEM_SET_RESET,             /**< Reset the modem. */
    CELLULAR_MODEM_SET_FACTORY_RESET      /**< Reset the modem to factory settings. */
} CellularModemOperatingConfiguration_t;

/**
 * @brief Represents the registered service types of cellular modem.
 *
 * It consists of four members with each representing a different registered service type.
 *
 */
typedef enum _CellularModemRegisteredServiceType_t
{
   CELLULAR_MODEM_REGISTERED_SERVICE_NONE = 0,    /**< No service is registered with the modem. */
   CELLULAR_MODEM_REGISTERED_SERVICE_PS,          /**< Packet-switched service is registered with the modem. */
   CELLULAR_MODEM_REGISTERED_SERVICE_CS,          /**< Circuit-switched service is registered with the modem. */
   CELLULAR_MODEM_REGISTERED_SERVICE_CS_PS        /**< Circuit-switched and Packet-switched service is registered with the modem. */
}
CellularModemRegisteredServiceType_t;

/**
 * @brief Represents the registration of the device in the cellular interface.
 *
 * It consists of two members with each representing the registration status of the device.
 *
 */
typedef enum _CellularRegistrationStatus_t
{
   DEVICE_REGISTERED = 1,        /**< Device is registered. */
   DEVICE_NOT_REGISTERED,        /**< Device is not registered. */
}CellularRegistrationStatus_t;

/**
 * @brief Represents the members of the cellular profile.
 *
 * It consists of all the members that are responsible for the profile of the cellular interface.
 *
 */
typedef  struct
{
    int ProfileID;                                   /**< @brief It represents the profile ID.
                                                         It is of integer datatype. 
                                                         The value should not exceed (2^31)-1. */   
    
    CellularProfileType_t ProfileType;               /**< @brief It represents the profile type.
                                                         Is from the enumerated datatype CellularProfileType_t. 
                                                         Possible value is 0 and 1. */
    
    int PDPContextNumber;                            /**< @brief It represents the Packet Data Protocol number.
                                                         It is of integer datatype. 
                                                         The value should not exceed (2^31)-1. */
    
    CellularPDPType_t PDPType;                       /**< @brief It represents the Packet Data Protocol type.
                                                         Is from the enumerated datatype CellularPDPType_t. 
                                                         Possible value is 0,1,2 and 3. */                      
    
    CellularPDPAuthentication_t PDPAuthentication;   /**< @brief It represents the Packet Data Protocol authentication type.
                                                         Is from the enumerated datatype CellularPDPAuthentication_t. 
                                                         Possible value is 0,1 and 2. */
    
    CellularPDPNetworkConfig_t PDPNetworkConfig;     /**< @brief It represents the Packet Data Protocol network configuration type.
                                                         Is from the enumerated datatype CellularPDPNetworkConfig_t. 
                                                         Possible value is 1 and 2. */
    
    char ProfileName[64];                            /**< @brief It represents the name of the profile.
                                                         It is a character array.
                                                         It is a vendor specific value. */
                      
    char APN[64];                                    /**< @brief It represents the Access Point Name of the gateway.
                                                         It is a character array.
                                                         It is a vendor specific value. */

    char Username[256];                              /**< @brief It represents the username.
                                                         It is a character array.
                                                         It is a vendor specific value. */
                              
    char Password[256];                              /**< @brief It represents the password.
                                                         It is a character array.
                                                         It is a vendor specific value. */

    char Proxy[45];                                  /**< @brief It stores the IP address of the proxy server that the device uses to connect to network.
                                                         It is a character array.
                                                         It is a vendor specific value. */

    unsigned int ProxyPort;                          /**< @brief It stores the port number that the device should use to connect to the proxy server.
                                                         It is a character array.
                                                         The buffer size should not exceed the value 65535.
                                                         It is a vendor specific value. */

    unsigned char bIsNoRoaming;                      /**< @brief It indicates whether the device is currently roaming on another network.
                                                         It is of unsigned char datatype. */

    unsigned char bIsAPNDisabled;                    /**< @brief It indicates whether the device’s access point name is disabled.
                                                         It is of unsigned char datatype. */

    unsigned char bIsThisDefaultProfile;             /**< @brief It indicates whether the current configuration is the default profile for the device.
                                                         It is of unsigned char datatype. */
} CellularProfileStruct;

/**
 * @brief Represents the members of the cellular context input.
 *
 * It consists of all the members that are responsible for the initialization of cellular interface.
 *
 */
typedef  struct
{
    CellularIpFamilyPref_t enIPFamilyPreference;                 /**< @brief It represents IP family for the cellular interface.
                                                                     Is from the enumerated datatype CellularIpFamilyPref_t. 
                                                                     Possible value is 1,2,3 and 4. */

    CellularProfileStruct stIfInput;                             /**< @brief It represents the various interface inputs.
                                                                     Is from the structure CellularProfileStruct. 
                                                                     Possible values include APN, username, password etc. */
                 
    CellularPrefAccessTechnology_t enPreferenceTechnology;       /**< @brief It represents the Access Technology for the cellular interface.
                                                                     Is from the enumerated datatype CellularPrefAccessTechnology_t. 
                                                                     Possible value is 1,2,3,4,5,6,7,and 8. */
} CellularContextInitInputStruct;

/**
 * @brief Represents the type of IP address that a cellular network uses.
 *
 * It consists of four members in the context of cellular networks to specify the type of IP address that is used by the network.
 */
typedef enum _CellularNetworkIPType_t
{
    CELLULAR_NETWORK_IP_FAMILY_UNKNOWN     = 0,    /**< The preferred IP family is unknown. */
    CELLULAR_NETWORK_IP_FAMILY_IPV4,               /**< The preferred IP family is IPV4. */
    CELLULAR_NETWORK_IP_FAMILY_IPV6,               /**< The preferred IP family is IPV6. */
    CELLULAR_NETWORK_IP_FAMILY_UNSPECIFIED         /**< The preferred IP family is not specified. */
} CellularNetworkIPType_t;

/**
 * @brief Represents the cellular network connections.
 *
 * It consists of members used to store information about a cellular network connection.
 */
typedef  struct
{
    char WANIFName[16];                       /**< @brief It represents the name of the WAN interface.
                                                  It is a character array.
                                                  Example: "wwan0" */

    char IPAddress[128];                      /**< @brief It represents IP address of the device.
                                                  It is a character array.
                                                  Example: "192.168.1.10" */

    CellularNetworkIPType_t IPType;           /**< @brief It represents the type of IP address that the cellular network uses.
                                                  Is from the enumerated datatype CellularNetworkIPType_t.
                                                  Possible value is 0,1,2 and 3. */

    char SubnetMask[128];                     /**< @brief It represents the subnet mask of the device.
                                                  It is a character array.
                                                  Example: "255.255.255.0" */

    char DefaultGateWay[128];                 /**< @brief It represents the IP address of the default gateway.
                                                  It is a character array.
                                                  Example: "192.168.1.1" */

    char DNSServer1[128];                     /**< @brief It represents the IP address of the primary DNS server.
                                                  It is a character array.
                                                  Example: "8.8.8.8" */

    char DNSServer2[128];                     /**< @brief It represents the IP address of the secondary DNS server.
                                                  It is a character array.
                                                  Example:  "1.1.1.1" */

    char Domains[256];                        /**< @brief It represents the domain names that the device is associated with.
                                                  It is a character array.
                                                  Example:  "hsd.pa.crnrstn.comcast.net" */

    unsigned int MTUSize;                     /**< @brief It represents stores the maximum transmission unit size of the device.
                                                  It is an unsigned integer.
                                                  The possible range of acceptable values is 1280 to 9000. */
} CellularIPStruct;

/**
 * @brief Represents the data transfer statistics of a cellular network connection.
 *
 * It consists of members used to store information about the number of packets and bytes sent and received over a cellular network connection.
 */
typedef  struct
{
    unsigned long BytesSent;                      /**< @brief It represents the total number of bytes sent over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long BytesReceived;                  /**< @brief It represents the total number of bytes received over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long PacketsSent;                    /**< @brief It represents the total number of packets sent over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long PacketsReceived;                /**< @brief It represents the total number of packets received over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long PacketsSentDrop;                /**< @brief It represents  the total number of packets that were dropped while being sent over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long PacketsReceivedDrop;            /**< @brief It represents that stores the total number of packets that were dropped while being received over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long UpStreamMaxBitRate;             /**< @brief It represents the maximum bit rate for upstream data transfer over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */

    unsigned long DownStreamMaxBitRate;           /**< @brief It represents the maximum bit rate for downstream data transfer over the cellular network connection.
                                                      It is of unsigned long integer datatype. 
                                                      The value should not exceed (2^31)-1. */
} CellularPacketStatsStruct;

/**
 * @brief Represents the form factors of the UICC.
 *
 * It consists of four members used to represent the form factor of a Universal Integrated Circuit Card used in cellular devices. 
 */
typedef enum _CellularUICCFormFactor_t
{
    CELLULAR_UICC_FORM_FACTOR_1FF     = 0,        /**< First form factor of the UICC also known as the full-size SIM card. */
    CELLULAR_UICC_FORM_FACTOR_2FF,                /**< Second form factor of the UICC also known as the mini SIM card. */
    CELLULAR_UICC_FORM_FACTOR_3FF,                /**< Third form factor of the UICC also known as the micro SIM card. */
    CELLULAR_UICC_FORM_FACTOR_4FF                 /**< First form factor of the UICC also known as the nano SIM card. */
} CellularUICCFormFactor_t;

/**
 * @brief Represents the current SIM status.
 *
 * It consists of four members used to represent the status of a Universal Integrated Circuit Card used in cellular devices.
 */
typedef enum _CellularUICCStatus_t
{
    CELLULAR_UICC_STATUS_VALID     = 0,        /**<  UICC is valid and can be used for cellular communication. */
    CELLULAR_UICC_STATUS_BLOCKED,              /**<  UICC is blocked and cannot be used for cellular communication. */
    CELLULAR_UICC_STATUS_ERROR,                /**<  UICC is error state and cannot be used for cellular communication. */
    CELLULAR_UICC_STATUS_EMPTY                 /**<  UICC is empty and has no data. */
} CellularUICCStatus_t;

/**
 * @brief Represents the type of application UICC uses.
 *
 * It consists of three members used to represent type of Universal Integrated Circuit Card application used in cellular devices.
 */
typedef enum _CellularUICCApplication_t
{
    CELLULAR_UICC_APPLICATION_USIM     = 0,        /**< Universal Subscriber Identity Module application used in 3G and 4G cellular networks. */
    CELLULAR_UICC_APPLICATION_ISIM,                /**< IP Multimedia Services Identity Module application used in IMS networks. */
    CELLULAR_UICC_APPLICATION_ESIM                 /**< Embedded SIM application used in devices that support remote SIM provisioning. */
} CellularUICCApplication_t;

/**
 * @brief Represents the UICC slot informations.
 *
 * It consists of structure members used to represent Universal Integrated Circuit Card slot informations.
 */
typedef  struct
{
    unsigned char SlotEnable;                    /**< @brief It represents the slot status.
                                                     It is of unsigned character datatype. */

    unsigned char IsCardPresent;                 /**< @brief It represents the card status.
                                                     It is of unsigned character datatype. */

    unsigned char CardEnable;                    /**< @brief It represents the slot status.
                                                     It is of unsigned character datatype. */

    CellularUICCFormFactor_t FormFactor;         /**< @brief It represents the form factor of a Universal Integrated Circuit Card used in cellular devices.
                                                     Is from the enumerated datatype CellularUICCFormFactor_t.
                                                     Possible value is 0,1,2 and 3. */

    CellularUICCApplication_t Application;       /**< @brief It represents type of Universal Integrated Circuit Card application used in cellular devices.
                                                     Is from the enumerated datatype CellularUICCApplication_t.
                                                     Possible value is 0,1 and 2. */

    CellularUICCStatus_t Status;                 /**< @brief It represents the status of a Universal Integrated Circuit Card used in cellular devices.
                                                     Is from the enumerated datatype CellularUICCStatus_t.
                                                     Possible value is 0,1,2 and 3. */

    char MnoName[32];                            /**< @brief It represents the mobile newtwork operator name who provides the service.
                                                     It is a vendor specific value. */

    char iccid[20];                              /**< @brief It represents the Integrated Circuit Card id.
                                                     It is a vendor specific value. */

    char msisdn[20];                             /**< @brief It a number uniquely identifying a subscription in a Global System for Mobile communications.
                                                     It is a vendor specific value. */
} CellularUICCSlotInfoStruct;


/**
 * @brief Represents the UICC slot informations.
 *
 * It consists of structure members used to represent Universal Integrated Circuit Card slot informations.
 */
typedef  struct
{
    int RSSI;                     /**< @brief It represents the strength of a signal.
                                      It is of integer datatype.
                                      The value ranges from -30 dBm to -90 dBm. */

    int RSRQ;                     /**< @brief It represents the quality of a received signal.
                                      It is of integer datatype.
                                      The value ranges from -3 dB to -19.5 dB. */

    int RSRP;                     /**< @brief It represents the average received power of a single RS resource element.
                                      It is of integer datatype.
                                      The value ranges from -140 dBm to -44 dBm. */

    int SNR;                      /**< @brief It represents the Signal-to-noise ratio.
                                      It is of integer datatype.
                                      The value ranges from -20 dB to 30 dB. */

    int TXPower;                  /**< @brief It represents the signal level leaving from that device within the transmitter power range.
                                      It is of integer datatype.
                                      The value ranges from 0 dBm to 30 dBm. */
} CellularSignalInfoStruct;

/**
 * @brief Represents the cellular location informations.
 *
 * It consists of structure members used to represent location informations.
 */
typedef struct
{
    unsigned int globalCellId;          /**< @brief It represents the unique identifier of the cell in the network.
                                            It is of unsigned integer datatype.
                                            The value ranges from 0 to 68719476735. */

    unsigned int bandInfo;              /**< @brief It represents the frequency band used by the cell.
                                            It is of unsigned integer datatype.
                                            The value ranges from 0 to 32767. */

    unsigned int servingCellId;         /**< @brief It represents the unique identifier of the serving cell.
                                            It is of unsigned integer datatype.
                                            The value ranges from 0 to 68719476735. */
} CellLocationInfoStruct;

/**
 * @brief Represents the UICC slot informations.
 *
 * It consists of structure members used to represent Universal Integrated Circuit Card slot informations.
 */
typedef  struct
{
    char plmn_name[32];                                            /**< @brief It represents the plmn network information name.
                                                                       It is of character array datatype. */

    unsigned int MCC;                                              /**< @brief It represents the mobile country code.
                                                                       It is of unsigned integer datatype.
                                                                       MCC is a 3 digit number ranges from 000 to 999. */

    unsigned int MNC;                                              /**< @brief It represents the mobile network code.
                                                                       It is of unsigned integer datatype.
                                                                       MNC is a 3 digit number ranges from 000 to 999. */

    CellularRegistrationStatus_t registration_status;              /**< @brief It represents the registration status.
                                                                       Is from the enumerated datatype CellularRegistrationStatus_t.
                                                                       Possible value is 1 and 2. */

    CellularModemRegisteredServiceType_t registered_service;       /**< @brief It represents registered service types of cellular modem.
                                                                       Is from the enumerated datatype CellularModemRegisteredServiceType_t.
                                                                       Possible value is 0,1,2 and 3. */

    unsigned char roaming_enabled;                                 /**< @brief It represents whether the roaming is enabled or not.
                                                                       It is of unsigned character datatype.
                                                                       It should not exceed (2^8)-1. */

    unsigned int area_code;                                        /**< @brief It represents the area code.
                                                                       It is of unsigned integer datatype.
                                                                       It should not exceed (2^31)-1. */

    unsigned long cell_id;                                         /**< @brief It represents the cell id.
                                                                       It is of unsigned long integer datatype.
                                                                       It should not exceed (2^31)-1. */
} CellularCurrentPlmnInfoStruct;

/**
 * @brief Represents the scan results information of the network.
 *
 * It consists of structure members used to represent the network scan results information.
 */
typedef  struct
{
    char network_name[32];                    /**< @brief It represents the name of the network.
                                                  It is of character array datatype. */

    unsigned int MCC;                         /**< @brief It represents the mobile country code.
                                                  It is of unsigned integer datatype.
                                                  MCC is a 3 digit number ranges from 000 to 999. */

    unsigned int MNC;                         /**< @brief It represents the mobile network code.
                                                  It is of unsigned integer datatype.
                                                  MNC is a 3 digit number ranges from 000 to 999. */

    unsigned char network_allowed_flag;       /**< @brief It represents if the network is allowed or not.
                                                  It is of unsigned character datatype. */
} CellularNetworkScanResultInfoStruct;

/**
 * @brief Represents the detection status of the cellular device.
 *
 * It consists of two members used to represent whether the cellular device is detected or not.
 */
typedef enum _CellularDeviceDetectionStatus_t
{
   DEVICE_DETECTED = 1,        /**< Cellular device detected. */
   DEVICE_REMOVED,             /**< Cellular device removed. */
}CellularDeviceDetectionStatus_t;

/**
 * @brief Represents the open status of the cellular device.
 *
 * It consists of three members used to represent the open status of the cellular device.
 */
typedef enum _CellularDeviceOpenStatus_t
{
   DEVICE_OPEN_STATUS_NOT_READY = 1,        /**< Cellular device open status not ready. */
   DEVICE_OPEN_STATUS_INPROGRESS,           /**< Cellular device open status in progress. */
   DEVICE_OPEN_STATUS_READY,                /**< Cellular device open status ready. */
}CellularDeviceOpenStatus_t;

/**
 * @brief Represents the slot status of the cellular device.
 *
 * It consists of three members used to represent the slot status of the cellular device.
 */
typedef enum _CellularDeviceSlotStatus_t
{
   DEVICE_SLOT_STATUS_NOT_READY = 1,        /**< Cellular device slot status not ready. */
   DEVICE_SLOT_STATUS_SELECTING,            /**< Cellular device slot status selecting. */
   DEVICE_SLOT_STATUS_READY,                /**< Cellular device slot status ready. */
} CellularDeviceSlotStatus_t;

/**
 * @brief Represents the NAS status of the cellular device.
 *
 * It consists of three members used to represent the NAS status of the cellular device.
 */
typedef enum _CellularDeviceNASStatus_t
{
   DEVICE_NAS_STATUS_NOT_REGISTERED = 1,        /**< Cellular device NAS status not registered. */
   DEVICE_NAS_STATUS_REGISTERING,               /**< Cellular device NAS status not registering. */
   DEVICE_NAS_STATUS_REGISTERED,                /**< Cellular device NAS status registered. */
} CellularDeviceNASStatus_t;

/**
 * @brief Represents the NAS roaming status of the cellular device.
 *
 * It consists of two members used to represent the NAS roaming status of the cellular device.
 */
typedef enum _CellularDeviceNASRoamingStatus_t
{
   DEVICE_NAS_STATUS_ROAMING_OFF = 1,        /**< Cellular device NAS roaming off. */
   DEVICE_NAS_STATUS_ROAMING_ON,             /**< Cellular device NAS roaming on. */
} CellularDeviceNASRoamingStatus_t;

/**
 * @brief Represents the context profile status of the cellular device.
 *
 * It consists of two members used to represent the context profile status of the cellular device.
 */
typedef enum _CellularContextProfileStatus_t
{
   PROFILE_STATUS_INACTIVE = 1,        /**< Cellular device context profile status inactive. */
   PROFILE_STATUS_ACTIVE,              /**< Cellular device context profile status active. */
} CellularContextProfileStatus_t;

/**
 * @brief Represents the profile status of the cellular device.
 *
 * It consists of four members used to represent the profile status of the cellular device.
 */
/** Cellular Device Profile Selection Status */
typedef enum _CellularDeviceProfileSelectionStatus_t
{
   DEVICE_PROFILE_STATUS_NOT_READY = 1,        /**< Cellular device profile status not ready. */
   DEVICE_PROFILE_STATUS_CONFIGURING,          /**< Cellular device profile status configuring. */
   DEVICE_PROFILE_STATUS_READY,                /**< Cellular device profile status ready. */
   DEVICE_PROFILE_STATUS_DELETED               /**< Cellular device profile status deleted. */
} CellularDeviceProfileSelectionStatus_t;

/**
 * @brief Represents the IP ready status of the cellular device.
 *
 * It consists of two members used to represent the IP status of the cellular device.
 */
typedef enum _CellularDeviceIPReadyStatus_t
{
   DEVICE_NETWORK_IP_NOT_READY = 1,        /**< Cellular device IP not ready. */
   DEVICE_NETWORK_IP_READY,                /**< Cellular device IP ready. */
} CellularDeviceIPReadyStatus_t;

/**
 * @brief Represents the network packet status of the cellular device.
 *
 * It consists of two members used to represent the network packet status of the cellular device.
 */
typedef enum _CellularNetworkPacketStatus_t
{
   DEVICE_NETWORK_STATUS_DISCONNECTED = 1,        /**< Cellular device network status disconnected. */
   DEVICE_NETWORK_STATUS_CONNECTED,               /**< Cellular device network status connected. */
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
* @param[in] device_open_status_cb is a structure member of cellular_device_open_status_api_callback function.
* @param[in] device_remove_status_cb is a structure member of cellular_device_removed_status_api_callback function.
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
* @param[in] device_registration_status variable is from the enumerated datatype CellularDeviceNASStatus_t.
* @param[in] roaming_status variable is from the enumerated datatype CellularDeviceNASRoamingStatus_t.
* @param[in] registered_service variable is from the enumerated datatype CellularModemRegisteredServiceType_t.
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
* @param[in] device_network_ip_ready_cb is from cellular_device_network_ip_ready_api_callback function.
* @param[in] packet_service_status_cb variable is from cellular_network_packet_service_status_api_callback function.
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
* @param[in] iprequest_type variable is from the enumerated datatype CellularNetworkIPType_t which receives IP configuration for stopped network from driver.
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
* @param[out] CellularNetworkScanResultInfoStruct is a structure CellularNetworkScanResultInfoStruct filled with available networks information from Modem.
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