#ifndef PARSER_HPP
# define PARSER_HPP

# include <Webserv.hpp>

/*
 *	IAction: interface for all parsing actions
 *	~IAction: virtual destructor
 *	clone: pure virtual method to create a deep copy of the action
 *	parse: pure virtual method to execute the specific parsing logic for a directive
 * */
class IAction {
	public:
		virtual ~IAction() {}
		virtual IAction* clone() const = 0;
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) = 0;
};

/*
 *	name: string representing the exact name of the directive (e.g., "listen", "root")
 *	action: pointer to the specific IAction implementation that handles this directive
 * */
struct ActionNode {
	std::string name;
	IAction* action;
};

/*
 *	_actions: vector storing the registered parsing rules and templates for each directive
 *
 *	Parser: default constructor
 *	Parser (copy): copy constructor
 *	~Parser: virtual class destructor
 *	operator=: assignment operator overload
 *
 *	registerAction: associates a text directive name with its corresponding parsing action
 *	clearActions: frees the dynamic memory of all registered actions in the vector
 *	initMap: pure virtual method for each child class to initialize its specific actions
 *	parseBlock: reads and processes a configuration block by iterating over tokens until the closing brace is found
 * */
class Parser {
	protected:
		std::vector<ActionNode> _actions;

		void registerAction(const std::string& directive, IAction* action);
		void clearActions();

	public:
		Parser();
		virtual ~Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &src);

		virtual void initMap() = 0;
		virtual bool parseBlock(size_t &pos, const fileVector &file);
};

/*
 *	_member: pointer to the class member variable where the parsed value will be stored
 *	_name: string representing the directive name for error logging
 *
 *	SimpleAction: constructor initializing the member pointer and directive name
 *	clone: creates a deep copy of the SimpleAction instance
 *	parse: extracts a single value from the configuration file and stores it in the target member
 * */
template <class C, typename T>
class SimpleAction : public IAction {
	private:
		T C::*_member;
		std::string _name;
	public:
		SimpleAction(T C::*member, const std::string& name) : _member(member), _name(name) {}
		virtual IAction* clone() const { return new SimpleAction<C, T>(_member, _name); }
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			C* instance = static_cast<C*>(obj);
			pos++;
			if (pos >= file.size() || file[pos] == ";") {
				print_msg(_name + ": missing value", ERR);
				return false;
			}
			std::stringstream ss(file[pos]);
			if (!(ss >> (instance->*_member)) || !ss.eof()) {
				print_msg(_name + ": invalid value format", ERR);
				return false;
			}
			pos++;
			if (pos >= file.size() || file[pos] != ";") {
				print_msg(_name + ": expected semicolon", ERR);
				return false;
			}
			return true;
		}
};

/*
 *	_member: pointer to the class boolean member variable
 *	_name: string representing the directive name for error logging
 *
 *	BoolAction: constructor initializing the member pointer and directive name
 *	clone: creates a deep copy of the BoolAction instance
 *	parse: evaluates "on" or "off" tokens and stores the corresponding boolean in the target member
 * */
template <class C>
class BoolAction : public IAction {
	private:
		bool C::*_member;
		std::string _name;
	public:
		BoolAction(bool C::*member, const std::string& name) : _member(member), _name(name) {}
		virtual IAction* clone() const { return new BoolAction<C>(_member, _name); }
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			C* instance = static_cast<C*>(obj);
			pos++;
			if (pos >= file.size() || file[pos] == ";") {
				print_msg(_name + ": missing boolean value", ERR);
				return false;
			}
			if (file[pos] == "on")       (instance->*_member) = true;
			else if (file[pos] == "off") (instance->*_member) = false;
			else {
				print_msg(_name + ": must be 'on' or 'off'", ERR);
				return false;
			}
			pos++;
			if (pos >= file.size() || file[pos] != ";") return false;
			return true;
		}
};

/*
 *	_member: pointer to the class vector member variable
 *	_name: string representing the directive name for error logging
 *
 *	VectorAction: constructor initializing the member pointer and directive name
 *	clone: creates a deep copy of the VectorAction instance
 *	parse: extracts multiple values until a semicolon is found and pushes them into the target vector
 * */
template <class C, typename V>
class VectorAction : public IAction {
	private:
		std::vector<V> C::*_member;
		std::string _name;
	public:
		VectorAction(std::vector<V> C::*member, const std::string& name) : _member(member), _name(name) {}
		virtual IAction* clone() const { return new VectorAction<C, V>(_member, _name); }
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			C* instance = static_cast<C*>(obj);
			pos++;
			if (pos >= file.size() || file[pos] == ";") {
				print_msg(_name + ": missing values", ERR);
				return false;
			}
			while (pos < file.size() && file[pos] != ";") {
				V val;
				std::stringstream ss(file[pos]);
				if (!(ss >> val) || !ss.eof()) return false;
				(instance->*_member).push_back(val);
				pos++;
			}
			if (pos >= file.size() || file[pos] != ";") return false;
			return true;
		}
};

/*
 *	_member: pointer to the class string member variable
 *	_name: string representing the directive name for error logging
 *
 *	StringConcatAction: constructor initializing the member pointer and directive name
 *	clone: creates a deep copy of the StringConcatAction instance
 *	parse: extracts multiple string tokens and concatenates them with spaces into a single string
 * */
template <class C, typename K, typename V>
class MapAction : public IAction {
	private:
		std::map<K, V> C::*_member;
		std::string _name;
	public:
		MapAction(std::map<K, V> C::*member, const std::string& name) : _member(member), _name(name) {}
		virtual IAction* clone() const { return new MapAction<C, K, V>(_member, _name); }
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			C* instance = static_cast<C*>(obj);
			pos++;
			if (pos + 1 >= file.size() || file[pos] == ";") {
				print_msg(_name + ": missing key-value pair", ERR);
				return false;
			}
			K key; V val;
			std::stringstream sk(file[pos]);
			std::stringstream sv(file[pos + 1]);
			if (!(sk >> key) || !sk.eof() || !(sv >> val) || !sv.eof()) {
				print_msg(_name + ": invalid key or value format", ERR);
				return false;
			}
			(instance->*_member)[key] = val;
			pos += 2;
			if (pos >= file.size() || file[pos] != ";") {
				print_msg(_name + ": expected semicolon", ERR);
				return false;
			}
			return true;
		}
};

template <class C>
class StringConcatAction : public IAction {
	private:
		std::string C::*_member;
		std::string _name;
	public:
		StringConcatAction(std::string C::*member, const std::string& name) : _member(member), _name(name) {}
		virtual IAction* clone() const { return new StringConcatAction<C>(_member, _name); }
		virtual bool parse(void* obj, size_t &pos, const fileVector &file) {
			C* instance = static_cast<C*>(obj);
			pos++;
			if (pos >= file.size() || file[pos] == ";") {
				print_msg(_name + ": missing values", ERR);
				return false;
			}
			std::string res = "";
			while (pos < file.size() && file[pos] != ";") {
				if (!res.empty()) res += " ";
				res += file[pos];
				pos++;
			}
			(instance->*_member) = res;
			return true;
		}
};

#endif
