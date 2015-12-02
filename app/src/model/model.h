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

namespace red_king {

class model {
 public:

    model();

    void init();
    void step();

    double *get_hosts() { return x0; }
    double *get_parasites() { return y; }

 private:

    void init_trait_values();
    void init_cost_functions();
    void init_matrix();

    // parameters for the input
    double *u, *v, **E, *a, *beta;

    // parameters for the model
    double *x0, *y, **y0;
    double  rtype, r1, r2, xtotal, xcum, temp;
    int *host_ind, *par_ind;
    int i, j, evol_count, nh, np, mutator, pop_choice;

    // result stored here
    double **xout;

};

}
