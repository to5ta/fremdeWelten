#include <string>


class Material{

	/// constructor, tell MaterialManager?
	Material();

	/// deconstructor, notify users
	~Material();

private:
	std::string name;
	int users;



};