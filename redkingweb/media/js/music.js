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

var context;
var bufferLoader;

function sound_handler() {
    try { this.context = new AudioContext(); }
    catch(e) { alert("Web Audio API is not supported in this browser"); }	

    this.running = false;

    this.buffer_load_completed = function(buffer,h) { 
	h.reset_timing();
    }    

    this.loop_length = 0.75;
    this.safe_time = 0.2;
    this.logical_time = 0
    this.next_update_time = 0
    this.sequence = 0;

    this.sample_list = []
    for (var i=1; i<7; i++) {
	this.sample_list.push("/media/sound/dx7-o-"+i+".wav");
    }
    for (var i=1; i<7; i++) {
	this.sample_list.push("/media/sound/dx7-"+i+".wav");
    }
    
    this.buffer_loader = new BufferLoader(
        this.context,
        this.sample_list,
	this.buffer_load_completed,this
    );

    
    this.init = function() {
	this.buffer_loader.load();
    }
    
    this.set_running = function(s) {
	this.running=s;
	if (s) this.reset_timing();
    }

    this.reset_timing = function() {
	this.logical_time = this.context.currentTime+this.loop_length;
	this.next_update_time = this.logical_time-this.safe_time;
    }
    
    this.make_sound = function(type,i,strains,length,tempo) {
	
	var x = Math.floor((strains[i][0]/length)*10);
	var y = strains[i][1];
	if (y>1) y=1;
	if (y<0) y=0;
	y=Math.floor(y*5);
	y+=type*6;
	var gain = 0.5;
	if (type==1) gain=0.1;

	this.play_sound(this.buffer_loader.bufferList[y],
			this.logical_time+tempo*i+tempo/strains.length,
			this.note_to_pitch(x+60),gain);
    }
    
    this.update = function(host,parasite) {
	if (!this.running) return;
	var t = this.context.currentTime;
	if (t<this.next_update_time) return;

	if (this.sequence==0) {
	    host_strains = find_strain_centres(host);
	    var tempo = this.loop_length/host_strains.length;
	    for (var i=0; i<host_strains.length; i++) {		
		this.make_sound(this.sequence,i,host_strains,host.length,tempo);
	    }
	} else {
	    parasite_strains = find_strain_centres(parasite);
	    var tempo = this.loop_length/parasite_strains.length;	    
	    for (var i=0; i<parasite_strains.length; i++) {
		var x = Math.floor((parasite_strains[i][0]/parasite.length)*10);
		var y = parasite_strains[i][1];
		if (y>1) y=1;
		if (y<0) y=0;
		y=Math.floor(y*9);
		this.make_sound(this.sequence,i,parasite_strains,parasite.length,tempo);
	    }
	}
			
	this.sequence = (this.sequence+1)%2;
	this.next_update_time+=this.loop_length/2;
	this.logical_time+=this.loop_length/2;
    }

    this.note_to_pitch = function(note) {
	return (Math.pow(2,(note-69)/12));
    }

    this.play_sound = function(buffer, time, rate, vol) {
	var source = this.context.createBufferSource();
	source.buffer = buffer;
	source.playbackRate.value = rate;
	var gain = this.context.createGain();
	gain.connect(this.context.destination);
	gain.gain.value = vol;
	source.connect(gain);
	source.start(time);
    }
}
