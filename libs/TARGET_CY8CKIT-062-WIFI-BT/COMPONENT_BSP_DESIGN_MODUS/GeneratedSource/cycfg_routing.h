/*******************************************************************************
* File Name: cycfg_routing.h
*
* Description:
* Establishes all necessary connections between hardware elements.
* This file was automatically generated and should not be modified.
* Tools Package 2.1.0.1266
* psoc6pdl 1.5.1.2655
* personalities_2.0 2.0.0.0
* udd 1.2.0.136
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
********************************************************************************/

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cycfg_notices.h"
void init_cycfg_routing(void);
#define init_cycfg_connectivity() init_cycfg_routing()
#define ioss_0_port_0_pin_0_ANALOG P0_0_SRSS_WCO_IN
#define ioss_0_port_0_pin_1_ANALOG P0_1_SRSS_WCO_OUT
#define ioss_0_port_10_pin_4_HSIOM P10_4_AUDIOSS_PDM_CLK
#define ioss_0_port_10_pin_5_HSIOM P10_5_AUDIOSS_PDM_DATA
#define ioss_0_port_5_pin_1_HSIOM P5_1_AUDIOSS_TX_SCK
#define ioss_0_port_5_pin_2_HSIOM P5_2_AUDIOSS_TX_WS
#define ioss_0_port_5_pin_3_HSIOM P5_3_AUDIOSS_TX_SDO
#define ioss_0_port_6_pin_4_HSIOM P6_4_CPUSS_SWJ_SWO_TDO
#define ioss_0_port_6_pin_6_HSIOM P6_6_CPUSS_SWJ_SWDIO_TMS
#define ioss_0_port_6_pin_7_HSIOM P6_7_CPUSS_SWJ_SWCLK_TCLK

#define CYBSP_DMA_I2S_tr_in_0_TRIGGER_OUT TRIG0_OUT_CPUSS_DW0_TR_IN3
#define CYBSP_DMA_PDM_tr_in_0_TRIGGER_OUT TRIG0_OUT_CPUSS_DW0_TR_IN2
#define CYBSP_I2S_tr_i2s_tx_req_0_TRIGGER_IN_0 TRIG0_IN_TR_GROUP13_OUTPUT13
#define CYBSP_I2S_tr_i2s_tx_req_0_TRIGGER_IN_1 TRIG13_IN_AUDIOSS_TR_I2S_TX_REQ
#define CYBSP_PDM_tr_pdm_rx_req_0_TRIGGER_IN_0 TRIG0_IN_TR_GROUP13_OUTPUT5
#define CYBSP_PDM_tr_pdm_rx_req_0_TRIGGER_IN_1 TRIG13_IN_AUDIOSS_TR_PDM_RX_REQ

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_ROUTING_H */
