#!/usr/bin/python
import os

#DATA ID
#20220111_21x21
DATA_ID = ""
#DATA_ID = "20220304_2020"
def MkInput(Qmag):
	FILENAME="parFile.dat"
	#FILENAME="/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/par"+DATA_ID+"/parFile"+str(int(Qmag))+".dat"
	ROOTNAME="/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data"+DATA_ID+"/case"+str(int(Qmag))+".root"
	#ROOTNAME="/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/data/case"+str(int(Qmag))+".root"

	f=open(FILENAME,"w")
	f.write(ROOTNAME+"\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/mom_"+str(int(Qmag))+".dat\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/xpt_"+str(int(Qmag))+".dat\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/ypt_"+str(int(Qmag))+".dat\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/xt_"+str(int(Qmag))+".dat\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/yt_"+str(int(Qmag))+".dat\n") 
	f.write("/data/43a/ELS/okuyama/E12-15-008_wPCS/simulation/G4_HES/run/scan/temp/matrix"+DATA_ID+"/zt_"+str(int(Qmag))+".dat\n") 
	f.close()

Qmag=0
while Qmag<=441:
		MkInput(Qmag)
		command="./bin/getMatrix"
		os.system(command) 
		Qmag=Qmag+1
