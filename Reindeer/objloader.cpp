#include "objloader.h"
#include "reindeer.h"

bool loadOBJ(
	const char * path,
	std::vector<RDRVEC3> & out_vertices,
	std::vector<RDRVEC2> & out_uvs,
	std::vector<RDRVEC3> & out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<int> vertexIndices, uvIndices, normalIndices;
	std::vector<RDRVEC3> temp_vertices;
	std::vector<RDRVEC2> temp_uvs;
	std::vector<RDRVEC3> temp_normals;


	FILE * file = fopen(path, "r");
	if ( file == NULL ) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while ( 1 ) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if ( strcmp( lineHeader, "v" ) == 0 ){
			RDRVEC3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			RDRVEC2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			RDRVEC3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
			//std::string vertex1, vertex2, vertex3;
			int vertexIndex[3] = {-1, -1, -1},
				uvIndex[3] = {-1, -1, -1},
				normalIndex[3] = {-1, -1, -1};
			size_t len = 0;
			char buff[50];
			buff[0] = '\0';
			int in, lastIn = ' ', type = 0, index = 0;
			while (1)
			{
				in = fgetc(file);
				if (!isalnum(in))
				{
					if (len > 0)
					{
						switch (type)
						{
						case 0:
							sscanf(buff, "%d", &vertexIndex[index]);
							break;
						case 1:
							sscanf(buff, "%d", &uvIndex[index]);
							break;
						case 2:
							sscanf(buff, "%d", &normalIndex[index]);
							break;
						}
					}

					if (in == EOF)
						break;
					if (in == '\n')
						break;

					if (in == '/')
						type = (type + 1) % 3;
					else if (lastIn != ' ')
					{
						index = (index + 1) % 3;
						type = 0;
					}
					len = 0;
					buff[0] = '\0';
				}
				else
				{
					char inStr[] = {(char)in, '\0'};
					strcat(buff, inStr);
					len++;
				}
				lastIn = in;
			}

			//int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			//if (matches != 9){
			//        printf("File can't be read by our simple parser :-( Try exporting with other options\n");
			//        return false;
			//}

			if (vertexIndex[0] != -1)
			{
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
			}
			if (uvIndex[0] != -1)
			{
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
			}
			if (normalIndex[0] != -1)
			{
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		} else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i=0; i<vertexIndices.size(); i++) {
	
		// Get the indices of its attributes
		int vertexIndex = vertexIndices[i];
	
		// Get the attributes thanks to the index
		RDRVEC3 vertex = temp_vertices[ vertexIndex-1 ];
	
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		int uvIndex = uvIndices[i];
		RDRVEC2 uv = temp_uvs[ uvIndex-1 ];
		out_uvs     .push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		int normalIndex = normalIndices[i];
        RDRVEC3 normal = temp_normals[ normalIndex-1 ];
        out_normals .push_back(normal);
	}

	return true;
}
