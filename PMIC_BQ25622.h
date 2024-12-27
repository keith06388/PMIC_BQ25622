#ifndef PMIC_BQ25622_H
#define PMIC_BQ25622_H

#include "Arduino.h"
#include "Wire.h"

typedef enum {
    BQ25622_ADDR = 0x6B
} bq25622_addr_t;

typedef enum {
    Charge_Current_Limit_LSB	= 0x02,
    Charge_Current_Limit_MSB   	= 0x03,
	Charge_Voltage_Limit_LSB	= 0x04,
	Charge_Voltage_Limit_MSB	= 0x05,
    Input_Voltage_Limit_LSB    	= 0x08,
    Input_Voltage_Limit_MSB    	= 0x09,
	Precharge_Control_LSB		= 0x10,
	Precharge_Control_MSB		= 0x11,
    Termination_Control_LSB    	= 0x12,
    Termination_Control_MSB  	= 0x13,
    Charge_Control_0        	= 0x14,
	Charger_Control_2       	= 0x17,
    Charger_Control_4       	= 0x19,
    NTC_Control_0    			= 0x1A,
    ADC_Control       			= 0x26,
    ADC_Function_Disable_0  	= 0x27,
    IBAT_ADC_LSB   				= 0x2A,
    IBAT_ADC_MSB       			= 0x2B,
    VBUS_ADC_LSB      			= 0x2C,
    VBUS_ADC_MSB        		= 0x2D,
    VBAT_ADC_LSB        		= 0x30,
    VBAT_ADC_MSB       			= 0x31,
} bq25622_reg_t;

typedef enum {
    BQ_OK = 0x00,
    BQ_RANGE_ERR
} bq25622_error_t;

typedef struct {
    uint8_t reserved:6;
    uint8_t ichg:6;
    uint8_t reserved1:4;
} ichg_reg_t __attribute__(());

typedef struct {
    uint8_t reserved:3;
    uint16_t vreg:9;
    uint8_t reserved1:4;
} vreg_reg_t __attribute__(());

typedef struct {
    uint8_t reserved:5;
    uint16_t vindpm:9;
    uint8_t reserved1:2;
} vindpm_reg_t __attribute__(());

typedef struct {
	uint8_t reserved:4;
    uint8_t iprechg:5;
    uint8_t reserved1:7;
} ipre_reg_t __attribute__(());

typedef struct {
	uint8_t reserved:3;
    uint8_t iterm:6;
	uint8_t reserved1:7;
} iterm_reg_t __attribute__(());

typedef struct {
    uint8_t vrechg:1;
    uint8_t vindpm_bat_track:1;
    uint8_t en_term:1;
    uint8_t topoff_tmr:2;
    uint8_t itrickle:1;
    uint8_t q4_fullon:1;
    uint8_t q1_fullon:1;
} ctrl0_reg_t __attribute__(());

typedef struct {
    uint8_t vbus_ovp:1;
    uint8_t reserved1:1;
    uint8_t set_conv_strn:2;
    uint8_t set_conv_freq:2;
    uint8_t treg:1;
	uint8_t reg_rst:1;
} ctrl2_reg_t __attribute__(());

typedef struct {
    uint8_t chg_rate:2;
    uint8_t en_extlim:1;
    uint8_t reserved:2;
    uint8_t vbat_uvlo:1;
    uint8_t ibat_pk:2;
} ctrl4_reg_t __attribute__(());

typedef struct {
    uint8_t ts_iset_cool:2;
    uint8_t ts_iset_warm:2;
    uint8_t reserved:3;
    uint8_t ts_ignore:1;
} ntc_reg_t __attribute__(());

typedef struct {
    uint8_t reserved:2;
    uint8_t adc_avg_init:1;
    uint8_t adc_avg:1;
    uint8_t adc_sample:2;
    uint8_t adc_rate:1;
    uint8_t adc_en:1;
} adc_ctrl_reg_t __attribute__(());

typedef struct {
    uint8_t vpmid_adc_dis:1;
    uint8_t tdie_adc_dis:1;
    uint8_t ts_adc_dis:1;
    uint8_t vsys_adc_dis:1;
    uint8_t vbat_adc_dis:1;
    uint8_t vbus_adc_dis:1;
	uint8_t ibat_adc_dis:1;
    uint8_t ibus_adc_dis:1;
} adc_dis_reg_t __attribute__(());

typedef struct {
	uint8_t reserved:1;
    uint16_t ichgr:15;
} ichgr_reg_t __attribute__(());

typedef struct {
	uint8_t reserved1:1;
    uint16_t vbusv:13;
    uint8_t reserved:2;
} vbusv_reg_t __attribute__(());

typedef struct {
	uint8_t reserved1:3;
    uint16_t batv:12;
    uint8_t reserved:1;
} batv_reg_t __attribute__(());


class PMIC_BQ25622 {

    // Arduino's I2C library
    TwoWire *_i2c;

    // I2C address
    bq25622_addr_t _i2c_addr;

    // Reads 16 bytes from a register.
    void _read(bq25622_reg_t reg, uint8_t *val);

    // Writes 16 bytes to a register.
    void _write(bq25622_reg_t reg, uint8_t *val);

public:

    PMIC_BQ25622(bq25622_addr_t addr = BQ25622_ADDR) : _i2c_addr(addr) {};
    // Initializes BQ25622
    void begin(TwoWire *theWire = &Wire);

    // Check if IC is communicating
    bool isConnected();

    // Resets BQ25622
    void reset();
	
	// REG02
	ichg_reg_t getICHG_reg();
	bq25622_error_t setICHG(int value);
	uint16_t getICHG();
	
	// REG04
	vreg_reg_t getVREG_reg();
	bq25622_error_t setVREG(int value);
	uint16_t getVREG();

	// REG08
	vindpm_reg_t getVINDPM_reg();
	bq25622_error_t setVINDPM(int value);
	uint16_t getVINDPM();

	// REG10
	ipre_reg_t getIPRE_reg();
	bq25622_error_t setIPRECHG(int value);
	uint16_t getIPRECHG();

	// REG12
	iterm_reg_t getITERM_reg();
	bq25622_error_t setITERM(int value);
	uint16_t getITERM();

	// REG14
	ctrl0_reg_t getCTRL0_reg();
	void setQ1_FULLON(bool value);
	void setQ4_FULLON(bool value);

	// REG17
	ctrl2_reg_t getCTRL2_reg();
	void setREG_RST(bool value);

	// REG19
	ctrl4_reg_t getCTRL4_reg();
	void setEN_EXTLIM(bool value);

	// REG 1A
	ntc_reg_t getNTC_reg();
	void setTS_IGNORE(bool value);

	// REG26
	adc_ctrl_reg_t getADC_CTRL_reg();
	void setCONV_START(bool value);
	void setCONV_RATE(bool value);
	bq25622_error_t setADC_SAMPLE(int value);

	// REG27
	adc_dis_reg_t getADC_DIS_reg();
	void setVPMID_DIS(bool value);
	void setTDIE_DIS(bool value);
	void setTS_DIS(bool value);
	void setVSYS_DIS(bool value);
	void setVBAT_DIS(bool value);
	void setVBUS_DIS(bool value);
	void setIBAT_DIS(bool value);
	void setIBUS_DIS(bool value);

	// REG2A
	ichgr_reg_t getICHGR_reg();
	uint16_t getICHGR();

	// REG2C
	vbusv_reg_t getVBUSV_reg();
	uint16_t getVBUSV();

	// REG30
	batv_reg_t getBATV_reg();
	uint16_t getBATV();


};

#endif