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

#include <boost/foreach.hpp>

#include "ballcutter.h"
#include "numeric.h"

namespace ocl
{

BallCutter::BallCutter() {
    std::cout << " usage: BallCutter( double diameter, double length )\n";
    assert(0);
}

BallCutter::BallCutter(double d, double l) {
    assert( d>0.0 );
    diameter = d;
    radius = d/2.0;
    assert( l>0.0 );
    length = l;
    normal_length = radius;
    xy_normal_length = 0.0;
    center_height = radius;
}

// height of cutter at radius r
double BallCutter::height(double r) const {
    return radius - sqrt( square(radius) - square(r) );
}

// width of cutter at height h
double BallCutter::width(double h) const {
    if ( h >= radius )
        return radius;
    else 
        return sqrt( square(radius) - square(radius-h) );
}

// offset of ball is a bigger ball
MillingCutter* BallCutter::offsetCutter(double d) const {
    return  new BallCutter(diameter+2*d, length+d) ;
}

// drop-cutter methods: vertex and facet are handled in base-class

// drop-cutter edgeDrop 
CC_CLZ_Pair BallCutter::singleEdgeContact(const Point& u1, const Point& u2) const {
    // the plane of the line will slice the spherical cutter at
    // a distance d==u1.y==u2.y from the center of the cutter
    // here the radius of the circular section is s:
    double s = sqrt( square(radius) - square( u1.y ) );            
    Point normal(u2.z - u1.z, -(u2.x - u1.x), 0); // (dz, -du) is a normal to the line 
    normal.xyNormalize();
    if (normal.y < 0)  // flip normal so it points upward
        normal = -1*normal;
    Point cc( -s*normal.x, u1.y, 0); // from (0,u1.y,0) we go a distance -s in the normal direction
    cc.z_projectOntoEdge(u1,u2);
    double cl_z = cc.z + s*normal.y - radius;
    return CC_CLZ_Pair( cc.x , cl_z);
}


// push-cutter: vertex and facet handled in base-class

// push-cutter edge-push
bool BallCutter::edgePush(const Fiber& f, Interval& i,  const Triangle& t) const {
    bool result = false;
    for (int n=0;n<3;n++) { // loop through all three edges
        int start=n;
        int end=(n+1)%3;
        const Point p1 = t.p[start]; // edge is from p1 to p2
        const Point p2 = t.p[end];
        
        const Point ufp1 = f.p1 + Point(0,0,radius); // take a fiber which is raised up by radius
        const Point ufp2 = f.p2 + Point(0,0,radius);
        // and intersect it with a cylinder around the edge p1-p2
        //--------------------------------------------------------------------------
        // Ray : P(t) = O + t*V    from point O, in direction V
        // Cylinder [A, B, r]   from point A to point B, radius r
        // Point P on infinite cylinder if ((P - A) x (B - A))^2 = r^2 * (B - A)^2
        // expand : ((O - A) x (B - A) + t * (V x (B - A)))^2 = r^2 * (B - A)^2
        // equation in the form (X + t * Y)^2 = d
        // where : 
        //  X = (O - A) x (B - A)
        //  Y = V x (B - A)
        //  d = r^2 * (B - A)^2
        // expand the equation :
        // t^2 * (Y . Y) + t * (2 * (X . Y)) + (X . X) - d = 0
        // => second order equation in the form : a*t^2 + b*t + c = 0 where
        // a = (Y . Y)
        // b = 2 * (X . Y)
        // c = (X . X) - d
        //--------------------------------------------------------------------------
        Point ab = p2-p1; // axis of the cylinder
        Point ao = (ufp1 - p1); // cyl start to ray start
        Point ao_x_ab = ao.cross(ab); // cross product
        Point v_x_ab  = (ufp2-ufp1).cross(ab); // cross product
        double ab2 = ab.dot(ab); // dot product
        double a = v_x_ab.dot(v_x_ab); // dot product
        double b = 2 * ( v_x_ab.dot(ao_x_ab) ); // dot product
        double c = ao_x_ab.dot(ao_x_ab) - (radius*radius * ab2);
        // solve second order equation : a*t^2 + b*t + c = 0
        // t = (-b +/- sqrt( b^2 - 4ac ) )   / 2a
        double discr = b*b-4*a*c;
        double t1;
        double t2;
        if ( isZero_tol( discr ) ) { // tangent case
            //std::cout << "TANGENT CASE!\n";
            t1= -b/(2*a); // only one root
            Point cl1 = f.point(t1);
            Point cl1_center = f.point(t1) + Point(0,0,radius);
            CCPoint cc_tmp = cl1_center.closestPoint(p1,p2);
            cc_tmp.type = EDGE;
            double cct = (cc_tmp-p1).dot(p2-p1) / (p2-p1).dot(p2-p1) ;
            if ( cct > 0.0 && cct < 1.0 && ((cl1_center-cc_tmp).z >=0) ) {
                i.update( t1  , cc_tmp );
                result = true;
            }
        } else if ( discr > 0.0 ) { // two roots
            t1 = (-b + sqrt( discr))/(2*a);
            t2 = (-b - sqrt( discr))/(2*a);
            Point cl1 = f.point(t1);// now calculate the cl-points
            Point cl2 = f.point(t2);
            Point cl1_center = f.point(t1) + Point(0,0,radius);
            Point cl2_center = f.point(t2) + Point(0,0,radius);
            // cc-point is on p1-p2 line, closest to CL
            CCPoint cc_tmp1 = cl1_center.closestPoint(p1,p2);
            CCPoint cc_tmp2 = cl2_center.closestPoint(p1,p2);
            // edge: p1 + t*(p2-p1) = cc_tmp
            // so t = (cc_tmp-p1)dot(p2-p1) / (p2-p1).dot(p2-p1)
            double cct1 = (cc_tmp1-p1).dot(p2-p1) / (p2-p1).dot(p2-p1) ;
            double cct2 = (cc_tmp2-p1).dot(p2-p1) / (p2-p1).dot(p2-p1) ;
            cc_tmp1.type = EDGE;
            cc_tmp2.type = EDGE;
            if ( cct1 > 0.0 && cct1 < 1.0 && ((cl1_center-cc_tmp1).z >=0) ) {
                i.update( t1  , cc_tmp1 );
                result = true;
            }
            // edge: p1 + t*(p2-p1) = cc_tmp
            // so t = (cc_tmp-p1)dot(p2-p1) / (p2-p1).dot(p2-p1)
            if ( cct2 > 0.0 && cct2 < 1.0 && ((cl2_center-cc_tmp2).z >=0) ) {
                i.update( t2  , cc_tmp2 );
                result = true;
            }
        } else {
        }
            
            // no solution to quadratic(?), i.e. no contact with the ball
            // instead we test for contact with the cylindrical shaft
            // fiber is f.p1 + v*(f.p2-f.p1)
            // line  is p1 + u*(p2-p1)
            double u,v;
            if ( xy_line_line_intersection(p1, p2, u, f.p1, f.p2, v ) ) { // find XY-intersection btw fiber and edge
                Point q = p1 + u*(p2-p1); // intersection point, on edge
                // Point q = f.p1 + v*(f.p2-f.p1); // q on fiber
                // from q, go v_cc*xy_tangent, then r*xy_normal, and end up on fiber:
                // q + v_cc*tangent + r*xy_normal = p1 + t_cl*(p2-p1)
                Point xy_tang=p2-p1;
                xy_tang.z=0;
                xy_tang.xyNormalize();
                Point xy_normal = xy_tang.xyPerp();
                Point q1 = q  + radius*xy_normal;
                Point q2 = q1 + (p2-p1);
                double u_cc, t_cl;
                if ( xy_line_line_intersection( q1 , q2, u_cc, f.p1, f.p2, t_cl ) ) {
                    double t_cl1 = t_cl;
                    double t_cl2 = v + (v-t_cl );
                    CCPoint cc_tmp1 = q + u_cc*(p2-p1);
                    CCPoint cc_tmp2 = q - u_cc*(p2-p1); 
                    cc_tmp1.type = EDGE_CYL;
                    cc_tmp2.type = EDGE_CYL;
                    if( cc_tmp1.isInsidePoints(p1,p2) && (cc_tmp1.z >= (f.p1.z+radius) ) ) {
                        i.update( t_cl1  , cc_tmp1 );
                        result = true;
                    }
                    if( cc_tmp2.isInsidePoints(p1,p2) && (cc_tmp2.z >= (f.p1.z+radius) ) ) {
                        i.update( t_cl2  , cc_tmp2 );
                        result = true;
                    }
                }
            }// shaft contact case
        
    } // loop through all edges
    return result;
}

std::string BallCutter::str() const {
    std::ostringstream o;
    o << *this; 
    return o.str();
}

std::ostream& operator<<(std::ostream &stream, BallCutter c) {
  stream << "BallCutter(d=" << c.diameter << ", r=" << c.radius << ", L=" << c.length << ")";
  return stream;
}

} // end namespace
// end file ballcutter.cpp
