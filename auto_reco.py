import time
import numpy as np
import getpass
import os
import subprocess as sp
import socket
import sys
import glob
import subprocess
from subprocess import Popen, PIPE
import pipes 
from pipes import quote

def check(list_to_check):
    bool = True
    for element in list_to_check:
        if not element.isdigit():
             print 'Removing %s element in the list' % element
             list_to_check.remove(element)
             bool = False
    return bool, list_to_check

def exists_remote(host, path):
    """Test if a file exists at path on a host accessible with SSH."""
    #print pipes.quote(path)
    status = subprocess.call(['ssh', host, 'test -f {0}'.format(pipes.quote(path))])
    if status == 0:
        return True
    if status == 1:
        return False
    raise Exception('SSH failed')

def errorfile(run_number, error):
    errorfile_handle = open(basedir+"error_runs.txt", "a+") 
    errorfile_handle.write("Run Number: " + str(run_number)+ "..........Error in ")
    errorfile_handle.write(error + "\n")
    errorfile_handle.close()

basedir = '/home/daq/fnal_tb_18_11/' 
basedir_rulinux = '/data/TestBeam/2018_11_November_CMSTiming/'
timingDAQ_location = '/uscms/home/rheller/work/TimingDAQ/'
trackdir_local = '/home/daq/fnal_tb_18_11/Tracks'
#'/data/TestBeam/2018_11_November_CMSTiming/'

root -l ./DT5742Dat2Root --input_file=/home/daq/fnal_tb_18_11/DT5742Mount/DT5742_RAW_Run4665.dat --output_file=/home/daq/fnal_tb_18_11//LocalData/RECO/DT5742/v8/run_DT_4665.root --config_file=config/FNAL_TestBeam_1811/DT5742.config --save_meas --verbose  ^C


while(1):
                #list of all the run numbers                                                                                                                                                                                                 
                list_raw_to_check = [(x.split("_Run")[1].split(".dat")[0].split("_")[0]) for x in glob.glob(basedir+'DT5742Mount/*_Run*')]
                list_reco = [(x.split("_Run")[1].split(".root")[0].split("_")[0]) for x in glob.glob(basedir+'LocalData/RECO/DT5742/v8/*_Run*')]
                #list_track = [(x.split("_Run")[1].split(".root")[0].split("_")[0]) for x in glob.glob(basedir+'RECO/v7/*_Run*')] ### what is this for?

                #Check if the list is fine                                                                                                                                                                                                   
                bool, list_raw = check(list_raw_to_check)
                if(bool):
                    print '\n The Filenames in the list are fine. \n'
                else:
                    print '\n Filenames in the list are screwed up, not processing bad file names!!!!!!!!!!!!!! \n'
                time.sleep(5)

                #sets containing run numbers from raw folder and reco folder                                                                                                                                                        
                set_raw = set([int(x) for x in list_raw])
                set_reco = set([int(x) for x in list_reco])
                set_toprocess = set_raw - set_reco
                if len(set_toprocess) == 0:
                        print '\n ###################### NO RUNS TO PROCESS ####################### \n'
                        

                for x in set_toprocess:
                    #isnext_rawfile = x+1 in set_raw
                    bad_scp = False
                    if x >= int(sys.argv[1]):
                        #Check if the track is already there
                        print '############################ RUN NUMBER %i ############################' % x
                        trackrulinux_path = basedir_rulinux+'CMSTimingConverted/Run%i_CMSTiming_converted.root' % x
                        tracklocal_path = trackdir_local+'Run%i_CMSTiming_converted.root' % x
                        
                        if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', trackrulinux_path):
                            if os.path.exists(tracklocal_path):
                                print '\n Track file exist and is already present on daq-08, no need to do Tracking or copying \n' 
                            else:
                                print '\n Track file exist on rulinux but not on daq-08, copying Track file \n'
                                copy_cmd = 'scp otsdaq@rulinux04.dhcp.fnal.gov:' + trackrulinux_path + " " + trackdir_local
                                os.system(copy_cmd)
                            bad_scp = not os.path.exists(tracklocal_path)
                        else:
                            continue;
                            print '\n Doing Tracking \n'
                            #Doing the tracking for the run
                            session = subprocess.Popen(["ssh", "otsdaq@rulinux04.dhcp.fnal.gov", ". /home/otsdaq/CMSTiming/HyperScriptFastTrigger_NewGeo_18_12_11_meraj.sh %i" % x], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
                            stdout, stderr = session.communicate()
                            if stderr:
                                bad_scp = True
                                errorfile(x, "Tracking")
                                print '\n Tracking threw an error \n'
                                if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', basedir+'CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  not os.path.exists(tracklocal_path):
                                    print '\n The error was due to a problem in SCPing from RULINUX to timngdaq02 \n \n Not doing the Tracking now \n'
                                elif not  exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', basedir+'CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  not os.path.exists(tracklocal_path):
                                    print '\n The error was an intrinsic Tracking error \n'
                                    errorfile(x, "Intrinsic Tracking")
                                    
                            else:
                                print "\n Tracking didn't throw any error \n" 
                                #Checking if the track file now exists on rulinux    
                                if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', basedir+'CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  os.path.exists(tracklocal_path):
                                    print '\n Track file now exists on rulinux and local \n'

                        #Checking Track file size
                        if not bad_scp: 
                            print '\n Checking Tracking file size local \n'
                            statinfo = os.stat(tracklocal_path)
                            if statinfo.st_size < 10000:
                                print '\n The track file size is too small, tracking was bad: not doing the reco \n'
                                errorfile(x, "SMALL TRACKING FILE")
                            else:    
                                print '\n The track file size is fine, Doing the reco \n'
                                # print 'cd %s;source setup_cmslpc.sh;python automation/DecodeData.py --vVME v7 -R %i;cd -' % (timingDAQ_location,x)
                                # break
                                session2 = subprocess.Popen('cd %s;source setup_cmslpc.sh;python automation/DecodeData.py --vVME v7 -R %i;cd -' % (timingDAQ_location,x),stdout=PIPE, stderr=PIPE, shell=True)
                                stdout, stderr = session2.communicate()
                                print 'stderr is '
                                print stderr
                                print stdout
                                if str(stderr) == str("VMEDat2Root: app/VMEDat2Root.cc:22: int main(int, char**): Assertion `false' failed."):
                                    print '\n The reco was fine \n'
                                elif stderr and "glibc" in str(stderr):
                                    print '\n Seems like there is corruption. Try first 2k events only. \n'
                                    session2 = subprocess.Popen('rm /eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/DataVMETiming_Run%i.root;cd %s;source setup_cmslpc.sh;python automation/DecodeData.py --vVME v7 -R %i --N_evts=2000;cd -' % (x,timingDAQ_location,x),stdout=PIPE, stderr=PIPE, shell=True)

                                elif stderr and str(stderr) != "VMEDat2Root: app/VMEDat2Root.cc:22: int main(int, char**): Assertion `false' failed.":
                                    print '\n The reco threw some error \n'
                                    errorfile(x, "RECO" + str(stderr)) 
                        else:
                            print '\n There was an error in SCPing from RULINUX TO DAQ08 \n \n Not doing the Tracking now \n'
                            errorfile(x, "SCP from RULINUX TO DAQ08")
                        print '############################ DONE WITH RUN NUMBER %i ############################' % x
                        print '\n \n \n \n'
                print 'Going to sleep for 120 seconds...................'
                time.sleep(120)



















