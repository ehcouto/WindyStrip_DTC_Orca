/**
 *  @file
 *
 *  @brief      Reveal modules implementations.
 *
 *  @details    Reveal is the application protocol that runs over LLC (Wide or Win) under the SAP 4
 *
 *  @section    Applicable_Documents
 *                  List here all the applicable documents if needed. <tr>
 *
 *  $Header: $
 *
 *  @copyright  Copyright 2019-$Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "Compiler_defs.h"
#include "Reveal.h"
#include "Reveal_prv.h"
#include <string.h>

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

typedef enum
{
    API001_PUBLISH_ACK          = 1,
    API001_PUBLISH_HEARTBEAT    = 2,
    API001_SET_HEARTBEAT        = 3,
    API001_PUBLISH_MEM          = 4,
    API001_READ_MEM_24B         = 5,
    API001_READ_EE              = 6,
    API001_SEND_EVENTS          = 7,
    API001_PUBLISH_EE           = 8,
    API001_READ_MEM_32B         = 9,
    API001_PUBLISH_HEARTBEAT_PERIOD  = 16,
    API001_MAX_MESSAGE_SIZE     = 17,
    API001_BLOCK_MESSAGES       = 18,
    API001_PUBLISH_WAKEUP       = 30,
    API001_PUBLISH_ACK_WAKEUP   = 31
}API001CORE_OPCODE_TYPE;

#define REVEAL_MSG_OVERHEAD     (sizeof(REVEAL_COMMAND_TYPE) - REVEAL_PAYLOAD_SIZE)

static REVEAL_MSG_TYPE Reveal_Adapter_Buffer;

#ifndef REVEAL_MAX_PAYLOAD_CALLBACK
    #define REVEAL_MAX_PAYLOAD_CALLBACK(node,max_size)
#endif


#ifndef REVEAL_SYNCH_PROCESS
    #define REVEAL_SYNCH_PROCESS DISABLED
#endif

#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_DISABLED)
    static uint8 Reveal_Queue_Reentry_Counter;
#endif


#if (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_EXTERNAL)
    static const REVEAL_ROUTING_MODE_TYPE REVEAL_ROUNTING_TABLE[15] = {ROUTING_TABLE_LIST};
    #if (REVEAL_DYNAMIC_ROUTER_FEATURE == ENABLED)
        static REVEAL_ROUTING_MODE_TYPE Reveal_Routing[15];
    #else
        #define Reveal_Routing REVEAL_ROUNTING_TABLE
    #endif
#endif

#ifndef REVEALQUEUE_QUEUE_OVERFLOW_NORMAL_EXCEPTION
    #define REVEALQUEUE_QUEUE_OVERFLOW_NORMAL_EXCEPTION(api,op)
#endif

#ifndef REVEAL_READ_EEPROM_FEATURE
    #define REVEAL_READ_EEPROM_FEATURE DISABLED
#endif

#if (REVEAL_READ_EEPROM_FEATURE == ENABLED)
    #ifndef REVEAL_EEPROM_READ
        #define REVEAL_EEPROM_READ(source,address,size)
    #endif
#endif

#ifndef REVEAL__ACK_INCOMING_MESSAGE
    #define REVEAL__ACK_INCOMING_MESSAGE(node,api,op_code,reason_code)
#endif

#ifndef REVEAL__SENDEVENTS
    #define REVEAL__SENDEVENTS(api,op)
#endif

#ifndef REVEAL_QUEUE_DEBUG
    #define REVEAL_QUEUE_DEBUG  DISABLED
#endif

typedef struct
{
    REVEAL_RECIPE_STATUS_TYPE   reason_code;
    uint8 api;
    uint8 op_code;
    uint8 node;
}REVEAL_ACK_TYPE;

static REVEAL_COMMAND_TYPE Reveal_Single_Queue;
static REVEAL_COMMAND_TYPE Reveal_Queue[REVEAL_QUEUE_SIZE];
static uint8 Reveal_Queue_Write;
static uint8 Reveal_Queue_Read;
#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    static uint8 Reveal_Queue_Retry;
    static uint16 Reveal_Queue_Timeout;
#endif

#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    static REVEAL_COMMAND_TYPE Reveal_Queue_Priority[REVEAL_QUEUE_PRIORITY_SIZE];
    static uint8 Reveal_Queue_Priority_Write;
    static uint8 Reveal_Queue_Priority_Read;
    static uint8 Reveal_Queue_Priority_Retry;
    static uint16 Reveal_Queue_Priority_Timeout;
    #if (REVEAL_QUEUE_DEBUG == ENABLED)
        static uint8 Reveal_Priority_Min_Space;
    #endif
#endif

typedef enum
{
    REVEAL_OUTGOING_STATE_IDLE,
    REVEAL_OUTGOING_STATE_SENDING,
    REVEAL_OUTGOING_STATE_TIMING
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    ,
    REVEAL_OUTGOING_STATE_SENDING_PRIORITY,
    REVEAL_OUTGOING_STATE_TIMING_PRIORITY
#endif
} REVEAL_OUTGOING_STATE_TYPE;

#ifndef REVEAL_ACK_FIFO
    #define REVEAL_ACK_FIFO 4
#endif

#ifndef REVEAL_FULL_SERACH_INTERNAL
    #define REVEAL_FULL_SERACH_INTERNAL    TRUE
#endif

#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    static REVEAL_ACK_TYPE Reveal_Ack_Buffer[REVEAL_ACK_FIFO];
    static uint8 Reveal_Ack_Write;
    static uint8 Reveal_Ack_Read;
    static REVEAL_OUTGOING_STATE_TYPE Reveal_Outgoing_State;
#endif

#define REVEAL_NUM_PARSERS      (sizeof(REVEAL_PARSER)/sizeof(REVEAL_PARSER_TYPE))
static uint16 Reveal_Heartbeat_Timer;
static uint16 Reveal_Heartbeat_Counter;
static uint16 Reveal_1s;
static uint8 Reveal_Aux;
#define REVEAL_1S_TICKS     1000
#define REVEAL_API_OP_SIZE  2
#if (REVEAL_QUEUE_DEBUG == ENABLED)
    static uint8 Reveal_Min_Space;
#endif


static uint16 Reveal_Node_Status;
static BOOL_TYPE Reveal_Drop_flag;

// Macro defines depth of recursive Reveal__Queue (if REVEAL_ROUTER_INTERNAL_ONLY is defined.)
#ifndef    REVEAL_QUEUE_MAX_REENTRIES
    #define REVEAL_QUEUE_MAX_REENTRIES          3
#endif

#ifndef   REVEAL_EXCEPTION_REENTRY_CNT_OVERLOADED
    #define REVEAL_EXCEPTION_REENTRY_CNT_OVERLOADED(api_id,op_code)
#endif

static uint8 Reveal_Heartbeat_Node_Counter[15];

//! Track the amount of time left to block messages (5ms increments)
static uint16 Reveal_Blocked_Messages_Remaining_Time;

//! Convert the block messages time in seconds to ticks managed by the module
//! Value is based on 5ms call rate where the blocked messages remaining time is updated
#define REVEAL_BLOCKED_MESSAGES_TIME_TO_TICKS   200 // 200 = 1s / 5ms

//! Determines if any messages are currently being blocked
#define MESSAGE_BLOCKING_ACTIVE() (Reveal_Blocked_Messages_Remaining_Time != 0)

//! Define the byte offset (0-based) in the Block Messages payload where the API/Opcode pairs start
#define BLOCKED_API_OPCODE_BYTE_OFFSET_START 3

//! Define the number of bytes in the Block Messages payload that contain the information about a single api and opcode
#define BLOCKED_API_OPCODE_NUM_BYTES         5

//! Variable to keep track of which APIs/Opcodes are blocked
//! Each entry in the array corresponds to an API in the POSSIBLE_BLOCKED_APIS_LIST table
//! Each bit within a single entry represents whether the opcode for the API is blocked (1) or not (0)
static uint32 Reveal_Blocked_Messages[ELEMENTS_IN_ARRAY(POSSIBLE_BLOCKED_APIS_LIST)];

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------
static BOOL_TYPE IsMessageBlocked(uint8 api, uint8 opcode);
static void RevealHandlerIncomming(void);
static void RevealHandlerOutgoing(void);

#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    static void DropMessageFromQueue(uint8 priority);
#endif
static BOOL_TYPE RequestedAPIsCanBeBlocked(uint8 * pt, uint8 number_of_bytes);
static void Reveal1sTasks(void);
static void RevealHandleAckParser(uint8 node, uint8 api, uint8 op_code, REVEAL_RECIPE_STATUS_TYPE reason, uint8 full_search);
static REVEAL_RECIPE_STATUS_TYPE RevealHandleParser(REVEAL_MSG_TYPE * pt,uint8 feedback, uint8 address_checking, uint8 full_search);
static void UpdateBlockedMessagesTimer(void);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module Reveal and its variables
 *
 */
void Reveal__Initialize(void)
{
    REVEAL_COMMAND_TYPE * new_msg;

    Reveal_Queue_Write = 0;
    Reveal_Queue_Read = 0;
    Reveal_Heartbeat_Timer = REVEAL_INITIAL_HEARTBEAT;
    Reveal_Heartbeat_Counter = 0;
    Reveal_1s = 0;

    // Initialize the timers
    memset(Reveal_Heartbeat_Node_Counter,(REVEAL_INITIAL_HEARTBEAT*3)+1,sizeof(Reveal_Heartbeat_Node_Counter));

    #if (REVEAL_QUEUE_DEBUG == ENABLED)
        Reveal_Min_Space = REVEAL_QUEUE_SIZE;
    #endif
    #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
        Reveal_Ack_Write = 0;
        Reveal_Ack_Read = 0;
    #endif
    #if (REVEAL_QUEUE_PRIORITY == ENABLED)
        Reveal_Queue_Priority_Write = 0;
        Reveal_Queue_Priority_Read = 0;
        Reveal_Queue_Priority_Retry = 0;
        Reveal_Queue_Priority_Timeout = 0;
        #if (REVEAL_QUEUE_DEBUG == ENABLED)
            Reveal_Priority_Min_Space = REVEAL_QUEUE_PRIORITY_SIZE;
        #endif
    #endif
    #if ((REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_EXTERNAL) && (REVEAL_DYNAMIC_ROUTER_FEATURE == ENABLED))
        memcpy((uint8 *)Reveal_Routing,(uint8 *)REVEAL_ROUNTING_TABLE,sizeof(Reveal_Routing));
    #endif
    #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_DISABLED)
        Reveal_Queue_Reentry_Counter = 0;
    #endif

    // Modified initialization to set broadcast and the node itself
    Reveal_Node_Status = 0x8000;
    BIT_SET(Reveal_Node_Status,LLC_OWN_ADDRESS);
    Reveal_Drop_flag = FALSE;

    new_msg = Reveal__NewFeedback(0x1,API001_PUBLISH_WAKEUP, REVEAL_BROADCAST_NODE,0);
    if (new_msg != (REVEAL_COMMAND_TYPE *)NULL)
    {
       Reveal__QueueCommand(new_msg);
    }

    // Clear blocked api/opcodes
    memset(Reveal_Blocked_Messages, 0, sizeof(Reveal_Blocked_Messages));

    // Clear blocked messages timer to prevent any messages from being blocked after a reset
    Reveal_Blocked_Messages_Remaining_Time = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This handler manages the incoming and outgoing message threads
 * @details This must be called at every 5ms from the MainCommunicationThread()
 */
void Reveal__Handler(void)
{
    RevealHandlerIncomming();
    RevealHandlerOutgoing();
    UpdateBlockedMessagesTimer();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This API manages the 1ms Heart-beat
 */
void Reveal__Handler1ms(void)
{
#if (REVEAL_1S_EXTERNAL_REFFERENCE == DISABLED)
    Reveal_1s++;
    if (Reveal_1s >= REVEAL_1S_TICKS)
    {
        Reveal_1s = 0;
        Reveal1sTasks();

        if (Reveal_Heartbeat_Timer > 0)
        {
            uint8 nd;
            for(nd=0;nd<sizeof(Reveal_Heartbeat_Node_Counter);nd++)
            {
                if (nd != LLC_OWN_ADDRESS)
                {
                    if (Reveal_Heartbeat_Node_Counter[nd] > 0)
                    {
                        Reveal_Heartbeat_Node_Counter[nd]--;
                        if (Reveal_Heartbeat_Node_Counter[nd] == 0)
                        {
                            // Fault detected
                            BIT_CLR(Reveal_Node_Status,nd);
                        }
                    }
                }
            }
        }
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
#if ((REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_EXTERNAL) && (REVEAL_DYNAMIC_ROUTER_FEATURE == ENABLED))
/**
 * @brief This method is used for system that require dynamic configuration for the router
 * @param node
 * @param routing
 */
void Reveal__SetRouterMode(uint8 node,REVEAL_ROUTING_MODE_TYPE routing)
{
    if (node < sizeof(Reveal_Routing))
    {
        Reveal_Routing[node] = routing;
    }
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used for system to get dynamic configuration of the router.
 * @param node
 * @return router type.
 */
uint8 Reveal__GetRouterMode(uint8 node)
{
    uint8 router_type;
    //router_type = REVEAL_ROUTING_EXTERNAL_DEF;
    if (node < sizeof(Reveal_Routing))
    {
        router_type = Reveal_Routing[node];
    }
    return router_type;
}
#endif
//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Creates a empty Reveal message and them populate api, opcode and node fields and by default set the feedback flag.
 *
 *    @details  This function should be called whenever a new outgoing Reveal feedback message is desired.
 *              Object creation of a new message is simulated by returning a pointer into a queue of structures.
 *              The specified api and opcode are set for the new message and the length is initialized to 0.
 *
 *    @param    api - the api id of the command
 *    @param    opcode - the opcode of the command
 *    @param    node - the node id to send the message to
 *    @param    size - size of the data byte to be transfered excluding the API and OPCODE
 *    @return   REVEAL_COMMAND_TYPE* - a pointer to the reveal message structure
 */
REVEAL_COMMAND_TYPE * Reveal__NewFeedback(uint8 api, uint8 opcode, uint8 node,uint8 size)
{
    uint8 take_message;
    uint8 aux;
    take_message = FALSE;

    // Determine if the message is currently blocked
    // Rely on short circuit evaluation to increase possibility of shorter execution time
    if (!((MESSAGE_BLOCKING_ACTIVE() == TRUE) &&
          (IsMessageBlocked(api, opcode) == TRUE)))
    {
        if (node <= REVEAL_BROADCAST_NODE)
        {
            #if (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_DISABLED)

                Reveal_Aux = (uint8)(Reveal_Queue_Write + 1);
                if (Reveal_Aux >= REVEAL_QUEUE_SIZE)
                {
                    Reveal_Aux = 0;
                }

                #if (REVEAL_QUEUE_DEBUG == ENABLED)
                    if (Reveal_Queue_Write >= Reveal_Queue_Read)
                    {
                        aux = REVEAL_QUEUE_SIZE - 1 - (   Reveal_Queue_Write  - Reveal_Queue_Read);
                    }
                    else
                    {
                        aux = Reveal_Queue_Read - Reveal_Queue_Write - 1;
                    }

                    if (Reveal_Min_Space > aux)
                    {
                        Reveal_Min_Space = aux;
                    }
                #endif


                if (Reveal_Aux != Reveal_Queue_Read)
                {
                    take_message = TRUE;
                }
            #elif (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_ONLY)
                take_message = TRUE;
            #else
                // internal / external message
                if (node < REVEAL_BROADCAST_NODE)
                {
                    if (Reveal_Routing[node] == REVEAL_ROUTING_INTERNAL)
                    {
                        // internal message is always accepted
                        take_message = TRUE;
                    }
                    else
                    {
                        // external message depends on the queue available space
                        Reveal_Aux = (uint8)(Reveal_Queue_Write + 1);
                        if (Reveal_Aux >= REVEAL_QUEUE_SIZE)
                        {
                            Reveal_Aux = 0;
                        }

                        #if (REVEAL_QUEUE_DEBUG == ENABLED)
                            if (Reveal_Queue_Write > Reveal_Queue_Read)
                            {
                                aux = REVEAL_QUEUE_SIZE - 1 - (   Reveal_Queue_Write  - Reveal_Queue_Read);
                            }
                            else
                            {
                                aux = Reveal_Queue_Write  + (REVEAL_QUEUE_SIZE - Reveal_Queue_Read - 1);
                            }

                            if (Reveal_Min_Space > aux)
                            {
                                Reveal_Min_Space = aux;
                            }
                        #endif

                        if (Reveal_Aux != Reveal_Queue_Read)
                        {
                            take_message = TRUE;
                        }
                    }
                }
                else
                {
                    take_message = TRUE;
                }
            #endif
        }
        if (take_message == TRUE)
        {
            Reveal_Single_Queue.api = api;
            Reveal_Single_Queue.opcode = opcode;
            Reveal_Single_Queue.node = node;
            Reveal_Single_Queue.length = 0;
            Reveal_Single_Queue.priority = FALSE;
            Reveal_Single_Queue.feedback  =1;
            Reveal_Single_Queue.timeout = 0;
            return (&Reveal_Single_Queue);
        }
        else
        {
            REVEAL_QUEUE_OVERFLOW_NORMAL_EXCEPTION(api,opcode);
        }
    }
    // else, message is currently blocked

    return ((REVEAL_COMMAND_TYPE *)NULL);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Calls Reveal__NewFeedback with the feedback parameter to a command format
 *              and adds it to the message queue.
 *
 *    @details  This function should be called whenever a new outgoing Reveal command message is desired.
 *              Object creation of a new message is simulated by returning a pointer into a queue of structures.
 *              The specified api and opcode are set for the new message and the length is initialized to 0.
 *
 *    @param    api - the api id of the command
 *    @param    opcode - the opcode of the command
 *    @param    node - the node id to send the message to
 *    @param    size - size of the data byte to be transfered excluding the API and OPCODE
 *    @return   REVEAL_COMMAND_TYPE* - a pointer to the reveal message structure
 */
REVEAL_COMMAND_TYPE * Reveal__NewCommand(uint8 api, uint8 opcode, uint8 node, uint8 size)
{
    REVEAL_COMMAND_TYPE * pt;
    pt = Reveal__NewFeedback(api,opcode,node,size);
    if (pt != (REVEAL_COMMAND_TYPE *)NULL)
    {
        pt->feedback = 0;
    }
    return (pt);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Creates a empty Reveal message and them populate api, opcode and node fields and by default set the feedback flag.
 *
 *    @details  This function should be called whenever a new outgoing Reveal feedback message is desired.
 *              Object creation of a new message is simulated by returning a pointer into a queue of structures.
 *              The specified api and opcode are set for the new message and the length is initialized to 0.
 *
 *    @param    api - the api id of the command
 *    @param    opcode - the opcode of the command
 *    @param    node - the node id to send the message to
 *    @param    size - size of the data byte to be transfered excluding the API and OPCODE
 *    @return   REVEAL_COMMAND_TYPE* - a pointer to the reveal message structure
 */
REVEAL_COMMAND_TYPE * Reveal__PriorityNewFeedback(uint8 api, uint8 opcode, uint8 node, uint8 size)
{
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    uint8 take_message;
    uint8 aux;
    take_message = FALSE;
    if (node <= REVEAL_BROADCAST_NODE)
    {
        #if (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_DISABLED)

            Reveal_Aux = (uint8)(Reveal_Queue_Priority_Write + 1);
            if (Reveal_Aux >= REVEAL_QUEUE_PRIORITY_SIZE)
            {
                Reveal_Aux = 0;
            }

            #if (REVEAL_QUEUE_DEBUG == ENABLED)
                if (Reveal_Queue_Priority_Write >= Reveal_Queue_Priority_Read)
                {
                    aux = REVEAL_QUEUE_PRIORITY_SIZE - 1 - (   Reveal_Queue_Priority_Write  - Reveal_Queue_Priority_Read);
                }
                else
                {
                    aux = Reveal_Queue_Priority_Read - Reveal_Queue_Priority_Write  - 1;
                }

                if (Reveal_Priority_Min_Space > aux)
                {
                    Reveal_Priority_Min_Space = aux;
                }
            #endif

            if (Reveal_Aux != Reveal_Queue_Priority_Read)
            {
                take_message = TRUE;
            }
        #elif (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_ONLY)
            take_message = TRUE;
        #else
            // internal / external message
            if (node < REVEAL_BROADCAST_NODE)
            {
                if (Reveal_Routing[node] == REVEAL_ROUTING_INTERNAL)
                {
                    // internal message is always accepted
                    take_message = TRUE;
                }
                else
                {
                    // external message depends on the queue available space
                    Reveal_Aux = (uint8)(Reveal_Queue_Priority_Write + 1);
                    if (Reveal_Aux >= REVEAL_QUEUE_SIZE)
                    {
                        Reveal_Aux = 0;
                    }

                    #if (REVEAL_QUEUE_DEBUG == ENABLED)
                        if (Reveal_Queue_Priority_Write > Reveal_Queue_Priority_Read)
                        {
                            aux = REVEAL_QUEUE_PRIORITY_SIZE - 1 - (   Reveal_Queue_Priority_Write  - Reveal_Queue_Priority_Read);
                        }
                        else
                        {
                            aux = Reveal_Queue_Priority_Write  + (REVEAL_QUEUE_PRIORITY_SIZE - Reveal_Queue_Priority_Read - 1);
                        }

                        if (Reveal_Priority_Min_Space > aux)
                        {
                            Reveal_Priority_Min_Space = aux;
                        }
                    #endif

                    if (Reveal_Aux != Reveal_Queue_Priority_Read)
                    {
                        take_message = TRUE;
                    }
                }
            }
            else
            {
                take_message = TRUE;
            }
        #endif
     }
    if (take_message == TRUE)
    {
        Reveal_Single_Queue.api = api;
        Reveal_Single_Queue.opcode = opcode;
        Reveal_Single_Queue.node = node;
        Reveal_Single_Queue.length = 0;
        Reveal_Single_Queue.priority = TRUE;
        Reveal_Single_Queue.feedback  =1;
        Reveal_Single_Queue.timeout = 0;
        return (&Reveal_Single_Queue);
    }
    else
    {
        REVEAL_QUEUE_OVERFLOW_PRIORITY_EXCEPTION(api,opcode);
        return ((REVEAL_COMMAND_TYPE *)NULL);
    }
#else
    return (Reveal__NewFeedback(api,opcode,node,size));
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Calls Reveal__NewProrityFeedback with  the feedback parameter to a command format
 *              and adds it to the message queue.
 *
 *    @details  This function should be called whenever a new outgoing Reveal command message is desired.
 *              Object creation of a new message is simulated by returning a pointer into a queue of structures.
 *              The specified api and opcode are set for the new message and the length is initialized to 0.
 *
 *    @param    api - the api id of the command
 *    @param    opcode - the opcode of the command
 *    @param    node - the node id to send the message to
 *    @param    size - size of the data byte to be transfered excluding the API and OPCODE
 *    @return   REVEAL_COMMAND_TYPE* - a pointer to the reveal message structure
 */
REVEAL_COMMAND_TYPE * Reveal__PriorityNewCommand(uint8 api, uint8 opcode, uint8 node, uint8 size)
{
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    REVEAL_COMMAND_TYPE * pt;
    pt = Reveal__PriorityNewFeedback(api, opcode, node, size);
    if (pt != (REVEAL_COMMAND_TYPE *)NULL)
    {
        pt->feedback = 0;
    }
    return (pt);
#else
    return (Reveal__NewCommand(api,opcode,node,size));
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to debug the minimum space available in the Reveal queue over time.
 * @return uint8 which represents the minimum space available in the Reveal Queue
 */
uint8 Reveal__GetMinimumQueueSpace(void)
{
#if (REVEAL_QUEUE_DEBUG == ENABLED)
    return (Reveal_Min_Space);                      // return the minimum available space
#else
    return (0);                                     // debug code disabled reports no space available
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method is used to debug the minimum space available in the Reveal queue over time.
 * @return uint8 which represents the minimum space available in the priority Reveal Queue
 */
uint8 Reveal__GetMinimumPriorityQueueSpace(void)
{
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    #if (REVEAL_QUEUE_DEBUG == ENABLED)
        return (Reveal_Priority_Min_Space);         // return the minimum available space
    #else
        return (0);                                 // debug code disabled reports no space available
    #endif
#else
    return (0);                                     // priority queue disabled reports no space available
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Add an 8-bit uint8 data type to a Reveal message.
 *
 *    @details  This function adds the specified data type to a Reveal queue driver message. This is a
 *              memory endianess independent method that will result in a big endian message. This function
 *              should be called in order each time another piece of data needs to be packed into the
 *              message structure.
 *
 *    @param    command* - pointer to the reveal message structure
 *    @param    u8 - the data byte to add to the message
 *    @return   PASS_FAIL_TYPE
 */
PASS_FAIL_TYPE Reveal__AddDataU8(REVEAL_COMMAND_TYPE * command, uint8 u8)
{
    PASS_FAIL_TYPE retval;
    retval = FAIL;
    if (command != (REVEAL_COMMAND_TYPE *)NULL)
    {
        if (command->length < REVEAL_PAYLOAD_SIZE)
        {
            command->data[command->length ] = u8;
            command->length += 1;
            retval = PASS;
        }
    }
    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Add a character buffer to a Reveal message.
 *
 *    @details  This function is a helper method and should only be used when a a character buffer
 *              is to be added to a message. It should be used with caution as it does not manage
 *              endianess like the other AddData functions in this module. The maximum size of a
 *              buffer to add is equal to the max REVEAL_PAYLOAD_SIZE.
 *
 *    @param    *command - pointer to the reveal message struct
 *    @param    *u8_buffer - pointer to the character buffer to add to the message
 *    @param    length - length of the character buffer
 *    @return   void
 */
PASS_FAIL_TYPE Reveal__AddDataU8Buffer(REVEAL_COMMAND_TYPE* command, uint8 *u8_buffer, uint8 length)
{
    PASS_FAIL_TYPE retval;
    retval = FAIL;

    if (command != (REVEAL_COMMAND_TYPE *)NULL)
    {
        retval = PASS;
        if(length > (REVEAL_PAYLOAD_SIZE - command->length))
        {
            length = (uint8)(REVEAL_PAYLOAD_SIZE - command->length);
            retval = FAIL;
        }
        memcpy(&command->data[command->length],u8_buffer,length);
        command->length += length;

    }
    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Add a 16-bit uint16 data type to a Reveal message.
 *
 *    @details  This function adds the specified data type to a Reveal queue driver message. This is a
 *              memory endianess independent method that will result in a big endian message. This function
 *              should be called in order each time another piece of data needs to be packed into the
 *              message structure.
 *
 *    @param    command* - pointer to the reveal message structure
 *    @param    u16 - the 16-bit data to add to the message
 *    @return   void
 */
PASS_FAIL_TYPE Reveal__AddDataU16(REVEAL_COMMAND_TYPE* command, uint16 u16)
{
    uint8 index;
    PASS_FAIL_TYPE retval;
    retval = FAIL;

    if (command != (REVEAL_COMMAND_TYPE *)NULL)
    {
        index = command->length;
        if (index < (REVEAL_PAYLOAD_SIZE - 1))
        {
            command->data[ index   ] = (uint8)(u16>>8);
            command->data[(index)+1] = (uint8) u16;
            command->length += 2;
            retval = PASS;
        }
    }

    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Add a 16-bit sint16 data type to a Reveal message.
 *
 *    @details  This function adds the specified data type to a Reveal queue driver message. This is a
 *              memory endianness independent method that will result in a big endian message. This function
 *              should be called in order each time another piece of data needs to be packed into the
 *              message structure.
 *
 *    @param    command* - pointer to the reveal message structure
 *    @param    s16 - the 16-bit data to add to the message
 *    @return   void
 */
PASS_FAIL_TYPE Reveal__AddDataS16(REVEAL_COMMAND_TYPE* command, sint16 s16)
{
    uint8 index;
    PASS_FAIL_TYPE retval;
    retval = FAIL;
    if (command != (REVEAL_COMMAND_TYPE *)NULL)
    {
        index = command->length;
        if (index < (REVEAL_PAYLOAD_SIZE - 1))
        {
            command->data[ index   ] = (uint8)(((uint16)s16 & 0xFF00) >> 8);
            command->data[(index)+1] =  (uint8)((uint16)s16 & 0x00FF);
            command->length += 2;
            retval = PASS;
        }
    }
    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Add a 32-bit uint32 data type to a Reveal message.
 *
 *    @details  This function adds the specified data type to a Reveal queue driver message. This is a
 *              memory Endianess independent method that will result in a big endian message. This function
 *              should be called in order each time another piece of data needs to be packed into the
 *              message structure.
 *
 *    @param    command* - pointer to the reveal message structure
 *    @param    u32 - the 32-bit data to add to the message
 *    @return   void
 */
PASS_FAIL_TYPE Reveal__AddDataU32(REVEAL_COMMAND_TYPE* command, uint32 u32)
{
    uint8 index;
    PASS_FAIL_TYPE retval;
    retval = FAIL;
    if (command != (REVEAL_COMMAND_TYPE *)NULL)
    {
        index = command->length;
        if (index < (REVEAL_PAYLOAD_SIZE - 3))
        {
            command->data[ index   ] = (uint8)(u32>>24);
            command->data[(index)+1] = (uint8)(u32>>16);
            command->data[(index)+2] = (uint8)(u32>>8 );
            command->data[(index)+3] = (uint8) u32;
            command->length += 4;
            retval = PASS;
        }
    }
    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Adds a completed message to the Reveal queue for transmission on the LLC bus.
 *
 *    @details  Once a Reveal message structure is created and its bytes are added this function
 *              should be called as the final step for transmitting a message.
 *
 *    @param   command
 *    @return  success / fail
 */
BOOL_TYPE Reveal__QueueCommand(REVEAL_COMMAND_TYPE * command)
{
    BOOL_TYPE retval;
#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_DISABLED)
    REVEAL_MSG_TYPE Reveal_Transfer_Buffer;
    REVEAL_RECIPE_STATUS_TYPE reasoncode;
    uint8 send_internal;
    #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
        uint8 send_external;
        send_external = TRUE;                           //Set by default external routing to TRUE
    #endif
#endif
    retval = FALSE;
    if ((command != (REVEAL_COMMAND_TYPE *)NULL) &&
        (!((MESSAGE_BLOCKING_ACTIVE() == TRUE) &&
        (IsMessageBlocked(command->api, command->opcode) == TRUE))))
    {
        #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_DISABLED)
            send_internal = TRUE;

        #if (REVEAL_ROUTER_FEATURE == REVEAL_ROUTER_INTERNAL_EXTERNAL)

                if (command->node < REVEAL_BROADCAST_NODE)         //Check destination node is lesser than REVEAL_BROADCAST_NODE '15'
                {
                    if (Reveal_Routing[command->node] == REVEAL_ROUTING_INTERNAL)
                    {
                        send_external = FALSE;          //Routing Internally
                    }
                    else
                    {
                        send_internal = FALSE;          //Routing externally
                    }
                }
            #if (REVEAL_ROUTERING_BROADCAST_EXTERNAL == DISABLED)
                else
                {
                    send_external = FALSE;
                }
            #endif

        #endif
                ////////////////////////////Start of Internal Routing ////////////////////////////////
            if (send_internal == TRUE)
            {
                Reveal_Transfer_Buffer.destination = command->node;
                Reveal_Transfer_Buffer.node_id  = 0xF;
                Reveal_Transfer_Buffer.sdu_length = (uint8)(command->length + 2);
                Reveal_Transfer_Buffer.sap = REVEAL_SAP;
                Reveal_Transfer_Buffer.api_id = command->api;
                // it copies: opcode, frag, mmp and feedback
                Reveal_Transfer_Buffer.op_code = (command->opcode & 0x1F);

                Reveal_Transfer_Buffer.cmd_fb = 0;
                Reveal_Transfer_Buffer.frag = 0;
                Reveal_Transfer_Buffer.mmp = 0;

                if (BIT_TEST((command->opcode), 5))
                {
                Reveal_Transfer_Buffer.cmd_fb = 1;
                }

                if (BIT_TEST((command->opcode), 6))
                {
                Reveal_Transfer_Buffer.frag = 1;
                }

                if (BIT_TEST((command->opcode), 7))
                {
                Reveal_Transfer_Buffer.mmp = 1;
                }
                // it overwrites the feedback
                Reveal_Transfer_Buffer.cmd_fb = command->feedback;
                memcpy(Reveal_Transfer_Buffer.payload,command->data,command->length);
                Reveal_Transfer_Buffer.size = command->length;


                if (Reveal_Queue_Reentry_Counter < REVEAL_QUEUE_MAX_REENTRIES)
                {
                    Reveal_Queue_Reentry_Counter++;

                    reasoncode = RevealHandleParser(&Reveal_Transfer_Buffer,Reveal_Transfer_Buffer.cmd_fb, TRUE, REVEAL_FULL_SERACH_INTERNAL);
                    if (Reveal_Transfer_Buffer.cmd_fb == 0)
                    {
                        // Command
                        RevealHandleAckParser(Reveal_Transfer_Buffer.node_id,Reveal_Transfer_Buffer.api_id,Reveal_Transfer_Buffer.op_code,reasoncode,TRUE);
                    }

                    Reveal_Queue_Reentry_Counter--;
                    retval = TRUE;
                }
                else
                {
                    retval = FALSE;
                    REVEAL_EXCEPTION_REENTRY_CNT_OVERLOADED(Reveal_Transfer_Buffer.api_id,Reveal_Transfer_Buffer.op_code);
                }
            }
            ////////////////////////////End of Internal Routing ////////////////////////////////

          #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
            ////////////////////////////Start of External Routing ////////////////////////////////
            if (send_external == TRUE)
          #endif
        #endif
        #if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
            {
                #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                    if (command->priority == TRUE)
                    {
                        memcpy((uint8 *)&Reveal_Queue_Priority[Reveal_Queue_Priority_Write],(uint8 *)command,REVEAL_MSG_OVERHEAD + command->length);


                        // High Priority
                        Reveal_Aux = Reveal_Queue_Priority_Write;
                        Reveal_Aux++;
                        if (Reveal_Aux >= REVEAL_QUEUE_PRIORITY_SIZE)
                        {
                            Reveal_Aux = 0;
                        }
                        if (Reveal_Aux != Reveal_Queue_Priority_Read)
                        {
                            Reveal_Queue_Priority_Write = Reveal_Aux;
                            retval = TRUE;
                        }
                        else
                        {
                            REVEAL_QUEUE_OVERFLOW_PRIORITY_EXCEPTION(command->api,command->opcode);
                        }
                    }
                    else
                #endif
                    {
                        memcpy(&Reveal_Queue[Reveal_Queue_Write],command,REVEAL_MSG_OVERHEAD + command->length);

                        Reveal_Aux = Reveal_Queue_Write;
                        Reveal_Aux++;
                        if (Reveal_Aux >= REVEAL_QUEUE_SIZE)
                        {
                            Reveal_Aux = 0;
                        }
                        if (Reveal_Aux != Reveal_Queue_Read)
                        {
                            Reveal_Queue_Write = Reveal_Aux;
                            retval = TRUE;
                        }
                        else
                        {
                            REVEALQUEUE_QUEUE_OVERFLOW_NORMAL_EXCEPTION(command->api,command->opcode);
                        }
                    }
            }
            ////////////////////////////End of External Routing ////////////////////////////////
        #endif
    }
    return (retval);
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This function returns the free space in the queue
 * @return return free space in the priority queue
 */
uint8 Reveal__GetFreeQueueSpace(void)
{
    uint8 retval;
    retval = 0;

    if (Reveal_Queue_Write >= Reveal_Queue_Read)
    {
        retval = REVEAL_QUEUE_SIZE - 1 - (   Reveal_Queue_Write  - Reveal_Queue_Read);
    }
    else
    {
        retval = Reveal_Queue_Read - Reveal_Queue_Write - 1;
    }

    return(retval);

}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This Function returns the number of messages in the queue
 * @return number of messages in queue
 */
uint8 Reveal__GetNumMessagesQueue(void)
{
    uint8 retval;
    retval = 0;

    if (Reveal_Queue_Write >= Reveal_Queue_Read)
    {
        retval = Reveal_Queue_Write  - Reveal_Queue_Read;
    }
    else
    {
        retval = REVEAL_QUEUE_SIZE - Reveal_Queue_Read + Reveal_Queue_Write;
    }

    return(retval);
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This function returns the free space in the priority queue
 * @return return free space in the priority queue, if the priority queue is disable returns '0'
 */
uint8 Reveal__GetFreePriorityQueueSpace(void)
{
    uint8 retval;
    retval = 0;
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    if (Reveal_Queue_Priority_Write >= Reveal_Queue_Priority_Read)
    {
        retval = REVEAL_QUEUE_PRIORITY_SIZE - 1 - (   Reveal_Queue_Priority_Write  - Reveal_Queue_Priority_Read);
    }
    else
    {
        retval = Reveal_Queue_Priority_Read - Reveal_Queue_Priority_Write - 1;
    }
#endif

    return(retval);

}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This Function returns the number of messages in the priority queue
 * @return number of messages in priority queue, if the priority queue is disable returns '0'
 */
uint8 Reveal__GetNumMessagesPriorityQueue(void)
{
    uint8 retval;
    retval = 0;
#if (REVEAL_QUEUE_PRIORITY == ENABLED)
    if (Reveal_Queue_Priority_Write >= Reveal_Queue_Priority_Read)
    {
        retval = Reveal_Queue_Priority_Write  - Reveal_Queue_Priority_Read;
    }
    else
    {
        retval = REVEAL_QUEUE_PRIORITY_SIZE - Reveal_Queue_Priority_Read + Reveal_Queue_Priority_Write;
    }
#endif

    return(retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Send the command to configure the heartbeat time in another node.
 * @param node
 * @param time
 */
void Reveal__SetNodeHeartBeat(uint8 node, uint16 time)
{
    REVEAL_COMMAND_TYPE * ptr;
    if (node < 0xF)
    {
        ptr = Reveal__NewCommand(API001CORE_NUM,API001_SET_HEARTBEAT,node,2);
        if (ptr!=(REVEAL_COMMAND_TYPE *)NULL)
        {
            Reveal__AddDataU16(ptr,time);
            Reveal__QueueCommand(ptr);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method exposes the status of the nodes based on Reveal messages.
 * @return
 */
uint16 Reveal__GetNodestatus(void)
{
    return (Reveal_Node_Status);
}

#if (REVEAL_1S_EXTERNAL_REFFERENCE == ENABLED)
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief External procedure to be called every 1s in order to generate the heart beat
 */
void Reveal__1sHandler(void)
{
    Reveal1sTasks();

    if (Reveal_Heartbeat_Timer > 0)
    {
        uint8 nd;

        for(nd=0;nd<sizeof(Reveal_Heartbeat_Node_Counter);nd++)
        {
            if (nd != LLC_OWN_ADDRESS)
            {
                if (Reveal_Heartbeat_Node_Counter[nd] > 0)
                {
                    Reveal_Heartbeat_Node_Counter[nd]--;
                    if (Reveal_Heartbeat_Node_Counter[nd] == 0)
                    {
                        // Fault detected
                        BIT_CLR(Reveal_Node_Status,nd);
                    }
                }
            }
        }
    }
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add an 8-bit uint8 data type to a Reveal message.
 * @param api - the api id of the command
 * @param opcode - the opcode of the command
 * @param node - the node id to send the message to
 * @param is_command - TRUE = Command, FALSE = Feedback
 * @param is_priority TRUE = Priority, FALSE = Normal
  * @return result of the operation
 *  @retval TRUE - Message was successfully queued
 *  @retval FALSE - Message failed to queued
 */
BOOL_TYPE Reveal__QueueMessageNoData(uint8 api, uint8 opcode, uint8 node,BOOL_TYPE is_command, BOOL_TYPE is_priority )
{
    REVEAL_COMMAND_TYPE * ptr;
    if(is_command == TRUE)
    {
        ptr = Reveal__NewCommand(api, opcode, node,0);
    }
    else
    {
        ptr = Reveal__NewFeedback(api, opcode, node,0);
    }
    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__QueueCommand(ptr);
        return (TRUE);
    }
    else
    {
        return(FALSE);
    }


}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add an 8-bit uint8 data type to a Reveal message.
 * @param api - the api id of the command
 * @param opcode - the opcode of the command
 * @param node - the node id to send the message to
 * @param is_command - TRUE = Command, FALSE = Feedback
 * @param is_priority TRUE = Priority, FALSE = Normal
 * @param data - the data byte to add to the message
 * @return result of the operation
 *  @retval TRUE - Message was successfully queued
 *  @retval FALSE - Message failed to queued
 */
BOOL_TYPE Reveal__QueueMessageU8(uint8 api, uint8 opcode, uint8 node,BOOL_TYPE is_command, BOOL_TYPE is_priority,uint8 data )
{
    REVEAL_COMMAND_TYPE * ptr;
    if(is_command == TRUE)
    {
        ptr = Reveal__NewCommand(api, opcode, node,1);
    }
    else
    {
        ptr = Reveal__NewFeedback(api, opcode, node,1);
    }
    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__AddDataU8(ptr, data);
        Reveal__QueueCommand(ptr);
        return (TRUE);
    }
    else
    {
        return(FALSE);
    }


}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a 16-bit uint16 data type to a Reveal message.
 * @param api - the api id of the command
 * @param opcode - the opcode of the command
 * @param node - the node id to send the message to
 * @param is_command TRUE = Command, FALSE = Feedback
 * @param is_priority TRUE = Priority, FALSE = Normal
 * @param data - the 16-bit data to add to the message
 * @return result of the operation
 *  @retval TRUE - Message was successfully queued
 *  @retval FALSE - Message failed to queued
 */
BOOL_TYPE Reveal__QueueMessageU16(uint8 api, uint8 opcode, uint8 node,BOOL_TYPE is_command ,BOOL_TYPE is_priority, uint16 data)
{
    REVEAL_COMMAND_TYPE * ptr;
    if(is_command == TRUE)
    {
        ptr = Reveal__NewCommand(api, opcode, node,2);
    }
    else
    {
        ptr = Reveal__NewFeedback(api, opcode, node,2);
    }
    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__AddDataU16(ptr, data);
        Reveal__QueueCommand(ptr);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a 16-bit sint16 data type to a Reveal message.
 * @param api - the api id of the command
 * @param opcode - the opcode of the command
 * @param node - the node id to send the message to
 * @param is_command TRUE = Command, FALSE = Feedback
 * @param is_priority TRUE = Priority, FALSE = Normal
 * @param data - the 16-bit data to add to the message
 * @return result of the operation
 *  @retval TRUE - Message was successfully queued
 *  @retval FALSE - Message failed to queued
 */
BOOL_TYPE Reveal__QueueMessageS16(uint8 api, uint8 opcode, uint8 node,BOOL_TYPE is_command ,BOOL_TYPE is_priority, sint16 data)
{
    REVEAL_COMMAND_TYPE * ptr;
    if(is_command == TRUE)
    {
        ptr = Reveal__NewCommand(api, opcode, node,2);
    }
    else
    {
        ptr = Reveal__NewFeedback(api, opcode, node,2);
    }
    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__AddDataS16(ptr, data);
        Reveal__QueueCommand(ptr);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a 32-bit uint32 data type to a Reveal message.
 * @param api - the api id of the command
 * @param opcode - the opcode of the command
 * @param node - the node id to send the message to
 * @param is_command TRUE = Command, FALSE = Feedback
 * @param is_priority TRUE = Priority, FALSE = Normal
 * @param data - the 32-bit data to add to the message
 * @return result of the operation
 *  @retval TRUE - Message was successfully queued
 *  @retval FALSE - Message failed to queued
 */
BOOL_TYPE Reveal__QueueMessageU32(uint8 api, uint8 opcode, uint8 node,BOOL_TYPE is_command ,BOOL_TYPE is_priority, uint32 data)
{
    REVEAL_COMMAND_TYPE * ptr;
    if(is_command == TRUE)
    {
        ptr = Reveal__NewCommand(api, opcode, node,4);
    }
    else
    {
        ptr = Reveal__NewFeedback(api, opcode, node,4);
    }
    if (ptr != (REVEAL_COMMAND_TYPE *) NULL)
    {
        Reveal__AddDataU32(ptr, data);
        Reveal__QueueCommand(ptr);
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 *    @brief    Determines if the combination of api and opcode are currently blocked
 *
 *    @param    api - the api id of the command
 *    @param    opcode - the opcode of the command
 *
 *    @return  TRUE if the message is blocked, else FALSE
 */
static BOOL_TYPE IsMessageBlocked(uint8 api, uint8 opcode)
{
    BOOL_TYPE is_message_blocked = FALSE;

    // Walk the POSSIBLE_BLOCKED_APIS_LIST table to find the index that corresponds to Reveal_Blocked_Messages array
    for (uint8 index = 0; index < ELEMENTS_IN_ARRAY(POSSIBLE_BLOCKED_APIS_LIST); index++)
    {
        if (api == POSSIBLE_BLOCKED_APIS_LIST[index])
        {
            // Found index in POSSIBLE_BLOCKED_APIS_LIST table with the API
            // Check bit that corresponds to the opcode to know if it is currently blocked
            is_message_blocked = (BOOL_TYPE)(BIT_TEST(Reveal_Blocked_Messages[index], opcode));
            break;
        }
    }

    return(is_message_blocked);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method handles the incoming messages
 * @details This method shall be called every5ms
 */
static void RevealHandlerIncomming(void)
{
#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    REVEAL_RECIPE_STATUS_TYPE reason_code;
    PT_RX_TYPE * pt;
    REVEAL_COMMAND_TYPE * new_msg;
    uint8 api;
    uint8 op_code;
    uint8 index;
    uint8 count;
    uint8 feedback;
    uint32 opcode_bitmap;
#if (REVEAL_READ_EEPROM_FEATURE == ENABLED)
    uint32 eeprom_address;
    uint16 eeprom_size;
#endif

#if (REVEAL_READ_MEMORY_FEATURE == ENABLED)
    #if (REVEAL_READ_MEMORY_END > 0xFFFF)
        #ifndef FAR
            #define FAR
        #endif

        #define REVEAL_MEM_POINTER_CAST FAR
    #else
        #ifndef NEAR
            #define NEAR
        #endif
        #define REVEAL_MEM_POINTER_CAST NEAR
    #endif
    uint32 memory_idx;
    uint8 memory_size;
#endif

    // Handle Incoming message
    reason_code = NVO_RS_UNSUPPORTED;
    pt = GET__MESSAGE(REVEAL_SAP);
    if (pt != (PT_RX_TYPE *)NULL)
    {

        BIT_SET(Reveal_Node_Status,pt->source);

        if (Reveal_Heartbeat_Timer > 0)
        {
            // We reload the heartbeat even if the message is not a heartbeat.
            // the point is to if we see communication for a node that means that node is alive.
            // This implementation intend to reduce SIR for no communication when there is communication.
            if (pt->source < sizeof(Reveal_Heartbeat_Node_Counter))
            {
                Reveal_Heartbeat_Node_Counter[pt->source] = (Reveal_Heartbeat_Timer * 3) + 1;
            }
        }

        api = pt->payload[0];
        op_code = (uint8)(pt->payload[1] & 0x1F);
        feedback = (uint8)((pt->payload[1] >> 5) & 1);
        if (api == 0x1)
        {
            if(feedback == TRUE)
            {
                // Feedback
                switch (op_code)
                {
                    case API001_PUBLISH_ACK: // ACK
                        if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                        {
                           if ((Reveal_Queue[Reveal_Queue_Read].api == pt->payload[3]) &&
                               (Reveal_Queue[Reveal_Queue_Read].opcode == pt->payload[4]))
                           {
                               // Command acknowledge
                               DropMessageFromQueue(FALSE);
                               Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                               RevealHandleAckParser(pt->source, pt->payload[3], pt->payload[4],(REVEAL_RECIPE_STATUS_TYPE)pt->payload[2],FALSE);
                           }
                        }
                    #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                        else if (Reveal_Outgoing_State  == REVEAL_OUTGOING_STATE_TIMING_PRIORITY)
                        {
                            if ((Reveal_Queue_Priority[Reveal_Queue_Priority_Read].api == pt->payload[3]) &&
                                (Reveal_Queue_Priority[Reveal_Queue_Priority_Read].opcode == pt->payload[4]))
                            {
                                // Command acknowledge
                                DropMessageFromQueue(TRUE);
                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                                RevealHandleAckParser(pt->source, pt->payload[3], pt->payload[4],(REVEAL_RECIPE_STATUS_TYPE)pt->payload[2],FALSE);
                            }
                        }
                    #endif
                        break;
                    case API001_PUBLISH_HEARTBEAT: // Heart-beat
                        REVEAL_HEARTBEAT_CALLBACK(pt->source);
                        break;
                    case API001_PUBLISH_WAKEUP:

                        new_msg = Reveal__NewFeedback(0x1,API001_PUBLISH_ACK_WAKEUP, REVEAL_BROADCAST_NODE,1);
                        if (new_msg != (REVEAL_COMMAND_TYPE *)NULL)
                        {
                           Reveal__AddDataU8(new_msg,pt->source);
                           Reveal__QueueCommand(new_msg);
                        }

                        break;
                    case API001_PUBLISH_ACK_WAKEUP:
                        if (pt->payload[2] < 15)
                        {
                            BIT_SET(Reveal_Node_Status,pt->payload[2]);
                        }
                        break;
                    case API001_MAX_MESSAGE_SIZE:
                        REVEAL_MAX_PAYLOAD_CALLBACK(pt->source,pt->payload[2]);
                        break;
                    default:
                        reason_code = NVO_RS_UNSUP_OP_CODE;
                        break;
                }
            }
            else
            {
                // Command
                switch ((API001CORE_OPCODE_TYPE)op_code)
                {
                    case API001_SET_HEARTBEAT: // Set Heart-beat
                        reason_code = NVO_RS_READY;
                        Reveal_Heartbeat_Timer = (((uint16)(pt->payload[2])) <<8) + (uint16)(pt->payload[3]);
                        new_msg = Reveal__NewFeedback(0x1,API001_PUBLISH_HEARTBEAT_PERIOD, 0xF,2);
                        if (new_msg != (REVEAL_COMMAND_TYPE *)NULL)
                        {
                            Reveal__AddDataU8(new_msg,pt->payload[2]);
                            Reveal__AddDataU8(new_msg,pt->payload[3]);
                            Reveal__QueueCommand(new_msg);
                        }
                        break;
#if (REVEAL_READ_EEPROM_FEATURE == ENABLED)
                    case API001_READ_EE:  // Read eeprom
                        reason_code = NVO_RS_READY;
                        eeprom_address = ( ((uint32)pt->payload[2])<<16 );
                        eeprom_address +=( ((uint32)pt->payload[3])<<8 );
                        eeprom_address +=( ((uint32)pt->payload[4]) );
                        eeprom_size =( ((uint32)pt->payload[3])<<8 );
                        eeprom_size +=( ((uint32)pt->payload[4]) );
                        REVEAL_EEPROM_READ(pt->source,eeprom_address,eeprom_size);

                        break;
#endif
                    case API001_SEND_EVENTS: // Send Events
                        count = pt->sdu_length - 3; // remove API, OPCODE and API Id
                        while (count > 0)
                        {
                            REVEAL__SENDEVENTS(pt->payload[2],pt->payload[2+count]);
                            count--;
                        }
                        reason_code = NVO_RS_READY;
                        break;
                #if (REVEAL_READ_MEMORY_FEATURE == ENABLED)

                    #if (REVEAL_READ_MEMORY_END < 0x1FFFFFF)
                        case API001_READ_MEM_24B: // Read memory

                            if ((pt->payload[5] > 0) ||
                                (pt->payload[6] > REVEAL_PAYLOAD_SIZE))
                            {
                                memory_size = REVEAL_PAYLOAD_SIZE;
                            }
                            else
                            {
                                memory_size = pt->payload[6];
                            }
                            memory_idx = ( ((uint32)pt->payload[2])<<16 );
                            memory_idx +=( ((uint32)pt->payload[3])<<8 );
                            memory_idx +=( ((uint32)pt->payload[4]) );
                            if (
                                #if (REVEAL_READ_MEMORY_START > 0)
                                    (memory_idx < (uint32)REVEAL_READ_MEMORY_START) ||
                                #endif
                                (REVEAL_READ_MEMORY_END < memory_idx) ||
                                ((REVEAL_READ_MEMORY_END - memory_idx) < memory_size) ||
                                (memory_size == 0))
                            {
                                reason_code = NVO_RS_UNSUP_INVALID_PARAM;
                            }
                            else
                            {
                                new_msg = Reveal__NewFeedback(0x1,0x4, pt->source);
                                if (new_msg != NULL)
                                {
                                    for (count=0;count<memory_size;count++)
                                    {
                                        Reveal__AddDataU8(new_msg,*((REVEAL_MEM_POINTER_CAST uint8 *)memory_idx));
                                        memory_idx++;
                                    }
                                    Reveal__QueueCommand(new_msg);
                                }
                                reason_code = NVO_RS_READY;
                            }
                            break;
                    #else

                        case API001_READ_MEM_32B:
                            if ((pt->payload[6] > 0) ||
                                (pt->payload[7] > REVEAL_PAYLOAD_SIZE))
                            {
                                memory_size = REVEAL_PAYLOAD_SIZE;
                            }
                            else
                            {
                                memory_size = pt->payload[7];
                            }
                            memory_idx = ( ((uint32)pt->payload[2])<<24 );
                            memory_idx +=( ((uint32)pt->payload[3])<<16 );
                            memory_idx +=( ((uint32)pt->payload[4])<<8 );
                            memory_idx +=( ((uint32)pt->payload[5]) );
                            if (
                                #if (REVEAL_READ_MEMORY_START > 0)
                                    (memory_idx < (uint32)REVEAL_READ_MEMORY_START) ||
                                #endif
                                #if (REVEAL_READ_MEMORY_END < 0xFFFFFFFF)
                                    (REVEAL_READ_MEMORY_END < memory_idx) ||
                                #endif
                                ((REVEAL_READ_MEMORY_END - memory_idx + 1) < memory_size) ||
                                (memory_size == 0))
                            {
                                reason_code = NVO_RS_UNSUP_INVALID_PARAM;
                            }
                            else
                            {
                                new_msg = Reveal__NewFeedback(0x1,0x4, pt->source,memory_size);
                                if (new_msg != NULL)
                                {
                                    for (count=0;count<memory_size;count++)
                                    {
                                        Reveal__AddDataU8(new_msg,*((REVEAL_MEM_POINTER_CAST uint8 *)memory_idx));  //lint !e923 Suppress Info: cast from unsigned long to pointer [Encompasses MISRA 2004 Rule 11.1], [MISRA 2004 Rule 11.3]
                                        memory_idx++;
                                    }
                                    Reveal__QueueCommand(new_msg);
                                }
                                reason_code = NVO_RS_READY;
                            }
                            break;
                        case API001_MAX_MESSAGE_SIZE:
                            new_msg = Reveal__NewFeedback(0x1,API001_MAX_MESSAGE_SIZE, REVEAL_BROADCAST_NODE,2);
                            if (new_msg != NULL)
                            {
                                Reveal__AddDataU8(new_msg,REVEAL_PAYLOAD_SIZE);
                                Reveal__AddDataU8(new_msg,REVEAL_PAYLOAD_INCOMING_SIZE);

                                Reveal__QueueCommand(new_msg);
                                reason_code = NVO_RS_READY;
                            }
                            else
                            {
                                reason_code = NVO_RS_BUSY;
                            }

                            break;
                        case API001_BLOCK_MESSAGES:
                            //......................................................
                            // payload[2] = time to block the api(s) and opcode(s) in seconds
                            // payload[3] = api
                            // payload[4] = opcode bitmap byte (MSB)
                            // payload[5] = opcode bitmap byte
                            // payload[6] = opcode bitmap byte
                            // payload[7] = opcode bitmap byte (LSB)
                            // payload[8] = api
                            // payload[9] = opcode bitmap byte (MSB)
                            // payload[10] = .............
                            // pattern repeats as much or as little as required
                            //......................................................
                            // if a time of 0 is specified, un-block all apis and opcodes ELSE
                            // if first api is 0xFF and opcode is 0xFFFFFFFF, block all apis and opcodes
                            //......................................................


                            reason_code = NVO_RS_UNSUP_INVALID_PARAM;

                            if (pt->sdu_length > 2)        // An empty payload is invalid
                            {
                                if (pt->payload[2] == 0)
                                {
                                    // Clear all blocked api/opcodes
                                    // No need to look at any other bytes in the payload
                                    memset(Reveal_Blocked_Messages, 0, sizeof(Reveal_Blocked_Messages));

                                    Reveal_Blocked_Messages_Remaining_Time = 0;
                                    reason_code = NVO_RS_READY;
                                }
                                else if ((pt->sdu_length > BLOCKED_API_OPCODE_BYTE_OFFSET_START) &&                                         // A payload with a non-zero blocking time but no api and opcodes to block is invalid
                                         (((pt->sdu_length - BLOCKED_API_OPCODE_BYTE_OFFSET_START) % BLOCKED_API_OPCODE_NUM_BYTES) == 0))   // A payload with the wrong number of bytes for the api and opcodes is invalid
                                {
                                    // Check if the request is to block all APIs and Opcodes
                                    if ((pt->payload[3] == 0xFF) &&
                                        (pt->payload[4] == 0xFF) &&
                                        (pt->payload[5] == 0xFF) &&
                                        (pt->payload[6] == 0xFF) &&
                                        (pt->payload[7] == 0xFF))
                                    {
                                        // Block all api/opcodes (except API 1)
                                        memset(Reveal_Blocked_Messages, 0xFF, sizeof(Reveal_Blocked_Messages));

                                        reason_code = NVO_RS_READY;
                                    }
                                    else if (RequestedAPIsCanBeBlocked(&(pt->payload[BLOCKED_API_OPCODE_BYTE_OFFSET_START]), (pt->sdu_length - BLOCKED_API_OPCODE_BYTE_OFFSET_START)) == TRUE) // Specific api(s) and opcode(s) requested to be blocked
                                    {
                                        reason_code = NVO_RS_READY;

                                        // Clear blocked api/opcodes
                                        memset(Reveal_Blocked_Messages, 0, sizeof(Reveal_Blocked_Messages));

                                        // Set the opcodes that should be blocked for each API/Opcode pair in the payload
                                        // Starting at index BLOCKED_API_OPCODE_BYTE_OFFSET_START to skip over content at beginning of the payload
                                        // Incrementing by BLOCKED_API_OPCODE_NUM_BYTES to skip over api/opcode pairs which are BLOCKED_API_OPCODE_NUM_BYTES bytes each
                                        for (count = BLOCKED_API_OPCODE_BYTE_OFFSET_START; count < pt->sdu_length; count+=BLOCKED_API_OPCODE_NUM_BYTES)
                                        {
                                            // Walk the POSSIBLE_BLOCKED_APIS_LIST table to find the index that corresponds to Reveal_Blocked_Messages array
                                            for (index = 0; index < ELEMENTS_IN_ARRAY(POSSIBLE_BLOCKED_APIS_LIST); index++)
                                            {
                                                if (pt->payload[count] == POSSIBLE_BLOCKED_APIS_LIST[index])
                                                {
                                                    opcode_bitmap  = (uint32)pt->payload[count + 1] << 24;
                                                    opcode_bitmap += (uint32)pt->payload[count + 2] << 16;
                                                    opcode_bitmap += (uint32)pt->payload[count + 3] << 8;
                                                    opcode_bitmap += (uint32)pt->payload[count + 4];

                                                    Reveal_Blocked_Messages[index] |= opcode_bitmap;
                                                    break;
                                                }
                                                //else, did not match API in POSSIBLE_BLOCKED_APIS_LIST, need to check next one
                                            }
                                        }
                                    }

                                    if (reason_code == NVO_RS_READY)
                                    {
                                        // Only set the blocking time if the message was successfully processed
                                        Reveal_Blocked_Messages_Remaining_Time = pt->payload[2];

                                        // Convert time from seconds to 5ms increments based on call rate of Reveal__Handler()
                                        Reveal_Blocked_Messages_Remaining_Time *= REVEAL_BLOCKED_MESSAGES_TIME_TO_TICKS;
                                    }
                                }
                            }

                            break;

                    #endif
                #endif
                    default:
                        reason_code = NVO_RS_UNSUP_OP_CODE;
                        break;
                }
            }
        }
        else
        {
            if (pt->sdu_length <= (REVEAL_PAYLOAD_INCOMING_SIZE + REVEAL_API_OP_SIZE))
            {
                Reveal_Adapter_Buffer.destination = pt->destination;
                Reveal_Adapter_Buffer.node_id = pt->source;
                Reveal_Adapter_Buffer.sdu_length = pt->sdu_length;
                Reveal_Adapter_Buffer.sap = pt->sap;
                memcpy(&Reveal_Adapter_Buffer.api_id,pt->payload,pt->sdu_length + REVEAL_LLC_CRC_SIZE);

                reason_code = RevealHandleParser(&Reveal_Adapter_Buffer,feedback,FALSE,FALSE);
            }
            else
            {
                //reason_code
            }
        }

        if ((feedback == 0) &&  // If this is a command, it prepares the ack message
            (reason_code != NVO_RS_DONT_SEND_ACK))
        {
            // Command
            Reveal_Aux = (uint8)(Reveal_Ack_Write + 1);
            if (Reveal_Aux >= REVEAL_ACK_FIFO)
            {
                Reveal_Aux = 0;
            }

            if (Reveal_Aux != Reveal_Ack_Read)
            {
                Reveal_Ack_Buffer[Reveal_Ack_Write].node = pt->source;
                Reveal_Ack_Buffer[Reveal_Ack_Write].api = api;
                Reveal_Ack_Buffer[Reveal_Ack_Write].op_code = op_code;
                Reveal_Ack_Buffer[Reveal_Ack_Write].reason_code = reason_code;
                Reveal_Ack_Write = Reveal_Aux;
            }
            else
            {
                REVEAL_EXCEPTION_ACK_QUEUE_FULL(pt->source,api,op_code,reason_code);
            }

            REVEAL__ACK_INCOMING_MESSAGE(pt->source,api,op_code,reason_code);
        }
        FREE_MESSAGER(REVEAL_SAP);
    }
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method handles the outgoing messages
 * @details This method could be called every 25ms or every 5ms depending on the system demand.
 */
static void RevealHandlerOutgoing(void)
{
#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    LLC_QUEUED_TYPE queue_retval;
    uint8 count;
    uint8 api;
    PT_TX_TYPE * pt;
    REVEAL_COMMAND_TYPE * ptr;
#endif


#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
    // Handle Outgoing message
    if (Reveal_Ack_Write != Reveal_Ack_Read)
    {
        // process acknowledgment
        pt = ALLOCATE_BUFFER(Reveal_Ack_Buffer[Reveal_Ack_Read].node);
        if (pt != (PT_TX_TYPE *)NULL)
        {
            // Transfer the package from Reveal to LLC in here
            pt->destination = Reveal_Ack_Buffer[Reveal_Ack_Read].node;
            pt->sap = REVEAL_SAP;
            pt->sdu_length = 5;
            pt->payload[0] = REVEAL_API_CORE;
            pt->payload[1] = 0x21;
            pt->payload[2] = Reveal_Ack_Buffer[Reveal_Ack_Read].reason_code;
            pt->payload[3] = Reveal_Ack_Buffer[Reveal_Ack_Read].api;
            pt->payload[4] = Reveal_Ack_Buffer[Reveal_Ack_Read].op_code;
            QUEUE_MESSAGER(Reveal_Ack_Buffer[Reveal_Ack_Read].node);
            Reveal_Ack_Read++;
            if (Reveal_Ack_Read >= REVEAL_ACK_FIFO)
            {
                Reveal_Ack_Read= 0;
            }
        }
    }
    else
    {
        switch (Reveal_Outgoing_State)
        {
            case REVEAL_OUTGOING_STATE_IDLE:

                #if (REVEAL_QUEUE_PRIORITY == ENABLED)

                    if (Reveal_Queue_Priority_Write != Reveal_Queue_Priority_Read)
                    {
                        Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING_PRIORITY;
                        Reveal_Queue_Retry = REVEAL_RETRY_TIMES;
                        //No Break Falls through
                    }
                    else
                    {
                        if (Reveal_Queue_Write != Reveal_Queue_Read)
                        {
                            Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING;
                            Reveal_Queue_Retry = REVEAL_RETRY_TIMES;
                            //No Break Falls through
                        }
                        else
                        {
                            break;
                        }
                    }
                #else
                    if (Reveal_Queue_Write != Reveal_Queue_Read)
                    {
                        Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING;
                        Reveal_Queue_Retry = REVEAL_RETRY_TIMES;
                        //No Break Falls through
                    }
                    else
                    {
                        break;
                    }
                #endif
                //lint -fallthrough
            case REVEAL_OUTGOING_STATE_SENDING:
        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
            case REVEAL_OUTGOING_STATE_SENDING_PRIORITY:
        #endif
                // Transfer the package from Reveal to LLC protocol in here
                #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                    if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_SENDING)
                    {
                        ptr = &Reveal_Queue[Reveal_Queue_Read];
                    }
                    else
                    {
                        ptr = &Reveal_Queue_Priority[Reveal_Queue_Priority_Read];
                    }
                #else
                    ptr = &Reveal_Queue[Reveal_Queue_Read];
                #endif

                Reveal_Drop_flag = FALSE;
                #if (REVEAL_SYNCH_PROCESS == ENABLED)
                    if ((ptr->node < REVEAL_BROADCAST_NODE)
                        &&
                        (BIT_TEST(Reveal_Node_Status, ptr->node) == 0  ))
                    {
                        // force the message to be dropped
                        Reveal_Queue_Timeout = 0;
                        Reveal_Queue_Retry = 1;

                        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                            if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_SENDING)
                            {
                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING;
                            }
                            else
                            {
                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING_PRIORITY;
                            }
                        #else
                            Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING;
                        #endif

                        Reveal_Drop_flag = TRUE;
                    }
                    else
                #endif
                    {

                        pt = ALLOCATE_BUFFER(ptr->node);
                        if (pt != (PT_TX_TYPE *)NULL)
                        {
                            pt->destination = ptr->node;
                            pt->sap = REVEAL_SAP;
                            pt->payload[0] = ptr->api;
                            pt->payload[1] = ptr->opcode;
                            if (ptr->feedback == 1)
                            {
                                pt->payload[1] |= 0x20;
                            }
                            else
                            {
                                pt->payload[1] &= 0xDF;
                            }
                            memcpy(&(pt->payload[2]),ptr->data,ptr->length);
                            pt->sdu_length = (uint8)(ptr->length + 2);
                            if (ptr->feedback == 0)
                            {
                                // Command
                                Reveal_Queue_Timeout = (REVEAL_TIMEOUT * (ptr->timeout + 1));
                                #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                                    if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_SENDING)
                                    {
                                        Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING;
                                    }
                                    else
                                    {
                                        Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING_PRIORITY;
                                    }
                                #else
                                    Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_TIMING;
                                #endif
                            }
                            else
                            {
                                // Feedback
                                #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                                    if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_SENDING)
                                    {
                                        DropMessageFromQueue(FALSE);
                                    }
                                    else
                                    {
                                        DropMessageFromQueue(TRUE);
                                    }
                                #else
                                    DropMessageFromQueue(FALSE);
                                #endif

                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                            }
                            queue_retval = QUEUE_MESSAGER(ptr->node);
                            // The following code prevents the Reveal to trying to send a external message to himself
                            if ((queue_retval == LLC_INVALID_ADDRESS) ||
                                (queue_retval == LLC_INVALID_SIZE))
                            {
                                #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                                    if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                                    {
                                        DropMessageFromQueue(FALSE);
                                    }
                                    else
                                    {
                                        DropMessageFromQueue(TRUE);
                                    }
                                #else
                                    DropMessageFromQueue(FALSE);
                                #endif
                                 Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                            }
                        }
                    }
                break;
            case REVEAL_OUTGOING_STATE_TIMING:
        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
            case REVEAL_OUTGOING_STATE_TIMING_PRIORITY:
        #endif
                if (Reveal_Queue_Timeout)
                {
                    Reveal_Queue_Timeout--;
                }
                else
                {
                    Reveal_Queue_Retry--;
                    if (Reveal_Queue_Retry)
                    {
                        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                            if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                            {
                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING;
                            }
                            else
                            {
                                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING_PRIORITY;
                            }
                        #else
                            Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_SENDING;
                        #endif
                    }
                    else
                    {
                        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                            if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                            {
                                api = Reveal_Queue[Reveal_Queue_Read].api;
                            }
                            else
                            {
                                api = Reveal_Queue_Priority[Reveal_Queue_Priority_Read].api;
                            }
                        #else
                            api = Reveal_Queue[Reveal_Queue_Read].api;
                        #endif


                        for (count=0;count< REVEAL_NUM_PARSERS;count++)
                        {
                           if ((REVEAL_PARSER[count].API == api) ||    // Specific API parser or
                               (REVEAL_PARSER[count].API == 0))        // Generic parser (API = 0) which should receive all APIs
                           {
                               if (REVEAL_PARSER[count].ACK_PARSER != NULL)
                               {
                                    #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                                        if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                                        {
                                            REVEAL_PARSER[count].ACK_PARSER(Reveal_Queue[Reveal_Queue_Read].node,
                                                                            Reveal_Queue[Reveal_Queue_Read].api,
                                                                            Reveal_Queue[Reveal_Queue_Read].opcode,
                                                                            NVO_RS_TIMEOUT);

                                        }
                                        else
                                        {
                                            REVEAL_PARSER[count].ACK_PARSER(Reveal_Queue_Priority[Reveal_Queue_Priority_Read].node,
                                                                            Reveal_Queue_Priority[Reveal_Queue_Priority_Read].api,
                                                                            Reveal_Queue_Priority[Reveal_Queue_Priority_Read].opcode,
                                                                            NVO_RS_TIMEOUT);

                                        }
                                    #else
                                        REVEAL_PARSER[count].ACK_PARSER(Reveal_Queue[Reveal_Queue_Read].node,
                                                                        Reveal_Queue[Reveal_Queue_Read].api,
                                                                        Reveal_Queue[Reveal_Queue_Read].opcode,
                                                                        NVO_RS_TIMEOUT);

                                    #endif
                               }
                               if (REVEAL_PARSER[count].API != 0)      // If the generic parser is found it keeps searching for the specific parsers, otherwise it breaks and stop searching
                               {
                                   break;
                               }
                           }
                        }


                        // If there is a message trying to be sent and it fail all retries it will set the node to fail state
                        if ((Reveal_Queue[Reveal_Queue_Read].node < 15) &&
                            (Reveal_Drop_flag == FALSE))
                        {
                            BIT_CLR(Reveal_Node_Status,Reveal_Queue[Reveal_Queue_Read].node);
                        }


                        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                            if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                            {
                                REVEAL_DROPPED_MESSAGE_EXCEPTION(Reveal_Queue[Reveal_Queue_Read].api,
                                                                Reveal_Queue[Reveal_Queue_Read].opcode,
                                                                Reveal_Queue[Reveal_Queue_Read].node);
                            }
                            else
                            {
                                REVEAL_DROPPED_MESSAGE_EXCEPTION(Reveal_Queue_Priority[Reveal_Queue_Priority_Read].api,
                                                                 Reveal_Queue_Priority[Reveal_Queue_Priority_Read].opcode,
                                                                 Reveal_Queue_Priority[Reveal_Queue_Priority_Read].node);
                            }
                        #else

                            REVEAL_DROPPED_MESSAGE_EXCEPTION(Reveal_Queue[Reveal_Queue_Read].api,
                                                            Reveal_Queue[Reveal_Queue_Read].opcode,
                                                            Reveal_Queue[Reveal_Queue_Read].node);

                        #endif



                        #if (REVEAL_QUEUE_PRIORITY == ENABLED)
                            if (Reveal_Outgoing_State == REVEAL_OUTGOING_STATE_TIMING)
                            {
                                DropMessageFromQueue(FALSE);
                            }
                            else
                            {
                                DropMessageFromQueue(TRUE);
                            }
                        #else

                                DropMessageFromQueue(FALSE);

                        #endif
                        Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                    }
                }
                break;
            default:
                Reveal_Outgoing_State = REVEAL_OUTGOING_STATE_IDLE;
                break;
        }
    }
#endif
}

#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_INTERNAL_ONLY)
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Drop the message in the head of the queue.
 * @details This function is used when a message is acknowledged or it's a bad message and it must be dropped.
 * @param priority
 */
static void DropMessageFromQueue(uint8 priority)
{
    #if (REVEAL_QUEUE_PRIORITY == ENABLED)
        if (priority == TRUE)
        {
            if (Reveal_Queue_Priority_Read != Reveal_Queue_Priority_Write)
            {
                Reveal_Queue_Priority_Read++;
                if (Reveal_Queue_Priority_Read >= REVEAL_QUEUE_PRIORITY_SIZE)
                {
                    Reveal_Queue_Priority_Read = 0;
                }
            }
        }
        else
        {
            if (Reveal_Queue_Read != Reveal_Queue_Write)
            {
                Reveal_Queue_Read++;
                if (Reveal_Queue_Read >= REVEAL_QUEUE_SIZE)
                {
                    Reveal_Queue_Read = 0;
                }
            }
        }
    #else

        if (Reveal_Queue_Read != Reveal_Queue_Write)
        {
            Reveal_Queue_Read++;
            if (Reveal_Queue_Read >= REVEAL_QUEUE_SIZE)
            {
                Reveal_Queue_Read = 0;
            }
        }
    #endif
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief   Method to determine if the requested APIs from the Block Messages feature can be blocked. Result is based on
 *          the list of possible APIs that can be blocked in the POSSIBLE_BLOCKED_APIS_LIST table.
 * @param   pt: pointer to the list of APIs and opcodes requested to be blocked
 * @param   number_of_bytes: number of bytes in the buffer pointed to by the "pt" argument
 * @return  TRUE if all the requested APIs can be blocked, else FALSE.
 */
static BOOL_TYPE RequestedAPIsCanBeBlocked(uint8 * pt, uint8 number_of_bytes)
{
    uint8 index;
    BOOL_TYPE apis_can_be_blocked = TRUE;

    // Verify all the requested APIs are in POSSIBLE_BLOCKED_APIS_LIST
    for (uint8 count = 0; count < number_of_bytes; count+=BLOCKED_API_OPCODE_NUM_BYTES)   // Incrementing by BLOCKED_API_OPCODE_NUM_BYTES to skip over api/opcode pairs which are BLOCKED_API_OPCODE_NUM_BYTES bytes each
    {
        // Walk the POSSIBLE_BLOCKED_APIS_LIST table to see if the requested API can be blocked
        for (index = 0; index < ELEMENTS_IN_ARRAY(POSSIBLE_BLOCKED_APIS_LIST); index++)
        {
            if (pt[count] == POSSIBLE_BLOCKED_APIS_LIST[index])
            {
                // Found the API is POSSIBLE_BLOCKED_APIS_LIST
                break;
            }
            //else, did not match API in POSSIBLE_BLOCKED_APIS_LIST, need to check next one
        }

        // Check if the requested API was not found in the POSSIBLE_BLOCKED_APIS_LIST table
        if (index == ELEMENTS_IN_ARRAY(POSSIBLE_BLOCKED_APIS_LIST))
        {
            // API was not in the POSSIBLE_BLOCKED_APIS_LIST table and cannot be blocked
            apis_can_be_blocked = FALSE;
            break;
        }
    }

    return(apis_can_be_blocked);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method handles a incoming Ack message.
 * @param    node - the node id to send the message to
 * @param    api - the api id of the command
 * @param    opcode - the opcode of the command
 * @param reason
 * @param full_seach
 */
static void RevealHandleAckParser(uint8 node, uint8 api, uint8 op_code, REVEAL_RECIPE_STATUS_TYPE reason, uint8 full_search)
{
    uint8 count;
    for (count=0;count< REVEAL_NUM_PARSERS;count++)
    {
       if ((REVEAL_PARSER[count].API == api) ||    // Specific API parser or
           (REVEAL_PARSER[count].API == 0))        // Generic parser (API = 0) which should receive all APIs
       {
           if (REVEAL_PARSER[count].ACK_PARSER != NULL) // If the parser for the feedback is present
           {
               REVEAL_PARSER[count].ACK_PARSER(node,api, op_code,reason);
           }

           if (full_search == FALSE)
           {
               if (REVEAL_PARSER[count].API != 0)      // If the generic parser is found it keeps searching for the specific parsers, otherwise it breaks and stop searching
               {
                   break;
               }
           }
       }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief This method handles the incoming messages such as Commands and Feedbacks
 * @param pt
 * @param feedback
 * @param address_checking
 * @return The return depends how the specific parser will parse the message. It has to be with have the parser, api and opcode and be able to handle the parameters as they come.
 */
static REVEAL_RECIPE_STATUS_TYPE RevealHandleParser(REVEAL_MSG_TYPE * pt,uint8 feedback, uint8 address_checking, uint8 full_search)
{
    REVEAL_RECIPE_STATUS_TYPE retval;
    uint8 count;
    retval = NVO_RS_UNSUP_UNAVAILABLE;

    for (count=0;count< REVEAL_NUM_PARSERS;count++)
    {
        if ((REVEAL_PARSER[count].API == pt->api_id) ||    // Specific API parser or
            (REVEAL_PARSER[count].API == 0))        // Generic parser (API = 0) which should receive all APIs
        {
#if (REVEAL_ROUTER_FEATURE != REVEAL_ROUTER_DISABLED)
            if ((address_checking == FALSE) ||
                (REVEAL_PARSER[count].ADDRESS == pt->destination) ||
                (pt->destination == REVEAL_BROADCAST_NODE))
#endif
            {
                pt->size = (uint8)(pt->sdu_length-2);
                if (feedback == TRUE)                   // If it's a Feedback
                {
                    retval = NVO_RS_READY;
                    if (REVEAL_PARSER[count].FEEDBACK_PARSER != NULL) // If the parser for the feedback is present
                    {
                        REVEAL_PARSER[count].FEEDBACK_PARSER(pt);
                    }
                }
                else
                {
                                                        // If it's a command
                    if (REVEAL_PARSER[count].COMMAND_PARSER != NULL)    // If the parser for the command is present
                    {
                        retval = REVEAL_PARSER[count].COMMAND_PARSER(pt);
                    }
                }
                if (full_search == FALSE)
                {
                    if (REVEAL_PARSER[count].API != 0)      // If the generic parser is found it keeps searching for the specific parsers, otherwise it breaks and stop searching
                    {
                        break;
                    }
                }
            }
        }
    }
    return (retval);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Local 1s counter for heart-beat
 */
static void Reveal1sTasks(void)
{
    REVEAL_COMMAND_TYPE * ptr;
    if (Reveal_Heartbeat_Timer > 0)
    {
        Reveal_Heartbeat_Counter++;
        if (Reveal_Heartbeat_Counter >= Reveal_Heartbeat_Timer)
        {
            Reveal_Heartbeat_Counter = 0;
            ptr = Reveal__NewFeedback(0x1,0x22, 0xF,0);
            if (ptr != NULL)
            {
                Reveal__QueueCommand(ptr);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Controls the timer used to block api/opcode messages and clears all the blocked messages when the timer
 *              expires
 */
static void UpdateBlockedMessagesTimer(void)
{
    if (Reveal_Blocked_Messages_Remaining_Time > 0)
    {
        // Timer is still running, update the remaining time
        Reveal_Blocked_Messages_Remaining_Time--;

        if (Reveal_Blocked_Messages_Remaining_Time == 0)
        {
            // Timer has expired, clear all blocked api/opcodes
            memset(Reveal_Blocked_Messages, 0, sizeof(Reveal_Blocked_Messages));
        }
    }
}
