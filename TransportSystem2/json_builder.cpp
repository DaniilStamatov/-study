#include "json_builder.h"

namespace json {

	Builder::Builder()
	{
		Node* root_ptr = &root_;
		nodes_stack_.emplace_back(root_ptr);
	}

	DictKeyContext Builder::Key(std::string str)
	{
		if (nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
			throw std::logic_error("Key can be placed only in dictionary");
		}
		if (!key_) {
			key_ = std::move(str);
		}

		return *this;
	}

	Builder& Builder::Value(Node::Value value)
	{
		auto* top_node = nodes_stack_.back();

		if (top_node->IsDict()) {
			if (!key_) throw std::logic_error("Key can be placed only in dictionary");
			auto& dict = std::get<Dict>(top_node->GetValue());
			auto [pos, _] = dict.emplace(std::move(key_.value()), Node{});
			key_ = std::nullopt;
			top_node = &pos->second;
			top_node->GetValue() = std::move(value);
		}
		else if (top_node->IsArray()) {
			auto& arr = std::get<Array>(top_node->GetValue());
			arr.push_back(GetNode(value));
			top_node = &arr.back();
		}
		else if (top_node->IsNull()) {
			root_.GetValue() = std::move(value);
		}
		else throw std::logic_error("Value() called in unknow container");

		return *this;
	}

	Node Builder::GetNode(Node::Value value) {
		if (std::holds_alternative<int>(value)) return Node(std::get<int>(value));
		if (std::holds_alternative<double>(value)) return Node(std::get<double>(value));
		if (std::holds_alternative<std::string>(value)) return Node(std::get<std::string>(value));
		if (std::holds_alternative<std::nullptr_t>(value)) return Node(std::get<std::nullptr_t>(value));
		if (std::holds_alternative<bool>(value)) return Node(std::get<bool>(value));
		if (std::holds_alternative<Dict>(value)) return Node(std::get<Dict>(value));
		if (std::holds_alternative<Array>(value)) return Node(std::get<Array>(value));
		return {};
	}


	DictItemContext Builder::StartDict()
	{
		auto* top_node = nodes_stack_.back();

		if (top_node->IsDict()) {
			if (!key_) throw std::logic_error("couldnt create dict without key");
			auto& dict = std::get<Dict>(top_node->GetValue());
			auto [pos, _] = dict.emplace(key_.value(), Dict());
			key_ = std::nullopt;
			nodes_stack_.emplace_back(&pos->second);
		}
		else if (top_node->IsArray()) {
			auto& arr = std::get<Array>(top_node->GetValue());
			arr.emplace_back(Dict());
			nodes_stack_.emplace_back(&arr.back());
		}
		else if (top_node->IsNull()) {
			top_node->GetValue() = Dict();
		}
		else throw std::logic_error("Wrong prev node");

		return *this;
	}

	ArrayItemContext Builder::StartArray()
	{
		auto* top_node = nodes_stack_.back();

		if (top_node->IsDict()) {
			if (!key_) throw std::logic_error("couldnt create dict without key");
			auto& dict = std::get<Dict>(top_node->GetValue());
			auto [pos, _] = dict.emplace(key_.value(), Array());
			key_ = std::nullopt;
			nodes_stack_.emplace_back(&pos->second);
		}
		else if (top_node->IsArray()) {
			auto& arr = std::get<Array>(top_node->GetValue());
			arr.emplace_back(Array());
			nodes_stack_.emplace_back(&arr.back());
		}
		else if (top_node->IsNull()) {
			top_node->GetValue() = Array();
		}
		else throw std::logic_error("Wrong prev node");

		return *this;
	}

	Builder& Builder::EndDict()
	{
		auto* top_node = nodes_stack_.back();

		if (!top_node->IsDict()) throw std::logic_error("Prev node is not a Dict");
		nodes_stack_.pop_back();

		return *this;
	}

	Builder& Builder::EndArray()
	{
		auto* top_node = nodes_stack_.back();

		if (!top_node->IsArray()) throw std::logic_error("Prev node is not a Array");
		nodes_stack_.pop_back();

		return *this;
	}

	Node Builder::Build()
	{
		if (root_.IsNull() || nodes_stack_.size() > 1) throw std::logic_error("Wrong Build()");
		return root_;
	}
	DictItemContext::DictItemContext(Builder& builder) : builder_(builder)
	{
	}

	DictKeyContext DictItemContext::Key(std::string str)
	{
		return builder_.Key(str);
	}

	Builder& DictItemContext::EndDict()
	{
		return builder_.EndDict();
	}

	DictKeyContext::DictKeyContext(Builder& builder) :builder_(builder)
	{
	}

	DictItemContext DictKeyContext::Value(Node::Value value)
	{
		return builder_.Value(value);
	}

	ArrayItemContext DictKeyContext::StartArray()
	{
		return builder_.StartArray();
	}

	DictItemContext DictKeyContext::StartDict()
	{
		return builder_.StartDict();
	}

	ArrayItemContext::ArrayItemContext(Builder& builder) : builder_(builder)
	{
	}
	ArrayItemContext ArrayItemContext::Value(Node::Value value)
	{
		return builder_.Value(value);
	}
	DictItemContext ArrayItemContext::StartDict()
	{
		return builder_.StartDict();
	}
	Builder& ArrayItemContext::EndArray()
	{
		return builder_.EndArray();
	}
	ArrayItemContext ArrayItemContext::StartArray()
	{
		return builder_.StartArray();
	}
}