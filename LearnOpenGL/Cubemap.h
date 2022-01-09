#pragma once

#include <string>
#include <vector>

class Cubemap
{
public:
	Cubemap(std::vector<std::string> faces);

	unsigned int getId() { return id; }

protected:
	unsigned int id;

private:
	void loadTextures(std::vector<std::string> faces);
};

