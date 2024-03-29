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

#ifndef CLFILTER_H
#define CLFILTER_H

#include <iostream>
#include <string>
#include <list>

#include "clpoint.h"

namespace ocl
{  

///
/// \brief CL point filter virtual base class
///
class CLFilter { 
    public:
        /// constructor
        CLFilter() {} ;
        virtual ~CLFilter() {};
        
        /// add CLPoint
        virtual void addCLPoint(const CLPoint& p) = 0;
        /// set the tolerance value
        virtual void setTolerance(const double tol) = 0;
        
        /// run filter
        virtual void run() = 0;
        
        /// the list of CL-points to be processed
        std::list<CLPoint> clpoints;
        /// tolerance
        double tol;
};

} // end namespace
#endif
// end file clfilter.h
