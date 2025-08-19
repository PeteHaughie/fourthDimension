#pragma once

#include "FrameBuffer.h"
#include "VideoInputManager.h"
#include "ofMain.h"
#include "ofxHapPlayer.h"

class ofApp : public ofBaseApp {

public:
	ofApp();
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;

private:
	int bufferLength = 255; // 360 would be the same as height of the window but there are only 255 steps available in RGB space
	bool debug = false;
	// ofVideoPlayer video;
	ofxHapPlayer video;
	VideoInputManager videoInputManager;
	ofShader shader;
	FrameBuffer frameBuffer;
	ofFbo pattern, videoFbo;
	float noiseScale = 1.0;
	ofImage patterns[6]; // 6 different patterns
	int temporalPatternIndex = 0;
};
