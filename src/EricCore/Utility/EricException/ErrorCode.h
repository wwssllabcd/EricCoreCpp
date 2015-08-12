

#ifndef _ERROR_CODE_
#define _ERROR_CODE_


#define MYEXCEPTION_NO_ERROR                  0x0000
#define GENERAL_FAIL                          0x0001

//Block Manager
#define BM_GET_ONE_BLOCK_NO_BLOCK                   0x0101
#define BM_GET_TYPEC_EVEN_BLOCK_NOT_ENOUGHT         0x0102
#define BM_GET_TYPEC_ODD_BLOCK_NOT_ENOUGHT          0x0103
#define BM_GET_TYPEC_BLOCK_NOT_ENOUGHT              0x0104
#define BM_GET4BYTE_BLOCK_FAIL                      0x0105
#define BM_GET_ONE_BLOCK_FAIL                       0x0106
#define BM_GET_ONE_BLOCK_NOT_ENOUGHT                0x0107
#define BM_GET_TYPEA_2BYTE_ERROR                    0x0108
#define BM_INTERLEAVE_NOT_IMPLEMENT_YET             0x0109
#define BM_BLOCK_NOT_ENOUGHT                        0x010A
#define BM_BLOCKEND_SHOULD_NOT_BE_ZERO              0x010B

// Usb Command
#define USBC_SCSI_CMD_FAIL                           0x0201
#define USBC_VENDOR_CMD_FAIL                         0x0202
#define USBC_WRITE_8K_FAIL                           0x0203
#define USBC_WRITE_8K_VERIFY_FAIL                    0x0204
#define USBC_WRITE_8K_VERIFY_EXTRABYTE_FAIL          0x0205
#define USBC_DOWNLOAD_ISP_FAIL                       0x0206
#define USBC_STRENGTHEN_DRIVE_FAIL                   0x0207
#define USBC_ERASE_FAIL                              0x0208
#define USBC_ERASE_ALL_FAIL                          0x0209
#define USBC_SCAN_ALL_FAIL                           0x020B
#define USBC_READ_RAM_FAIL                           0x020C
#define USBC_WRITE_RAM_FAIL                          0x020D
#define USBC_TEST_RAM1K                              0x020E
#define USBC_READ_8K_FAIL                            0x020F
#define USBC_READ_8K_WITHOUT_ECC                     0x0210
#define USBC_WRITE_10_FAIL                           0x0211
#define USBC_READ_10_FAIL                            0x0212
#define USBC_HOMO_ENABLE_FAIL                        0x0213
#define USBC_HOMO_DISABLE_FAIL                       0x0214
#define USBC_REBOOT_FAIL                             0x0215
#define USBC_REBOOT_ONCARD_FAIL                      0x0216
#define USBC_SET_7SEG_FAIL                           0x0217
#define USBC_SET_FLASH_TIMING_FAIL                   0x0218
#define USBC_ON_CARD_SORT_FAIL                       0x0219
#define USBC_WRITE_4K_FAIL                           0x021A
#define USBC_POLLING_ONCARDSORT_FAIL                 0x021B
#define USBC_SET_FEATURE_FAIL                        0x021C
#define USBC_WRITE_8K_RETRY_WRITE_FAIL               0x021D
#define USBC_TESTUNIT_READY_FAIL                     0x021E
#define USBC_GETDEVICE_INFO_FAIL                     0x021F
#define USBC_GET_FALHIDS_FAIL                        0x0220


// Utility
#define UTI_OBS_REG_FAIL_NO                         0x0301
#define UTI_SEMD_MSG_FAIL                           0x0302
#define UTI_HI_LO_CHG_MUST_EVEN                     0x0303
#define UTI_GETDATESTRING_PARSER_FAIL               0x0304
#define UTI_TOFILE_FUNCTION_FAIL                    0x0305
#define UTI_SET_BIT_FAIL                            0x0306
#define UTI_TOBOOL_FUNCTION_FAIL                    0x0307
#define UTI_GET_FILE_DATA_FAIL                      0x0308
#define UTI_GET_FILE_SIZE_FAIL                      0x0309
#define UTI_GET_INT_PARAM_ERROR                     0x030A
#define UTI_PARAM_ERROR                             0x030B



//Device Service
#define DS_CHECK_DRIVE_FAIL                         0x0401
#define DS_NO_HANDLE                                0x0402

//Chip
#define CHIP_GET_PAGE_SIZE_FAIL                     0x0501
#define CHIP_GET_LEVEL_CELL_FAIL                    0x0502
#define CHIP_LOAD_BLOCK_SIZE_TYPE_FAIL              0x0503


//Flash Base
#define FB_GET_CHIP_FAIL                            0x0601
#define FB_GET_MODE_TYPE_FAIL                       0x0602
#define FB_CHECK_CHIPS_FAIL                         0x0603
#define FB_LOAD_CHS_FAIL                            0x0604

//Flash
#define FLASH_SET_PRIPERTY_FAIL                     0x0701
#define FLASH_GETCHIPS_8BYTE_FORMAT_FAIL            0x0702
#define FLASH_GET_MMODE_TYPE_8BYTE_FORMAT_FAIL      0x0703
#define FLASH_GET_CHIP_FAIL                         0x0701

//On Card Sort
#define OCS_laod_0_7FF_FAIL                         0x0801
#define OCS_ISP_RAM_TEST_FAIL                       0x0802
#define OCS_SHOW_ONCARDSORT_STATUS_FAIL             0x0803

//Sort Board Utility
#define SBU_ADJUST_VALUE_FAIL                       0x0901

// Preformat Flow
#define PF_CANT_ALLLCATE_DEFECT_BLOCK              0x0A01
#define PF_INTERLEAVE_SETTING_ERROR                0x0A02
#define PF_CONVERT_LOWPAGE_ERROR                   0x0A03
#define PF_BLOCK_NOT_ENOUGHT                       0x0A04
#define PF_CANT_ALLLCATE_BOOT_BLOCK                0x0A05
#define PF_CANT_ALLLCATE_BOOT_BLOCK_BACKUP         0x0A06
#define PF_RW_TEST_FAIL                            0x0A07
#define PF_ABS_WRITE_FAIL                          0x0A08
#define PF_WIN_CNT_ZERO                            0x0A09
#define PF_MAXLB_LESS_USERNEED                     0x0A0A
#define PF_MAXLB_LESS_ERASESKIPCNT                 0x0A0B
#define PF_BAD_RELINK_L1_BLOCK                     0x0A0C
#define PF_BAD_SLC_TABLE                           0x0A0D



// ProjectUtility
#define PU_CHECK_CAPACITY_FAIL                     0x0B01
#define PU_TEST_FAIL                               0x0B02
#define PU_ROM_CHECKSUM_FAIL                       0x0B03

// DialogUtility
#define DU_NOT_SUPPORT_IN_VC6                      0x0C01



#endif
