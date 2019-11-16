Raspberry Pi Air Cooling using Intel stock CPU fan 
==================================================
Tested on Raspberry Pi 4 running Raspbian busty.
CPU fan used is FANINT1150[Intel Original CPU Fan (OEM Package) for Socket LGA1151/1150], which is 12V based but running fine on Raspberry Pi's 5V GPIO.

Hardward Wiring
---------------
TODO

Production
----------
Apply the same pin and frequency to fan_ctrl.py, then
```shell
$ make fan_ctrl
$ sudo ./fan_ctrl &
```
If you want to run the script whenever system start up, put it in rc.local/cron job.
Or, even better, running as a service
```shell
$ sudo cp fan-ctrl.service /lib/systemd/system/ 
$ sudo systemctl enable fan-ctrl.service
$ sudo systemctl start fan-ctrl.service
```

