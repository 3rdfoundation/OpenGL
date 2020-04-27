#pragma once

class Vertex {

public:

	const static int VERTEX_SIZE = 10 * sizeof(float);

	static void* buildData(std::vector<Vertex*> vertices) {
		std::vector<float> data;
		for (int index = 0; index < vertices.size(); index++) {
			Vertex* vertex = vertices[index];
			data.push_back(vertex->m_Position[0]);
			data.push_back(vertex->m_Position[1]);
			data.push_back(vertex->m_Position[2]);
			data.push_back(vertex->m_Color[0]);
			data.push_back(vertex->m_Color[1]);
			data.push_back(vertex->m_Color[2]);
			data.push_back(vertex->m_Color[3]);
			data.push_back(vertex->m_TexCoords[0]);
			data.push_back(vertex->m_TexCoords[1]);
			data.push_back(vertex->m_TexID);
		}
		return data.data();
	}

	Vertex(std::initializer_list<float> position, 
		std::initializer_list<float> color, 
		std::initializer_list<float> texCoords, 
		float texID) {

		int index = 0;
		for (auto element : position) {
			m_Position[index] = element;
			index++;
			if (index > 2) {
				break;
			}
		}

		index = 0;
		for (auto element : color) {
			m_Color[index] = element;
			index++;
			if (index > 3) {
				break;
			}
		}

		index = 0;
		for (auto element : texCoords) {
			m_TexCoords[index] = element;
			index++;
			if (index > 1) {
				break;
			}
		}

		m_TexID = texID;
	};

	~Vertex() {};

	float m_Position[3];
	float m_Color[4];
	float m_TexCoords[2];
	float m_TexID;

};