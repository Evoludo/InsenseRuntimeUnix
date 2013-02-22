

/*
 * Impl Source file for the Insense radio
 *
 * Paul Harvey (modified by Jon Lewis)
 *
 */

//#include "net/mac/xmac.h"
#include "InsenseRadio.h"
#include "RadioPacket.h"
//#include "events.h"
#include <stdio.h>

#define DEBUG 1

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#define CHAN_PRINT(...) channel_print(__VA_ARGS__)
#else
#define PRINTF(...)
#define CHAN_PRINT(...)
#endif

insense_radio_pntr radio_glob;


/*----------------------------------------------------------------------------*/
void insense_radio_constructor(void *radio, int argc, void *argv[]) {

	insense_radio_pntr this = (insense_radio_pntr) radio;
	this->broadcastSend_comp = NULL;//channel_create(CHAN_IN, sizeof(AnyTypePNTR), true);
	this->unicastSend_comp = NULL;//channel_create(CHAN_IN, sizeof(RadioPacket_PNTR), true);
	this->received_comp  = NULL;//channel_create(CHAN_OUT, sizeof(RadioPacket_PNTR), true);

	this->fromXmac = channel_create(CHAN_IN, sizeof(RadioPacketSP_PNTR), true);
	this->toXmac = channel_create(CHAN_OUT, sizeof(RadioPacketSP_PNTR), true);

	// TODO JL Check ASYNCH  appears to be error here, same direction for fromXmac and xmac_get_recieve_packet_chan()
	int toX = NULL;//channel_bind(this->fromXmac, xmac_component_glob->receive_chan);
	int fromX = NULL;//channel_bind(this->toXmac, xmac_component_glob->send_chan);
	PRINTF("xbind from %d\n", toX);
	PRINTF("xbind to   %d\n", fromX);

	radio_glob = this;
	//}
	/*----------------------------------------------------------------------------*/
	//void insense_radio_behaviour(void *radio) {
	//insense_radio_pntr this = (insense_radio_pntr) radio;
	unsigned char buffer[sizeof(void*)];
	unsigned int counter = 0;
	struct select_struct selection;
	chan_id chans[3] = {this->broadcastSend_comp, this->unicastSend_comp, this->fromXmac};


	AnyTypePNTR out_any = NULL;
	RadioPacket_PNTR out_packet = NULL;

	//ptr = temp;
	selection.buffer = buffer;
	selection.chans  = chans;
	selection.nchans = 3;
	selection.have_default = false;


	while(1)
	{
		//        disable_interrupts();
		//        enable_preemption();
		//        enable_interrupts();

		//get a request to send something
		int choice = channel_select(&selection);


		//        disable_interrupts();
		//        disable_preemption();
		//        enable_interrupts();

		PRINTF("Radio: got data on channel %i\n", choice);

		if( choice == this->broadcastSend_comp ) { // broadcast

			PRINTF("****************************************                                                            %d\n", counter++);

			PRINTF("Radio: BCAST\n");

			//out_any is a GC pointer
			DAL_assign(&out_any, *((void **) selection.buffer) );

			PRINTF("Radio: pointer buffer from channel\n");
			int size = 0;

			//new payload with a GC pointer
			void *serial_payload = serialiseAnyType(out_any, &size);


			//this returns a bare metal malloced pointer
			PRINTF("Radio: Serialised output packet\n");
			RadioPacketSP_PNTR packet_sp = construct_RadioPacketSP(0, serial_payload, size);



			// try and send via the mac layer
			// TODO JL Check ASYNCH what to do when send fails here?
			int res;

			while( (res = channel_send(this->toXmac, &packet_sp, NULL )) == ChannelBufferFullException ){
				// TODO JL Check ASYNCH  yield component temporarily when send fails?
				PRINTF("Radio: Fail to send, yield and retry\n");
				component_yield();

			}

			PRINTF("Radio: Free out_any\n");

			/*need to manually free what was passed from the in channel
			 *
			 * For the Time being we do this here (incremented twice in the app)
			 * but this will (apparently) be done in the receiving component
			 */
			DAL_decRef(out_any);
			PRINTF("Radio: done\n");
			PRINTF("****************************************");

		}
		if( choice == this->unicastSend_comp ) { // unicast

			PRINTF("Radio: UNICAST\n");
			DAL_assign(&out_packet,  *((void **) selection.buffer) );
			int size = 0;
			void *serial_payload = serialiseAnyType(out_packet->payload, &size);
			RadioPacketSP_PNTR packet_sp = construct_RadioPacketSP(out_packet->addr, serial_payload, size);
			// try and send via the mac layer
			// TODO JL Check ASYNCH what to do when send fails here?
			int res;
			while( (res = channel_send(this->toXmac, &packet_sp, NULL )) == ChannelBufferFullException ){
				// TODO JL Check ASYNCH  yield component temporarily when send fails?
				component_yield();
			}

			/*need to manually free what was passed from the in channel
			 *
			 * For the Time being we do this here (incremented twice in the app)
			 * but this will (apparently) be done in the receiving component
			 */

			DAL_decRef(out_packet);

			PRINTF("Radio: done\n");
		}
		if(choice == this->fromXmac ) { //got a radio packet from xmac layer

			RadioPacket_PNTR in_packet = NULL;

			//should disable preemption if we want to free radio packet buffer
			PRINTF("Radio: got packet from xmac\n");

			RadioPacketSP_PNTR packet_sp = *((void **) selection.buffer);
			AnyTypePNTR in_any_payload = deserialiseToAnyType(packet_sp->payload);


			if(in_any_payload == NULL) {
				PRINTF("Radio : can't make temp payload \n");
				continue;
			}
			/*
            PRINTF("%s              disable_interrupts();
            enable_preemption();
            enable_interrupts();: %d\n",in_any_payload->type, in_any_payload->value);
			 */

			RadioPacket_PNTR p = construct_RadioPacket(packet_sp->addr, in_any_payload);


			DAL_assign(&in_packet, (void*)p );


			// manually free of memory for serial packet as not needed anymore
			/*
            free(packet_sp->payload);

            free(packet_sp);
			 */

			PRINTF("Radio: from %d.%d (%d)\n", ((in_packet->addr) & 0xff), ((in_packet->addr) >> 8), in_packet->addr);
#if DEBUG
			void **var_pntr = (void **) &in_packet;
			void *old_mem = *var_pntr; // can deref void** but not void*
			MemHeader header = (MemHeader) (old_mem - sizeof(MemHeaderStruct));
			PRINTF("DAL_decRef: ref count = %d\n", header->ref_count);
#endif
			// TODO JL Check ASYNCH  don't need special asynch code here as we don't care if anyone is listening on the other end or not
			int res = channel_multicast_send(this->received_comp, &in_packet );
			//  disable_interrupts();
			int i = 0;
			for( ; i < res - 1; i++) {
				DAL_incRef(in_packet);
				//PRINTF("Increment channel packet\n");
			}
			//   enable_interrupts();
#if DEBUG
			var_pntr = (void **) &in_packet;
			old_mem = *var_pntr; // can deref void** but not void*
			header = (MemHeader) (old_mem - sizeof(MemHeaderStruct));
			PRINTF("DAL_decRef: ref count = %d\n", header->ref_count);
#endif
			PRINTF("Radio: incoming packet sent to %d channels, incremented %d times\n", res, i);
		}
	}
}


