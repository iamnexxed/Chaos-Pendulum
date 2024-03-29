#include "Model.h"
//#include "Primitives/Utils.h"

Model::Model(const char* file, glm::vec3 defaultColor)
{
	std::string text = get_file_contents(file);
	// std::cout << text << std::endl;

	this->JSON = json::parse(text);
	// std::cout << "Parsing JSON \n\n";
	this->file = file;
	this->defaultColor = defaultColor;
	this->data = GetData();
	TraverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{		
		meshes[i].DrawInstance(shader, camera);
	}
}

void Model::LoadMesh(unsigned int indMesh)
{
	unsigned int possAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	std::vector<float> posVec = GetFloats(JSON["accessors"][possAccInd]);
	//showFloatVector(posVec, 3);
	std::vector<glm::vec3> positions = GroupFloatsVec3(posVec);

	std::vector<float> normalVec = GetFloats(JSON["accessors"][normalAccInd]);
	//showFloatVector(normalVec, 3);
	std::vector<glm::vec3> normals = GroupFloatsVec3(normalVec);

	std::vector<float> texVec = GetFloats(JSON["accessors"][texAccInd]);
	//showFloatVector(texVec, 2);
	std::vector<glm::vec2> texUVs = GroupFloatsVec2(texVec);

	std::vector<Vertex> vertices = AssembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = GetIndices(JSON["accessors"][indAccInd]);
	//std::cout << "Indices index: " << indAccInd << std::endl;
	//showIntVector(indices, 3);
	std::vector<Texture> textures = GetTextures();

	meshes.push_back(Mesh(vertices, indices, textures));
	
	std::cout << "Loaded Mesh.\n\n";
}

// TODO: Solve error when traversing multiple nodes
void Model::TraverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	json node = JSON["nodes"][nextNode];

	
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];

		for (unsigned int i = 0; i < node["translation"].size(); ++i)
		{
			transValues[i] = node["translation"][i];

		}
		translation = glm::make_vec3(transValues);
	}

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3], // w GLM to GLFW
			node["rotation"][0], // x
			node["rotation"][1], // y
			node["rotation"][2]  // z
		};
		rotation = glm::make_quat(rotValues);
	}

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];

		for (unsigned int i = 0; i < node["scale"].size(); ++i)
		{
			scaleValues[i] = node["scale"][i];

		}
		scale = glm::make_vec3(scaleValues);
	}

	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];

		for (unsigned int i = 0; i < node["matrix"].size(); ++i)
			matValues[i] = node["matrix"][i];

		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.find("mesh") != node.end())
	{
	
		LoadMesh(node["mesh"]);
		//std::cout << "Meshes size: " << meshes.size() << " Next node: " << nextNode << std::endl;
		meshes[nextNode].translation = translation;
		meshes[nextNode].rotation = rotation;
		meshes[nextNode].scale = scale;
		// TODO: Test the matrix operation with multiple node matrix
		meshes[nextNode].matrix = matNextNode;
	}

	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); ++i)
		{
			TraverseNode(node["children"][i], matNextNode);
		}
	}
	
}

std::vector<unsigned char> Model::GetData()
{
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileStr = std::string(file);

	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	//bytesText = uri.c_str();
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<float> Model::GetFloats(json accessor)
{
	std::vector<float> floatVec;

	// accessor.value("bufferview", 1); gets bufferview if not found assign a default value of 1
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVertex;
	if (type == "SCALAR") numPerVertex = 1;
	else if (type == "VEC2") numPerVertex = 2;
	else if (type == "VEC3") numPerVertex = 3;
	else if (type == "VEC4") numPerVertex = 4;
	else throw std::invalid_argument("Type is invalid (not a SCALAR, VEC2, VEC3, VEC4)");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVertex;

	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData;)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };

		float value;

		std::memcpy(&value, bytes, sizeof(float));

		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::GetIndices(json accessor)
{
	std::vector<GLuint> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int beginningOfData = byteOffset + accByteOffset;

	if (componentType == 5125) // unsigned int
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4;)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };

			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));

			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123) // unsigned short
	{
		//std::cout << "Indices Chars: " << std::endl;
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2;)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			
			std::memcpy(&value, bytes, sizeof(unsigned short));
			//std::cout << " " << value;
			indices.push_back((GLuint)value);


		}
		std::cout << std::endl;
	}
	else if (componentType == 5122) // short
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2;)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);


		}
	}
	//showIntVector(indices, 3);
	return indices;
}

std::vector<Texture> Model::GetTextures()
{
	std::vector<Texture> textures;
	
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	
	for (unsigned int i = 0; i < JSON["images"].size(); ++i)
	{
		std::string texPath = JSON["images"][i]["uri"];

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}

	}
	return textures;
}


std::vector<Vertex> Model::AssembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				defaultColor,
				texUVs[i]
			}
		);
	}
	return vertices;
}


std::vector<glm::vec2> Model::GroupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size();i += 2)
	{
		vectors.push_back(glm::vec2(floatVec[i], floatVec[i + 1]));
	}
	return vectors;
}

std::vector<glm::vec3> Model::GroupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i += 3)
	{
		vectors.push_back(glm::vec3(floatVec[i], floatVec[i + 1], floatVec[i + 2]));
	}
	return vectors;
}

std::vector<glm::vec4> Model::GroupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i += 4)
	{
		vectors.push_back(glm::vec4(floatVec[i], floatVec[i + 1], floatVec[i + 2], floatVec[i + 3]));
	}
	return vectors;
}

