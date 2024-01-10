#include "SPI_LCD.hpp"



// void SPI_LCD_Class::LCD_Thread_main(void)
// {
//     CS=1;
//     init();


//     while(1)
//     {
//         LCD_Queue.dispatch_forever();
//     }   
// }

void SPI_LCD_Class::Send_Data(std::string data)
{
    LCD_Data *LCD_Mail = SPI_LCD_mail.alloc();   // telling what mailbox to send data to
    LCD_Mail->data = data;       // set temperature 
    LCD_Mail->cmd = 0;         // set pressure 
    SPI_LCD_mail.put(LCD_Mail);                // send samples to terminal mailbox
    LCD_Queue->call(callback(this, &SPI_LCD_Class::print));
}

void SPI_LCD_Class::Send_CMD(int cmd)
{
    LCD_Data *LCD_Mail = SPI_LCD_mail.alloc();   // telling what mailbox to send data to
    LCD_Mail->data = "";       // set temperature 
    LCD_Mail->cmd = cmd;         // set pressure 
    SPI_LCD_mail.put(LCD_Mail);                // send samples to terminal mailbox
    LCD_Queue->call(callback(this, &SPI_LCD_Class::write_cmd));
}




void SPI_LCD_Class::print(void)
{
    //open mailbox and print to line to LCD
    osEvent evt = SPI_LCD_mail.get();
    if (evt.status == osEventMail) {
        LCD_Data *mail = (LCD_Data*)evt.value.p;

        const char *string = mail->data.c_str();

        while(*string)
        {
            write_data(*string++);
            wait_us(40);
        }

        SPI_LCD_mail.free(mail);    // Free up the mailbox
    }


}

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







void SPI_LCD_Class::init(void)
{
    thread_sleep_for(40);
    shift_out(0x30);
    wait_us(37);
    write_cmd_raw(0x20);
    wait_us(37);
    write_cmd_raw(0x20);
    wait_us(37);
    write_cmd_raw(0x0C);
    wait_us(37);
    write_cmd_raw(0x01);
    wait_us(1520);
    write_cmd_raw(0x06);
    wait_us(37);
    write_cmd_raw(0x28);
    wait_us(37);

    clr();
}


void SPI_LCD_Class::clr(void)
{
    write_cmd_raw(0x01);
    wait_us(1520);
}


void SPI_LCD_Class::shift_out(int data)
{
    CS = 0;
    ser_port.write(data);
    CS = 1;
}


void SPI_LCD_Class::write_cmd_raw(int cmd)
{
    write_4bit(cmd, COMMAND_MODE);
}


void SPI_LCD_Class::write_data(char c)
{
    write_4bit(c, DATA_MODE);
}


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