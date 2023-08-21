#include "IRTransmit.hpp"

void IRTransmit::send(int64SendTypes protocol, uint64_t data) {
    switch(protocol){
        case int64SendTypes::NEC: return sendNEC(data);
        case int64SendTypes::Sherwood: return sendSherwood(data);
        case int64SendTypes::LG: return sendLG(data);
        case int64SendTypes::LG2: return sendLG2(data);
        case int64SendTypes::JVC: return sendJVC(data);
        case int64SendTypes::Denon: return sendDenon(data);
        case int64SendTypes::DISH: return sendDISH(data);
        case int64SendTypes::RCMM: return sendRCMM(data);
        case int64SendTypes::Mitsubishi: return sendMitsubishi(data);
        case int64SendTypes::Mitsubishi2: return sendMitsubishi2(data);
        case int64SendTypes::AiwaRCT501: return sendAiwaRCT501(data);
        case int64SendTypes::Nikai: return sendNikai(data);
        case int64SendTypes::Midea: return sendMidea(data);
        case int64SendTypes::Lasertag: return sendLasertag(data);
        case int64SendTypes::CarrierAC: return sendCarrierAC(data);
        case int64SendTypes::CarrierAC40: return sendCarrierAC40(data);
        case int64SendTypes::CarrierAC64: return sendCarrierAC64(data);
        case int64SendTypes::GICable: return sendGICable(data);
        case int64SendTypes::Lutron: return sendLutron(data);
        case int64SendTypes::Epson: return sendEpson(data);
        case int64SendTypes::Symphony: return sendSymphony(data);
        case int64SendTypes::Airwell: return sendAirwell(data);
        case int64SendTypes::DelonghiAc: return sendDelonghiAc(data);
        case int64SendTypes::TechnibelAc: return sendTechnibelAc(data);        
    }
};
void IRTransmit::send(constInt64SendTypes protocol, const uint64_t data) {
    switch(protocol){
        case constInt64SendTypes::Sony: return sendSony(data);
        case constInt64SendTypes::Sony38: return sendSony38(data);
        case constInt64SendTypes::SAMSUNG: return sendSAMSUNG(data);
        case constInt64SendTypes::Samsung36: return sendSamsung36(data);
        case constInt64SendTypes::SharpRaw: return sendSharpRaw(data);
        case constInt64SendTypes::SanyoLC7461: return sendSanyoLC7461(data);
        case constInt64SendTypes::Panasonic64: return sendPanasonic64(data);
        case constInt64SendTypes::RC5: return sendRC5(data);
        case constInt64SendTypes::RC6: return sendRC6(data);
        case constInt64SendTypes::COOLIX: return sendCOOLIX(data);
        case constInt64SendTypes::Coolix48: return sendCoolix48(data);
        case constInt64SendTypes::Whynter: return sendWhynter(data);
        case constInt64SendTypes::Inax: return sendInax(data);
        case constInt64SendTypes::Daikin64: return sendDaikin64(data);
        case constInt64SendTypes::Gree: return sendGree(data);
        case constInt64SendTypes::Goodweather: return sendGoodweather(data);
        case constInt64SendTypes::Gorenje: return sendGorenje(data);
        case constInt64SendTypes::Midea24: return sendMidea24(data);
        case constInt64SendTypes::MagiQuest: return sendMagiQuest(data);
        case constInt64SendTypes::PanasonicAC32: return sendPanasonicAC32(data);
        case constInt64SendTypes::Pioneer: return sendPioneer(data);
        case constInt64SendTypes::VestelAc: return sendVestelAc(data);
        case constInt64SendTypes::Teco: return sendTeco(data);
        case constInt64SendTypes::LegoPf: return sendLegoPf(data);
        case constInt64SendTypes::Doshisha: return sendDoshisha(data);
        case constInt64SendTypes::Multibrackets: return sendMultibrackets(data);
        case constInt64SendTypes::Zepeal: return sendZepeal(data);
        case constInt64SendTypes::Metz: return sendMetz(data);
        case constInt64SendTypes::Transcold: return sendTranscold(data);
        case constInt64SendTypes::Elitescreens: return sendElitescreens(data);
        case constInt64SendTypes::Milestag2: return sendMilestag2(data);
        case constInt64SendTypes::Ecoclim: return sendEcoclim(data);
        case constInt64SendTypes::Xmp: return sendXmp(data);
        case constInt64SendTypes::Truma: return sendTruma(data);
        case constInt64SendTypes::Kelon: return sendKelon(data);
        case constInt64SendTypes::Bose: return sendBose(data);
        case constInt64SendTypes::Arris: return sendArris(data);
        case constInt64SendTypes::Airton: return sendAirton(data);
        case constInt64SendTypes::Toto: return sendToto(data);
        case constInt64SendTypes::ClimaButler: return sendClimaButler(data);
        case constInt64SendTypes::Wowwee: return sendWowwee(data);
    }
};
void IRTransmit::send(charArrSendType protocol, const unsigned char data[]) {
    switch(protocol){
        case charArrSendType::SamsungAC: return sendSamsungAC(data);
        case charArrSendType::SharpAc: return sendSharpAc(data);
        case charArrSendType::Mirage: return sendMirage(data);
        case charArrSendType::Mitsubishi136: return sendMitsubishi136(data);
        case charArrSendType::Mitsubishi112: return sendMitsubishi112(data);
        case charArrSendType::MitsubishiAC: return sendMitsubishiAC(data);
        //case charArrSendType::FujitsuAC: return sendFujitsuAC(data); add other args check lib version
        case charArrSendType::Kelvinator: return sendKelvinator(data);
        case charArrSendType::Daikin: return sendDaikin(data);
        case charArrSendType::Daikin128: return sendDaikin128(data);
        case charArrSendType::Daikin152: return sendDaikin152(data);
        case charArrSendType::Daikin160: return sendDaikin160(data);
        case charArrSendType::Daikin176: return sendDaikin176(data);
        case charArrSendType::Daikin2: return sendDaikin2(data);
        case charArrSendType::Daikin200: return sendDaikin200(data);
        case charArrSendType::Daikin216: return sendDaikin216(data);
        case charArrSendType::Daikin312: return sendDaikin312(data);
        case charArrSendType::Argo: return sendArgo(data);
        case charArrSendType::ArgoWREM3: return sendArgoWREM3(data);
        case charArrSendType::Trotec: return sendTrotec(data);
        case charArrSendType::Trotec3550: return sendTrotec3550(data);
        case charArrSendType::HaierAC: return sendHaierAC(data);
        case charArrSendType::HaierACYRW02: return sendHaierACYRW02(data);
        case charArrSendType::HaierAC160: return sendHaierAC160(data);
        case charArrSendType::HaierAC176: return sendHaierAC176(data);
        case charArrSendType::HitachiAC: return sendHitachiAC(data);
        case charArrSendType::HitachiAC1: return sendHitachiAC1(data);
        case charArrSendType::HitachiAC2: return sendHitachiAC2(data);
        // case charArrSendType::HitachiAc3: return sendHitachiAc3(data); Add other args
        case charArrSendType::HitachiAc264: return sendHitachiAc264(data);
        case charArrSendType::HitachiAc296: return sendHitachiAc296(data);
        case charArrSendType::HitachiAc344: return sendHitachiAc344(data);
        case charArrSendType::HitachiAc424: return sendHitachiAc424(data);
        case charArrSendType::WhirlpoolAC: return sendWhirlpoolAC(data);
        case charArrSendType::ElectraAC: return sendElectraAC(data);
        case charArrSendType::PanasonicAC: return sendPanasonicAC(data);
        //case charArrSendType::MWM: return sendMWM(data); TODO find good way to add other args
        case charArrSendType::Tcl96Ac: return sendTcl96Ac(data);
        case charArrSendType::Tcl112Ac: return sendTcl112Ac(data);
        case charArrSendType::Neoclima: return sendNeoclima(data);
        case charArrSendType::Amcor: return sendAmcor(data);
        case charArrSendType::Voltas: return sendVoltas(data);
        case charArrSendType::Teknopoint: return sendTeknopoint(data);
        case charArrSendType::Kelon168: return sendKelon168(data);
        case charArrSendType::Rhoss: return sendRhoss(data);
        case charArrSendType::Bosch144: return sendBosch144(data);
        case charArrSendType::York: return sendYork(data);
    }
};