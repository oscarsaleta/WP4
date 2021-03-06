/*  This file is part of WP4 (http://github.com/oscarsaleta/WP4)
 *
 *  Copyright (C) 2016  O. Saleta
 *
 *  WP4 is free software: you can redistribute it and/or modify
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
/*  This file is part of P4
 *
 *  Copyright (C) 1996-2016  J.C. Artés, C. Herssens, P. De Maesschalck,
 *                           F. Dumortier, J. Llibre, O. Saleta
 *
 *  P4 is free software: you can redistribute it and/or modify
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

#ifndef WIN_SPHERE_H
#define WIN_SPHERE_H

/*!
 * @brief This file implements the class WSphere
 * @file WSphere.h
 *
 * The WSphere class holds a WVFStudy object and also
 * contains all the information and methods needed for plotting
 * the contents of the study (the output from Maple and also
 * the separatrices computed numerically) in a plane or sphere
 * projection.
 */

#include "ScriptHandler.h"
#include "custom.h"
#include "file_tab.h"

#include <Wt/WContainerWidget>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>
#include <Wt/WPointF>

#define EVAL_GCF_NONE 0            ///< no gcf evaluation
#define EVAL_GCF_R2 1              ///< gcf evaluation in R^2
#define EVAL_GCF_U1 2              ///< gcf evaluation in U1
#define EVAL_GCF_U2 3              ///< gcf evaluation in U2
#define EVAL_GCF_V1 4              ///< gcf evaluation in V1
#define EVAL_GCF_V2 5              ///< gcf evaluation in V2
#define EVAL_GCF_FINISHPOINCARE 6  ///< finish gcf evaluation in sphere
#define EVAL_GCF_LYP_R2 7          ///< gcf evaluation in R^2 with PL weights
#define EVAL_GCF_CYL1 8            ///< gcf evaluation in the cylinder
#define EVAL_GCF_CYL2 9            ///< gcf evaluation in the cylinder
#define EVAL_GCF_CYL3 10           ///< gcf evaluation in the cylinder
#define EVAL_GCF_CYL4 11           ///< gcf evaluation in the cylinder
#define EVAL_GCF_FINISHLYAPUNOV 12 ///< finish gcf evaluation with PL weights

#define GCF_DASHES 1  ///< gcf dashes is 1 by default
#define GCF_POINTS 40 ///< gcf npoints is 40 by default
#define GCF_PRECIS 12 ///< gcf precision is 12 by default

#define EVAL_CURVE_NONE 0           ///< no curve evaluation
#define EVAL_CURVE_R2 1             ///< curve evaluation in R^2
#define EVAL_CURVE_U1 2             ///< curve evaluation in U1
#define EVAL_CURVE_U2 3             ///< curve evaluation in U2
#define EVAL_CURVE_V1 4             ///< curve evaluation in V1
#define EVAL_CURVE_V2 5             ///< curve evaluation in V2
#define EVAL_CURVE_FINISHPOINCARE 6 ///< finish curve evaluation in sphere
#define EVAL_CURVE_LYP_R2 7         ///< curve evaluation in R^2 with PL weights
#define EVAL_CURVE_CYL1 8           ///< curve evaluation in the cylinder
#define EVAL_CURVE_CYL2 9           ///< curve evaluation in the cylinder
#define EVAL_CURVE_CYL3 10          ///< curve evaluation in the cylinder
#define EVAL_CURVE_CYL4 11          ///< curve evaluation in the cylinder
#define EVAL_CURVE_FINISHLYAPUNOV 12 ///< finish curve with PL weights

#define CURVE_DASHES 1   ///< curve dashes is 1 by default
#define CURVE_POINTS 400 ///< curve npoints is 400 by default
#define CURVE_PRECIS 12  ///< curve precision is 12 by default

//#define SELECTINGPOINTSTEPS         5
//#define SELECTINGPOINTSPEED         150

/**
 * Sphere class, which performs the plotting work
 * @class WSphere
 *
 * This class derives from Wt::WPaintedWidget, so it is in fact
 * a widget in which we can paint.
 *
 * It also contains an object of the class WVFStudy, because
 * this is the object that parses Maple results and therefore
 * has all the information about what has to be plotted.
 *
 * In P4, the QVFStudy (WVFStudy here) object was originally a global
 * object called VFResults. This does not work for us, because we need
 * several studies and different plots to be executed at the same time
 * by different users of P4Web. This means that a single global object
 * causes problems (each new vector field study modifies that object
 * and renders other sessions unusable).
 *
 * This is why we made that every WSphere has its own WVFStudy,
 * this way there are no conflicts.
 */
class WSphere : public Wt::WPaintedWidget
{
  public:
    /**
     * Constructor method for a spherical plot
     * @param parent       container widget which created the sphere
     * @param width         width of the painting area
     * @param height        height of the painting area
     * @param basename      name of the file that contains Maple output for the
     * current vector field
     * @param projection    projection for the sphere
     */
    WSphere(Wt::WContainerWidget *parent = 0, ScriptHandler *s = 0,
            int width = 255, int height = 255, std::string basename = "",
            double projection = -1.0, WVFStudy *study = 0);
    /**
     * Constructor method for a planar (or chart) plot
     * @param parent   container widget which created the sphere
     * @param width     width of the painting area
     * @param height    height of the painting area
     * @param basename  name of the file that contains Maple output for the
     * current vector field
     * @param type      type of view
     * @param minx      minimum x for plot
     * @param maxx      maximum x for plot
     * @param miny      minimum y for plot
     * @param maxy      maximum y for plot
     */
    WSphere(Wt::WContainerWidget *parent = 0, ScriptHandler *s = 0,
            int width = 255, int height = 255, std::string basename = "",
            int type = 1, double minx = -1, double maxx = 1, double miny = -1,
            double maxy = 1, WVFStudy *study = 0);
    /**
     * Destructor method
     */
    ~WSphere();

    int width_;       ///< width of the plotting area
    int height_;      ///< height of the plotting area
    WVFStudy *study_; ///< WVFStudy object which will parse results from Maple
    std::string basename_; ///< filename where Maple output is stored
    int typeOfView_;       ///< type of view for *study_
    int projection_;       ///< used for sphere view
    double viewMinX_;      ///< used for plane view
    double viewMaxX_;      ///< used for plane view
    double viewMinY_;      ///< used for plane view
    double viewMaxY_;      ///< used for plane view

    /**
     * X Coordinate change: from world (double) to window (int) coordinates
     * @param  x coordinate to transform
     * @return   tranformed coordinate
     */
    int coWinX(double x);
    /**
    * Y Coordinate change: from world (double) to window (int) coordinates
    * @param  y coordinate to transform
    * @return   transformed coordinate
    */
    int coWinY(double y);
    /**
     * X Coordinate change: from window (int) to world (double) coordinates
     * @param  x coordinate to transform
     * @return   tranformed coordinate
     */
    double coWorldX(int x);
    /**
     * Y Coordinate change: from (int) to world (double) coordinates
     * @param  y coordinate to transform
     * @return   transformed coordinate
     */
    double coWorldY(int y);
    /**
     * Horizontal distance tranformation: from world (double) to window (int)
     * @param  deltax distance to transform
     * @return        tranformed distance
     */
    int coWinH(double deltax);
    /**
     * Vertical distance transformation: from world (double) to window (int)
     * @param  deltay distance to transform
     * @return        transformed distance
     */
    int coWinV(double deltay);

    double x0; ///< world-coordinates of upper-left corner
    double y0; ///< world-coordinates of upper-left corner
    double x1; ///< world-coordinates of upper-right corner
    double y1; ///< world-coordinates of upper-right corner
    double dx; ///< x1-x0
    double dy; ///< y1-y0

    int paintedXMin; /**< to know the update rectangle after painting
                     we keep to smallest rectangle enclosing
                     all painted objects. */
    int paintedXMax; /**< to know the update rectangle after painting
                     we keep to smallest rectangle enclosing
                     all painted objects. */
    int paintedYMin; /**< to know the update rectangle after painting
                     we keep to smallest rectangle enclosing
                     all painted objects. */
    int paintedYMax; /**< to know the update rectangle after painting
                     we keep to smallest rectangle enclosing
                     all painted objects. */

    /**
     * string that identifies which chart we're in
     */
    Wt::WString chartString_;
    /**
     * string that shows type of view and cursor coordinates
     */
    Wt::WString plotCaption_;

    /**
     * background color
     */
    int spherebgcolor;
    /**
     * next WSphere (linked list)
     */
    WSphere *next;
    // int SelectingX, SelectingY, SelectingPointStep, SelectingPointRadius;
    // QTimer * SelectingTimer;

    /**
     * Get chart coordinates for a point
     * @param  chart Which chart (defined in custom.h)
     * @param  x1    Coordinate 1 of point
     * @param  y1    Coordinate 2 of point
     * @param  pos   Array where coordinates are stored
     * @return       Always @c true
     */
    bool getChartPos(int chart, double x1, double y1, double *pos);

    /**
     * Plot a saddle
     * @param p saddle struct
     */
    void plotPoint(saddle *p);
    /**
     * Plot a node
     * @param p node struct
     */
    void plotPoint(node *p);
    /**
     * Plot a semi-elementary singularity
     * @param p semi-elementary singularity struct
     */
    void plotPoint(semi_elementary *p);
    /**
     * Plot a weak focus
     * @param p weak focus struct
     */
    void plotPoint(weak_focus *p);
    /**
     * Plot a strong focus
     * @param p strong focus struct
     */
    void plotPoint(strong_focus *p);
    /**
     * Plot a degenerate singularity
     * @param p degenerate singularity struct
     */
    void plotPoint(degenerate *p);
    /**
     * Plot separatrices around a semi-elementary singular point
     * @param p semi-elementary singularity struct
     */
    void plotPointSeparatrices(semi_elementary *p);
    /**
     * Plot separatrices around a saddle
     * @param p saddle struct
     */
    void plotPointSeparatrices(saddle *p);
    /**
     * Plot separatrices around a degenerate singular point
     * @param p degenerate singularity struct
     */
    void plotPointSeparatrices(degenerate *p);
    /**
     * Plot all singularities
     */
    void plotPoints(void);
    /**
     * Plot all separatrices
     */
    void plotSeparatrices(void);
    /**
     * Plot Poincaré sphere (circle at infinity)
     */
    void plotPoincareSphere(void);
    /**
     * Plot Poincaré-Lyapunov sphere (circle at infinity)
     */
    void plotPoincareLyapunovSphere(void);
    /**
     * Plot line at infinity
     */
    void plotLineAtInfinity(void);

    // void MarkSelection( int x1, int y1, int x2, int y2, int selectiontype );

    /**
     * Produce a linked list of lines with the shape of an ellipse
     * @param  cx     x coordinate of center
     * @param  cy     y coordinate of center
     * @param  a      major semi axis
     * @param  b      minor semi axis
     * @param  dotted is the ellipse dotted or a line
     * @param  resa   @p a converted to window length
     * @param  resb   @p b converted to window length
     * @return        linked list of lines that form the ellipse
     */
    P4POLYLINES *produceEllipse(double cx, double cy, double a, double b,
                                bool dotted, double resa, double resb);

    /**
     * Draw a point with a given color
     * @param x     point is (x,y)
     * @param y     point is (x,y)
     * @param color color (defined in color.h)
     */
    void drawPoint(double x, double y, int color);
    /**
     * Draw a line given by two points with a given color
     * @param x1    point 1 is (x1,y1)
     * @param y1    point 1 is (x1,y1)
     * @param x2    point 2 is (x2,y2)
     * @param y2    point 2 is (x2,y2)
     * @param color color (defined in color.h)
     */
    void drawLine(double x1, double y1, double x2, double y2, int color);

    /**
     * Setup everything before starting to plot
     *
     * This function triggers a parse of the Maple results file,
     * which fills the WVFStudy object member of this class. It
     * also sets up the WVFStudy pointer-to-member functions to
     * point at the correct functions for the selected configuration
     * of the study (correct view, sphere, etc). Then, it deletes
     * any remaining circles or lines at infinity and creates a
     * new one depending on the configuration.
     *
     * @return @c true if setup was successful, @c false if results could
     * not be read
     */
    bool setupPlot(void);
    // void updatePointSelection( void );

    /**
     * Start orbit integration in a given direction
     *
     * Implemented in math_orbits.cc
     *
     * @param dir -1, 1 indicates direction, 0 tells the function to continue
     */
    void integrateOrbit(int dir);
    /**
     * Start orbit integration from a point
     * @param  x x coordinate of starting point
     * @param  y y coordinate of starting point
     * @param  R if 0 then point selected in the drawing canvas else in the
     * orbit window
     * @return   @c true if ok, @c false if orbit is already integrated
     */
    bool startOrbit(double x, double y, bool R);
    /**
     * Delete last integrated orbit from @c study_ and the plot
     */
    void deleteLastOrbit(void);

    /**
     * React to a mouse hover event to set a string
     *
     * This function takes the coordinates of the mouse cursor,
     * transforms them to the current view (poincaré sphere, p-l,
     * plane, or one of the charts) and sets plotCaption_ to reflect
     * these coordinates.
     *
     * @param e WMouseEvent, contains the coordinates of the mouse cursor
     */
    void mouseMovementEvent(Wt::WMouseEvent e);

    /**
     * React to a mouse click event to emit the coordinates
     *
     * This function takes the coordinates of the mouse cursor
     * where the click has been performed, and sends them to the
     * parent widget (HomeRight)
     *
     * @param e WMouseEvent, contains the coordinates of the mouse cursor
     */
    void mouseClickEvent(Wt::WMouseEvent e);

    /**
     * Method that sends a signal when mouse is hovered over plot region
     *
     * This is used for printing mouse coordinates in the plot caption
     *
     * @return the signal
     */
    Wt::Signal<Wt::WString> &hoverSignal() { return hoverSignal_; }
    /**
     * Method that sends a signal when user clicks on plot region
     *
     * This is used for selecting a point where orbit integration has
     * to start
     *
     * @return the signal
     */
    Wt::Signal<bool, double, double> &clickedSignal() { return clickedSignal_; }

    /**
     * Method that sends a signal to print some message
     *
     * The message is written in the output text area from #HomeRight
     *
     * @return the signal
     */
    Wt::Signal<std::string> &errorSignal() { return errorSignal_; }

    /**
     * Pointer to a painter linked to a paint device created in a paint event.
     * This makes possible to distribute painting to different functions and
     * compiling units (even from outside the object)
     */
    Wt::WPainter *staticPainter;

    /**
     * Flag used to not replot every time we just want to update something
     */
    bool plotDone_;

    /**
     * Flag used to make the sphere compute gcf
     */
    bool gcfEval_;
    /**
     * Name of Maple script from first execution, to be
     * reused for gcf
     */
    std::string gcfFname_;
    /**
     * Number of points for gcf
     */
    int gcfNPoints_;
    /**
     * Precision of zeros for gcf
     */
    int gcfPrec_;
    /**
     * Points (0) or dashes (1) for gcf plot
     */
    int gcfDashes_;

    /**
     * Name of Maple script from first execution, to be
     * reused for curve
     */
    std::string curveFname_;
    /**
     * Number of points for curve
     */
    int curveNPoints_;
    /**
     * Precision of zeros for curve
     */
    int curvePrec_;
    /**
     * Points (0) or dashes (1) for curve plot
     */
    int curveDashes_;
    /**
     * Indicates if there was any error when computing curves
     */
    bool curveError_;

    /**
     * Start curve evaluation
     *
     * @param fname  name of script that will be used for sequential evaluations
     * @param dashes flag to use dashes or dots in plot
     * @param points number of points used in computations
     * @param precis precision used in computations
     * @return       @c true if no error, @c false if error
     */
    bool evalCurveStart(std::string fname, int dashes, int points, int precis);
    /**
     * After starting evaluation, continue with the other charts
     *
     * @param fname  name of script that will be used for sequential evaluations
     * @param points number of points used in computations
     * @param prec   precision used in computations
     * @return       @c true if no error, @c false if error
     */
    bool evalCurveContinue(std::string fname, int points, int prec);
    /**
     * Finish the curve evaluation
     */
    bool evalCurveFinish(void);

    /**
     * Name of Maple script from first execution, to be
     * reused for isocline
     */
    std::string isoclineFname_;
    /**
     * Number of points for isocline
     */
    int isoclineNPoints_;
    /**
     * Precision of zeros for isocline
     */
    int isoclinePrec_;
    /**
     * Points (0) or dashes (1) for isocline plot
     */
    int isoclineDashes_;
    /**
     * Indicates if there was any error when computing isoclines
     */
    bool isoclineError_;

    /**
     * Start isocline evaluation
     *
     * @param fname  name of script that will be used for sequential evaluations
     * @param dashes flag to use dashes or dots in plot
     * @param points number of points used in computations
     * @param precis precision used in computations
     * @return       @c true if no error, @c false if error
     */
    bool evalIsoclineStart(std::string fname, int dashes, int points,
                           int precis);
    /**
     * After starting evaluation, continue with the other charts
     *
     * @param fname  name of script that will be used for sequential evaluations
     * @param points number of points used in computations
     * @param prec   precision used in computations
     * @return       @c true if no error, @c false if error
     */
    bool evalIsoclineContinue(std::string fname, int points, int prec);
    /**
     * Finish the isocline evaluation
     */
    bool evalIsoclineFinish(void);

  protected:
    /**
     * Paint event for this painted widget
     * @param p paint device (passed automatically by the generator of the
     * event)
     *
     * When an update is called upon the painted widget (this), a paint event is
     * generated and this function implements the behavior.
     */
    void paintEvent(Wt::WPaintDevice *p);

  private:
    // signal fired when mouse is hovered over the plot region (for caption)
    Wt::Signal<Wt::WString> hoverSignal_;
    // signal fired when user clicks on plot (for orbits)
    Wt::Signal<bool, double, double> clickedSignal_;
    // signal emitted when there's an error while reading results from Maple
    Wt::Signal<std::string> errorSignal_;

    /**
     * parent widget (stored from @c parent, argument passed to constructor)
     */
    Wt::WContainerWidget *parentWnd;
    /**
     * when calculating coordinates: this determines orientation of horizontal
     * axis.  Normally false, only true when printing.
     */
    bool ReverseYaxis;
    /**
     * linked list of lines that form the Poincaré circle
     */
    P4POLYLINES *CircleAtInfinity;
    /**
     * linked list of lines that form the Poincaré-Lyapunov circle
     */
    P4POLYLINES *PLCircle;

    /**
     * used for plot caption
     */
    void setChartString(int p, int q, bool isu1v1chart, bool negchart);
    /**
     * used for not redoing plot when orbits or other objects are plotted over
     * the original
     */
    bool plotPrepared_;
    /**
     * used for plotting background the first time
     */
    bool firstTimePlot_;
    // integrate orbit from a point and store the result as a linked list
    orbits_points *integrate_orbit(double pcoord[3], double step, int dir,
                                   int color, int points_to_int,
                                   struct orbits_points **orbit);
    // draw orbit starting from a point
    void drawOrbit(double *pcoord, struct orbits_points *points, int color);
    // draw all orbits (calling drawOrbit() for each one)
    void drawOrbits();

    // used for gcf
    bool gcfError_;
    int gcfTask_;
    bool evalGcfStart(std::string fname, int dashes, int points, int precis);
    bool evalGcfContinue(std::string fname, int points, int prec);
    bool evalGcfFinish(void);
    int runTask(std::string fname, int task, int points, int prec);
    void draw_gcf(orbits_points *sep, int color, int dashes);
    void plotGcf(void);
    bool read_gcf(std::string fname,
                  void (WVFStudy::*chart)(double, double, double *));
    bool readTaskResults(std::string fname, int task);

    // used for curves
    int curveTask_;
    int runTaskCurve(std::string fname, int task, int points, int prec);
    void draw_curve(orbits_points *sep, int color, int dashes);
    void plotCurves(void);
    bool read_curve(std::string fname,
                    void (WVFStudy::*chart)(double, double, double *));
    bool readTaskCurveResults(std::string fname, int task);

    // used for isoclines
    int isoclineTask_;
    int runTaskIsocline(std::string fname, int task, int points, int prec);
    void draw_isocline(orbits_points *sep, int color, int dashes);
    void plotIsoclines(void);
    bool read_isocline(std::string fname,
                       void (WVFStudy::*chart)(double, double, double *));
    bool readTaskIsoclineResults(std::string fname, int task);

    // script handler
    ScriptHandler *scriptHandler_;
    // flag to know if study was copied or will be created
    bool studyCopied_;
};

#endif /* WIN_SPHERE_H */
