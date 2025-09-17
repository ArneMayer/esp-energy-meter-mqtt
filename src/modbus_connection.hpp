#pragma once

#include <SoftwareSerial.h>
#include <stdint.h>
#include <ModbusRTU.h>
#include "register_types.hpp"

bool cb_write(Modbus::ResultCode result_code, uint16_t transactionId, void* data) {
    debug_print("Request result: ");
    debug_print(modbus_result_to_string(result_code));
    debug_print(", Mem: ");
    debug_println(ESP.getFreeHeap());

    return true;
}

class ModbusConnection {
  public:
    ModbusRTU mb;

    ModbusConnection(int8_t RX, int8_t TX, int16_t RE_DE, uint16_t baud) : serial(SoftwareSerial(RX, TX)) {
        serial.begin(baud, SWSERIAL_8N1);
        mb.begin(&serial, RE_DE);
        mb.master();
    }

    void wait_for_result() {
        while(mb.slave()) {
            mb.task();
            yield();
        }
    }

    void read_and_get(RegisterType reg_type, uint16_t modbus_id, uint16_t address, uint16_t num_regs, uint16_t* data) {
        if (reg_type == RegisterType::Ireg) {
            mb.readIreg(modbus_id, address, data, num_regs, cb_write);
        } else if (reg_type == RegisterType::Hreg) {
            mb.readHreg(modbus_id, address, data, num_regs, cb_write);
        }
        wait_for_result();
    }

    
    static const char* modbus_result_to_string(Modbus::ResultCode result_code) {
        switch(result_code) {
            case Modbus::ResultCode::EX_SUCCESS:
                return "EX_SUCCESS";
            case Modbus::ResultCode::EX_ILLEGAL_FUNCTION:
                return "EX_ILLEGAL_FUNCTION";
            case Modbus::ResultCode::EX_ILLEGAL_ADDRESS:
                return "EX_ILLEGAL_ADDRESS";  
            case Modbus::ResultCode::EX_ILLEGAL_VALUE:
                return "EX_ILLEGAL_VALUE";  
            case Modbus::ResultCode::EX_SLAVE_FAILURE:
                return "EX_SLAVE_FAILURE";  
            case Modbus::ResultCode::EX_ACKNOWLEDGE:
                return "EX_ACKNOWLEDGE";  
            case Modbus::ResultCode::EX_SLAVE_DEVICE_BUSY:
                return "EX_SLAVE_DEVICE_BUSY";  
            case Modbus::ResultCode::EX_MEMORY_PARITY_ERROR:
                return "EX_MEMORY_PARITY_ERROR";  
            case Modbus::ResultCode::EX_PATH_UNAVAILABLE:
                return "EX_PATH_UNAVAILABLE";  
            case Modbus::ResultCode::EX_DEVICE_FAILED_TO_RESPOND:
                return "EX_DEVICE_FAILED_TO_RESPOND";  
            case Modbus::ResultCode::EX_GENERAL_FAILURE:
                return "EX_GENERAL_FAILURE";  
            case Modbus::ResultCode::EX_DATA_MISMACH:
                return "EX_DATA_MISMACH";  
            case Modbus::ResultCode::EX_UNEXPECTED_RESPONSE:
                return "EX_UNEXPECTED_RESPONSE";  
            case Modbus::ResultCode::EX_TIMEOUT:
                return "EX_TIMEOUT";  
            case Modbus::ResultCode::EX_CONNECTION_LOST:
                return "EX_CONNECTION_LOST";  
            case Modbus::ResultCode::EX_CANCEL:
                return "EX_CANCEL"; 
            default:
                return "Unknown Error";
        }
    }

  private:
    SoftwareSerial serial;
};