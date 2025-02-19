#include "ObjLoader.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/CPUMesh.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include <unordered_map>

bool ObjLoader::Load(const std::string& fileName, std::vector<Vertex_PCUTBN>& outVertexes, std::vector<unsigned int>& outIndexes, bool& outHasNormals, bool& outHasUVs, const Mat44& transform /*= Mat44()*/)
{
	std::string fileBuffer;
	if (FileReadToString(fileBuffer, fileName) == 0)
	{
		return false;
	}

	std::vector<Vec3> pList;
	std::vector<Vec2> tList;
	std::vector<Vec3> nList;
	std::vector<Face> faceList;
	std::map<std::string, Rgba8> mtllibList;

	std::string currentMaterial;
	outHasNormals = false;
	outHasUVs = false;

	// Split Lines
	Strings lines;
	lines = SplitStringOnDelimiter(fileBuffer, "\n", false);

	// Reading Data
	for (size_t lineIndex = 0; lineIndex < lines.size(); lineIndex++)
	{
		std::string lineBuffer = lines[lineIndex];

		for (size_t i = 0; i < lineBuffer.size(); i++)
		{
			Strings tokens = SplitStringOnDelimiter(lineBuffer, ' ', true);

			if (tokens[0] == "#")
			{
				break;
			}

			if (tokens[0] == "mtllib")
			{
				std::string materialBuffer;

				if (FileReadToString(materialBuffer, fileName.substr(0, fileName.find_last_of("\\/")) + "\\/" + tokens[1]) == 0)
				{
					ERROR_AND_DIE("Unknown Materail File");
				}

				Strings materialLines;
				materialLines = SplitStringOnDelimiter(materialBuffer, "\n", false);

				std::string currentMaterialLineName;

				for (size_t materialLineIndex = 0; materialLineIndex < materialLines.size(); materialLineIndex++)
				{
					std::string materialLineBuffer = materialLines[materialLineIndex];

					for (size_t mtlLineIndex = 0; mtlLineIndex < materialLineBuffer.size(); mtlLineIndex++)
					{
						Strings materialTokens = SplitStringOnDelimiter(materialLineBuffer, ' ', true);

						if (materialTokens[0] == "#")
						{
							break;
						}

						if (materialTokens[0] == "newmtl")
						{
							currentMaterialLineName = materialTokens[1];
							break;
						}

						if (materialTokens[0] == "Kd")
						{
							Vec3 colorFloat;

							const char* xChar = materialTokens[1].c_str();
							const char* yChar = materialTokens[2].c_str();
							const char* zChar = materialTokens[3].c_str();

							colorFloat.x = static_cast<float>(atof(xChar));
							colorFloat.y = static_cast<float>(atof(yChar));
							colorFloat.z = static_cast<float>(atof(zChar));

							mtllibList[currentMaterialLineName] = Rgba8::Create_FromVec3(colorFloat);
							break;
						}
					}
				}
			}

			if (tokens[0] == "usemtl")
			{
				currentMaterial = tokens[1];
			}

			if (tokens[0] == "v")
			{
				Vec3 position;

				const char* xChar = tokens[1].c_str();
				const char* yChar = tokens[2].c_str();
				const char* zChar = tokens[3].c_str();

				position.x = static_cast<float>(atof(xChar));
				position.y = static_cast<float>(atof(yChar));
				position.z = static_cast<float>(atof(zChar));

				Vec3 transformedPos = transform.TransformPosition3D(position);

				pList.push_back(transformedPos);
				break;
			}

			if (tokens[0] == "vt")
			{
				outHasUVs = true;

				Vec2 uvs;

				const char* xChar = tokens[1].c_str();
				const char* yChar = tokens[2].c_str();

				uvs.x = static_cast<float>(atof(xChar));
				uvs.y = static_cast<float>(atof(yChar));


				tList.push_back(uvs);
				break;
			}

			if (tokens[0] == "vn")
			{
				outHasNormals = true;

				Vec3 normal;

				const char* xChar = tokens[1].c_str();
				const char* yChar = tokens[2].c_str();
				const char* zChar = tokens[3].c_str();

				normal.x = static_cast<float>(atof(xChar));
				normal.y = static_cast<float>(atof(yChar));
				normal.z = static_cast<float>(atof(zChar));

				Vec3 transformedNormal = transform.TransformPosition3D(normal);

				nList.push_back(transformedNormal);
				break;
			}

			if (tokens[0] == "f")
			{
				Face face;

				for (size_t faceIndex = 1; faceIndex < tokens.size(); faceIndex++)
				{
					Strings doubleSlashFormat = SplitStringOnDelimiter(tokens[faceIndex], "//", true);

					if (doubleSlashFormat.size() == 1)
					{
						Strings singleSlashFormat = SplitStringOnDelimiter(tokens[faceIndex], "/", true);

						if (singleSlashFormat.size() == 1)
						{
							face.m_vertPositionIndexes.push_back(atoi(doubleSlashFormat[0].c_str()) - 1);
						}
						else
						{
							face.m_vertPositionIndexes.push_back(atoi(singleSlashFormat[0].c_str()) - 1);
							face.m_vertTextureIndexes.push_back(atoi(singleSlashFormat[1].c_str()) - 1);
							if (singleSlashFormat.size() > 2 && !singleSlashFormat[2].empty())
							{
								face.m_vertNormalIndexes.push_back(atoi(singleSlashFormat[2].c_str()) - 1);
							}
						}
					}
					else
					{
						face.m_vertPositionIndexes.push_back(atoi(doubleSlashFormat[0].c_str()) - 1);
						if (!doubleSlashFormat[1].empty())
						{
							face.m_vertNormalIndexes.push_back(atoi(doubleSlashFormat[1].c_str()) - 1);
						}

					}

					face.m_color = mtllibList[currentMaterial];
				}

				faceList.push_back(face);
				break;
			}


		}
	}

	std::vector<Triangle> triangles;

	for (Face& face : faceList)
	{
		int posIndexes[3];
		int texIndexes[3];
		int normIndexes[3];

		posIndexes[0] = face.m_vertPositionIndexes[0];
		texIndexes[0] = face.m_vertTextureIndexes.size() > 0 ? face.m_vertTextureIndexes[0] : -1;
		normIndexes[0] = face.m_vertNormalIndexes.size() > 0 ? face.m_vertNormalIndexes[0] : -1;

		for (size_t i = 2; i < face.m_vertPositionIndexes.size(); i++)
		{
			posIndexes[1] = face.m_vertPositionIndexes[i - 1];
			posIndexes[2] = face.m_vertPositionIndexes[i];

			texIndexes[1] = face.m_vertTextureIndexes.size() > i - 1 ? face.m_vertTextureIndexes[i - 1] : -1;
			texIndexes[2] = face.m_vertTextureIndexes.size() > i ? face.m_vertTextureIndexes[i] : -1;

			normIndexes[1] = face.m_vertNormalIndexes.size() > i - 1 ? face.m_vertNormalIndexes[i - 1] : -1;
			normIndexes[2] = face.m_vertNormalIndexes.size() > i ? face.m_vertNormalIndexes[i] : -1;

			Triangle t(posIndexes[0], posIndexes[1], posIndexes[2],
				texIndexes[0], texIndexes[1], texIndexes[2],
				normIndexes[0], normIndexes[1], normIndexes[2],
				face.m_color);
			triangles.push_back(t);
		}
	}

	outVertexes.reserve((int)(triangles.size() * 3));
	outIndexes.reserve((int)(triangles.size() * 3));
	std::unordered_map<Vertex, int, VertexHash, VertexEqual> vertexMap;

	for (Triangle & tri : triangles)
	{
		for (int i = 0; i < 3; ++i)
		{
			const Vec3& p = pList[tri.m_vertexPositionIndex[i]];
			const Vec2& t = (tri.m_vertexTextureCoordinateIndex[i] == -1) ? Vec2::ZERO : tList[tri.m_vertexTextureCoordinateIndex[i]];
			const Vec3& n = (tri.m_vertexNormalIndex[i] == -1) ? Vec3::ZERO : nList[tri.m_vertexNormalIndex[i]];
			const Rgba8& c = tri.m_color;

			Vertex_PCUTBN gv(p.x, p.y, p.z, c.r, c.g, c.b, c.a, t.x, t.y, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, n.x, n.y, n.z);
			Vertex v{ tri.m_vertexPositionIndex[i], tri.m_vertexTextureCoordinateIndex[i], tri.m_vertexNormalIndex[i] };

			auto it = vertexMap.find(v);
			if (it == vertexMap.end())
			{
				int index = static_cast<int>(outVertexes.size());
				outVertexes.push_back(gv);
				vertexMap[v] = index;
				outIndexes.push_back(index);
			}
			else
			{
				outIndexes.push_back(it->second);
			}
		}
	}

	DebuggerPrintf("\n---------------------------------------\n");
	DebuggerPrintf("OBJ name: %s \n", fileName.c_str());
	DebuggerPrintf("Positions: %i \n", pList.size());
	DebuggerPrintf("UVs: %i \n", tList.size());
	DebuggerPrintf("Normals: %i \n", nList.size());
	DebuggerPrintf("Faces: %i \n", faceList.size());
	DebuggerPrintf("Triangles: %i \n", triangles.size());
	DebuggerPrintf("Vertexes: %i \n", outVertexes.size());
	DebuggerPrintf("Indexes: %i \n", outIndexes.size());
	DebuggerPrintf("\n---------------------------------------\n");

	return true;
}
