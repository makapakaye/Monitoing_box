rmdir /s /q "%USERPROFILE%\Documents\Arduino\libraries\BME68x_Sensor_library"
rmdir /s /q "%USERPROFILE%\Documents\Arduino\libraries\bsec2"

xcopy /e /i "%USERPROFILE%\Documents\GitHub\Monitoing_box\sensor_node\lib\*" "%USERPROFILE%\Documents\Arduino\libraries\"
