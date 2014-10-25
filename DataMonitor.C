
#include <stdlib.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TTree.h>
#include <TClass.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TVirtualPadEditor.h>
#include <TGResourcePool.h>
#include <TGListBox.h>
#include <TGListTree.h>
#include <TGFSContainer.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGTable.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGDoubleSlider.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TGShutter.h>
#include <TGProgressBar.h>
#include <TGColorSelect.h>
#include <RQ_OBJECT.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TEnv.h>
#include <TFile.h>
#include <TKey.h>
#include <TGDockableFrame.h>
#include <TGFontDialog.h>


enum ETestCommandIdentifiers {
   M_FILE_OPEN,
   M_FILE_SAVE,
   M_FILE_SAVEAS,
   M_FILE_PRINT,
   M_FILE_PRINTSETUP,
   M_FILE_EXIT,

   M_TEST_DLG,
   M_TEST_MSGBOX,
   M_TEST_SLIDER,
   M_TEST_SHUTTER,
   M_TEST_DIRLIST,
   M_TEST_FILELIST,
   M_TEST_PROGRESS,
   M_TEST_NUMBERENTRY,
   M_TEST_FONTDIALOG,
   M_TEST_NEWMENU,

   M_VIEW_ENBL_DOCK,
   M_VIEW_ENBL_HIDE,
   M_VIEW_DOCK,
   M_VIEW_UNDOCK,

   M_HELP_CONTENTS,
   M_HELP_SEARCH,
   M_HELP_ABOUT,

   M_CASCADE_1,
   M_CASCADE_2,
   M_CASCADE_3,

   M_NEW_REMOVEMENU,

   VId1,
   HId1,
   VId2,
   HId2,

   VSId1,
   HSId1,
   VSId2,
   HSId2,
   kB_openDataFile,
   kB_Run
};


Int_t mb_button_id[13] = { kMBYes, kMBNo, kMBOk, kMBApply,
                           kMBRetry, kMBIgnore, kMBCancel,
                           kMBClose, kMBYesAll, kMBNoAll, 
                           kMBNewer, kMBAppend, kMBDismiss};

EMsgBoxIcon mb_icon[4] = { kMBIconStop, kMBIconQuestion,
                           kMBIconExclamation, kMBIconAsterisk };

const char *filetypes[] = { "All files",     "*",
                            "ROOT files",    "*.root",
                            "ROOT macros",   "*.C",
                            "Text files",    "*.[tT][xX][tT]",
                            0,               0 };

struct shutterData_t {
   const char *pixmap_name;
   const char *tip_text;
   Int_t       id;
   TGButton   *button;
};

shutterData_t histo_data[] = {
   { "h1_s.xpm",        "TH1",      1001,  0 },
   { "h2_s.xpm",        "TH2",      1002,  0 },
   { "h3_s.xpm",        "TH3",      1003,  0 },
   { "profile_s.xpm",   "TProfile", 1004,  0 },
   { 0,                 0,          0,     0 }
};

shutterData_t function_data[] = {
   { "f1_s.xpm",        "TF1",      2001,  0 },
   { "f2_s.xpm",        "TF2",      2002,  0 },
   { 0,                 0,          0,     0 }
};

shutterData_t tree_data[] = {
   { "ntuple_s.xpm",    "TNtuple",  3001,  0 },
   { "tree_s.xpm",      "TTree",    3002,  0 },
   { "chain_s.xpm",     "TChain",   3003,  0 },
   { 0,                 0,          0,     0 }
};




class MainFrame {
RQ_OBJECT("MainFrame")

private:
   TGMainFrame        *fMain;
   TGTextEntry        *fFileText;
   TGButton           *fTestButton;
   TGColorSelect      *fColorSel;

   TGFileInfo fi;
   TGTab *fMainTab;
 
   TGCompositeFrame *fMenuFrame;
   TGCompositeFrame *fTableFrame;
   TGCompositeFrame *GeneralMainFrame;
   TGTable *GeneralTable;
   TGLabel *GeneralLable;
   TGTextView  *GeneralTextView;
   TGCompositeFrame *GeneralF1;
   TRootEmbeddedCanvas *GeneralF1_Canvas1;
   TRootEmbeddedCanvas *GeneralF1_Canvas2;

   TGCompositeFrame *PSDMainFrame;
   TGCompositeFrame *PSDF1;
   TRootEmbeddedCanvas *PSDF1_Canvas1;
   TRootEmbeddedCanvas *PSDF1_Canvas2;

   TGCompositeFrame *PSDF2;
   TRootEmbeddedCanvas *PSDF2_Canvas1;
   TRootEmbeddedCanvas *PSDF2_Canvas2;

   TGCompositeFrame *PSDButtonFrame;
   TGNumberEntry *PSDeventnumberEntry;
   TGTextButton *PSDGotoEventButton;
   TGTextButton *PSDPreviousEventButton;
   TGTextButton *PSDNextEventButton;

   TGCompositeFrame *BGOMainFrame;
   TGCompositeFrame *BGOFrame1;
   TGCompositeFrame *BGOFrame1_1;
   TGCompositeFrame *BGOFrame1_2;
   TRootEmbeddedCanvas *BGOFrame1_1_Canvas1;
   TRootEmbeddedCanvas *BGOFrame1_1_Canvas2;
   TRootEmbeddedCanvas *BGOFrame1_2_Canvas1;
   TRootEmbeddedCanvas *BGOFrame1_2_Canvas2;
  
   TGCompositeFrame *BGOButtonFrame;
   TGNumberEntry *BGOeventnumberEntry;
   TGTextButton *BGOGotoEventButton;
   TGTextButton *BGOPreviousEventButton;
   TGTextButton *BGONextEventButton;

   TGCompositeFrame *BGOFrame2;
   TGCompositeFrame *BGOFrame2_1;
   TGCompositeFrame *BGOFrame2_2;
   TGCompositeFrame *BGOFrame2_3;
   TRootEmbeddedCanvas *BGOFrame2_1_Canvas1;
   TRootEmbeddedCanvas *BGOFrame2_1_Canvas2;
   TRootEmbeddedCanvas *BGOFrame2_2_Canvas1;
   TRootEmbeddedCanvas *BGOFrame2_2_Canvas2;
   TRootEmbeddedCanvas *BGOFrame2_3_Canvas1;
   TRootEmbeddedCanvas *BGOFrame2_3_Canvas2;
   TRootEmbeddedCanvas *BGOFrame2_3_Canvas3;
   TGCompositeFrame *NUDF1;
   TRootEmbeddedCanvas *NUDF1_Canvas1;
   TRootEmbeddedCanvas *NUDF1_Canvas2;

   TGCompositeFrame *NUDF2;
   TRootEmbeddedCanvas *NUDF2_Canvas1;
   TRootEmbeddedCanvas *NUDF2_Canvas2;
   TH1F *h1;
   TH2F *h2;
   TFile *file;
   TTree *tree;
  Int_t           TMCode_s;
  Int_t           TMCode_ms;
  Int_t           DeltaT;
  Int_t           TriggerID;
  Float_t         ebgo[14][22];
  Float_t         ebgop[14][22];
  Float_t         ebgon[14][22];
  Float_t         eps[2][41];
  Float_t         epsp[2][41];
  Float_t         epsn[2][41];
  Float_t         etotbgo;
  Float_t         etotbgopos;
  Float_t         etotbgoneg;
  Float_t         etotpsd;
  Float_t         etotpsdpos;
  Float_t         etotpsdneg;
  Int_t           hittotbgo;
  Int_t           hittotpsd;
  Float_t         elayer[14];
  Float_t         EdepXZ[22];
  Float_t         EdepYZ[22];
  Float_t         XCoW[14];
  Float_t         YCoW[14];
  Float_t         Edep3;
  Float_t         Edep5;
  Float_t         RMS2[14];
  Float_t         FValue[14];
  Float_t         AngleXZ;
  Float_t         AngleYZ;
  Float_t         PosAtSurfaceX;
  Float_t         PosAtSurfaceY;
  Float_t         Chi2X;
  Float_t         Chi2Y;
  Int_t           nud[4];
  Float_t         nudcount;

  TH2F *hist_trajectory_xz;
  TH2F *hist_trajectory_yz;
  TH1F *hist_longprofile;
  TH1F *hist_lateralprofilexz;
  TH1F *hist_lateralprofileyz;
  TH2F *hist_profile_psd_1;
  TH2F *hist_profile_psd_2;
public:
   MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MainFrame();
   // slots
   void OpenData();
   void ProcessFile();
   void DrawOneEvent();
   void PreEvent();
   void NextEvent();
   void PSDDrawOneEvent();
   void PSDPreEvent();
   void PSDNextEvent();
   void CloseWindow();
   void DoButton();
   void InitTree();
   void LoadBuffer(const char *buffer);

   void Created() { Emit("Created()"); } //*SIGNAL*
   void Welcome() { printf("MainFrame has been created. Welcome!\n"); }
};


class TestProgress {

private:
   TGTransientFrame  *fMain;
   TGHorizontalFrame *fHframe1;
   TGVerticalFrame   *fVframe1;
   TGLayoutHints     *fHint1, *fHint2, *fHint3, *fHint4, *fHint5;
   TGHProgressBar    *fHProg1, *fHProg2, *fHProg3;
   TGVProgressBar    *fVProg1, *fVProg2;
   TGTextButton      *fGO;
   Bool_t             fClose;

public:
   TestProgress(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h);
   virtual ~TestProgress();

   // slots
   void CloseWindow();
   void DoClose();
   void DoGo();
};



MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h)
{
   // Create test main frame. A TGMainFrame is a top level window.

   fMain = new TGMainFrame(p, w, h);

   // use hierarchical cleaning
   fMain->SetCleanup(kDeepCleanup);

   fMain->Connect("CloseWindow()", "MainFrame", this, "CloseWindow()");
   
   fMenuFrame = new TGCompositeFrame(fMain,60,60,kHorizontalFrame);
   TGTextButton *OpenB = new TGTextButton(fMenuFrame,"&Open..",1);
   OpenB->SetToolTipText("Open a Reconstructed ROOT file");
   TGTextButton *RunB = new TGTextButton(fMenuFrame,"&Run..",2);
   RunB->SetToolTipText("Process Events");
   OpenB->Connect("Clicked()","MainFrame",this,"OpenData()");
   RunB->Connect("Clicked()","MainFrame",this,"ProcessFile()");
   fMenuFrame->AddFrame(OpenB,new TGLayoutHints(kLHintsTop|kLHintsLeft,0,0,1,1));
   fMenuFrame->AddFrame(RunB,new TGLayoutHints(kLHintsTop|kLHintsLeft,0,0,1,1));

   fFileText = new TGTextEntry(fMenuFrame,new TGTextBuffer(100));
   fFileText->SetToolTipText("opened root file name");
   fFileText->Resize(400,fFileText->GetDefaultHeight());
   fMenuFrame->AddFrame(fFileText,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,1,1,1,1));
   fMain->AddFrame(fMenuFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft,0,0,1,0));

   TGHorizontal3DLine *hsep_1 = new TGHorizontal3DLine(fMain,3,3);
   fMain->AddFrame(hsep_1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,1,1,1,1));


   TGGC myGC = *gClient->GetResourcePool()->GetFrameGC();
   TGFont *myfont = gClient->GetFont("-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
   if(myfont) myGC.SetFont(myfont->GetFontHandle());

   fMainTab = new TGTab(fMain,300,300);

   fTableFrame = fMainTab->AddTab("General");
   GeneralMainFrame = new TGCompositeFrame(fTableFrame,60,60,kHorizontalFrame);
   //GeneralTable = new TGTable(GeneralMainFrame,60,0,20,5);
   GeneralTextView = new TGTextView(GeneralMainFrame,600,400,20);
   GeneralF1 = new TGCompositeFrame(GeneralMainFrame,60,60,kVerticalFrame);
   GeneralF1_Canvas1 = new TRootEmbeddedCanvas("ecl1",GeneralF1,100,100);
   GeneralF1_Canvas2 = new TRootEmbeddedCanvas("ecl1",GeneralF1,100,100);
   GeneralF1->AddFrame(GeneralF1_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   GeneralF1->AddFrame(GeneralF1_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   
   //GeneralMainFrame->AddFrame(GeneralTable,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsCenterX|kLHintsCenterY,5,5,5,5));
   GeneralMainFrame->AddFrame(GeneralTextView,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   GeneralMainFrame->AddFrame(GeneralF1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   
   fTableFrame->AddFrame(GeneralMainFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));


   fTableFrame = fMainTab->AddTab("PSD");
   PSDMainFrame = new TGCompositeFrame(fTableFrame,60,60,kHorizontalFrame);
   
   PSDF1 = new TGCompositeFrame(PSDMainFrame,60,60,kVerticalFrame);
   PSDF1_Canvas1 = new TRootEmbeddedCanvas("ecl1",PSDF1,100,100);
   PSDF1->AddFrame(PSDF1_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   PSDF1_Canvas2 = new TRootEmbeddedCanvas("ecl2",PSDF1,100,100);
   PSDF1->AddFrame(PSDF1_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   PSDMainFrame->AddFrame(PSDF1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));   

   PSDF2 = new TGCompositeFrame(PSDMainFrame,60,60,kVerticalFrame);
   PSDF2_Canvas1 = new TRootEmbeddedCanvas("ecl1",PSDF2,100,100);
   PSDF2->AddFrame(PSDF2_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   PSDF2_Canvas2 = new TRootEmbeddedCanvas("ecl2",PSDF2,100,100);
   PSDF2->AddFrame(PSDF2_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   PSDButtonFrame = new TGCompositeFrame(PSDF2,60,20,kHorizontalFrame|kSunkenFrame|kFixedHeight);
   PSDeventnumberEntry = new TGNumberEntry(PSDButtonFrame,0,20,12, (TGNumberEntry::EStyle) 0);
   PSDGotoEventButton = new TGTextButton(PSDButtonFrame,"Goto",3); 
   PSDGotoEventButton->Connect("Clicked()","MainFrame",this,"PSDDrawOneEvent()");
   PSDPreviousEventButton = new TGTextButton(PSDButtonFrame,"Previous",4);
   PSDPreviousEventButton->Connect("Clicked()","MainFrame",this,"PSDPreEvent()");
   PSDNextEventButton = new TGTextButton(PSDButtonFrame,"Next",5);
   PSDNextEventButton->Connect("Clicked()","MainFrame",this,"PSDNextEvent()");
   TGLabel *label_psd_event = new TGLabel(PSDButtonFrame,"Event:",myGC(),myfont->GetFontStruct());

   PSDButtonFrame->AddFrame(label_psd_event,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   PSDButtonFrame->AddFrame(PSDeventnumberEntry,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   PSDButtonFrame->AddFrame(PSDGotoEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,0,2,2));
   PSDButtonFrame->AddFrame(PSDPreviousEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   PSDButtonFrame->AddFrame(PSDNextEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   PSDF2->AddFrame(PSDButtonFrame,new TGLayoutHints(kLHintsBottom|kLHintsRight,0,0,1,0));
   PSDMainFrame->AddFrame(PSDF2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   fTableFrame-> AddFrame(PSDMainFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   fTableFrame = fMainTab->AddTab("STK");
   fTableFrame = fMainTab->AddTab("BGO");
   BGOMainFrame = new TGCompositeFrame(fTableFrame,60,60,kHorizontalFrame);
   BGOFrame1 = new TGCompositeFrame(BGOMainFrame,60,60,kVerticalFrame);
   BGOFrame1_1 = new TGCompositeFrame(BGOFrame1,60,60,kHorizontalFrame);
   BGOFrame1_2 = new TGCompositeFrame(BGOFrame1,60,60,kHorizontalFrame);
   BGOFrame1_1_Canvas1 = new TRootEmbeddedCanvas("ecl1",BGOFrame1_1,100,100);
   BGOFrame1_1_Canvas2 = new TRootEmbeddedCanvas("ecl1",BGOFrame1_1,100,100);
   BGOFrame1_2_Canvas1 = new TRootEmbeddedCanvas("ecl1",BGOFrame1_2,100,100);
   BGOFrame1_2_Canvas2 = new TRootEmbeddedCanvas("ecl1",BGOFrame1_2,100,100);
   BGOFrame1_1->AddFrame(BGOFrame1_1_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame1_1->AddFrame(BGOFrame1_1_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame1_2->AddFrame(BGOFrame1_2_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame1_2->AddFrame(BGOFrame1_2_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
  
   BGOButtonFrame = new TGCompositeFrame(BGOFrame1,60,20,kHorizontalFrame|kSunkenFrame);
   BGOeventnumberEntry = new TGNumberEntry(BGOButtonFrame,0,20,12, (TGNumberEntry::EStyle) 0);
   BGOGotoEventButton = new TGTextButton(BGOButtonFrame,"Goto",6);
   BGOGotoEventButton->Connect("Clicked()","MainFrame",this,"DrawOneEvent()");
   BGOPreviousEventButton = new TGTextButton(BGOButtonFrame,"Previous",7);
   BGOPreviousEventButton->Connect("Clicked()","MainFrame",this,"PreEvent()");
   BGONextEventButton = new TGTextButton(BGOButtonFrame,"Next",8);
   BGONextEventButton->Connect("Clicked()","MainFrame",this,"NextEvent()");
   TGLabel *label_bgo_event = new TGLabel(BGOButtonFrame,"Event:",myGC(),myfont->GetFontStruct());

   BGOButtonFrame->AddFrame(label_bgo_event,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   BGOButtonFrame->AddFrame(BGOeventnumberEntry,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   BGOButtonFrame->AddFrame(BGOGotoEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,0,2,2));
   BGOButtonFrame->AddFrame(BGOPreviousEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
   BGOButtonFrame->AddFrame(BGONextEventButton,new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));

   BGOFrame1->AddFrame(BGOFrame1_1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame1->AddFrame(BGOFrame1_2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame1->AddFrame(BGOButtonFrame,new TGLayoutHints(kLHintsBottom|kLHintsRight,0,0,1,0));
   

   BGOFrame2 = new TGCompositeFrame(BGOMainFrame,60,60,kVerticalFrame);
   BGOFrame2_1 = new TGCompositeFrame(BGOFrame2,60,60,kHorizontalFrame);
   BGOFrame2_2 = new TGCompositeFrame(BGOFrame2,60,60,kHorizontalFrame);
   BGOFrame2_3 = new TGCompositeFrame(BGOFrame2,60,60,kHorizontalFrame);
   BGOFrame2_1_Canvas1 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_1,100,100);
   BGOFrame2_1_Canvas2 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_1,100,100);
   BGOFrame2_2_Canvas1 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_2,100,100);
   BGOFrame2_2_Canvas2 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_2,100,100);
   BGOFrame2_3_Canvas1 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_3,100,100);
   BGOFrame2_3_Canvas2 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_3,100,100);
   BGOFrame2_3_Canvas3 = new TRootEmbeddedCanvas("ecl1",BGOFrame2_3,100,100);
   BGOFrame2_1->AddFrame(BGOFrame2_1_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_1->AddFrame(BGOFrame2_1_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_2->AddFrame(BGOFrame2_2_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_2->AddFrame(BGOFrame2_2_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_3->AddFrame(BGOFrame2_3_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_3->AddFrame(BGOFrame2_3_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2_3->AddFrame(BGOFrame2_3_Canvas3,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   BGOFrame2->AddFrame(BGOFrame2_1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOFrame2->AddFrame(BGOFrame2_2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5)); 
   BGOFrame2->AddFrame(BGOFrame2_3,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   BGOMainFrame->AddFrame(BGOFrame1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   BGOMainFrame->AddFrame(BGOFrame2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   fTableFrame->AddFrame(BGOMainFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   fTableFrame = fMainTab->AddTab("NUD");
   NUDF1 = new TGCompositeFrame(fTableFrame,60,60,kHorizontalFrame);
   NUDF1_Canvas1 = new TRootEmbeddedCanvas("ecl1",NUDF1,100,100);
   NUDF1->AddFrame(NUDF1_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   //NUDF1_Canvas2 = new TRootEmbeddedCanvas("ecl2",NUDF1,100,100);
   //NUDF1->AddFrame(NUDF1_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   fTableFrame->AddFrame(NUDF1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   NUDF2 = new TGCompositeFrame(fTableFrame,60,60,kHorizontalFrame);
   NUDF2_Canvas1 = new TRootEmbeddedCanvas("ecl1",NUDF2,100,100);
   NUDF2->AddFrame(NUDF2_Canvas1,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   //NUDF2_Canvas2 = new TRootEmbeddedCanvas("ecl2",NUDF2,100,100);
   //NUDF2->AddFrame(NUDF2_Canvas2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));
   fTableFrame->AddFrame(NUDF2,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX|kLHintsExpandY,5,5,5,5));

   TGLayoutHints *fLT = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,
                                                    0, 0, 2, 2);
   fMain->AddFrame(fMainTab,fLT);

   fMain->SetWindowName("DAMPE Data Monitor");

   fMain->MapSubwindows();

   // we need to use GetDefault...() to initialize the layout algorithm...
   fMain->Resize();
   fMain->MapWindow();
   fMain->Print();
   Connect("Created()", "MainFrame", this, "Welcome()");
   Created();
}
MainFrame::~MainFrame()
{
   // Delete all created widgets.

   delete fMenuFile;
   delete fMenuTest;
   delete fMenuView;
   delete fMenuHelp;
   delete fCascadeMenu;
   delete fCascade1Menu;
   delete fCascade2Menu;
   delete fMenuNew1;
   delete fMenuNew2;

   delete fContainer;
   delete fMain;
}
void MainFrame::OpenData(){
   new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &fi);
   printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
   dir = fi.fIniDir;
   fFileText->SetText(fi.fFilename);
   InitTree();
}
void MainFrame::InitTree()
{
  file = TFile::Open(fi.fFilename);
  if(file ==0){
    printf("Error: cannot open %s",fi.fFilename);
  }
  file->GetObject("event",tree);
  
  tree->SetBranchAddress("TMCode_s", &TMCode_s);
  tree->SetBranchAddress("TMCode_ms", &TMCode_ms);
  tree->SetBranchAddress("DeltaT", &DeltaT);
  tree->SetBranchAddress("TriggerID", &TriggerID);
  tree->SetBranchAddress("ebgo", ebgo);
  tree->SetBranchAddress("ebgop", ebgop);
  tree->SetBranchAddress("ebgon", ebgon);
  tree->SetBranchAddress("eps", eps);
  tree->SetBranchAddress("epsp", epsp);
  tree->SetBranchAddress("epsn", epsn);
  tree->SetBranchAddress("etotbgo", &etotbgo);
  tree->SetBranchAddress("etotbgopos", &etotbgopos);
  tree->SetBranchAddress("etotbgoneg", &etotbgoneg);
  tree->SetBranchAddress("etotpsd", &etotpsd);
  tree->SetBranchAddress("etotpsdpos", &etotpsdpos);
  tree->SetBranchAddress("etotpsdneg", &etotpsdneg);
  tree->SetBranchAddress("hittotbgo", &hittotbgo);
  tree->SetBranchAddress("hittotpsd", &hittotpsd);
  tree->SetBranchAddress("elayer", elayer);
  tree->SetBranchAddress("EdepXZ", EdepXZ);
  tree->SetBranchAddress("EdepYZ", EdepYZ);
  tree->SetBranchAddress("XCoW", XCoW);
  tree->SetBranchAddress("YCoW", YCoW);
  tree->SetBranchAddress("Edep3", &Edep3);
  tree->SetBranchAddress("Edep5", &Edep5);
  tree->SetBranchAddress("RMS2", RMS2);
  tree->SetBranchAddress("FValue", FValue);
  tree->SetBranchAddress("AngleXZ", &AngleXZ);
  tree->SetBranchAddress("AngleYZ", &AngleYZ);
  tree->SetBranchAddress("PosAtSurfaceX", &PosAtSurfaceX);
  tree->SetBranchAddress("PosAtSurfaceY", &PosAtSurfaceY);
  tree->SetBranchAddress("Chi2X", &Chi2X);
  tree->SetBranchAddress("Chi2Y", &Chi2Y);
  tree->SetBranchAddress("nud", nud);
  tree->SetBranchAddress("nudcount", &nudcount);
  hist_trajectory_xz = new TH2F("hist_trajectory_xz","",22,0,22,14,0,14);
  hist_trajectory_xz->SetTitle("Energy Profile XZ");
  hist_trajectory_xz->GetXaxis()->SetTitle("Cell");
  hist_trajectory_xz->GetYaxis()->SetTitle("Layer");
  hist_trajectory_yz = new TH2F("hist_trajectory_yz","",22,0,22,14,0,14);
  hist_trajectory_yz->SetTitle("Energy Profile YZ");
  hist_trajectory_yz->GetXaxis()->SetTitle("Cell");
  hist_trajectory_yz->GetYaxis()->SetTitle("Layer");
  hist_longprofile = new TH1F("hist_longprofile","",14,0,14);
  hist_longprofile->SetTitle("Longitudinal Profile");
  hist_longprofile->GetXaxis()->SetTitle("Layer");
  hist_longprofile->GetYaxis()->SetTitle("Energy");
  hist_lateralprofilexz = new TH1F("hist_lateralprofilexz","",22,0,22);
  hist_lateralprofilexz->SetTitle("Lateral Profile");
  hist_lateralprofilexz->GetXaxis()->SetTitle("Cell");
  hist_lateralprofilexz->GetYaxis()->SetTitle("Energy");
  hist_lateralprofilexz->SetLineColor(2);
  hist_lateralprofileyz = new TH1F("hist_lateralprofileyz","",22,0,22);
  hist_lateralprofileyz->SetTitle("Lateral Profile");
  hist_lateralprofileyz->GetXaxis()->SetTitle("Cell");
  hist_lateralprofileyz->GetYaxis()->SetTitle("Energy");
  hist_lateralprofileyz->SetLineColor(4);
  hist_profile_psd_1 = new TH2F("Layer1_profile","",41,0,41,1,0,1);
  hist_profile_psd_1->SetLineColor(2);
  hist_profile_psd_1->GetXaxis()->SetTitle("Cell");
  hist_profile_psd_1->GetYaxis()->SetTitle("Layer");
  hist_profile_psd_2 = new TH2F("Layer2_profile","",41,0,41,1,0,1);
  hist_profile_psd_2->SetLineColor(4);
  hist_profile_psd_2->GetXaxis()->SetTitle("Cell");
  hist_profile_psd_2->GetYaxis()->SetTitle("Layer");
  
}
void MainFrame::ProcessFile()
{

  int EventNumber = tree->GetEntries();
  tree->GetEntry(0);
  int StartTime = TMCode_s;
  tree->GetEntry(EventNumber-1);
  int StopTime = TMCode_s;

  char T_header[100];
  char T_Filename[100];
  char T_RunNumber[100];
  char T_Statistic[100];
  char T_PID[100];
  char T_Moment[100];
  char T_Pos[100];
  char T_Starttime[100];
  char T_Stoptime[100];
  sprintf(T_header,       "  Item      Parameter    Value ");
  sprintf(T_Filename,     "     1      FileName     %s",fi.fFilename);
  sprintf(T_RunNumber,    "     2      Run Number   20001");
  sprintf(T_Statistic,    "     3      Statistic    %d",EventNumber);
  sprintf(T_PID,          "     4      Beam PID     muon");
  sprintf(T_Moment,       "     5      Moment       10GeV");
  sprintf(T_Pos,          "     6      PosX:PosY    100:-10mm");
  sprintf(T_Starttime,    "     7      Start Time   %d",StartTime);
  sprintf(T_Stoptime,     "     8      Stop Time    %d",StopTime);
  
  LoadBuffer(T_header);
  LoadBuffer(T_Filename);
  LoadBuffer(T_RunNumber);
  LoadBuffer(T_Statistic);
  LoadBuffer(T_PID);
  LoadBuffer(T_Moment);
  LoadBuffer(T_Pos);
  LoadBuffer(T_Starttime);
  LoadBuffer(T_Stoptime);
  GeneralTextView->Update();
  DrawOneEvent();
  gStyle->SetOptStat(1);
  TCanvas *TMCode_s_c1 = GeneralF1_Canvas1->GetCanvas();
  TCanvas *Delta_c    = GeneralF1_Canvas2->GetCanvas();
  TMCode_s_c1->cd();
  int nbins = StopTime - StartTime;
  //printf("nbins=%d, %d, %d\n",nbins,StartTime,StopTime);
  TH1F *time_h = new TH1F("EventRate","",nbins,StartTime,StopTime);
  time_h->SetTitle("Event Rate");
  time_h->GetXaxis()->SetTitle("Time Code(s)");
  tree->Draw("TMCode_s>>EventRate","","");
  
  TMCode_s_c1->Update();
  Delta_c->cd();
  tree->Draw("DeltaT","","");
  Delta_c->Update();
  
  TCanvas *PSDEnergy_c = PSDF1_Canvas1->GetCanvas();
  TCanvas *PSDHit_c = PSDF1_Canvas2->GetCanvas();
  PSDEnergy_c->cd();
  tree->Draw("etotpsd>>HPSD0(100,0,0.5)","","");
  tree->Draw("etotpsdpos>>HPSD1(100,0,0.5)","","same");
  tree->Draw("etotpsdneg>>HPSD2(100,0,0.5)","","same");
  HPSD0->SetLineColor(kRed);
  HPSD0->SetLineWidth(3);
  HPSD0->SetFillColor(kRed);
  HPSD0->SetFillStyle(3004);
  HPSD1->SetLineColor(kBlue);
  HPSD1->SetLineWidth(3);
  HPSD2->SetLineColor(12);
  HPSD2->SetLineWidth(3);
  gPad->SetLogy(1);
  TLegend *lg_psd_totenergy = gPad->BuildLegend();
  lg_psd_totenergy->SetFillColor(10);
  //lg_psd_totenergy->SetTextSize();
  PSDEnergy_c->Update();
/* TLegend *lg_psd_etot = new TLegend(0.7,0.7,0.89,0.89);
  lg_psd_etot->AddEntry(HPSD0,"Pos*Neg","fl");
  lg_psd_etot->AddEntry(HPSD1,"Pos","l");
  lg_psd_etot->AddEntry(HPSD2,"Neg","l");
  lg_psd_etot->SetTextColor(0);
  lg_psd_etot->Draw();
*/
  PSDHit_c->cd();
  gPad->SetLogy(1);
  tree->Draw("hittotpsd","","");
  PSDHit_c->Update();
  
  TCanvas *BGOEnergy_c = BGOFrame2_1_Canvas1->GetCanvas();
  TCanvas *BGOHit_c = BGOFrame2_1_Canvas2->GetCanvas();
  TCanvas *BGOAngleXZ_c = BGOFrame2_2_Canvas1->GetCanvas();
  TCanvas *BGOAngleYZ_c = BGOFrame2_2_Canvas2->GetCanvas();
  TCanvas *BGOPosXY_c = BGOFrame2_3_Canvas1->GetCanvas();
  TCanvas *BGOEvsHit_c = BGOFrame2_3_Canvas2->GetCanvas();
  TCanvas *TotalEnergy_c = BGOFrame2_3_Canvas3->GetCanvas();

  BGOEnergy_c->cd();
  tree->Draw("etotbgo>>HBGO0(100,0,2)","","");
  tree->Draw("etotbgopos>>HBGO1(100,0,2)","","same");
  tree->Draw("etotbgoneg>>HBGO2(100,0,2)","","same");

  HBGO0->SetLineColor(kRed);
  HBGO0->SetLineWidth(3);
  HBGO0->SetFillColor(kRed);
  HBGO0->SetFillStyle(3004);
  HBGO1->SetLineColor(kBlue);
  HBGO1->SetLineWidth(3);
  HBGO2->SetLineColor(12);
  HBGO2->SetLineWidth(3);
  gPad->BuildLegend();

  BGOHit_c->cd();
  tree->Draw("hittotbgo","","");

  BGOAngleXZ_c->cd();
  tree->Draw("AngleXZ>>HAnglexz(200)","AngleXZ>-180","");
  HAnglexz->SetTitle("ProfileAngle_xz");
  BGOAngleYZ_c->cd();
  tree->Draw("AngleYZ>>HAngleyz(200)","AngleYZ>-180","");
  HAngleyz->SetTitle("ProfileAngle_yz");

  BGOPosXY_c->cd();
  tree->Draw("PosAtSurfaceX:PosAtSurfaceY","PosAtSurfaceX!=-9999&&PosAtSurfaceY!=-9999","colz");

  BGOEvsHit_c->cd();
  tree->Draw("etotbgo:hittotbgo","","");

  BGOEnergy_c->Update();
  BGOHit_c->Update();
  BGOAngleXZ_c->Update();
  BGOAngleYZ_c->Update();
  BGOPosXY_c->Update();
  BGOEvsHit_c->Update();

  TCanvas *NUD1_c = NUDF1_Canvas1->GetCanvas();
  TCanvas *NUD2_c = NUDF2_Canvas1->GetCanvas();
  NUD1_c->cd();
  tree->Draw("nud[0]>>HNUD0(110,-10,100)","","");
  tree->Draw("nud[1]>>HNUD1(110,-10,100)","","same");
  tree->Draw("nud[2]>>HNUD2(110,-10,100)","","same");
  tree->Draw("nud[3]>>HNUD3(110,-10,100)","","same");
  HNUD0->SetLineColor(2);
  HNUD0->SetLineWidth(2);
  HNUD0->GetXaxis()->SetTitle("ADC");
  HNUD1->SetLineColor(3);
  HNUD1->SetLineWidth(2);
  HNUD2->SetLineColor(4);
  HNUD2->SetLineWidth(2);
  HNUD3->SetLineColor(5);
  HNUD3->SetLineWidth(2);
  gPad->BuildLegend();
  NUD2_c->cd();
  gPad->SetLogy(1);
  tree->Draw("nudcount","","");
  NUD1_c->Update();
  NUD2_c->Update();

  Long64_t nentries = tree->GetEntriesFast();
  const int kUPDATE = 10000;
#if 0
  for(Long64_t jentry=0; jentry<nentries;jentry++) {
      tree->GetEntry(jentry);
      if(jentry%5000==0)printf("*************%d\n",jentry);
      h1->Fill(etotbgo);
  }
#endif
}
void MainFrame::DrawOneEvent()
{  
  hist_trajectory_xz->Reset();
  hist_trajectory_yz->Reset();
  hist_longprofile->Reset();
  hist_lateralprofilexz->Reset();
  hist_lateralprofileyz->Reset();
  hist_profile_psd_1->Reset();
  hist_profile_psd_2->Reset();

  int CurrentEventNumber = BGOeventnumberEntry->GetIntNumber();
  PSDeventnumberEntry->SetIntNumber(CurrentEventNumber);
  gStyle->SetOptStat(0);
  tree->GetEntry(CurrentEventNumber);
  for(int itt=0;itt<14;++itt) {
    hist_longprofile->Fill(itt,elayer[itt]);
    for(int jtt=0;jtt<22;++jtt){
       if((itt%2)!=0) hist_trajectory_xz->Fill(jtt,itt,ebgo[itt][jtt]+0.0023);
       else           hist_trajectory_yz->Fill(jtt,itt,ebgo[itt][jtt]+0.0023);
    }
  }
  for(int i=0;i<22;++i){
      hist_lateralprofilexz->Fill(i,EdepXZ[i]);
      hist_lateralprofileyz->Fill(i,EdepYZ[i]);
  }
  TCanvas *BGOProfileXZ_c = BGOFrame1_1_Canvas1->GetCanvas();
  BGOProfileXZ_c->cd();
  hist_trajectory_xz->Draw("colz");  
  BGOProfileXZ_c->Update();
  TCanvas *BGOProfileYZ_c = BGOFrame1_1_Canvas2->GetCanvas();
  BGOProfileYZ_c->cd();
  hist_trajectory_yz->Draw("colz");  
  BGOProfileYZ_c->Update(); 
  TCanvas *BGOLongProfile_c = BGOFrame1_2_Canvas1->GetCanvas();
  BGOLongProfile_c->cd();
  hist_longprofile->Draw();
  BGOLongProfile_c->Update();
  TCanvas *BGOLaterPofile_c = BGOFrame1_2_Canvas2->GetCanvas();
  BGOLaterPofile_c->cd();
  hist_lateralprofilexz->Draw();
  hist_lateralprofileyz->Draw("same");
  BGOLaterPofile_c->Update();

  for(int i=0;i<41;++i){
     hist_profile_psd_1->Fill(i,0,eps[1][i]+0.1*0.002);
     hist_profile_psd_2->Fill(i,0,eps[0][i]+0.1*0.002);
  }
  TCanvas *PSD1Layer_c = PSDF2_Canvas1->GetCanvas();
  PSD1Layer_c->cd(); 
  hist_profile_psd_1->Draw("colz");
  PSD1Layer_c->Update();
  TCanvas *PSD2Layer_c = PSDF2_Canvas2->GetCanvas();
  PSD2Layer_c->cd();
  hist_profile_psd_2->Draw("colz");
  PSD2Layer_c->Update();
}
void MainFrame::PreEvent()
{
   int CurrentEventNumber = BGOeventnumberEntry->GetIntNumber()-1;
   BGOeventnumberEntry->SetIntNumber(CurrentEventNumber);
   PSDeventnumberEntry->SetIntNumber(CurrentEventNumber);
   DrawOneEvent();
}
void MainFrame::NextEvent()
{                                
   int CurrentEventNumber = BGOeventnumberEntry->GetIntNumber()+1;
   BGOeventnumberEntry->SetIntNumber(CurrentEventNumber);
   PSDeventnumberEntry->SetIntNumber(CurrentEventNumber);
   DrawOneEvent();
}
void MainFrame::PSDDrawOneEvent()
{
  int CurrentEventNumber = PSDeventnumberEntry->GetIntNumber();
  BGOeventnumberEntry->SetIntNumber(CurrentEventNumber);
  DrawOneEvent();
}
void MainFrame::PSDPreEvent()
{
  int CurrentEventNumber = PSDeventnumberEntry->GetIntNumber()-1;
  BGOeventnumberEntry->SetIntNumber(CurrentEventNumber);
  PSDeventnumberEntry->SetIntNumber(CurrentEventNumber);
  DrawOneEvent();
}
void MainFrame::PSDNextEvent()
{
  int CurrentEventNumber = PSDeventnumberEntry->GetIntNumber()+1;
  BGOeventnumberEntry->SetIntNumber(CurrentEventNumber);
  PSDeventnumberEntry->SetIntNumber(CurrentEventNumber);
  DrawOneEvent();
}
void MainFrame::LoadBuffer(const char *buffer)
{
  GeneralTextView->AddLineFast(buffer);
}
void MainFrame::CloseWindow()
{
   // Got close message for this MainFrame. Terminates the application.
   gApplication->Terminate();
}

TestProgress::TestProgress(const TGWindow *p, const TGWindow *main,
                           UInt_t w, UInt_t h)
{
   // Dialog used to test the different supported progress bars.

   fClose = kTRUE;

   fMain = new TGTransientFrame(p, main, w, h);
   fMain->Connect("CloseWindow()", "TestProgress", this, "DoClose()");
   fMain->DontCallClose();

   // use hierarchical cleaning
   fMain->SetCleanup(kDeepCleanup);

   fMain->ChangeOptions((fMain->GetOptions() & ~kVerticalFrame) | kHorizontalFrame);

   fHframe1 = new TGHorizontalFrame(fMain, 0, 0, 0);

   fVProg1 = new TGVProgressBar(fHframe1, TGProgressBar::kFancy, 300);
   fVProg1->SetBarColor("purple");
   fVProg2 = new TGVProgressBar(fHframe1, TGProgressBar::kFancy, 300);
   fVProg2->SetFillType(TGProgressBar::kBlockFill);
   fVProg2->SetBarColor("green");

   fHframe1->Resize(300, 300);

   fVframe1 = new TGVerticalFrame(fMain, 0, 0, 0);

   fHProg1 = new TGHProgressBar(fVframe1, 300);
   fHProg1->ShowPosition();
   fHProg2 = new TGHProgressBar(fVframe1, TGProgressBar::kFancy, 300);
   fHProg2->SetBarColor("lightblue");
   fHProg2->ShowPosition(kTRUE, kFALSE, "%.0f events");
   fHProg3 = new TGHProgressBar(fVframe1, TGProgressBar::kStandard, 300);
   fHProg3->SetFillType(TGProgressBar::kBlockFill);

   fGO = new TGTextButton(fVframe1, "Go", 10);
   fGO->Connect("Clicked()", "TestProgress", this, "DoGo()");

   fVframe1->Resize(300, 300);

   fHint1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 10, 5, 5);
   fHint2 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 5, 5,  5, 10);
   fHint3 = new TGLayoutHints(kLHintsTop | kLHintsRight, 0, 50, 50, 0);
   fHint4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 0, 0, 0, 0);
   fHint5 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0);

   fHframe1->AddFrame(fVProg1, fHint1);
   fHframe1->AddFrame(fVProg2, fHint1);

   fVframe1->AddFrame(fHProg1, fHint2);
   fVframe1->AddFrame(fHProg2, fHint2);
   fVframe1->AddFrame(fHProg3, fHint2);
   fVframe1->AddFrame(fGO,     fHint3);

   fMain->AddFrame(fHframe1, fHint4);
   fMain->AddFrame(fVframe1, fHint5);

   fMain->SetWindowName("Progress Test");
   TGDimension size = fMain->GetDefaultSize();
   fMain->Resize(size);

   // position relative to the parent's window
   fMain->CenterOnParent();

   fMain->MapSubwindows();
   fMain->MapWindow();

   gClient->WaitFor(fMain);
}

TestProgress::~TestProgress()
{
   // Delete dialog.

   fMain->DeleteWindow();   // deletes fMain
}

void TestProgress::CloseWindow()
{
   // Called when window is closed via the window manager.

   delete this;
}

void TestProgress::DoClose()
{
   // If fClose is false we are still in event processing loop in DoGo().
   // In that case, set the close flag true and use a timer to call
   // CloseWindow(). This gives us change to get out of the DoGo() loop.
   // Note: calling SendCloseMessage() will not work since that will
   // bring us back here (CloseWindow() signal is connected to this method)
   // with the fClose flag true, which will cause window deletion while
   // still being in the event processing loop (since SendCloseMessage()
   // is directly processed in ProcessEvents() without exiting DoGo()).

   if (fClose)
      CloseWindow();
   else {
      fClose = kTRUE;
      TTimer::SingleShot(150, "TestProgress", this, "CloseWindow()");
   }
}

void TestProgress::DoGo()
{
   // Handle Go button.

   fClose = kFALSE;
   fVProg1->Reset(); fVProg2->Reset();
   fHProg1->Reset(); fHProg2->Reset(); fHProg3->Reset();
   fVProg2->SetBarColor("green");
   int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0;
   int inc1 = 4, inc2 = 3, inc3 = 2, inc4 = 1;
   while (cnt1 < 100 || cnt2 < 100 || cnt3 < 100 || cnt4 <100) {
      if (cnt1 < 100) {
         cnt1 += inc1;
         fVProg1->Increment(inc1);
      }
      if (cnt2 < 100) {
         cnt2 += inc2;
         fVProg2->Increment(inc2);
         if (cnt2 > 75)
            fVProg2->SetBarColor("red");
      }
      if (cnt3 < 100) {
         cnt3 += inc3;
         fHProg1->Increment(inc3);
      }
      if (cnt4 < 100) {
         cnt4 += inc4;
         fHProg2->Increment(inc4);
         fHProg3->Increment(inc4);
      }
      gSystem->Sleep(100);
      gSystem->ProcessEvents();
      // if user closed window return
      if (fClose) return;
   }
   fClose = kTRUE;
}



void DataMonitor()
{
   new MainFrame(gClient->GetRoot(), 400, 220);
}

//---- Main program ------------------------------------------------------------
#ifdef STANDALONE
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   guitest();

   theApp.Run();

   return 0;
}
#endif
