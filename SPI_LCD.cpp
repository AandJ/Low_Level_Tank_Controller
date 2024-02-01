#include "SPI_LCD.hpp"


// Public function - Queues data to be printed on the LCD
void SPI_LCD_Class::Send_Data(std::string data)
{
    LCD_Data *LCD_Mail = SPI_LCD_mail.alloc();   
    LCD_Mail->data = data;        
    LCD_Mail->cmd = 0;        
    SPI_LCD_mail.put(LCD_Mail);                
    LCD_Queue->call(callback(this, &SPI_LCD_Class::print));
}


// Public function - Queues a command for the LCD
void SPI_LCD_Class::Send_CMD(int cmd)
{
    LCD_Data *LCD_Mail = SPI_LCD_mail.alloc();   
    LCD_Mail->data = "";       
    LCD_Mail->cmd = cmd;          
    SPI_LCD_mail.put(LCD_Mail);               
    LCD_Queue->call(callback(this, &SPI_LCD_Class::write_cmd));
}



// Private function, is queued on the provided eventqueue when another thread add data to the print queue
void SPI_LCD_Class::print(void)
{
    //open mailbox and print to line to LCD
    osEvent evt = SPI_LCD_mail.get();
    if (evt.status == osEventMail) {
        LCD_Data *mail = (LCD_Data*)evt.value.p;

        const char *string = mail->data.c_str();

        while(*string)
        {
            write_4bit(*string++, DATA_MODE);

            wait_us(40);
        }

        SPI_LCD_mail.free(mail);    // Free up the mailbox
    }


}

// Private function, is queued on the provided eventqueue when another thread gives a command for the LCD
void SPI_LCD_Class::write_cmd(void)
{
    //open mailbox and print to line to LCD
    osEvent evt = SPI_LCD_mail.get();
    if (evt.status == osEventMail) {
        LCD_Data *mail = (LCD_Data*)evt.value.p;

        int cmd = mail->cmd;

        write_4bit(cmd, COMMAND_MODE);

        SPI_LCD_mail.free(mail);    // Free up the mailbox
    }
}


// Initialise the LCD and clear the screen
void SPI_LCD_Class::init(void)
{
    thread_sleep_for(40);
    shift_out(0x30);
    wait_us(37);
    write_4bit(0x20, COMMAND_MODE);
    wait_us(37);
    write_4bit(0x20, COMMAND_MODE);
    wait_us(37);
    write_4bit(0x0C, COMMAND_MODE);
    wait_us(37);
    write_4bit(0x01, COMMAND_MODE);
    wait_us(1520);
    write_4bit(0x06, COMMAND_MODE);
    wait_us(37);
    write_4bit(0x28, COMMAND_MODE);
    wait_us(37);

    clr();
}

// Write command to LCD to clear the screen
void SPI_LCD_Class::clr(void)
{
    write_4bit(0x01, COMMAND_MODE);
    wait_us(1520);
}

// Enable the LCD and send data or command via SPI
void SPI_LCD_Class::shift_out(int data)
{
    CS = 0;
    spi_port.write(data);
    CS = 1;
}

//
void SPI_LCD_Class::write_4bit(int data, int mode)
{
    int hi_n;
    int lo_n;
    hi_n = (data & 0xF0);
    lo_n = ((data << 4) &0xF0);
    shift_out(hi_n | ENABLE | mode);
    wait_us(1);
    shift_out(hi_n & ~ENABLE);
    shift_out(lo_n | ENABLE | mode);
    wait_us(1);
    shift_out(lo_n & ~ENABLE);
}