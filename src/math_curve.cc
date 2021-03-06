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

#include "WSphere.h"

#include "MyLogger.h"
#include "ScriptHandler.h"
#include "custom.h"
#include "math_p4.h"
#include "plot_tools.h"

#include <cmath>

// static global variables
static int s_CurveDashes = 1;

// function definitions
bool WSphere::evalCurveStart(std::string fname, int dashes, int points,
                             int precis)
{
    if (study_->plweights_)
        curveTask_ = EVAL_CURVE_LYP_R2;
    else
        curveTask_ = EVAL_CURVE_R2;

    curveError_ = false;
    s_CurveDashes = dashes;
    if (runTaskCurve(fname, curveTask_, points, precis) < 0)
        return false;
    return true;
}

// returns true when finished.  Then run EvalGCfFinish to see if error occurred
// or not
bool WSphere::evalCurveContinue(std::string fname, int points, int prec)
{
    if (curveTask_ == EVAL_CURVE_NONE)
        return true;

    if (!readTaskCurveResults(fname, curveTask_)) {
        curveError_ = true;
        g_globalLogger.error("[WSphere] error at curve readTaskCurveResults");
        return true;
    }
    curveTask_++;
    if (curveTask_ == EVAL_CURVE_FINISHPOINCARE ||
        curveTask_ == EVAL_CURVE_FINISHLYAPUNOV) {
        return true;
    }

    if (runTaskCurve(fname, curveTask_, points, prec) < 0) {
        curveError_ = true;
        g_globalLogger.error("[WSphere] error at curve runTaskCurve");
        return true;
    }

    return false; // still busy
}

bool WSphere::evalCurveFinish(void) // return false in case an error occured
{
    if (curveTask_ != EVAL_CURVE_NONE) {
        curveTask_ = EVAL_CURVE_NONE;
        if (curveError_) {
            curveError_ = false;
            return false;
        }
    }
    return true;
}

int WSphere::runTaskCurve(std::string fname, int task, int points, int prec)
{
    bool value;

    g_globalLogger.debug("[WSphere] will run curve task=" +
                         std::to_string(task) + " using file=" + fname);
    // TODO: prepareCurve functions
    switch (task) {
    case EVAL_CURVE_R2:
        value = scriptHandler_->prepareCurve(
            fname, study_->curve_vector_.back().r2, -1, 1, prec, points);
        break;
    case EVAL_CURVE_U1:
        value = scriptHandler_->prepareCurve(
            fname, study_->curve_vector_.back().u1, 0, 1, prec, points);
        break;
    case EVAL_CURVE_V1:
        value = scriptHandler_->prepareCurve(
            fname, study_->curve_vector_.back().u1, -1, 0, prec, points);
        break;
    case EVAL_CURVE_U2:
        value = scriptHandler_->prepareCurve(
            fname, study_->curve_vector_.back().u2, 0, 1, prec, points);
        break;
    case EVAL_CURVE_V2:
        value = scriptHandler_->prepareCurve(
            fname, study_->curve_vector_.back().u2, -1, 0, prec, points);
        break;
    case EVAL_CURVE_LYP_R2:
        value = scriptHandler_->prepareCurve_LyapunovR2(
            fname, study_->curve_vector_.back().r2, prec, points);
        break;
    case EVAL_CURVE_CYL1:
        value = scriptHandler_->prepareCurve_LyapunovCyl(
            fname, study_->curve_vector_.back().c, -PI_DIV4, PI_DIV4, prec,
            points);
        break;
    case EVAL_CURVE_CYL2:
        value = scriptHandler_->prepareCurve_LyapunovCyl(
            fname, study_->curve_vector_.back().c, PI_DIV4, PI - PI_DIV4, prec,
            points);
        break;
    case EVAL_CURVE_CYL3:
        value = scriptHandler_->prepareCurve_LyapunovCyl(
            fname, study_->curve_vector_.back().c, PI - PI_DIV4, PI + PI_DIV4,
            prec, points);
        break;
    case EVAL_CURVE_CYL4:
        value = scriptHandler_->prepareCurve_LyapunovCyl(
            fname, study_->curve_vector_.back().c, -PI + PI_DIV4, -PI_DIV4,
            prec, points);
        break;
    default:
        value = false;
        break;
    }

    if (value)
        return scriptHandler_->evaluateMapleScript(fname, 60).si_status;
    else
        return -1;
}

bool WSphere::readTaskCurveResults(std::string fname, int task)
{
    g_globalLogger.debug("[WSphere] called readTaskCurveResults with fname=" +
                         fname + " and task=" + std::to_string(task));
    bool value;

    switch (task) {
    case EVAL_CURVE_R2:
        value = read_curve(fname, &WVFStudy::R2_to_psphere);
        break;
    case EVAL_CURVE_U1:
        value = read_curve(fname, &WVFStudy::U1_to_psphere);
        break;
    case EVAL_CURVE_V1:
        value = read_curve(fname, &WVFStudy::VV1_to_psphere);
        break;
    case EVAL_CURVE_U2:
        value = read_curve(fname, &WVFStudy::U2_to_psphere);
        break;
    case EVAL_CURVE_V2:
        value = read_curve(fname, &WVFStudy::VV2_to_psphere);
        break;
    case EVAL_CURVE_LYP_R2:
        value = read_curve(fname, &WVFStudy::rplane_plsphere0);
        break;
    case EVAL_CURVE_CYL1:
        value = read_curve(fname, &WVFStudy::cylinder_to_plsphere);
        break;
    case EVAL_CURVE_CYL2:
        value = read_curve(fname, &WVFStudy::cylinder_to_plsphere);
        break;
    case EVAL_CURVE_CYL3:
        value = read_curve(fname, &WVFStudy::cylinder_to_plsphere);
        break;
    case EVAL_CURVE_CYL4:
        value = read_curve(fname, &WVFStudy::cylinder_to_plsphere);
        break;
    default:
        value = false;
    }

    return value;
}

void WSphere::draw_curve(orbits_points *sep, int color, int dashes)
{
    double pcoord[3];

    while (sep != nullptr) {
        if (sep->dashes && dashes)
            (*plot_l)(this, pcoord, sep->pcoord, color);
        else
            (*plot_p)(this, sep->pcoord, color);
        copy_x_into_y(sep->pcoord, pcoord);

        sep = sep->next_point;
    }
}

void WVFStudy::insert_curve_point(double x0, double y0, double z0, int dashes)
{
    if (curve_vector_.back().points != nullptr) {
        last_curves_point_->next_point = new orbits_points;
        last_curves_point_ = last_curves_point_->next_point;
    } else {
        last_curves_point_ = new orbits_points;
        curve_vector_.back().points = last_curves_point_;
    }

    last_curves_point_->pcoord[0] = x0;
    last_curves_point_->pcoord[1] = y0;
    last_curves_point_->pcoord[2] = z0;

    last_curves_point_->dashes = dashes;
    last_curves_point_->color = CCURV;
    last_curves_point_->next_point = nullptr;
}

bool WSphere::read_curve(std::string fname,
                         void (WVFStudy::*chart)(double, double, double *))
{
    int k;
    FILE *fp;
    double x, y;
    double pcoord[3];
    int d, c;

    fp = fopen(std::string(fname + "_curve.tab").c_str(), "r");
    if (fp == nullptr) {
        g_globalLogger.debug("[WSphere] cannot open file " +
                             std::string(fname + "_curve.tab") +
                             " for reading");
        return false;
    }

    k = 0;
    while (1) {
        d = 0;
        while (fscanf(fp, "%lf %lf", &x, &y) == 2) {
            k++;
            (study_->*chart)(x, y, pcoord);
            study_->insert_curve_point(pcoord[0], pcoord[1], pcoord[2], d);
            // d=1;
            d = s_CurveDashes;
        }
        for (c = getc(fp); isspace(c);)
            c = getc(fp);
        if (c != ',')
            break;
    }

    fclose(fp);
    return true;
}
