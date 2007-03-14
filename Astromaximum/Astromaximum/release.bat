set JAVA_HOME=d:\Progra~1\jdk160\
if "%1"=="imei" goto imei

:timebomb
"d:\Program Files\netbeans-5.5\ide7\ant\bin\ant.bat" -Dconfig.active=midp2y2007release_tb -Dtb.timeout=%2 clean deploy
goto endit

:imei
"d:\Program Files\netbeans-5.5\ide7\ant\bin\ant.bat" -Dconfig.active=midp2y2007release -Dimei.code=%2 clean deploy
goto endit


:endit
