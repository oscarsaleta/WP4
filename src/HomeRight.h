/*  P4 (Polynomial Planar Phase Portraits) WEB VERSION SOURCE CODE
 *  Software to study polynomial planar differential systems and represent
 *  their phase portrait in several spaces, such as Poincaré sphere. 
 *  URL: http://github.com/oscarsaleta/P4Web
 *
 *  Copyright (C) 2016  O. Saleta
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HOMERIGHT_H
#define HOMERIGHT_H

#include "win_sphere.h"

#include <Wt/WContainerWidget>

#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WString>
#include <Wt/WTabWidget>
#include <Wt/WTextArea>
#include <Wt/WToolBar>

/** 
 * This class holds the UI from the right side of the website
 * 
 * @class HomeRight
 * 
 * This class derives from a WContainerWidget, and it serves as a container
 * for a WTabWidget (which is, in turn, a container widget organised by tabs).
 *
 * There are three tabs: <b>Output</b>, <b>Plot</b> and <b>Legend</b>.
 *
 * The <b>Output</b> tab has a text area where output generated by Maple is shown 
 * after an evaluation of a vector field, or where messages for the user
 * are printed. It also has 4 buttons: <i>Full results</i>, <i>Finite</i>,
 * <i>Infinite</i> and <i>Clear</i>. The three first buttons control
 * which results are shown in the output area, while the 4th button clears it.
 *
 * The <b>Plot</b> tab has a WPaintedWidget, which is a widget in which we can paint.
 * This tab holds the Poincaré sphere plots for the phase portraits of the
 * evaluated vector field. The plotting work is done by the #WWinSphere class,
 * which is instantiated inside HomeRight by a private @c WWinSphere @c sphere_
 * object.
 *
 * The <b>Legend</b> tab simply holds a static image picked from the original P4.
 * It shows the legend of the plots (kinds of singular points, etc).
 *
 * There are three methods in this class that are designed to act
 * upon the receival of a signal from #HomeLeft.
 */
class HomeRight : public Wt::WContainerWidget
{
public:
    /** 
     * Constructor method for HomeRight
     */
    HomeRight(Wt::WContainerWidget *parent = 0);
    /**
     * Destructor method for HomeRight
     */
    ~HomeRight();

    /**
     * Read results from Maple .res files
     *
     * This method is linked in #MainUI to the evaluated signal from #HomeLeft,
     * so when a vector field is evaluated, HomeRight automatically displays
     * the results of the computations in the output text area.
     * 
     * @param fname Filename where Maple has written the results of the vector
     * field evaluation
     */
    void readResults(std::string fname);

    /**
     * Print a custom message in the output area
     *
     * This method does not only print errors, even though the name suggests
     * so. It can also be used to print any other string in the output area,
     * such as suggestions or warnings.
     * 
     * @param message String that will be printed in the output area
     */
    void printError(std::string message);

    /**
     * Method that triggers a plot in the plot tab
     *
     * This method is connected to the Plot button of HomeLeft by the onPlot
     * signal. Here there is no "plotting" work, everything is handled by
     * #WWinSphere. This method only triggers an @c update of the @c sphere_
     * object.
     * 
     * @param basename This is a filename for the Maple results file. The
     * #WWinSphere class needs this file to parse it and paint accordingly.
     */
    void onPlot(std::string basename);

private:
    Wt::WTabWidget *tabWidget_;

    Wt::WContainerWidget *outputContainer_;
    Wt::WTextArea *outputTextArea_;
    Wt::WString outputTextAreaContent_;
    Wt::WToolBar *outputButtonsToolbar_;
    Wt::WPushButton *fullResButton_;
    Wt::WPushButton *finResButton_;
    Wt::WPushButton *infResButton_;
    Wt::WPushButton *clearOutputButton_;

    std::string fileName_;
    std::string fullResults_;
    std::string finResults_;
    std::string infResults_;

    WWinSphere *sphere_;

    Wt::WContainerWidget *plotContainer_;
    /*Wt::WToolBar *plotButtonsToolbar_;
    Wt::WPushButton *clearPlotButton_;
    Wt::WPushButton *plotPointsButton_;
    Wt::WPushButton *plotSeparatricesButton_;*/

    Wt::WContainerWidget *legendContainer_;
    Wt::WImage *legend_;

    void setupUI();
    void setupConnectors();

    void fullResults();
    void showFinResults();
    void showInfResults();
    void clearResults();

    /*void plotSingularPoints();
    void plotSeparatrices();
    void clearPlot();*/
    
};

#endif // HOMERIGHT_H