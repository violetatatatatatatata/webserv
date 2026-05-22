#include <Parser.hpp>

Parser::Parser() {}

Parser::~Parser() {
	clearActions();
}

Parser::Parser(const Parser &src) {
	*this = src;
}

Parser &Parser::operator=(const Parser &src) {
	if (this != &src) {
		clearActions();
		try {
			for (size_t i = 0; i < src._actions.size(); ++i) {
				ActionNode node;
				node.name = src._actions[i].name;
				node.action = src._actions[i].action->clone();
				_actions.push_back(node);
			}
		} catch (...) {
			clearActions();
			throw;
		}
	}
	return *this;
}

void Parser::clearActions() {
	for (size_t i = 0; i < _actions.size(); ++i) {
		delete _actions[i].action;
	}
	_actions.clear();
}

void Parser::registerAction(const std::string& directive, IAction* action) {
	ActionNode node;
	node.name = directive;
	node.action = action;
	_actions.push_back(node);
}

bool Parser::parseBlock(size_t &pos, const fileVector &file) {
	bool found;

	while (pos < file.size() && file[pos] != "}") {
		found = false;
		for (size_t i = 0; i < _actions.size(); ++i) {
			if (_actions[i].name == file[pos]) {
				found = true;
				if (!_actions[i].action->parse(this, pos, file)) {
					return false;
				}
				pos++;
				break;
			}
		}
		if (!found) {
			print_msg("Unexpected or unsupported directive: " + file[pos], ERR);
			return false;
		}
	}
	if (pos >= file.size() || file[pos] != "}") {
		print_msg("Block not closed properly. Expected '}'", FATAL);
		return false;
	}
	return true;
}
