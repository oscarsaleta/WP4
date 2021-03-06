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

#ifndef MATH_POLYNOM_H
#define MATH_POLYNOM_H

/*!
 * @brief Polynomial related functions
 * @file math_polynom.h
 */

#include "file_tab.h"

/**
 * Calculates p(t) for a polynomial p and a value t
 * @param  p     Polynomial p
 * @param  value Value t
 * @return       Result p(t)
 */
double eval_term1(P4POLYNOM1 p, double value);
/**
 * Calculates f(x,y) for a polynomial f and values x and y.
 * @param  f     Polynomial f
 * @param  value Array (x,y)
 * @return       Result f(x,y)
 */
double eval_term2(P4POLYNOM2 f, double *value);
/**
 * Calculates F( r, cos(theta), sin(theta) ) for a polynomial F
 * and values of r and theta
 * @param  F     Polynomial F
 * @param  value Array (r,theta)
 * @return       Result F(r,cos(theta),sin(theta))
 */
double eval_term3(P4POLYNOM3 F, double *value);

/**
 * Delete a one variable polynomial
 * @param p polynomial
 */
void delete_term1(P4POLYNOM1 p);
/**
 * Delete a two variables polynomial
 * @param p polynomial
 */
void delete_term2(P4POLYNOM2 p);
/**
 * Delete a three variables polynomial
 * @param p polynomial
 */
void delete_term3(P4POLYNOM3 p);

/**
 * Used for creating GCF files
 * @param buf       string where result is stored
 * @param f         linked list of terms a*x^i*y^j
 * @param isfirst   indicates whether the
 * @param x         name of "x" variable
 * @param y         name of "y" variable
 */
char *printterm2(char *buf, P4POLYNOM2 f, bool isfirst, const char *x,
                 const char *y);
/**
 * Used for creating GCF files
 * @param buf string where result is stored
 * @param f linked list of terms a*r^i*cos(theta1)^j*sin(theta2)^k
 * @param isfirst
 * @param r
 * @param Co
 * @param Si
 */
char *printterm3(char *buf, P4POLYNOM3 f, bool isfirst, const char *r,
                 const char *Co, const char *Si);

#endif // MATH_POLYNOM_H
