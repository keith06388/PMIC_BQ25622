#include "PMIC_BQ25622.h"

void PMIC_BQ25622::_read(bq25622_reg_t reg, uint8_t *val) {
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->endTransmission();

    _i2c->requestFrom(_i2c_addr, 1);

    if (_i2c->available())
    {
      *val = _i2c->read();
    }
}
void PMIC_BQ25622::_read2(bq25622_reg_t reg, uint16_t *val) {
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->endTransmission();

    _i2c->requestFrom(_i2c_addr,2);

    if (_i2c->available() >= 2) {
        uint8_t lsb = _i2c->read();
        uint8_t msb = _i2c->read();
        *val = (msb << 8) | lsb; // Combine bytes, assuming little-endian order
    }
}

void PMIC_BQ25622::_write(bq25622_reg_t reg, uint8_t *val) {
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->write(*val);
    _i2c->endTransmission();
}

void PMIC_BQ25622::_write2(bq25622_reg_t reg, uint16_t *val) {
	uint16_t value = *val;
	_i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->write(value & 0xFF);
	_i2c->write((value >> 8) & 0xFF);
    _i2c->endTransmission();
}

void PMIC_BQ25622::begin(TwoWire *theWire){
    _i2c = theWire;
    _i2c->begin();
}

bool PMIC_BQ25622::isConnected(){
    _i2c->beginTransmission(_i2c_addr);
    uint8_t error = _i2c->endTransmission();
    if(error == 0) return true;
    else return false;
}

void PMIC_BQ25622::reset(){
    PMIC_BQ25622::setREG_RST(1);
}

// REG02
ichg_reg_t PMIC_BQ25622::getICHG_reg(){
    ichg_reg_t temp_reg;
	_read2(Charge_Current_Limit_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setICHG(int value){
    ichg_reg_t temp_reg;
    if(value < 80 || value > 3040){
        return BQ_RANGE_ERR;
    }
    uint8_t data = value / 80;
	_read2(Charge_Current_Limit_LSB, (uint16_t*)&temp_reg);
    temp_reg.ichg = data;
    _write2(Charge_Current_Limit_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getICHG(){
    ichg_reg_t temp_reg = PMIC_BQ25622::getICHG_reg();
    uint16_t data = temp_reg.ichg * 80;
    return data;
}

// REG04
vreg_reg_t PMIC_BQ25622::getVREG_reg(){
    vreg_reg_t temp_reg;
	_read2(Charge_Voltage_Limit_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setVREG(int value){
    vreg_reg_t temp_reg;
    if(value < 3500 || value > 4800){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value / 10;
	_read2(Charge_Voltage_Limit_LSB, (uint16_t*)&temp_reg);
    temp_reg.vreg = data;
    _write2(Charge_Voltage_Limit_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getVREG(){
    vreg_reg_t temp_reg = PMIC_BQ25622::getVREG_reg();
    uint16_t data = temp_reg.vreg * 10;
    return data;
}

// REG06
iindpm_reg_t PMIC_BQ25622::getIINDPM_reg(){
    iindpm_reg_t temp_reg;
	_read2(Input_Current_Limit_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setIINDPM(int value){
    iindpm_reg_t temp_reg;
    if(value < 100 || value > 3200){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value / 20;
	_read2(Input_Current_Limit_LSB, (uint16_t*)&temp_reg);
    temp_reg.iindpm = data;
    _write2(Input_Current_Limit_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getIINDPM(){
    iindpm_reg_t temp_reg = PMIC_BQ25622::getIINDPM_reg();
    uint16_t data = (temp_reg.iindpm * 20);
    return data;
}

// REG08
vindpm_reg_t PMIC_BQ25622::getVINDPM_reg(){
    vindpm_reg_t temp_reg;
	_read2(Input_Voltage_Limit_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setVINDPM(int value){
    vindpm_reg_t temp_reg;
    if(value < 3800 || value > 16800){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value / 40;
	_read2(Input_Voltage_Limit_LSB, (uint16_t*)&temp_reg);
    temp_reg.vindpm = data;
    _write2(Input_Voltage_Limit_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getVINDPM(){
    vindpm_reg_t temp_reg = PMIC_BQ25622::getVINDPM_reg();
    uint16_t data = (temp_reg.vindpm * 40);
    return data;
}

// REG10
ipre_reg_t PMIC_BQ25622::getIPRE_reg(){
    ipre_reg_t temp_reg;
	_read2(Precharge_Control_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setIPRECHG(int value){
    ipre_reg_t temp_reg;
    if(value < 20 || value > 620){
        return BQ_RANGE_ERR;
    }
    uint8_t data = value / 20;
	_read2(Precharge_Control_LSB, (uint16_t*)&temp_reg);
    temp_reg.iprechg = data;
    _write2(Precharge_Control_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getIPRECHG(){
    ipre_reg_t temp_reg = PMIC_BQ25622::getIPRE_reg();
    uint16_t data = (temp_reg.iprechg * 20);
    return data;
}

// REG12
iterm_reg_t PMIC_BQ25622::getITERM_reg(){
    iterm_reg_t temp_reg;
	_read2(Termination_Control_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
bq25622_error_t PMIC_BQ25622::setITERM(int value){
    iterm_reg_t temp_reg;
    if(value < 10 || value > 620){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value / 10;
	_read2(Termination_Control_LSB, (uint16_t*)&temp_reg);
    temp_reg.iterm = data;
    _write2(Termination_Control_LSB, (uint16_t*)&temp_reg); // Write the low byte
    return BQ_OK;
}
uint16_t PMIC_BQ25622::getITERM(){
    iterm_reg_t temp_reg = PMIC_BQ25622::getITERM_reg();
    uint16_t data = (temp_reg.iterm * 10);
    return data;
}

// REG14
ctrl0_reg_t PMIC_BQ25622::getCTRL0_reg(){
    ctrl0_reg_t temp_reg;
    _read(Charge_Control_0, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setQ1_FULLON(bool value){
    ctrl0_reg_t temp_reg;
    _read(Charge_Control_0, (uint8_t*)&temp_reg);
    temp_reg.q1_fullon = value;
    _write(Charge_Control_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setQ4_FULLON(bool value){
    ctrl0_reg_t temp_reg;
    _read(Charge_Control_0, (uint8_t*)&temp_reg);
    temp_reg.q4_fullon = value;
    _write(Charge_Control_0, (uint8_t*)&temp_reg);
}

// REG16
ctrl1_reg_t PMIC_BQ25622::getCTRL1_reg(){
    ctrl1_reg_t temp_reg;
    _read(Charger_Control_1, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setEN_CHG(bool value){
    ctrl1_reg_t temp_reg;
    _read(Charger_Control_1, (uint8_t*)&temp_reg);
    temp_reg.en_chg = value;
    _write(Charger_Control_1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setWATCHDOG(bool value){
    ctrl1_reg_t temp_reg;
    _read(Charger_Control_1, (uint8_t*)&temp_reg);
    temp_reg.watchdog = value;
    _write(Charger_Control_1, (uint8_t*)&temp_reg);
}

// REG17
ctrl2_reg_t PMIC_BQ25622::getCTRL2_reg(){
    ctrl2_reg_t temp_reg;
    _read(Charger_Control_2, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setCONV_STRN(int value){
    ctrl2_reg_t temp_reg;
    _read(Charger_Control_2, (uint8_t*)&temp_reg);
    temp_reg.set_conv_strn = value;
    _write(Charger_Control_2, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setCONV_FREQ(int value){
    ctrl2_reg_t temp_reg;
    _read(Charger_Control_2, (uint8_t*)&temp_reg);
    temp_reg.set_conv_freq = value;
    _write(Charger_Control_2, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setREG_RST(bool value){
    ctrl2_reg_t temp_reg;
    _read(Charger_Control_2, (uint8_t*)&temp_reg);
    temp_reg.reg_rst = value;
    _write(Charger_Control_2, (uint8_t*)&temp_reg);
}

// REG19
ctrl4_reg_t PMIC_BQ25622::getCTRL4_reg(){
    ctrl4_reg_t temp_reg;
    _read(Charger_Control_4, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setEN_EXTLIM(bool value){
    ctrl4_reg_t temp_reg;
    _read(Charger_Control_4, (uint8_t*)&temp_reg);
    temp_reg.en_extlim = value;
    _write(Charger_Control_4, (uint8_t*)&temp_reg);
}

// REG 1A
ntc_reg_t PMIC_BQ25622::getNTC_reg(){
    ntc_reg_t temp_reg;
    _read(NTC_Control_0, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setTS_IGNORE(bool value){
    ntc_reg_t temp_reg;
    _read(NTC_Control_0, (uint8_t*)&temp_reg);
    temp_reg.ts_ignore = value;
    _write(NTC_Control_0, (uint8_t*)&temp_reg);
}

// REG26
adc_ctrl_reg_t PMIC_BQ25622::getADC_CTRL_reg(){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_Control, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setCONV_START(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_Control, (uint8_t*)&temp_reg);
    temp_reg.adc_en = value;
    _write(ADC_Control, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setCONV_RATE(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_Control, (uint8_t*)&temp_reg);
    temp_reg.adc_rate = value;
    _write(ADC_Control, (uint8_t*)&temp_reg);
}
bq25622_error_t PMIC_BQ25622::setADC_SAMPLE(int value){
    adc_ctrl_reg_t temp_reg;
    if(value < 0 || value > 3){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value;
    _read(ADC_Control, (uint8_t*)&temp_reg);
    temp_reg.adc_sample = data;
    _write(ADC_Control, (uint8_t*)&temp_reg);
    return BQ_OK; 
}

// REG27
adc_dis_reg_t PMIC_BQ25622::getADC_DIS_reg(){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25622::setVPMID_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.vpmid_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setTDIE_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.tdie_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setTS_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.ts_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setVSYS_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.vsys_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setVBAT_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.vbat_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setVBUS_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.vbus_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setIBAT_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.ibat_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}
void PMIC_BQ25622::setIBUS_DIS(bool value){
    adc_dis_reg_t temp_reg;
    _read(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
    temp_reg.ibus_adc_dis = value;
    _write(ADC_Function_Disable_0, (uint8_t*)&temp_reg);
}

// REG2A
ichgr_reg_t PMIC_BQ25622::getICHGR_reg(){
    ichgr_reg_t temp_reg;
	_read2(IBAT_ADC_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25622::getICHGR(){
    ichgr_reg_t temp_reg = PMIC_BQ25622::getICHGR_reg();
    uint16_t data = temp_reg.ichgr * 4.0;
    return data;
}

// REG2C
vbusv_reg_t PMIC_BQ25622::getVBUSV_reg(){
    vbusv_reg_t temp_reg;
	_read2(VBUS_ADC_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25622::getVBUSV(){
    vbusv_reg_t temp_reg = PMIC_BQ25622::getVBUSV_reg();
    uint16_t data = (temp_reg.vbusv * 3.97);
    return data;
}

// REG30
batv_reg_t PMIC_BQ25622::getBATV_reg(){
    batv_reg_t temp_reg;
	_read2(VBAT_ADC_LSB, (uint16_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25622::getBATV(){
    batv_reg_t temp_reg = PMIC_BQ25622::getBATV_reg();
    uint16_t data = (temp_reg.batv * 1.99);
    return data;
}