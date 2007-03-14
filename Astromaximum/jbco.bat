@echo off
SET CLASSPATH=D:\Willow\prj\astrology\nomad_prj\Nomad\build\midp2y2007\compiled;D:\jdk1.6.0\jre1.6\lib\rt.jar;D:\WTK25\lib\midpapi20.jar;D:\WTK25\lib\cldcapi11.jar;D:\jdk1.6.0\jre1.6\lib\jce.jar;D:\Willow\soot\classes;D:\Willow\soot\jasminclasses-2327.jar;D:\Willow\soot\polyglotclasses-1.3.2.jar;D:\WTK25\lib\j2me-ws.jar;D:\WTK25\lib\j2me-xmlrpc.jar;D:\WTK25\lib\jsr75.jar;D:\WTK25\lib\jsr082.jar;D:\WTK25\lib\jsr179.jar;D:\WTK25\lib\jsr180.jar;D:\WTK25\lib\jsr184.jar;D:\WTK25\lib\jsr211.jar;D:\WTK25\lib\jsr226.jar;D:\WTK25\lib\jsr229.jar;D:\WTK25\lib\jsr234.jar;D:\WTK25\lib\jsr238.jar;D:\WTK25\lib\mmapi.jar;D:\WTK25\lib\satsa-apdu.jar;D:\WTK25\lib\satsa-crypto.jar;D:\WTK25\lib\satsa-jcrmi.jar;D:\WTK25\lib\satsa-pki.jar;D:\WTK25\lib\wma11.jar;D:\WTK25\lib\wma20.jar
rem java soot.jbco.Main -jbco:help >help.txt
java -Xmx400m soot.jbco.Main -app Nomad -jbco:verbose -t:8:bb.jbco_iii
rem java -classpath ./;polyglotclasses-1.3.2.jar;D:\jdk1.6.0\jre1.6\lib\rt.jar  soot.Main -help
pause 
rem -t:5:wjtp.jbco_mr -t:5:wjtp.jbco_fr -t:5:wjtp.jbco_bapibm -t:5:wjtp.jbco_blbc  -t:3:jtp.jbco_adss -t:5:jtp.jbco_cae2bo -t:3:bb.jbco_rds -t:5:bb.jbco_riitcb -t:5:bb.jbco_iii 
