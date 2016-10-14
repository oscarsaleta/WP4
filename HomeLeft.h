#ifndef __HOMELEFT_H_INCLUDE__
#define __HOMELEFT_H_INCLUDE__

#include <Wt/WContainerWidget>

class HomeLeft : public Wt::WContainerWidget
{
public:
    HomeLeft(Wt::WContainerWidget *parent = 0);
    ~HomeLeft();
    
private:
    Wt::WGroupBox *fileUploadBox_;
    Wt::WFileUpload *fileUploadWidget_;
    Wt::WPushButton *fileUploadButton_;
    std::string fileUploadName_;

    Wt::WGroupBox *equationsBox_;
    Wt::WText *xLabel_;
    Wt::WLineEdit *xEquationInput_;
    Wt::WText *yLabel_;
    Wt::WLineEdit *yEquationInput_;
    Wt::WPushButton *evalButton_;
    Wt::WPushButton *saveButton_;


    void fileUploaded();
    void fileTooLarge();

    void saveFile();
    std::string openTempStream(std::string, std::string, std::ofstream&);
    void prepareMapleFile();
    void fillMapleScript(std::string, std::ofstream&);
    void evaluate();



};

#endif // __HOMELEFT_H_INCLUDE__