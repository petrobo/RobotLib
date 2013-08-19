#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>

/**
 * Sketch para teste da comunicaçao serial entre PC-Arduino.
 * 
 * Tudo que eh recebido pela porta serial eh enviado de volta.
 */

class MyRobot : public Robot {
public:
  MyRobot() : serial(Serial,9600) {
    addConnection(serial);
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    connection.sendMessage(data,size);
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
  //Adicionar os dispositivos aqui
  SerialConnection serial;
};

MyRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
