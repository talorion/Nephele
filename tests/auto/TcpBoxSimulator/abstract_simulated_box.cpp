#include "abstract_simulated_box.hpp"

abstract_simulated_box::abstract_simulated_box(QObject *par) : QObject(par)
{

}

abstract_simulated_box::~abstract_simulated_box()
{

}

QString abstract_simulated_box::getHelp_string()
{
  QString ret ="";
  ret +="d\r\n";
  ret +="fuse\r\n";
  ret +="hostname\r\n";
  ret +="reset\r\n";
  ret +="wdreset\r\n";
  ret +="uibk getAll\r\n";
  ret +="uibk setById\r\n";
  ret +="uibk saveChannelInfos\r\n";
  ret +="uibk setDefaultChannelInfos\r\n";
  ret +="uibk setChannelName\r\n";
  ret +="uibk setChannelUnits\r\n";
  ret +="uibk setChannelSetmin\r\n";
  ret +="uibk setChannelSetmax\r\n";
  ret +="uibk setChannelActmin\r\n";
  ret +="uibk setChannelActmax\r\n";
  ret +="uibk setChannelSet\r\n";
  ret +="uibk frametest\r\n";
  ret +="io\r\n";
  ret +="adc get\r\n";
  ret +="mcp970x get\r\n";
  ret +="mac\r\n";
  ret +="pwm servo_set\r\n";
  ret +="pwm servo_inc\r\n";
  ret +="pwm servo_dec\r\n";
  ret +="netmask\r\n";
  ret +="ip\r\n";
  ret +="gw\r\n";
  ret +="help\r\n";
  ret +="version\r\n";
  ret +="eeprom reinit\r\n";
  ret += "\r\n\0";

  return ret;

}

