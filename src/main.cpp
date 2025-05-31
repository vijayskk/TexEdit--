#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <sstream>


wxChar * filename;
wxTextCtrl * textCtrl;


class TexApp: public wxApp{
    public:
        virtual bool OnInit();
        
};

class TexFrame : public wxFrame{
    public: 
        TexFrame();
    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnHello(wxCommandEvent& event);
};
enum{
    ID_Hello = 1
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
    
    menufile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");

    menufile->Append(wxID_EXIT);

    wxMenuBar * menubar = new wxMenuBar;
    menubar->Append(menufile, "&File");
    SetMenuBar(menubar);

    CreateStatusBar();
    SetStatusText(filename);

    textCtrl = new wxTextCtrl(this, wxID_ANY, "",
                                  wxDefaultPosition, wxDefaultSize,
                                  wxTE_MULTILINE | wxTE_RICH | wxTE_PROCESS_TAB);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(textCtrl, 1, wxEXPAND);
    SetSizer(sizer);


    Bind(wxEVT_MENU, &TexFrame::OnHello,this,ID_Hello);
    Bind(wxEVT_MENU, &TexFrame::OnExit,this,wxID_EXIT);
    
    std::ifstream file(std::string(wxString(filename).mb_str()).c_str());
        if (file){
            std::stringstream buffer;
            buffer << file.rdbuf();
            textCtrl->SetValue(buffer.str());
        }

}

void TexFrame::OnExit(wxCommandEvent& event){
    Close(true);
}
void TexFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}