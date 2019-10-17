#ifndef  BVH_H
#define  BVH_H

#include <vector>
#include <stack>
#include <string.h>
#include <fstream>
#include "../freeglut/include/GL/glut.h"


struct Offset 
{
	float x, y, z;
};

enum Channels
{
	XPOSITION, YPOSITION, ZPOSITION,
	XROTATION, YROTATION, ZROTATION
};

struct Joint 
{
	std::string name;
	Joint *parent;
	Offset offset;
	bool end;
	unsigned int numChannels;
	unsigned int offsetChannel;
	std::vector<Channels> channelEnums;
	std::vector<Joint*> children;

};

struct Motion
{
	unsigned int numFrames;
	float frameTime;
	float **data;
};

class BVH {

public: 
	BVH(wchar_t *filename);
	~BVH();

	bool isLoaded();
	float getFrameTime();
	bool load(wchar_t *filename);

	std::vector<std::string> splitString(std::string &str, char delimiter);
	bool loadHierarchy(std::ifstream *stream, Joint *parent, Joint *out);
	bool loadMotion(std::ifstream *stream, Motion *out); 

	void clearHierarchy(Joint *joint);
	void clear();

	void Render(unsigned int frameNo);
	void RenderFigure(Joint* joint, float* data);
	void RenderLine(float x, float y, float z); 

private: 
	std::ifstream file;
	std::vector<std::string> vec;

	Joint* root;
	Motion* motion;

	bool loaded;

};


#endif