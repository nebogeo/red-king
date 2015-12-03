// Copyright (C) 2015 Foam Kernow
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "types.h"

#pragma once

namespace red_king {

class range;

class model {
 public:

    model();

    void init();
    void step();

    rk_real *get_hosts() { return x0; }
    rk_real *get_parasites() { return y; }
    rk_real *get_host_cost() { return a; }
    rk_real *get_parasite_cost() { return beta; }

 private:

    void init_trait_values();
    void init_cost_functions(rk_real amin, rk_real amax,
                                rk_real umin, rk_real umax,
                                rk_real a_p,
                                rk_real betmin, rk_real bemaxtime,
                                rk_real vmin, rk_real vmax,
                                rk_real beta_p);
    void init_matrix();

    void check_phenotypes(int &nh, int& np);
    void mutate();

    // the underlying model
    range *m_range;

    // parameters for the input
    rk_real *u, *v, **E, *a, *beta;

    // parameters for the model
    rk_real *x0, *y, **y0;
    int *host_ind, *par_ind;
};

}
