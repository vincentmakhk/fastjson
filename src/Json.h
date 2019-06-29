#pragma once

#include <vector>

const char* get_node_end(const char* p, const char* end);

class FastJson;

class FastJsonNode
{
public:
	enum NodeType { Empty, Null, Node, String, Bool, Numeric, Array } node_type;
	const char* tag_start;
	int tag_len;
	//int start, end;
	const char* data_start;
	int data_len;

public:
	FastJsonNode() : node_type(Empty), tag_start(NULL), tag_len(0), data_start(NULL), data_len(0), child(NULL) { }
	FastJsonNode(NodeType n, const char* ts, int tl, const char* ds, int dl) : node_type(n), tag_start(ts), tag_len(tl), data_start(ds), data_len(dl), child(NULL) { }
	~FastJsonNode();
	FastJsonNode& get_child(const char* node_name);

public:
	static FastJsonNode empty_node;

private:
	FastJson* child;
};


class FastJson
{
private:
	std::vector<FastJsonNode> nodes;

public:
	FastJson(const char* ch, int sz);
	~FastJson();
	FastJsonNode& get_child(const char* node_name);
};

