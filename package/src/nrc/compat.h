/*
 * Copyright (c) 2016-2019 Newracom, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __COMPAT_H__
#define __COMPAT_H__

#include <net/mac80211.h>
#include "nrc.h"
#include "wim.h"

#if !defined(ieee80211_hw_set)
#define ieee80211_hw_set(hw, flg) ((hw)->flags |= IEEE80211_HW_##flg)
#endif

#if !defined(CONFIG_SUPPORT_BEACON_TEMPLATE)
#define ieee80211_beacon_get_template(hw, vif, N) ieee80211_beacon_get(hw, vif)
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 15, 0)
#define del_timer	timer_delete
#define del_timer_sync	timer_delete_sync
#define from_timer(x,y,z)	timer_container_of(x,y,z)
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 16, 0)
#define try_to_del_timer_sync	timer_delete_sync_try
#endif

#ifndef MAC80211_VERSION_CODE
#define MAC80211_VERSION_CODE LINUX_VERSION_CODE
#endif

#endif
