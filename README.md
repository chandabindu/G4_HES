# G4_HES
Hypernuclear HES simulations

HES simulation code
 Registered by S.Nagao
Original files (E05-115 version) are provided by T.Gogami

CMakeLists.txt was prepared by okuyama, who has little experience in using it. (please fix me anytime!)

1. Preparation
Please make links. The following procedure is an example on farm machine.
   ln -s /data/42a/ELS/E12-15-008/G4_rootfile root
   ln -s /data/42a/ELS/E12-15-008/TOSCA_FieldMap/ param
   ln -s /data/42a/ELS/E12-15-008/G4_Seed/ seed

//okuyama
(ana/root , ./run/param , ./run/seed 
,but I don't think it is necessary to make a link of @G4_rootfile.)
 I mean...
  $ cd run/
  $ ln -s /data/42a/ELS/E12-15-008/TOSCA_FieldMap/ param
  $ ln -s /data/42a/ELS/E12-15-008/G4_Seed/ seed
  $ cd ../ana/
  $ mkdir root/
  $ cd ../

2. Get ready
   $ source /usr/local/cern/geant4.9.6.p04/bin/geant4.csh
  ($ mkdir build/)
   $ cd build/
   $ cmake3 ../

3. Compile (you are now under build/)
   $ make

4. Execute (you are now under build/)
w/ visualization
   $./HES ../run/macro/oku_test.mac ../run/input/oku_test.in
w/o visualization
   $./HES ../run/macro/gun.mac ../run/input/oku_test.in

(I recommend you make a shell script doing this.)



if you want to use GNUmakefile (original)
1. Preparation
2. Get ready
   $ source /usr/local/cern/geant4.9.6.p04/share/Geant4-9.6.4/geant4make/geant4.csh
   $ source /usr/local/cern/geant4.9.6.p04/bin/geant4.csh
3. Compile (you are now in the same directory as this README)
   $ make
4. Execute (you are now in the same directory as this README)
   $ ./krun_GNU.sh
