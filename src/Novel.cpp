#include "Novel.h"
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <oslib/oslib.h>

using namespace VNPSP;

Novel::Novel(std::string dir) :
	m_dir(dir), m_thumbnail(0)
{
	this->readInfo();
}

Novel::~Novel()
{
	
}

OSL_IMAGE* Novel::thumbnail()
{
	if(!m_thumbnail)
		this->loadThumbnail();
	return m_thumbnail;
}

void Novel::loadThumbnail()
{
	if(!m_thumbnail)
	{
		std::string str = m_dir + std::string("/thumbnail.png");
		std::vector<char> path(str.size() + 1);
		std::copy(str.begin(), str.end(), path.begin());
		m_thumbnail = oslLoadImageFilePNG(&path[0], OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	}
}

void Novel::unloadThumbnail()
{
	if(m_thumbnail)
	{
		oslDeleteImage(m_thumbnail);
		m_thumbnail = 0;
	}
}

std::string Novel::pathForResource(std::string section, std::string filename)
{
	return m_dir + "/" + section + "/" + filename;
}

void Novel::readInfo()
{
	std::ifstream file((m_dir + "/info.txt").c_str());
	std::string line;
	while(std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string key, value;
		std::getline(ss, key, '=');
		std::getline(ss, value, '\n');
		info[key] = value;
	}
}
