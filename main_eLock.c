/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#define READ_ID(id)  (id >> 18)

static uint32_t status = 0;
static uint8_t rxFiFo0[CAN0_RX_FIFO0_SIZE];
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
static void print_message(uint8_t numberOfMessage, CAN_RX_BUFFER *rxBuf, uint8_t rxBufLen)
{
    uint8_t length = 0;
    uint8_t msgLength = 0;
    uint32_t id = 0;

    for (uint8_t count = 0; count < numberOfMessage; count++)
    {
        /* Print message to Console */
        printf(" Rx FIFO0 : New Message Received\r\n");
        id = rxBuf->xtd ? rxBuf->id : READ_ID(rxBuf->id);
        msgLength = rxBuf->dlc;
        length = msgLength;
        printf(" Message - ID : 0x%x Length : 0x%x ", (unsigned int)id, (unsigned int)msgLength);
        printf("Message : ");
        while(length)
        {
            printf("0x%x ", rxBuf->data[msgLength - length--]);
        }
        printf("\r\n");
        rxBuf += rxBufLen;
    }
}

int main ( void )
{
    uint8_t        numberOfMessage = 0;
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    SYSTICK_TimerStart();
    
    printf(" ------------------------------ \r\n");
    printf("            CAN Demo            \r\n");
    printf(" ------------------------------ \r\n");
    
    while ( true )
    {
        LED_BLUE_Toggle();
        LED_YELLOW_Toggle();
        SYSTICK_DelayMs(500);
        
         if (CAN0_InterruptGet(CAN_INTERRUPT_RF0N_MASK))
        {    
            CAN0_InterruptClear(CAN_INTERRUPT_RF0N_MASK);

            /* Check CAN Status */
            status = CAN0_ErrorGet();

            if (((status & CAN_PSR_LEC_Msk) == CAN_ERROR_NONE) || ((status & CAN_PSR_LEC_Msk) == CAN_ERROR_LEC_NC))
            {
                numberOfMessage = CAN0_RxFifoFillLevelGet(CAN_RX_FIFO_0);
                if (numberOfMessage != 0)
                {
                    memset(rxFiFo0, 0x00, (numberOfMessage * CAN0_RX_FIFO0_ELEMENT_SIZE));
                    if (CAN0_MessageReceiveFifo(CAN_RX_FIFO_0, numberOfMessage, (CAN_RX_BUFFER *)rxFiFo0) == true)
                    {
                        print_message(numberOfMessage, (CAN_RX_BUFFER *)rxFiFo0, CAN0_RX_FIFO0_ELEMENT_SIZE);
                    }
                    else
                    {
                        printf(" Error in received message\r\n");
                    }
                }
            }
            else
            {
                printf(" Error in received message\r\n");
            }
        }
    
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        //SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

