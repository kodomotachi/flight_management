#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <string>
#include <fstream>
#include "Plane.h"

// Define a new application type
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title);
    void OnOpenSecondaryWindow(wxCommandEvent &event);
    void GetString(wxCommandEvent &event);              // get string
    void CreateFlightManagement(wxCommandEvent &event); // create window for flight management

private:
    wxTextCtrl *check;
};

// Define a new frame for flight management
class FlightManagement : public wxFrame
{
    public:
        FlightManagement(const wxString &title, MyFrame *mainWin);
        // void OnMouseLeftDown(wxMouseEvent &event);
        // void OnListBoxSelect(wxCommandEvent &event);
        void UpdateButtonStates();
        void onClose(wxCloseEvent &event);
        void Create_add_session(wxCommandEvent &event);
        void Create_edit_session(wxCommandEvent &event);
        int getData(string fileName);
        void InitializeListCtrl();
        void OnListItemSelected(wxListEvent &event);

    private:
        MyFrame *mainWin;
        wxListCtrl *listBox;
        wxButton *addButton;
        wxButton *removeButton;
        wxButton *editButton;
        int cnt = 0;

        // wxDECLARE_EVENT_TABLE();
};

class FlightManagement_add_session : public wxFrame
{
    public:
        FlightManagement_add_session(const wxString &title, FlightManagement *main_window);
        void onClose(wxCloseEvent &event);
        void submitClicked(wxCommandEvent &event);

    private:
        FlightManagement *main_window;
        wxTextCtrl *textCtrl1;
        wxTextCtrl *textCtrl2;
        wxTextCtrl *textCtrl3;
        wxButton *submit;
};

class FlightManagement_edit_session : public wxFrame
{
    public:
        FlightManagement_edit_session(const wxString &title, FlightManagement *main_window);
        void onClose(wxCloseEvent &event);
        void submitClicked(wxCommandEvent &event);
        void getValue(int idx);

    private:
        FlightManagement *main_window;
        wxTextCtrl *textCtrl1;
        wxTextCtrl *textCtrl2;
        wxTextCtrl *textCtrl3;
        wxString tmp1;
        wxString tmp2;
        wxString tmp3;
        wxButton *submit;
        int idx;
};

enum
{
    ID_LISTBOX = 1,
    ID_ADD_BUTTON,
    ID_REMOVE_BUTTON,
    ID_EDIT_BUTTON
};

// Implement the application type
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Flight Management");
    frame->SetClientSize(1280, 720);
    frame->Center();
    frame->Show(true);
    return true;
}

// Implement the main frame
MyFrame::MyFrame(const wxString &title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxString describe_planes = "Add/Delete/Edit your flight list"; // describe flight management program button
    wxString describe_flight = "Add new flights information, status (of tickets), time of departure or cancellation";
    wxString describe_ticket = "Add new tickets according to the passengers' info and flights";
    wxString describe_customers = "Customers' info: Identity Card's number, Full Name and Gender";
    wxString describe_statistic = "Statistics the number flytimes in descending order";

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    // check = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(20, 20), wxSize(200, 25));

    wxButton *button_1 = new wxButton(panel, wxID_ANY, "Planes", wxPoint(230, 20), wxSize(250, 25));
    new wxStaticText(panel, wxID_ANY, describe_planes, wxPoint(550, 20), wxDefaultSize, 0);

    wxButton *button_2 = new wxButton(panel, wxID_ANY, "Flights", wxPoint(230, 60), wxSize(250, 25));
    new wxStaticText(panel, wxID_ANY, describe_flight, wxPoint(550, 60), wxDefaultSize, 0);

    wxButton *button_3 = new wxButton(panel, wxID_ANY, "Customer", wxPoint(230, 100), wxSize(250, 25));
    new wxStaticText(panel, wxID_ANY, describe_ticket, wxPoint(550, 100), wxDefaultSize, 0);

    wxButton *button_4 = new wxButton(panel, wxID_ANY, "Ticket", wxPoint(230, 140), wxSize(250, 25));
    new wxStaticText(panel, wxID_ANY, describe_customers, wxPoint(550, 140), wxDefaultSize, 0);

    wxButton *button_5 = new wxButton(panel, wxID_ANY, "Statistic", wxPoint(230, 180), wxSize(250, 25));
    new wxStaticText(panel, wxID_ANY, describe_statistic, wxPoint(550, 180), wxDefaultSize, 0);

    button_1->Bind(wxEVT_BUTTON, &MyFrame::CreateFlightManagement, this);
}

void MyFrame::CreateFlightManagement(wxCommandEvent &event)
{
    FlightManagement *flightManagement = new FlightManagement("Planes Management", this);
    flightManagement->Center();
    flightManagement->Show(true);
    this->Hide();
}

FlightManagement::FlightManagement(const wxString &title, MyFrame *mainWin)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720)), mainWin(mainWin)
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    listBox = new wxListCtrl(panel, ID_LISTBOX, wxDefaultPosition, wxSize(140, 300), wxLC_REPORT);

    // Add some test data
    loadDataFlight("C:\\Users\\KodomoTachi\\Desktop\\wx_cmake_fetchcontent_template\\data.txt");
    InitializeListCtrl();

    addButton = new wxButton(panel, ID_ADD_BUTTON, "Add", wxDefaultPosition);
    removeButton = new wxButton(panel, ID_REMOVE_BUTTON, "Remove", wxDefaultPosition);
    editButton = new wxButton(panel, ID_EDIT_BUTTON, "Edit", wxDefaultPosition);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(addButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    buttonSizer->Add(removeButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(listBox, 1, wxEXPAND | wxALL, 0);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    panel->SetSizer(mainSizer);

    removeButton->Disable();
    editButton->Disable();

    // Bind the selection event in the FlightManagement constructor
    listBox->Bind(wxEVT_LIST_ITEM_SELECTED, &FlightManagement::OnListItemSelected, this);

    addButton->Bind(wxEVT_BUTTON, &FlightManagement::Create_add_session, this);
    editButton->Bind(wxEVT_BUTTON, &FlightManagement::Create_edit_session, this);
    Bind(wxEVT_CLOSE_WINDOW, &FlightManagement::onClose, this);
}

// Define the event handler function in your FlightManagement class
void FlightManagement::OnListItemSelected(wxListEvent &event)
{
    // Get the selected item index
    cnt = event.GetIndex();

    // Optionally, you can retrieve data associated with the selected item
    // wxString planeId = listBox->GetItemText(itemIndex, 1);  // Get planeId
    // wxString type = listBox->GetItemText(itemIndex, 2);     // Get type
    // wxString seats = listBox->GetItemText(itemIndex, 3);    // Get seats

    // Example: Enable the edit and remove buttons when an item is selected
    editButton->Enable();
    removeButton->Enable();
}

void FlightManagement::InitializeListCtrl()
{
    // Add columns
    listBox->InsertColumn(0, "Number", wxLIST_FORMAT_LEFT, 150);
    listBox->InsertColumn(1, "Plane ID", wxLIST_FORMAT_LEFT, 150);
    listBox->InsertColumn(2, "Type", wxLIST_FORMAT_LEFT, 350);
    listBox->InsertColumn(3, "Seats", wxLIST_FORMAT_LEFT, 150);
    listBox->InsertColumn(4, "Status", wxLIST_FORMAT_LEFT, 150);

    int check_cnt = checkNumber();
    Flight *cur = head;

    // Add items
    for (int i = 0; i < check_cnt; ++i)
    {
        std::string idx = std::to_string(i + 1);
        wxString tmp(idx);

        long itemIndex = listBox->InsertItem(i, tmp);

        wxString tmp_planeId(cur->planeId);
        wxString tmp_type(cur->type);
        wxString tmp_seats(std::to_string(cur->seats));

        listBox->SetItem(itemIndex, 1, tmp_planeId);
        listBox->SetItem(itemIndex, 2, tmp_type);
        listBox->SetItem(itemIndex, 3, tmp_seats);
        listBox->SetItem(itemIndex, 4, "Available");

        cur = cur->next;
    }
}

void FlightManagement::onClose(wxCloseEvent &event)
{
    // Perform custom action before closing
    wxMessageDialog *dlg = new wxMessageDialog(this, "Are you sure you want to close this window?", "Confirm Exit", wxYES_NO | wxICON_QUESTION);
    if (dlg->ShowModal() == wxID_YES)
    {
        // Destroy the frame if user confirms
        saveDataFlight("C:\\Users\\KodomoTachi\\Desktop\\wx_cmake_fetchcontent_template\\data.txt");
        Destroy();
        mainWin->Show(true);
    }
    else
    {
        // Veto the event to prevent closing
        event.Veto();
    }
}

void FlightManagement::Create_add_session(wxCommandEvent &event)
{
    FlightManagement_add_session *flightManagement_add_session = new FlightManagement_add_session("Edit data", this);

    flightManagement_add_session->Center();
    flightManagement_add_session->Show(true);
    this->Hide();
}

void FlightManagement::Create_edit_session(wxCommandEvent &event)
{
    FlightManagement_edit_session *flightManagement_edit_session = new FlightManagement_edit_session("Edit data", this);
    flightManagement_edit_session->Center();
    flightManagement_edit_session->Show(true);
    flightManagement_edit_session->getValue(cnt);
    this->Hide();
}

FlightManagement_add_session::FlightManagement_add_session(const wxString &title, FlightManagement *main_window)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720)), main_window(main_window)
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create three wxTextCtrl controls
    textCtrl1 = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    textCtrl2 = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    textCtrl3 = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));

    // "Submit" button
    submit = new wxButton(panel, wxID_ANY, "Submit", wxDefaultPosition, wxSize(200, 30));

    // Create a vertical box sizer to hold the text controls
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Add some space at the top
    vbox->Add(0, 0, 1, wxEXPAND);

    // Add the text controls to the sizer with center alignment
    vbox->Add(textCtrl1, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(textCtrl2, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(textCtrl3, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(submit, 0, wxALIGN_CENTER | wxALL, 10);

    // Add some space at the bottom
    vbox->Add(0, 0, 1, wxEXPAND);

    // Set the sizer for the panel
    panel->SetSizer(vbox);

    submit->Bind(wxEVT_BUTTON, &FlightManagement_add_session::submitClicked, this);
    Bind(wxEVT_CLOSE_WINDOW, &FlightManagement_add_session::onClose, this);
}

void FlightManagement_add_session::onClose(wxCloseEvent &event)
{
    // Perform custom action before closing
    wxMessageDialog *dlg = new wxMessageDialog(this, "Are you sure you want to close this window?", "Confirm Exit", wxYES_NO | wxICON_QUESTION);
    if (dlg->ShowModal() == wxID_YES)
    {
        // Destroy the frame if user confirms
        Destroy();
        main_window->Show(true);
    }
    else
    {
        // Veto the event to prevent closing
        event.Veto();
    }
}

void FlightManagement_add_session::submitClicked(wxCommandEvent &event)
{
    wxString text_value_1 = textCtrl1->GetValue();
    wxString text_value_2 = textCtrl2->GetValue();
    wxString text_value_3 = textCtrl3->GetValue();

    if (!check_before_add_plane(text_value_1.ToStdString(), text_value_2.ToStdString(), text_value_3.ToStdString()))
        wxMessageBox("Input data error\nNote:\nPlaneID cannot be empty and must have a maximum of 15 characters,\nAircraft type cannot be empty and must have a maximum of 40 characters,\nSeat count must contain only numbers and must be 20 or more", "Info", wxOK | wxICON_INFORMATION, this);
    else
    {
        if (!check_numberOfSeats_before_add(text_value_3.ToStdString()))
            wxMessageBox("Numer of seats is not in range (note: number of seats from 20 to 300)", "Info", wxOK | wxICON_INFORMATION, this);
        else
        {
            addFlight(text_value_1.ToStdString(), text_value_2.ToStdString(), std::stoi(text_value_3.ToStdString()));
            main_window->InitializeListCtrl(); // temporary
            wxMessageBox("Successfully", "Info", wxOK | wxICON_INFORMATION, this);
        }
    }
}

FlightManagement_edit_session::FlightManagement_edit_session(const wxString &title, FlightManagement *main_window)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720)), main_window(main_window)
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create three wxTextCtrl controls
    textCtrl1 = new wxTextCtrl(panel, wxID_ANY, tmp1, wxDefaultPosition, wxSize(200, 30));
    textCtrl2 = new wxTextCtrl(panel, wxID_ANY, tmp2, wxDefaultPosition, wxSize(200, 30));
    textCtrl3 = new wxTextCtrl(panel, wxID_ANY, tmp3, wxDefaultPosition, wxSize(200, 30));

    // "Submit" button
    submit = new wxButton(panel, wxID_ANY, "Submit", wxDefaultPosition, wxSize(200, 30));

    // Create a vertical box sizer to hold the text controls
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Add some space at the top
    vbox->Add(0, 0, 1, wxEXPAND);

    // Add the text controls to the sizer with center alignment
    vbox->Add(textCtrl1, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(textCtrl2, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(textCtrl3, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(submit, 0, wxALIGN_CENTER | wxALL, 10);

    // Add some space at the bottom
    vbox->Add(0, 0, 1, wxEXPAND);

    // Set the sizer for the panel
    panel->SetSizer(vbox);

    // submit->Bind(wxEVT_BUTTON, &FlightManagement_edit_session::submitClicked, this);
    Bind(wxEVT_CLOSE_WINDOW, &FlightManagement_edit_session::onClose, this);
}

void FlightManagement_edit_session::getValue(int cnt)
{
    Flight* tmp = search(cnt);

    wxString tmp_tmp1(tmp->planeId);
    wxString tmp_tmp2(tmp->type);
    wxString tmp_tmp3(std::to_string(tmp->seats));

    tmp1 = tmp_tmp1;
    tmp2 = tmp_tmp2; 
    tmp3 = tmp_tmp3;
}

void FlightManagement_edit_session::onClose(wxCloseEvent &event)
{
    // Perform custom action before closing
    wxMessageDialog *dlg = new wxMessageDialog(this, "Are you sure you want to close this window?", "Confirm Exit", wxYES_NO | wxICON_QUESTION);
    if (dlg->ShowModal() == wxID_YES)
    {
        // Destroy the frame if user confirms
        Destroy();
        main_window->Show(true);
    }
    else
    {
        // Veto the event to prevent closing
        event.Veto();
    }
}