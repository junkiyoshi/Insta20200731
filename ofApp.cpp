#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int x = -300; x <= 300; x += 600) {

		for (int y = -300; y <= 300; y += 600) {

			auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
			this->setRingToMesh(this->face, this->frame, glm::vec3(x, y, 0), 0, 140, 30, 0, 360, noise_seed);
			for (auto radius = 120; radius < 240; radius += 10) {

				for (auto deg = 0; deg < 360; deg += 60) {

					this->setRingToMesh(this->face, this->frame, glm::vec3(x, y, 0), radius, 10, 60, deg, deg + 45, noise_seed);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, int deg_start, int deg_end, glm::vec2 noise_seed) {

	auto index = frame_target.getNumVertices();

	auto noise_value = radius != 0 ? glm::vec2(ofNoise(noise_seed.x - radius * 0.0025 + ofGetFrameNum() * 0.0085), ofNoise(noise_seed.y - radius * 0.0025 + ofGetFrameNum() * 0.0085)) :
		glm::vec2(ofNoise(noise_seed.x - 100 * 0.0025 + ofGetFrameNum() * 0.0085), ofNoise(noise_seed.y - 100 * 0.0025 + ofGetFrameNum() * 0.0085));

	auto angle_x = ofMap(noise_value.x, 0, 1, -PI * 2, PI * 2);
	auto angle_y = ofMap(noise_value.y, 0, 1, -PI * 2, PI * 2);

	auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
	auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));

	int deg_span = 3;
	for (int deg = deg_start; deg < deg_end; deg += deg_span) {

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius + width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius + width * 0.5) * cos(deg * DEG_TO_RAD), (radius + width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos((deg + deg_span) * DEG_TO_RAD), (radius - width * 0.5) * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3((radius - width * 0.5) * cos(deg * DEG_TO_RAD), (radius - width * 0.5) * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			vertex = location + glm::vec4(vertex, 0) * rotation_y * rotation_x;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	}

	if (radius != 0) {

		face_target.addIndex(index + 0); face_target.addIndex(index + 3); face_target.addIndex(index + 7);
		face_target.addIndex(index + 0); face_target.addIndex(index + 7); face_target.addIndex(index + 4);

		frame_target.addIndex(index + 0); frame_target.addIndex(index + 3);
		frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
		frame_target.addIndex(index + 7); frame_target.addIndex(index + 3);
		frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

		index = frame_target.getNumVertices() - 8;

		face_target.addIndex(index + 1); face_target.addIndex(index + 2); face_target.addIndex(index + 6);
		face_target.addIndex(index + 1); face_target.addIndex(index + 6); face_target.addIndex(index + 5);

		frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
		frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
		frame_target.addIndex(index + 6); frame_target.addIndex(index + 2);
		frame_target.addIndex(index + 6); frame_target.addIndex(index + 5);
	}

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}