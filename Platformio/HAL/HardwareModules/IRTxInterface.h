#pragma once
#include "Notification.hpp"
#include <stdint.h>

class IRInterface {
    public:
    // uint64_t data, uint16_t nbits = <defaulted>, uint16_t repeat = <defaulted>
    enum class int64SendTypes{
        NEC,Sherwood,LG,LG2,JVC,Denon,DISH,RCMM,Mitsubishi,
        Mitsubishi2,AiwaRCT501,Nikai,Midea,Lasertag,CarrierAC,
        CarrierAC40,CarrierAC64,GICable,Lutron,Epson,
        Symphony,Airwell,DelonghiAc,TechnibelAc
    };

    // const uint64_t data, uint16_t nbits = <defaulted>, uint16_t repeat = <defaulted>
    enum class constInt64SendTypes{
        Sony,Sony38,SAMSUNG,Samsung36,SharpRaw,SanyoLC7461,Panasonic64,
        RC5,RC6,COOLIX,Coolix48,Whynter,Inax,Daikin64,
        Gree,Goodweather,Gorenje,Midea24,MagiQuest,PanasonicAC32,
        Pioneer,VestelAc,Teco,LegoPf,Doshisha,Multibrackets,Zepeal,
        Metz,Transcold,Elitescreens,Milestag2,Ecoclim,Xmp,
        Truma,Kelon,Bose,Arris,Airton,Toto,ClimaButler,Wowwee
    };

    // const unsigned char data[], const uint16_t nbytes = <defaulted>, const uint16_t repeat = <defaulted>)
    enum class charArrSendType{
        SamsungAC,SharpAc,Mirage,Mitsubishi136,Mitsubishi112,MitsubishiAC,FujitsuAC,
        Kelvinator,Daikin,Daikin128,Daikin152,Daikin160,Daikin176,Daikin2,
        Daikin200,Daikin216,Daikin312,Argo,ArgoWREM3,Trotec,Trotec3550,HaierAC,
        HaierACYRW02,HaierAC160,HaierAC176,HitachiAC,HitachiAC1,HitachiAC2,HitachiAc3,
        HitachiAc264,HitachiAc296,HitachiAc344,HitachiAc424,WhirlpoolAC,ElectraAC,PanasonicAC,MWM,
        Tcl96Ac,Tcl112Ac,Neoclima,Amcor,Voltas,Teknopoint,Kelon168,Rhoss,Bosch144,York
    };

    virtual void send(int64SendTypes protocol, uint64_t data) = 0;
    virtual void send(constInt64SendTypes protocol, const uint64_t data) = 0;
    virtual void send(charArrSendType protocol, const unsigned char data[]) = 0;

};