var context;
var bufferLoader;

function sound_handler() {
    try { this.context = new AudioContext(); }
    catch(e) { alert("Web Audio API is not supported in this browser"); }	

    this.running = false;

    this.buffer_load_completed = function(buffer,h) { 
	console.log("buffer load complete");
	h.running=true;
	h.reset_timing();
    }    

    this.loop_length = 0.5;
    this.safe_time = 0.2;
    this.logical_time = 0
    this.next_update_time = 0
    this.sequence = 0;

    this.buffer_loader = new BufferLoader(
        this.context,
        [
	    "media/sound/BT3AADA.WAV",
	    "media/sound/HANDCLP2.WAV",
	    "media/sound/ST0TAS3.WAV"
        ],
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

    this.update = function(host,parasite) {
	if (!this.running) return;
	var t = this.context.currentTime;
	if (t<this.next_update_time) return;

	if (this.sequence==0) {
	    host_strains = find_strain_centres(host);
	    var host_tempo = this.loop_length/host_strains.length;
	    
	    for (var i=0; i<host_strains.length; i++) {
		this.play_sound(this.buffer_loader.bufferList[0],
				this.logical_time+host_tempo*i);
	    }
	} else {
	    parasite_strains = find_strain_centres(parasite);
	    var host_tempo = this.loop_length/parasite_strains.length;
	    
	    for (var i=0; i<parasite_strains.length; i++) {
		this.play_sound(this.buffer_loader.bufferList[1],
				this.logical_time+host_tempo*i);
	    }
	}
			
	this.sequence = (this.sequence+1)%2;
	this.next_update_time+=this.loop_length;
	this.logical_time+=this.loop_length;
    }

    this.play_sound = function(buffer, time) {
	var source = this.context.createBufferSource();
	source.buffer = buffer;
	source.connect(this.context.destination);
	source.start(time);
    }
}
