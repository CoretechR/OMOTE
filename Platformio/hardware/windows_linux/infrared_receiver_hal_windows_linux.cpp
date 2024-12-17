#include "infrared_receiver_hal_windows_linux.h"

#include <map>
#include <string>

#include <flirc/flirc.h>
#include <ir/ir.h>

#include "util.h"

bool irReceiverEnabled = false;

std::map<enum rc_proto, std::string> protoToString = {
  {RC_PROTO_UNKNOWN, "UNKNOWN"},
  {RC_PROTO_INVALID, "INVALID"},
  {RC_PROTO_OTHER, "OTHER"},
  {RC_PROTO_RC5, "RC5"},
  {RC_PROTO_RC5X_20, "RC5X_20"},
  {RC_PROTO_RC5_SZ, "RC5_SZ"},
  {RC_PROTO_JVC, "JVC"},
  {RC_PROTO_SONY12, "SONY12"},
  {RC_PROTO_SONY15, "SONY15"},
  {RC_PROTO_SONY20, "SONY20"},
  {RC_PROTO_NEC, "NEC"},
  {RC_PROTO_NECX, "NECX"},
  {RC_PROTO_NEC32, "NEC32"},
  {RC_PROTO_NEC48, "NEC48"},
  {RC_PROTO_SANYO, "SANYO"},
  {RC_PROTO_MCIR2_KBD, "MCIR2_KBD"},
  {RC_PROTO_MCIR2_MSE, "MCIR2_MSE"},
  {RC_PROTO_RC6_0, "RC6_0"},
  {RC_PROTO_RC6_6A_20, "RC6_6A_20"},
  {RC_PROTO_RC6_6A_24, "RC6_6A_24"},
  {RC_PROTO_RC6_6A_32, "RC6_6A_32"},
  {RC_PROTO_RC6_MCE, "RC6_MCE"},
  {RC_PROTO_SHARP, "SHARP"},
  {RC_PROTO_XMP, "XMP"},
  {RC_PROTO_CEC, "CEC"},
  {RC_PROTO_IMON, "IMON"},
  {RC_PROTO_RCMM12, "RCMM12"},
  {RC_PROTO_RCMM24, "RCMM24"},
  {RC_PROTO_RCMM32, "RCMM32"},
  {RC_PROTO_ORTEK, "ORTEK"},
  {RC_PROTO_DENON, "DENON"},
  {RC_PROTO_DENONK, "DENONK"},
  {RC_PROTO_FLIRC, "FLIRC"},
  {RC_PROTO_NOKIA12, "NOKIA12"},
  {RC_PROTO_NOKIA24, "NOKIA24"},
  {RC_PROTO_NOKIA32, "NOKIA32"},
  {RC_PROTO_TDC, "TDC"},
  {RC_PROTO_AMINO, "AMINO"},
  {RC_PROTO_NEC_APPLE, "NEC_APPLE"},
  {RC_PROTO_NEC_REPEAT, "NEC_REPEAT"},
  {RC_PROTO_PANASONIC, "PANASONIC"},
  {RC_PROTO_GAP, "GAP"},
};

void start_infraredReceiver_HAL() {
  fl_dev_flush();
}

void shutdown_infraredReceiver_HAL() {
}

tAnnounceNewIRmessage_cb thisAnnounceNewIRmessage_cb = NULL;
void set_announceNewIRmessage_cb_HAL(tAnnounceNewIRmessage_cb pAnnounceNewIRmessage_cb) {
  thisAnnounceNewIRmessage_cb = pAnnounceNewIRmessage_cb;  
}

struct ir_prot d;
struct ir_packet p;

void infraredReceiver_loop_HAL() {
  switch (fl_ir_packet_poll(&p)) {
    case (1):
    {
      printf("----------------\n");
      ir_decode_packet(&p, &d);

      printf("0x%08X - %s : %d : hash: 0x%08X\n",
          d.scancode, d.desc, d.protocol, d.hash);
      printf("----------------\n");
      fflush(stdout);

      fl_dev_flush();   

      char code[11];
      unsigned long reverse = reverseBits(d.scancode);
      std::string typeString = protoToString[d.protocol];
      snprintf(code, sizeof(code), "0x%08lX", reverse);

      if (thisAnnounceNewIRmessage_cb != NULL) {
        thisAnnounceNewIRmessage_cb(typeString + " " + code);
      }
    }
		break;
	case (0):
		break;
	case (-1):
		printf("flirc: error, disconnecting\n");
		break;
	default:
		printf("flirc: UNKNOWN ERROR\n");
		break;
	}
}

bool get_irReceiverEnabled_HAL() {
  return irReceiverEnabled;
}
void set_irReceiverEnabled_HAL(bool aIrReceiverEnabled) {
  irReceiverEnabled = aIrReceiverEnabled;
}






