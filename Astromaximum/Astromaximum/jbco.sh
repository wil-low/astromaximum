#!/bin/sh
INPUT_DIR=$1
OUTPUT_DIR=$2
WTK=$3
SOOT_HOME=$4

# java -cp soot-dev/classes soot.jbco.Main -jbco:help; exit
CLASSP=$WTK/cldcapi11.jar:$WTK/midpapi20.jar:$WTK/jsr75.jar:$WTK/mmapi.jar:\
:$WTK/jsr211.jar:$WTK/midpapi21.jar:\
$SOOT_HOME/jasminclasses-2.3.0.jar:$SOOT_HOME/polyglotclasses-1.3.5.jar:$SOOT_HOME/sootclasses-2.3.0.jar:\
$INPUT_DIR

java -cp $CLASSP -Xms256m -Xmx1024m soot.jbco.Main \
-j2me -process-dir $INPUT_DIR -d $OUTPUT_DIR -main-class Astromaximum \
-t:9:bb.jbco_rlaii \
-t:9:bb.jbco_iii \
-t:9:bb.jbco_ctbcb \
-t:9:bb.jbco_riitcb \
-t:2:bb.jbco_plvb \
-t:9:jtp.jbco_gia \
-t:3:bb.jbco_ptss

#-t:9:bb.jbco_ecvf #? thread\
#-t:5:wjtp.jbco_mr \
#-t:5:wjtp.jbco_fr #thread \
#-t:5:jtp.jbco_adss #thread \
#-t:9:bb.jbco_rds #? too long for render\
#-t:9:jtp.jbco_cae2bo #?\
#-t:9:wjtp.jbco_blbc #?\
#-t:5:wjtp.jbco_cr #?\
#-t:5:wjtp.jbco_mr \
#-t:6:wjtp.jbco_bapibm ? thread\

#	wjtp.jbco_cr       -  Rename Classes
#	wjtp.jbco_mr       -  Rename Methods
#	wjtp.jbco_fr       -  Rename Fields
#	wjtp.jbco_blbc     -  Build API Buffer Methods
#	wjtp.jbco_bapibm   -  Build Library Buffer Classes
#	jtp.jbco_gia       -  Goto Instruction Augmentation
#	jtp.jbco_adss      -  Add Dead Switche Statements
#	jtp.jbco_cae2bo    -  Convert Arith. Expr. To Bitshifting Ops
#	bb.jbco_cb2ji      -  Convert Branches to JSR Instructions
#	bb.jbco_dcc        -  Disobey Constructor Conventions
#	bb.jbco_rds        -  Reuse Duplicate Sequences
#	bb.jbco_riitcb     -  Replace If(Non)Nulls with Try-Catch
#	bb.jbco_iii        -  Indirect If Instructions
#	bb.jbco_plvb       -  Pack Locals into Bitfields
#	bb.jbco_rlaii      -  Reorder Loads Above Ifs
#	bb.jbco_ctbcb      -  Combine Try and Catch Blocks
#	bb.jbco_ecvf       -  Embed Constants in Fields
#	bb.jbco_ptss       -  Partially Trap Switches
