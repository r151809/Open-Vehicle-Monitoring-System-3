/*
;    Project:       Open Vehicle Monitor System
;    Date:          3rd September 2020
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2011       Michael Stegen / Stegen Electronics
;    (C) 2011-2017  Mark Webb-Johnson
;    (C) 2011        Sonny Chen @ EPRO/DX
;    (C) 2020       Chris Staite
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

#include "vehicle_mgev.h"
#include "mg_obd_pids.h"
#include "metrics_standard.h"

void OvmsVehicleMgEv::IncomingTpmsPoll(uint16_t pid, uint8_t* data, uint8_t length)
{
    switch (pid)
    {
        case tyrePressurePid:
            StandardMetrics.ms_v_tpms_fl_p->SetValue(data[0] * 4);
            StandardMetrics.ms_v_tpms_fr_p->SetValue(data[1] * 4);
            StandardMetrics.ms_v_tpms_rr_p->SetValue(data[2] * 4);
            StandardMetrics.ms_v_tpms_rl_p->SetValue(data[3] * 4);
            break;
        case typeTemperaturePid:
            StandardMetrics.ms_v_tpms_fl_t->SetValue(data[0] * 0.5);
            StandardMetrics.ms_v_tpms_fr_t->SetValue(data[1] * 0.5);
            StandardMetrics.ms_v_tpms_rr_t->SetValue(data[2] * 0.5);
            StandardMetrics.ms_v_tpms_rl_t->SetValue(data[3] * 0.5);
            break;
    }
}
