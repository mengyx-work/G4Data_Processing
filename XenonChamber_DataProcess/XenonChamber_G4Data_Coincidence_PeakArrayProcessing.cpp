#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"

#include "THStack.h"
#include "TMath.h"
#include "TH2F.h"
#include "TLegend.h"

//#include "DrawWaveform.h"

//using namespace std;


/*
 //-------------------------------
August 26th 2013
 
1. Optimize for the G4Data from Xenon Chamber GEANT4 simulation
 
2. Add the feature to identify the multiple scattering withtin TPC LXe
 
3. The positions of multiple scattering are recorded in an array
 
4. Cross-check other modules
 
 //-------------------------------
 */



int main(int argc, char **argv)
//void test()
{

    
    if(argc!=2)
    {
        std::cout<<"ABORT!"<<std::endl;
        std::cout<<" the parameter order is incorrect! "<<std::endl;
        std::cout<<"./Executable File + ROOT File Name "<<std::endl;
        return 1;
    }
   
    gROOT->ProcessLine("#include <vector>");
    gROOT->ProcessLine("#include <string>");

    
    
    TFile          *f = new TFile(argv[1], "READ");
    TTree *_DataTTree = (TTree*)f->Get("G4Data");

    
    
    f->cd();
    
    //-----Start Accessing the ROOT Data------------
     
    int EventID;
    int InteractingVolumeInt;
    
    std::vector<int>            *ParentID     = new std::vector<int>;
    std::vector<int>            *TrackNbr     = new std::vector<int>;
    std::vector<int>             *StepNbr     = new std::vector<int>;
    std::vector<double>           *PositionX  = new std::vector<double>;
    std::vector<double>           *PositionY  = new std::vector<double>;
    std::vector<double>           *PositionZ  = new std::vector<double>;
    std::vector<double>           *DirectionX = new std::vector<double>;
    std::vector<double>           *DirectionY = new std::vector<double>;
    std::vector<double>           *DirectionZ = new std::vector<double>;
    std::vector<double>     *dDepositeEnergy  = new std::vector<double>;
    std::vector<double>          *dKinEnergy  = new std::vector<double>;
    std::vector<double>          *globalTime  = new std::vector<double>;
    std::vector<std::string>                *Particle = new std::vector<std::string>;
    std::vector<std::string>          *CreatorProcess = new std::vector<std::string>;
    std::vector<std::string>          *ParentParticle = new std::vector<std::string>;
    std::vector<std::string>          *DepositProcess = new std::vector<std::string>;
    std::vector<std::string>          *PhysicalVolume = new std::vector<std::string>; 
    
    
    _DataTTree->SetBranchAddress("EventID",                                                     &EventID);
    _DataTTree->SetBranchAddress("InteractingVolumeInt",                           &InteractingVolumeInt);
    _DataTTree->SetBranchAddress("ParentID",                                                   &ParentID);
    _DataTTree->SetBranchAddress("TrackNbr",                                                   &TrackNbr);
    _DataTTree->SetBranchAddress("StepNbr",                                                     &StepNbr);
    _DataTTree->SetBranchAddress("PositionX",                                                 &PositionX);
    _DataTTree->SetBranchAddress("PositionY",                                                 &PositionY);
    _DataTTree->SetBranchAddress("PositionZ",                                                 &PositionZ);
    _DataTTree->SetBranchAddress("DirectionX",                                               &DirectionX);
    _DataTTree->SetBranchAddress("DirectionY",                                               &DirectionY);
    _DataTTree->SetBranchAddress("DirectionZ",                                               &DirectionZ);
    _DataTTree->SetBranchAddress("dDepositeEnergy",                                     &dDepositeEnergy);
    _DataTTree->SetBranchAddress("dKinEnergy",                                               &dKinEnergy);
    _DataTTree->SetBranchAddress("globalTime",                                               &globalTime);
    _DataTTree->SetBranchAddress("Particle",                                                   &Particle);
    _DataTTree->SetBranchAddress("ParentParticle",                                       &ParentParticle);
    _DataTTree->SetBranchAddress("CreatorProcess",                                       &CreatorProcess);
    _DataTTree->SetBranchAddress("DepositProcess",                                       &DepositProcess);
    _DataTTree->SetBranchAddress("PhysicalVolume",                                       &PhysicalVolume);
    
 
    //---------------------------------------------------------------------- 
    
    
    
    
    
    
    
    //---------------------------------------------------------------------- 
    
    std::string ResultFileName = argv[1];
    
    int TotSize = ResultFileName.size();
    ResultFileName.erase(TotSize-5, 5);
    //ResultFileName.append("_LAr-0-5keVTrigger_EJ301-50keVTrigger.root");
    ResultFileName.append("_Result.root");
    
    TFile  *dataFile = new TFile(ResultFileName.c_str(), "RECREATE");
    
    //dataFile->cd();
 	TTree     *tData = new TTree("tData", "Peak Array from GEANT4");   
		
    
    std::vector<float>    *fDetector1_PeakStartTime    = new std::vector<float>;
    std::vector<float>    *fDetector1_PeakEndTime      = new std::vector<float>;
    std::vector<float>    *fDetector1_MaxPeakEnergy    = new std::vector<float>;
    std::vector<float>    *fDetector1_TotPeakEnergy    = new std::vector<float>;
    std::vector<float>    *fDetector1_MaxPeakTime      = new std::vector<float>;
    std::vector<float>    *fDetector1_PeakDirectionX   = new std::vector<float>;
    std::vector<float>    *fDetector1_PeakDirectionY   = new std::vector<float>;
    std::vector<float>    *fDetector1_PeakDirectionZ   = new std::vector<float>;
    std::vector<float>    *fDetector1_MaxPeakPositionX = new std::vector<float>;
    std::vector<float>    *fDetector1_MaxPeakPositionY = new std::vector<float>;
    std::vector<float>    *fDetector1_MaxPeakPositionZ = new std::vector<float>;
   
    std::vector<float>    *fDetector1_MultiScat_PositionX = new std::vector<float>;
    std::vector<float>    *fDetector1_MultiScat_PositionY = new std::vector<float>;
    std::vector<float>    *fDetector1_MultiScat_PositionZ = new std::vector<float>;

    
    std::vector<std::string>    *sDetector1_PeakCreatorParticle = new std::vector<std::string>;
    std::vector<std::string>    *sDetector1_PeakCreatorProcess  = new std::vector<std::string>;
    std::vector<std::string>    *sDetector1_PeakParentParticle  = new std::vector<std::string>;

    
    
    Int_t           iDetector1_MultipleScattering   = 0;
    Int_t           iDetector1_NonElasticScattering = 0;
    Int_t           iDetector1_PeakNbr              = 0;
    Float_t         fDetector1_TriggerEnergy        = 0;
    Float_t         fDetector1_TriggerTime          = 0;
    Float_t         fDetector1_TriggerPositionX     = -999;
    Float_t         fDetector1_TriggerPositionY     = -999;
    Float_t         fDetector1_TriggerPositionZ     = -999;
    Float_t         fDetector1_TriggerDirectionX    = 0;
    Float_t         fDetector1_TriggerDirectionY    = 0;
    Float_t         fDetector1_TriggerDirectionZ    = 0;

    Float_t         fDetector1_TotEneDeposit = 0;
    
    char     sDetector1_TriggerParticle[30]       = "";
    char     sDetector1_TriggerProcess[30]        = "";
    char     sDetector1_TriggerParentParticle[30] = "";
    
    
    Int_t           iInteractingVolume;
    
    
    tData->Branch("iInteractingVolume",                         &iInteractingVolume);
    
    tData->Branch("fDetector1_PeakStartTime",              &fDetector1_PeakStartTime);
	tData->Branch("fDetector1_PeakEndTime",                  &fDetector1_PeakEndTime);
    tData->Branch("fDetector1_MaxPeakEnergy",              &fDetector1_MaxPeakEnergy);
    tData->Branch("fDetector1_TotPeakEnergy",              &fDetector1_TotPeakEnergy);
	tData->Branch("fDetector1_MaxPeakTime",                  &fDetector1_MaxPeakTime);
    tData->Branch("fDetector1_MaxPeakPositionX",        &fDetector1_MaxPeakPositionX);
	tData->Branch("fDetector1_MaxPeakPositionY",        &fDetector1_MaxPeakPositionY);
    tData->Branch("fDetector1_MaxPeakPositionZ",        &fDetector1_MaxPeakPositionZ);
    
    tData->Branch("fDetector1_MultiScat_PositionX",        &fDetector1_MultiScat_PositionX);
	tData->Branch("fDetector1_MultiScat_PositionY",        &fDetector1_MultiScat_PositionY);
    tData->Branch("fDetector1_MultiScat_PositionZ",        &fDetector1_MultiScat_PositionZ);

    
    tData->Branch("fDetector1_PeakDirectionX",          &fDetector1_PeakDirectionX);
	tData->Branch("fDetector1_PeakDirectionY",          &fDetector1_PeakDirectionY);
    tData->Branch("fDetector1_PeakDirectionZ",          &fDetector1_PeakDirectionZ);
    tData->Branch("sDetector1_PeakCreatorParticle",     &sDetector1_PeakCreatorParticle);
    tData->Branch("sDetector1_PeakParentParticle",      &sDetector1_PeakParentParticle);
    tData->Branch("sDetector1_PeakCreatorProcess",      &sDetector1_PeakCreatorProcess);

    
    tData->Branch("iDetector1_MultipleScattering",          &iDetector1_MultipleScattering,               "iDetector1_MultipleScattering/I");
    tData->Branch("iDetector1_NonElasticScattering",        &iDetector1_NonElasticScattering,           "iDetector1_NonElasticScattering/I");
    tData->Branch("iDetector1_PeakNbr",                     &iDetector1_PeakNbr,                          "iDetector1_PeakNbr/I");
    tData->Branch("fDetector1_TriggerEnergy",               &fDetector1_TriggerEnergy,                    "fDetector1_TriggerEnergy/F");
    tData->Branch("fDetector1_TotEneDeposit",               &fDetector1_TotEneDeposit,                    "fDetector1_TotEneDeposit/F");
    tData->Branch("fDetector1_TriggerTime",                 &fDetector1_TriggerTime,                      "fDetector1_TriggerTime/F");
    tData->Branch("fDetector1_TriggerPositionX",            &fDetector1_TriggerPositionX,                 "fDetector1_TriggerPositionX/F");
    tData->Branch("fDetector1_TriggerPositionY",            &fDetector1_TriggerPositionY,                 "fDetector1_TriggerPositionY/F");
    tData->Branch("fDetector1_TriggerPositionZ",            &fDetector1_TriggerPositionZ,                 "fDetector1_TriggerPositionZ/F");
    tData->Branch("fDetector1_TriggerDirectionX",           &fDetector1_TriggerDirectionX,                "fDetector1_TriggerDirectionX/F");
    tData->Branch("fDetector1_TriggerDirectionY",           &fDetector1_TriggerDirectionY,                "fDetector1_TriggerDirectionY/F");
    tData->Branch("fDetector1_TriggerDirectionZ",           &fDetector1_TriggerDirectionZ,                "fDetector1_TriggerDirectionZ/F");
    tData->Branch("sDetector1_TriggerParticle",             &sDetector1_TriggerParticle,                  "sDetector1_TriggerParticle/C");
    tData->Branch("sDetector1_TriggerParentParticle",       &sDetector1_TriggerParentParticle,            "sDetector1_TriggerParentParticle/C");
    tData->Branch("sDetector1_TriggerProcess",              &sDetector1_TriggerProcess,                   "sDetector1_TriggerProcess/C");



    //-------------------------
    
    std::vector<float>    *fDetector2_PeakStartTime    = new std::vector<float>;  
    std::vector<float>    *fDetector2_PeakEndTime      = new std::vector<float>;
    std::vector<float>    *fDetector2_MaxPeakEnergy    = new std::vector<float>;
    std::vector<float>    *fDetector2_TotPeakEnergy    = new std::vector<float>;
    std::vector<float>    *fDetector2_MaxPeakTime      = new std::vector<float>;
    std::vector<float>    *fDetector2_MaxPeakPositionX = new std::vector<float>;
    std::vector<float>    *fDetector2_MaxPeakPositionY = new std::vector<float>;
    std::vector<float>    *fDetector2_MaxPeakPositionZ = new std::vector<float>;
    
    std::vector<std::string>    *sDetector2_PeakCreatorParticle = new std::vector<std::string>;
    std::vector<std::string>    *sDetector2_PeakCreatorProcess  = new std::vector<std::string>;
    std::vector<std::string>    *sDetector2_PeakParentParticle  = new std::vector<std::string>;
    std::vector<std::string>    *sDetector2_PhysicalVolume      = new std::vector<std::string>;

    
    Int_t           iDetector2_PeakNbr = 0;
    Float_t         fDetector2_TriggerEnergy = 0;
    Float_t         fDetector2_TriggerTime = 0;
    Float_t         fDetector2_TriggerPositionX = -999;
    Float_t         fDetector2_TriggerPositionY = -999;
    Float_t         fDetector2_TriggerPositionZ = -999;
    Float_t         fDetector2_TotEneDeposit = 0;
    
    char            sDetector2_TriggerParticle[30] = "";
    char            sDetector2_TriggerProcess[30] = "";
    char            sDetector2_TriggerParentParticle[30] = "";

    
    
    tData->Branch("fDetector2_PeakStartTime",         "vector<float>",               &fDetector2_PeakStartTime);
    tData->Branch("fDetector2_PeakEndTime",           "vector<float>",               &fDetector2_PeakEndTime);
    tData->Branch("fDetector2_MaxPeakEnergy",         "vector<float>",               &fDetector2_MaxPeakEnergy);
    tData->Branch("fDetector2_TotPeakEnergy",         "vector<float>",               &fDetector2_TotPeakEnergy);
    tData->Branch("fDetector2_MaxPeakTime",           "vector<float>",               &fDetector2_MaxPeakTime) ;
    tData->Branch("fDetector2_MaxPeakPositionX",      "vector<float>",               &fDetector2_MaxPeakPositionX);
    tData->Branch("fDetector2_MaxPeakPositionY",      "vector<float>",               &fDetector2_MaxPeakPositionY);
    tData->Branch("fDetector2_MaxPeakPositionZ",      "vector<float>",               &fDetector2_MaxPeakPositionZ);
    
    tData->Branch("sDetector2_PeakCreatorParticle",   "std::vector<std::string>",    &sDetector2_PeakCreatorParticle);
    tData->Branch("sDetector2_PeakCreatorProcess",    "std::vector<std::string>",    &sDetector2_PeakCreatorProcess);
    tData->Branch("sDetector2_PeakParentParticle",    "std::vector<std::string>",    &sDetector2_PeakParentParticle);
    tData->Branch("sDetector2_PhysicalVolume",        "std::vector<std::string>",    &sDetector2_PhysicalVolume);
  
    
    tData->Branch("iDetector2_PeakNbr",                     &iDetector2_PeakNbr,                    "iDetector2_PeakNbr/I");
    tData->Branch("fDetector2_TriggerEnergy",               &fDetector2_TriggerEnergy,              "fDetector2_TriggerEnergy/F");
    tData->Branch("fDetector2_TotEneDeposit",               &fDetector2_TotEneDeposit,              "fDetector2_TotEneDeposit/F");
    tData->Branch("fDetector2_TriggerTime",                 &fDetector2_TriggerTime,                "fDetector2_TriggerTime/F");
    tData->Branch("fDetector2_TriggerPositionX",            &fDetector2_TriggerPositionX,           "fDetector2_TriggerPositionX/F");
    tData->Branch("fDetector2_TriggerPositionY",            &fDetector2_TriggerPositionY,           "fDetector2_TriggerPositionY/F");
    tData->Branch("fDetector2_TriggerPositionZ",            &fDetector2_TriggerPositionZ,           "fDetector2_TriggerPositionZ/F");
    tData->Branch("sDetector2_TriggerParticle",             &sDetector2_TriggerParticle,            "sDetector2_TriggerParticle/C");
    tData->Branch("sDetector2_TriggerProcess",              &sDetector2_TriggerProcess,             "sDetector2_TriggerProcess/C");
    tData->Branch("sDetector2_TriggerParentParticle",       &sDetector2_TriggerParentParticle,      "sDetector2_TriggerParentParticle/C");
     
 

    
//----------------------------------------------------------------------
    
    

    
    
    
    
//----------------------------------------------------------------------    
    
    Int_t TotG4RawEventNbr = (Int_t)_DataTTree->GetEntries();
    

    Float_t       Detector1_PeakThreshold = 0.1;      //--- Unit: keV ---
    Float_t       Detector2_PeakThreshold = 0.1;      //--- Unit: keV ---
    
    Float_t       Detector1_TriggerThreshold = 0.5;     //--- Unit: keV ---
    Float_t       Detector2_TriggerThreshold = 50;     //--- Unit: keV ---
    
    Float_t       Detector1_DigitalTimeResolution = 10;  //---Unit: ns ---
    Float_t       Detector2_DigitalTimeResolution = 20; //---Unit: ns ---

    
    Float_t       DecayChainTimeLimit = 1e4;  //--- Unit: ns ---
     
    
//----------------------------------------------------------------------
    
    
    float   Detector2_PeakStartTime = 0;
    float   Detector2_PeakEndTime = 0;
    float   Detector2_MaxPeakEnergy = 0;
    float   Detector2_TotPeakEnergy = 0;
    float   Detector2_MaxPeakTime = 0;
    float   Detector2_MaxPeakPositionX = 0;
    float   Detector2_MaxPeakPositionY = 0;
    float   Detector2_MaxPeakPositionZ = 0;
  
    std::string Detector2_CreatorParticle  = "Null";
    std::string Detector2_CreatorProcess   = "Null";
    std::string Detector2_ParentPartcile   = "Null";
    std::string Detector2_PhysicalVolume   = "Null";

 
    
    Int_t         Detector2_PeakNbr = 0;
    Float_t       Detector2_TriggerEnergy = 0;
    Float_t       Detector2_TriggerTime = 0;
    Float_t       Detector2_TriggerPositionX = -999;
    Float_t       Detector2_TriggerPositionY = -999;
    Float_t       Detector2_TriggerPositionZ = -999;
    Float_t       Detector2_TotEneDeposit = 0;
    std::string   Detector2_TriggerParticle       = "Null";
    std::string   Detector2_TriggerParentParticle = "Null";
    std::string   Detector2_TriggerProcess        = "NUll";
    

    
    
    
    float   Detector1_PeakStartTime = 0;
    float   Detector1_PeakEndTime = 0;
    float   Detector1_MaxPeakEnergy = 0;
    float   Detector1_TotPeakEnergy = 0;
    float   Detector1_MaxPeakTime = 0;
    float   Detector1_MaxPeakPositionX = 0;
    float   Detector1_MaxPeakPositionY = 0;
    float   Detector1_MaxPeakPositionZ = 0;
    float   Detector1_PeakDirectionX = 0;
    float   Detector1_PeakDirectionY = 0;
    float   Detector1_PeakDirectionZ = 0;
    
    
    std::string Detector1_CreatorParticle = "Null";
    std::string Detector1_CreatorProcess  = "Null";
    std::string Detector1_ParentPartcile  = "Null";
    
    
    Int_t         Detector1_MultipleScattering   = 0;
    Int_t         Detector1_NonElasticScattering = 0;

    Int_t         Detector1_PeakNbr = 0;
    Float_t       Detector1_TriggerEnergy = 0;
    Float_t       Detector1_TriggerTime = 0;
    Float_t       Detector1_TriggerPositionX = -999;
    Float_t       Detector1_TriggerPositionY = -999;
    Float_t       Detector1_TriggerPositionZ = -999;
    Float_t       Detector1_TriggerDirectionX  = 0;
    Float_t       Detector1_TriggerDirectionY  = 0;
    Float_t       Detector1_TriggerDirectionZ  = 0;
    Float_t       Detector1_TotEneDeposit = 0;
    std::string   Detector1_TriggerParticle       = "Null";
    std::string   Detector1_TriggerProcess        = "NUll";
    std::string   Detector1_TriggerParentParticle = "Null";
    
    
      


    
    /*
    //^^^^^ open the file stream ^^^^^^^^^
     
         int width = 10, precision = 4;
     
       //ofstream StepInfo_out("StepInformation_100keVEvent.txt");
        ofstream outResult("Output_SceneG4Steps.txt", std::ios::app);
    // vector<float> *time = new vector<float>;
    // vector<float> *count = new vector<float>;
    

    //std::cout<<"Total Entries#: "<<TotG4RawEventNbr<<std::endl;
 
  
    //============loop the entire events set==================
    
    //for (Int_t k=0; k<TotG4RawEventNbr; k++) 
    for (Int_t k=0; k<3000; k++) 
    {
              _DataTTree->GetEntry(k);
        
                const int TotStepNbr = ParentID->size();
        
                int width = 10, precision = 4;
        
        //std::cout<<"Event# "<<k<<std::endl;
        
        
        
        //if( (EventID==9155561)||(EventID==930443)||(EventID==9624047)||(EventID==9730793)||(EventID==9789483) )
        //if( (EventID==5009530)&&(TotStepNbr==222) )
        //{  
                for(unsigned int i=0; i<TotStepNbr; i++)
                {
                    //std::cout<<globalTime->at(i)<<std::endl;
                    
                    if( (PositionY->at(i)<Detector1_MaxPositionY)&&(PositionY->at(i)>Detector1_MinPositionY)&&(PositionX->at(i)<Detector1_MaxPositionX)&&(PositionX->at(i)>Detector1_MinPositionX) )
                    {
                        outResult<<std::setw(2)
                        <<EventID<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<ParentID->at(i)<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<TrackNbr->at(i)<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<StepNbr->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<PositionX->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<PositionY->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<PositionZ->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<dKinEnergy->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<dDepositeEnergy->at(i)<<"    "<<std::setw(width)<<std::fixed<<std::setprecision(precision)
                        <<globalTime->at(i)<<"    "<<std::setw(width)
                        <<Particle->at(i)<<"    "<<std::setw(width)	
                        <<CreatorProcess->at(i)<<"    "<<std::setw(width)
                        <<DepositProcess->at(i)<<"    "<<std::setw(width)
                        <<InteractingVolumeInt<<"    "<<std::setw(width)
                        <<std::endl;
                    }

                }
        
        //}  
    }
    */
    
    
    for (Int_t k=0; k<TotG4RawEventNbr; k++)
{
    
        _DataTTree->GetEntry(k);
  
    const int TotStepNbr = ParentID->size();
    
    //-------Integral for Interacting Volume--------
      iInteractingVolume = InteractingVolumeInt;
    
    
    
     //std::cout<<"For Event# "<<k<<"  The TotalSept# is: "<<TotStepNbr<<std::endl;
    
    
    //---------Reset all parameters ------------------ 
    
    Detector2_PeakStartTime = 0;
    Detector2_PeakEndTime = 0;
    Detector2_MaxPeakEnergy = 0;
    Detector2_TotPeakEnergy = 0;
    Detector2_MaxPeakTime = 0;
    Detector2_MaxPeakPositionX = 0;
    Detector2_MaxPeakPositionY = 0;
    Detector2_MaxPeakPositionZ = 0;
    
    Detector2_CreatorParticle.clear();
    Detector2_CreatorProcess.clear();
    Detector2_ParentPartcile.clear();
    Detector2_PhysicalVolume.clear();
    
    Detector2_CreatorParticle = "Null";
    Detector2_CreatorProcess  = "Null";
    Detector2_ParentPartcile  = "Null";
    Detector2_PhysicalVolume  = "Null";
    
    Detector1_PeakStartTime = 0;
    Detector1_PeakEndTime = 0;
    Detector1_MaxPeakEnergy = 0;
    Detector1_TotPeakEnergy = 0;
    Detector1_MaxPeakTime = 0;
    Detector1_MaxPeakPositionX = 0;
    Detector1_MaxPeakPositionY = 0;
    Detector1_MaxPeakPositionZ = 0;
    Detector1_CreatorParticle.clear();
    Detector1_CreatorProcess.clear();
    Detector1_ParentPartcile.clear();
    Detector1_CreatorParticle = "Null";
    Detector1_CreatorProcess  = "Null";
    Detector1_ParentPartcile  = "Null";
    
    Detector1_MultipleScattering   = 0;
    Detector1_NonElasticScattering = 0;
    Detector1_PeakNbr = 0;
    Detector1_TriggerEnergy = 0;
    Detector1_TriggerTime = 0;
    Detector1_TriggerPositionX = -999;
    Detector1_TriggerPositionY = -999;
    Detector1_TriggerPositionZ = -999;
    Detector1_TriggerDirectionX  = 0;
    Detector1_TriggerDirectionY  = 0;
    Detector1_TriggerDirectionZ  = 0;
    Detector1_TotEneDeposit = 0;
    
    
    Detector2_PeakNbr = 0;
    Detector2_TriggerEnergy = 0;
    Detector2_TriggerTime = 0;
    Detector2_TriggerPositionX = -999;
    Detector2_TriggerPositionY = -999;
    Detector2_TriggerPositionZ = -999;
    Detector2_TotEneDeposit = 0;
    
    
    fDetector1_PeakStartTime->clear();
    fDetector1_PeakEndTime->clear();
    fDetector1_MaxPeakEnergy->clear();
    fDetector1_TotPeakEnergy->clear();
    fDetector1_MaxPeakTime->clear();
    fDetector1_MaxPeakPositionX->clear();
    fDetector1_MaxPeakPositionY->clear();
    fDetector1_MaxPeakPositionZ->clear();
    
    fDetector1_MultiScat_PositionX->clear();
    fDetector1_MultiScat_PositionY->clear();
    fDetector1_MultiScat_PositionZ->clear();
    
    fDetector1_PeakDirectionX->clear();
    fDetector1_PeakDirectionY->clear();
    fDetector1_PeakDirectionZ->clear();
    sDetector1_PeakCreatorParticle->clear();
    sDetector1_PeakCreatorProcess->clear();
    sDetector1_PeakParentParticle->clear();
    
    
    fDetector2_PeakStartTime->clear();
    fDetector2_PeakEndTime->clear();
    fDetector2_MaxPeakEnergy->clear();
    fDetector2_TotPeakEnergy->clear();
    fDetector2_MaxPeakTime->clear();
    fDetector2_MaxPeakPositionX->clear();
    fDetector2_MaxPeakPositionY->clear();
    fDetector2_MaxPeakPositionZ->clear();
    sDetector2_PeakCreatorParticle->clear();
    sDetector2_PeakCreatorProcess->clear();
    sDetector2_PeakParentParticle->clear();
    sDetector2_PhysicalVolume->clear();
    
    
    
    bool    Detector1_PeakCollecting = false;
    bool    Detector2_PeakCollecting = false;

    
    
    for(int i=0; i<TotStepNbr; i++)
    {
        
            
            //if( (PositionY->at(i)<Detector1_MaxPositionY)&&(PositionY->at(i)>Detector1_MinPositionY)&&(PositionX->at(i)<Detector1_MaxPositionX)&&(PositionX->at(i)>Detector1_MinPositionX)&&(globalTime->at(i)<DecayChainTimeLimit) ) //-------- To check whether in LXe active volume ----------
        
        std::string tempPhysicalVolume =  PhysicalVolume->at(i); //---- name of physical volume -----
        
        if( (tempPhysicalVolume==std::string("ActiveGXe_Physical"))||(tempPhysicalVolume==std::string("ActiveLXe_Physical")) ) //---- Using physical volume name to Identify the Volume, GXe Name: ActiveGXe_Physical -----
            {
                
                
                //---------- Count the multiple scattering in Active TPC -----------
                std::string tempCreatorProcess =  CreatorProcess->at(i);

                if( (tempCreatorProcess==std::string("HadronElastic"))&&(dDepositeEnergy->at(i)>0) ){
                    
                    Detector1_MultipleScattering++;
                    
                    fDetector1_MultiScat_PositionX->push_back(PositionX->at(i));
                    fDetector1_MultiScat_PositionY->push_back(PositionY->at(i));
                    fDetector1_MultiScat_PositionZ->push_back(PositionZ->at(i));
                    
                }
                //------------------------------------------------------------------


                
                //-------- Count the NonElastic Scattering in Active TPC -----------

                
                
                if( (tempCreatorProcess!=std::string("HadronElastic"))&&(dDepositeEnergy->at(i)>0) )
                    Detector1_NonElasticScattering = 1;
                
                //------------------------------------------------------------------

                
                
                //%%%%%Check Step above PeakThreshold%%%%%
                  if(dDepositeEnergy->at(i)>Detector1_PeakThreshold)
                  {
                      
                                            
                              if (Detector1_PeakCollecting==false) 
                              {
                                  Detector1_PeakCollecting = true;
                                   
                                  
                                  Detector1_PeakStartTime = globalTime->at(i);
                                    Detector1_PeakEndTime = globalTime->at(i);
                                    Detector1_MaxPeakTime = globalTime->at(i);
                                  
                                  Detector1_TotPeakEnergy = dDepositeEnergy->at(i);
                                  Detector1_MaxPeakEnergy = dDepositeEnergy->at(i);
                                  
                                  
                                  Detector1_MaxPeakPositionX = PositionX->at(i);
                                  Detector1_MaxPeakPositionY = PositionY->at(i);
                                  Detector1_MaxPeakPositionZ = PositionZ->at(i);
                                  
                                  Detector1_PeakDirectionX= DirectionX->at(i);
                                  Detector1_PeakDirectionY= DirectionY->at(i);
                                  Detector1_PeakDirectionZ= DirectionZ->at(i);
                                  
                                  Detector1_CreatorProcess  = CreatorProcess->at(i);
                                  Detector1_CreatorParticle = Particle->at(i);
                                  Detector1_ParentPartcile  = ParentParticle->at(i);

                              }
                              else //--PeakCollecting==true--
                              {
                                           if ( (Detector1_PeakEndTime+Detector1_DigitalTimeResolution) < globalTime->at(i) ) //--in case a seperate peak--
                                           {
                                               //--a new/seperate peak is found, push_back to save the previous peak information--
                                               fDetector1_PeakStartTime->push_back(Detector1_PeakStartTime);
                                               fDetector1_PeakEndTime->push_back(Detector1_PeakEndTime);
                                               fDetector1_MaxPeakTime->push_back(Detector1_MaxPeakTime);
                                               
                                               fDetector1_MaxPeakEnergy->push_back(Detector1_MaxPeakEnergy);
                                               fDetector1_TotPeakEnergy->push_back(Detector1_TotPeakEnergy);
                                               
                                               fDetector1_MaxPeakPositionX->push_back(Detector1_MaxPeakPositionX);
                                               fDetector1_MaxPeakPositionY->push_back(Detector1_MaxPeakPositionY);
                                               fDetector1_MaxPeakPositionZ->push_back(Detector1_MaxPeakPositionZ);
                                               
                                               fDetector1_PeakDirectionX->push_back(Detector1_PeakDirectionX);
                                               fDetector1_PeakDirectionY->push_back(Detector1_PeakDirectionY);
                                               fDetector1_PeakDirectionZ->push_back(Detector1_PeakDirectionZ);

                                               
                                               sDetector1_PeakCreatorParticle->push_back(Detector1_CreatorParticle);
                                               sDetector1_PeakCreatorProcess->push_back(Detector1_CreatorProcess);
                                               sDetector1_PeakParentParticle->push_back(Detector1_ParentPartcile); 
                                               
                                             //  cout<<"for the step "<<StepNumber<<", the Time Resolution exceeded! with GlobalTime: "<<globalTime->at(i)<<" with the PeakEndTime  "<<Detector1_PeakEndTime<<endl;
                                               
                                               
                                               //--new peak information is saved to variable--
                                               Detector1_PeakCollecting = true;
                                             
                                               
                                               Detector1_PeakStartTime = globalTime->at(i);
                                               Detector1_PeakEndTime = globalTime->at(i);
                                               Detector1_MaxPeakTime = globalTime->at(i);
                                               
                                               Detector1_TotPeakEnergy = dDepositeEnergy->at(i);
                                               Detector1_MaxPeakEnergy = dDepositeEnergy->at(i);
                                               
                                               Detector1_MaxPeakPositionX = PositionX->at(i);
                                               Detector1_MaxPeakPositionY = PositionY->at(i);
                                               Detector1_MaxPeakPositionZ = PositionZ->at(i);
     
                                               Detector1_PeakDirectionX= DirectionX->at(i);
                                               Detector1_PeakDirectionY= DirectionY->at(i);
                                               Detector1_PeakDirectionZ= DirectionZ->at(i);

                                               
                                               Detector1_CreatorProcess  = CreatorProcess->at(i);
                                               Detector1_CreatorParticle = Particle->at(i);
                                               Detector1_ParentPartcile  = ParentParticle->at(i);


                                           }
                                           else //-- a continuous step of one peak when (Detector1_PeakStartTime+Detector1_DigitalTimeResolution) > GlobalTime--
                                           {
                                               Detector1_TotPeakEnergy = Detector1_TotPeakEnergy + dDepositeEnergy->at(i);
                                                 Detector1_PeakEndTime = globalTime->at(i);
                                               
                                                        if(dDepositeEnergy->at(i)>Detector1_MaxPeakEnergy)
                                                        {   
                                                            Detector1_MaxPeakEnergy = dDepositeEnergy->at(i);
                                                            Detector1_MaxPeakTime = globalTime->at(i);
                                                            Detector1_MaxPeakPositionX = PositionX->at(i);
                                                            Detector1_MaxPeakPositionY = PositionY->at(i);
                                                            Detector1_MaxPeakPositionZ = PositionZ->at(i);

                                                        }
                                               
                                           }
                              }          
                  }
                
                
                  //%%%%%Check Step beneath PeakThreshold%%%%%%%
                 if(dDepositeEnergy->at(i)<Detector1_PeakThreshold)
                 {
                         if( (Detector1_PeakCollecting==true)&&( (Detector1_PeakEndTime+Detector1_DigitalTimeResolution)<globalTime->at(i) ) ) 
                         {
                              Detector1_PeakCollecting = false;
                        
                              //--push_back to save the previous peak information--
                             fDetector1_PeakStartTime->push_back(Detector1_PeakStartTime);
                             fDetector1_PeakEndTime->push_back(Detector1_PeakEndTime);
                             fDetector1_MaxPeakTime->push_back(Detector1_MaxPeakTime);
                             
                             fDetector1_MaxPeakEnergy->push_back(Detector1_MaxPeakEnergy);
                             fDetector1_TotPeakEnergy->push_back(Detector1_TotPeakEnergy);
                             
                             fDetector1_MaxPeakPositionX->push_back(Detector1_MaxPeakPositionX);
                             fDetector1_MaxPeakPositionY->push_back(Detector1_MaxPeakPositionY);
                             fDetector1_MaxPeakPositionZ->push_back(Detector1_MaxPeakPositionZ);
                             
                             fDetector1_PeakDirectionX->push_back(Detector1_PeakDirectionX);
                             fDetector1_PeakDirectionY->push_back(Detector1_PeakDirectionY);
                             fDetector1_PeakDirectionZ->push_back(Detector1_PeakDirectionZ);

                             sDetector1_PeakCreatorParticle->push_back(Detector1_CreatorParticle);
                             sDetector1_PeakCreatorProcess->push_back(Detector1_CreatorProcess);
                             sDetector1_PeakParentParticle->push_back(Detector1_ParentPartcile); 

                            // cout<<"for the step "<<StepNumber<<", the energy is below threshold with DepositEnergy: "<<DepositEnergy<<endl;
                        }
                      //---else case that not PeakCollecting and not reach the PeakThreshold, no action for this----
              //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

                }
                
            
            }
           //---End of Steps in Detector1 --------
            
            
            
            //------------------------------ Step in Detector#2 --------------------------------------------------
        
        else if( (globalTime->at(i)<DecayChainTimeLimit)&&(tempPhysicalVolume.compare(0, 5, "EJ301")==0) ) //---- Using Volume Name to Identify the Hits -----
        {
            
            
                //%%%%%Check Step above PeakThreshold%%%%%
                if(dDepositeEnergy->at(i)>Detector2_PeakThreshold)
                {
                            if (Detector2_PeakCollecting==false) 
                            {
                                        Detector2_PeakCollecting = true;
                                                               
                                        Detector2_PeakStartTime = globalTime->at(i);
                                        Detector2_PeakEndTime = globalTime->at(i);
                                        Detector2_MaxPeakTime = globalTime->at(i);
                        
                                        Detector2_TotPeakEnergy = dDepositeEnergy->at(i);
                                        Detector2_MaxPeakEnergy = dDepositeEnergy->at(i);
                   
                                        Detector2_MaxPeakPositionX = PositionX->at(i);
                                        Detector2_MaxPeakPositionY = PositionY->at(i);
                                        Detector2_MaxPeakPositionZ = PositionZ->at(i);
                                
                                        Detector2_CreatorProcess  = CreatorProcess->at(i);
                                        Detector2_CreatorParticle = Particle->at(i);
                                        Detector2_ParentPartcile  = ParentParticle->at(i);
                                        Detector2_PhysicalVolume  = PhysicalVolume->at(i);
                                
                            }
                            else //--PeakCollecting==true--
                            {
                                        if( (Detector2_PeakEndTime+Detector2_DigitalTimeResolution) < globalTime->at(i)||(PhysicalVolume->at(i)!=Detector2_PhysicalVolume) ) //--in case a seperate peak or in the other EJ301--
                                        {
                                                        //--a new/seperate peak is found, push_back to save the previous peak information--
                                                        fDetector2_PeakStartTime->push_back(Detector2_PeakStartTime);
                                                        fDetector2_PeakEndTime->push_back(Detector2_PeakEndTime);
                                                        fDetector2_MaxPeakTime->push_back(Detector2_MaxPeakTime);
                                
                                                        fDetector2_MaxPeakEnergy->push_back(Detector2_MaxPeakEnergy);
                                                        fDetector2_TotPeakEnergy->push_back(Detector2_TotPeakEnergy);
                                
                                                        fDetector2_MaxPeakPositionX->push_back(Detector2_MaxPeakPositionX);
                                                        fDetector2_MaxPeakPositionY->push_back(Detector2_MaxPeakPositionY);
                                                        fDetector2_MaxPeakPositionZ->push_back(Detector2_MaxPeakPositionZ);
                            
                                                        sDetector2_PeakCreatorParticle->push_back(Detector2_CreatorParticle);
                                                        sDetector2_PeakCreatorProcess->push_back(Detector2_CreatorProcess);
                                                        sDetector2_PeakParentParticle->push_back(Detector2_ParentPartcile);
                                                        sDetector2_PhysicalVolume->push_back(Detector2_PhysicalVolume);

                                            
                                                        //--new peak information is stored to variable--
                                                        Detector2_PeakCollecting = true;
                                
                                            
                                                        Detector2_PeakStartTime = globalTime->at(i);
                                                        Detector2_PeakEndTime = globalTime->at(i);
                                                        Detector2_MaxPeakTime = globalTime->at(i);
                                
                                                        Detector2_TotPeakEnergy = dDepositeEnergy->at(i);
                                                        Detector2_MaxPeakEnergy = dDepositeEnergy->at(i);
                                
                                                        Detector2_MaxPeakPositionX = PositionX->at(i);
                                                        Detector2_MaxPeakPositionY = PositionY->at(i);
                                                        Detector2_MaxPeakPositionZ = PositionZ->at(i);
                                            
                                                        Detector2_CreatorProcess  = CreatorProcess->at(i);
                                                        Detector2_CreatorParticle = Particle->at(i);
                                                        Detector2_ParentPartcile  = ParentParticle->at(i);
                                                        Detector2_PhysicalVolume  = PhysicalVolume->at(i);
                            
                                        }
                                        else //-- a continuous step of one peak when (Detector2_PeakStartTime+Detector1_DigitalTimeResolution) > GlobalTime--
                                        {
                                                        Detector2_TotPeakEnergy = Detector2_TotPeakEnergy + dDepositeEnergy->at(i);
                                                        Detector2_PeakEndTime = globalTime->at(i);
                            
                                                        if(dDepositeEnergy->at(i)>Detector2_MaxPeakEnergy)
                                                            {
                                                                            Detector2_MaxPeakEnergy = dDepositeEnergy->at(i);
                                                                            Detector2_MaxPeakTime = globalTime->at(i);
                                                                            Detector2_MaxPeakPositionX = PositionX->at(i);
                                                                            Detector2_MaxPeakPositionY = PositionY->at(i);
                                                                            Detector2_MaxPeakPositionZ = PositionZ->at(i);
                                                            }
                            
                                        }
                           } 
                        //---- End of PeakCollecting==true ----         
                  }
                //--- End of the condition (dDepositeEnergy->at(i)>Detector2_PeakThreshold) ---
                
                //%%%%%Check Step beneath PeakThreshold%%%%%%%
                if(dDepositeEnergy->at(i)<Detector2_PeakThreshold)
                {
                            if( (Detector2_PeakCollecting==true)&&( (Detector2_PeakEndTime+Detector2_DigitalTimeResolution)<globalTime->at(i) ) ) 
                                {
                                            Detector2_PeakCollecting = false;
                        
                                            //--push_back to save the previous peak information--
                                            fDetector2_PeakStartTime->push_back(Detector2_PeakStartTime);
                                            fDetector2_PeakEndTime->push_back(Detector2_PeakEndTime);
                                            fDetector2_MaxPeakTime->push_back(Detector2_MaxPeakTime);
                        
                                            fDetector2_MaxPeakEnergy->push_back(Detector2_MaxPeakEnergy);
                                            fDetector2_TotPeakEnergy->push_back(Detector2_TotPeakEnergy);
                        
                                            fDetector2_MaxPeakPositionX->push_back(Detector2_MaxPeakPositionX);
                                            fDetector2_MaxPeakPositionY->push_back(Detector2_MaxPeakPositionY);
                                            fDetector2_MaxPeakPositionZ->push_back(Detector2_MaxPeakPositionZ);

                                            sDetector2_PeakCreatorParticle->push_back(Detector2_CreatorParticle);
                                            sDetector2_PeakCreatorProcess->push_back(Detector2_CreatorProcess);
                                            sDetector2_PeakParentParticle->push_back(Detector2_ParentPartcile);
                                            sDetector2_PhysicalVolume->push_back(Detector2_PhysicalVolume);
                                    
                        
                                }
                            //--- else case that not PeakCollecting and not reach the PeakThreshold, no action for this ----
                            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    
                }
                //--- End of Condition (dDepositeEnergy->at(i)<Detector2_PeakThreshold)----
                
                
            }
         //----End of Steps in Detector2, also the end for analysis ot steps within event --------   
            
               
    
    
    
    if( i==(TotStepNbr-1) || (globalTime->at(i)>=DecayChainTimeLimit) )

     {
           //##########For special case there's single peak all over the event##########
           
           if(Detector1_PeakCollecting == true)
           {
                    fDetector1_PeakStartTime->push_back(Detector1_PeakStartTime);
                    fDetector1_PeakEndTime->push_back(Detector1_PeakEndTime);
                    fDetector1_MaxPeakTime->push_back(Detector1_MaxPeakTime);
               
                    fDetector1_MaxPeakEnergy->push_back(Detector1_MaxPeakEnergy);
                    fDetector1_TotPeakEnergy->push_back(Detector1_TotPeakEnergy);
               
                    fDetector1_MaxPeakPositionX->push_back(Detector1_MaxPeakPositionX);
                    fDetector1_MaxPeakPositionY->push_back(Detector1_MaxPeakPositionY);
                    fDetector1_MaxPeakPositionZ->push_back(Detector1_MaxPeakPositionZ);
               
                    fDetector1_PeakDirectionX->push_back(Detector1_PeakDirectionX);
                    fDetector1_PeakDirectionY->push_back(Detector1_PeakDirectionY);
                    fDetector1_PeakDirectionZ->push_back(Detector1_PeakDirectionZ);

                    sDetector1_PeakCreatorParticle->push_back(Detector1_CreatorParticle);
                    sDetector1_PeakCreatorProcess->push_back(Detector1_CreatorProcess);
                    sDetector1_PeakParentParticle->push_back(Detector1_ParentPartcile); 

           }
         
         
         
         if(Detector2_PeakCollecting == true)
         {
                    fDetector2_PeakStartTime->push_back(Detector2_PeakStartTime);
                    fDetector2_PeakEndTime->push_back(Detector2_PeakEndTime);
                    fDetector2_MaxPeakTime->push_back(Detector2_MaxPeakTime);
             
                    fDetector2_MaxPeakEnergy->push_back(Detector2_MaxPeakEnergy);
                    fDetector2_TotPeakEnergy->push_back(Detector2_TotPeakEnergy);
             
                    fDetector2_MaxPeakPositionX->push_back(Detector2_MaxPeakPositionX);
                    fDetector2_MaxPeakPositionY->push_back(Detector2_MaxPeakPositionY);
                    fDetector2_MaxPeakPositionZ->push_back(Detector2_MaxPeakPositionZ);
             
                    sDetector2_PeakCreatorParticle->push_back(Detector2_CreatorParticle);
                     sDetector2_PeakCreatorProcess->push_back(Detector2_CreatorProcess);
                     sDetector2_PeakParentParticle->push_back(Detector2_ParentPartcile);
                         sDetector2_PhysicalVolume->push_back(Detector2_PhysicalVolume);


         }
         
         //##########end of  special case there's single peak all over the event##########
         
         
         
         
         
         
         iDetector1_PeakNbr = fDetector1_TotPeakEnergy->size();
         
         fDetector1_TotEneDeposit = 0;
         
         
         //====== To Find the Trigger Peak from Peak Arrays in Detector1 ===========
         
         if(fDetector1_TotPeakEnergy->size()>0)
         {
                        for(unsigned int k=0; k<fDetector1_TotPeakEnergy->size(); k++)
                                fDetector1_TotEneDeposit = fDetector1_TotEneDeposit + fDetector1_TotPeakEnergy->at(k);
             
                    bool PeakTrigger = false;
             
                        for(unsigned int k=0; k<fDetector1_TotPeakEnergy->size(); k++)
                        {
                                    //--- To Check the First Peak that exceeds the Trigger Energy Threshold ----
                                    if( (fDetector1_TotPeakEnergy->at(k)>Detector1_TriggerThreshold)&&(PeakTrigger==false) )
                                    {
                                            Detector1_TriggerEnergy         = fDetector1_TotPeakEnergy->at(k);
                                            Detector1_TriggerTime           = fDetector1_PeakStartTime->at(k);
                                            Detector1_TriggerPositionX      = fDetector1_MaxPeakPositionX->at(k);
                                            Detector1_TriggerPositionY      = fDetector1_MaxPeakPositionY->at(k);
                                            Detector1_TriggerPositionZ      = fDetector1_MaxPeakPositionZ->at(k);
                                            Detector1_TriggerParticle       = sDetector1_PeakCreatorParticle->at(k);
                                            Detector1_TriggerParentParticle = sDetector1_PeakParentParticle->at(k);
                                            Detector1_TriggerProcess        = sDetector1_PeakCreatorProcess->at(k);
                                            Detector1_TriggerDirectionX     = fDetector1_PeakDirectionX->at(k);
                                            Detector1_TriggerDirectionY     = fDetector1_PeakDirectionY->at(k);
                                            Detector1_TriggerDirectionZ     = fDetector1_PeakDirectionZ->at(k);
                                                            PeakTrigger     = true;
                                    }
                        }
         
        }
         
         //-------Save the Trigger Information into TTree data----------
         fDetector1_TriggerEnergy    = Detector1_TriggerEnergy;
         fDetector1_TriggerTime      = Detector1_TriggerTime;
         fDetector1_TriggerPositionX = Detector1_TriggerPositionX;
         fDetector1_TriggerPositionY = Detector1_TriggerPositionY;
         fDetector1_TriggerPositionZ = Detector1_TriggerPositionZ;
         
         fDetector1_TriggerDirectionX = Detector1_TriggerDirectionX;
         fDetector1_TriggerDirectionY = Detector1_TriggerDirectionY;
         fDetector1_TriggerDirectionZ = Detector1_TriggerDirectionZ;
         
         strcpy(sDetector1_TriggerParticle,       Detector1_TriggerParticle.c_str());
         strcpy(sDetector1_TriggerProcess,        Detector1_CreatorProcess.c_str());
         strcpy(sDetector1_TriggerParentParticle, Detector1_TriggerParentParticle.c_str());

         //sDetector1_TriggerParticle  = Detector1_TriggerParticle.c_str();
         //sDetector1_TriggerProcess   = Detector1_CreatorProcess.c_str();
         
        //=============================================================== 
         
         
         
         
         
         
        //=============================================================== 
         
         
         iDetector2_PeakNbr = fDetector2_TotPeakEnergy->size();

         fDetector2_TotEneDeposit = 0;  
         
         
        //====== To Find the Trigger Peak from Peak Arrays in Detector2 ===========
         
         if(fDetector2_TotPeakEnergy->size()>0)
         {
                for(unsigned int k=0; k<fDetector2_TotPeakEnergy->size(); k++)
                fDetector2_TotEneDeposit = fDetector2_TotEneDeposit + fDetector2_TotPeakEnergy->at(k);
             
          
             
             bool PeakTrigger = false;
             
                for(unsigned int k=0; k<fDetector2_TotPeakEnergy->size(); k++)
                {
                        //--- To Check the First Peak that exceeds the Trigger Energy Threshold ----
                            if( (fDetector2_TotPeakEnergy->at(k)>Detector2_TriggerThreshold)&&(PeakTrigger==false) )
                            {
                                Detector2_TriggerEnergy         = fDetector2_TotPeakEnergy->at(k);
                                Detector2_TriggerTime           = fDetector2_PeakStartTime->at(k);
                                Detector2_TriggerPositionX      = fDetector2_MaxPeakPositionX->at(k);
                                Detector2_TriggerPositionY      = fDetector2_MaxPeakPositionY->at(k);
                                Detector2_TriggerPositionZ      = fDetector2_MaxPeakPositionZ->at(k);
                                Detector2_TriggerParticle       = sDetector2_PeakCreatorParticle->at(k);
                                Detector2_TriggerParentParticle = sDetector2_PeakParentParticle->at(k);
                                Detector2_TriggerProcess        = sDetector2_PeakCreatorProcess->at(k);
                                                    PeakTrigger = true;

                            }
                }
             
         }
         
         //-------Save the Trigger Information into TTree data----------
         fDetector2_TriggerEnergy    = Detector2_TriggerEnergy;
         fDetector2_TriggerTime      = Detector2_TriggerTime;
         fDetector2_TriggerPositionX = Detector2_TriggerPositionX;
         fDetector2_TriggerPositionY = Detector2_TriggerPositionY;
         fDetector2_TriggerPositionZ = Detector2_TriggerPositionZ;
         
         strcpy(sDetector2_TriggerParticle,       Detector2_TriggerParticle.c_str());
         strcpy(sDetector2_TriggerProcess,        Detector2_CreatorProcess.c_str());
         strcpy(sDetector2_TriggerParentParticle, Detector2_TriggerParentParticle.c_str());
         
         //========================================================================
         
         
         break;   //------------break from last step loop--------------
         
            }
            //-----end for the last step or the timeLimit edge: ( i==(TotStepNbr-1) || (globalTime->at(i)>=DecayChainTimeLimit) )---------
    
         
    
                 

         
     }
    //-----------End of The TotStepNbr loop--------------
     
    
    
    //---Save the Multiple scattering infomation-----
    iDetector1_MultipleScattering   = Detector1_MultipleScattering;
    iDetector1_NonElasticScattering = Detector1_NonElasticScattering;
    
    
    //------To Save Data into ROOT ---------
    
    if( (fDetector1_TotPeakEnergy->size()>0) && (fDetector2_TotPeakEnergy->size()>0) ) //--Standard Requirement for both Trigger---
    {
        tData->Fill();
    }
   
    
    
} //-----End of Entire Loop -----
    

    //cout<<"the total number of trigger is "<<tempDetector1_TotPeakNbr<<endl;
   
   // DrawWaveform(time, count);
    //delete time;
    //delete count;
    
    
    dataFile->Write();
    dataFile->Close();

    
    
    
    
    
  
/*
 //---------------Check Data whether sorted by time-------------------   
    
    double   TestCurrentTime;
    
    for (Int_t k=0; k<TotG4RawEventNbr; k++) 
    {
        
        _DataTTree->GetEntry(k);  
        
        const int TotStepNbr = ParentID->size();
        
        
        for(int i=0; i<TotStepNbr; i++)
        {
        
                if(i==0)
                TestCurrentTime = globalTime->at(i);

    
                if( (globalTime->at(i)<TestCurrentTime)&&(i!=0) )
                {
                        std::cout<<"For the event: "<<EventID<<", the First TestTime is: "<<TestCurrentTime<<std::endl;
                        std::cout<<"errot in Data!\n Data is not sorted by time for the event: "<<EventID<<",  with step#: "<<TotStepNbr<<"\n the Step#: "<<i<<".  Whether hit wherelse: "<<iInteractingVolume<<std::endl;   
                }
            
        }
        
           
    }
    
//----------------------------------------------------------------------    
*/


    //outResult.close();
    
    f->Close();
    
     return 0;
        
}
