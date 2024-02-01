/*------------------------------------------------------------------------------
Creator : Jonathan Wheadon
Date : 30/11/2023
Dependencies : 
Purpose : This cpp defines the functions for the Serial_Comms class which are used
to control the Serial_Comms with the PC or Raspberry Pi
------------------------------------------------------------------------------*/

#include "Serial_Comms.hpp"


// queue for serial comms events, need to find a way to move this within serial comm class
EventQueue Serial_Comms_Queue(32 * EVENTS_EVENT_SIZE);

/*------------------------------------------------------------------------------
 initialize Serial_Comms, print table, initialize variables and attach interupts
------------------------------------------------------------------------------*/
void Serial_Comms::init(void)
{
    // Set baud rate for serial object pc
    pc.baud (Baudrate);  
     
    // initialise variables    
    buffer_pointer_In = 0;

    // Attach interupts
    pc.attach(callback(this, &Serial_Comms::Input_Handler), pc.RxIrq);
}


/*------------------------------------------------------------------------------
 Function decodes recieved commands stored in input buffer and executes it
------------------------------------------------------------------------------*/
void Serial_Comms::Decode_Command(void)
{
    // If the message is the correct length for the chosen comm protocol 
    if(buffer_pointer_In == Com_Protocol_Length)
    {
        // Convert the recieved 8-bit chars (Serial_Comms_buffer_In) into one 32-bit Int (Val)
        unsigned int Val = 0;

        for(int idx = 0; idx < Com_Protocol_Length; idx++)
        {
            Val = Val + ((0x0000007F & Serial_Comms_buffer_In[idx]) << (7 * idx)); // bit shift LSB to MSB
        }

        // Call the stored function that executes the recieved command
        Recieved_Command_Handler_(Val);
    } 
    else 
    {   // The recived data does not match the set comms protocol
        std::string output;
        output = "Error : ";

        // The recieved message does not match current com protocol length
                // Fill debug string
        char buffer[100];
        int len = sprintf(buffer, "length is %d \n", buffer_pointer_In);
        std::string S;
        for (int i = 0; i < len; i++) {
            S = S + buffer[i];
        }
        output = output + S;
        Print(output);
    }
    
    //Print debug string
    // LCD.Send_Data(S);
    buffer_pointer_In = 0;
    // Print(S);
    pc.attach(callback(this, &Serial_Comms::Input_Handler), pc.RxIrq);
}


/*------------------------------------------------------------------------------
 ISR for whenever data is in Serial_Comms buffer, add function to Serial_Comms queue
 to check which charecteter has been entered and add it to a buffer.
------------------------------------------------------------------------------*/
void Serial_Comms::Input_Handler(void)
{
    pc.attach(NULL,pc.RxIrq);
    if(pc.readable()!= 0)
    {
        char gotkey; 
        pc.read(&gotkey, 1);

        // If recieved key is invalid, ignore
        if(gotkey == NULL) 
        {
            // do nothing, re attach interupt
            pc.attach(callback(this, &Serial_Comms::Input_Handler), pc.RxIrq);

        }  // If recieved key is carridge return the execute the stored command
        else if (gotkey == 0x0D) 
        {
            Serial_Comms_buffer_In[buffer_pointer_In] = 0x00;
            Serial_Comms_Queue.call(this, &Serial_Comms::Decode_Command);

        } 
        else // Store recieved key in serial comm buffer
        {
            Serial_Comms_buffer_In[buffer_pointer_In] = gotkey;
            buffer_pointer_In++;

            pc.attach(callback(this, &Serial_Comms::Input_Handler), pc.RxIrq);
        }
    } else {
        pc.attach(callback(this, &Serial_Comms::Input_Handler), pc.RxIrq); 
        // Do error thing
    }
}


/*------------------------------------------------------------------------------
 Function transmits a string down the serial line
------------------------------------------------------------------------------*/
void Serial_Comms::Print(std::string string)
{
    char local_buffer_pointer;            // stores current point in Serial_Comms input line
    char local_Serial_Comms_buffer[Com_Buffer_Length];

    int idx = 0;
    for(idx = 0; string[idx] != 0; idx++){
        local_Serial_Comms_buffer[idx] = string[idx];
    }
    local_buffer_pointer = idx;


    Serial_Comms_Data *Serial_Comms_mail = Serial_Comms_mailbox.alloc();   // telling what mailbox to send data to

    Serial_Comms_mail->buffer_pointer = local_buffer_pointer;       
    memcpy(Serial_Comms_mail->Serial_Comms_buffer, local_Serial_Comms_buffer, sizeof(local_Serial_Comms_buffer));       

    Serial_Comms_mailbox.put(Serial_Comms_mail);                  // send samples to terminal mailbox
    Serial_Comms_Queue.call(this, &Serial_Comms::Send);
    return;
}

/*------------------------------------------------------------------------------
 Function is queued to send the data in the serial comms mailbox
------------------------------------------------------------------------------*/
void Serial_Comms::Send()
{
    //open mailbox and print to line to LCD
    osEvent evt = Serial_Comms_mailbox.get();
    if (evt.status == osEventMail) {
        Serial_Comms_Data *mail = (Serial_Comms_Data*)evt.value.p;

        pc.write(mail->Serial_Comms_buffer, mail->buffer_pointer);

        Serial_Comms_mailbox.free(mail);    // Free up the mailbox

    } else {
        // do error thing
    }
}



/*------------------------------------------------------------------------------
 Thread for serial communications, handles evenmts as they
 are added to the serial comms event queue
------------------------------------------------------------------------------*/
void Serial_Comms::Serial_Comms_Thread_main(void)
{
    init();
    
    // Enter Forever loop dispatching serial comms events as they are called by other threads
    while(1)
    {
        Serial_Comms_Queue.dispatch_forever();
    }
}







/*------------------------------------------------------------------------------
 Function converts string to FLOAT_32, one decimal place expected must be 4 bytes
 long, PAd with '0', example 1.2f would be "01.2" and 12.3f would be "12.3"
------------------------------------------------------------------------------*/
float strTOflt(char ary[4])
{
    //check that values entered are in format "00.0"
    for(int tester = 0; tester < 4; tester++)
    {
        if(tester != 2){
            int testing = (int)(ary[tester]-'0');
            if((testing > 9) || (testing < 0)) {
                return NULL;
            }
        } else if(ary[tester] != '.') {
            return NULL;
        }
    }
    
    float retFlt;
    
    retFlt = ((float)(ary[0]-'0'))*10.0f;
    retFlt += ((float)(ary[1]-'0'));
    retFlt += ((float)(ary[3]-'0'))/10.0f;
    
    if(retFlt > 60.0f){
        return NULL;
    } else {
        return retFlt;
    }
}

int strTOint(char ary[3])
{   
    //check that values entered are in format "000"
    for(int tester = 0; tester < 3; tester++)
    {
        int testing = (int)(ary[tester]-'0');
        if((testing > 9) || (testing < 0)) {
            return NULL;
        }
    }
    
    int temp_int = (int)(ary[0]-'0')*100;
    temp_int += (int)(ary[1]-'0')*10;
    temp_int += (int)(ary[2]-'0');

    return temp_int;
}

std::string Char_Buffer_To_STD_String(char* array, int len)
{
    std::string ret_str;

    for (int i = 0; i < len; i++) {
        ret_str = ret_str + array[i];
    }

    return ret_str;
}

