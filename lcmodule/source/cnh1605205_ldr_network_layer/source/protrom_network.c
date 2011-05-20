/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/
/**
 *  @addtogroup ldr_communication_serv
 *  @{
 *    @addtogroup protrom_family
 *    @{
 *      @addtogroup ldr_network_layer
 *      @{
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "c_system.h"
#include "t_basicdefinitions.h"
#include "r_protrom_family.h"
#include "r_protrom_transport.h"
#include "r_protrom_network.h"
#include "r_protrom_protocol.h"
#include "r_protrom_header.h"
#include "r_communication_service.h"
#include "r_debug.h"
#include "r_debug_macro.h"

#ifdef  WIN32
#include <windows.h>
#endif

/*******************************************************************************
 * Declaration of file local functions
 ******************************************************************************/
static ErrorCode_e Protrom_Network_ReceiveHeader(Communication_t *Communication_p);
static ErrorCode_e Protrom_Network_ReceivePayload(Communication_t *Communication_p);
static ErrorCode_e Protrom_Network_TransmiterHandler(Communication_t *Communication_p);
static void Protrom_InHashCallback(void *Data_p, uint32 Length, const uint8 *const Hash_p, void *Param_p);
static void Protrom_QueueCallback(const void *const Queue_p, void *Param_p);

/*******************************************************************************
 * File scope types, constants and variables
 ******************************************************************************/
#define PROTROM_RESET_INBOUND(c, s) do { (c)->ReqData = 0; (c)->RecData = 0; (c)->ReqBuffOffset = 0; (c)->Target_p = NULL; (c)->State = (s); } while(0);
#define PROTROM_SYNC_HEADER(c, d, t) do { (c)->ReqData = (d); (c)->Target_p = (t); } while(0);
#define PROTROM_SET_INBOUND(c, s, d) do { (c)->ReqData = (d); (c)->RecData = 0; (c)->ReqBuffOffset = 0; (c)->State = (s); } while(0);

/*******************************************************************************
 * Definition of external functions
 ******************************************************************************/

/*
 * Initializes the PROTOROM network layer.
 *
 * @param [in]  Communication_p Communication module context.
 *
 * @retval  E_SUCCESS                   After successful execution.
 * @retval  E_FAILED_TO_INIT_COM_DEVICE Failed to initialize the communication
 *                                      device.
 */
ErrorCode_e Protrom_Network_Initialize(Communication_t *Communication_p)
{
    memset(PROTROM_NETWORK(Communication_p), 0, sizeof(Protrom_NetworkContext_t));

    /* Simulate a finished read to get the inbound state-machine going. */
    Protrom_Network_ReadCallback(NULL, 0, Communication_p);
    (void)QUEUE(Communication_p, Fifo_SetCallback_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Outbound_p, QUEUE_NONEMPTY, Protrom_QueueCallback, Communication_p);

    return E_SUCCESS;
}

/*
 * Shutdown the PROTROM network layer.
 *
 * @param [in]  Communication_p Communication module context.
 *
 * @retval  E_SUCCESS After successful execution.
 */
ErrorCode_e Protrom_Network_Shutdown(const Communication_t *const Communication_p)
{
    Protrom_Inbound_t *In_p = &(PROTROM_NETWORK(Communication_p)->Inbound);

    if (NULL != In_p->Packet_p) {
        if (NULL != In_p->Packet_p->Buffer_p) {
            free(In_p->Packet_p->Buffer_p);
        }

        free(In_p->Packet_p);
        In_p->Packet_p = NULL;
    }

    (void)QUEUE(Communication_p, Fifo_SetCallback_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Outbound_p, QUEUE_NONEMPTY, NULL, NULL);
    (void)QUEUE(Communication_p, Fifo_SetCallback_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Outbound_p, QUEUE_EMPTY, NULL, NULL);

    (void)QUEUE(Communication_p, Fifo_SetCallback_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Inbound_p, QUEUE_NONEMPTY, NULL, NULL);
    (void)QUEUE(Communication_p, Fifo_SetCallback_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Inbound_p, QUEUE_EMPTY, NULL, NULL);

    return E_SUCCESS;
}

/*
 * This callback function handles the received packets.
 *
 * @param [in] Data_p  Pointer to the received data.
 * @param [in] Length  Length of the received data.
 * @param [in] Param_p Extra parameteres.
 *
 * @return none.
 */
void Protrom_Network_ReadCallback(const void *Data_p, const uint32 Length, void *Param_p)
{
    Communication_t *Communication_p = (Communication_t *)Param_p;

    A_(printf("protrom_family.c (%d) RecLength(%d) RecBackupData (%d)\n", __LINE__, Length, PROTROM_NETWORK(Communication_p)->Inbound.RecBackupData);)
    PROTROM_NETWORK(Communication_p)->Inbound.RecData = Length + PROTROM_NETWORK(Communication_p)->Inbound.RecBackupData;
    PROTROM_NETWORK(Communication_p)->Inbound.RecBackupData = 0;

    if (PROTROM_NETWORK(Communication_p)->Inbound.ReqData == 0) {
        ErrorCode_e ReturnValue = E_GENERAL_COMMUNICATION_ERROR;

        if (PROTROM_NETWORK(Communication_p)->Inbound.StopTransfer) {
            if (PROTROM_NETWORK(Communication_p)->Inbound.PacketsBeforeReceiverStop){
                PROTROM_NETWORK(Communication_p)->Inbound.PacketsBeforeReceiverStop--;
            }
            else {
                PROTROM_NETWORK(Communication_p)->Inbound.State = PROTROM_RECEIVE_IDLE;
            }
        }

        switch (PROTROM_NETWORK(Communication_p)->Inbound.State) {

        case PROTROM_RECEIVE_IDLE:
            ReturnValue = E_SUCCESS;
            break;
        
        case PROTROM_RECEIVE_HEADER:
            ReturnValue = Protrom_Network_ReceiveHeader(Communication_p);
            break;

        case PROTROM_RECEIVE_PAYLOAD:
            ReturnValue = Protrom_Network_ReceivePayload(Communication_p);
            break;

        default:
            PROTROM_NETWORK(Communication_p)->Inbound.State = PROTROM_RECEIVE_HEADER;
            PROTROM_NETWORK(Communication_p)->Inbound.RecData = 0;
            PROTROM_NETWORK(Communication_p)->Inbound.ReqData = PROTROM_HEADER_LENGTH;
            PROTROM_NETWORK(Communication_p)->Inbound.Target_p = PROTROM_NETWORK(Communication_p)->Inbound.Scratch;
            return;
        }

        if (E_SUCCESS != ReturnValue) {
            PROTROM_NETWORK(Communication_p)->Inbound.State = PROTROM_RECEIVE_ERROR;
            return;
        }
    }
}

/*
 * This function checks if new data has been received.
 *
 * @param [in]  Communication_p Communication module context.
 *
 * @return  none.
 */
void Protrom_Network_ReceiverHandler(Communication_t *Communication_p)
{
    uint32 ReqData;
    Protrom_Inbound_t *In_p = &(PROTROM_NETWORK(Communication_p)->Inbound);

    /* new data for receiving ? */
    if (In_p->ReqData > 0) {
        if (Communication_p->BackupCommBufferSize != 0) {
            if (Communication_p->BackupCommBufferSize < In_p->ReqData) {
                memcpy(In_p->Target_p + In_p->ReqBuffOffset, Communication_p->BackupCommBuffer_p, Communication_p->BackupCommBufferSize);
                In_p->RecBackupData = Communication_p->BackupCommBufferSize;
                (void)Communication_p->CommunicationDevice_p->Read(In_p->Target_p + In_p->ReqBuffOffset + Communication_p->BackupCommBufferSize,
                        In_p->ReqData - Communication_p->BackupCommBufferSize,
                        Protrom_Network_ReadCallback, Communication_p);
                C_(printf("protrom_network.c (%d) ReqData(%d) RecData(%d)\n", __LINE__, In_p->ReqData, In_p->RecData);)
                C_(printf("protrom_network.c (%d) Communication_p->BackupCommBufferSize(%d) RecBackupData (%d)\n", __LINE__, Communication_p->BackupCommBufferSize, In_p->RecBackupData);)
                In_p->RecData = 0;
                In_p->ReqBuffOffset = 0;
                Communication_p->BackupCommBufferSize = 0;
                In_p->ReqData = 0;
            } else {
                memcpy(In_p->Target_p + In_p->ReqBuffOffset, Communication_p->BackupCommBuffer_p, In_p->ReqData);
                Communication_p->BackupCommBufferSize = Communication_p->BackupCommBufferSize - In_p->ReqData;
                ReqData = In_p->ReqData;
                In_p->ReqData = 0;
                Protrom_Network_ReadCallback(In_p->Target_p + In_p->ReqBuffOffset, ReqData, Communication_p);
                In_p->RecData = 0;
            }
        } else {
            ReqData = In_p->ReqData;
            In_p->ReqData = 0;
            In_p->RecData = 0;
            (void)Communication_p->CommunicationDevice_p->Read(In_p->Target_p + In_p->ReqBuffOffset, ReqData, Protrom_Network_ReadCallback, Communication_p);
            C_(printf("protrom_network.c (%d) ReqData(%d) RecData(%d) \n", __LINE__, In_p->ReqData, In_p->RecData);)
            C_(printf("protrom_network.c (%d) Communication_p->BackupCommBufferSize(%d) RecBackupData (%d)\n", __LINE__, Communication_p->BackupCommBufferSize, In_p->RecBackupData);)
            In_p->ReqBuffOffset = 0;
        }
    }

    /* check for receiver sinhronization */
    if (In_p->State == PROTROM_RECEIVE_ERROR) {
        In_p->ReqData = 0;
        In_p->RecData = 0;
        In_p->ReqBuffOffset = 0;
        (void)Communication_p->CommunicationDevice_p->Read(In_p->Target_p, PROTROM_HEADER_LENGTH, Protrom_Network_ReadCallback, Communication_p);
        In_p->State = PROTROM_RECEIVE_HEADER;
    }
}

/*
 * Handler function that is called after successful transmission
 * of a packet.
 *
 * If new packet is ready for transmitting it starts
 * the transmission of the packet.
 *
 * @param [in] Data_p  Pointer to the data for transmitting.
 * @param [in] Length  Length of the received data.
 * @param [in] Param_p Extra parameters.
 *
 * @return none.
 */
void Protrom_Network_WriteCallback(const void *Data_p, const uint32 Length, void *Param_p)
{
    Communication_t *Communication_p = (Communication_t *)Param_p;
    Protrom_Outbound_t *Out_p = &(PROTROM_NETWORK(Communication_p)->Outbound);

    if (NULL != Out_p->Packet_p) {
        if (NULL != Out_p->Packet_p->Buffer_p) {
            free(Out_p->Packet_p->Buffer_p);
        }

        free(Out_p->Packet_p);
        Out_p->Packet_p = NULL;
    }

    Out_p->State = PROTROM_SEND_IDLE;

    /* check for more stuff to send. */
    (void)Protrom_Network_TransmiterHandler(Communication_p);
}


/*******************************************************************************
 * Definition of internal functions
 ******************************************************************************/

static ErrorCode_e Protrom_Network_ReceiveHeader(Communication_t *Communication_p)
{
    Protrom_Inbound_t *In_p = &(PROTROM_NETWORK(Communication_p)->Inbound);

    if (In_p->RecData == 0) {
        In_p->ReqData = PROTROM_HEADER_LENGTH;
        In_p->Target_p = In_p->Scratch;
        In_p->ReqBuffOffset = 0;
    } else {
        if (Protrom_IsReceivedHeader(In_p)) {
            if (Protrom_IsValidHeader(In_p->Scratch)) {
                In_p->Packet_p = (Protrom_Packet_t *)malloc(sizeof(Protrom_Packet_t));

                if (NULL == In_p->Packet_p) {
                    return E_ALLOCATE_FAILED;
                }

                In_p->Packet_p->Communication_p = Communication_p;
                Protrom_DeserializeHeader(&In_p->Packet_p->Header, In_p->Scratch);
                In_p->Packet_p->Buffer_p = (uint8 *)malloc(In_p->Packet_p->Header.PayloadLength + PROTROM_HEADER_LENGTH + PROTROM_CRC_LENGTH);

                if (NULL == In_p->Packet_p->Buffer_p) {
                    return E_ALLOCATE_FAILED;
                }

                In_p->Target_p = In_p->Packet_p->Buffer_p;
                memcpy(In_p->Target_p, In_p->Scratch, PROTROM_HEADER_LENGTH);
                In_p->Target_p += PROTROM_HEADER_LENGTH;
                PROTROM_SET_INBOUND(In_p, PROTROM_RECEIVE_PAYLOAD, In_p->Packet_p->Header.PayloadLength + PROTROM_CRC_LENGTH);
            } else {
                /* Sync the header. */
                PROTROM_RESET_INBOUND(In_p, PROTROM_RECEIVE_HEADER);
                PROTROM_SYNC_HEADER(In_p, PROTROM_HEADER_LENGTH, In_p->Scratch);
            }
        }
    }

    return E_SUCCESS;
}


static ErrorCode_e Protrom_Network_ReceivePayload(Communication_t *Communication_p)
{
    Protrom_Inbound_t *In_p = &(PROTROM_NETWORK(Communication_p)->Inbound);
    Protrom_Packet_t *Packet_p = In_p->Packet_p;

    Packet_p->Communication_p = Communication_p;
    Communication_p->HashDevice_p->Calculate(OBJECT_HASH(Communication_p),
            Communication_p->CurrentFamilyHash, Packet_p->Buffer_p,
            Packet_p->Header.PayloadLength + PROTROM_HEADER_LENGTH,
            (uint8 *)&Packet_p->CRC, (HashCallback_t)Protrom_InHashCallback,
            (void *)Packet_p);

    In_p->Packet_p = NULL;
    PROTROM_RESET_INBOUND(In_p, PROTROM_RECEIVE_HEADER);
    PROTROM_SYNC_HEADER(In_p, PROTROM_HEADER_LENGTH, In_p->Scratch);
    return E_SUCCESS;
}


static ErrorCode_e Protrom_Network_TransmiterHandler(Communication_t *Communication_p)
{
    Protrom_Outbound_t *Out_p = &(PROTROM_NETWORK(Communication_p)->Outbound);

    if (Out_p->InLoad) {
        return E_SUCCESS;
    }

    Out_p->InLoad = TRUE;

    switch (Out_p->State) {
    case PROTROM_SEND_IDLE:
        Out_p->Packet_p = (Protrom_Packet_t *)QUEUE(Communication_p, FifoDequeue_Fn)(OBJECT_QUEUE(Communication_p), Communication_p->Outbound_p);

        if (NULL != Out_p->Packet_p) {
            /* get next packet for transmiting */
            Out_p->State = PROTROM_SEND_PACKET;
        } else {
            break;
        }

        /* FALLTHROUGH */

    case PROTROM_SEND_PACKET:
        Out_p->State = PROTROM_SENDING_PACKET;

        if (E_SUCCESS != Communication_p->CommunicationDevice_p->Write(Out_p->Packet_p->Buffer_p,
                PROTROM_HEADER_LENGTH + Out_p->Packet_p->Header.PayloadLength + PROTROM_CRC_LENGTH,
                Protrom_Network_WriteCallback, Communication_p)) {
            Out_p->State = PROTROM_SEND_PACKET;
            break;
        }

        /* FALLTHROUGH */

    case PROTROM_SENDING_PACKET:
        break;

    }

    Out_p->InLoad = FALSE;

    return E_SUCCESS;
}

static void Protrom_QueueCallback(const void *const Queue_p, void *Param_p)
{
    (void)Protrom_Network_TransmiterHandler((Communication_t *)Param_p);
}

static void Protrom_InHashCallback(void *Data_p, uint32 Length, const uint8 *const Hash_p, void *Param_p)
{
    Protrom_Packet_t *Packet_p = (Protrom_Packet_t *)Param_p;

    if (memcmp(Hash_p, Packet_p->Buffer_p + PROTROM_HEADER_LENGTH + Packet_p->Header.PayloadLength, sizeof(uint16)) == 0) {
        (void)QUEUE(Packet_p->Communication_p, FifoEnqueue_Fn)(OBJECT_QUEUE(Packet_p->Communication_p), Packet_p->Communication_p->Inbound_p, Packet_p);
    } else {
        /* Invalid packet */
        free(Packet_p->Buffer_p);
        free(Packet_p);
    }
}

/** @} */
/** @} */
/** @} */
