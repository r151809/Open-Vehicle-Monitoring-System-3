/*
;    Project:       Open Vehicle Monitor System
;    Date:          4th May 2020
;
;    Changes:
;    0.1  Initial release
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
;
; Currently support only passive listening of the CAN bus. 
; 
; Testing status: "Works for me", meaning everything is tried just once.  
;
; Most data is reverse engineered from Mercedes Benz B250E (W242), year 2015. 
; vehicle_smarted.cpp was used as an example, and some MsgID's are the same. 
; Some MsgID's matched other Mercedes models. OpenDBC was helpfull here: 
; https://github.com/commaai/opendbc/blob/master/mercedes_benz_e350_2010.dbc
*/

#include "ovms_log.h"
static const char *TAG = "v-mbb250e";

#define VERSION "0.1.0"

#include <stdio.h>
#include "ovms_metrics.h"


#include "vehicle_mercedesb250e.h"

OvmsVehicleMercedesB250e::OvmsVehicleMercedesB250e()
{
  ESP_LOGI(TAG, "Start Mercedes-Benz B250E vehicle module");

  mt_mb_trip_reset        = MyMetrics.InitFloat("xmb.v.display.trip.reset", SM_STALE_MIN, 0, Kilometers);
  mt_mb_trip_start        = MyMetrics.InitFloat("xmb.v.display.trip.start", SM_STALE_MIN, 0, Kilometers);
  mt_mb_consumption_start = MyMetrics.InitFloat("xmb.v.display.consumption.start", SM_STALE_MIN, 0, WattHoursPK);
  mt_mb_consumption_reset = MyMetrics.InitFloat("xmb.v.display.consumption.reset", SM_STALE_MIN, 0, WattHoursPK);
  mt_mb_eco_accel         = MyMetrics.InitFloat("xmb.v.display.accel", SM_STALE_MIN, 0, Percentage);
  mt_mb_eco_const         = MyMetrics.InitFloat("xmb.v.display.const", SM_STALE_MIN, 0, Percentage);
  mt_mb_eco_coast         = MyMetrics.InitFloat("xmb.v.display.coast", SM_STALE_MIN, 0, Percentage);
  mt_mb_eco_score         = MyMetrics.InitFloat("xmb.v.display.ecoscore", SM_STALE_MIN, 0, Percentage);
  mt_mb_fl_speed          = MyMetrics.InitFloat("xmb.v.fl_speed", SM_STALE_MIN, 0, Kph);
  mt_mb_fr_speed          = MyMetrics.InitFloat("xmb.v.fr_speed", SM_STALE_MIN, 0, Kph);
  mt_mb_rl_speed          = MyMetrics.InitFloat("xmb.v.rl_speed", SM_STALE_MIN, 0, Kph);
  mt_mb_rr_speed          = MyMetrics.InitFloat("xmb.v.rr_speed", SM_STALE_MIN, 0, Kph);
  
  RegisterCanBus(1, CAN_MODE_ACTIVE, CAN_SPEED_500KBPS);
}

OvmsVehicleMercedesB250e::~OvmsVehicleMercedesB250e()
{
  ESP_LOGI(TAG, "Shutdown MERCEDESB250E vehicle module");
}

class OvmsVehicleMercedesB250eInit
  {
  public: OvmsVehicleMercedesB250eInit();
} MyOvmsVehicleMercedesB250eInit  __attribute__ ((init_priority (9000)));

OvmsVehicleMercedesB250eInit::OvmsVehicleMercedesB250eInit()
{
  ESP_LOGI(TAG, "Registering Vehicle: MERCEDESB250E (9000)");

  MyVehicleFactory.RegisterVehicle<OvmsVehicleMercedesB250e>("MB", "Mercedes-Benz B250E, W242");
}

void OvmsVehicleMercedesB250e::IncomingFrameCan1(CAN_frame_t* p_frame)
{
  uint8_t *d = p_frame->data.u8;
   
  switch (p_frame->MsgID) {
    // case 0x4B: // d[3]&0x40 preheat button: 0x04B 01 01 0C 46 FF 04 
    // case 0x4B: // d[2]&0x80 range plus button: 0x04B 01 01 8C 06 FF 04 
    // case 0x73: // d[4]*0.1 Aux battery voltage, same as 0x205 d[1]*0.1, except 0x73 has 10x sample rate, and value 0xff should be ignored
    // case 0xff: // d[1].bit4 car on or similar, bits 0-2 tell something about startup
  case 0x105: // Motor RPM
    {
      int rpm = ((d[0]&0x3f) << 8) + d[1]; 
      StandardMetrics.ms_v_mot_rpm->SetValue(rpm); // RPM
      StandardMetrics.ms_v_env_throttle->SetValue(d[4]/2.50); // Drive pedal state [%], raw values are from 0 to 250
      break;
    }
  case 0x19F: // Speedo
    {
      float speed = ( ((d[0]&0xf) << 8) + d[1] ) * 0.1;
      float odo   = ( (d[5] << 16) + (d[6] << 8) + (d[7]) ) * 0.1;
      StandardMetrics.ms_v_pos_speed->SetValue(speed); // speed in km/h
      StandardMetrics.ms_v_pos_odometer->SetValue(odo); // ODO km
      // d3&d4 is a minute counter,
      // d2 *0.5 - 40 could be outdoor temp
      StandardMetrics.ms_v_env_temp->SetValue( (d[2]-80)*0.5 );
      break;
    }
  case 0x203: // Wheel speeds
    {
      /* Using wheel speed for speedo meter, as I have not yet found a better number */
      float fl_speed = ( ((d[0]&0xf) << 8) + d[1] ) * 0.0603504; 
      float fr_speed = ( ((d[2]&0xf) << 8) + d[3] ) * 0.0603504; 
      float rl_speed = ( ((d[4]&0xf) << 8) + d[5] ) * 0.0603504; 
      float rr_speed = ( ((d[5]&0xf) << 8) + d[7] ) * 0.0603504; 
      mt_mb_fl_speed->SetValue(fl_speed); // km/h alread
      mt_mb_fr_speed->SetValue(fr_speed); // km/h alread
      mt_mb_rl_speed->SetValue(rl_speed); // km/h alread
      mt_mb_rr_speed->SetValue(rr_speed); // km/h alread
      break;
    }
  case 0x205: // 12V battery voltage
    {
      StandardMetrics.ms_v_bat_12v_voltage->SetValue((float)d[1]*0.1); // Volts
      break;
    }	
  case 0x20B: // HVAC
    {
      StandardMetrics.ms_v_env_cabinsetpoint->SetValue((float)d[0]*0.1+10); // deg C
      // d[1] is right side set temp
      // d[4] ls nible, could be ms_v_env_cabinfan
      // d[5] contain ms_v_env_cooling (bit 7) and heating (bit 6), possibly separated for drive (lsb side) and park      
      StandardMetrics.ms_v_env_heating->SetValue( (d[5]>>6)&1 );
      StandardMetrics.ms_v_env_cooling->SetValue( (d[5]>>7)&1 );
      break;
    }
  case 0x245: 
    {
      //  d[1:0] signed steering wheel angle
      //  d[5:4] signed G
      break;
    }
  case 0x283: 
    {
      //  d[4] * 0.5 - 40 could be Motor temp
      break;
    }
    // 0x2C4 29:18, little endian, GPS 'west'
  case 0x2EF: // 
    {
      int temp = d[4] - 40;
      StandardMetrics.ms_v_bat_temp->SetValue(temp); // Probably water temp
      // StandardMetrics.ms_v_env_footbrake->SetValue( (bool)((d[3]>>7)&0x1)); // Friction brake
      bool hb = (bool)((d[3]>>5)&0x1);
      StandardMetrics.ms_v_env_handbrake->SetValue( hb ); // 
      break;
    }
  case 0x2FF: // TPMS
    {
      if (d[3] < 255)
	StandardMetrics.ms_v_tpms_fl_p->SetValue((float)d[3]*2.5); // KPa
      if (d[4] < 255)
	StandardMetrics.ms_v_tpms_fr_p->SetValue((float)d[4]*2.5); // KPa
      if (d[5] < 255)
	StandardMetrics.ms_v_tpms_rl_p->SetValue((float)d[5]*2.5); // KPa
      if (d[6] < 255)
	StandardMetrics.ms_v_tpms_rr_p->SetValue((float)d[6]*2.5); // KPa
      break;
    }	
  case 0x33D: // Momentary power
    {
      float power = d[4]-100; // Percents, +/- 100
      if (power > 0) 
	power *= 1.32; // 132 kW is the total power promised by manufacturer
      else
	power *= 0.4; // guess that maximum recuperation would be ~40 kW
        // See https://avt.inl.gov/sites/default/files/pdf/fsev/fact2015mercedesbclass.pdf
      StandardMetrics.ms_v_bat_power->SetValue(power); // kW
      // d[5]&0xf 0x1: // casting
      // d[5]&0xf 0x4: // full throttle
      // d[5]&0xf 0x7: // normal throttle
      // d[5]&0xf 0x8: // brake
      // d[5]&0xf 0xa: // park
      break;
    }	    
  case 0x34E:  // Distance Today , Distance since reset, scale is 0.1 km
    { 
      int trip_start = d[0] * 256 * 256 + d[1] * 256 + d[2];
      int trip_reset = d[3] * 256 * 256 + d[4] * 256 + d[5];
      if (trip_start < 0xfffffe) // for some reason undefined value here is 0xfffffe
	mt_mb_trip_start->SetValue(trip_start * 0.1); 
      if (trip_reset < 0xfffffe) 
        mt_mb_trip_reset->SetValue(trip_reset * 0.1);
      break;
    }      
  case 0x34F: // Range
    {
      int consumption = (d[0]&0x7)*256 + d[1];
      int range  = (d[6]&0x7)*256 + d[7]; // Car's estimate on remainging range
      if (range < 2047)
	StandardMetrics.ms_v_bat_range_est->SetValue((float)range); // km
      mt_mb_consumption_start->SetValue((float)consumption);
      consumption = (d[2]&0x7)*256 + d[3];
      mt_mb_consumption_reset->SetValue((float)consumption);
      /* The following metric is strong maybe. */
      //      StandardMetrics.ms_v_charge_inprogress->SetValue( (bool)((d[5]>>1) & 1) );
      break;
    }
  case 0x37D:
   {
     int cur = (d[4]&0x3)*256+d[3];
     if (cur & 0x200)
       cur |= (-1 & ~0x1ff); // sign-extend
     StandardMetrics.ms_v_env_charging12v->SetValue( (d[2]>>2)&0x1 );
     StandardMetrics.ms_v_bat_12v_current->SetValue( cur * 0.1 );
     break;
    }
  case 0x39F: // Car time now
    {
      // d0 hour
      // d1 minutes
      // d2 seconds
      // d3 day
      // d4 month
      // d5 year (two digits)
      break;
    }
    //case 0x3e7: // GPS
  case 0x3eb: // GPS
    {
      // d[1:0]*0.0368 is speed 
      // d[3:2] 12 bit is something odd 
      // d[5:4] 12 bit *0.1 is maybe consumption average
      // d[7:6] 12 bit *0.1 is maybe consumption average
      
      float gps_speed = (float)(d[0])*256 + (float)d[1]; // guess
      if (d[0] < 255) { // Update only with feasible results
	StandardMetrics.ms_v_pos_gpsspeed->SetValue(gps_speed*0.0368); // kph
      }
      break;
    }
    
  case 0x3F2: //Eco display
    {
      if (d[0] <= 200) { // Eco values show as 0xff when the car is switched on/off 
	mt_mb_eco_accel->SetValue((float)d[0] * 0.5);
	mt_mb_eco_const->SetValue((float)d[1] * 0.5);
	mt_mb_eco_coast->SetValue((float)d[2] * 0.5);
	mt_mb_eco_score->SetValue((float)d[3] * 0.5);
      }
      break;
    }
  default:
    //ESP_LOGD(TAG, "IFC %03x 8 %02x %02x %02x %02x %02x %02x %02x %02x",
    //p_frame->MsgID, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
    break;
  }  
}
