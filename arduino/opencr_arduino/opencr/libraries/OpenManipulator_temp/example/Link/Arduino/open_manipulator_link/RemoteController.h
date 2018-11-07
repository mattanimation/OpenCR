/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Darby Lim */

#ifndef REMOTE_CONTROLLER_H_
#define REMOTE_CONTROLLER_H_

#include "Link.h"
#include <RC100.h>

RC100 rc100;

void connectRC100()
{
  rc100.begin(1);
}

int availableRC100()
{
  return rc100.available();
}

uint16_t readRC100Data()
{
  return rc100.readData();
}

void fromRC100(uint16_t data)
{
  if (data & RC100_BTN_U)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(MOVESTEP, 0.0, 0.0), MOVETIME);
  else if (data & RC100_BTN_D)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(-MOVESTEP, 0.0, 0.0), MOVETIME);
  else if (data & RC100_BTN_L)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(0.0, MOVESTEP, 0.0), MOVETIME);
  else if (data & RC100_BTN_R)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(0.0, -MOVESTEP, 0.0), MOVETIME);
  else if (data & RC100_BTN_1)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(0.0, 0.0, MOVESTEP), MOVETIME);
  else if (data & RC100_BTN_3)
    omlink.setMove(SUCTION, OM_MATH::makeVector3(0.0, 0.0, -MOVESTEP), MOVETIME);
  else if (data & RC100_BTN_2)
  {
    suctionOn();
  }
  else if (data & RC100_BTN_4)
  {
    suctionOff();
  }
  else if (data & RC100_BTN_5)
  {
    motionStart();
  }
  else if (data & RC100_BTN_6)
  {
    motionStop();
  }
}
#endif