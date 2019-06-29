#include "Json.h"

const char* get_node_end(const char* p, const char* end)
{
	if(*p == '[' || *p == '{')
	{
		int count = 1;
		++p;
		while(p < end && count > 0)
		{
			if(*p == '[' || *p == '{')
			{
				++count;
			}
			else if(*p == ']' || *p == '}')
			{
				--count;
			}
			++p;
		}
	}
	else // not array, not nodes
	{
		while(p < end)
		{
			if(*p == ',')
				break;
			++p;
		}
	}
	return p;
}


FastJsonNode FastJsonNode::empty_node;

FastJsonNode::~FastJsonNode() { delete child; }

FastJsonNode& FastJsonNode::get_child(const char* node_name)
{
	if( FastJsonNode::Node == node_type )
	{
		if(NULL == child)
			child = new FastJson(data_start, data_len);

		return child->get_child(node_name);
	}
	else
		return empty_node;
}


FastJson::FastJson(const char* ch, int sz)
{
	nodes.reserve(40);

	// suppose starts with { end with }
	int i = 1;
	while(i < sz && ch[i] != '}')
	{
		// first char is "
		++i;
		int start_tag = i;
		const char* ch_start_tag = ch+i;
		while(i<sz && ch[i] != '"')
			++i;
		int end_tag = i;

		++i; // :

		++i; // start of data

		const char* end = get_node_end(ch+i, ch+sz);

		if( ch[i] == '[' ) // array
		{
			FastJsonNode f( FastJsonNode::Array, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] == '{' )
		{
			FastJsonNode f( FastJsonNode::Node, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] == 'n' )
		{
			FastJsonNode f( FastJsonNode::Null, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] == 't' || ch[i] == 'f')
		{
			FastJsonNode f( FastJsonNode::Bool, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] == '"' )
		{
			FastJsonNode f( FastJsonNode::String, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] == '-' && (ch[i+1] >= '0' && ch[i+1] <= '9') )
		{
			FastJsonNode f( FastJsonNode::Numeric, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else if(ch[i] >= '0' && ch[i] <= '9' )
		{
			FastJsonNode f( FastJsonNode::Numeric, ch+start_tag, end_tag-start_tag, ch+i, end-(ch+i) );
			nodes.push_back(f);
		}
		else
		{
			throw std::exception("invalid node");
		}

		// end at , move i to next
		i = (end-ch)+1;
	}
}

FastJson::~FastJson(void)
{
}

FastJsonNode& FastJson::get_child(const char* node_name)
{
	int sz = nodes.size();
	for(int i=0; i<sz; ++i)
	{
		if( 0 == strncmp(nodes[i].tag_start, node_name, nodes[i].tag_len) )
			return nodes[i];
	}
	return FastJsonNode::empty_node;
}