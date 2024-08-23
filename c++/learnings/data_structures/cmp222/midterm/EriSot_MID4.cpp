#include <iostream>

using namespace std;

enum SelectState {SS_UNSET, SS_UNCHECKED, SS_CHECKED };
enum ControlType {CT_UNSET = 0, CT_TEXTBOX, CT_BUTTON, CT_CHECK };

class Control {
    public:
        ControlType type;
};

class Textbox : public Control {
    public:
        string text;

        Textbox() { 
            type = CT_TEXTBOX;
            text = ""; 
        }
        Textbox(string t)
        {
            type = CT_TEXTBOX;
            text = t;
        }


        // set text to blank in default constructor
        // set text to parameter value in overloaded constructor
};

class Checkbox : public Control {
    public: 
        SelectState checked;

        Checkbox (){
            type = CT_CHECK;
            checked =SS_UNSET;
        }
        Checkbox(int s)
        {
            type = CT_CHECK;
            checked =SS_CHECKED;
        }


        // default checked to SS_UNSET in constructor
};

class Button : public Control {
    public:
        string caption;
        Button() { 
            type = CT_BUTTON;
        }
        void press()
        {
            cout << "Submitted\n";
        }
};

class Form 
{
    private:
        Control *_controls[100];    // nothing to change on this line
        int _count = 0;
    public:
        bool validate();

        // you need to implement the add functions below
        // these functions will add to the _controls array
        // the control added is passed in via the parameter
        void add_button(Button *btn) {

            _controls[_count] = btn;

            _count++;
        }
        void add_textbox(Textbox *tb) {

            _controls[_count] =tb;

            _count++;

        }
        void add_checkbox(Checkbox *cb) {

            _controls[_count] =cb;

            _count++;

        }
};

// implement Form::validate such that it will return true
// when all textboxes have content in their text property (not blank)
// and all checkboxes are checked
bool Form::validate()
{
    Textbox *tb;
    Checkbox *cb;
    int num_controls =1;       // set this to the real number of controls
    bool result = true;

    for (int i = 0; i < num_controls; i++)
    {
        // check for textbox controls
        if (_controls[i]->type == CT_TEXTBOX)
        {
            // this line makes tb a valid pointer to an element in
            // the _controls array. It casts an element of the 
            // array into an object of type Textbox
            tb = static_cast<Textbox*>(_controls[i]);

            // from this point forward tb is a valid pointer to a Textbox
            // and you can use it to check for blanks
            if(tb->text == "")
            {
                result = false;

                return result;
                break;
            }


        }
            
        if(_controls[i]->type ==CT_CHECK)
        {
            cb = static_cast<Checkbox*>(_controls[i]);
        }
        if(cb->checked == SS_CHECKED)
        {
            result = false;
            return result;
            break;
        }



        // check for checkbox controls
        // you need to fill this out similar to above

    }

    return result;
}

int main()
{
    Form form;
    Button submit;
    Checkbox one(1),two,three(3);
    Textbox onne("idi");
    Textbox twoo("erieie");
    Textbox threee("didiei");

    // add one button - DONE
    form.add_button(&submit);       // notice the reference?

    // add 3 checkboxes to the form
    form.add_checkbox(& one);
    form.add_checkbox(& two);
    form.add_checkbox(& three);

    // add 3 textboxes to the form
    form.add_textbox(& onne);
    form.add_textbox(& twoo);
    form.add_textbox(& threee);
    // to test that your validate fn is working
    // correctly, set different text values for your textboxes above
    // and set different states for your checkboxes above
    // there's nothing to do after this section
    if (form.validate())
        submit.press();
    else
        cout << "Required fields are missing" << endl;

    return 0;
}
