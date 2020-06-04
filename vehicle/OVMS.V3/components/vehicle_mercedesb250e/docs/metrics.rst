----------------
Standard Metrics
----------------

============================= ============================================
Metric name                   Notes
============================= ============================================
ms_v_mot_rpm                  
ms_v_env_throttle             Resolution of 0.4 %
ms_v_pos_speed                Resolution of 0.1 km/h
ms_v_pos_odometer             Resolution of 0.1 km
ms_v_bat_12v_voltage          Resolution of 0.1 V
ms_v_bat_power                Engine power (heating is excluded from this value). Car reports percents, so the kW is just calculated by multiplying by 132 kW. Negative side is scaled to -40 kW. This is based on these measurements: https://avt.inl.gov/sites/default/files/pdf/fsev/fact2015mercedesbclass.pdf 
ms_v_bat_range_est
ms_v_bat_consumption          Instantious consumption, Wh/km
ms_v_tpms_*_p
ms_v_env_cabinsetpoint        There are two figures, left and right. Using just one for now.
ms_v_env_charging12v
ms_v_bat_12v_current          Scaling is not verified. It's signed 10 bits, and scaling is guessed to 0.1A
ms_v_env_heating              Heater or heating on, in preheat mode (may differ when the car is on)
ms_v_env_cooling              Cooling on, in preheat mode (may differ when the car is on)
ms_v_env_temp                 Environment temperature (guessing, could be some other temp also)
ms_v_bat_temp                 This is probably liquid temperature rather than actual battery temp.
ms_v_env_throttle             Throttle position (also cruise uses this) - drive mode effect on this is still unknown
============================= ============================================


--------------
Custom Metrics
--------------

======================================== ======================== ============================================
Metric name                              Example value            Description
======================================== ======================== ============================================
xmb.v.display.trip.reset                 2733.2km                 Dashbord Trip value since Reset
xmb.v.display.trip.start                 17.5km                   Dashbord Trip value since Start (today)
xmb.v.display.consumption.start          25.9 kWh                 Dashbord Trip consumption since Start (today)
xmb.v.display.consumption.reset          22.9 kWh                 Dashbord Trip consumption since Reset
xmb.v.display.accel                      89%                      Eco score on acceleration over last 6 hours
xmb.v.display.const                      18%                      Eco score on constant driving over last 6 hours
xmb.v.display.coast                      100%                     Eco score on coasting over last 6 hours
xmb.v.display.ecoscore                   69%                      Eco score shown on dashboard over last 6 hours
xmb.v.fl_speed                           37.83 km/h               Front Left wheel speed
xmb.v.fr_speed                           37.85 km/h               Front Right wheel speed
xmb.v.rl_speed                           37.80 km/h               Rear Left wheel speed
xmb.v.rr_speed                           37.82 km/h               Rear Right wheel speed
xmb.v.steering_wheel                     0.002                    Steering wheel angle, left side is positive
xmb.v.forward_g                          0.02                     Forward (longitudal) accelration, range +/- 1.0 of maximum (what ever that is)..
xmb.v.side_g                             0.02                     Turning accelration, range +/- 1.0 of maximum (what ever that is), left is positive side..
======================================== ======================== ============================================
