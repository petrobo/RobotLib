/**
 * @file RadioRobot.cpp
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Anderson Antunes <anderson.utf@gmail.com>
 * 
 * RobotLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RobotLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with RobotLib.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "RadioRobot.h"

void RadioRobot::messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
  uint8_t offset = 0;
  uint8_t cmd;
  uint8_t id;
  uint8_t length;
  uint8_t i;
  while (offset < size){
    cmd = data[offset];
    offset++;
    switch (cmd){
      case STOP:
        {
          id = data[offset];
          offset++;
          if (id == ALL){
            for (i = 0; i < getDeviceListSize(); i++){
              getDevice(i)->stop();
            }
          } else {
            Device * d = getDevice(id);
            if (d){
              d->stop();
            }
          }
          //DONE
          uint8_t * tmpBuffer = buffer +size;
          tmpBuffer[0] = DONE;
          tmpBuffer[1] = STOP;
          tmpBuffer[2] = id;
          tmpBuffer[3] = 0;
          connection.sendMessage(tmpBuffer,4);
          break;
        }
      case ECHO:
        {
          length = data[offset];
          offset++;
          connection.sendMessage(data+offset,length);
          offset += length;
          break;
        }
      case PRINT:
        {
          id = data[offset];
          offset++;
          length = data[offset];
          offset++;
          Connection * c = NULL;
          if (id == ALL){
            for (i = 0; i < getConnectionListSize(); i++){
              c = getConnection(i);
              if (c){
                c->sendMessage(data+offset,length);
              }
            }
          } else {
            c = getConnection(id);
            if (c){
              c->sendMessage(data+offset,length);
            }
          }
          offset += length;
          //DONE
          uint8_t * tmpBuffer = buffer +size;
          tmpBuffer[0] = DONE;
          tmpBuffer[1] = PRINT;
          tmpBuffer[2] = id;
          tmpBuffer[3] = length;
          connection.sendMessage(tmpBuffer,4);
          break;
        }
      case GET:
        {
          id = data[offset];
          offset++;
          length = data[offset];
          offset++;
          // usa parte restante do buffer principal
          uint8_t * tmpBuffer = buffer +size;
          Device * device = NULL;
          if (id == FREE_RAM){
            int free = freeRam();
            //envia set
            tmpBuffer[0] = SET;
            tmpBuffer[1] = FREE_RAM;
            tmpBuffer[2] = sizeof(int);
            memcpy(tmpBuffer+3, &free, sizeof(int));
            connection.sendMessage(tmpBuffer,3+sizeof(int));
          } else if (id == ALL){
            for (i = 0; i < getDeviceListSize(); i++){
              device = getDevice(i);
              if (device){
                //device.get(...)
                uint8_t tmpLen = device->get(tmpBuffer +3, BUFFER_SIZE -size -3);
                //envia set
                tmpBuffer[0] = SET;
                tmpBuffer[1] = id;
                tmpBuffer[2] = tmpLen;
                connection.sendMessage(tmpBuffer,tmpLen +3);
              }
            }
          } else {
            device = getDevice(id);
            if (device){
              //device.get(...)
              uint8_t tmpLen = device->get(tmpBuffer +3, BUFFER_SIZE -size -3);
              //envia set
              tmpBuffer[0] = SET;
              tmpBuffer[1] = id;
              tmpBuffer[2] = tmpLen;
              connection.sendMessage(tmpBuffer,tmpLen +3);
            }
          }
          offset+=length;
          break;
        }
      case SET:
        {
          id = data[offset];
          offset++;
          length = data[offset];
          offset++;
          Device * device = getDevice(id);
          if (device){
            //device.set
            device->set(data+offset, length);
            offset+=length;
            //DONE
            uint8_t * tmpBuffer = buffer +size;
            tmpBuffer[0] = DONE;
            tmpBuffer[1] = SET;
            tmpBuffer[2] = id;
            tmpBuffer[3] = length;
            connection.sendMessage(tmpBuffer,4);
          }
          break;
        }
      case ADD:
        {
          id = data[offset];
          offset++;
          length = data[offset];
          offset++;
          Device * device = createNew(id, data+offset, length);
          if (device){
            addDevice(*device);
            //DONE
            uint8_t * tmpBuffer = buffer +size;
            tmpBuffer[0] = DONE;
            tmpBuffer[1] = ADD;
            tmpBuffer[2] = id;
            tmpBuffer[3] = length;
            connection.sendMessage(tmpBuffer,4);
            device->begin();
          }
          offset+=length;
        }
        break;
      case RESET:
        {
          id = data[offset];
          offset++;
          if (id == ALL){
            for (i = 0; i < getDeviceListSize(); i++){
              getDevice(i)->reset();
            }
          } else {
            Device * d = getDevice(id);
            if (d){
              d->reset();
            }
          }
          //DONE
          uint8_t * tmpBuffer = buffer +size;
          tmpBuffer[0] = DONE;
          tmpBuffer[1] = RESET;
          tmpBuffer[2] = id;
          tmpBuffer[3] = 0;
          connection.sendMessage(tmpBuffer,4);
        }
        break;
      case DONE:
        offset+=4; //avança o tamanho do comando DONE (4 bytes)
        break;
      case NO_OP:
        break;
      default:
        return;
    }
  }
}
