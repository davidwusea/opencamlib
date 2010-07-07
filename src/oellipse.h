/*  $Id$
 * 
 *  Copyright 2010 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 *  
 *  This file is part of OpenCAMlib.
 *
 *  OpenCAMlib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCAMlib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenCAMlib.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OELLIPSE_H
#define OELLIPSE_H

//#include <iostream>
#include <list>
#include "point.h"
#include "epos.h"

namespace ocl
{
    
/// An Ellipse. 
class Ellipse {
    public:
        /// dummy constructor
        Ellipse(){}; 
        /// create an Ellipse with centerpoint center, X-axis a, Y-axis b, and offset distance offset.
        Ellipse(Point& centerin, double a, double b, double offset);
        
        /// string repr
        friend std::ostream &operator<<(std::ostream &stream, const Ellipse& e);
        
        /// return a point on the ellipse at given Epos
        Point ePoint(Epos& position);
        
        /// return a point on the offset-ellipse at given Epos
        Point oePoint(Epos& position);
         
        /// return a normalized normal vector of the ellipse at the given Epos
        Point normal(Epos& position);
        
        /// return a normalized tangent vector to the ellipse at the given Epos
        Point tangent(Epos& position);
        
        /// offset-ellipse solver
        static int solver(Ellipse& e, Point& p); // why static?
        
        /// error function for the solver
        double error(Epos& position, Point& p);
        
        /// calculate ellipse center
        Point calcEcenter(Point& cl, Point& up1, Point& up2, int sln);
        
        /// the center point of the ellipse
        Point center;
        
        /// a-axis
        double a;
        /// b-axis
        double b;
        /// eccentricity = a/b
        double eccen;
        /// offset
        double offset;
        
        /// first Epos solution found by solver()
        Epos epos1;
        /// second Epos solution found by solver()
        Epos epos2;
        
};

} // end namespace
#endif
// end file oellipse.h