var context = new AudioContext(),
	sourceBuffers = [],
	soundNumbers = [155, 156, 157, 158, 149, 150, 151, 152, 159, 144, 145, 146],
	sound,
	animationLength = 1;


//--------------------------------------------Midi and Sound-Visualisation --------------------------------------------

function initialize() {
	// load sounds in buffer
	for (let p = 0; p < soundNumbers.length; p++) {
		loadSounds(soundNumbers[p]);
	}

	// MIDI
	let midi = null;  // global MIDIAccess object
	let midiInputs = [];
	function onMIDISuccess(midiAccess) {
		console.log("MIDI ready!");
		midi = midiAccess;
		listInputsAndOutputs(midi);
	}

	function onMIDIFailure(msg) {
		console.log("Failed to get MIDI access - " + msg);
	}

	navigator.requestMIDIAccess().then(onMIDISuccess, onMIDIFailure);
	function listInputsAndOutputs(midiAccess) {
		const inputPortSelector = document.getElementById('inputportselector');
		for (let input of midiAccess.inputs.values()) {
			var opt = document.createElement("option");
			opt.text = input.name;
			document.getElementById("inputportselector").add(opt);
			midiInputs.push(input);
			console.log("Input port [type:'" + input.type + "'] id:'" + input.id +
				"' manufacturer:'" + input.manufacturer + "' name:'" + input.name +
				"' version:'" + input.version + "'");
		}
		if (midiInputs.length > 1) {
			inputPortSelector.addEventListener('click', () => {
				selectMidiInput(inputPortSelector.selectedIndex);
			});
		}
		selectMidiInput(inputPortSelector.selectedIndex);
	}

	function selectMidiInput(index) {
		const selectedIndex = document.getElementById('inputportselector').selectedIndex;
		const midiInput = midiInputs[selectedIndex];
		midiInput.onmidimessage = MIDIMessageEventHandler;
		return midiInput;
	}
	function MIDIMessageEventHandler(event) {
		noteOn(event.data[0], event.data[1], event.data[2]); // noteNumber, x, y
	}

	// Form-Sound-Matching --> noteOn-Function
	function noteOn(noteNumber, x, y) {
		var img = document.createElement("img");
		playSound(noteNumber, y);

		switch (noteNumber) {
			case 155:
				img.src = "img/155.png";
				img.alt = "greenTriangle";
				break;
			case 156:
				img.src = "img/156.png";
				img.alt = "blueTriangle";
				break;
			case 157:
				img.src = "img/157.png";
				img.alt = "redTriangle";
				break;
			case 158:
				img.src = "img/158.png";
				img.alt = "yellowTriangle";
				break;
			case 149:
				img.src = "img/149.png";
				img.alt = "greenQuad";
				break;
			case 150:
				img.src = "img/150.png";
				img.alt = "blueQuad";
				break;
			case 151:
				img.src = "img/151.png";
				img.alt = "redQuad";
				break;
			case 152:
				img.src = "img/152.png";
				img.alt = "yellowQuad";
				break;
			case 159:
				img.src = "img/143.png";
				img.alt = "greenPentagon";
				break;
			case 144:
				img.src = "img/144.png";
				img.alt = "bluePentagon";
				break;
			case 145:
				img.src = "img/145.png";
				img.alt = "redPentagon";
				break;
			case 146:
				img.src = "img/146.png";
				img.alt = "yellowPentagon";
				break;
		}
		img.setAttribute("style", "left:" + (x * 10 + 25) + "%; top:" + (y) + "%; width:" + ((100 -y) * 3) + "px; height: " + ((100 -y) *3) + "px;");
		img.style.WebkitAnimationDuration = animationLength + "s"; // Chrome, Safari, Opera
		img.style.animationDuration = animationLength + "s";
		img.id = "animationIMG";
		document.getElementById("animationSection").appendChild(img);
		setTimeout(function () { document.getElementById("animationSection").removeChild(img); }, animationLength * 1000 - 10);
		console.log("--------------------------------------");
		console.log(`note on: note=${noteNumber}, x = ${x}, y = ${y}`);
	}
}

// function to load sounds 
function loadSounds(i) {
	var request = new XMLHttpRequest();
	request.open('GET', "sounds/" + (i) + ".wav", true);
	request.responseType = 'arraybuffer';

	request.onload = function () {
		context.decodeAudioData(request.response, function (buffer) {
			sourceBuffers[i] = buffer;
		});
	};
	request.send();
}

// play sound 
function playSound(i, soundVolume) {
	setupSound(i, soundVolume);
	sound.start(0);
}






