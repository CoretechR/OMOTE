#pragma once
#include "Notification.hpp"
#include <stdint.h>

class IRInterface {
public:
  enum class protocol {
    UNKNOWN = -1,
    UNUSED = 0,
    RC5,
    RC6,
    NEC,
    SONY,
    PANASONIC, // (5)
    JVC,
    SAMSUNG,
    WHYNTER,
    AIWA_RC_T501,
    LG, // (10)
    SANYO,
    MITSUBISHI,
    DISH,
    SHARP,
    COOLIX, // (15)
    DAIKIN,
    DENON,
    KELVINATOR,
    SHERWOOD,
    MITSUBISHI_AC, // (20)
    RCMM,
    SANYO_LC7461,
    RC5X,
    GREE,
    PRONTO, // Technically not a protocol, but an encoding. (25)
    NEC_LIKE,
    ARGO,
    TROTEC,
    NIKAI,
    RAW,         // Technically not a protocol, but an encoding. (30)
    GLOBALCACHE, // Technically not a protocol, but an encoding.
    TOSHIBA_AC,
    FUJITSU_AC,
    MIDEA,
    MAGIQUEST, // (35)
    LASERTAG,
    CARRIER_AC,
    HAIER_AC,
    MITSUBISHI2,
    HITACHI_AC, // (40)
    HITACHI_AC1,
    HITACHI_AC2,
    GICABLE,
    HAIER_AC_YRW02,
    WHIRLPOOL_AC, // (45)
    SAMSUNG_AC,
    LUTRON,
    ELECTRA_AC,
    PANASONIC_AC,
    PIONEER, // (50)
    LG2,
    MWM,
    DAIKIN2,
    VESTEL_AC,
    TECO, // (55)
    SAMSUNG36,
    TCL112AC,
    LEGOPF,
    MITSUBISHI_HEAVY_88,
    MITSUBISHI_HEAVY_152, // 60
    DAIKIN216,
    SHARP_AC,
    GOODWEATHER,
    INAX,
    DAIKIN160, // 65
    NEOCLIMA,
    DAIKIN176,
    DAIKIN128,
    AMCOR,
    DAIKIN152, // 70
    MITSUBISHI136,
    MITSUBISHI112,
    HITACHI_AC424,
    SONY_38K,
    EPSON, // 75
    SYMPHONY,
    HITACHI_AC3,
    DAIKIN64,
    AIRWELL,
    DELONGHI_AC, // 80
    DOSHISHA,
    MULTIBRACKETS,
    CARRIER_AC40,
    CARRIER_AC64,
    HITACHI_AC344, // 85
    CORONA_AC,
    MIDEA24,
    ZEPEAL,
    SANYO_AC,
    VOLTAS, // 90
    METZ,
    TRANSCOLD,
    TECHNIBEL_AC,
    MIRAGE,
    ELITESCREENS, // 95
    PANASONIC_AC32,
    MILESTAG2,
    ECOCLIM,
    XMP,
    TRUMA, // 100
    HAIER_AC176,
    TEKNOPOINT,
    KELON,
    TROTEC_3550,
    SANYO_AC88, // 105
    BOSE,
    ARRIS,
    RHOSS,
    AIRTON,
    COOLIX48, // 110
    HITACHI_AC264,
    KELON168,
    HITACHI_AC296,
    DAIKIN200,
    HAIER_AC160, // 115
    CARRIER_AC128,
    TOTO,
    CLIMABUTLER,
    TCL96AC,
    BOSCH144, // 120
    SANYO_AC152,
    DAIKIN312,
    GORENJE,
    WOWWEE,
    CARRIER_AC84, // 125
    YORK
  };

  // uint64_t data, uint16_t nbits = <defaulted>, uint16_t repeat = <defaulted>
  enum class int64SendTypes {
    NEC = static_cast<int>(protocol::NEC),
    Sherwood = static_cast<int>(protocol::SHERWOOD),
    LG = static_cast<int>(protocol::LG),
    LG2 = static_cast<int>(protocol::LG2),
    JVC = static_cast<int>(protocol::JVC),
    Denon = static_cast<int>(protocol::DENON),
    DISH = static_cast<int>(protocol::DISH),
    RCMM = static_cast<int>(protocol::RCMM),
    Mitsubishi = static_cast<int>(protocol::MITSUBISHI),
    Mitsubishi2 = static_cast<int>(protocol::MITSUBISHI2),
    AiwaRCT501 = static_cast<int>(protocol::AIWA_RC_T501),
    Nikai = static_cast<int>(protocol::NIKAI),
    Midea = static_cast<int>(protocol::MIDEA),
    Lasertag = static_cast<int>(protocol::LASERTAG),
    CarrierAC = static_cast<int>(protocol::CARRIER_AC),
    CarrierAC40 = static_cast<int>(protocol::CARRIER_AC40),
    CarrierAC64 = static_cast<int>(protocol::CARRIER_AC64),
    GICable = static_cast<int>(protocol::GICABLE),
    Lutron = static_cast<int>(protocol::LUTRON),
    Epson = static_cast<int>(protocol::EPSON),
    Symphony = static_cast<int>(protocol::SYMPHONY),
    Airwell = static_cast<int>(protocol::AIRWELL),
    DelonghiAc = static_cast<int>(protocol::DELONGHI_AC),
    TechnibelAc = static_cast<int>(protocol::TECHNIBEL_AC)
  };

  // const uint64_t data, uint16_t nbits = <defaulted>, uint16_t repeat =
  // <defaulted>
  enum class constInt64SendTypes {
    Sony = static_cast<int>(protocol::SONY),
    Sony38 = static_cast<int>(protocol::SONY_38K),
    SAMSUNG = static_cast<int>(protocol::SAMSUNG),
    Samsung36 = static_cast<int>(protocol::SAMSUNG36),
    SharpRaw = static_cast<int>(protocol::SHARP),
    SanyoLC7461 = static_cast<int>(protocol::SANYO_LC7461),
    Panasonic64 = static_cast<int>(protocol::PANASONIC),
    RC5 = static_cast<int>(protocol::RC5),
    RC6 = static_cast<int>(protocol::RC6),
    COOLIX = static_cast<int>(protocol::COOLIX),
    Coolix48 = static_cast<int>(protocol::COOLIX48),
    Whynter = static_cast<int>(protocol::WHYNTER),
    Inax = static_cast<int>(protocol::INAX),
    Daikin64 = static_cast<int>(protocol::DAIKIN64),
    Gree = static_cast<int>(protocol::GREE),
    Goodweather = static_cast<int>(protocol::GOODWEATHER),
    Gorenje = static_cast<int>(protocol::GORENJE),
    Midea24 = static_cast<int>(protocol::MIDEA24),
    MagiQuest = static_cast<int>(protocol::MAGIQUEST),
    PanasonicAC32 = static_cast<int>(protocol::PANASONIC_AC32),
    Pioneer = static_cast<int>(protocol::PIONEER),
    VestelAc = static_cast<int>(protocol::VESTEL_AC),
    Teco = static_cast<int>(protocol::TECO),
    LegoPf = static_cast<int>(protocol::LEGOPF),
    Doshisha = static_cast<int>(protocol::DOSHISHA),
    Multibrackets = static_cast<int>(protocol::MULTIBRACKETS),
    Zepeal = static_cast<int>(protocol::ZEPEAL),
    Metz = static_cast<int>(protocol::METZ),
    Transcold = static_cast<int>(protocol::TRANSCOLD),
    Elitescreens = static_cast<int>(protocol::ELITESCREENS),
    Milestag2 = static_cast<int>(protocol::MILESTAG2),
    Ecoclim = static_cast<int>(protocol::ECOCLIM),
    Xmp = static_cast<int>(protocol::XMP),
    Truma = static_cast<int>(protocol::TRUMA),
    Kelon = static_cast<int>(protocol::KELON),
    Bose = static_cast<int>(protocol::BOSE),
    Arris = static_cast<int>(protocol::ARRIS),
    Airton = static_cast<int>(protocol::AIRTON),
    Toto = static_cast<int>(protocol::TOTO),
    ClimaButler = static_cast<int>(protocol::CLIMABUTLER),
    Wowwee = static_cast<int>(protocol::WOWWEE)
  };

  // const unsigned char data[], const uint16_t nbytes = <defaulted>, const
  // uint16_t repeat = <defaulted>)
  enum class charArrSendType {
    SamsungAC = static_cast<int>(protocol::SAMSUNG_AC),
    SharpAc = static_cast<int>(protocol::SHARP_AC),
    Mirage = static_cast<int>(protocol::MIRAGE),
    Mitsubishi136 = static_cast<int>(protocol::MITSUBISHI136),
    Mitsubishi112 = static_cast<int>(protocol::MITSUBISHI112),
    MitsubishiAC = static_cast<int>(protocol::MITSUBISHI_AC),
    FujitsuAC = static_cast<int>(protocol::FUJITSU_AC),
    Kelvinator = static_cast<int>(protocol::KELVINATOR),
    Daikin = static_cast<int>(protocol::DAIKIN),
    Daikin128 = static_cast<int>(protocol::DAIKIN128),
    Daikin152 = static_cast<int>(protocol::DAIKIN152),
    Daikin160 = static_cast<int>(protocol::DAIKIN160),
    Daikin176 = static_cast<int>(protocol::DAIKIN176),
    Daikin2 = static_cast<int>(protocol::DAIKIN2),
    Daikin200 = static_cast<int>(protocol::DAIKIN200),
    Daikin216 = static_cast<int>(protocol::DAIKIN216),
    Daikin312 = static_cast<int>(protocol::DAIKIN312),
    Argo = static_cast<int>(protocol::ARGO),
    Trotec = static_cast<int>(protocol::TROTEC),
    Trotec3550 = static_cast<int>(protocol::TROTEC_3550),
    HaierAC = static_cast<int>(protocol::HAIER_AC),
    HaierACYRW02 = static_cast<int>(protocol::HAIER_AC_YRW02),
    HaierAC160 = static_cast<int>(protocol::HAIER_AC160),
    HaierAC176 = static_cast<int>(protocol::HAIER_AC176),
    HitachiAC = static_cast<int>(protocol::HITACHI_AC),
    HitachiAC1 = static_cast<int>(protocol::HITACHI_AC1),
    HitachiAC2 = static_cast<int>(protocol::HITACHI_AC2),
    HitachiAc3 = static_cast<int>(protocol::HITACHI_AC3),
    HitachiAc264 = static_cast<int>(protocol::HITACHI_AC264),
    HitachiAc296 = static_cast<int>(protocol::HITACHI_AC296),
    HitachiAc344 = static_cast<int>(protocol::HITACHI_AC344),
    HitachiAc424 = static_cast<int>(protocol::HITACHI_AC424),
    WhirlpoolAC = static_cast<int>(protocol::WHIRLPOOL_AC),
    ElectraAC = static_cast<int>(protocol::ELECTRA_AC),
    PanasonicAC = static_cast<int>(protocol::PANASONIC_AC),
    MWM = static_cast<int>(protocol::MWM),
    Tcl96Ac = static_cast<int>(protocol::TCL96AC),
    Tcl112Ac = static_cast<int>(protocol::TCL112AC),
    Neoclima = static_cast<int>(protocol::NEOCLIMA),
    Amcor = static_cast<int>(protocol::AMCOR),
    Voltas = static_cast<int>(protocol::VOLTAS),
    Teknopoint = static_cast<int>(protocol::TEKNOPOINT),
    Kelon168 = static_cast<int>(protocol::KELON),
    Rhoss = static_cast<int>(protocol::RHOSS),
    Bosch144 = static_cast<int>(protocol::BOSCH144),
    York = static_cast<int>(protocol::YORK)
  };

  struct RawIR {
    protocol mprotocol;
    std::vector<uint16_t> data;
  };

  virtual void send(int64SendTypes protocol, uint64_t data) = 0;
  virtual void send(constInt64SendTypes protocol, const uint64_t data) = 0;
  virtual void send(charArrSendType protocol, const unsigned char data[]) = 0;
  virtual void send(RawIR aRawIr) = 0;

  virtual int8_t calibrateTx() = 0;

  virtual void enableRx() = 0;
  virtual void disableRx() = 0;
  virtual void loopHandleRx() = 0;

  std::shared_ptr<Notification<RawIR>> IRRecievedNotification() {
    return mIRReceived;
  };

protected:
  std::shared_ptr<Notification<RawIR>> mIRReceived =
      std::make_shared<Notification<RawIR>>();
};