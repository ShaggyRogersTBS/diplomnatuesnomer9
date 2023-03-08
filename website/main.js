function showData(divId) {
	var state = {
    "graph": false,
    "graphprec": false,
    "graphhum": false,
    "graphwinds": false
  	};
    // hide all divs first
    Object.keys(state).forEach(function(key) {
      document.getElementById(key).style.display = "none";
    });
    // then show the selected div
    document.getElementById(divId).style.display = "block";
    state[divId] = true;
}

function showData2(divId) {
	var state = {
    "graph2": false,
    "graphprec2": false,
    "graphhum2": false,
    "graphwinds2": false
  	};
    // hide all divs first
    Object.keys(state).forEach(function(key) {
      document.getElementById(key).style.display = "none";
    });
    // then show the selected div
    document.getElementById(divId).style.display = "block";
    state[divId] = true;
}
