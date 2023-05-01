#ifndef mcp2515_can_driver_hpp
#define mcp2515_can_driver_hpp

#include "../abstract_can_driver.hpp"
#include "neotje/mcp2515.hpp"

class mcp2515_can_driver : public abstract_can_driver, public mcp2515 {
    private:
        void setup();
        
    public:
        mcp2515_can_driver(mcp2515_config_t *config);
        mcp2515_can_driver(spi_inst_t* port, uint miso, uint mosi, uint sck, uint cs, uint baudrate);
        
        // Inherited from abstract_can_driver
        int send_message(can_frame_t *message);
        int receive_message(can_frame_t *message);
};

#endif