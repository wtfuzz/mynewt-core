/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef _HW_DRIVERS_IPC_NRF5340_H
#define _HW_DRIVERS_IPC_NRF5340_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type of callback called when signal was received on IPC
 *
 * @param channel     IPC channel number the signal was received on
 * @param user_data   User data
 */
typedef void (*ipc_nrf5340_recv_cb)(int channel, void *user_data);

/**
 * Initialize IPC driver. Should be called only once. On application core
 * this also releases networking core force-reset and thus enable it.
 */
void ipc_nrf5340_init(void);

/**
 * Reset IPC and NetCore. Can be used to re-sync with Network Core without
 * restarting Application Core.
 */
void ipc_nrf5340_reset(void);

/**
 * Enable reception on specified IPC channel. Calling with NULL callback
 * disables reception.
 *
 * @param channel     IPC channel number to receive on
 * @param cb          Callback to be called when IPC signal is received
 * @param user_data   User data passed to callback
 */
void ipc_nrf5340_recv(int channel, ipc_nrf5340_recv_cb cb, void *user_data);

/**
 * Sends data over specified IPC channel. IPC uses ring buffer for data passing.
 * If IPC_NRF5340_BLOCKING_WRITE is not enable and there is not enough space to
 * store data SYS_ENOMEM error is returned.
 *
 * @param channel     IPC channel number to send on
 * @param data        Data to be sent over IPC. If this is NULL only signal is
 *                    sent
 * @param len         Data length
 *
 * @return            0 on success and negative error on failure
 */
int ipc_nrf5340_send(int channel, const void *data, uint16_t len);

/**
 * Reads data from IPC ring buffer to specified flat buffer. Should be used only
 * from ipc_nrf5340_recv_cb context.
 *
 * @param channel     IPC channel number to read from
 * @param buf         Buffer to read data to
 * @param len         Length of buffer
 *
 * @return            Number of bytes read from IPC
 */
uint16_t ipc_nrf5340_read(int channel, void *buf, uint16_t len);

/**
 * Reads data from IPC ring buffer to specified mbuf. Should be used only
 * from ipc_nrf5340_recv_cb context.
 *
 * @param channel     IPC channel number to read from
 * @param om          mbuf to read data to
 * @param len         Length of buffer
 *
 * @return            Number of bytes read from IPC.
 */
uint16_t ipc_nrf5340_read_om(int channel, struct os_mbuf *om, uint16_t len);

/**
 * Returns number of data bytes available in IPC ring buffer. Should be used
 * only from ipc_nrf5340_recv_cb context.
 *
 * @param channel     IPC channel number
 *
 * @return            Number of bytes available in IPC ring buffer
 */
uint16_t ipc_nrf5340_available(int channel);

/**
 * Consumes data from IPC ring buffer without copying. Should be used only
 * from ipc_nrf5340_recv_cb context.
 *
 * @param channel     IPC channel number to consume from
 * @param len         Number of bytes to consume
 *
 * @return            Number of bytes consumed from IPC.
 */
uint16_t ipc_nrf5340_consume(int channel, uint16_t len);

/**
 * Set function to be called when net core restarts.
 *
 * Note: Function will be called from IPC interrupt context.
 *
 * @param on_crash - function to be called
 */
void ipc_nrf5340_set_net_core_restart_cb(void (*on_restart)(void));

#ifdef __cplusplus
}
#endif

#endif /* _HW_DRIVERS_IPC_NRF5340_H */
