#include <iostream>
#include <memory>

class Entity
{
public:

	Entity()
	{
		std::cout << "Entity Created\n";
	}

	~Entity()
	{
		std::cout << "Entity Destroyed\n";
	}

};

int main()
{
	//Unique pointer : cannot be shared, destroyed when gone out of scope
	std::cout << "\nScope_1\n";
	{
		std::cout << "Begin\n";
		std::unique_ptr<Entity> uniqueEntity = std::make_unique<Entity>();
	}
	std::cout << "End Scope_1\n";

	//Shared pointer : can be shared, destroyed when last sharing copy goes out of scope
	//Weak pointer : takes value from a shared pointer, does not increase ref count of shared pointer
	std::cout << "\nScope_2\n";
	{
		std::cout << "Begin\n";
		std::weak_ptr<Entity> weakEntityCopy;
		std::cout << "Scope_2_1\n";
		{
			std::shared_ptr<Entity> sharedEntityCopy;
			std::cout << "Scope_2_1_1\n";
			{
				std::cout << "Begin\n";
				std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
				sharedEntityCopy = sharedEntity;
			}
			std::cout << "End Scope_2_1_1\n";
		}
		std::cout << "End Scope_2_1\n";
	}
	std::cout << "End Scope_2\n";

	std::cin.get();
	return 0;
}