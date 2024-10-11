#pragma once
#include <stdexcept>
#include <vector>

typedef unsigned long long Id;

struct Vector2 {
	float x, y = 0;
	Vector2(float newX, float newY) {
		x = newX;
		y = newY;
	}

	Vector2() {
		x = 0;
		y = 0;
	};
};

struct Vector3 {
	float x, y, z = 0;
	Vector3(float newX, float newY, float newZ) {
		x = newX;
		y = newY;
		z = newZ;
	}
	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	};
};

struct Quaternion {
	float a, i, j, k = 0;

	Quaternion(float newA, float newI, float newJ, float newK) {
		a = newA;
		i = newI;
		j = newJ;
		k = newK;
	}
	Quaternion() {
		a = 0;
		i = 0;
		j = 0;
		k = 0;
	}
};

struct Color {
	float r, g, b, a;
};

class Tickable {
	virtual void tick(const double delta) = 0;
};

class GameObject : public Tickable {
private:
	static Id generateID() {
		static int idCounter = 0;
		idCounter++;
		return idCounter;
	}
public:
	Id id;
	GameObject() {
		id = generateID();
	}
};

class Geometry {
public:
	size_t verticesAmount() {
		return _verticesAmount;
	}

	size_t trianglesAmount() {
		return _trianglesAmount;
	}


	void loadVertices(void* data, int& offset, size_t size) {
		_vertices = new Vector3[size];
		for (int i = 0; i < size; i++) {
			_vertices[i].x = ((float*)data)[offset];
			_vertices[i].y = ((float*)data)[offset+1];
			_vertices[i].z = ((float*)data)[offset+2];
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
	}

	void loadTriangles(void* data, int& offset, size_t size) {
		_triangles = new Vector3[size];
		for (int i = 0; i < size; i++) {
			_triangles[i].x = ((float*)data)[offset];
			_triangles[i].y = ((float*)data)[offset + 1];
			_triangles[i].z = ((float*)data)[offset + 2];
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
	}

	Vector3* getVertices() {
		return _vertices;
	}

	Vector3* getTriangles() {
		return _triangles;
	}

	void loadVertices(Vector3* vertices, unsigned int size) {
		_vertices = vertices;
		_verticesAmount = size;
		_verticesInicialized = true;
	}

	void loadTriangles(Vector3* triangles, unsigned int size) {
		bool error = false;
		for (unsigned int i = 0; i < size; i++) {
			if (triangles[i].x > _verticesAmount) {
				error = true;
				throw std::runtime_error("triangle array referencing to point that doesnt exist");
			}
			if (triangles[i].y > _verticesAmount) {
				error = true;
				throw std::runtime_error("triangle array referencing to point that doesnt exist");
			}
			if (triangles[i].z > _verticesAmount) {
				error = true;
				throw std::runtime_error("triangle array referencing to point that doesnt exist");
			}
		}
		if (!error) {
			_triangles = triangles;
			_trianglesAmount = size;
			_trianglesInicialized = true;
		}
	}

protected:
	Vector3* _vertices;
	Vector3* _triangles;

	unsigned int _verticesAmount;
	unsigned int _trianglesAmount;
	bool _verticesInicialized;
	bool _trianglesInicialized;
};

class Mesh : public Geometry{
protected:
	Vector3* _normals;
	Vector2* _UVs;

public:
	Vector3* getNormals() {
		return _normals;
	}
	
	Vector2* getUVs() {
		return _UVs;
	}

	size_t getSize() {
		size_t s = 0;
		//printf("vertices amount:%d\n", _verticesAmount);
		//printf("sizeof vector3: %d, sizeof uint32: %d\n", sizeof(Vector3), sizeof(uint32_t));
		s += _verticesAmount * sizeof(Vector3) / sizeof(uint32_t)*2;
		//printf("s after first operation:%d\n", s);
		s += _verticesAmount * sizeof(Vector2) / sizeof(uint32_t);
		//printf("s after second operation:%d\n", s);
		s += _trianglesAmount * sizeof(Vector3) / sizeof(uint32_t);
		//printf("s after third operation:%d\n", s);
		return  s;
	}

	void loadNormals(void* data, int& offset, size_t size) {
		_normals = new Vector3[size];
		for (int i = 0; i < size; i++) {
			_normals[i].x = ((float*)data)[offset];
			_normals[i].y = ((float*)data)[offset + 1];
			_normals[i].z = ((float*)data)[offset + 2];
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
	}

	void loadUVs(void* data, int& offset, size_t size) {
		_UVs = new Vector2[size];
		for (int i = 0; i < size; i++) {
			_UVs[i].x = ((float*)data)[offset];
			_UVs[i].y = ((float*)data)[offset + 1];
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
	}

	void loadNormals(Vector3* normals, unsigned int size) {
		if (size != _verticesAmount) {
			throw std::invalid_argument("size of the normals array dont match the size of the vertices array");
		}
		else {
			_normals = normals;
		}
	}

	void loadUVs(Vector2* UVs, unsigned int size) {
		if (size != _verticesAmount) {
			throw std::invalid_argument("size of the UVs array dont match the size of the vertices array");
		}
		else {
			_UVs = UVs;
		}
	}
};

class Collider : public Geometry {
	Vector3 centerOfMass;
public:
	size_t getSize() {
		size_t s = 0;
		s += _verticesAmount * sizeof(Vector3) / sizeof(uint32_t);
		s += _trianglesAmount * sizeof(Vector3) / sizeof(uint32_t);
		return  s;
	}
};

class PhysicalObject : public GameObject {
private:
	Mesh m;
	Collider c;
	Vector3 meshPivot;
	Quaternion rotation;//TODO fix 

public:
	void tick(const double delta) {

	}

	size_t getSize() {
		size_t s = 0;
		s += m.getSize();
		//printf("got mesh size: %d\n",s);
		s += c.getSize();
		//printf("got collider size: %d\n", s);
		s += sizeof(Vector3) / sizeof(uint32_t);
		s += sizeof(Quaternion) / sizeof(uint32_t);
		s += 6;
		return s;
	}

	size_t getMeshSize() {
		return m.getSize();
	}

	size_t getColliderSize() {
		return c.getSize();
	}

	void setMesh(const Mesh& newMesh) {
		m = newMesh;
	}

	Mesh& getMesh() {
		return m;
	}

	void setCollider(const Collider& newCollider) {
		c = newCollider;
	}

	Collider& getCollider() {
		return c;
	}

	void setMeshPivot(const Vector3& newPivot) {
		meshPivot.x = newPivot.x;
		meshPivot.y = newPivot.y;
		meshPivot.z = newPivot.z;
	}

	Vector3 getMeshPivot() {
		return meshPivot;
	}

	void setRotation(const Quaternion& newRotation) {
		rotation.a = newRotation.a;
		rotation.i = newRotation.i;
		rotation.j = newRotation.j;
		rotation.k = newRotation.k;
	}

	Quaternion getRotation() {
		return rotation;
	}
};

class Scene {
private:
	std::vector<PhysicalObject> objects;

	Mesh loadMesh(void* data, int& offset) {
		int verticesSize = ((uint32_t*)data)[offset];
		offset++;
		int trianglesSize = ((uint32_t*)data)[offset];
		offset++;
		int normalsSize = ((uint32_t*)data)[offset];
		offset++;
		int UVsSize = ((uint32_t*)data)[offset];
		offset++;

		//Vector3* vertices = new Vector3[verticesSize];
		//Vector3* triangles = new Vector3[trianglesSize];
		//Vector3* normals = new Vector3[normalsSize];
		//Vector3* UVs = new Vector3[UVsSize];

		Mesh newMesh;
		newMesh.loadVertices(data,offset,verticesSize);
		newMesh.loadTriangles(data, offset, trianglesSize);
		newMesh.loadNormals(data, offset, normalsSize);
		newMesh.loadUVs(data, offset, UVsSize);

		return newMesh;
	}

	Collider loadCollider(void* data, int& offset) {
		Collider newCollider;
		int verticesSize = ((uint32_t*)data)[offset];
		offset++;
		int trianglesSize = ((uint32_t*)data)[offset];
		offset++;
		newCollider.loadVertices(data, offset, verticesSize);
		newCollider.loadTriangles(data, offset, verticesSize);
		return newCollider;
	}

	PhysicalObject loadObject(void* data, int& offset) {
		PhysicalObject newObject;

		void* meshPivotLocation = (void*)((uint32_t*)data)[offset];
		newObject.setMeshPivot(*(Vector3*)meshPivotLocation);
		offset += sizeof(Vector3) / sizeof(uint32_t);

		void* rotationLocation = (void*)((uint32_t*)data)[offset];
		newObject.setRotation(*(Quaternion*)rotationLocation);
		offset += sizeof(Quaternion) / sizeof(uint32_t);

		newObject.setMesh(loadMesh(data, offset));
		newObject.setCollider(loadCollider(data, offset));

		return newObject;
	}

	void writeMesh(void* data, int& offset, Mesh i) {
		uint32_t verticesAmount = i.verticesAmount();
		uint32_t trianglesAmount = i.trianglesAmount();
		((uint32_t*)data)[offset] = verticesAmount;
		offset++;
		((uint32_t*)data)[offset] = trianglesAmount;
		offset++;
		((uint32_t*)data)[offset] = verticesAmount;
		offset++;
		((uint32_t*)data)[offset] = verticesAmount;
		offset++;

		Vector3* vertices = i.getVertices();
		for (int j = 0; j < verticesAmount;  j++) {
			((float*)data)[offset] = vertices[j].x;
			((float*)data)[offset+1] = vertices[j].y;
			((float*)data)[offset+2] = vertices[j].z;
			offset += sizeof(Vector3)/sizeof(uint32_t);
		}
		Vector3* triangles = i.getTriangles();
		for (int j = 0; j < trianglesAmount; j++) {
			((float*)data)[offset] = triangles[j].x;
			((float*)data)[offset + 1] = triangles[j].y;
			((float*)data)[offset + 2] = triangles[j].z;
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
		Vector3* normals = i.getNormals();
		for (int j = 0; j < verticesAmount; j++) {
			((float*)data)[offset] = normals[j].x;
			((float*)data)[offset + 1] = normals[j].y;
			((float*)data)[offset + 2] = normals[j].z;
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
		Vector2* UVs = i.getUVs();
		for (int j = 0; j < verticesAmount; j++) {
			((float*)data)[offset] = UVs[j].x;
			((float*)data)[offset + 1] = UVs[j].y;
			offset += sizeof(Vector2) / sizeof(uint32_t);
		}
	}

	void writeCollider(void* data, int& offset, Collider i) {
		uint32_t verticesAmount = i.verticesAmount();
		uint32_t trianglesAmount = i.trianglesAmount();
		((uint32_t*)data)[offset] = verticesAmount;
		offset++;
		((uint32_t*)data)[offset] = trianglesAmount;
		offset++;
		Vector3* vertices = i.getVertices();
		for (int j = 0; j < verticesAmount; j++) {
			((float*)data)[offset] = vertices[j].x;
			((float*)data)[offset + 1] = vertices[j].y;
			((float*)data)[offset + 2] = vertices[j].z;
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
		Vector3* triangles = i.getTriangles();
		for (int j = 0; j < trianglesAmount; j++) {
			((float*)data)[offset] = triangles[j].x;
			((float*)data)[offset + 1] = triangles[j].y;
			((float*)data)[offset + 2] = triangles[j].z;
			offset += sizeof(Vector3) / sizeof(uint32_t);
		}
	}

	void writeObject(void* data, int& offset, PhysicalObject i) {
		//printf("new data location after passing: %xlld\n", data);
		//printf("offset: %d", offset);
		Vector3 meshPivot = i.getMeshPivot();
		((Vector3*)data)[offset] = meshPivot;
		offset += sizeof(Vector3) / sizeof(uint32_t);

		Quaternion objectRotation = i.getRotation();
		((Quaternion*)data)[offset] = objectRotation;
		offset += sizeof(Quaternion) / sizeof(uint32_t);

		
		writeMesh(data, offset, i.getMesh());
		//printf("Mesh written\n");
		writeCollider(data, offset, i.getCollider());
		//printf("Collider written\n");
	}


public:
	void addObject(PhysicalObject& objectToAdd) {
		objects.push_back(objectToAdd);
	}

	void physicsTick(double delta) {
		for (int i = 0; i < objects.size(); i++) {
			objects[i].tick(delta);
		}
	}

	void* dropAllData(int& dataSize) {
		//printf("amogus\n");
		size_t bufferSize = 1;
		uint32_t size = objects.size();
		for (int i = 0; i < size; i++) {
			//printf("a\n");
			bufferSize += objects[i].getSize();
		}
		//printf("finished estimating size: %d\n", bufferSize);
		uint32_t* newData = new uint32_t[bufferSize];
		//printf("created object\n");

		int offset = 0;
		((uint32_t*)newData)[0] = size;
		//printf("new data location: %xlld\n", newData);
		offset++;
		for (int i = 0; i < size; i++) {
			//printf("b\n");
			writeObject(newData, offset, objects[i]);
			//printf("c\n");
		}

		dataSize = bufferSize;
		return newData;

	}

	void setAllData(void* data) {
		uint32_t objectsAmount = ((uint32_t*)data)[0];
		void* index = data;
		int offset = 1;

		for (int i = 0; i < objectsAmount; i++) {
			loadObject(index, offset);
		}
	}

	void initAllData(void* data) {
		setAllData(data);
	}
};