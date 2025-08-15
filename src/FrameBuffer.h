#include "ofMain.h"

class FrameBuffer {
public:
	FrameBuffer(long unsigned int maxFrames)
		: maxFrames(maxFrames) { }

	void addFrame(const ofPixels & pixels) {
		if (frames.size() >= maxFrames) {
			frames.erase(frames.begin()); // Remove the oldest frame
		}
		frames.push_back(pixels); // Add the new frame
	}

	void removeFirstFrame() {
		if (!frames.empty()) {
			frames.erase(frames.begin()); // Remove the oldest frame
		}
	}

	void clear() {
		frames.clear();
	}

	const ofPixels & getFrame(int index) const {
		return frames[index];
	}

	const ofTexture & getTexture(int index) const {
		static ofTexture texture;
		texture.allocate(frames[index].getWidth(), frames[index].getHeight(), GL_RGBA);
		texture.loadData(frames[index]);
		return texture;
	}

	const ofPixels & getPixels(int index) const {
		return frames[index];
	}

	const std::vector<ofPixels> & getAllFrames() const { return frames; }

	void setFrames(const std::vector<ofPixels> & newFrames) {
		frames.clear();
		for (const auto & px : newFrames) {
			ofPixels copy;
			copy = px; // This should do a deep copy
			frames.push_back(copy);
		}
		while (frames.size() > maxFrames) {
			frames.erase(frames.begin());
		}
	}

	void resize(int newSize) {
		maxFrames = newSize;
		while (frames.size() > maxFrames) {
			frames.erase(frames.begin()); // Remove oldest frames if exceeding new size
		}
	}

	int size() const {
		return frames.size();
	}

private:
	int maxFrames;
	vector<ofPixels> frames;
};
