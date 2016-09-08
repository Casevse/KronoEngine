#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <glm\glm.hpp>
#include <Windows.h>

bool centerMesh;
glm::vec3 maxPoint;
glm::vec3 minPoint;
int	numVertices;
unsigned int numFaces;
int	numVerticesSilhouette;
unsigned int numFacesSilhouette;

float *v = NULL;
float *n = NULL;
float *tc = NULL;
unsigned int *el = NULL;
float *vs = NULL;
unsigned int *els = NULL;

void TrimString(std::string &str) {
	const char *whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}

void GenerateAveragedNormals(const std::vector<glm::vec3> &points, std::vector<glm::vec3> &normals, const std::vector<int> &faces) {
	for (unsigned int i = 0; i < points.size(); i++) {
		normals.push_back(glm::vec3(0.0f));
	}

	for (unsigned int i = 0; i < faces.size(); i += 3) {
		const glm::vec3 &p1 = points[faces[i]];
		const glm::vec3 &p2 = points[faces[i + 1]];
		const glm::vec3 &p3 = points[faces[i + 2]];

		glm::vec3 a = p2 - p1;
		glm::vec3 b = p3 - p1;
		glm::vec3 n = glm::normalize(glm::cross(a, b));

		normals[faces[i]] += n;
		normals[faces[i + 1]] += n;
		normals[faces[i + 2]] += n;
	}

	for (unsigned int i = 0; i < normals.size(); i++) {
		normals[i] = glm::normalize(normals[i]);
	}
}

void Center(std::vector<glm::vec3> &points) {
	glm::vec3 maxPoint = points[0];
	glm::vec3 minPoint = points[0];

	for (unsigned int i = 0; i < points.size(); ++i) {
		glm::vec3 &point = points[i];

		if (point.x > maxPoint.x) {
			maxPoint.x = point.x;
		} else if (point.x < minPoint.x) {
			minPoint.x = point.x;
		}

		if (point.y > maxPoint.y) {
			maxPoint.y = point.y;
		} else if (point.y < minPoint.y) {
			minPoint.y = point.y;
		}
		if (point.z > maxPoint.z) {
			maxPoint.z = point.z;
		} else if (point.z < minPoint.z) {
			minPoint.z = point.z;
		}
	}

	glm::vec3 center = glm::vec3((maxPoint.x + minPoint.x) / 2.0f,
		(maxPoint.y + minPoint.y) / 2.0f,
		(maxPoint.z + minPoint.z) / 2.0f);

	for (unsigned int i = 0; i < points.size(); ++i) {
		glm::vec3 &point = points[i];
		point = point - center;
	}
}

void CalculateBoundingBox(const std::vector<glm::vec3> &points) {
	maxPoint = points[0];
	minPoint = points[0];
	glm::vec3 point;

	for (unsigned int i = 0; i < points.size(); ++i) {
		point = points[i];
		if (point.x > maxPoint.x) {
			maxPoint.x = point.x;
		} else if (point.x < minPoint.x) {
			minPoint.x = point.x;
		}

		if (point.y > maxPoint.y) {
			maxPoint.y = point.y;
		} else if (point.y < minPoint.y) {
			minPoint.y = point.y;
		}

		if (point.z > maxPoint.z) {
			maxPoint.z = point.z;
		} else if (point.z < minPoint.z) {
			minPoint.z = point.z;
		}
	}
}

void DetermineAdjacency(std::vector<int> &faces) {
	std::vector<int> elementsAdj;

	for (unsigned int i = 0; i < faces.size(); i += 3) {
		elementsAdj.push_back(faces[i]);
		elementsAdj.push_back(-1);
		elementsAdj.push_back(faces[i + 1]);
		elementsAdj.push_back(-1);
		elementsAdj.push_back(faces[i + 2]);
		elementsAdj.push_back(-1);
	}

	for (unsigned int i = 0; i < elementsAdj.size(); i += 6) {
		int a1 = elementsAdj[i];
		int b1 = elementsAdj[i + 2];
		int c1 = elementsAdj[i + 4];

		for (unsigned int j = i + 6; j < elementsAdj.size(); j += 6) {
			int a2 = elementsAdj[j];
			int b2 = elementsAdj[j + 2];
			int c2 = elementsAdj[j + 4];

			if ((a1 == a2 && b1 == b2) || (a1 == b2 && b1 == a2)) {
				elementsAdj[i + 1] = c2;
				elementsAdj[j + 1] = c1;
			}

			if ((a1 == b2 && b1 == c2) || (a1 == c2 && b1 == b2)) {
				elementsAdj[i + 1] = a2;
				elementsAdj[j + 3] = c1;
			}

			if ((a1 == c2 && b1 == a2) || (a1 == a2 && b1 == c2)) {
				elementsAdj[i + 1] = b2;
				elementsAdj[j + 5] = c1;
			}

			if ((b1 == a2 && c1 == b2) || (b1 == b2 && c1 == a2)) {
				elementsAdj[i + 3] = c2;
				elementsAdj[j + 1] = a1;
			}

			if ((b1 == b2 && c1 == c2) || (b1 == c2 && c1 == b2)) {
				elementsAdj[i + 3] = a2;
				elementsAdj[j + 3] = a1;
			}

			if ((b1 == c2 && c1 == a2) || (b1 == a2 && c1 == c2)) {
				elementsAdj[i + 3] = b2;
				elementsAdj[j + 5] = a1;
			}

			if ((c1 == a2 && a1 == b2) || (c1 == b2 && a1 == a2)) {
				elementsAdj[i + 5] = c2;
				elementsAdj[j + 1] = b1;
			}

			if ((c1 == b2 && a1 == c2) || (c1 == c2 && a1 == b2)) {
				elementsAdj[i + 5] = a2;
				elementsAdj[j + 3] = b1;
			}

			if ((c1 == c2 && a1 == a2) || (c1 == a2 && a1 == c2)) {
				elementsAdj[i + 5] = b2;
				elementsAdj[j + 5] = b1;
			}
		}
	}

	for (unsigned int i = 0; i < elementsAdj.size(); i += 6) {
		if (elementsAdj[i + 1] == -1) {
			elementsAdj[i + 1] = elementsAdj[i + 4];
		}
		if (elementsAdj[i + 3] == -1) {
			elementsAdj[i + 3] = elementsAdj[i];
		}
		if (elementsAdj[i + 5] == -1) {
			elementsAdj[i + 5] = elementsAdj[i + 2];
		}
	}

	faces = elementsAdj;
}

void StoreVBO(const std::vector<glm::vec3> &points, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &textureCoords, const std::vector<int> &faces) {
	numVertices = points.size();
	numFaces = faces.size();

	v = new float[3 * numVertices];
	n = new float[3 * numVertices];
	tc = new float[2 * numVertices];
	el = new unsigned int[numFaces];

	int idx = 0;
	int tcIdx = 0;
	for (int i = 0; i < numVertices; ++i) {
		v[idx] = -points[i].x;
		v[idx + 1] = points[i].y;
		v[idx + 2] = -points[i].z;
		n[idx] = -normals[i].x;
		n[idx + 1] = normals[i].y;
		n[idx + 2] = -normals[i].z;
		idx += 3;
		tc[tcIdx] = textureCoords[i].x;
		tc[tcIdx + 1] = textureCoords[i].y;
		tcIdx += 2;
	}

	for (unsigned int i = 0; i < numFaces; ++i) {
		el[i] = faces[i];
	}
}

void StoreVBOSilhouette(const std::vector<glm::vec3> &points, const std::vector<int> &faces) {
	numVerticesSilhouette = points.size();
	numFacesSilhouette = faces.size();

	vs = new float[3 * numVerticesSilhouette];
	els = new unsigned int[numFacesSilhouette];

	int idx = 0;
	for (int i = 0; i < numVerticesSilhouette; ++i) {
		vs[idx] = -points[i].x;
		vs[idx + 1] = points[i].y;
		vs[idx + 2] = -points[i].z;
		idx += 3;
	}

	for (unsigned int i = 0; i < numFacesSilhouette; ++i) {
		els[i] = faces[i];
	}
}

void LoadMeshAdj(std::string path) {
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<int> faces;
	std::vector<int> faceTC;

	int numFaces = 0;

	std::ifstream objStream(path.c_str(), std::ios::in);

	if (!objStream) {
		std::cout << "ERROR: Can't read -> " << path << std::endl;
		return;
	}

	std::string line, token;

	std::getline(objStream, line);

	while (!objStream.eof()) {
		TrimString(line);

		if (line.length() > 0 && line.at(0) != '#') {
			std::istringstream lineStream(line);
			lineStream >> token;

			if (token == "v") {
				float x, y, z;
				lineStream >> x >> y >> z;
				points.push_back(glm::vec3(x, y, z));
			} else if (token == "vt") {
				float u, v;
				lineStream >> u >> v;
				textures.push_back(glm::vec2(u, v));
			} else if (token == "vn") {
				// Ignore the normals.
				/*float x, y, z;
				lineStream >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));*/
			} else if (token == "f") {
				numFaces++;

				size_t slash1, slash2;
				int faceVertices = 0;

				while (lineStream.good()) {
					faceVertices++;
					std::string verticeString;
					lineStream >> verticeString;
					int pointIndex = -1;
					int normalIndex = -1;
					int textureIndex = -1;

					slash1 = verticeString.find("/");
					if (slash1 == std::string::npos) {
						pointIndex = atoi(verticeString.c_str()) - 1;
					} else {
						slash2 = verticeString.find("/", slash1 + 1);
						pointIndex = atoi(verticeString.substr(0, slash1).c_str()) - 1;

						if (slash2 == std::string::npos || slash2 > slash1 + 1) {
							textureIndex = atoi(verticeString.substr(slash1 + 1, slash2).c_str()) - 1;
						}

						if (slash2 != std::string::npos) {
							normalIndex = atoi(verticeString.substr(slash2 + 1, std::string::npos).c_str()) - 1;
						}
					}

					if (pointIndex != -1) {
						faces.push_back(pointIndex);
					}

					if (textureIndex != -1) {
						faceTC.push_back(textureIndex);
					}
				}
			}
		}

		std::getline(objStream, line);
	}

	objStream.close();

	std::vector<glm::vec3> pointsAux = points;
	std::vector<int> facesAux = faces;

	std::vector<glm::vec2> textureCoords;
	for (unsigned int i = 0; i < points.size(); i++) {
		textureCoords.push_back(glm::vec2(0.0f));
	}

	std::map<int, int> pointToTexture;
	for (unsigned int i = 0; i < faces.size(); i++) {
		int point = faces[i];
		int textureCoord = faceTC[i];
		std::map<int, int>::iterator iterator = pointToTexture.find(point);

		if (iterator == pointToTexture.end()) {
			pointToTexture[point] = textureCoord;
			textureCoords[point] = textures[textureCoord];
		} else {
			if (textureCoord != iterator->second) {
				points.push_back(points[point]);
				textureCoords.push_back(textures[textureCoord]);
				faces[i] = points.size() - 1;
			}
		}
	}

	GenerateAveragedNormals(points, normals, faces);

	if (centerMesh) {
		Center(points);
	}

	CalculateBoundingBox(points);

	DetermineAdjacency(faces);

	StoreVBO(points, normals, textureCoords, faces);

	if (centerMesh) {
		Center(pointsAux);
	}

	DetermineAdjacency(facesAux);

	StoreVBOSilhouette(pointsAux, facesAux);
}

void WriteKMBin(std::string path) {
	std::ofstream stream(path.c_str(), std::ios::binary);

	stream.seekp(0);

	stream.write((char *)&numVertices, sizeof(int));
	stream.write((char *)&numVerticesSilhouette, sizeof(int));
	stream.write((char *)&numFaces, sizeof(unsigned int));

	stream.write((char *)&maxPoint.x, sizeof(float));
	stream.write((char *)&maxPoint.y, sizeof(float));
	stream.write((char *)&maxPoint.z, sizeof(float));
	stream.write((char *)&minPoint.x, sizeof(float));
	stream.write((char *)&minPoint.y, sizeof(float));
	stream.write((char *)&minPoint.z, sizeof(float));

	stream.write((char *)v, numVertices * 3 * sizeof(float));
	stream.write((char *)n, numVertices * 3 * sizeof(float));
	stream.write((char *)tc, numVertices * 2 * sizeof(float));
	stream.write((char *)el, numFaces * sizeof(unsigned int));
	stream.write((char *)vs, numVerticesSilhouette * 3 * sizeof(float));
	stream.write((char *)els, numFacesSilhouette * sizeof(unsigned int));

	stream.close();

	delete[] v;
	delete[] n;
	delete[] tc;
	delete[] el;
	delete[] vs;
	delete[] els;
}

int main(void) {
	std::cout << "KronoMesh tool - OBJ to KM converter\n";
	std::cout << "------------------------------------\n";
	std::cout << "1. Centered\n";
	std::cout << "2. Not centered\n\n";

	int key;
	std::cout << "Introduce an option: ";
	std::cin >> key;

	if (key == 1) {
		centerMesh = true;
	} else {
		centerMesh = false;
	}

	HANDLE handle;
	WIN32_FIND_DATA data;

	handle = FindFirstFile("*.*", &data);
	if (handle != INVALID_HANDLE_VALUE) {
		do {
			std::string file(data.cFileName);
			size_t pointPos = file.find_last_of(".");
			if (pointPos != std::string::npos) {
				std::string extension = file.substr(pointPos);
				if (extension == ".obj") {
					if (CreateDirectory("output", NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
						std::cout << "Processing... " << file << std::endl;
						
						LoadMeshAdj(file);
						
						std::string name = file.substr(0, pointPos);
						name += ".km";
						name = std::string("output/") + name;
						
						WriteKMBin(name);
					}


				}
			}
		} while (FindNextFile(handle, &data));
		FindClose(handle);
	}

	std::cout << "\nPress any key...";
	std::cin.get();

	return 1;
}