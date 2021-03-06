/**
 * @file Compass.h
 * @author Diego Lee <diegolee7@gmail.com>
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Diego Lee <diegolee7@gmail.com>
 *                       Anderson Antunes <anderson.utf@gmail.com>
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

#ifndef COMPASS_H
#define COMPASS_H

#include <stdint.h>
#include <HMC5883L.h>
#include "Device.h"
#include "Clock.h"

class Compass : public TimedDevice {
public:
  Compass();
  Compass(float xmin, float xmax, float ymin, float ymax);
  void begin();
  void stop();
  void reset();
  void update();
  bool isReady();
  bool available();
  uint8_t get(uint8_t * buffer, uint8_t size);
  void set(const uint8_t * data, uint8_t size = 1);

  int getAngle();
  void calibrate(float xmin, float xmax, float ymin, float ymax);
  HMC5883L * getCompass();
private:
  HMC5883L compass;
  Timer timer;
  int angleInt;
  bool newValue;
  float Xmin, Xmax, Ymin, Ymax;
};

#endif  /* COMPASS_H */

