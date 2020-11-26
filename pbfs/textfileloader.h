#ifndef TEXTFILELOADER_H
#define TEXTFILELOADER_H

#include "graph.h"
#include "graphbuilder.h"

#include <string>

class TextFile;

template <>
class GraphBuilder<TextFile>
{
public:
	Graph create(const std::string &fileName);
};

#endif	// TEXTFILELOADER_H