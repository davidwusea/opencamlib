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

#ifndef MILLING_CUTTER_WRAP_H
#define MILLING_CUTTER_WRAP_H

#include <boost/python.hpp>

#include "millingcutter.h"

namespace ocl
{

/* required wrapper class for virtual functions in boost-python */
/// \brief a wrapper required for boost-python
// see documentation:
// http://www.boost.org/doc/libs/1_43_0/libs/python/doc/tutorial/doc/html/python/exposing.html#python.inheritance
class MillingCutterWrap : public MillingCutter, public boost::python::wrapper<MillingCutter>
{
    public:
        // vertex
        bool vertexDrop(CLPoint &cl, const Triangle &t) const
        {
            if ( boost::python::override ovr_vertexDrop = this->get_override("vertexDrop"))
                return ovr_vertexDrop(cl, t);
            return MillingCutter::vertexDrop(cl, t);
        };
        /// python-wrapper boilerplate...
        bool default_vertexDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::vertexDrop(cl,t);
        };
        
        // facet
        bool facetDrop(CLPoint &cl, const Triangle &t) const
        {
            if ( boost::python::override ovr_facetDrop = this->get_override("facetDrop"))
                return ovr_facetDrop(cl, t);
            return MillingCutter::facetDrop(cl, t);
        };
        /// python-wrapper boilerplate...
        bool default_facetDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::facetDrop(cl,t);
        };
        
        
        // edge
        bool edgeDrop(CLPoint &cl, const Triangle &t) const
        {   
            if ( boost::python::override ovr_edgeDrop = this->get_override("edgeDrop"))
                return ovr_edgeDrop(cl, t);
            return MillingCutter::edgeDrop(cl, t);
        };    
        /// python-wrapper boilerplate...
        bool default_edgeDrop(CLPoint &cl, const Triangle &t) const{
            return this->MillingCutter::edgeDrop(cl,t);
        };
        
        // offset cutter
        MillingCutter* offsetCutter(double d) const
        {
            if ( boost::python::override ovr_offsetCutter = this->get_override("offsetCutter") )
                return ovr_offsetCutter(d);
            return MillingCutter::offsetCutter(d);
        };   
        /// python-wrapper boilerplate...
        MillingCutter* default_offsetCutter(double d) const {
            return this->MillingCutter::offsetCutter(d);
        };
        
        // string repr
        std::string str() const
        {
            if ( boost::python::override ovr_str = this->get_override("str")) {
                return ovr_str();
            }
            return MillingCutter::str();
        }; 
        /// python-wrapper boilerplate...
        std::string default_str() const {
            return this->MillingCutter::str();
        };
};

} // end namespace
#endif
// end file millingcutter_wrap.h
