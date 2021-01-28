#ifdef CONFIG_HUAWEI_DSM
#include "hw_dsm_notify.h"

static struct dsm_dev wifi_dsm_info = {
    .name = DSM_WIFI_MOD_NAME,
    .device_name = NULL,
    .ic_name = "QCOM",
    .module_name = NULL,
    .fops = NULL,
    .buff_size = DSM_WIFI_BUF_SIZE,
};

static struct dsm_client *wifi_dclient = NULL;

void wifi_dsm_register(void)
{
    if (NULL != wifi_dclient)
    {
        printk(KERN_INFO "wifi_dclient had been register!\n");
        return;
    }

    wifi_dclient = dsm_register_client(&wifi_dsm_info);
    if(NULL == wifi_dclient)
    {
        printk(KERN_ERR "wifi_dclient register failed!\n");
    }
    printk(KERN_ERR "wifi_dclient register success!\n");
    return;
}
EXPORT_SYMBOL(wifi_dsm_register);

void wifi_dsm_unregister(void)
{
   if(NULL != wifi_dclient) {
        dsm_unregister_client(wifi_dclient, &wifi_dsm_info);
        wifi_dclient = NULL;
   }
   printk(KERN_ERR "wifi_dclient unregister success!\n");
   return;
}
EXPORT_SYMBOL(wifi_dsm_unregister);

void wifi_dsm_report_info(int dsm_id, const char *fmt, ...)
{
    char buf[DSM_WIFI_BUF_SIZE] = {0};
    va_list args;

    va_start(args, fmt);
    if(wifi_dclient && !dsm_client_ocuppy(wifi_dclient)) {
        if(fmt) {
            vsnprintf(buf, DSM_WIFI_BUF_SIZE, fmt, args);
            dsm_client_record(wifi_dclient, buf);
        }
        dsm_client_notify(wifi_dclient, dsm_id);
        printk(KERN_ERR "notify dsm_id=%d success", dsm_id);
    } else {
        printk(KERN_ERR "notify dsm_id=%d failed", dsm_id);
    }
    va_end(args);
    return;
}
EXPORT_SYMBOL(wifi_dsm_report_info);

#endif

