/*
;    Project:       Open Vehicle Monitor System
;    Date:          4th May 2020
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2020       Jarkko Ruoho
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

#ifndef __VEHICLE_MERCEDESB250E_H__
#define __VEHICLE_MERCEDESB250E_H__

#include "vehicle.h"

using namespace std;

class OvmsVehicleMercedesB250e : public OvmsVehicle
  {
  public:
    OvmsVehicleMercedesB250e();
    ~OvmsVehicleMercedesB250e();

  public:
    void IncomingFrameCan1(CAN_frame_t* p_frame);

  protected:
    OvmsMetricFloat *mt_mb_trip_reset;        // Distance since reset
    OvmsMetricFloat *mt_mb_trip_start;        // Distance since default trip started
    OvmsMetricFloat *mt_mb_consumption_start; // Consumption since default trip started
    OvmsMetricFloat *mt_mb_consumption_reset; // Consumption since long time average reseted
    OvmsMetricFloat *mt_mb_eco_accel;         // eco score on acceleration over last 6 hours
    OvmsMetricFloat *mt_mb_eco_const;         // eco score on constant driving over last 6 hours
    OvmsMetricFloat *mt_mb_eco_coast;         // eco score on coasting over last 6 hours
    OvmsMetricFloat *mt_mb_eco_score;         // eco score shown on dashboard over last 6 hours
    
    OvmsMetricFloat *mt_mb_fl_speed;          // Front Left  wheel speed, km/h
    OvmsMetricFloat *mt_mb_fr_speed;          // Front Right wheel speed, km/h
    OvmsMetricFloat *mt_mb_rl_speed;          // Rear  Left  wheel speed, km/h
    OvmsMetricFloat *mt_mb_rr_speed;          // Rear  Right wheel speed, km/h

    OvmsMetricFloat *mt_mb_steering_wheel;    // Steering wheel angle, 1.0 is max left and -1.0 is right

    OvmsMetricFloat *mt_mb_forward_g;         // 1.0 is the max
    OvmsMetricFloat *mt_mb_side_g;            // 1.0 is the max turn left

    OvmsMetricFloat *mt_mb_temperature1;      // deg C, unknown temperature sensor
    OvmsMetricFloat *mt_mb_temperature2;      // deg C, unknown temperature sensor
    OvmsMetricFloat *mt_mb_temperature3;      // deg C, unknown temperature sensor
    OvmsMetricFloat *mt_mb_drive_pedal_slow;  // percent

    char m_vin[18];
 
  };

#endif //#ifndef __VEHICLE_MERCEDESB250E_H__
