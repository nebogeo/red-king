#!/usr/bin/env python
# Red King Simulation Sonification
# Copyright (C) 2016 Foam Kernow
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# code to find strains in a simulation time slice
def find_centres(level):
    av = 0
    mx = 0
    # find average and max for this slice
    for i in range(0,len(level)):
        av+=level[i]
        if level[i]>mx: mx=level[i]
    av/=len(level)
    av/=10 # threshold the detection
    if mx==0: mx=1
    # record the positions of any strains above the threshold
    # until they drop below the threshold
    in_strain = False
    strain_start = 0
    strains = []
    for i in range(0,len(level)):
        if not in_strain and level[i]>av:
            in_strain = True
            strain_start = i
        if in_strain and level[i]<av:
            in_strain = False
            strains.append([strain_start,i])
    # find the centres of the strains and record the maximum there
    strain_centres = []
    for strain in strains:
        strain_max = 0
        strain_pos = 0
        for i in range(strain[0],strain[1]):
            if level[i]>strain_max:
                strain_max=level[i]
                strain_pos=i
        strain_centres.append([strain_pos,strain_max])

    return strain_centres


#ta = [0,0,0.5,1,0.5,0,0,0.1,0,0,0.1,0.2,1,0.2,0,0,0]
#print(find_centres(ta))
