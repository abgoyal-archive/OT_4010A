
#ifndef __HCI_H
#define __HCI_H

#ifdef __cplusplus
#endif
#include <sys/socket.h>
#include <sys/ioctl.h>
#define HCI_MAX_DEV 16
#define HCI_MAX_ACL_SIZE 1024
#define HCI_MAX_SCO_SIZE 255
#define HCI_MAX_EVENT_SIZE 260
#define HCI_MAX_FRAME_SIZE (HCI_MAX_ACL_SIZE + 4)
#define HCI_DEV_REG 1
#define HCI_DEV_UNREG 2
#define HCI_DEV_UP 3
#define HCI_DEV_DOWN 4
#define HCI_DEV_SUSPEND 5
#define HCI_DEV_RESUME 6
#define HCI_VIRTUAL 0
#define HCI_USB 1
#define HCI_PCCARD 2
#define HCI_UART 3
#define HCI_RS232 4
#define HCI_PCI 5
#define HCI_SDIO 6
#define HCIDEVUP _IOW('H', 201, int)
#define HCIDEVDOWN _IOW('H', 202, int)
#define HCIDEVRESET _IOW('H', 203, int)
#define HCIDEVRESTAT _IOW('H', 204, int)
#define HCIGETDEVLIST _IOR('H', 210, int)
#define HCIGETDEVINFO _IOR('H', 211, int)
#define HCIGETCONNLIST _IOR('H', 212, int)
#define HCIGETCONNINFO _IOR('H', 213, int)
#define HCIGETAUTHINFO _IOR('H', 215, int)
#define HCISETRAW _IOW('H', 220, int)
#define HCISETSCAN _IOW('H', 221, int)
#define HCISETAUTH _IOW('H', 222, int)
#define HCISETENCRYPT _IOW('H', 223, int)
#define HCISETPTYPE _IOW('H', 224, int)
#define HCISETLINKPOL _IOW('H', 225, int)
#define HCISETLINKMODE _IOW('H', 226, int)
#define HCISETACLMTU _IOW('H', 227, int)
#define HCISETSCOMTU _IOW('H', 228, int)
#define HCISETSECMGR _IOW('H', 230, int)
#define HCIINQUIRY _IOR('H', 240, int)
#ifndef __NO_HCI_DEFS
#define HCI_COMMAND_PKT 0x01
#define HCI_ACLDATA_PKT 0x02
#define HCI_SCODATA_PKT 0x03
#define HCI_EVENT_PKT 0x04
#define HCI_VENDOR_PKT 0xff
#define HCI_2DH1 0x0002
#define HCI_3DH1 0x0004
#define HCI_DM1 0x0008
#define HCI_DH1 0x0010
#define HCI_2DH3 0x0100
#define HCI_3DH3 0x0200
#define HCI_DM3 0x0400
#define HCI_DH3 0x0800
#define HCI_2DH5 0x1000
#define HCI_3DH5 0x2000
#define HCI_DM5 0x4000
#define HCI_DH5 0x8000
#define HCI_HV1 0x0020
#define HCI_HV2 0x0040
#define HCI_HV3 0x0080
#define HCI_EV3 0x0008
#define HCI_EV4 0x0010
#define HCI_EV5 0x0020
#define HCI_2EV3 0x0040
#define HCI_3EV3 0x0080
#define HCI_2EV5 0x0100
#define HCI_3EV5 0x0200
#define SCO_PTYPE_MASK (HCI_HV1 | HCI_HV2 | HCI_HV3)
#define ACL_PTYPE_MASK (HCI_DM1 | HCI_DH1 | HCI_DM3 | HCI_DH3 | HCI_DM5 | HCI_DH5)
#define ESCO_HV1 0x0001
#define ESCO_HV2 0x0002
#define ESCO_HV3 0x0004
#define ESCO_EV3 0x0008
#define ESCO_EV4 0x0010
#define ESCO_EV5 0x0020
#define ESCO_2EV3 0x0040
#define ESCO_3EV3 0x0080
#define ESCO_2EV5 0x0100
#define ESCO_3EV5 0x0200
#define SCO_ESCO_MASK (ESCO_HV1 | ESCO_HV2 | ESCO_HV3)
#define EDR_ESCO_MASK (ESCO_2EV3 | ESCO_3EV3 | ESCO_2EV5 | ESCO_3EV5)
#define ALL_ESCO_MASK (SCO_ESCO_MASK | ESCO_EV3 | ESCO_EV4 | ESCO_EV5 |  EDR_ESCO_MASK)
#define HCI_UNKNOWN_COMMAND 0x01
#define HCI_NO_CONNECTION 0x02
#define HCI_HARDWARE_FAILURE 0x03
#define HCI_PAGE_TIMEOUT 0x04
#define HCI_AUTHENTICATION_FAILURE 0x05
#define HCI_PIN_OR_KEY_MISSING 0x06
#define HCI_MEMORY_FULL 0x07
#define HCI_CONNECTION_TIMEOUT 0x08
#define HCI_MAX_NUMBER_OF_CONNECTIONS 0x09
#define HCI_MAX_NUMBER_OF_SCO_CONNECTIONS 0x0a
#define HCI_ACL_CONNECTION_EXISTS 0x0b
#define HCI_COMMAND_DISALLOWED 0x0c
#define HCI_REJECTED_LIMITED_RESOURCES 0x0d
#define HCI_REJECTED_SECURITY 0x0e
#define HCI_REJECTED_PERSONAL 0x0f
#define HCI_HOST_TIMEOUT 0x10
#define HCI_UNSUPPORTED_FEATURE 0x11
#define HCI_INVALID_PARAMETERS 0x12
#define HCI_OE_USER_ENDED_CONNECTION 0x13
#define HCI_OE_LOW_RESOURCES 0x14
#define HCI_OE_POWER_OFF 0x15
#define HCI_CONNECTION_TERMINATED 0x16
#define HCI_REPEATED_ATTEMPTS 0x17
#define HCI_PAIRING_NOT_ALLOWED 0x18
#define HCI_UNKNOWN_LMP_PDU 0x19
#define HCI_UNSUPPORTED_REMOTE_FEATURE 0x1a
#define HCI_SCO_OFFSET_REJECTED 0x1b
#define HCI_SCO_INTERVAL_REJECTED 0x1c
#define HCI_AIR_MODE_REJECTED 0x1d
#define HCI_INVALID_LMP_PARAMETERS 0x1e
#define HCI_UNSPECIFIED_ERROR 0x1f
#define HCI_UNSUPPORTED_LMP_PARAMETER_VALUE 0x20
#define HCI_ROLE_CHANGE_NOT_ALLOWED 0x21
#define HCI_LMP_RESPONSE_TIMEOUT 0x22
#define HCI_LMP_ERROR_TRANSACTION_COLLISION 0x23
#define HCI_LMP_PDU_NOT_ALLOWED 0x24
#define HCI_ENCRYPTION_MODE_NOT_ACCEPTED 0x25
#define HCI_UNIT_LINK_KEY_USED 0x26
#define HCI_QOS_NOT_SUPPORTED 0x27
#define HCI_INSTANT_PASSED 0x28
#define HCI_PAIRING_NOT_SUPPORTED 0x29
#define HCI_TRANSACTION_COLLISION 0x2a
#define HCI_QOS_UNACCEPTABLE_PARAMETER 0x2c
#define HCI_QOS_REJECTED 0x2d
#define HCI_CLASSIFICATION_NOT_SUPPORTED 0x2e
#define HCI_INSUFFICIENT_SECURITY 0x2f
#define HCI_PARAMETER_OUT_OF_RANGE 0x30
#define HCI_ROLE_SWITCH_PENDING 0x32
#define HCI_SLOT_VIOLATION 0x34
#define HCI_ROLE_SWITCH_FAILED 0x35
#define HCI_EIR_TOO_LARGE 0x36
#define HCI_SIMPLE_PAIRING_NOT_SUPPORTED 0x37
#define HCI_HOST_BUSY_PAIRING 0x38
#define ACL_CONT 0x01
#define ACL_START 0x02
#define ACL_ACTIVE_BCAST 0x04
#define ACL_PICO_BCAST 0x08
#define SCO_LINK 0x00
#define ACL_LINK 0x01
#define ESCO_LINK 0x02
#define LMP_3SLOT 0x01
#define LMP_5SLOT 0x02
#define LMP_ENCRYPT 0x04
#define LMP_SOFFSET 0x08
#define LMP_TACCURACY 0x10
#define LMP_RSWITCH 0x20
#define LMP_HOLD 0x40
#define LMP_SNIFF 0x80
#define LMP_PARK 0x01
#define LMP_RSSI 0x02
#define LMP_QUALITY 0x04
#define LMP_SCO 0x08
#define LMP_HV2 0x10
#define LMP_HV3 0x20
#define LMP_ULAW 0x40
#define LMP_ALAW 0x80
#define LMP_CVSD 0x01
#define LMP_PSCHEME 0x02
#define LMP_PCONTROL 0x04
#define LMP_TRSP_SCO 0x08
#define LMP_BCAST_ENC 0x80
#define LMP_EDR_ACL_2M 0x02
#define LMP_EDR_ACL_3M 0x04
#define LMP_ENH_ISCAN 0x08
#define LMP_ILACE_ISCAN 0x10
#define LMP_ILACE_PSCAN 0x20
#define LMP_RSSI_INQ 0x40
#define LMP_ESCO 0x80
#define LMP_EV4 0x01
#define LMP_EV5 0x02
#define LMP_AFH_CAP_SLV 0x08
#define LMP_AFH_CLS_SLV 0x10
#define LMP_EDR_3SLOT 0x80
#define LMP_EDR_5SLOT 0x01
#define LMP_SNIFF_SUBR 0x02
#define LMP_PAUSE_ENC 0x04
#define LMP_AFH_CAP_MST 0x08
#define LMP_AFH_CLS_MST 0x10
#define LMP_EDR_ESCO_2M 0x20
#define LMP_EDR_ESCO_3M 0x40
#define LMP_EDR_3S_ESCO 0x80
#define LMP_EXT_INQ 0x01
#define LMP_SIMPLE_PAIR 0x08
#define LMP_ENCAPS_PDU 0x10
#define LMP_ERR_DAT_REP 0x20
#define LMP_NFLUSH_PKTS 0x40
#define LMP_LSTO 0x01
#define LMP_INQ_TX_PWR 0x02
#define LMP_EXT_FEAT 0x80
#define HCI_LP_RSWITCH 0x0001
#define HCI_LP_HOLD 0x0002
#define HCI_LP_SNIFF 0x0004
#define HCI_LP_PARK 0x0008
#define HCI_LM_ACCEPT 0x8000
#define HCI_LM_MASTER 0x0001
#define HCI_LM_AUTH 0x0002
#define HCI_LM_ENCRYPT 0x0004
#define HCI_LM_TRUSTED 0x0008
#define HCI_LM_RELIABLE 0x0010
#define HCI_LM_SECURE 0x0020
#define OGF_LINK_CTL 0x01
#define OCF_INQUIRY 0x0001
#define INQUIRY_CP_SIZE 5
#define STATUS_BDADDR_RP_SIZE 7
#define OCF_INQUIRY_CANCEL 0x0002
#define OCF_PERIODIC_INQUIRY 0x0003
#define PERIODIC_INQUIRY_CP_SIZE 9
#define OCF_EXIT_PERIODIC_INQUIRY 0x0004
#define OCF_CREATE_CONN 0x0005
#define CREATE_CONN_CP_SIZE 13
#define OCF_DISCONNECT 0x0006
#define DISCONNECT_CP_SIZE 3
#define OCF_ADD_SCO 0x0007
#define ADD_SCO_CP_SIZE 4
#define OCF_CREATE_CONN_CANCEL 0x0008
#define CREATE_CONN_CANCEL_CP_SIZE 6
#define OCF_ACCEPT_CONN_REQ 0x0009
#define ACCEPT_CONN_REQ_CP_SIZE 7
#define OCF_REJECT_CONN_REQ 0x000A
#define REJECT_CONN_REQ_CP_SIZE 7
#define OCF_LINK_KEY_REPLY 0x000B
#define LINK_KEY_REPLY_CP_SIZE 22
#define OCF_LINK_KEY_NEG_REPLY 0x000C
#define OCF_PIN_CODE_REPLY 0x000D
#define PIN_CODE_REPLY_CP_SIZE 23
#define OCF_PIN_CODE_NEG_REPLY 0x000E
#define OCF_SET_CONN_PTYPE 0x000F
#define SET_CONN_PTYPE_CP_SIZE 4
#define OCF_AUTH_REQUESTED 0x0011
#define AUTH_REQUESTED_CP_SIZE 2
#define OCF_SET_CONN_ENCRYPT 0x0013
#define SET_CONN_ENCRYPT_CP_SIZE 3
#define OCF_CHANGE_CONN_LINK_KEY 0x0015
#define CHANGE_CONN_LINK_KEY_CP_SIZE 2
#define OCF_MASTER_LINK_KEY 0x0017
#define MASTER_LINK_KEY_CP_SIZE 1
#define OCF_REMOTE_NAME_REQ 0x0019
#define REMOTE_NAME_REQ_CP_SIZE 10
#define OCF_REMOTE_NAME_REQ_CANCEL 0x001A
#define REMOTE_NAME_REQ_CANCEL_CP_SIZE 6
#define OCF_READ_REMOTE_FEATURES 0x001B
#define READ_REMOTE_FEATURES_CP_SIZE 2
#define OCF_READ_REMOTE_EXT_FEATURES 0x001C
#define READ_REMOTE_EXT_FEATURES_CP_SIZE 3
#define OCF_READ_REMOTE_VERSION 0x001D
#define READ_REMOTE_VERSION_CP_SIZE 2
#define OCF_READ_CLOCK_OFFSET 0x001F
#define READ_CLOCK_OFFSET_CP_SIZE 2
#define OCF_READ_LMP_HANDLE 0x0020
#define OCF_SETUP_SYNC_CONN 0x0028
#define SETUP_SYNC_CONN_CP_SIZE 17
#define OCF_ACCEPT_SYNC_CONN_REQ 0x0029
#define ACCEPT_SYNC_CONN_REQ_CP_SIZE 21
#define OCF_REJECT_SYNC_CONN_REQ 0x002A
#define REJECT_SYNC_CONN_REQ_CP_SIZE 7
#define OCF_IO_CAPABILITY_REPLY 0x002B
#define IO_CAPABILITY_REPLY_CP_SIZE 9
#define OCF_USER_CONFIRM_REPLY 0x002C
#define USER_CONFIRM_REPLY_CP_SIZE 6
#define OCF_USER_CONFIRM_NEG_REPLY 0x002D
#define OCF_USER_PASSKEY_REPLY 0x002E
#define USER_PASSKEY_REPLY_CP_SIZE 10
#define OCF_USER_PASSKEY_NEG_REPLY 0x002F
#define OCF_REMOTE_OOB_DATA_REPLY 0x0030
#define REMOTE_OOB_DATA_REPLY_CP_SIZE 38
#define OCF_REMOTE_OOB_DATA_NEG_REPLY 0x0033
#define OCF_IO_CAPABILITY_NEG_REPLY 0x0034
#define IO_CAPABILITY_NEG_REPLY_CP_SIZE 7
#define OGF_LINK_POLICY 0x02
#define OCF_HOLD_MODE 0x0001
#define HOLD_MODE_CP_SIZE 6
#define OCF_SNIFF_MODE 0x0003
#define SNIFF_MODE_CP_SIZE 10
#define OCF_EXIT_SNIFF_MODE 0x0004
#define EXIT_SNIFF_MODE_CP_SIZE 2
#define OCF_PARK_MODE 0x0005
#define PARK_MODE_CP_SIZE 6
#define OCF_EXIT_PARK_MODE 0x0006
#define EXIT_PARK_MODE_CP_SIZE 2
#define OCF_QOS_SETUP 0x0007
#define HCI_QOS_CP_SIZE 17
#define QOS_SETUP_CP_SIZE (3 + HCI_QOS_CP_SIZE)
#define OCF_ROLE_DISCOVERY 0x0009
#define ROLE_DISCOVERY_CP_SIZE 2
#define ROLE_DISCOVERY_RP_SIZE 4
#define OCF_SWITCH_ROLE 0x000B
#define SWITCH_ROLE_CP_SIZE 7
#define OCF_READ_LINK_POLICY 0x000C
#define READ_LINK_POLICY_CP_SIZE 2
#define READ_LINK_POLICY_RP_SIZE 5
#define OCF_WRITE_LINK_POLICY 0x000D
#define WRITE_LINK_POLICY_CP_SIZE 4
#define WRITE_LINK_POLICY_RP_SIZE 3
#define OCF_READ_DEFAULT_LINK_POLICY 0x000E
#define OCF_WRITE_DEFAULT_LINK_POLICY 0x000F
#define OCF_FLOW_SPECIFICATION 0x0010
#define OCF_SNIFF_SUBRATING 0x0011
#define SNIFF_SUBRATING_CP_SIZE 8
#define OGF_HOST_CTL 0x03
#define OCF_SET_EVENT_MASK 0x0001
#define SET_EVENT_MASK_CP_SIZE 8
#define OCF_RESET 0x0003
#define OCF_SET_EVENT_FLT 0x0005
#define SET_EVENT_FLT_CP_SIZE 2
#define FLT_CLEAR_ALL 0x00
#define FLT_INQ_RESULT 0x01
#define FLT_CONN_SETUP 0x02
#define INQ_RESULT_RETURN_ALL 0x00
#define INQ_RESULT_RETURN_CLASS 0x01
#define INQ_RESULT_RETURN_BDADDR 0x02
#define CONN_SETUP_ALLOW_ALL 0x00
#define CONN_SETUP_ALLOW_CLASS 0x01
#define CONN_SETUP_ALLOW_BDADDR 0x02
#define CONN_SETUP_AUTO_OFF 0x01
#define CONN_SETUP_AUTO_ON 0x02
#define OCF_FLUSH 0x0008
#define OCF_READ_PIN_TYPE 0x0009
#define READ_PIN_TYPE_RP_SIZE 2
#define OCF_WRITE_PIN_TYPE 0x000A
#define WRITE_PIN_TYPE_CP_SIZE 1
#define OCF_CREATE_NEW_UNIT_KEY 0x000B
#define OCF_READ_STORED_LINK_KEY 0x000D
#define READ_STORED_LINK_KEY_CP_SIZE 7
#define READ_STORED_LINK_KEY_RP_SIZE 5
#define OCF_WRITE_STORED_LINK_KEY 0x0011
#define WRITE_STORED_LINK_KEY_CP_SIZE 1
#define READ_WRITE_LINK_KEY_RP_SIZE 2
#define OCF_DELETE_STORED_LINK_KEY 0x0012
#define DELETE_STORED_LINK_KEY_CP_SIZE 7
#define DELETE_STORED_LINK_KEY_RP_SIZE 3
#define OCF_CHANGE_LOCAL_NAME 0x0013
#define CHANGE_LOCAL_NAME_CP_SIZE 248 
#define OCF_READ_LOCAL_NAME 0x0014
#define READ_LOCAL_NAME_RP_SIZE 249 
#define OCF_READ_CONN_ACCEPT_TIMEOUT 0x0015
#define READ_CONN_ACCEPT_TIMEOUT_RP_SIZE 3
#define OCF_WRITE_CONN_ACCEPT_TIMEOUT 0x0016
#define WRITE_CONN_ACCEPT_TIMEOUT_CP_SIZE 2
#define OCF_READ_PAGE_TIMEOUT 0x0017
#define READ_PAGE_TIMEOUT_RP_SIZE 3
#define OCF_WRITE_PAGE_TIMEOUT 0x0018
#define WRITE_PAGE_TIMEOUT_CP_SIZE 2
#define OCF_READ_SCAN_ENABLE 0x0019
#define READ_SCAN_ENABLE_RP_SIZE 2
#define OCF_WRITE_SCAN_ENABLE 0x001A
#define SCAN_DISABLED 0x00
#define SCAN_INQUIRY 0x01
#define SCAN_PAGE 0x02
#define OCF_READ_PAGE_ACTIVITY 0x001B
#define READ_PAGE_ACTIVITY_RP_SIZE 5
#define OCF_WRITE_PAGE_ACTIVITY 0x001C
#define WRITE_PAGE_ACTIVITY_CP_SIZE 4
#define OCF_READ_INQ_ACTIVITY 0x001D
#define READ_INQ_ACTIVITY_RP_SIZE 5
#define OCF_WRITE_INQ_ACTIVITY 0x001E
#define WRITE_INQ_ACTIVITY_CP_SIZE 4
#define OCF_READ_AUTH_ENABLE 0x001F
#define OCF_WRITE_AUTH_ENABLE 0x0020
#define AUTH_DISABLED 0x00
#define AUTH_ENABLED 0x01
#define OCF_READ_ENCRYPT_MODE 0x0021
#define OCF_WRITE_ENCRYPT_MODE 0x0022
#define ENCRYPT_DISABLED 0x00
#define ENCRYPT_P2P 0x01
#define ENCRYPT_BOTH 0x02
#define OCF_READ_CLASS_OF_DEV 0x0023
#define READ_CLASS_OF_DEV_RP_SIZE 4 
#define OCF_WRITE_CLASS_OF_DEV 0x0024
#define WRITE_CLASS_OF_DEV_CP_SIZE 3
#define OCF_READ_VOICE_SETTING 0x0025
#define READ_VOICE_SETTING_RP_SIZE 3
#define OCF_WRITE_VOICE_SETTING 0x0026
#define WRITE_VOICE_SETTING_CP_SIZE 2
#define OCF_READ_AUTOMATIC_FLUSH_TIMEOUT 0x0027
#define OCF_WRITE_AUTOMATIC_FLUSH_TIMEOUT 0x0028
#define OCF_READ_NUM_BROADCAST_RETRANS 0x0029
#define OCF_WRITE_NUM_BROADCAST_RETRANS 0x002A
#define OCF_READ_HOLD_MODE_ACTIVITY 0x002B
#define OCF_WRITE_HOLD_MODE_ACTIVITY 0x002C
#define OCF_READ_TRANSMIT_POWER_LEVEL 0x002D
#define READ_TRANSMIT_POWER_LEVEL_CP_SIZE 3
#define READ_TRANSMIT_POWER_LEVEL_RP_SIZE 4
#define OCF_READ_SYNC_FLOW_ENABLE 0x002E
#define OCF_WRITE_SYNC_FLOW_ENABLE 0x002F
#define OCF_SET_CONTROLLER_TO_HOST_FC 0x0031
#define OCF_HOST_BUFFER_SIZE 0x0033
#define HOST_BUFFER_SIZE_CP_SIZE 7
#define OCF_HOST_NUM_COMP_PKTS 0x0035
#define HOST_NUM_COMP_PKTS_CP_SIZE 1
#define OCF_READ_LINK_SUPERVISION_TIMEOUT 0x0036
#define READ_LINK_SUPERVISION_TIMEOUT_RP_SIZE 5
#define OCF_WRITE_LINK_SUPERVISION_TIMEOUT 0x0037
#define WRITE_LINK_SUPERVISION_TIMEOUT_CP_SIZE 4
#define WRITE_LINK_SUPERVISION_TIMEOUT_RP_SIZE 3
#define OCF_READ_NUM_SUPPORTED_IAC 0x0038
#define MAX_IAC_LAP 0x40
#define OCF_READ_CURRENT_IAC_LAP 0x0039
#define READ_CURRENT_IAC_LAP_RP_SIZE 2+3*MAX_IAC_LAP
#define OCF_WRITE_CURRENT_IAC_LAP 0x003A
#define WRITE_CURRENT_IAC_LAP_CP_SIZE 1+3*MAX_IAC_LAP
#define OCF_READ_PAGE_SCAN_PERIOD_MODE 0x003B
#define OCF_WRITE_PAGE_SCAN_PERIOD_MODE 0x003C
#define OCF_READ_PAGE_SCAN_MODE 0x003D
#define OCF_WRITE_PAGE_SCAN_MODE 0x003E
#define OCF_SET_AFH_CLASSIFICATION 0x003F
#define SET_AFH_CLASSIFICATION_CP_SIZE 10
#define SET_AFH_CLASSIFICATION_RP_SIZE 1
#define OCF_READ_INQUIRY_SCAN_TYPE 0x0042
#define READ_INQUIRY_SCAN_TYPE_RP_SIZE 2
#define OCF_WRITE_INQUIRY_SCAN_TYPE 0x0043
#define WRITE_INQUIRY_SCAN_TYPE_CP_SIZE 1
#define WRITE_INQUIRY_SCAN_TYPE_RP_SIZE 1
#define OCF_READ_INQUIRY_MODE 0x0044
#define READ_INQUIRY_MODE_RP_SIZE 2
#define OCF_WRITE_INQUIRY_MODE 0x0045
#define WRITE_INQUIRY_MODE_CP_SIZE 1
#define WRITE_INQUIRY_MODE_RP_SIZE 1
#define OCF_READ_PAGE_SCAN_TYPE 0x0046
#define OCF_WRITE_PAGE_SCAN_TYPE 0x0047
#define OCF_READ_AFH_MODE 0x0048
#define READ_AFH_MODE_RP_SIZE 2
#define OCF_WRITE_AFH_MODE 0x0049
#define WRITE_AFH_MODE_CP_SIZE 1
#define WRITE_AFH_MODE_RP_SIZE 1
#define OCF_READ_EXT_INQUIRY_RESPONSE 0x0051
#define READ_EXT_INQUIRY_RESPONSE_RP_SIZE 242
#define OCF_WRITE_EXT_INQUIRY_RESPONSE 0x0052
#define WRITE_EXT_INQUIRY_RESPONSE_CP_SIZE 241
#define WRITE_EXT_INQUIRY_RESPONSE_RP_SIZE 1
#define OCF_REFRESH_ENCRYPTION_KEY 0x0053
#define REFRESH_ENCRYPTION_KEY_CP_SIZE 2
#define REFRESH_ENCRYPTION_KEY_RP_SIZE 1
#define OCF_READ_SIMPLE_PAIRING_MODE 0x0055
#define READ_SIMPLE_PAIRING_MODE_RP_SIZE 2
#define OCF_WRITE_SIMPLE_PAIRING_MODE 0x0056
#define WRITE_SIMPLE_PAIRING_MODE_CP_SIZE 1
#define WRITE_SIMPLE_PAIRING_MODE_RP_SIZE 1
#define OCF_READ_LOCAL_OOB_DATA 0x0057
#define READ_LOCAL_OOB_DATA_RP_SIZE 33
#define OCF_READ_INQUIRY_TRANSMIT_POWER_LEVEL 0x0058
#define READ_INQUIRY_TRANSMIT_POWER_LEVEL_RP_SIZE 2
#define OCF_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL 0x0059
#define WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CP_SIZE 1
#define WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_RP_SIZE 1
#define OCF_READ_DEFAULT_ERROR_DATA_REPORTING 0x005A
#define READ_DEFAULT_ERROR_DATA_REPORTING_RP_SIZE 2
#define OCF_WRITE_DEFAULT_ERROR_DATA_REPORTING 0x005B
#define WRITE_DEFAULT_ERROR_DATA_REPORTING_CP_SIZE 1
#define WRITE_DEFAULT_ERROR_DATA_REPORTING_RP_SIZE 1
#define OCF_ENHANCED_FLUSH 0x005F
#define ENHANCED_FLUSH_CP_SIZE 3
#define OCF_SEND_KEYPRESS_NOTIFY 0x0060
#define SEND_KEYPRESS_NOTIFY_CP_SIZE 7
#define SEND_KEYPRESS_NOTIFY_RP_SIZE 1
#define OGF_INFO_PARAM 0x04
#define OCF_READ_LOCAL_VERSION 0x0001
#define READ_LOCAL_VERSION_RP_SIZE 9
#define OCF_READ_LOCAL_COMMANDS 0x0002
#define READ_LOCAL_COMMANDS_RP_SIZE 65
#define OCF_READ_LOCAL_FEATURES 0x0003
#define READ_LOCAL_FEATURES_RP_SIZE 9
#define OCF_READ_LOCAL_EXT_FEATURES 0x0004
#define READ_LOCAL_EXT_FEATURES_CP_SIZE 1
#define READ_LOCAL_EXT_FEATURES_RP_SIZE 11
#define OCF_READ_BUFFER_SIZE 0x0005
#define READ_BUFFER_SIZE_RP_SIZE 8
#define OCF_READ_COUNTRY_CODE 0x0007
#define OCF_READ_BD_ADDR 0x0009
#define READ_BD_ADDR_RP_SIZE 7
#define OGF_STATUS_PARAM 0x05
#define OCF_READ_FAILED_CONTACT_COUNTER 0x0001
#define READ_FAILED_CONTACT_COUNTER_RP_SIZE 4
#define OCF_RESET_FAILED_CONTACT_COUNTER 0x0002
#define RESET_FAILED_CONTACT_COUNTER_RP_SIZE 4
#define OCF_READ_LINK_QUALITY 0x0003
#define READ_LINK_QUALITY_RP_SIZE 4
#define OCF_READ_RSSI 0x0005
#define READ_RSSI_RP_SIZE 4
#define OCF_READ_AFH_MAP 0x0006
#define READ_AFH_MAP_RP_SIZE 14
#define OCF_READ_CLOCK 0x0007
#define READ_CLOCK_CP_SIZE 3
#define READ_CLOCK_RP_SIZE 9
#define OGF_TESTING_CMD 0x3e
#define OCF_READ_LOOPBACK_MODE 0x0001
#define OCF_WRITE_LOOPBACK_MODE 0x0002
#define OCF_ENABLE_DEVICE_UNDER_TEST_MODE 0x0003
#define OCF_WRITE_SIMPLE_PAIRING_DEBUG_MODE 0x0004
#define WRITE_SIMPLE_PAIRING_DEBUG_MODE_CP_SIZE 1
#define WRITE_SIMPLE_PAIRING_DEBUG_MODE_RP_SIZE 1
#define OGF_VENDOR_CMD 0x3f
#define EVT_INQUIRY_COMPLETE 0x01
#define EVT_INQUIRY_RESULT 0x02
#define INQUIRY_INFO_SIZE 14
#define EVT_CONN_COMPLETE 0x03
#define EVT_CONN_COMPLETE_SIZE 13
#define EVT_CONN_REQUEST 0x04
#define EVT_CONN_REQUEST_SIZE 10
#define EVT_DISCONN_COMPLETE 0x05
#define EVT_DISCONN_COMPLETE_SIZE 4
#define EVT_AUTH_COMPLETE 0x06
#define EVT_AUTH_COMPLETE_SIZE 3
#define EVT_REMOTE_NAME_REQ_COMPLETE 0x07
#define EVT_REMOTE_NAME_REQ_COMPLETE_SIZE 255
#define EVT_ENCRYPT_CHANGE 0x08
#define EVT_ENCRYPT_CHANGE_SIZE 5
#define EVT_CHANGE_CONN_LINK_KEY_COMPLETE 0x09
#define EVT_CHANGE_CONN_LINK_KEY_COMPLETE_SIZE 3
#define EVT_MASTER_LINK_KEY_COMPLETE 0x0A
#define EVT_MASTER_LINK_KEY_COMPLETE_SIZE 4
#define EVT_READ_REMOTE_FEATURES_COMPLETE 0x0B
#define EVT_READ_REMOTE_FEATURES_COMPLETE_SIZE 11
#define EVT_READ_REMOTE_VERSION_COMPLETE 0x0C
#define EVT_READ_REMOTE_VERSION_COMPLETE_SIZE 8
#define EVT_QOS_SETUP_COMPLETE 0x0D
#define EVT_QOS_SETUP_COMPLETE_SIZE (4 + HCI_QOS_CP_SIZE)
#define EVT_CMD_COMPLETE 0x0E
#define EVT_CMD_COMPLETE_SIZE 3
#define EVT_CMD_STATUS 0x0F
#define EVT_CMD_STATUS_SIZE 4
#define EVT_HARDWARE_ERROR 0x10
#define EVT_HARDWARE_ERROR_SIZE 1
#define EVT_FLUSH_OCCURRED 0x11
#define EVT_FLUSH_OCCURRED_SIZE 2
#define EVT_ROLE_CHANGE 0x12
#define EVT_ROLE_CHANGE_SIZE 8
#define EVT_NUM_COMP_PKTS 0x13
#define EVT_NUM_COMP_PKTS_SIZE 1
#define EVT_MODE_CHANGE 0x14
#define EVT_MODE_CHANGE_SIZE 6
#define EVT_RETURN_LINK_KEYS 0x15
#define EVT_RETURN_LINK_KEYS_SIZE 1
#define EVT_PIN_CODE_REQ 0x16
#define EVT_PIN_CODE_REQ_SIZE 6
#define EVT_LINK_KEY_REQ 0x17
#define EVT_LINK_KEY_REQ_SIZE 6
#define EVT_LINK_KEY_NOTIFY 0x18
#define EVT_LINK_KEY_NOTIFY_SIZE 23
#define EVT_LOOPBACK_COMMAND 0x19
#define EVT_DATA_BUFFER_OVERFLOW 0x1A
#define EVT_DATA_BUFFER_OVERFLOW_SIZE 1
#define EVT_MAX_SLOTS_CHANGE 0x1B
#define EVT_MAX_SLOTS_CHANGE_SIZE 3
#define EVT_READ_CLOCK_OFFSET_COMPLETE 0x1C
#define EVT_READ_CLOCK_OFFSET_COMPLETE_SIZE 5
#define EVT_CONN_PTYPE_CHANGED 0x1D
#define EVT_CONN_PTYPE_CHANGED_SIZE 5
#define EVT_QOS_VIOLATION 0x1E
#define EVT_QOS_VIOLATION_SIZE 2
#define EVT_PSCAN_REP_MODE_CHANGE 0x20
#define EVT_PSCAN_REP_MODE_CHANGE_SIZE 7
#define EVT_FLOW_SPEC_COMPLETE 0x21
#define EVT_FLOW_SPEC_COMPLETE_SIZE (5 + HCI_QOS_CP_SIZE)
#define EVT_INQUIRY_RESULT_WITH_RSSI 0x22
#define INQUIRY_INFO_WITH_RSSI_SIZE 14
#define INQUIRY_INFO_WITH_RSSI_AND_PSCAN_MODE_SIZE 15
#define EVT_READ_REMOTE_EXT_FEATURES_COMPLETE 0x23
#define EVT_READ_REMOTE_EXT_FEATURES_COMPLETE_SIZE 13
#define EVT_SYNC_CONN_COMPLETE 0x2C
#define EVT_SYNC_CONN_COMPLETE_SIZE 17
#define EVT_SYNC_CONN_CHANGED 0x2D
#define EVT_SYNC_CONN_CHANGED_SIZE 9
#define EVT_SNIFF_SUBRATING 0x2E
#define EVT_SNIFF_SUBRATING_SIZE 11
#define EVT_EXTENDED_INQUIRY_RESULT 0x2F
#define EXTENDED_INQUIRY_INFO_SIZE 254
#define EVT_ENCRYPTION_KEY_REFRESH_COMPLETE 0x30
#define EVT_ENCRYPTION_KEY_REFRESH_COMPLETE_SIZE 3
#define EVT_IO_CAPABILITY_REQUEST 0x31
#define EVT_IO_CAPABILITY_REQUEST_SIZE 6
#define EVT_IO_CAPABILITY_RESPONSE 0x32
#define EVT_IO_CAPABILITY_RESPONSE_SIZE 9
#define EVT_USER_CONFIRM_REQUEST 0x33
#define EVT_USER_CONFIRM_REQUEST_SIZE 10
#define EVT_USER_PASSKEY_REQUEST 0x34
#define EVT_USER_PASSKEY_REQUEST_SIZE 6
#define EVT_REMOTE_OOB_DATA_REQUEST 0x35
#define EVT_REMOTE_OOB_DATA_REQUEST_SIZE 6
#define EVT_SIMPLE_PAIRING_COMPLETE 0x36
#define EVT_SIMPLE_PAIRING_COMPLETE_SIZE 7
#define EVT_LINK_SUPERVISION_TIMEOUT_CHANGED 0x38
#define EVT_LINK_SUPERVISION_TIMEOUT_CHANGED_SIZE 4
#define EVT_ENHANCED_FLUSH_COMPLETE 0x39
#define EVT_ENHANCED_FLUSH_COMPLETE_SIZE 2
#define EVT_USER_PASSKEY_NOTIFY 0x3B
#define EVT_USER_PASSKEY_NOTIFY_SIZE 10
#define EVT_KEYPRESS_NOTIFY 0x3C
#define EVT_KEYPRESS_NOTIFY_SIZE 7
#define EVT_REMOTE_HOST_FEATURES_NOTIFY 0x3D
#define EVT_REMOTE_HOST_FEATURES_NOTIFY_SIZE 14
#define EVT_TESTING 0xFE
#define EVT_VENDOR 0xFF
#define EVT_STACK_INTERNAL 0xFD
#define EVT_STACK_INTERNAL_SIZE 2
#define EVT_SI_DEVICE 0x01
#define EVT_SI_DEVICE_SIZE 4
#define EVT_SI_SECURITY 0x02
#define HCI_TYPE_LEN 1
#define HCI_COMMAND_HDR_SIZE 3
#define HCI_EVENT_HDR_SIZE 2
#define HCI_ACL_HDR_SIZE 4
#define HCI_SCO_HDR_SIZE 3
#define HCI_MSG_HDR_SIZE 6
#define cmd_opcode_pack(ogf, ocf) (uint16_t)((ocf & 0x03ff)|(ogf << 10))
#define cmd_opcode_ogf(op) (op >> 10)
#define cmd_opcode_ocf(op) (op & 0x03ff)
#define acl_handle_pack(h, f) (uint16_t)((h & 0x0fff)|(f << 12))
#define acl_handle(h) (h & 0x0fff)
#define acl_flags(h) (h >> 12)
#endif
#define HCI_DATA_DIR 1
#define HCI_FILTER 2
#define HCI_TIME_STAMP 3
#define HCI_CMSG_DIR 0x0001
#define HCI_CMSG_TSTAMP 0x0002
#define HCI_DEV_NONE 0xffff
#define HCI_FLT_TYPE_BITS 31
#define HCI_FLT_EVENT_BITS 63
#define HCI_FLT_OGF_BITS 63
#define HCI_FLT_OCF_BITS 127
#define IREQ_CACHE_FLUSH 0x0001
#ifdef __cplusplus
#endif
struct hci_dev_stats {
	uint32_t err_rx;
	uint32_t err_tx;
	uint32_t cmd_tx;
	uint32_t evt_rx;
	uint32_t acl_tx;
	uint32_t acl_rx;
	uint32_t sco_tx;
	uint32_t sco_rx;
	uint32_t byte_rx;
	uint32_t byte_tx;
};
struct hci_dev_info {
	uint16_t dev_id;
	char     name[8];

	bdaddr_t bdaddr;

	uint32_t flags;
	uint8_t  type;

	uint8_t  features[8];

	uint32_t pkt_type;
	uint32_t link_policy;
	uint32_t link_mode;

	uint16_t acl_mtu;
	uint16_t acl_pkts;
	uint16_t sco_mtu;
	uint16_t sco_pkts;

	struct   hci_dev_stats stat;
};
enum {
	HCI_UP,
	HCI_INIT,
	HCI_RUNNING,

	HCI_PSCAN,
	HCI_ISCAN,
	HCI_AUTH,
	HCI_ENCRYPT,
	HCI_INQUIRY,

	HCI_RAW,

	HCI_SECMGR
};
struct sockaddr_hci {
	sa_family_t	hci_family;
	unsigned short	hci_dev;
};
struct hci_conn_info {
        uint16_t handle;
        bdaddr_t bdaddr;
        uint8_t  type;
        uint8_t  out;
        uint16_t state;
        uint32_t link_mode;
        uint32_t mtu;
        uint32_t cnt;
        uint32_t pkts;
};
struct hci_conn_list_req {
        uint16_t dev_id;
        uint16_t conn_num;
        struct hci_conn_info conn_info[0];
};
typedef struct {
	uint16_t	opcode;
	uint8_t		plen;
} __attribute__ ((packed))	hci_command_hdr;
#define HCI_COMMAND_HDR_SIZE 	3
#endif
