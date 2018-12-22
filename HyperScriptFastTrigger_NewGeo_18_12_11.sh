#!/bin/bash

SourcePrefix=2018_11_November

cd /home/otsdaq/CMSTiming/Megascript
#cd /home/otsdaq/2018_11_November_T992/Megascript
#./Megascript.py --RunChewie false --CopyRawFiles true --MergeRawFiles true -d -g Run1116_Merged.geo -r $1
./Megascript.py --RunChewie false --CopyRawFiles true --MergeRawFiles true -d -g Run664_Merged.geo -r $1
cd /home/otsdaq/CMSTiming/Monicelli/
#cd /home/otsdaq/2018_11_November_T992/Monicelli/
source setup${SourcePrefix}_CMSTiming.sh
cd templates/
./RunCMSTimingFastTrigger.py $1
#rsync -uv /data/TestBeam/${SourcePrefix}_CMSTiming/CMSTimingConverted/* sxie@cmslpc31:/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11/data/Tracks/
#rsync -uv /data/TestBeam/2018_06_June_CMSTiming/CMSTimingConverted/* sxie@cmslpc31:/eos/uscms/store/user/cmstestbeam/ETL/2018_04/OTSDAQ/CMSTiming/VME/RAW/
#rsync -uv /data/TestBeam/2018_03_March2018_06_June_CMSTiming/CMSTimingConverted/* sxie@cmslpc31:/uscms_data/d1/sxie/data/CMSTiming/
scp /data/TestBeam/2018_11_November_CMSTiming/CMSTimingConverted/Run$1_CMSTiming_converted.root cmstiming@ftbf-daq-08.fnal.gov:/data/TestBeam/2018_11_November_CMSTiming/Tracks
cd `pwd`
