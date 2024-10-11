#pragma once

#include "engine.h"
#pragma once

#define EXPORT __declspec(dllexport)

extern "C" EXPORT char tester();
extern "C" EXPORT void* getAllData(int& size);
extern "C" EXPORT void sendAllData(void* data);
extern "C" EXPORT void initScene(void* data);
extern "C" EXPORT void tickScene(double delta);

Scene mainScene;

EXPORT char tester() { 
	PhysicalObject obj;
	Collider c;

	Vector3 verticesC[3] = {
		{0,0,0},
		{0,1,0},
		{1,1,0}
	};
	Vector3 trianglesC[1] = {
		{0,1,2}
	};
	c.loadVertices(verticesC,3);
	c.loadTriangles(trianglesC, 1);


	Vector3 verticesM[3]{
		{0,0,0},
		{0,1,0},
		{1,1,0}
	};
	Vector3 trianglesM[1]{
		{0,1,2}
	};
	Vector2 UVsM[3]{
		{0,0},
		{0,1},
		{1,1}
	};
	Vector3 normalsM[3]{
		{0,0,0},
		{0,0,0},
		{0,0,0}
	};
	Mesh m;
	m.loadVertices(verticesM, 3);
	m.loadTriangles(trianglesM, 1);
	m.loadUVs(UVsM, 3);
	m.loadNormals(normalsM, 3);

	obj.setCollider(c);
	obj.setMesh(m);
	obj.setMeshPivot(Vector3(1,2,3));
	obj.setRotation(Quaternion(0, 1, 0, 0));

	mainScene.addObject(obj);


	static int lolz;
	lolz++;
	return lolz;
}

EXPORT void initScene(void* data) {
	mainScene.initAllData(data);
}

EXPORT void* getAllData(int& size) {
	return mainScene.dropAllData(size);
}

EXPORT void sendAllData(void* data) {
	mainScene.setAllData(data);
}

EXPORT void tickScene(double delta) {
	mainScene.physicsTick(delta);
}
