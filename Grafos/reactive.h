#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC

namespace rct {

struct subject;
struct observer;



	struct observer {

	private:
		using name = std::string;
		using action = std::function<void(void*)>;
		using funct_by_name = std::unordered_map<name, action>;

		funct_by_name my_actions;
    

	protected:
		void learn(name _name, action _action) {
			my_actions[_name] = _action;
		}

	public:

		void apply(name _name, void* args = nullptr) {
			auto _action = my_actions.find(_name);
			if (_action != end(my_actions))
				_action->second(args);
		}


	};



	struct subject {
	private:
		using ptr_o = std::shared_ptr<observer>;
		using name = std::string;
		using container = std::unordered_map<name, ptr_o>;

    container collection;
    
		
	public:

    


		void attach(name n, ptr_o obs) {
			collection[n] = obs;
		}

		void trigger(name n, void* args = nullptr) {
			for (auto c : collection) {
				c.second->apply(n, args);
			}
		}

    ptr_o& operator[](name index){
      return collection[index];
    }
	};

}