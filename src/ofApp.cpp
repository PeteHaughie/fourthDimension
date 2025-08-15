#include "ofApp.h"

ofApp::ofApp()
	: frameBuffer(bufferLength) { }

//--------------------------------------------------------------
void ofApp::setup() {
	ofDisableArbTex();
	ofSetWindowTitle("Fourth Dimension");

	patterns[0].load("gradient-h.png");
	patterns[1].load("gradient-v.png");
	patterns[2].load("gradient-ramp.png");
	patterns[3].load("gradient-ramp-inverse.png");
	patterns[4].load("gradient-conical.png");
	patterns[5].load("gradient-dots.png");

	pattern.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	video.load("venus.mov");
	video.setLoopState(OF_LOOP_NORMAL);
	video.play();

	// fill frameBuffer
	for (int i = 0; i < bufferLength; ++i) {
		ofPixels pixels;
		pixels.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
		// Fill pixels with some data
		for (int y = 0; y < pixels.getHeight(); ++y) {
			for (int x = 0; x < pixels.getWidth(); ++x) {
				ofColor color = ofColor::black;
				pixels.setColor(x, y, color);
			}
		}
		frameBuffer.addFrame(pixels);
	}

	// setup the video input manager
	videoInputManager.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {
	videoInputManager.update();
	if (videoInputManager.getInput()->isFrameNew()) {
		ofPixels newFrame = videoInputManager.getInput()->getPixels();
		frameBuffer.addFrame(newFrame);
	}
	// video.update();
	// if (video.isFrameNew()) {
	// 	// Get the new frame from the video
	// 	ofPixels newFrame = video.getPixels();
	// 	// Add the new frame to the frame buffer
	// 	frameBuffer.addFrame(newFrame);
	// }
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Draw evolving 2D Perlin noise field
	float time = ofGetElapsedTimef();
	int w = ofGetWidth();
	int h = ofGetHeight();
	int scale = 4; // Adjust for resolution/performance

	ofPixels noisePixels;
	noisePixels.allocate(w, h, OF_PIXELS_GRAY);

	for (int y = 0; y < h; y += scale) {
		for (int x = 0; x < w; x += scale) {
			float nx = float(x) / w;
			float ny = float(y) / h;
			float noiseValue = ofNoise(nx * noiseScale, ny * noiseScale, time * 0.2); // Adjust frequency/time speed
			unsigned char brightness = static_cast<unsigned char>(noiseValue * 255);
			for (int dy = 0; dy < scale; ++dy) {
				for (int dx = 0; dx < scale; ++dx) {
					if (x + dx < w && y + dy < h) {
						noisePixels.setColor(x + dx, y + dy, ofColor(brightness));
					}
				}
			}
		}
	}

	pattern.begin();
	if (temporalPatternIndex < 6) {
		patterns[temporalPatternIndex].draw(0, 0, ofGetWidth(), ofGetHeight());
	} else if (temporalPatternIndex == 6) {
		ofTexture noiseTex;
		noiseTex.allocate(noisePixels);
		noiseTex.loadData(noisePixels);
		noiseTex.draw(0, 0, ofGetWidth(), ofGetHeight());
	} else {
		videoInputManager.getInput()->draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	pattern.end();

	ofPixels patternPixels;
	pattern.readToPixels(patternPixels);

	ofPixels outputPixels;
	outputPixels.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);

	// For each pixel in the pattern image
	for (int y = 0; y < patternPixels.getHeight(); ++y) {
		for (int x = 0; x < patternPixels.getWidth(); ++x) {
			// Get brightness (assuming grayscale or use .getColor(x, y).getBrightness())
			int value = patternPixels.getColor(x, y).getBrightness(); // 0-255

			// Clamp value to available frames
			int frameIndex = ofMap(value, 0, 255, 0, bufferLength - 1, true);

			// Get corresponding pixel from frameBuffer
			const ofPixels & framePixels = frameBuffer.getPixels(frameIndex);
			ofColor frameColor = framePixels.getColor(x, y);

			// Set output pixel
			outputPixels.setColor(x, y, frameColor);
		}
	}

	// Upload outputPixels to a texture for drawing
	ofTexture outputTexture;
	outputTexture.allocate(outputPixels);
	outputTexture.loadData(outputPixels);

	// Draw the result
	outputTexture.draw(0, 0, ofGetWidth(), ofGetHeight());

	if (debug) {
		// video.draw(0, 0, ofGetWidth() / 3, ofGetHeight() / 3);
		videoInputManager.getInput()->draw(0, 0, ofGetWidth() / 3, ofGetHeight() / 3);
		pattern.draw(ofGetWidth() / 3 * 2, 0, ofGetWidth() / 3, ofGetHeight() / 3);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		videoInputManager.incrementDeviceID();
	}
	if (key == 'a') {
		temporalPatternIndex = (temporalPatternIndex + 1) % 8;
	}
	if (key == 'd') {
		debug = !debug;
	}
	if (key == '1') {
		noiseScale = 0.5; // Smoother noise
	}
	if (key == '2') {
		noiseScale = 1.0; // Default noise
	}
	if (key == '3') {
		noiseScale = 2.0; // Rougher noise
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
