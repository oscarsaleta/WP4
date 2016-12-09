#ifndef HOMELEFT_H
#define HOMELEFT_H

#include <Wt/WContainerWidget>
#include <Wt/WSignal>

class HomeLeft : public Wt::WContainerWidget
{
public:
    HomeLeft(Wt::WContainerWidget *parent = 0);
    ~HomeLeft();

    Wt::Signal<int, std::string>& evaluated();
    
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

    Wt::WAnchor *saveButton_;
    std::string saveFileName_;
    Wt::WFileResource *saveFileResource_;

    void fileUploaded();
    void fileTooLarge();

    std::string openTempStream(std::string, std::string, std::ofstream&);
    void prepareMapleFile();
    void fillMapleScript(std::string, std::ofstream&);
    void evaluate();

    void saveFile();
    void prepareSaveFile();

    Wt::Signal<int, std::string> evaluated_;


};

#endif // HOMELEFT_H