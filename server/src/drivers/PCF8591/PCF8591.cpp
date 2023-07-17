#include <iostream>
#include "drivers/PCF8591/PCF8591.h"
#include <cstring>
#include "../../../../PCF8591/pcf_ioctl.h"


using rpi::PiPCF8591::PCF8591; 

PCF8591::PCF8591()
{
    open();
}

PCF8591::~PCF8591()
{
    std::cout << "close device PCF8591" << std::endl;
    if(fp_ != NULL)
    {
        fclose(fp_);
        fp_ = NULL;
    }
}

uint32_t PCF8591::readADC(uint8_t channel)
{
    if(fp_ == NULL)
        return 0.0;

    if (ioctl(fileno(fp_), PCF_IOC_SET_CHANNEL, &channel))
    {
        std::cout << "PCF_IOC_SET_CHANNEL error" << std::endl;
    }
    fflush(fp_);

    uint8_t max_retries = 10;
    bool is_value_correct = false;
    uint8_t buf[4];

    uint32_t value1 = 0;
    uint32_t value2 = 0;
    while(!is_value_correct && (--max_retries > 0) )
    {
        memset(buf, 0, sizeof(buf));
        size_t bytes_read = fread(buf, 1, sizeof(buf), fp_);
        std::cout << "read " << bytes_read << " bytes. data " << buf << std::endl;
        fflush(fp_);
        if(bytes_read > 0)
        {
            value1 = getValue(buf);
            std::cout << "value1 " << (int)value1 << std::endl;
        }

        memset(buf, 0, sizeof(buf));
        bytes_read = fread(buf, 1, sizeof(buf), fp_);
        std::cout << "read " << bytes_read << " bytes. data " << buf << std::endl;
        fflush(fp_);
        if(bytes_read > 0)
        {
            value2 = getValue(buf);
            std::cout << "value2 " << (int)value2 << std::endl;
            is_value_correct = (value1 == value2);
        }
    }
    return value1;
}

uint32_t PCF8591::getValue(uint8_t buf[])
{
    std::cout << (int)buf[0] << ":" << (int)buf[1] << ":" << (int)buf[2] << ":" << (int)buf[3] << std::endl;
    uint32_t result = static_cast<uint32_t>(buf[0] & 0xFF);
	result |= static_cast<uint32_t>((buf[1] & 0xFF) << 8);
	result |= static_cast<uint32_t>((buf[2] & 0xFF) << 16);
	result |= static_cast<uint32_t>((buf[3] & 0xFF) << 24);
    return result;
}

void PCF8591::open()
{
    fp_ = fopen(device_.c_str(), "a+");
    std::cout << "open device " << device_ << std::endl;
    if (fp_ == NULL)
    {
        std::cout << "open device error";
        //throw std::system_error(errno, std::system_category(), "Could not open pca i2c communication");
    }
}