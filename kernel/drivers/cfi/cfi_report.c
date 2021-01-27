#include <chipset_common/security/check_root.h>
#ifdef CONFIG_HUAWEI_VENDOR_EXCEPTION
#include <huawei_platform/vendor_exception/vendor_exception.h>
#else
#include <linux/bug.h>
#endif

void __cfi_report(void)
{
	pr_err("CFI exception detected\n");
	stp_save_trace_log(STP_NAME_CFI);

#ifdef CONFIG_HUAWEI_VENDOR_EXCEPTION
	VENDOR_EXCEPTION(VENDOR_MODID_AP_S_CFI, 0, 0);
#else
	BUG();
#endif
}
EXPORT_SYMBOL(__cfi_report);

void __efistub___cfi_report(void)
{
	__cfi_report();
}
EXPORT_SYMBOL(__efistub___cfi_report);
