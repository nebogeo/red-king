function OneDArray(x) {
    var ret = []
    for (var i=0; i<x; i++) {
	ret.push(0)
    }
    return ret
}

function TwoDArray(x,y) {
    var ret = []
    for (var i=0; i<x; i++) {
	ret.push(OneDArray(y))
    }
    return ret
}


 var requestAnimFrame = (function() {
    return window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame ||
    function(/* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {
    window.setTimeout(callback, 1000/60);    };
    })();

