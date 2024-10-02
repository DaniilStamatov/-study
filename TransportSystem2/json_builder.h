#pragma once
#include "json.h"
#include <optional>
namespace json {

	class DictItemContext;
	class DictKeyContext;
	class ArrayItemContext;

	class Builder {
	private:
		Node root_{ nullptr };
		std::vector<Node*> nodes_stack_;
		std::optional<std::string> key_{ std::nullopt };
	public:
		Builder();
		DictKeyContext Key(std::string str);
		Builder& Value(Node::Value value);
		DictItemContext StartDict();
		ArrayItemContext StartArray();
		Builder& EndDict();
		Builder& EndArray();
		Node Build();
		Node GetNode(Node::Value  value);
	};

	class DictItemContext {
	private:
		Builder& builder_;
	public:
		DictItemContext(Builder& builder);

		DictKeyContext Key(std::string str);
		Builder& EndDict();
	};

	class ArrayItemContext {
	public:
		ArrayItemContext(Builder& builder);
		ArrayItemContext Value(Node::Value value);
		DictItemContext StartDict();
		Builder& EndArray();
		ArrayItemContext StartArray();
	private:
		Builder& builder_;
	};

	class DictKeyContext {
	private:
		Builder& builder_;
	public:
		DictKeyContext(Builder& builder);

		DictItemContext Value(Node::Value value);
		ArrayItemContext StartArray();
		DictItemContext StartDict();
	};
}