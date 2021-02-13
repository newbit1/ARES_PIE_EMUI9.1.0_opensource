#ifdef  CONFIG_HUAWEI_DSM
#ifndef __HW_DSM_NOTIFY_H__
#define __HW_DSM_NOTIFY_H__

#include <dsm/dsm_pub.h>

#define DSM_WIFI_BUF_SIZE           (1024)   /*Byte*/
#define DSM_WIFI_MOD_NAME           "dsm_wifi"


void wifi_dsm_register(void);
void wifi_dsm_unregister(void);
void wifi_dsm_report_info(int dsm_id, const char *fmt, ...);
#endif
#endif
