#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>


bool isAutoSave = false;
std::string autoSaveState = "Off";
bool isDark = true;
std::string wordcount = "";
bool isSaved = false;
wxChar * filename;
int filesize =0;
wxTextCtrl * textCtrl;
bool loadFile(wxChar * fname);
bool saveFile(wxChar * fname);


class TexApp: public wxApp{
    public:
        virtual bool OnInit();
        
};

class TexFrame : public wxFrame{
    public: 
        TexFrame();
    private:
        void OnClose(wxCloseEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnClear(wxCommandEvent& event);
        void ToggleDarkMode(wxCommandEvent& event);
        void ToggleAutoSave(wxCommandEvent& event);
};
enum{
    ID_Save = 1,
    ID_Dark = 2,
    ID_AutoSave = 3,
    ID_Clear = 4
};

wxIMPLEMENT_APP(TexApp);

bool TexApp::OnInit(){

    int argc = wxTheApp->argc;
    wxChar** argv = wxTheApp->argv;
    if(argc>1){
        filename = argv[1];
        TexFrame * frame = new TexFrame();
        frame-> Show(true);
        return true;
    }else{
        std::cout<<"No file loaded"<<std::endl;
        exit(0);
        return false;
    }
    
}

TexFrame::TexFrame() : wxFrame(NULL, wxID_ANY , "TexEdit++",wxDefaultPosition,wxSize(800, 600)){
    wxMenu * menufile = new wxMenu;
    
    menufile->Append(ID_Save, "&Save...\tCtrl-S",
                     "Save the file");
    
    menufile->Append(ID_AutoSave, "&Toggle Autosave...\tCtrl-U",
                     "Saves the file automatically in 10 seconds");
    
    menufile->Append(ID_Clear, "&Clear...\tCtrl-L",
                     "Clear the content");

    menufile->Append(wxID_EXIT);

    wxMenu * menuview = new wxMenu;
    menuview->Append(ID_Dark, "&Toggle Dark Mode...\tCtrl-D",
                     "Toggle Dark or White Mode");

    wxMenuBar * menubar = new wxMenuBar;
    menubar->Append(menufile, "&File");
    menubar->Append(menuview, "&View");


    SetMenuBar(menubar);

    CreateStatusBar();
    if(isSaved){
        SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Saved");
    }else{
        SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Not Saved");
    }

    textCtrl = new wxTextCtrl(this, wxID_ANY, "",
                                  wxDefaultPosition, wxDefaultSize,
                                  wxTE_MULTILINE | wxTE_RICH | wxTE_PROCESS_TAB);
    textCtrl->SetMargins(10, 10);
    textCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent&) {
        isSaved = false;
        SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Not Saved");
        wxString content = textCtrl->GetValue();
        wordcount = std::to_string(wxSplit(content, '\n').size());
        filesize = content.size();
    });

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(textCtrl, 1, wxEXPAND);
    SetSizer(sizer);


    Bind(wxEVT_MENU, &TexFrame::OnSave,this,ID_Save);
    Bind(wxEVT_MENU, &TexFrame::OnClear,this,ID_Clear);
    Bind(wxEVT_MENU, &TexFrame::ToggleDarkMode,this,ID_Dark);
    Bind(wxEVT_MENU, &TexFrame::ToggleAutoSave,this,ID_AutoSave);
    Bind(wxEVT_MENU, &TexFrame::OnExit,this,wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &TexFrame::OnClose, this);
    
    bool loadres = loadFile(filename);
    if(!loadres){
        std::cout<<"Here\n";
        wxMessageBox("Cannot Open File!","Oops!!",wxCANCEL,this);
        exit(0);
    }
    SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t File opened");

    wxTimer* autoSaveTimer = new wxTimer(this);
    Bind(wxEVT_TIMER, [&](wxTimerEvent&){
        if(isAutoSave){
            isSaved = true;
            this->SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Auto Saved");
            saveFile(filename);
        }
    });
    autoSaveTimer->Start(10000);
}

void TexFrame::OnClose(wxCloseEvent& event){
    if(!isSaved){
        int result = wxMessageBox("Do you want to save changes to the file?",
                              "Confirm Exit",
                              wxYES_NO | wxCANCEL | wxICON_QUESTION, this);
        if (result == wxYES){
            int res = saveFile(filename);
            if (!res){
                wxMessageBox("Permission denied. File not saved","Oops!!",wxCANCEL,this);
            }
            event.Skip(); 
        }
        else if (result == wxNO) {
            event.Skip(); 
        }
        else {
            event.Veto();
        }
    }else{
        event.Skip();
    }
    
}

void TexFrame::OnExit(wxCommandEvent& event){
    Close(true);
}

void TexFrame::OnSave(wxCommandEvent& event)
{
    isSaved = true;
    bool res = saveFile(filename);
    if (res){
        isSaved = true;
        this->SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Saved");
    }else{
        isAutoSave = false;
        this->SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Permission denied. File not saved");
    }
}

void TexFrame::OnClear(wxCommandEvent& event)
{
    textCtrl->Clear();
    this->SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Saved");
}

void TexFrame::ToggleDarkMode(wxCommandEvent& event){
    if (isDark){
        textCtrl->SetBackgroundColour(*wxWHITE);
        textCtrl->SetForegroundColour(*wxBLACK);
        textCtrl->Refresh();
    }else{
        textCtrl->SetBackgroundColour(*wxTransparentColour);
        textCtrl->SetForegroundColour(*wxWHITE);
        textCtrl->Refresh();
    }
    isDark = !isDark;
};

void TexFrame::ToggleAutoSave(wxCommandEvent& event){
    if(isAutoSave){
        autoSaveState = "Off";
        isAutoSave = false;
    }else{
        autoSaveState = "On";
        isAutoSave = true;
    }
    this->SetStatusText(std::string(wxString(filename).mb_str()) +" ("+std::to_string(filesize)+" bytes)"+ +"\t|\tAutosave: "+ autoSaveState + "\t|\tLines:"+wordcount+"\t|\t Auto Saved");

};

bool loadFile(wxChar * fname){
    std::ifstream file(std::string(wxString(filename).mb_str()).c_str());
    if (access(std::string(wxString(filename).mb_str()).c_str(),R_OK|W_OK) != -1){
        std::stringstream buffer;
        buffer << file.rdbuf();
        textCtrl->SetValue(buffer.str());
        isSaved = true;
        return true;
    }else{
        return false;
    }
}

bool saveFile(wxChar * fname){
    std::ofstream file(std::string(wxString(filename).mb_str()).c_str());
    if (file.is_open()){
        file << textCtrl->GetValue().ToStdString();
        return true;
    }else{
        return false;
    }
}