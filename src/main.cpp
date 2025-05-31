#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <sstream>

bool isDark = true;
std::string wordcount = "";
bool isSaved = false;
wxChar * filename;
wxTextCtrl * textCtrl;
void loadFile(wxChar * fname);
void saveFile(wxChar * fname);


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
        void ToggleDarkMode(wxCommandEvent& event);
};
enum{
    ID_Save = 1,
    ID_Dark = 2
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

TexFrame::TexFrame() : wxFrame(NULL, wxID_ANY , "TexEdit++"){
    wxMenu * menufile = new wxMenu;
    
    menufile->Append(ID_Save, "&Save...\tCtrl-S",
                     "Save the file");

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
        SetStatusText(std::string(wxString(filename).mb_str()) + " \t|\tLines:"+wordcount+"\t|\t Saved");
    }else{
        SetStatusText(std::string(wxString(filename).mb_str()) + " \t|\tLines:"+wordcount+"\t|\t Not Saved");
    }

    textCtrl = new wxTextCtrl(this, wxID_ANY, "",
                                  wxDefaultPosition, wxDefaultSize,
                                  wxTE_MULTILINE | wxTE_RICH | wxTE_PROCESS_TAB);
    textCtrl->SetMargins(10, 10);
    textCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent&) {
        isSaved = false;
        SetStatusText(std::string(wxString(filename).mb_str()) + " \t|\tLines:"+wordcount+"\t|\t Not Saved");
        wxString content = textCtrl->GetValue();
        wordcount = std::to_string(wxSplit(content, '\n').size());
    });

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(textCtrl, 1, wxEXPAND);
    SetSizer(sizer);


    Bind(wxEVT_MENU, &TexFrame::OnSave,this,ID_Save);
    Bind(wxEVT_MENU, &TexFrame::ToggleDarkMode,this,ID_Dark);
    Bind(wxEVT_MENU, &TexFrame::OnExit,this,wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &TexFrame::OnClose, this);
    
    loadFile(filename);
    SetStatusText(std::string(wxString(filename).mb_str()) + " \t|\tLines:"+wordcount+"\t|\t File opened");

}

void TexFrame::OnClose(wxCloseEvent& event){
    if(!isSaved){
        int result = wxMessageBox("Do you want to save changes to the file?",
                              "Confirm Exit",
                              wxYES_NO | wxCANCEL | wxICON_QUESTION, this);
        if (result == wxYES){
            saveFile(filename);
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
    this->SetStatusText(std::string(wxString(filename).mb_str()) + " \t|\tLines:"+wordcount+"\t|\t Saved");
    saveFile(filename);
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


void loadFile(wxChar * fname){
    std::ifstream file(std::string(wxString(filename).mb_str()).c_str());
    if (file){
        std::stringstream buffer;
        buffer << file.rdbuf();
        textCtrl->SetValue(buffer.str());
        isSaved = true;
    }
}

void saveFile(wxChar * fname){
    std::ofstream file(std::string(wxString(filename).mb_str()).c_str());
    if (file){
        file << textCtrl->GetValue().ToStdString();
    }
}