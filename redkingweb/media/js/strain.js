// Red King Sonification Copyright (C) 2016-2017 FoAM Kernow
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////

// find local maxima by comparing first derivatives,
// when they switch from up to down, record the position
function local_max(arr) {
    var ret = [];
    var last_deriv = 0;
    for (var i=1; i<arr.length; i++) {
        deriv = arr[i]-arr[i-1];
        if (deriv<0 && last_deriv>0) {
            ret.push(i)
	}
        last_deriv = deriv
    }
    return ret;
}

// return the position and strength of each strain
function find_strain_centres(level) {
    var strains = local_max(level);
    var strain_centres = [];
    for (var s=0; s<strains.length; s++) {
        strain_centres.push([strains[s],level[strains[s]]])
    }
    return strain_centres;
}

