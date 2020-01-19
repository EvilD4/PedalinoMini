/*
__________           .___      .__  .__                 _____  .__       .__     ___ ________________    ___    
\______   \ ____   __| _/____  |  | |__| ____   ____   /     \ |__| ____ |__|   /  / \__    ___/     \   \  \   
 |     ___// __ \ / __ |\__  \ |  | |  |/    \ /  _ \ /  \ /  \|  |/    \|  |  /  /    |    | /  \ /  \   \  \  
 |    |   \  ___// /_/ | / __ \|  |_|  |   |  (  <_> )    Y    \  |   |  \  | (  (     |    |/    Y    \   )  ) 
 |____|    \___  >____ |(____  /____/__|___|  /\____/\____|__  /__|___|  /__|  \  \    |____|\____|__  /  /  /  
               \/     \/     \/             \/               \/        \/       \__\                 \/  /__/   
                                                                                   (c) 2018-2020 alf45star
                                                                       https://github.com/alf45tar/PedalinoMini
 */

#ifdef WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <AsyncUDP.h>
#include <AppleMidi.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// WiFi MIDI interface to comunicate with AppleMIDI/RTP-MDI devices

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h

// ipMIDI

AsyncUDP                ipMIDI;
IPAddress               ipMIDImulticast(225, 0, 0, 37);
const unsigned int      ipMIDIdestPort = 21928;

// WiFi OSC comunication

AsyncUDP                oscUDPout;               // A UDP instance to let us send packets over UDP
IPAddress               oscRemoteIp;             // remote IP of an external OSC device or broadcast address
const unsigned int      oscRemotePort = 9000;    // remote port of an external OSC device

AsyncUDP                oscUDPin;                // A UDP instance to let us receive packets over UDP
const unsigned int      oscLocalPort  = 8000;    // local port to listen for OSC packets
#endif  // WIFI

bool                    appleMidiConnected = false;
unsigned long           wifiLastOn         = 0;

#ifdef NOWIFI
#define AppleMidiSendNoteOn(...)
#define AppleMidiSendNoteOff(...)
#define AppleMidiSendAfterTouchPoly(...)
#define AppleMidiSendControlChange(...)
#define AppleMidiSendProgramChange(...)
#define AppleMidiSendAfterTouch(...)
#define AppleMidiSendPitchBend(...)
#define AppleMidiSendSystemExclusive(...)
#define AppleMidiSendTimeCodeQuarterFrame(...)
#define AppleMidiSendSongPosition(...)
#define AppleMidiSendSongSelect(...)
#define AppleMidiSendTuneRequest(...)
#define AppleMidiSendClock(...)
#define AppleMidiSendStart(...)
#define AppleMidiSendContinue(...)
#define AppleMidiSendStop(...)
#define AppleMidiSendActiveSensing(...)
#define AppleMidiSendSystemReset(...)
#define ipMIDISendChannelMessage1(...)
#define ipMIDISendChannelMessage2(...)
#define ipMIDISendSystemCommonMessage1(...)
#define ipMIDISendSystemCommonMessage2(...)
#define ipMIDISendRealTimeMessage(...)
#define ipMIDISendNoteOn(...)
#define ipMIDISendNoteOff(...)
#define ipMIDISendAfterTouchPoly(...)
#define ipMIDISendControlChange(...)
#define ipMIDISendProgramChange(...)
#define ipMIDISendAfterTouch(...)
#define ipMIDISendPitchBend(...)
#define ipMIDISendSystemExclusive(...)
#define ipMIDISendTimeCodeQuarterFrame(...)
#define ipMIDISendSongPosition(...)
#define ipMIDISendSongSelect(...)
#define ipMIDISendTuneRequest(...)
#define ipMIDISendClock(...)
#define ipMIDISendStart(...)
#define ipMIDISendContinue(...)
#define ipMIDISendStop(...)
#define ipMIDISendActiveSensing(...)
#define ipMIDISendSystemReset(...)
#define OSCSendNoteOn(...)
#define OSCSendNoteOff(...)
#define OSCSendAfterTouchPoly(...)
#define OSCSendControlChange(...)
#define OSCSendProgramChange(...)
#define OSCSendAfterTouch(...)
#define OSCSendPitchBend(...)
#define OSCSendSystemExclusive(...)
#define OSCSendTimeCodeQuarterFrame(...)
#define OSCSendSongPosition(...)
#define OSCSendSongSelect(...)
#define OSCSendTuneRequest(...)
#define OSCSendClock(...)
#define OSCSendStart(...)
#define OSCSendContinue(...)
#define OSCSendStop(...)
#define OSCSendActiveSensing(...)
#define OSCSendSystemReset(...)
#else

void AppleMidiSendNoteOn(byte note, byte velocity, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendNoteOn(note, velocity, channel);
}

void AppleMidiSendNoteOff(byte note, byte velocity, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendNoteOff(note, velocity, channel);
}

void AppleMidiSendAfterTouchPoly(byte note, byte pressure, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendPolyPressure(note, pressure, channel);
}

void AppleMidiSendControlChange(byte number, byte value, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendControlChange(number, value, channel);
}

void AppleMidiSendProgramChange(byte number, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendProgramChange(number, channel);
}

void AppleMidiSendAfterTouch(byte pressure, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendAfterTouch(pressure, channel);
}

void AppleMidiSendPitchBend(int bend, byte channel)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendPitchBend(bend, channel);
}

void AppleMidiSendSystemExclusive(const byte* array, unsigned size)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendSysEx(array, size);
}

void AppleMidiSendTimeCodeQuarterFrame(byte data)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendTimeCodeQuarterFrame(data);
}

void AppleMidiSendSongPosition(unsigned int beats)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendSongPosition(beats);
}

void AppleMidiSendSongSelect(byte songnumber)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendSongSelect(songnumber);
}

void AppleMidiSendTuneRequest(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendTuneRequest();
}

void AppleMidiSendClock(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendClock();
}

void AppleMidiSendStart(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendStart();
}

void AppleMidiSendContinue(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendContinue();
}

void AppleMidiSendStop(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendStop();
}

void AppleMidiSendActiveSensing(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendActiveSensing();
}

void AppleMidiSendSystemReset(void)
{
  if (wifiEnabled && interfaces[PED_RTPMIDI].midiOut) AppleMIDI.sendReset();
}


// Send messages to WiFi ipMIDI interface

void ipMIDISendChannelMessage1(byte type, byte channel, byte data1)
{
  byte midiPacket[2];

  if (!wifiEnabled || !interfaces[PED_IPMIDI].midiOut) return;

  midiPacket[0] = (type & 0xf0) | ((channel - 1) & 0x0f);
  midiPacket[1] = data1;
  ipMIDI.writeTo(midiPacket, 2, ipMIDImulticast, ipMIDIdestPort);
}

void ipMIDISendChannelMessage2(byte type, byte channel, byte data1, byte data2)
{
  byte midiPacket[3];

  if (!wifiEnabled || !interfaces[PED_IPMIDI].midiOut) return;

  midiPacket[0] = (type & 0xf0) | ((channel - 1) & 0x0f);
  midiPacket[1] = data1;
  midiPacket[2] = data2;
  ipMIDI.writeTo(midiPacket, 3, ipMIDImulticast, ipMIDIdestPort);
}

void ipMIDISendSystemCommonMessage1(byte type, byte data1)
{
  byte midiPacket[2];

  if (!wifiEnabled || !interfaces[PED_IPMIDI].midiOut) return;

  midiPacket[0] = type;
  midiPacket[1] = data1;
  ipMIDI.writeTo(midiPacket, 2, ipMIDImulticast, ipMIDIdestPort);
}

void ipMIDISendSystemCommonMessage2(byte type, byte data1, byte data2)
{
  byte  midiPacket[3];

  if (!wifiEnabled || !interfaces[PED_IPMIDI].midiOut) return;

  midiPacket[0] = type;
  midiPacket[1] = data1;
  midiPacket[2] = data2;
  ipMIDI.writeTo(midiPacket, 3, ipMIDImulticast, ipMIDIdestPort);
}

void ipMIDISendRealTimeMessage(byte type)
{
  byte midiPacket[1];

  if (!wifiEnabled || !interfaces[PED_IPMIDI].midiOut) return;

  midiPacket[0] = type;
  ipMIDI.writeTo(midiPacket, 1, ipMIDImulticast, ipMIDIdestPort);
}

void ipMIDISendNoteOn(byte note, byte velocity, byte channel)
{
  ipMIDISendChannelMessage2(midi::NoteOn, channel, note, velocity);
}

void ipMIDISendNoteOff(byte note, byte velocity, byte channel)
{
  ipMIDISendChannelMessage2(midi::NoteOff, channel, note, velocity);
}

void ipMIDISendAfterTouchPoly(byte note, byte pressure, byte channel)
{
  ipMIDISendChannelMessage2(midi::AfterTouchPoly, channel, note, pressure);
}

void ipMIDISendControlChange(byte number, byte value, byte channel)
{
  ipMIDISendChannelMessage2(midi::ControlChange, channel, number, value);
}

void ipMIDISendProgramChange(byte number, byte channel)
{
  ipMIDISendChannelMessage1(midi::ProgramChange, channel, number);
}

void ipMIDISendAfterTouch(byte pressure, byte channel)
{
  ipMIDISendChannelMessage1(midi::AfterTouchChannel, channel, pressure);
}

void ipMIDISendPitchBend(int bend, byte channel)
{
  const unsigned ubend = unsigned(bend - int(MIDI_PITCHBEND_MIN));
  ipMIDISendChannelMessage2(midi::PitchBend, channel, ubend & 0x7f, (ubend >> 7) & 0x7f);
}

void ipMIDISendSystemExclusive(const byte* array, unsigned size)
{
  //
  //  to be implemented
  //
}

void ipMIDISendTimeCodeQuarterFrame(byte data)
{
  ipMIDISendSystemCommonMessage1(midi::TimeCodeQuarterFrame, data);
}

void ipMIDISendSongPosition(unsigned int beats)
{
  ipMIDISendSystemCommonMessage2(midi::SongPosition, beats >> 4, beats & 0x0f);
}

void ipMIDISendSongSelect(byte songnumber)
{
  ipMIDISendSystemCommonMessage1(midi::SongSelect, songnumber);
}

void ipMIDISendTuneRequest(void)
{
  ipMIDISendRealTimeMessage(midi::TuneRequest);
}

void ipMIDISendClock(void)
{
  ipMIDISendRealTimeMessage(midi::Clock);
}

void ipMIDISendStart(void)
{
  ipMIDISendRealTimeMessage(midi::Start);
}

void ipMIDISendContinue(void)
{
  ipMIDISendRealTimeMessage(midi::Continue);
}

void ipMIDISendStop(void)
{
  ipMIDISendRealTimeMessage(midi::Stop);
}

void ipMIDISendActiveSensing(void)
{
  ipMIDISendRealTimeMessage(midi::ActiveSensing);
}

void ipMIDISendSystemReset(void)
{
  ipMIDISendRealTimeMessage(midi::SystemReset);
}


// Send messages to WiFI OSC interface

void OSCSendNoteOn(byte note, byte velocity, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/note/";
  msg += note;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)(velocity / 127.0)).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendNoteOff(byte note, byte velocity, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;
  
  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/note/";
  msg += note;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)0).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendAfterTouchPoly(byte note, byte pressure, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/aftertouchpoly/";
  msg += note;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)(pressure / 127.0)).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendControlChange(byte number, byte value, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/cc/";
  msg += number;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)(value / 127.0)).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendProgramChange(byte number, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/pc/";
  msg += number;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);  
}

void OSCSendAfterTouch(byte pressure, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/aftertouchchannel/";
  msg += channel;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)(pressure / 127.0)).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendPitchBend(int bend, byte channel)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/pitchbend/";
  msg += channel;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((float)((bend + 8192) / 16383.0)).add((int32_t)channel).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendSystemExclusive(const byte* array, unsigned size)
{
}

void OSCSendTimeCodeQuarterFrame(byte data)
{
}

void OSCSendSongPosition(unsigned int beats)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/songpostion/";
  msg += beats;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((int32_t)beats).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendSongSelect(byte songnumber)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  String msg = "/pedalino/midi/songselect/";
  msg += songnumber;
  OSCMessage oscMsg(msg.c_str());
  oscMsg.add((int32_t)songnumber).send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendTuneRequest(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/tunerequest/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendClock(void)
{
}

void OSCSendStart(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/start/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendContinue(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/continue/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendStop(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/stop/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendActiveSensing(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/activesensing/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}

void OSCSendSystemReset(void)
{
  if (!wifiEnabled || !interfaces[PED_OSC].midiOut) return;

  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/pedalino/midi/reset/");
  oscMsg.send(udpMsg).empty();
  oscUDPout.send(udpMsg);
}
#endif  //  NOWIFI