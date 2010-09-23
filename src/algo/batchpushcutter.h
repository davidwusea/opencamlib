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

#ifndef BPC_H
#define BPC_H

#include <iostream>
#include <string>
#include <vector>

#include "point.h"
#include "fiber.h"
#include "kdtree3.h"

namespace ocl
{

class STLSurf;
//class KDNode2;
//class KDTree;
class Triangle;
class MillingCutter;

///
/// BatchPushCutter takes a MillingCutter, an STLSurf, and many Fibers
/// and pushes the cutter along the fibers into contact with the surface.
/// When this runs the Fibers will be updated with the correct interval data.
/// This is then used to build a weave and extract a waterline.
class BatchPushCutter {
    public:
        BatchPushCutter();
        virtual ~BatchPushCutter();
        
        /// set the STL-surface and build kd-tree
        void setSTL(const STLSurf& s);
        /// set the MillingCutter to use
        void setCutter(const MillingCutter *cutter);
        /// set number of OpenMP threads. Defaults to OpenMP::omp_get_num_procs()
        void setThreads(unsigned int n) {nthreads = n;};
        int  getThreads() const {return nthreads;};
        void setXDirection() {x_direction=true;y_direction=false;};
        void setYDirection() {x_direction=false;y_direction=true;};
        /// append to list of Fibers to evaluate
        void appendFiber(Fiber& f);
        
        int getBucketSize() const {return bucketSize;};
        void setBucketSize(unsigned int s) {bucketSize = s;};
        int getCalls() const {return nCalls;};
        
        /// run push-cutter
        void run() {this->pushCutter3();};

        /// pointer to list of Fibers
        std::vector<Fiber>* fibers;
        
    protected:
        void pushCutter1();
        void pushCutter2();
        void pushCutter3();
    
    // DATA
        /// how many low-level calls were made
        int nCalls;
        /// size of bucket-node in KD-tree
        unsigned int bucketSize;
        /// the MillingCutter used
        const MillingCutter* cutter;
        /// the STLSurf which we test against.
        const STLSurf* surf;
        /// root of the kd-tree
        KDTree<Triangle>* root;
        /// number of threads to use
        unsigned int nthreads;
        bool x_direction;
        bool y_direction;
};

} // end namespace

#endif
