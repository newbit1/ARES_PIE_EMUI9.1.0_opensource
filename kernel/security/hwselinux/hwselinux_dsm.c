/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2018. All rights reserved.
 * Description: the hwselinux_dsm.c for selinux radar
 * Author: wangjin <wangjin79@huawei.com>
 * Create: 2019-02-21
 */

#include "hwselinux_dsm.h"

void selinux_dsm_process(struct audit_buffer *ab, void *a, u32 denied)
{
	if (!ab || !a)
		return;

#ifdef CONFIG_LOG_EXCEPTION
	if (!runmode_is_factory()) {
		unsigned int log_type = get_logusertype_flag();

		if ((log_type == BETA_USER) || (log_type == OVERSEA_USER))
			audit_log_format(ab, " dsm=QC_P1");
	}
#endif
}
