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
    errorfile_handle = open("/data/TestBeam/2018_11_November_CMSTiming/error_runs.txt", "a+") 
    errorfile_handle.write("Run Number: " + str(run_number)+ "..........Error in ")
    errorfile_handle.write(error + "\n")
    errorfile_handle.close()

while(1):
                #list of all the run numbers                                                                                                                                                                                                 
                list_raw_to_check = [(x.split("_Run")[1].split(".dat")[0].split("_")[0]) for x in glob.glob('/data/TestBeam/2018_11_November_CMSTiming/CMSTiming/*_Run*')]
                list_reco = [(x.split("_Run")[1].split(".root")[0].split("_")[0]) for x in glob.glob('/data/TestBeam/2018_11_November_CMSTiming/RECO/v7/*_Run*')]
                list_track = [(x.split("_Run")[1].split(".root")[0].split("_")[0]) for x in glob.glob('/data/TestBeam/2018_11_November_CMSTiming/RECO/v7/*_Run*')]

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
                        trackrulinux_path = '/data/TestBeam/2018_11_November_CMSTiming/CMSTimingConverted/Run%i_CMSTiming_converted.root' % x
                        trackdaq08_path = '/data/TestBeam/2018_11_November_CMSTiming/Tracks/Run%i_CMSTiming_converted.root' % x
                        if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', trackrulinux_path):
                            if os.path.exists(trackdaq08_path):
                                print '\n Track file exist and is already present on daq-08, no need to do Tracking or copying \n' 
                            else:
                                print '\n Track file exist on rulinux but not on daq-08, copying Track file \n'
                                copy_cmd = 'scp ' + trackrulinux_path + ' cmstiming@ftbf-daq-08.fnal.gov:/data/TestBeam/2018_11_November_CMSTiming/Tracks'
                                os.system(copy_cmd)
                            bad_scp = not os.path.exists(trackdaq08_path)
                        else:
                            print '\n Doing Tracking \n'
                            #Doing the tracking for the run
                            session = subprocess.Popen(["ssh", "otsdaq@rulinux04.dhcp.fnal.gov", ". /home/otsdaq/CMSTiming/HyperScriptFastTrigger_NewGeo_18_12_11.sh %i" % x], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
                            stdout, stderr = session.communicate()
                            if stderr:
                                bad_scp = True
                                errorfile(x, "Tracking")
                                print '\n Tracking threw an error \n'
                                if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', '/data/TestBeam/2018_11_November_CMSTiming/CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  not os.path.exists(trackdaq08_path):
                                    print '\n The error was due to a problem in SCPing from RULINUX TO DAQ08 \n \n Not doing the Tracking now \n'
                                elif not  exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', '/data/TestBeam/2018_11_November_CMSTiming/CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  not os.path.exists(trackdaq08_path):
                                    print '\n The error was an intrinsic Tracking error \n'
                                    errorfile(x, "Intrinsic Tracking")
                                    
                            else:
                                print "\n Tracking didn't throw any error \n" 
                                #Checking if the track file now exists on rulinux    
                                if exists_remote('otsdaq@rulinux04.dhcp.fnal.gov', '/data/TestBeam/2018_11_November_CMSTiming/CMSTimingConverted/Run%i_CMSTiming_converted.root' % x) and  os.path.exists(trackdaq08_path):
                                    print '\n Track file now exists on rulinux and daq08 \n'

                        #Checking Track file size
                        if not bad_scp: 
                            print '\n Checking Tracking file size on daq08 \n'
                            statinfo = os.stat(trackdaq08_path)
                            if statinfo.st_size < 10000:
                                print '\n The track file size is too small, tracking was bad: not doing the reco \n'
                                errorfile(x, "SMALL TRACKING FILE")
                            else:    
                                print '\n The track file size is fine, Doing the reco \n'
                                session2 = subprocess.Popen('cd /data/TestBeam/2018_11_November_CMSTiming/TimingDAQ/;source ~/cmstiming_setup_meraj.sh;python automation/DecodeData.py --vVME v7 -R %i;cd -' % x,stdout=PIPE, stderr=PIPE, shell=True)
                                stdout, stderr = session2.communicate()
                                if str(stderr) == str("VMEDat2Root: app/VMEDat2Root.cc:22: int main(int, char**): Assertion `false' failed."):
                                    print '\n The reco was fine \n'
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



















