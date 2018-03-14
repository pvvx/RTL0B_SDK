/**
  ******************************************************************************
  * @file	 rtl8711b_adc.h
  * @author
  * @version V1.0.0
  * @date	 2016-05-17
  * @brief	 This file contains all the functions prototypes for the ADC firmware
  *			 library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _RTL8710B_ADC_H_
#define _RTL8710B_ADC_H_

/** @addtogroup AmebaZ_Periph_Driver
  * @{
  */

/** @defgroup ADC
  * @brief ADC driver modules
  * @{
  */

/** @addtogroup ADC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * ADC:
  *		- Base Address: ADC
  *		- Channel: 4
  *		- 0 internal ADC channel (reserved for internal thermal meter output)
  *		- 1&3 (GPIOA19&GPIOA20)	 external ADC channel (normal ADC, power < 3.3V)
  *		- 2 (VBAT) external ADC VBAT channel (batery voltage measurement, power < 5V, 1/4 voltage will be get)
  *		- Sample rate: max frequency up to 1MHz per channel, configurable
  *		- Resolution: 12 bit, but when there is only set of output(audio mode), resolution is 16bit
  *		- Analog signal sampling: support 0 ~ 3.3V
  *		- IRQ: ADC_IRQ
  *		- Support one shot mode for power save
  *		- GDMA source handshake interface: 12
  *
  *****************************************************************************************
  * One shot mode
  *****************************************************************************************
  * Timer trigger one shot sampling
  * ADC does one single conversion
  *
  *****************************************************************************************
  * How to use ADC in interrupt mode
  *****************************************************************************************
  *		  To use ADC in interrupt mode, the following steps are mandatory:
  *
  *		 1. Enable the ADC interface clock:
  *			RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);
  *
  *		 2. Fill the ADC_InitStruct with the desired parameters.
  *
  *		 3. Init Hardware use step2 parameters.
  *			ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
  *
  *		 4. Clear ADC interrupt:
  *			ADC_INTClear()
  *
  *		 5. To configure interrupts:
  *			ADC_INTConfig(IntrMSK, ENABLE)
  *
  *		 6. Activate the ADC:
  *			ADC_Cmd(ENABLE).
  *
  *		 @note1	If use ADC compare mode, call ADC_SetComp(ChanIdx, ADCCompTD, ADCCompCtrl) to configure
  *
  *		 @note2	If power save needed, call ADC_SetOneShot(ENABLE, Period, InterruptThresh) to enable ADC one
  *			shot mode(2*Period ms interval will set)
  *
  *****************************************************************************************
  * How to use ADC in DMA mode
  *****************************************************************************************
  *		  To use ADC in DMA mode, the following steps are mandatory:
  *
  *		 1. Enable the ADC interface clock:
  *			RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);
  *
  *		 2. Fill the ADC_InitStruct with the desired parameters.
  *
  *		 3. Init Hardware use step2 parameters.
  *			ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
  *
  *		 4. Disable ADC interrupts:
  *			ADC_INTConfig(IntrMSK,DISABLE)
  *
  *		 5. Clear ADC interrupt:
  *			ADC_INTClear()
  *
  *		 6. GDMA related configurations(source address/destination address/block size etc.).
  *
  *		 7. Configure GDMA with the corresponding configuration.
  *			GDMA_Init()
  *
  *		 8. Enable the GDMA:
  *			GDMA_Cmd().
  *
  *		 9. Activate the ADC peripheral:
  *			ADC_Cmd(ENABLE).
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup ADC_Exported_Types ADC Exported Types
  * @{
  */

/**
  * @brief	ADC Init structure definition
  */
typedef struct {
	u32 ADC_CompOnly;		/*!< Specifies ADC compare mode only enable (without FIFO enable)
								This parameter can be set to ENABLE or DISABLE
								ADC_InitStruct() set DISABLE */

	u32 ADC_OneShotEn;		/*!< Specifies ADC one-shot mode enable
								This parameter can be set to ENABLE or DISABLE
								ADC_InitStruct() set DISABLE */

	u32 ADC_OverWREn;		/*!< Specifies ADC overwrite mode enable
								This parameter can be set to ENABLE or DISABLE
								ADC_InitStruct() set DISABLE */

	u32 ADC_Endian;			/*!< Specifies ADC endian selection, but actually it's for 32-bit ADC data swap control
								This parameter can be set to 1'b0: no swap, 'b1: swap the upper 16-bit and the lower 16-bit
								ADC_InitStruct() set 0 */

	u32 ADC_BurstSz;		/*!< Specifies ADC DMA operation threshold
								This parameter must range from 1 to 32 dword.
								@note This parameter had better not set too big
								ADC_InitStruct() set 8 */

	u32 ADC_OneShotTD;		/*!< Specifies ADC one shot mode threshold
								This parameter must range from 1 to 32 dword.
								@note This parameter had better not set too big
								ADC_InitStruct() set 8 */

	u32 ADC_SampleClk;		/*!< ADC Sample clock, This parameter can be a value of @ref ADC_SampleClk_Definitions
								ADC_InitStruct() set 3 */

	u32 ADC_DbgSel;		/*!< Specifies ADC select debug mode, can be @ref  ADC_DBG_Select, user dont set this parameter.
								ADC_InitStruct() set 0	*/
} ADC_InitTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants ADC Exported Constants
  * @{
  */

/** @defgroup ADC_Chn_Selection
  * @{
  */
#define ADC0_SEL							((u8)0x00)
#define ADC1_SEL							((u8)0x01)
#define ADC2_SEL							((u8)0x02)
#define ADC3_SEL							((u8)0x03)

#define IS_ADC_CHN_SEL(SEL)		(((SEL) == ADC0_SEL) || \
									((SEL) == ADC1_SEL) || \
									((SEL) == ADC2_SEL) || \
									((SEL) == ADC3_SEL))

/**
  * @}
  */


/** @defgroup ADC_SampleClk_Definitions
  * @{
  */
#define ADC_SAMPLE_CLK_2_1P5625M		((u32)0x00000000)
#define ADC_SAMPLE_CLK_2_3P1250M		((u32)0x00000001)
#define ADC_SAMPLE_CLK_2_6P2500M		((u32)0x00000002)
#define ADC_SAMPLE_CLK_2_12P500M		((u32)0x00000003)
#define ADC_SAMPLE_CLK_2_25P000M		((u32)0x00000004) // ?

#define IS_ADC_SAMPLE_CLK(CLK)	   (((CLK) == ADC_SAMPLE_CLK_2_1P5625M) || \
										((CLK) == ADC_SAMPLE_CLK_2_3P1250M) || \
										((CLK) == ADC_SAMPLE_CLK_2_6P2500M) || \
										((CLK) == ADC_SAMPLE_CLK_2_12P500M))

/**
  * @}
  */


/** @defgroup ADC_Data_Endian
  * @{
  */
#define ADC_DATA_ENDIAN_LITTLE			((u32)0x00000000)
#define ADC_DATA_ENDIAN_BIG				((u32)0x00000001)

#define IS_ADC_DATA_ENDIAN(ENDIAN)	   (((ENDIAN) == ADC_DATA_ENDIAN_LITTLE) || \
										((ENDIAN) == ADC_DATA_ENDIAN_BIG))

/**
  * @}
  */

/** @defgroup ADC_Compare_Set
  * @{
  */
#define ADC_COMP_SMALLER_THAN			((u32)0x00000000)
#define ADC_COMP_GREATER_THAN			((u32)0x00000001)
/**
  * @}
  */

/** @defgroup ADC_DBG_Select
  * @{
  */
#define ADC_DBG_ADC_POWER				((u32)0x00000000)
#define ADC_DBG_ADC_ONESHOT				((u32)0x00000001)
#define ADC_DBG_ADC_FIFO				((u32)0x00000002)
#define ADC_DBG_ADC_DATA				((u32)0x00000003)
#define ADC_DBG_ADC_CTRL				((u32)0x00000004)
#define ADC_DBG_ADC_RST					((u32)0x00000005)
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */
_LONG_CALL_ void ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
_LONG_CALL_ void ADC_InitStruct(ADC_InitTypeDef *ADC_InitStruct);
_LONG_CALL_ u32 ADC_Read(void);
_LONG_CALL_ u32 ADC_GetISR(void);
_LONG_CALL_ void ADC_Cmd(u32 ADCEn);
_LONG_CALL_ void ADC_INTConfig(u32 IntrMSK, u32 NewState);
_LONG_CALL_ void ADC_INTClear(void);
_LONG_CALL_ void ADC_INTClearPendingBits(u32 Mask);
_LONG_CALL_ void ADC_SetAnalog(ADC_InitTypeDef* ADC_InitStruct);
_LONG_CALL_ void ADC_SetComp(u8 ChanIdx, u16 ADCCompTD, u16 ADCCompCtrl);
_LONG_CALL_ void ADC_SetOneShot(u32 NewState, u32 PeriodMs, u32 InterruptThresh);
_LONG_CALL_ void ADC_ReceiveBuf(u32 *pBuf);
_LONG_CALL_ void ADC_SetAudio(u32 NewState);
_LONG_CALL_ u32 ADC_RXGDMA_Init(GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc, u8* pDataBuf, u32 DataLen);
/**
  * @}
  */

/* Registers Definitions --------------------------------------------------------*/
/**************************************************************************//**
 * @defgroup ADC_Register_Definitions ADC Register Definitions
 * @{
 *****************************************************************************/
 
/**************************************************************************//**
 * @defgroup ADC_CONTROL
 * @{
 *****************************************************************************/
#define BIT_SHIFT_ADC_DBG_SEL 24
#define BIT_MASK_ADC_DBG_SEL 0x7
#define BIT_ADC_DBG_SEL(x)						(((x) & BIT_MASK_ADC_DBG_SEL) << BIT_SHIFT_ADC_DBG_SEL)
#define BIT_CTRL_ADC_DBG_SEL(x)					(((x) & BIT_MASK_ADC_DBG_SEL) << BIT_SHIFT_ADC_DBG_SEL)
#define BIT_GET_ADC_DBG_SEL(x)					(((x) >> BIT_SHIFT_ADC_DBG_SEL) & BIT_MASK_ADC_DBG_SEL)

#define BIT_SHIFT_ADC_THRESHOLD 16
#define BIT_MASK_ADC_THRESHOLD 0x3f
#define BIT_ADC_THRESHOLD(x)					(((x) & BIT_MASK_ADC_THRESHOLD) << BIT_SHIFT_ADC_THRESHOLD)
#define BIT_CTRL_ADC_THRESHOLD(x)				(((x) & BIT_MASK_ADC_THRESHOLD) << BIT_SHIFT_ADC_THRESHOLD)
#define BIT_GET_ADC_THRESHOLD(x)				(((x) >> BIT_SHIFT_ADC_THRESHOLD) & BIT_MASK_ADC_THRESHOLD)

#define BIT_SHIFT_ADC_BURST_SIZE 8
#define BIT_MASK_ADC_BURST_SIZE 0x1f
#define BIT_ADC_BURST_SIZE(x)					(((x) & BIT_MASK_ADC_BURST_SIZE) << BIT_SHIFT_ADC_BURST_SIZE)
#define BIT_CTRL_ADC_BURST_SIZE(x)				(((x) & BIT_MASK_ADC_BURST_SIZE) << BIT_SHIFT_ADC_BURST_SIZE)
#define BIT_GET_ADC_BURST_SIZE(x)				(((x) >> BIT_SHIFT_ADC_BURST_SIZE) & BIT_MASK_ADC_BURST_SIZE)

#define BIT_ADC_ENDIAN							BIT(3)
#define BIT_SHIFT_ADC_ENDIAN					3
#define BIT_MASK_ADC_ENDIAN						0x1
#define BIT_CTRL_ADC_ENDIAN(x)					(((x) & BIT_MASK_ADC_ENDIAN) << BIT_SHIFT_ADC_ENDIAN)

#define BIT_ADC_OVERWRITE						BIT(2)
#define BIT_SHIFT_ADC_OVERWRITE					2
#define BIT_MASK_ADC_OVERWRITE					0x1
#define BIT_CTRL_ADC_OVERWRITE(x)				(((x) & BIT_MASK_ADC_OVERWRITE) << BIT_SHIFT_ADC_OVERWRITE)

#define BIT_ADC_ONESHOT							BIT(1)
#define BIT_SHIFT_ADC_ONESHOT					1
#define BIT_MASK_ADC_ONESHOT					0x1
#define BIT_CTRL_ADC_ONESHOT(x)					(((x) & BIT_MASK_ADC_ONESHOT) << BIT_SHIFT_ADC_ONESHOT)

#define BIT_ADC_COMP_ONLY						BIT(0)
#define BIT_SHIFT_ADC_COMP_ONLY					0
#define BIT_MASK_ADC_COMP_ONLY					0x1
#define BIT_CTRL_ADC_COMP_ONLY(x)				(((x) & BIT_MASK_ADC_COMP_ONLY) << BIT_SHIFT_ADC_COMP_ONLY)
/** @} */

/**************************************************************************//**
 * @defgroup ADC_INTR_EN
 * @{
 *****************************************************************************/
#define BIT_ADC_AWAKE_CPU_EN					BIT(7)
#define BIT_SHIFT_ADC_AWAKE_CPU_EN				7
#define BIT_MASK_ADC_AWAKE_CPU_EN				0x1
#define BIT_CTRL_ADC_AWAKE_CPU_EN(x)			(((x) & BIT_MASK_ADC_AWAKE_CPU_EN) << BIT_SHIFT_ADC_AWAKE_CPU_EN)

#define BIT_ADC_FIFO_RD_ERROR_EN				BIT(6)
#define BIT_SHIFT_ADC_FIFO_RD_ERROR_EN			6
#define BIT_MASK_ADC_FIFO_RD_ERROR_EN			0x1
#define BIT_CTRL_ADC_FIFO_RD_ERROR_EN(x)		(((x) & BIT_MASK_ADC_FIFO_RD_ERROR_EN) << BIT_SHIFT_ADC_FIFO_RD_ERROR_EN)

#define BIT_ADC_FIFO_RD_REQ_EN					BIT(5)
#define BIT_SHIFT_ADC_FIFO_RD_REQ_EN			5
#define BIT_MASK_ADC_FIFO_RD_REQ_EN				0x1
#define BIT_CTRL_ADC_FIFO_RD_REQ_EN(x)			(((x) & BIT_MASK_ADC_FIFO_RD_REQ_EN) << BIT_SHIFT_ADC_FIFO_RD_REQ_EN)

#define BIT_ADC_FIFO_FULL_EN					BIT(4)
#define BIT_SHIFT_ADC_FIFO_FULL_EN				4
#define BIT_MASK_ADC_FIFO_FULL_EN				0x1
#define BIT_CTRL_ADC_FIFO_FULL_EN(x)			(((x) & BIT_MASK_ADC_FIFO_FULL_EN) << BIT_SHIFT_ADC_FIFO_FULL_EN)

#define BIT_ADC_COMP_3_EN						BIT(3)
#define BIT_SHIFT_ADC_COMP_3_EN					3
#define BIT_MASK_ADC_COMP_3_EN					0x1
#define BIT_CTRL_ADC_COMP_3_EN(x)				(((x) & BIT_MASK_ADC_COMP_3_EN) << BIT_SHIFT_ADC_COMP_3_EN)

#define BIT_ADC_COMP_2_EN						BIT(2)
#define BIT_SHIFT_ADC_COMP_2_EN					2
#define BIT_MASK_ADC_COMP_2_EN					0x1
#define BIT_CTRL_ADC_COMP_2_EN(x)				(((x) & BIT_MASK_ADC_COMP_2_EN) << BIT_SHIFT_ADC_COMP_2_EN)

#define BIT_ADC_COMP_1_EN						BIT(1)
#define BIT_SHIFT_ADC_COMP_1_EN					1
#define BIT_MASK_ADC_COMP_1_EN					0x1
#define BIT_CTRL_ADC_COMP_1_EN(x)				(((x) & BIT_MASK_ADC_COMP_1_EN) << BIT_SHIFT_ADC_COMP_1_EN)

#define BIT_ADC_COMP_0_EN						BIT(0)
#define BIT_SHIFT_ADC_COMP_0_EN					0
#define BIT_MASK_ADC_COMP_0_EN					0x1
#define BIT_CTRL_ADC_COMP_0_EN(x)				(((x) & BIT_MASK_ADC_COMP_0_EN) << BIT_SHIFT_ADC_COMP_0_EN)
/** @} */

/**************************************************************************//**
 * @defgroup ADC_INTR_STATUS
 * @{
 *****************************************************************************/
#define BIT_ADC_FIFO_THRESHOLD					BIT(7)
#define BIT_SHIFT_ADC_FIFO_THRESHOLD			7
#define BIT_MASK_ADC_FIFO_THRESHOLD				0x1
#define BIT_CTRL_ADC_FIFO_THRESHOLD(x)			(((x) & BIT_MASK_ADC_FIFO_THRESHOLD) << BIT_SHIFT_ADC_FIFO_THRESHOLD)

#define BIT_ADC_FIFO_RD_ERROR					BIT(6)
#define BIT_SHIFT_ADC_FIFO_RD_ERROR_ST			6
#define BIT_MASK_ADC_FIFO_RD_ERROR_ST			0x1
#define BIT_CTRL_ADC_FIFO_RD_ERROR_ST(x)		(((x) & BIT_MASK_ADC_FIFO_RD_ERROR_ST) << BIT_SHIFT_ADC_FIFO_RD_ERROR_ST)

#define BIT_ADC_FIFO_RD_REQ						BIT(5)
#define BIT_SHIFT_ADC_FIFO_RD_REQ_ST			5
#define BIT_MASK_ADC_FIFO_RD_REQ_ST				0x1
#define BIT_CTRL_ADC_FIFO_RD_REQ_ST(x)			(((x) & BIT_MASK_ADC_FIFO_RD_REQ_ST) << BIT_SHIFT_ADC_FIFO_RD_REQ_ST)

#define BIT_ADC_FIFO_FULL						BIT(4)
#define BIT_SHIFT_ADC_FIFO_FULL_ST				4
#define BIT_MASK_ADC_FIFO_FULL_ST				0x1
#define BIT_CTRL_ADC_FIFO_FULL_ST(x)			(((x) & BIT_MASK_ADC_FIFO_FULL_ST) << BIT_SHIFT_ADC_FIFO_FULL_ST)

#define BIT_ADC_COMP_3							BIT(3)
#define BIT_SHIFT_ADC_COMP_3_ST					3
#define BIT_MASK_ADC_COMP_3_ST					0x1
#define BIT_CTRL_ADC_COMP_3_ST(x)				(((x) & BIT_MASK_ADC_COMP_3_ST) << BIT_SHIFT_ADC_COMP_3_ST)

#define BIT_ADC_COMP_2							BIT(2)
#define BIT_SHIFT_ADC_COMP_2_ST					2
#define BIT_MASK_ADC_COMP_2_ST					0x1
#define BIT_CTRL_ADC_COMP_2_ST(x)				(((x) & BIT_MASK_ADC_COMP_2_ST) << BIT_SHIFT_ADC_COMP_2_ST)

#define BIT_ADC_COMP_1							BIT(1)
#define BIT_SHIFT_ADC_COMP_1_ST					1
#define BIT_MASK_ADC_COMP_1_ST					0x1
#define BIT_CTRL_ADC_COMP_1_ST(x)				(((x) & BIT_MASK_ADC_COMP_1_ST) << BIT_SHIFT_ADC_COMP_1_ST)

#define BIT_ADC_COMP_0							BIT(0)
#define BIT_SHIFT_ADC_COMP_0_ST					0
#define BIT_MASK_ADC_COMP_0_ST					0x1
#define BIT_CTRL_ADC_COMP_0_ST(x)				(((x) & BIT_MASK_ADC_COMP_0_ST) << BIT_SHIFT_ADC_COMP_0_ST)
/** @} */
/**************************************************************************//**
 * @defgroup ADC_COMP_VALUE_L
 * @{
 *****************************************************************************/
#define BIT_SHIFT_ADC_COMP_TH_1 16
#define BIT_MASK_ADC_COMP_TH_1 0xffff
#define BIT_ADC_COMP_TH_1(x)					(((x) & BIT_MASK_ADC_COMP_TH_1) << BIT_SHIFT_ADC_COMP_TH_1)
#define BIT_CTRL_ADC_COMP_TH_1(x)				(((x) & BIT_MASK_ADC_COMP_TH_1) << BIT_SHIFT_ADC_COMP_TH_1)
#define BIT_GET_ADC_COMP_TH_1(x)				(((x) >> BIT_SHIFT_ADC_COMP_TH_1) & BIT_MASK_ADC_COMP_TH_1)


#define BIT_SHIFT_ADC_COMP_TH_0 0
#define BIT_MASK_ADC_COMP_TH_0 0xffff
#define BIT_ADC_COMP_TH_0(x)					(((x) & BIT_MASK_ADC_COMP_TH_0) << BIT_SHIFT_ADC_COMP_TH_0)
#define BIT_CTRL_ADC_COMP_TH_0(x)				(((x) & BIT_MASK_ADC_COMP_TH_0) << BIT_SHIFT_ADC_COMP_TH_0)
#define BIT_GET_ADC_COMP_TH_0(x)				(((x) >> BIT_SHIFT_ADC_COMP_TH_0) & BIT_MASK_ADC_COMP_TH_0)
/** @} */
/**************************************************************************//**
 * @defgroup ADC_COMP_VALUE_H
 * @{
 *****************************************************************************/
#define BIT_SHIFT_ADC_COMP_TH_3 16
#define BIT_MASK_ADC_COMP_TH_3 0xffff
#define BIT_ADC_COMP_TH_3(x)					(((x) & BIT_MASK_ADC_COMP_TH_3) << BIT_SHIFT_ADC_COMP_TH_3)
#define BIT_CTRL_ADC_COMP_TH_3(x)				(((x) & BIT_MASK_ADC_COMP_TH_3) << BIT_SHIFT_ADC_COMP_TH_3)
#define BIT_GET_ADC_COMP_TH_3(x)				(((x) >> BIT_SHIFT_ADC_COMP_TH_3) & BIT_MASK_ADC_COMP_TH_3)


#define BIT_SHIFT_ADC_COMP_TH_2 0
#define BIT_MASK_ADC_COMP_TH_2 0xffff
#define BIT_ADC_COMP_TH_2(x)					(((x) & BIT_MASK_ADC_COMP_TH_2) << BIT_SHIFT_ADC_COMP_TH_2)
#define BIT_CTRL_ADC_COMP_TH_2(x)				(((x) & BIT_MASK_ADC_COMP_TH_2) << BIT_SHIFT_ADC_COMP_TH_2)
#define BIT_GET_ADC_COMP_TH_2(x)				(((x) >> BIT_SHIFT_ADC_COMP_TH_2) & BIT_MASK_ADC_COMP_TH_2)
/** @} */
/**************************************************************************//**
 * @defgroup ADC_COMP_SET
 * @{
 *****************************************************************************/
#define BIT_SHIFT_ADC_GREATER_THAN 0
#define BIT_MASK_ADC_GREATER_THAN 0xf
#define BIT_ADC_GREATER_THAN(x)					(((x) & BIT_MASK_ADC_GREATER_THAN) << BIT_SHIFT_ADC_GREATER_THAN)
#define BIT_CTRL_ADC_GREATER_THAN(x)			(((x) & BIT_MASK_ADC_GREATER_THAN) << BIT_SHIFT_ADC_GREATER_THAN)
#define BIT_GET_ADC_GREATER_THAN(x)				(((x) >> BIT_SHIFT_ADC_GREATER_THAN) & BIT_MASK_ADC_GREATER_THAN)
/** @} */

/**************************************************************************//**
 * @defgroup ADC_POWER
 * @{
 *****************************************************************************/
#define BIT_SHIFT_ADC_PWR_CUT_CNTR 16
#define BIT_MASK_ADC_PWR_CUT_CNTR 0xff
#define BIT_ADC_PWR_CUT_CNTR(x)					(((x) & BIT_MASK_ADC_PWR_CUT_CNTR) << BIT_SHIFT_ADC_PWR_CUT_CNTR)
#define BIT_CTRL_ADC_PWR_CUT_CNTR(x)			(((x) & BIT_MASK_ADC_PWR_CUT_CNTR) << BIT_SHIFT_ADC_PWR_CUT_CNTR)
#define BIT_GET_ADC_PWR_CUT_CNTR(x)				(((x) >> BIT_SHIFT_ADC_PWR_CUT_CNTR) & BIT_MASK_ADC_PWR_CUT_CNTR)

#define BIT_ADC_FIFO_ON_ST						BIT(11)
#define BIT_SHIFT_ADC_FIFO_ON_ST				11
#define BIT_MASK_ADC_FIFO_ON_ST					0x1
#define BIT_CTRL_ADC_FIFO_ON_ST(x)				(((x) & BIT_MASK_ADC_FIFO_ON_ST) << BIT_SHIFT_ADC_FIFO_ON_ST)

#define BIT_ADC_ISO_ON_ST						BIT(10)
#define BIT_SHIFT_ADC_ISO_ON_ST					10
#define BIT_MASK_ADC_ISO_ON_ST					0x1
#define BIT_CTRL_ADC_ISO_ON_ST(x)				(((x) & BIT_MASK_ADC_ISO_ON_ST) << BIT_SHIFT_ADC_ISO_ON_ST)

#define BIT_ADC_PWR33_ON_ST						BIT(9)
#define BIT_SHIFT_ADC_PWR33_ON_ST				9
#define BIT_MASK_ADC_PWR33_ON_ST				0x1
#define BIT_CTRL_ADC_PWR33_ON_ST(x)				(((x) & BIT_MASK_ADC_PWR33_ON_ST) << BIT_SHIFT_ADC_PWR33_ON_ST)

#define BIT_ADC_PWR12_ON_ST						BIT(8)
#define BIT_SHIFT_ADC_PWR12_ON_ST				8
#define BIT_MASK_ADC_PWR12_ON_ST				0x1
#define BIT_CTRL_ADC_PWR12_ON_ST(x)				(((x) & BIT_MASK_ADC_PWR12_ON_ST) << BIT_SHIFT_ADC_PWR12_ON_ST)

#define BIT_ADC_ISO_MANUAL						BIT(3)
#define BIT_SHIFT_ADC_ISO_MANUAL				3
#define BIT_MASK_ADC_ISO_MANUAL					0x1
#define BIT_CTRL_ADC_ISO_MANUAL(x)				(((x) & BIT_MASK_ADC_ISO_MANUAL) << BIT_SHIFT_ADC_ISO_MANUAL)

#define BIT_ADC_PWR33_MANUAL					BIT(2)
#define BIT_SHIFT_ADC_PWR33_MANUAL				2
#define BIT_MASK_ADC_PWR33_MANUAL				0x1
#define BIT_CTRL_ADC_PWR33_MANUAL(x)			(((x) & BIT_MASK_ADC_PWR33_MANUAL) << BIT_SHIFT_ADC_PWR33_MANUAL)

#define BIT_ADC_PWR12_MANUAL					BIT(1)
#define BIT_SHIFT_ADC_PWR12_MANUAL				1
#define BIT_MASK_ADC_PWR12_MANUAL				0x1
#define BIT_CTRL_ADC_PWR12_MANUAL(x)			(((x) & BIT_MASK_ADC_PWR12_MANUAL) << BIT_SHIFT_ADC_PWR12_MANUAL)

#define BIT_ADC_PWR_AUTO						BIT(0)
#define BIT_SHIFT_ADC_PWR_AUTO					0
#define BIT_MASK_ADC_PWR_AUTO					0x1
#define BIT_CTRL_ADC_PWR_AUTO(x)				(((x) & BIT_MASK_ADC_PWR_AUTO) << BIT_SHIFT_ADC_PWR_AUTO)
/** @} */

/**************************************************************************//**
 * @defgroup ADC_ANAPAR_AD0
 * @{
 *****************************************************************************/

#define BIT_SHIFT_ADC_SAMPLE_CLK				14
#define BIT_MASK_ADC_SAMPLE_CLK					0x3
#define BIT_CTRL_ADC_SAMPLE_CLKL(x)				(((x) & BIT_MASK_ADC_SAMPLE_CLK) << BIT_SHIFT_ADC_SAMPLE_CLK)

#define BIT_ADC_SAMPLE_CLK						((u32)0x00000003 << 14)

#define BIT_ADC_DEM_EN							BIT(3)

#define BIT_ADC_CHOP_EN							BIT(2)

#define BIT_ADC_AUDIO_EN						BIT(1)

#define BIT_ADC_EN_MANUAL						BIT(0)

/** @} */

/**************************************************************************//**
 * @defgroup ADC_ANAPAR_AD1
 * @{
 *****************************************************************************/
#define BIT_SHIFT_DOWN_SAMPLE_RATE				18
#define BIT_MASK_DOWN_SAMPLE_RATE				0x7
#define BIT_CTRL_DOWN_SAMPLE_RATE(x)			(((x) & BIT_MASK_DOWN_SAMPLE_RATE) << BIT_SHIFT_DOWN_SAMPLE_RATE)
#define BIT_ADC_DOWN_SAMPLE_RATE				((u32)0x00000007 << 18) /*!< ADC Factor, down sample rate, 000: 25M/2^7, 001: 25M/2^8 */
#define BIT_ADC_DECIMATION_FILTER_ORDER			((u32)0x00000001 << 12) /*!< decimation filter order, 0: order3, 1: order4 */
#define BIT_ADC_EXT_VREF_EN						((u32)0x00000001 << 2)
#define BIT_ADC_FEEDBK_CAPACITY_VAL				((u32)0x00000001 << 1)
#define BIT_ADC_DIGITAL_RST_BAR					((u32)0x00000001 << 0)
/** @} */
/** @} */

/**
  * @}
  */

/**
  * @}
  */

/* Other Definitions --------------------------------------------------------*/
extern u32 ADC_AnaparAd[6];

#endif /* _RTL8710B_ADC_H_ */

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
